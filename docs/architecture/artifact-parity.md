# Artifact Parity Contract

## Purpose

This document defines what it means for the modernized EA baseline to reproduce the original client, FDS, and tools outputs.

The goal is not to pretend that every modern toolchain can produce byte-identical binaries. The goal is to make parity measurable and to classify differences honestly.

## Parity tiers

### Tier 1: Byte-identical parity

The modernized build produces byte-identical artifacts compared with a reference legacy build.

This is the ideal but may only be feasible if:
- the same compiler family/version is used
- timestamps/debug info are normalized
- library versions match
- original third-party SDKs are available

Tier 1 checks:
- exact file hashes match
- exact sizes match
- PE resources match
- PE imports/exports match
- version metadata matches

### Tier 2: Functional parity

The modernized build produces equivalent product behavior and artifact structure, but binaries may differ.

This is the primary practical target.

Tier 2 checks:
- expected executable/dll/tool names exist
- expected resources are embedded
- expected config/data lookup behavior works
- client boots to expected startup state
- FDS starts and reads server configuration
- tools launch or fail only due to documented missing SDKs/assets
- no known behavior change is introduced by build modernization

### Tier 3: Structural parity

The modernized build reproduces the product/module shape but may not yet fully run.

Tier 3 checks:
- targets exist for client/FDS/tools
- source inventories match original project intent
- missing dependencies are documented
- build graph preserves original product boundaries

## Primary acceptance target

Use **Tier 2 functional parity** as the main acceptance target.

Tier 1 should be attempted where practical, especially for narrow tools or configurations, but should not block the entire modernization if toolchain or SDK differences make it unrealistic.

## Product artifact groups

### Client artifacts
Expected to include:
- Renegade client executable(s)
- required DLLs/libraries where applicable
- embedded icons/resources/version data
- copied runtime outputs under `Run/` or documented modern equivalent

### FDS artifacts
Expected to include:
- dedicated server executable
- server config files or documented expected config paths
- required DLLs/libraries where applicable

### Tools artifacts
Expected to include available builds for:
- LevelEdit
- W3DView
- WWConfig
- MakeMix
- MixViewer
- RenRem
- wdump
- W3DShellExt
- any other original tool projects documented as in scope

## Manifest fields

Artifact manifest tooling should collect:

- relative path
- file size
- SHA256 hash
- build target name
- build configuration
- compiler/toolchain info
- timestamp policy
- PE import table summary, where applicable
- PE export table summary, where applicable
- PE resource summary, where applicable
- version metadata, where applicable

## Deviation categories

Every difference should be classified as:

1. **Expected toolchain difference**
2. **Timestamp/debug-info difference**
3. **Third-party SDK unavailable**
4. **Modern replacement dependency**
5. **Intentional modernization**
6. **Regression / needs investigation**

## Required scripts

Planned scripts:

```text
scripts/artifacts/manifest_build_outputs.py
scripts/artifacts/compare_build_outputs.py
```

These scripts should be added before serious build-system refactoring so every stage can be measured.

## Minimum smoke tests

### Client smoke test
- executable starts
- reaches initial startup/menu or documented equivalent
- does not immediately crash due to missing build-system output

### FDS smoke test
- executable starts in dedicated server mode
- reads config or reports expected missing config
- binds or attempts expected network ports
- exits cleanly on command or signal

### Tools smoke test
- tool executable starts
- reports missing SDK/assets if unavailable
- does not crash due to bad packaging/build output

## Relationship to code-bucket refactor

The bucket refactor must not replace parity checks.

Before moving or reorganizing significant code:
1. generate a manifest before the change
2. generate a manifest after the change
3. classify differences
4. run relevant smoke tests

## Success criteria

The parity contract is satisfied when:
- each target has a manifest
- each known deviation is classified
- smoke tests exist or blockers are documented
- baseline modernization can proceed without losing track of original behavior
