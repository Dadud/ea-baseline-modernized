
# Command & Conquer Renegade

This repository includes source code for Command & Conquer Renegade. This release provides support to the [Steam Workshop](https://steamcommunity.com/workshop/browse/?appid=2229890) for the game.


## Modernization branch status

This fork is being modernized with an **EA-baseline-first** workflow: preserve and document the original Electronic Arts Renegade source layout first, build a conservative modern scaffold from the original Visual Studio 6 project metadata, and only later replay OpenW3D/fork changes onto a cleaner foundation.

Current modernization docs of interest:

- [`docs/architecture/modernization-strategy-v2.md`](docs/architecture/modernization-strategy-v2.md) — current strategy and workflow.
- [`docs/architecture/modernization-progress-roadmap.md`](docs/architecture/modernization-progress-roadmap.md) — progress and remaining work.
- [`docs/architecture/original-project-scaffold-status.md`](docs/architecture/original-project-scaffold-status.md) — original VC6 project status matrix.
- [`docs/architecture/deferred-source-ledger.md`](docs/architecture/deferred-source-ledger.md) — intentionally deferred source islands and why they remain deferred.
- [`docs/parity/target-status.md`](docs/parity/target-status.md) — distinguishes modeled targets, default green scaffold targets, and parity evidence state.
- [`docs/build/project-ingestion-and-probe-workflow.md`](docs/build/project-ingestion-and-probe-workflow.md) — repeatable target ingestion/probe workflow.

The current Linux bootstrap CMake scaffold builds selected low-level/media-adjacent source islands, not the final client/FDS/tools products yet:

```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON
cmake --build build/cmake-scaffold -j4
```

Currently modeled scaffold targets include:

```text
wwdebug
wwmath
wwbitpack
wwsaveload
wwtranslatedb
wwlib
wwutil
wwnet
BinkMovie
SControl
```

Scaffold success is tracked separately from full parity. Some original sources are intentionally deferred where they cross unresolved DirectDraw, WinSock, RAD/Bink, audio, UI, online-service, platform, or product-shell boundaries.

Additional subsystem seam probes may be configured separately. For example, the current `WWAudio` scaffold is opt-in because it intentionally stops at the proprietary Miles/DirectSound backend boundary, and `wwphys` is opt-in because it currently exposes mixed runtime/renderer/platform leakage:

```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_AUDIO_SEAMS=ON
cmake -S . -B build/cmake-phys-probe -DRENEGADE_BUILD_FOUNDATION_LIBS=ON -DRENEGADE_BUILD_PHYS_SEAMS=ON
```

## Dependencies

If you wish to rebuild the original source code and tools successfully you will need to find or write new replacements (or remove the code using them entirely) for the following libraries;

- DirectX SDK (Version 8.0 or higher) (expected path `\Code\DirectX\`)
- RAD Bink SDK - (expected path `\Code\BinkMovie\`)
- RAD Miles Sound System SDK - (expected path `\Code\Miles6\`)
- NvDXTLib SDK - (expected path `\Code\NvDXTLib\`)
- Lightscape SDK - (expected path `\Code\Lightscape\`)
- Umbra SDK - (expected path `\Code\Umbra\`)
- GameSpy SDK - (expected path `\Code\GameSpy\`)
- GNU Regex - (expected path `\Code\WWLib\`)
- SafeDisk API - (expected path `\Code\Launcher\SafeDisk\`)
- Microsoft Cab Archive Library - (expected path `\Code\Installer\Cab\`)
- RTPatch Library - (expected path `\Code\Installer\`)
- Java Runtime Headers - (expected path `\Code\Tools\RenegadeGR\`)


## Compiling (Win32 Only)

To use the compiled binaries, you must own the game. The C&C Ultimate Collection is available for purchase on [EA App](https://www.ea.com/en-gb/games/command-and-conquer/command-and-conquer-the-ultimate-collection/buy/pc) or [Steam](https://store.steampowered.com/bundle/39394/Command__Conquer_The_Ultimate_Collection/).

### Renegade

The quickest way to build all configurations in the project is to open `commando.dsw` in Microsoft Visual Studio C++ 6.0 (SP5 recommended for binary matching to patch 1.037) and select Build -> Batch Build, then hit the “Rebuild All” button.

If you wish to compile the code under a modern version of Microsoft Visual Studio, you can convert the legacy project file to a modern MSVC solution by opening the `commando.dsw` in Microsoft Visual Studio .NET 2003, and then opening the newly created project and solution file in MSVC 2015 or newer.

NOTE: As modern versions of MSVC enforce newer revisions of the C++ standard, you will need to make extensive changes to the codebase before it successfully compiles, even more so if you plan on compiling for the Win64 platform.

When the workspace has finished building, the compiled binaries will be copied to the `/Run/` directory found in the root of this repository. 


### Free Dedicated Server
It’s possible to build the Windows version of the FDS (Free Dedicated Server) for Command & Conquer Renegade from the source code in this repository, just uncomment `#define FREEDEDICATEDSERVER` in [Combat\specialbuilds.h](Combat\specialbuilds.h) and perform a “Rebuild All” action on the Release config.


### Level Edit (Public Release)
To build the public release build of Level Edit, modify the LevelEdit project settings and add `PUBLIC_EDITOR_VER` to the preprocessor defines.


## Known Issues

The “Debug” configuration of the “Commando” project (the Renegade main project) will sometimes fail to link the final executable. This is due to Windows Defender incorrectly detecting RenegadeD.exe containing a virus (possibly due to the embedded browser code). Excluding the output `/Run/` folder found in the root of this repository in Windows Defender should resolve this for you.


## Contributing

This repository will not be accepting contributions (pull requests, issues, etc). If you wish to create changes to the source code and encourage collaboration, please create a fork of the repository under your GitHub user/organization space.


## Support

This repository is for preservation purposes only and is archived without support. 


## License

This repository and its contents are licensed under the GPL v3 license, with additional terms applied. Please see [LICENSE.md](LICENSE.md) for details.
