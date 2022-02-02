# DynamicMacros

Dynamic macros are similar to [Macros][plugin:macros], but unlike them, they can
be re-defined without compiling and flashing new firmware: one can change
dynamic macros via [Focus][plugin:focus], using a tool like
[Chrysalis][chrysalis].

 [plugin:macros]: Kaleidoscope-Macros.md
 [plugin:focus]: Kaleidoscope-FocusSerial.md
 [chrysalis]: https://github.com/keyboardio/Chrysalis

Dynamic macros come with certain limitations, however: unlike the built-in
macros, dynamic ones do not support running custom code, they can only play back
a sequence of events (keys, mousekeys, etc), and do so whenever one presses the
dynamic macro key.

You can define up to 32 dynamic macros, there is no limit on their length,
except the amount of storage available on the keyboard.

## Using the plugin

To use the plugin, we need to include the header, tell the firmware to `use` the
plugin, and reserve storage space for the macros. This is best illustrated with
an example:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-EEPROMSettings.h>
#include <Kaleidoscope-FocusSerial.h>
#include <Kaleidoscope-DynamicMacros.h>

KALEIDOSCOPE_INIT_PLUGINS(
  EEPROMSettings,
  Focus,
  DynamicMacros
);

void setup() {
  Kaleidoscope.setup();

  DynamicMacros.reserve_storage(128);
}
```

## Keymap markup

### `DM(id)`

> Places a dynamic macro key on the keymap, with the `id` number (0 to 31) as
> identifier. Pressing the key will immediately run the associated dynamic
> macro.

## Plugin methods

The plugin provides a `DynamicMacros` object, with the following methods and properties available:

### `.reserve_storage(size)`

> Reserves `size` bytes of storage for dynamic macros. This must be called from
> the `setup()` method of your sketch, otherwise dynamic macros will not
> function. Two bytes of this are used for internal tracking, the remainder is used
> for your macro map.

### `.play(macro_id)`

> Plays back a macro, specified by `macro_id`.

## `MACRO` steps

The plugin supports the same [macro steps][doc:steps] as the Macros plugin,
please refer to the documentation therein.

 [doc:steps]: (Kaleidoscope-Macros.md#macro-steps)

## Focus commands

The plugin provides the following Focus commands:

### `macros.map [macros...]`

> Without arguments, displays all the stored macros. Each macro is terminated by
> an end marker (`MACRO_ACTION_END`). The plugin will send back only the used
> dynamic macro storage space.

> With arguments, it replaces the current set of dynamic macros with the newly
> given ones. Macros are terminated by an end marker.

> In both cases, the data sent or expected is a sequence of 8-bit values, a
> memory dump.

### `macros.appendMap [macros...]`

> Append to the current macro map. This is provided to allow for macro maps that
> exceed the serial input buffer for a single line.

### `macros.reset`

> Clear all previously defined macros.

### `macros.trigger macro_id`

> Runs the dynamic macro associated with `macro_id` immediately. This can be
> used to test macros without having to place them on the keymap.

## Dependencies

* [Kaleidoscope-EEPROM-Settings](Kaleidoscope-EEPROM-Settings.md)
* [Kaleidoscope-FocusSerial](Kaleidoscope-FocusSerial.md)
