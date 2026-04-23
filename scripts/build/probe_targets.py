#!/usr/bin/env python3
"""Probe and classify EA Renegade VC6/CMake target modernization risk.

The default mode is static triage from the VC6 manifest and source text. Use
--build to also invoke the current CMake target build for targets that already
exist in the scaffold.
"""

from __future__ import annotations

import argparse
import json
import re
import subprocess
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any

SIGNALS: list[tuple[str, str, re.Pattern[str]]] = [
    ("DirectDraw/Direct3D", "renderer/display", re.compile(r"\b(ddraw|d3d|direct3d|dx8|IDirect3D|IDirectDraw)\b", re.I)),
    ("DirectSound/Miles/audio backend", "audio", re.compile(r"\b(dsound|Miles|mss|AIL_|DirectSound)\b", re.I)),
    ("WinSock/network transport", "platform_sockets", re.compile(r"\b(winsock|SOCKET|WSA|sendto|recvfrom|select\s*\(|closesocket|SOCKADDR)\b", re.I)),
    ("RAD/Bink/media SDK", "media/video", re.compile(r"\b(Bink|RAD Game Tools|RAD\.H|BINK\.H|HBINK)\b", re.I)),
    ("GameSpy/WOL/online service", "online_services", re.compile(r"\b(GameSpy|WOL|WWOnline|WOLAPI|WOLBrowser)\b", re.I)),
    ("MFC/ATL/COM", "product_shell", re.compile(r"\b(afxwin|afxext|atlbase|IUnknown|IDispatch|CoCreate|COM|HRESULT)\b", re.I)),
    ("Win32 window/message loop", "platform_window_shell", re.compile(r"\b(windows\.h|HWND|HINSTANCE|WNDPROC|PeekMessage|DispatchMessage|ShellExecute)\b", re.I)),
    ("Win32 registry/config/version", "platform_registry_config", re.compile(r"\b(RegOpen|RegQuery|HKEY_|GetFileVersionInfo|VerQueryValue)\b", re.I)),
    ("threading/synchronization", "platform_threading", re.compile(r"\b(CreateThread|CRITICAL_SECTION|WaitForSingleObject|HANDLE|Sleep\s*\()\b", re.I)),
    ("string/encoding/WCHAR", "string_encoding", re.compile(r"\b(WCHAR|TCHAR|wcsicmp|wcsnicmp|WideString|MultiByteToWideChar|WideCharToMultiByte)\b", re.I)),
    ("old MSVC/C runtime spelling", "legacy_compiler", re.compile(r"\b(stricmp|strnicmp|strcmpi|_vsnprintf|__cdecl|_cdecl|__int64|iostream\.h|new\.h)\b", re.I)),
]

@dataclass
class Hit:
    category: str
    island: str
    path: str
    count: int

@dataclass
class TargetReport:
    name: str
    path: str
    project_type: str
    source_count: int
    missing_sources: list[str] = field(default_factory=list)
    hits: list[Hit] = field(default_factory=list)
    build_status: str | None = None
    build_first_error: str | None = None


def run(cmd: list[str], cwd: Path) -> subprocess.CompletedProcess[str]:
    return subprocess.run(cmd, cwd=cwd, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)


def find_project(manifest: dict[str, Any], name: str) -> dict[str, Any]:
    needle = name.lower()
    matches = [p for p in manifest.get("projects", []) if p.get("name", "").lower() == needle]
    if not matches:
        matches = [p for p in manifest.get("projects", []) if needle in p.get("name", "").lower() or needle in p.get("path", "").lower()]
    if not matches:
        raise SystemExit(f"target not found in manifest: {name}")
    if len(matches) > 1:
        raise SystemExit("ambiguous target: " + ", ".join(p.get("name", "?") for p in matches))
    return matches[0]


def read_text(path: Path) -> str:
    try:
        return path.read_text(errors="ignore")
    except Exception:
        return ""


