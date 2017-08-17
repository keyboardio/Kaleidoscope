# Kaleidoscope-LED-Palette-Theme

![status][st:experimental] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-LED-Palette-Theme.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-LED-Palette-Theme

 [st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

A common base for plugins that want to provide themes, or theme-related
capabilities, using a 16 color palette. In other words, this is for plugin
authors primarily. The primary aim of the plugin is to provide not only a common
palette, but tools that make it easier to use it too.

## Using the plugin

To use the plugin, one needs to do a bit more than include the header, and tell
the firmware to use it. Itself being a mere building block, to use it to its
full extent, we need to create our own plugin on top of it.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-LED-Palette-Theme.h>
#include <Kaleidoscope-Focus.h>

namespace example {

class TestLEDMode : public LEDMode {
 public:
  TestLEDMode() {}

 protected:
  void setup(void) final;
  void update(void) final;

  static bool focusHook(const char *command);

 private:
  static uint16_t map_base_;
};

uint16_t TestLEDMode::map_base_;

void TestLEDMode::setup(void) {
  map_base_ = LEDPaletteTheme.reserveThemes(1);
}

void TestLEDMode::update(void) {
  LEDPaletteTheme.updateHandler(map_base_, 0);
}

bool
TestLEDMode::focusHook(const char *command) {
  return LEDPaletteTheme.themeFocusHandler(command, PSTR("testLedMode.map"), map_base_, 1);
}

}

example::TestLEDMode TestLEDMode;

void setup() {
  Serial.begin(9600);

  Kaleidoscope.use(&Focus, &LEDPaletteTheme, &TestLEDMode, &EEPROMSettings);

  Kaleidoscope.setup();

  EEPROMSettings.seal();
  TestLEDMode.activate();

  Focus.addHook(FOCUS_HOOK_LEDPALETTETHEME);
  Focus.addHook(FOCUS_HOOK(TestLEDMode.focusHook, "testLEDMode.map"));
}
```

This is a simple extension, where it provides a `testLEDMode.map` Focus command,
with which one can set the theme which will be saved to EEPROM.

## Plugin methods

The plugin provides the `LEDPaletteTheme` object, which has the following methods and properties:

### `.reserveThemes(max_themes)`

> Reserve space in EEPROM for `max_themes`. Each key on a theme uses half a byte
> of space. The function returns the `theme_base` to be used with the rest of
> the methods.
>
> The `theme_base` is a pointer into the EEPROM where the theme storage starts.

### `.updateHandler(theme_base, theme)`

> A helper we can call in our plugin's `.update()` method: given an EEPROM
> location (`theme_base`), and a `theme` index, it will update the keyboard with
> the colors of the specified theme.
>
> The `theme` argument can be any index between zero and `max_themes`. How the
> plugin decides which theme to display depends entirely on the plugin.

### `.themeFocusHandler(command, expected_command, theme_base, max_themes)`

> To be used in a custom `Focus` handler: handles the `expected_command` Focus
> command, and provides a way to query and update the themes supported by the
> plugin.
>
> When queried, it will list the color indexes. When used as a setter, it
> expects one index per key.
>
> The palette can be set via the `palette` focus command, implemented by the
> `FOCUS_HOOK_LEDPALETTETHEME` hook explained below.

## Focus commands

The plugin provides a single `Focus` hook, `FOCUS_HOOK_LEDPALETTETHEME`,
implementing the following command:

### `palette`

> Without arguments, prints the palette: RGB values for all 16 colors.
>
> With arguments, updates the palette with new colors. One does not need to give
> the full palette, the plugin will process as many arguments as available, and
> ignore anything past the last index. It expects colors to have all three
> components specified, or none at all. Thus, partial palette updates are
> possible, but only on the color level, not at component level.

## Dependencies

* [Kaleidoscope-EEPROM-Settings](https://github.com/keyboardio/Kaleidoscope-EEPROM-Settings)
* [Kaleidoscope-Focus](https://github.com/keyboardio/Kaleidoscope-Focus)
* [Kaleidoscope-LEDControl](https://github.com/keyboardio/Kaleidoscope-LEDControl)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

  [plugin:example]: https://github.com/keyboardio/Kaleidoscope-LED-Palette-Theme/blob/master/examples/LED-Palette-Theme/LED-Palette-Theme.ino
