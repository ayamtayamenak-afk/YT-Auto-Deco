# YT Auto Deco — Geode project

**A Geode mod project for Geometry Dash.** The editor button is red and says `YT`. Tapping it opens a draggable panel with 33 styles and an `AUTO DECO` action.

## Included styles

1. Nightmare  2. Voltage  3. Ice  4. Blood  5. Hell  6. Polar  7. Inferno  8. Skyline  9. Neon  10. Aqua  11. Dessert
12. Forest  13. Candy  14. Cyber  15. Toxic  16. Lava  17. Galaxy  18. Sunset  19. Arctic  20. Shadow  21. Crystal  22. Techno
23. Ocean  24. Jungle  25. Royal  26. Retro  27. Glitch  28. Plasma  29. Sakura  30. Desert Storm  31. Mythic  32. Void  33. Classic

## What this first version does

- Hooks the Geometry Dash level editor and adds the YT button near the top-left.
- Presents the presets over three pages. Drag the red title bar to move the panel.
- Adds decoration objects around commonly used solid blocks. It does not modify the original blocks or intentionally add spikes, portals, orbs, pads, or triggers.
- Uses the level's existing primary and secondary colour channels.
- Caps one decoration run at 900 objects to prevent runaway object creation.

This is a **first playable source draft**, not a claim that every style has a fully hand-designed theme. Each style selects one of six placement patterns, with channel-order variations. The first iteration does not install custom colour triggers or replace the level palette.

## Build a `.geode` package

Geode's official build process uses the Geode SDK/CLI. A `.geode` package is produced by compiling the C++ project; renaming this ZIP to `.geode` will not work.

### Android 64-bit (recommended for most modern Android phones)

1. Put the contents of this folder in a GitHub repository.
2. Open the repository's **Actions** tab and run **Build YT Auto Deco** (or push a commit to trigger it).
3. Download the `YT-Auto-Deco-Android64` artifact. It contains the compiled `.geode` package.
4. Install that `.geode` file through Geode's mod installer, or copy it to:
   `/storage/emulated/0/Android/media/com.geode.launcher/game/geode/mods/`
5. Restart Geometry Dash and enable **YT Auto Deco** in Geode.

The workflow also builds a Windows 64-bit package. For a 32-bit Android device, change the workflow's `Android64` target to `Android32`.

### Local build

```bash
geode sdk install
geode sdk install-binaries
geode build
```

For Android 64-bit, install the Android NDK and Android Geode binaries, then run:

```bash
geode sdk install-binaries -p android64
geode build -p android64
```

## Tests

The pure C++ style/generator tests can run without the Geode SDK:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -Isrc \
  tests/test_generator.cpp src/Generator.cpp -o test_generator
./test_generator
```

## Compatibility

Targets Geode 5.x / Geometry Dash 2.2081. The Geode-facing C++ layer still needs a real SDK build and in-game test before it can be described as fully verified. If the SDK reports an API mismatch, check the first compiler error in the GitHub Actions build log.
