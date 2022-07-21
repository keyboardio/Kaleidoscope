# LayerNames

This plugin provides a [Focus][plugin:focus]-based interface for storing custom
layer names, to be used by software such as [Chrysalis][chrysalis]. The firmware
itself does nothing with the layer names, it is purely for use by applications
on the host side.

 [chrysalis]: https://github.com/keyboardio/Chrysalis

## Using the plugin

To use the plugin, we need to include the header, initialize the plugin with
`KALEIDOSCOPE_INIT_PLUGINS()`, and reserve storage space for the names. This is
best illustrated with an example:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-EEPROMSettings.h>
#include <Kaleidoscope-FocusSerial.h>
#include <Kaleidoscope-LayerNames.h>

KALEIDOSCOPE_INIT_PLUGINS(
  EEPROMSettings,
  Focus,
  LayerNames
);

void setup() {
  Kaleidoscope.setup();

  LayerNames.reserve_storage(128);
}
```

## Plugin methods

The plugin provides a `LayerNames` object, with the following method available:

### `.reserve_storage(size)`

> Reserves `size` bytes of storage for layer names. This must be called from the
> `setup()` method of your sketch.

## Focus commands

The plugin provides a single Focus command: `keymap.layerNames`.

### `keymap.layerNames [name_length name]...`

> Without arguments, displays all the stored layer names. Each layer is printed
> on its own line, preceded by its length. At the end, the plugin will also
> print an extra line with a name length of zero, followed by the string
> "size=", and then the total size of the storage reserved for layer names.
>
> To set custom names, a list of length & name pairs must be given. The plugin
> stops processing arguments when it encounters a name length of 0.

#### Example

```
> keymap.layerNames
< 6 Qwerty
< 6 Numpad
< 8 Function
< 0 size=128
< .

> keymap.layerNames 6 Dvorak 6 Numpad 8 Function 0
< .
```

## Dependencies

* [Kaleidoscope-EEPROM-Settings](Kaleidoscope-EEPROM-Settings.md)
* [Kaleidoscope-FocusSerial](Kaleidoscope-FocusSerial.md)
