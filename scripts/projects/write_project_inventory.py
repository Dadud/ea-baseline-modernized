#!/usr/bin/env python3
"""Write a Markdown project inventory from a VC6 JSON manifest."""

from __future__ import annotations

import argparse
import json
from collections import Counter
from pathlib import Path
from typing import Any


def esc(value: Any) -> str:
    return ("" if value is None else str(value)).replace("|", "\\|")


def project_kind(project_type: str | None) -> str:
    if not project_type:
        return "unknown"
    lower = project_type.lower()
    if "static library" in lower:
        return "static library"
    if "dynamic-link library" in lower:
        return "dll"
    if "console application" in lower:
        return "console app"
    if "application" in lower:
        return "gui app"
    return project_type


def write_markdown(manifest: dict[str, Any]) -> str:
    projects = manifest["projects"]
    workspaces = manifest["workspaces"]
    kind_counts = Counter(project_kind(project.get("project_type")) for project in projects)

    lines: list[str] = []
    lines.append("# VC6 Project Manifest")
    lines.append("")
    lines.append("Generated from the original EA Visual Studio 6 `.dsw` / `.dsp` files.")
    lines.append("")
    lines.append("## Summary")
    lines.append("")
    lines.append(f"- Workspaces: {len(workspaces)}")
    lines.append(f"- Projects: {len(projects)}")
    lines.append("")
    lines.append("### Project type counts")
    lines.append("")
    lines.append("| Type | Count |")
    lines.append("| --- | ---: |")
    for kind, count in sorted(kind_counts.items()):
        lines.append(f"| {esc(kind)} | {count} |")
    lines.append("")

    lines.append("## Workspaces")
    lines.append("")
    for workspace in workspaces:
        lines.append(f"### `{workspace['path']}`")
        lines.append("")
        if not workspace["projects"]:
            lines.append("No projects parsed.")
            lines.append("")
            continue
        lines.append("| Project | DSP path | Dependencies |")
        lines.append("| --- | --- | --- |")
        for project in workspace["projects"]:
            dependencies = ", ".join(project.get("dependencies", []))
            lines.append(f"| `{esc(project['name'])}` | `{esc(project['path'])}` | {esc(dependencies)} |")
        lines.append("")

    lines.append("## Projects")
    lines.append("")
    lines.append("| Project | Type | Configurations | Sources | DSP path |")
    lines.append("| --- | --- | ---: | ---: | --- |")
    for project in sorted(projects, key=lambda p: p["path"].lower()):
        lines.append(
            f"| `{esc(project['name'])}` | {esc(project_kind(project.get('project_type')))} | "
            f"{len(project.get('configurations', []))} | {project.get('source_count', 0)} | `{esc(project['path'])}` |"
        )
    lines.append("")

    lines.append("## Notes")
    lines.append("")
    lines.append("This manifest is descriptive. It preserves the original project names, paths, configurations, source lists, include paths, defines, link libraries, output paths, and workspace dependency edges. Modern build files should be derived from this manifest rather than hand-written from memory.")
    lines.append("")
    return "\n".join(lines)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("manifest", help="Path to vc6-projects.json")
    parser.add_argument("--output", required=True, help="Markdown output path")
    args = parser.parse_args()

    manifest = json.loads(Path(args.manifest).read_text())
    output = Path(args.output)
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(write_markdown(manifest) + "\n")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
