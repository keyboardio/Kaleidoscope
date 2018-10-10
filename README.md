# Kaleidoscope-EEPROM-Keymap

[![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-EEPROM-Keymap.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-EEPROM-Keymap

While keyboards usually ship with a keymap programmed in, to be able to change that keymap, without flashing new firmware, we need a way to place the keymap into a place we can update at run-time, and which persists across reboots. Fortunately, we have a bit of `EEPROM` on the keyboard, and can use it to store either the full keymap (and saving space in the firmware then), or store additional layers there.

In short, this plugin allows us to change our keymaps, without having to compile and flash new firmware. It does so through the use of the [FocusSerial][plugin:focusSerial] plugin.

 [plugin:focusSerial]: https://github.com/keyboardio/Kaleidoscope-FocusSerial

## Using the plugin

Using the plugin is reasonably simple: after including the header, enable the plugin, and configure how many layers at most we want to store in `EEPROM`. There are other settings one can tweak, but these two steps are enough to get started with.

Once these are set up, we can update the keymap via [Focus][plugin:focusSerial].

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-EEPROM-Keymap.h>
#include <Kaleidoscope-FocusSerial.h>

KALEIDOSCOPE_INIT_PLUGINS(EEPROMKeymap,
                          Focus);

void setup() {
  Kaleidoscope.setup();

  EEPROMKeymap.setup(1);
}
```

## Plugin methods

The plugin provides the `EEPROMKeymap` object, which has the following method:

### `.setup(layers[, mode])`

> Reserve space in EEPROM for up to `layers` layers, and set things up to work according to the specified `mode` (see below for a list of supported modes). To be called from the `setup` method of one's sketch.
>
> Supported modes are:
> - `EEPROMKeymap.Mode::EXTEND`: Extend the keymap with layers from EEPROM, treating them as extensions of the main keymap embedded in the firmware. The first layer in EEPROM will have a number one higher than the last layer in PROGMEM. In this case, the total number of layers will be the number of them in PROGMEM plus `layers`.
> - `EEPROMKeymap.Mode::CUSTOM`: For advanced use cases where the `EXTEND` mode is not appropriate. In this case, the plugin merely reserves a slice of EEPROM for the requested amount of layers, but does no other configuration - that's entirely up to the Sketch.

## Focus commands

The plugin provides the `keymap.map` and a `keymap.roLayers` commands.

### `keymap.map [codes...]`

> Without arguments, displays the keymap currently in effect. Each key is printed as its raw, 16-bit keycode.
>
> With arguments, it stores as many keys as given. One does not need to set all keys, on all layers: the command will start from the first key on the first layer, and go on as long as it has input. It will not go past the total amount of layers (that is, `layer_count`).

### `keymap.roLayers`

> Returns the number of read-only layers. This only makes sense for the `EEPROMKeymap.Mode::EXTEND` mode, where it returns the number of layers in PROGMEM. In any other case, it doesn't return anything, doing so is left for another event handler that understands what the correct value would be.

## Dependencies

* [Kaleidoscope-EEPROM-Settings](https://github.com/keyboardio/Kaleidoscope-EEPROM-Settings)
* [Kaleidoscope-FocusSerial](https://github.com/keyboardio/Kaleidoscope-FocusSerial)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting started with the plugin.

  [plugin:example]: https://github.com/keyboardio/Kaleidoscope-EEPROM-Keymap/blob/master/examples/EEPROM-Keymap/EEPROM-Keymap.ino
