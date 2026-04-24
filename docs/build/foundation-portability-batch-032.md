# Batch 032: Combat datasafe typedef shim

**Date:** 2026-04-24
**Status:** ✅ Completed
**Theme:** Break `datasafe.h` public header dependency from Combat headers

## Goal

Remove `#include "../Commando/datasafe.h"` from three Combat public headers (`damage.h`, `playerdata.h`, `weaponmanager.h`) without changing struct field declarations or breaking the build.

## Approach: Typedef Shim

The `safe_float`, `safe_int`, and `safe_unsigned_int` types are declared via a local `using` alias inside each header. Struct field declarations (`safe_float Health;`, `safe_int ClipSize;`, etc.) are unchanged — the shim makes `safe_float` = `float` locally, breaking the `datasafe.h` include chain.

This is a **semantically zero-change** refactor:
- Struct field layouts are unchanged
- Save/load serialization is unchanged (`WRITE_SAFE_MICRO_CHUNK` macros serialize raw `float` bytes — they work identically with `SafeDataClass<float>` or plain `float`)
- Hot-path casts `(float)field` become no-ops on plain `float` — still correct
- The `ArmorWarheadManager::Multipliers` / `Absorbsion` static arrays remain as `safe_float*` (now plain `float*`) and their `Set_Precision` call is harmless on plain floats

## Files Changed

### `Code/Combat/damage.h`
- Removed `#include "../Commando/datasafe.h"`
- Added shim: `using safe_float = float;`, `using safe_int = int;`, `using safe_unsigned_int = unsigned int;`
- `SafeArmorType` and `SafeWarheadType` (typedef aliases of `safe_unsigned_int`) now resolve via the shim → `unsigned int`
- All struct fields in `DefenseObjectClass` and `DefenseObjectDefClass` are unchanged

### `Code/Combat/playerdata.h`
- Removed `#include "../Commando/datasafe.h"`
- Added shim: `using safe_float = float;`, `using safe_int = int;`
- `Score` and `Money` fields in `PlayerDataClass` are unchanged

### `Code/Combat/weaponmanager.h`
- Removed `#include "../Commando/datasafe.h"`
- Added shim: `using safe_float = float;`, `using safe_int = int;`
- All fields in `WeaponDefinitionClass` and `AmmoDefinitionClass` are unchanged

## What This Unblocks

Any future code that includes these headers no longer transitively pulls in `datasafe.h`. The `datasafe.h` public header chain into the product layer is broken at three points.

## Next Steps

If desired, the shim can be removed in a second pass: replace all `safe_float` field declarations with `float` directly, remove the shim `using` declarations, and update `WRITE_SAFE_MICRO_CHUNK` calls to plain `Write()` — all without changing save format (the chunk format already stores raw float bytes).

## Verification

- Default scaffold configure: ✅
- Default scaffold build: ✅ (`[100%] Built target wwnet`)
- `check_doc_sync.py`: ✅
- `git diff --check`: ✅
