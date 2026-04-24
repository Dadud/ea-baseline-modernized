#!/usr/bin/env python3
"""Compare EA baseline and OpenW3D file groups for seam-focused replay work.

Outputs:
- files present in both repos
- only in baseline
- only in OpenW3D
- identical files
- differing files with quick stats

Usage examples:
  python3 scripts/replay/compare_openw3d_port.py \
      --baseline /tmp/openw3d-ea-baseline \
      --openw3d /tmp/openw3d \
      --paths Code/wwlib/rawfile.cpp Code/wwlib/rcfile.cpp

  python3 scripts/replay/compare_openw3d_port.py \
      --baseline /tmp/openw3d-ea-baseline \
      --openw3d /tmp/openw3d \
      --glob 'Code/WWAudio/openal/*'
"""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
from typing import Iterable


def sha256(path: Path) -> str:
    h = hashlib.sha256()
    with path.open('rb') as f:
        for chunk in iter(lambda: f.read(65536), b''):
            h.update(chunk)
    return h.hexdigest()


def relset_from_paths(base: Path, relpaths: Iterable[str]) -> set[str]:
    found: set[str] = set()
    for rel in relpaths:
        p = base / rel
        if p.exists():
            found.add(rel)
    return found


def relset_from_glob(base: Path, pattern: str) -> set[str]:
    return {
        str(p.relative_to(base))
        for p in base.glob(pattern)
        if p.is_file()
    }


def file_info(base: Path, rel: str) -> dict:
    p = base / rel
    return {
        'path': rel,
        'size': p.stat().st_size,
        'sha256': sha256(p),
    }


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument('--baseline', required=True)
    ap.add_argument('--openw3d', required=True)
    ap.add_argument('--paths', nargs='*', default=[])
    ap.add_argument('--glob', action='append', default=[])
    ap.add_argument('--json', action='store_true', help='Emit JSON instead of text')
    args = ap.parse_args()

    baseline = Path(args.baseline).resolve()
    openw3d = Path(args.openw3d).resolve()

    relpaths: set[str] = set(args.paths)
    for pattern in args.glob:
        relpaths |= relset_from_glob(baseline, pattern)
        relpaths |= relset_from_glob(openw3d, pattern)

    baseline_files = relset_from_paths(baseline, relpaths)
    openw3d_files = relset_from_paths(openw3d, relpaths)

    both = sorted(baseline_files & openw3d_files)
    only_baseline = sorted(baseline_files - openw3d_files)
    only_openw3d = sorted(openw3d_files - baseline_files)

    identical = []
    differing = []
    for rel in both:
        bi = file_info(baseline, rel)
        oi = file_info(openw3d, rel)
        if bi['sha256'] == oi['sha256']:
            identical.append({'path': rel, 'size': bi['size']})
        else:
            differing.append({
                'path': rel,
                'baseline_size': bi['size'],
                'openw3d_size': oi['size'],
                'baseline_sha256': bi['sha256'],
                'openw3d_sha256': oi['sha256'],
            })

    result = {
        'baseline': str(baseline),
        'openw3d': str(openw3d),
        'requested_paths': sorted(relpaths),
        'both': both,
        'only_baseline': only_baseline,
        'only_openw3d': only_openw3d,
        'identical': identical,
        'differing': differing,
    }

    if args.json:
        print(json.dumps(result, indent=2))
        return 0

    print(f'Baseline: {baseline}')
    print(f'OpenW3D: {openw3d}')
    print()
    print(f'Files in both repos: {len(both)}')
    for rel in both:
        print(f'  = {rel}')
    print()
    print(f'Only in baseline: {len(only_baseline)}')
    for rel in only_baseline:
        print(f'  - {rel}')
    print()
    print(f'Only in OpenW3D: {len(only_openw3d)}')
    for rel in only_openw3d:
        print(f'  + {rel}')
    print()
    print(f'Identical files: {len(identical)}')
    for item in identical:
        print(f"  ok {item['path']} ({item['size']} bytes)")
    print()
    print(f'Differing files: {len(differing)}')
    for item in differing:
        print(
            f"  != {item['path']} "
            f"(baseline {item['baseline_size']} bytes, openw3d {item['openw3d_size']} bytes)"
        )

    return 0


if __name__ == '__main__':
    raise SystemExit(main())
