# Initial CMake Scaffold

This scaffold is a conservative bridge from the original EA Visual Studio 6 project metadata to modern build tooling.

## Scope

The first CMake pass intentionally models a small set of low-level static libraries from `build-manifests/vc6-projects.json`:

- `wwdebug`
- `wwmath`
- `wwbitpack`
- `wwsaveload`
- `wwtranslatedb`

These are foundation-style projects with relatively low product/UI coupling. The scaffold does **not** attempt to build `commando`, FDS, renderer, online services, tools, or third-party SDK integrations yet.

## Naming rule

Targets keep their original VC6 project names for now. This is deliberate: the baseline phase records and validates historical project membership before introducing future architecture bucket names or physical source moves.

## Source-of-truth rule

The generated per-project `CMakeLists.txt` files are derived from:

```text
build-manifests/vc6-projects.json
```

Regenerate the manifest before expanding the scaffold if parser behavior changes.

## Missing legacy sources

The EA release tree still contains some VC6 references to files that are not present in the released source. The initial helper defaults to warning and skipping missing files so the scaffold can configure, while preserving a strict mode for audits:

```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_STRICT_SOURCE_MANIFEST=ON
```

At the time this scaffold was added, the selected foundation set had one known absent source reference:

```text
Code/WWMath/Vector3i.h
```

## Configure check

```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON
```

This validates target/source membership and CMake syntax. Compilation is a later step because the legacy source still contains compiler/platform assumptions that should be resolved systematically after the baseline target graph exists.
