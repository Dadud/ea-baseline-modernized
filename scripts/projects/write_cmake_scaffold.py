#!/usr/bin/env python3
"""Generate conservative CMake scaffolds from the parsed EA VC6 manifest.

This helper is intentionally descriptive: it preserves original project names and
manifest source membership. It does not decide final OpenW3D architecture.
"""

from __future__ import annotations

import argparse
import json
from pathlib import Path
from typing import Any


def cmake_quote(value: str) -> str:
    return value.replace('"', '\\"')


def find_project(manifest: dict[str, Any], name: str) -> dict[str, Any]:
    needle = name.lower()
    matches = [p for p in manifest.get("projects", []) if p.get("name", "").lower() == needle]
    if not matches:
        matches = [p for p in manifest.get("projects", []) if needle in p.get("name", "").lower() or needle in p.get("path", "").lower()]
    if not matches:
        raise SystemExit(f"target not found in manifest: {name}")
    if len(matches) > 1:
        names = ", ".join(f"{p.get('name')} ({p.get('path')})" for p in matches)
        raise SystemExit(f"ambiguous target {name!r}: {names}")
    return matches[0]


def project_dir(project: dict[str, Any], root: Path) -> Path:
    return root / Path(project["path"]).parent


def classify_project_type(project_type: str) -> str:
    text = project_type.lower()
    if "static library" in text:
        return "ea_vc6_static_library"
    return "# TODO: non-static-library project; add executable/shared-library helper before enabling\n# ea_vc6_static_library"


def format_list(values: list[str], indent: str = "  ") -> str:
    if not values:
        return ""
    return "\n".join(f"{indent}{v}" for v in values)


def generate(project: dict[str, Any]) -> str:
    target = project["name"]
    func = classify_project_type(project.get("project_type", ""))
    var = target.upper().replace("-", "_").replace(" ", "_")
    sources = [s for s in project.get("sources", []) if s != "$(InputPath)"]
    include_dirs = project.get("include_paths", [])
    defines = project.get("defines", [])

    lines: list[str] = []
    lines.append(f"# Conservative scaffold for the original EA VC6 {target} project.")
    lines.append("#")
    lines.append("# Generated from build-manifests/vc6-projects.json. Keep the original")
    lines.append("# target name and source inventory visible during baseline modernization.")
    lines.append("")
    lines.append(f"set({var}_SOURCES")
    lines.append(format_list(sources))
    lines.append(")")
    lines.append("")
    if func.startswith("# TODO"):
        lines.append(func)
        lines.append(f"# Project type: {project.get('project_type', 'unknown')}")
        lines.append(f"# Original outputs: {', '.join(project.get('outputs', [])) or 'none recorded'}")
        lines.append("")
    lines.append(f"{func if not func.startswith('# TODO') else 'ea_vc6_static_library'}({target}")
    lines.append("  SOURCES")
    lines.append(f"    ${{{var}_SOURCES}}")
    if include_dirs:
        lines.append("  INCLUDE_DIRS")
        lines.append(format_list(include_dirs, "    "))
    if defines:
        lines.append("  DEFINES")
        lines.append(format_list(defines, "    "))
    lines.append(")")
    lines.append("")
    return "\n".join(lines)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("target", help="Original VC6 project/target name")
    ap.add_argument("--root", default=".", help="Repository root")
    ap.add_argument("--manifest", default="build-manifests/vc6-projects.json")
    ap.add_argument("--write", action="store_true", help="Write Code/<target>/CMakeLists.txt instead of printing")
    ap.add_argument("--force", action="store_true", help="Overwrite an existing CMakeLists.txt when --write is used")
    args = ap.parse_args()

    root = Path(args.root).resolve()
    manifest = json.loads((root / args.manifest).read_text())
    project = find_project(manifest, args.target)
    text = generate(project)

    if args.write:
        out = project_dir(project, root) / "CMakeLists.txt"
        if out.exists() and not args.force:
            raise SystemExit(f"refusing to overwrite existing file without --force: {out}")
        out.write_text(text)
        print(out)
    else:
        print(text, end="")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
