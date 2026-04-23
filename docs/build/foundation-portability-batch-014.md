# Batch 014: `Combat` gameplay/runtime seam mapping

## Goal

Model the original EA `Code/Combat/Combat.dsp` target conservatively as an **opt-in gameplay/runtime seam**, keeping the default Linux bootstrap scaffold green while making the mixed responsibilities inside the historical `Combat` bucket explicit.

This batch does **not** claim a portable gameplay core or product parity. It records the first useful compile and boundary evidence for the largest pre-product shared gameplay bucket.

## Added CMake target

`Code/Combat/CMakeLists.txt` now models the original VC6 static library target name unchanged:

```text
Combat
```

The target is gated behind:

```text
RENEGADE_BUILD_COMBAT_SEAMS
```

Example configure command:

```bash
cmake -S . -B build/cmake-combat-probe \
  -DRENEGADE_BUILD_FOUNDATION_LIBS=ON \
  -DRENEGADE_BUILD_COMBAT_SEAMS=ON
```

## Source-island classification

The original source inventory is grouped into these first-pass logical islands:

```text
COMBAT_GAMEPLAY_RUNTIME_SOURCES
COMBAT_CLIENT_PRESENTATION_INPUT_SOURCES
COMBAT_SCRIPT_SAVE_NETWORK_SOURCES
COMBAT_AUDIO_CONVERSATION_SOURCES
```

Interpretation:

- `COMBAT_GAMEPLAY_RUNTIME_SOURCES` — shared game objects, combat rules, damage, weapons, vehicles, buildings, map/game settings, and other core runtime systems.
- `COMBAT_CLIENT_PRESENTATION_INPUT_SOURCES` — HUD, radar, message/text windows, camera, direct input, weapon view, and other client-presentation/input glue.
- `COMBAT_SCRIPT_SAVE_NETWORK_SOURCES` — script loading/execution, save/load, chunk IDs, and static network object glue.
- `COMBAT_AUDIO_CONVERSATION_SOURCES` — sound environment, combat sound registration, dialogue, conversations, orators, and viseme glue.

## Non-Windows handling

On non-Windows seam probes, the scaffold keeps the full source inventory visible but defers:

```text
COMBAT_CLIENT_PRESENTATION_INPUT_SOURCES
COMBAT_SCRIPT_SAVE_NETWORK_SOURCES
COMBAT_AUDIO_CONVERSATION_SOURCES
```

This allows the probe to test how much of the remaining gameplay/runtime layer is actually separable from product-shell, scripting, save/load, UI/input, audio, and network concerns.

## Narrow fixes applied before trusting the blockers

The first raw probe hit several Linux filesystem/include-identity issues before it reached the more meaningful seams. These were corrected as narrow, behavior-preserving fixes:

- `Always.h` -> `always.h`
- `Definition.h` -> `definition.h`
- `PersistFactory.h` -> `persistfactory.h`
- `DefinitionFactory.h` -> `definitionfactory.h`
- `SimpleDefinitionFactory.h` -> `simpledefinitionfactory.h`
- `CombatChunkID.h` -> `combatchunkid.h`
- `weathermgr.h` -> `WeatherMgr.h`
- `soundenvironment.h` -> `SoundEnvironment.h`
- `path.h` -> `Path.h`
- `audioevents.h` -> `AudioEvents.h`
- `audiblesound.h` -> `AudibleSound.h`
- `Code/Combat/CMakeLists.txt` include path corrected from `../wwaudio` to `../WWAudio`

These fixes were made only to get past Windows-case assumptions and reveal the real subsystem seams underneath.

## Probe result

Targeted probe command:

```bash
cmake --build build/cmake-combat-probe --target Combat -j4
```

After the narrow include-identity cleanup, the next meaningful blockers were:

### Blocker 1: product-shell coupling back into `Commando`

```text
Code/Combat/damage.h:49:25: fatal error: ..\commando\datasafe.h: No such file or directory
```

This is a strong signal that even nominally shared gameplay/runtime code still reaches back into the product shell (`Commando`) rather than cleanly sitting below it.

### Blocker 2: audio backend coupling through `WWAudio`

```text
Code/WWAudio/AudibleSound.h:45:10: fatal error: mss.h: No such file or directory
```

This surfaced through `WeatherMgr.cpp` and shows that parts of `Combat` still depend directly on the proprietary Miles-backed audio path rather than a backend-neutral gameplay/audio seam.

### Additional remaining noisy blocker

```text
Code/Combat/CNCModeSettings.cpp:24:10: fatal error: PlayerType.h: No such file or directory
```

This is another local include-case issue, but by the time it appeared the probe had already reached the more important `datasafe.h` and `mss.h` seams above.

## Architectural conclusion

`Combat` is **not** just shared gameplay logic. The current EA bucket mixes at least four different layers:

- shared gameplay/runtime systems
- client presentation/input glue
- script/save/network glue
- audio/conversation glue

Even after deferring the obvious client, script/save, and audio islands, the remaining gameplay/runtime subset still leaks:

- product-shell coupling back into `Commando` (`datasafe.h`)
- proprietary audio backend coupling (`mss.h` via `WWAudio`)

So the current batch records `Combat` as a **mixed engine_runtime/products/audio seam**, not a portable shared gameplay core.

## What this batch intentionally does not do

- no fake `mss.h` or Miles backend headers
- no fake `datasafe.h` shim
- no attempt to drag `Commando` into the build just to force `Combat` deeper
- no physical source moves
- no claim that `Combat` builds on Linux
- no claim of client/FDS parity

## Resulting status

- `Combat` is now **modeled**.
- `Combat` is **not** part of the default green scaffold.
- `Combat` currently has **configure/probe evidence and documented blockers**, not build or parity evidence.
