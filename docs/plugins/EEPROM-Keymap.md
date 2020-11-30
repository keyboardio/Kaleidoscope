# EEPROM-Keymap

While keyboards usually ship with a keymap programmed in, to be able to change that keymap, without flashing new firmware, we need a way to place the keymap into a place we can update at run-time, and which persists across reboots. Fortunately, we have a bit of `EEPROM` on the keyboard, and can use it to store either the full keymap (and saving space in the firmware then), or store additional layers there.

In short, this plugin allows us to change our keymaps, without having to compile and flash new firmware. It does so through the use of the [FocusSerial][plugin:focusSerial] plugin.

 [plugin:focusSerial]: FocusSerial.md

By default, the plugin extends the keymap in PROGMEM: it will only look for keys in EEPROM if looking up from a layer that's higher than the last one in PROGMEM. This behaviour can be changed either via `Focus` (see below), or by calling `EEPROMSettings.use_eeprom_layers_only` (see the [EEPROMSettings](EEPROM-Settings.md) documentation for more information).

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

### `.setup(layers)`

> Reserve space in EEPROM for up to `layers` layers, and set up the key lookup mechanism.

## Focus commands

The plugin provides three Focus commands: `keymap.default`, `keymap.custom`, and `keymap.useCustom`.

### `keymap.default`

> Display the default keymap from PROGMEM. Each key is printed as its raw, 16-bit keycode.
>
> Unlike `keymap.custom`, this does not support updating, because PROGMEM is read-only.

### `keymap.custom [codes...]`

> Without arguments, display the custom keymap stored in EEPROM. Each key is printed as its raw, 16-bit keycode.
>
> With arguments, it updates as many keys as given. One does not need to set all keys, on all layers: the command will start from the first key on the first layer (in EEPROM, which might be different than the first layer!), and go on as long as it has input. It will not go past the number of layers in EEPROM.

### `keymap.onlyCustom [0|1]`

> Without arguments, returns whether the firmware uses both the default and the custom layers (the default, `0`) or custom (EEPROM-stored) layers only (`1`).
>
> With an argument, sets whether to use custom layers only, or extend the built-in layers instead.

## Dependencies

* [Kaleidoscope-EEPROM-Settings](EEPROM-Settings.md)
* [Kaleidoscope-FocusSerial](FocusSerial.md)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting started with the plugin.

  [plugin:example]: /examples/Features/EEPROM/EEPROM-Keymap/EEPROM-Keymap.ino
