# CustomKeyLabels

This plugin provides a [Focus][plugin:focus]-based interface for storing custom
key labels, to be used by software such as [Chrysalis][chrysalis]. The firmware
itself does nothing with the labels, it is purely for use by applications on the
host side.

 [chrysalis]: https://github.com/keyboardio/Chrysalis

## Using the plugin

To use the plugin, we need to include the header, initialize the plugin with
`KALEIDOSCOPE_INIT_PLUGINS()`, and reserve storage space for the labels. This is
best illustrated with an example:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-EEPROMSettings.h>
#include <Kaleidoscope-FocusSerial.h>
#include <Kaleidoscope-CustomKeyLabels.h>

KALEIDOSCOPE_INIT_PLUGINS(
  EEPROMSettings,
  Focus,
  CustomKeyLabels
);

void setup() {
  Kaleidoscope.setup();

  CustomKeyLabels.reserve_storage(1024);
}
```

## Plugin methods

The plugin provides a `CustomKeyLabels` object, with the following method available:

### `.reserve_storage(size)`

> Reserves `size` bytes of storage for custom key labels. This must be called
> from the `setup()` method of your sketch.

## Focus commands

The plugin provides a single Focus command: `keymap.labels`.

### `keymap.labels [name_length key_code labelJSON]...`

> Without arguments, displays all the stored custom key labels. Each label is
> printed on its own line, preceded by its length and the key code the label is
> for. At the end the plugin will also print an extra line with name length and
> key code set to zero, followed by the string "size=", and then the total size
> of the storage reserved for custom labels.
>
> To set custom labels, a list of length, keycode & label triplets must be
> given. The plugin stops processing arguments when it encounters a name length
> of 0.

#### Example

```
> keymap.labels
< 14 4 {"base":"foo"}
< 0 0 size=1024
< .

> keymap.labels 14 4 {"base":"foo"}
< .
```

## Dependencies

* [Kaleidoscope-EEPROM-Settings](Kaleidoscope-EEPROM-Settings.md)
* [Kaleidoscope-FocusSerial](Kaleidoscope-FocusSerial.md)
