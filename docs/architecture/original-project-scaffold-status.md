# Original Project Scaffold Status Matrix

This matrix summarizes the generated VC6 project manifest after Batch 005. It is a planning aid for deciding what to model next; it is not a physical move list.

| Project | Type | VC6 project | Scaffold status | Likely future bucket |
| --- | --- | --- | --- | --- |
| `commando` | `Application` | `Code/Commando/commando.dsp` | not modeled | products/client + products/dedicated_server + online_services |
| `Installer` | `Application` | `Code/Installer/Installer.dsp` | not modeled | products/installer |
| `launcher` | `Application` | `Code/Launcher/launcher.dsp` | not modeled | products/launcher |
| `meshtest` | `Application` | `Code/Tests/MeshTest/meshtest.dsp` | not modeled | tests |
| `movietest` | `Application` | `Code/Tests/movietest/movietest.dsp` | not modeled | tests |
| `PhysTest` | `Application` | `Code/Tests/PhysTest/PhysTest.dsp` | not modeled | tests |
| `skeleton_gth` | `Application` | `Code/Tests/skeleton_gth/skeleton_gth.dsp` | not modeled | tests |
| `SplineTest` | `Application` | `Code/Tests/SplineTest/SplineTest.dsp` | not modeled | tests |
| `ChunkView` | `Application` | `Code/Tools/ChunkView/ChunkView.dsp` | not modeled | products/tools/engine_asset_content |
| `CommandoUpdate` | `Application` | `Code/Tools/CommandoUpdate/CommandoUpdate.dsp` | not modeled | products/tools/engine_asset_content |
| `CopyLocked` | `Application` | `Code/Tools/CopyLocked/CopyLocked.dsp` | not modeled | products/tools/engine_asset_content |
| `LevelEdit` | `Application` | `Code/Tools/LevelEdit/LevelEdit.dsp` | not modeled | products/tools/engine_asset_content |
| `LightMap` | `Application` | `Code/Tools/LightMap/LightMap.dsp` | not modeled | products/tools/engine_asset_content |
| `MaxFly` | `Application` | `Code/Tools/MaxFly/MaxFly.dsp` | not modeled | products/tools/engine_asset_content |
| `MixViewer` | `Application` | `Code/Tools/MixViewer/MixViewer.dsp` | not modeled | products/tools/engine_asset_content |
| `SimpleGraph` | `Application` | `Code/Tools/SimpleGraph/SimpleGraph.dsp` | not modeled | products/tools/engine_asset_content |
| `VidInit` | `Application` | `Code/Tools/vidinit/VidInit.dsp` | not modeled | products/tools/engine_asset_content |
| `W3DShellExt` | `Application` | `Code/Tools/W3DShellExt/W3DShellExt.dsp` | not modeled | products/tools/engine_asset_content |
| `W3DUpdate` | `Application` | `Code/Tools/W3DUpdate/W3DUpdate.dsp` | not modeled | products/tools/engine_asset_content |
| `W3DView` | `Application` | `Code/Tools/W3DView/W3DView.dsp` | not modeled | products/tools/engine_asset_content |
| `wdump` | `Application` | `Code/Tools/wdump/wdump.dsp` | not modeled | products/tools/engine_asset_content |
| `WWConfig` | `Application` | `Code/Tools/WWConfig/WWConfig.dsp` | not modeled | products/tools/engine_asset_content |
| `skeleton` | `Application` | `Code/ww3d2/hueshift/hueshift.dsp` | not modeled | renderer tests/tools |
| `skeleton` | `Application` | `Code/ww3d2/skeleton/skeleton.dsp` | not modeled | renderer tests/tools |
| `skeleton` | `Application` | `Code/ww3d2/sorttest/sorttest.dsp` | not modeled | renderer tests/tools |
| `bandy` | `Console Application` | `Code/Tests/Bandy/bandy.dsp` | not modeled | tests |
| `BitPackTest` | `Console Application` | `Code/Tests/BitPackTest/BitPackTest.dsp` | not modeled | tests |
| `collide` | `Console Application` | `Code/Tests/collide/collide.dsp` | not modeled | tests |
| `LocalHost` | `Console Application` | `Code/Tests/LocalHost/LocalHost.dsp` | not modeled | tests |
| `mathtest` | `Console Application` | `Code/Tests/mathtest/mathtest.dsp` | not modeled | tests |
| `bin2cpp` | `Console Application` | `Code/Tools/bin2cpp/bin2cpp.dsp` | not modeled | products/tools/engine_asset_content |
| `MakeMix` | `Console Application` | `Code/Tools/MakeMix/MakeMix.dsp` | not modeled | products/tools/engine_asset_content |
| `RenRem` | `Console Application` | `Code/Tools/RenRem/RenRem.dsp` | not modeled | products/tools/engine_asset_content |
| `SkeletonHack` | `Console Application` | `Code/Tools/SkeletonHack/SkeletonHack.dsp` | not modeled | products/tools/engine_asset_content |
| `VerStamp` | `Console Application` | `Code/Tools/VerStamp/VerStamp.dsp` | not modeled | products/tools/engine_asset_content |
| `BandTest` | `Dynamic-Link Library` | `Code/BandTest/BandTest.dsp` | not modeled | online_services/test |
| `Scripts` | `Dynamic-Link Library` | `Code/Scripts/Scripts.dsp` | not modeled | products/scripts/engine_runtime |
| `AMC_imp` | `Dynamic-Link Library` | `Code/Tools/AMC_IMP/AMC_IMP.dsp` | not modeled | products/tools/engine_asset_content |
| `asf_imp` | `Dynamic-Link Library` | `Code/Tools/ASF_IMP/ASF_IMP.dsp` | not modeled | products/tools/engine_asset_content |
| `Blender2` | `Dynamic-Link Library` | `Code/Tools/Blender2/Blender2.dsp` | not modeled | products/tools/engine_asset_content |
| `Clipbord` | `Dynamic-Link Library` | `Code/Tools/Clipbord/Clipbord.dsp` | not modeled | products/tools/engine_asset_content |
| `max2w3d` | `Dynamic-Link Library` | `Code/Tools/max2w3d/max2w3d.dsp` | not modeled | products/tools/engine_asset_content |
| `RenegadeGR` | `Dynamic-Link Library` | `Code/Tools/RenegadeGR/RenegadeGR.dsp` | not modeled | products/tools/engine_asset_content |
| `ViewTrans` | `Dynamic-Link Library` | `Code/Tools/ViewTrans/ViewTrans.dsp` | not modeled | products/tools/engine_asset_content |
| `WWCtrl` | `Dynamic-Link Library` | `Code/Tools/WWCtrl/WWCtrl.dsp` | not modeled | products/tools/engine_asset_content |
| `BinkMovie` | `Static Library` | `Code/BinkMovie/BinkMovie.dsp` | modeled/builds subtitle parser island; RAD/renderer islands deferred | engine_asset_content/products/client/renderer seam |
| `Combat` | `Static Library` | `Code/Combat/Combat.dsp` | not modeled | engine_runtime with presentation seams |
| `SControl` | `Static Library` | `Code/SControl/SControl.dsp` | not modeled | products/dedicated_server/online_services |
| `pluglib` | `Static Library` | `Code/Tools/pluglib/pluglib.dsp` | not modeled | products/tools/engine_asset_content |
| `ww3d2` | `Static Library` | `Code/ww3d2/ww3d2.dsp` | not modeled | renderer/engine_asset_content |
| `WWAudio` | `Static Library` | `Code/WWAudio/WWAudio.dsp` | not modeled | audio |
| `wwbitpack` | `Static Library` | `Code/wwbitpack/wwbitpack.dsp` | modeled/builds | foundation |
| `wwdebug` | `Static Library` | `Code/wwdebug/wwdebug.dsp` | modeled/builds | foundation |
| `wwlib` | `Static Library` | `Code/wwlib/wwlib.dsp` | modeled/builds | foundation/platform/content/renderer seams |
| `wwmath` | `Static Library` | `Code/WWMath/wwmath.dsp` | modeled/builds | foundation |
| `wwnet` | `Static Library` | `Code/wwnet/wwnet.dsp` | modeled/builds | platform/engine_runtime |
| `wwphys` | `Static Library` | `Code/wwphys/wwphys.dsp` | not modeled | engine_runtime/renderer seam |
| `wwsaveload` | `Static Library` | `Code/wwsaveload/wwsaveload.dsp` | modeled/builds | foundation |
| `wwtranslatedb` | `Static Library` | `Code/wwtranslatedb/wwtranslatedb.dsp` | modeled/builds | foundation |
| `wwui` | `Static Library` | `Code/wwui/wwui.dsp` | not modeled | products/client/input_and_control |
| `wwutil` | `Static Library` | `Code/wwutil/wwutil.dsp` | modeled/builds | foundation |
