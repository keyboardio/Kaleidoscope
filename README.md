# Kaleidoscope-EEPROM-Keymap

![status][st:stable] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-EEPROM-Keymap.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-EEPROM-Keymap

 [st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

While keyboards usually ship with a keymap programmed in, to be able to change
that keymap, without flashing new firmware, we need a way to place the keymap
into a place we can update at run-time, and which persists across reboots.
Fortunately, we have a bit of `EEPROM` on the keyboard, and can use it to store
either the full keymap (and saving space in the firmware then), or store an
overlay there. In the latter case, whenever there is a non-transparent key on
the overlay, we will use that instead of the keyboard default.

In short, this plugin allows us to change our keymaps, without having to compile
and flash new firmware. It does so through the use of the
[FocusSerial][plugin:focusSerial] plugin.

 [plugin:focusSerial]: https://github.com/keyboardio/Kaleidoscope-FocusSerial

## Using the plugin

Using the plugin is reasonably simple: after including the header, enable the
plugin, and configure how many layers at most we want to store in `EEPROM`.
There are other settings one can tweak, but these two steps are enough to get
started with.

Once these are set up, we can update the keymap via [Focus][plugin:focusSerial].

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-EEPROM-Keymap.h>
#include <Kaleidoscope-FocusSerial.h>

KALEIDOSCOPE_INIT_PLUGINS(EEPROMKeymap,
                          Focus,
                          FocusKeymapTransferCommand);

void setup() {
  Kaleidoscope.setup();

  EEPROMKeymap.max_layers(1);
}
```

## Plugin methods

The plugin provides the `EEPROMKeymap` object, which has the following methods:

### `.max_layers(max)`

> Tells the extension to reserve space in EEPROM for up to `max` layers. Can
> only be called once, any subsequent call will be a no-op.
> This should be set to the number of keymap layers you want to be
> able to program from EEPROM (probably the number of layers you have
> defined in your keymap).

## Focus commands

The plugin provides a `keymap.map` Focus command unconditionally, and a
`keymap.transfer` via the `FocusKeymapTransferCommand` object.

### `keymap.map [codes...]`

> Without arguments, displays the keymap currently in effect. Each key is
> printed as its raw, 16-bit keycode.
>
> With arguments, it stores as many keys as given. One does not need to set all
> keys, on all layers: the command will start from the first key on the first
> layer, and go on as long as it has input. It will not go past the layer set
> via the `.max_layers()` method.

### `keymap.transfer LAYER`

> Transfers the `LAYER` from the built-in memory of the keyboard into `EEPROM`
> storage.
>
> Useful mostly when one wants to remove the built-in keymap, and wants to
> easily transfer it into `EEPROM` first.
>
> This is generally not needed, and it is recommended to not enable this
> command, unless the feature this command implements is truly needed.

## Dependencies

* [Kaleidoscope-EEPROM-Settings](https://github.com/keyboardio/Kaleidoscope-EEPROM-Settings)
* [Kaleidoscope-FocusSerial](https://github.com/keyboardio/Kaleidoscope-FocusSerial)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

  [plugin:example]: https://github.com/keyboardio/Kaleidoscope-EEPROM-Keymap/blob/master/examples/EEPROM-Keymap/EEPROM-Keymap.ino
