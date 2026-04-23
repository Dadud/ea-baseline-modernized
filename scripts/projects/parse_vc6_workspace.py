#!/usr/bin/env python3
"""Parse Visual Studio 6 .dsw/.dsp project files into a JSON manifest.

The manifest is intentionally descriptive, not prescriptive. It records the
original EA project/workspace model so modern build scaffolding can be derived
from the legacy source of truth instead of hand-written from memory.
"""

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path
from typing import Any

TARGET_TYPE_RE = re.compile(r'# TARGTYPE "([^"]+)"')
PROJECT_NAME_RE = re.compile(r'# Microsoft Developer Studio Project File - Name="([^"]+)"')
WORKSPACE_PROJECT_RE = re.compile(r'Project: "([^"]+)"=(.+?) - Package')
WORKSPACE_DEP_RE = re.compile(r'Project_Dep_Name\s+(.+)')
CONFIG_RE = re.compile(r'!MESSAGE "([^"]+)" \(based on "([^"]+)"\)')
SOURCE_RE = re.compile(r'^\s*SOURCE=(.+)$', re.MULTILINE)
ADD_CPP_RE = re.compile(r'^\s*# ADD CPP\s+(.+)$', re.MULTILINE)
ADD_LINK_RE = re.compile(r'^\s*# ADD LINK32\s+(.+)$', re.MULTILINE)
OUTPUT_RE = re.compile(r'/out:"([^"]+)"', re.IGNORECASE)


def _dedupe(values: list[str]) -> list[str]:
    return sorted({v for v in values if v})


def _clean_path(value: str) -> str:
    value = value.strip().strip('"')
    return value.replace('\\', '/')


def _parse_cpp_flags(flag_line: str) -> dict[str, list[str]]:
    include_matches = re.findall(r'/I\s+"([^"]+)"|/I\s+([^\s"]+)', flag_line)
    define_matches = re.findall(r'/D\s+"([^"]+)"|/D\s+([^\s"]+)', flag_line)
    return {
        "include_paths": [_clean_path(a or b) for a, b in include_matches],
        "defines": [(a or b).strip() for a, b in define_matches],
    }


def _parse_link_flags(flag_line: str) -> dict[str, list[str]]:
    tokens = re.findall(r'"[^"]+"|\S+', flag_line)
    libraries: list[str] = []
    library_paths: list[str] = []
    for token in tokens:
        clean = token.strip('"')
        lower = clean.lower()
        if lower.startswith('/libpath:'):
            library_paths.append(_clean_path(clean.split(':', 1)[1]))
        elif lower.endswith('.lib'):
            libraries.append(clean)
    return {"libraries": libraries, "library_paths": library_paths}


def parse_dsp(path: Path, root: Path) -> dict[str, Any]:
    text = path.read_text(errors="ignore")
    rel_path = path.relative_to(root).as_posix()

    name_match = PROJECT_NAME_RE.search(text)
    target_type_match = TARGET_TYPE_RE.search(text)

    sources = []
    for source in SOURCE_RE.findall(text):
        cleaned = _clean_path(source)
        if cleaned and not cleaned.startswith('#'):
            sources.append(cleaned)

    include_paths: list[str] = []
    defines: list[str] = []
    for cpp_line in ADD_CPP_RE.findall(text):
        parsed = _parse_cpp_flags(cpp_line)
        include_paths.extend(parsed["include_paths"])
        defines.extend(parsed["defines"])

    libraries: list[str] = []
    library_paths: list[str] = []
    outputs: list[str] = []
    for link_line in ADD_LINK_RE.findall(text):
        parsed = _parse_link_flags(link_line)
        libraries.extend(parsed["libraries"])
        library_paths.extend(parsed["library_paths"])
        outputs.extend(_clean_path(o) for o in OUTPUT_RE.findall(link_line))

    configurations = [
        {"name": name, "based_on": based_on}
        for name, based_on in CONFIG_RE.findall(text)
    ]

    return {
        "name": name_match.group(1) if name_match else path.stem,
        "path": rel_path,
        "project_type": target_type_match.group(1) if target_type_match else None,
        "configurations": configurations,
        "sources": _dedupe(sources),
        "source_count": len(set(sources)),
        "include_paths": _dedupe(include_paths),
        "defines": _dedupe(defines),
        "libraries": _dedupe(libraries),
        "library_paths": _dedupe(library_paths),
        "outputs": _dedupe(outputs),
    }


def parse_dsw(path: Path, root: Path) -> dict[str, Any]:
    text = path.read_text(errors="ignore")
    projects: list[dict[str, Any]] = []
    current: dict[str, Any] | None = None

    for line in text.splitlines():
        project_match = WORKSPACE_PROJECT_RE.match(line)
        if project_match:
            current = {
                "name": project_match.group(1),
                "path": _clean_path(project_match.group(2)),
                "dependencies": [],
            }
            projects.append(current)
            continue

        if current:
            dep_match = WORKSPACE_DEP_RE.search(line)
            if dep_match:
                current["dependencies"].append(dep_match.group(1).strip())

    return {"path": path.relative_to(root).as_posix(), "projects": projects}


def build_manifest(root: Path) -> dict[str, Any]:
    workspaces = [parse_dsw(path, root) for path in sorted(root.glob("Code/**/*.dsw"))]
    projects = [parse_dsp(path, root) for path in sorted(root.glob("Code/**/*.dsp"))]
    return {
        "schema_version": 1,
        "description": "Original EA Visual Studio 6 workspace/project manifest.",
        "workspaces": workspaces,
        "projects": projects,
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root", default=".", help="Repository root to scan")
    parser.add_argument("--output", required=True, help="JSON manifest output path")
    args = parser.parse_args()

    root = Path(args.root).resolve()
    manifest = build_manifest(root)
    output = Path(args.output)
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(json.dumps(manifest, indent=2, sort_keys=True) + "\n")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
