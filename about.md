# YT Auto Deco

YT Auto Deco is a Geode mod concept for Geometry Dash's level editor.

- Adds a red **YT** button in the upper-left area of the editor.
- Opens a draggable style window.
- Includes 33 selectable presets: Nightmare, Voltage, Ice, Blood, Hell, Polar, Inferno, Skyline, Neon, Aqua, Dessert, and 22 more.
- Adds decorative rod objects around supported solid blocks. It deliberately skips spikes, orbs, portals, pads, and triggers.

The first version is a pattern-based auto-decorator. It uses the level's primary/secondary color channels; it does not overwrite the level's global color triggers.

## Build

Requires the Geode SDK and CLI. Run `geode build` from this project directory. Android 64-bit builds can use `geode build -p android64` after installing the Android SDK binaries.