def classify(project: dict[str, Any], root: Path, build: bool) -> TargetReport:
    target_dir = root / Path(project["path"]).parent
    report = TargetReport(project["name"], project["path"], project.get("project_type", ""), project.get("source_count", 0))

    counts: dict[tuple[str, str, str], int] = {}
    for src in project.get("sources", []):
        if src == "$(InputPath)":
            continue
        source_path = (target_dir / src).resolve()
        if not source_path.exists():
            report.missing_sources.append(str(source_path.relative_to(root)))
            continue
        if source_path.suffix.lower() not in {".cpp", ".c", ".h", ".hpp", ".inl", ".rc"}:
            continue
        text = read_text(source_path)
        rel = str(source_path.relative_to(root))
        for category, island, pattern in SIGNALS:
            n = len(pattern.findall(text))
            if n:
                counts[(category, island, rel)] = n
    for (category, island, rel), count in sorted(counts.items()):
        report.hits.append(Hit(category, island, rel, count))

    if build:
        cfg = run(["cmake", "-S", ".", "-B", "build/cmake-scaffold", "-DRENEGADE_BUILD_FOUNDATION_LIBS=ON"], root)
        if cfg.returncode != 0:
            report.build_status = "configure-failed"
            report.build_first_error = first_error(cfg.stdout)
        else:
            b = run(["cmake", "--build", "build/cmake-scaffold", "--target", report.name, "-j4"], root)
            report.build_status = "build-ok" if b.returncode == 0 else "build-failed"
            report.build_first_error = first_error(b.stdout) if b.returncode else None
    return report


def first_error(output: str) -> str:
    for line in output.splitlines():
        if "fatal error:" in line or " error:" in line or "No rule to make target" in line or "not found" in line:
            return line.strip()
    return ""


def markdown(reports: list[TargetReport]) -> str:
    lines = ["# Target Probe Report", "", "Generated from `build-manifests/vc6-projects.json` and source-text signal scanning.", ""]
    lines.append("| Target | Type | Sources | Missing | Top blocker categories | Build status |")
    lines.append("|---|---|---:|---:|---|---|")
    for r in reports:
        cat_counts: dict[str, int] = {}
        for h in r.hits:
            cat_counts[h.category] = cat_counts.get(h.category, 0) + h.count
        top = ", ".join(f"{k} ({v})" for k, v in sorted(cat_counts.items(), key=lambda kv: kv[1], reverse=True)[:4]) or "none detected"
        lines.append(f"| `{r.name}` | {r.project_type} | {r.source_count} | {len(r.missing_sources)} | {top} | {r.build_status or 'not built'} |")
    lines.append("")
    for r in reports:
        lines.append(f"## {r.name}")
        lines.append("")
        lines.append(f"- Project: `{r.path}`")
        lines.append(f"- Type: {r.project_type}")
        lines.append(f"- Sources in manifest: {r.source_count}")
        if r.build_status:
            lines.append(f"- Build status: {r.build_status}")
        if r.build_first_error:
            lines.append(f"- First build error: `{r.build_first_error}`")
        if r.missing_sources:
            lines.append("- Missing manifest sources:")
            for m in r.missing_sources[:20]:
                lines.append(f"  - `{m}`")
            if len(r.missing_sources) > 20:
                lines.append(f"  - ... {len(r.missing_sources)-20} more")
        by_cat: dict[str, list[Hit]] = {}
        for h in r.hits:
            by_cat.setdefault(h.category, []).append(h)
        if by_cat:
            lines.append("- Signal hits:")
            for cat, hits in sorted(by_cat.items()):
                total = sum(h.count for h in hits)
                examples = ", ".join(f"`{h.path}`" for h in sorted(hits, key=lambda h: h.count, reverse=True)[:5])
                lines.append(f"  - {cat}: {total} hit(s); examples: {examples}")
        lines.append("")
    return "\n".join(lines)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("targets", nargs="+", help="Target names to triage")
    ap.add_argument("--root", default=".")
    ap.add_argument("--manifest", default="build-manifests/vc6-projects.json")
    ap.add_argument("--build", action="store_true", help="Also attempt CMake target builds")
    ap.add_argument("--output", help="Write Markdown report")
    args = ap.parse_args()

    root = Path(args.root).resolve()
    manifest = json.loads((root / args.manifest).read_text())
    reports = [classify(find_project(manifest, t), root, args.build) for t in args.targets]
    text = markdown(reports)
    if args.output:
        Path(args.output).write_text(text)
        print(args.output)
    else:
        print(text)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
