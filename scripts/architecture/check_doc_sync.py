#!/usr/bin/env python3
"""Lightweight doc-sync checker for the EA baseline modernization repo.

Checks:
- every target mentioned in the deferred-source ledger appears in project scaffold status
- every target mentioned in project scaffold status appears in original project scaffold status (best-effort)
- seam docs expected by current architecture exist

This is intentionally heuristic, not a strict parser.
"""

from __future__ import annotations

import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
ARCH = ROOT / 'docs' / 'architecture'


def read(path: Path) -> str:
    return path.read_text(encoding='utf-8') if path.exists() else ''


def md_code_targets(text: str) -> set[str]:
    return set(re.findall(r'`([^`]+)`', text))


def table_first_col_targets(text: str) -> set[str]:
    out: set[str] = set()
    for line in text.splitlines():
        if not line.strip().startswith('|'):
            continue
        cols = [c.strip() for c in line.strip().strip('|').split('|')]
        if not cols:
            continue
        first = cols[0]
        m = re.fullmatch(r'`([^`]+)`', first)
        if m:
            out.add(m.group(1))
    return out


def normalize_targets(values: set[str]) -> dict[str, str]:
    return {v.lower(): v for v in values}


def main() -> int:
    ledger = read(ARCH / 'deferred-source-ledger.md')
    project_status = read(ARCH / 'project-scaffold-status.md')
    original_status = read(ARCH / 'original-project-scaffold-status.md')

    ledger_targets = table_first_col_targets(ledger)
    project_targets = table_first_col_targets(project_status)
    original_targets = table_first_col_targets(original_status)

    ledger_norm = normalize_targets(ledger_targets)
    project_norm = normalize_targets(project_targets)
    original_norm = normalize_targets(original_targets)

    missing_from_project = sorted(
        ledger_norm[key] for key in ledger_norm.keys() if key not in project_norm
    )
    missing_from_original = sorted(
        project_norm[key] for key in project_norm.keys() if original_norm and key not in original_norm
    )

    expected_docs = [
        'renderer-seam-contract.md',
        'platform-boundary-contracts.md',
        'math-types-extraction-spec.md',
        'stub-contracts.md',
        'w3dhub-port-evaluation.md',
        'deferred-source-ledger.md',
        'project-scaffold-status.md',
        'modernization-progress-roadmap.md',
    ]
    missing_docs = sorted(name for name in expected_docs if not (ARCH / name).exists())

    print('Doc sync report')
    print(f'Root: {ROOT}')
    print()
    print(f'Ledger targets: {len(ledger_targets)}')
    print(f'Project status targets: {len(project_targets)}')
    print(f'Original status targets: {len(original_targets)}')
    print()

    if missing_from_project:
        print('Targets present in deferred-source ledger but missing from project-scaffold-status.md:')
        for t in missing_from_project:
            print(f'  - {t}')
    else:
        print('OK: all ledger targets appear in project-scaffold-status.md')
    print()

    if original_targets:
        if missing_from_original:
            print('Targets present in project-scaffold-status.md but missing from original-project-scaffold-status.md:')
            for t in missing_from_original:
                print(f'  - {t}')
        else:
            print('OK: all project status targets appear in original-project-scaffold-status.md')
    else:
        print('SKIP: original-project-scaffold-status.md not found or empty')
    print()

    if missing_docs:
        print('Missing expected architecture docs:')
        for name in missing_docs:
            print(f'  - {name}')
    else:
        print('OK: expected architecture docs present')

    return 0 if not (missing_from_project or missing_docs) else 1


if __name__ == '__main__':
    raise SystemExit(main())
