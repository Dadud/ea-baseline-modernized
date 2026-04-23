# Current-to-Future Code Bucket Map

## Purpose

This document maps the original EA source tree into the long-term 9-bucket architecture. It is a planning aid, not an immediate move list.

Do **not** physically reorganize the tree until logical target and include boundaries are proven.

## Final target buckets

1. `foundation`
2. `platform`
3. `engine_runtime`
4. `engine_asset_content`
5. `renderer`
6. `audio`
7. `input_and_control`
8. `online_services`
9. `products`

## Current source map

### `Code/Combat`

Best current description:
- primary shared gameplay/simulation code
- also contains client presentation/input/render/audio-adjacent code

Future buckets:
- mostly `engine_runtime`
- some `input_and_control`
- some client presentation under `products/client`
- some render-facing behavior under `renderer` or presentation adapters

### `Code/Commando`

Best current description:
- overloaded app/product shell
- mixes client, dedicated server, online, frontend UI, config, game-mode orchestration, GameSpy/WOL glue

Future buckets:
- `products/client`
- `products/dedicated_server`
- `online_services`
- selected shared pieces to `engine_runtime`
- input configuration to `input_and_control`

### `Code/ww3d2`

Best current description:
- renderer, scene, visual asset, and Direct3D-era implementation layer

Future buckets:
- `renderer`
- some neutral asset/content pieces to `engine_asset_content`

### `Code/WWAudio`

Best current description:
- audio subsystem with backend-like structure already emerging

Future bucket:
- `audio`

### `Code/wwui`

Best current description:
- UI framework and IME/input UI support

Future buckets:
- mostly `products/client`
- some generic UI/input support may belong under `input_and_control` or a client UI subsystem

### `Code/wwphys`

Best current description:
- physics/pathing/visibility/scene-render hybrid

Future buckets:
- `engine_runtime`
- `engine_asset_content`
- `renderer` bridge pieces

### `Code/wwnet`

Best current description:
- low-level networking and replicated object/network helpers

Future buckets:
- `platform` for sockets
- `engine_runtime` for replicated network model

### `Code/WWOnline`, `Code/wolapi`, `Code/WOLBrowser`

Best current description:
- legacy Westwood Online and COM/browser integration

Future bucket:
- `online_services`

### `Code/Tools`

Best current description:
- original editors/viewers/config/archive utilities

Future buckets:
- `products/tools`
- shared format/content code to `engine_asset_content`

### `Code/BinkMovie`

Best current description:
- video/movie playback support and subtitle metadata

Future buckets:
- `products/client` for playback integration
- `engine_asset_content` for subtitle/metadata parsing
- possibly `audio` for audio integration

### `Code/Launcher`

Best current description:
- Windows client launcher/shell

Future bucket:
- `products/client` or `products/launcher`

### `Code/Scripts`

Best current description:
- script product/runtime code

Future bucket:
- `products/scripts`, with reusable script interfaces moved to `engine_runtime` if needed

### `Code/SControl`

Best current description:
- server-control/network-adjacent Windows-era component

Future buckets:
- `online_services` or `products/dedicated_server`, depending on final role

### `Code/Installer`

Best current description:
- legacy installer/patcher code

Future bucket:
- `products/tools` or `products/installer` if kept

### Foundation siblings

Current dirs:
- `Code/wwlib`
- `Code/wwutil`
- `Code/wwdebug`
- `Code/WWMath`
- `Code/wwbitpack`
- `Code/wwsaveload`
- `Code/wwtranslatedb`

Future buckets:
- mostly `foundation`
- platform-specific pieces moved to `platform`
- content/archive pieces moved to `engine_asset_content`

## High-priority split targets later

1. Split `Commando` source sets into client/server/shared/online.
2. Split `Combat` into shared gameplay vs client presentation/input.
3. Split `ww3d2` into renderer API/backend/content pieces.
4. Split `wwphys` into physics/runtime vs renderer bridge.
5. Split input into backend/action/config layers.
6. Split online service interfaces from legacy WOL/GameSpy implementations.
7. Extract shared format/content libraries for tools.
