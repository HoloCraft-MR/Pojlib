# Quest port notes (coah-quest-support)

This branch carries coah80's QuestCraft Visor changes on top of QuestCraftPlusPlus/Pojlib `QuestCraft-6.0.0`.

## Bundled mod binaries (`src/main/assets/mods/`)

The installer copies these jars from APK assets into the instance's `mods/` folder, so they are vendored.

| File | Source | License |
|---|---|---|
| `Visor.jar` | HoloCraft-MR/Visor `coah-quest-support` @ 9d17857, `:visor-fabric:1.20.1:buildAndCollect` -> `Visor-0.6.0-beta+mc1.20.1-fabric.jar` (sha256 ffec16d5d11cc6fcce5a2f50b942d08f68ebcfe202e98d1648eae0bf18292cb9) | MPL-2.0 (Visor), AtumVR inside |
| `VisorEssentials.jar` | prebuilt 0.4.0 by PhoenixRa, added by coah | LGPLv3 |
| `BadOptimizations.jar`, `C2ME.jar`, `Chunky.jar`, `MoreCulling.jar`, `Noisium.jar` | prebuilt Modrinth releases, added by coah | see each mod |

To update Visor: build the Visor branch above, then copy the jar over `src/main/assets/mods/Visor.jar` and update the version in `Installer.java`.

## LWJGL / OpenXR

Pojlib's `assets/lwjgl/lwjgl-glfw-classes.jar` is QuestCraft's LWJGL 3.3.3-based core and has no OpenXR classes.
The Visor jar includes `lwjgl-openxr-3.3.6` (and `lwjgl-vulkan-3.3.6`) as Fabric jar-in-jar, which AtumVR uses.
All `org.lwjgl.*` core members referenced by lwjgl-openxr 3.3.6, AtumVR and Visor were checked with `javap` against the bundled core jar and resolve, so no LWJGL core change is needed. This is a bytecode-level check only; it has not been run on a headset.
