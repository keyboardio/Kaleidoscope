# FingerPainter

The `FingerPainter` plugin provides an elaborate `LED` mode, in which one's able
to paint with their fingers: when edit mode is toggled on, keys will - instead
of performing their normal function - cycle through the global palette - as
provided by the [LED-Palette-Theme][plugin:l-p-t] plugin -, one by one for each tap.

This allows us to edit the theme with the keyboard only, without any special
software (except to toggle edit mode on and off).

## Using the plugin

To use the plugin, just include the header, add it to the list of used plugins.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LED-Palette-Theme.h>
#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-FingerPainter.h>
#include <Kaleidoscope-FocusSerial.h>

KALEIDOSCOPE_INIT_PLUGINS(LEDControl,
                          EEPromSettings,
                          LEDPaletteTheme,
                          FingerPainter,
                          Focus);

void setup() {
  Kaleidoscope.setup();
}
```

## Plugin methods

The plugin provides the `FingerPainter` object, which provides no public methods.

## Focus commands

### `fingerpainter.clear`

> Clears the canvas, so that one can start a new painting.

### `fingerpainter.toggle`

> Toggles the painting mode on and off.

## Dependencies

* [Kaleidoscope-EEPROM-Settings](EEPROM-Settings.md)
* [Kaleidoscope-FocusSerial](FocusSerial.md)
* [Kaleidoscope-LED-Palette-Theme][plugin:l-p-t]
* [Kaleidoscope-LEDControl](LEDControl.md)

  [plugin:l-p-t]: LED-Palette-Theme.md

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

  [plugin:example]: /examples/LEDs/FingerPainter/FingerPainter.ino
