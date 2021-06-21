# DynamicTapDance

The `DynamicTapDance` plugin allows one to set up [TapDance][plugin:tapdance] keys
without the need to compile and flash new firmware: one can change dynamic
dances via [Focus][plugin:focus], using a tool like [Chrysalis][chrysalis].

 [plugin:tapdance]: Kaleidoscope-TapDance.md
 [plugin:focus]: Kaleidoscope-FocusSerial.md
 [chrysalis]: https://github.com/keyboardio/Chrysalis

Dynamic dances come with certain limitations, however: unlike the built-in ones,
dynamic ones do not support running custom code. They can only choose a key from
a list of possibilities. Given a list of keys, the plugin will choose the one
corresponding to the number of taps on the key, just like `TapDance` itself does.

Basically, this plugin allows us to store `tapDanceActionKeys` key lists in the
on-board memory of our keyboard.

You can define up to 16 dynamic dances, there is no limit on their length,
except the amount of storage available on the keyboard. You can even mix them
with built-in dances! But the total number of tap-dances is 16.

## Using the plugin

To use the plugin, we need to include the header, tell the firmware to `use` the
plugin, and reserve storage space for the dances. This is best illustrated with
an example:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-EEPROMSettings.h>
#include <Kaleidoscope-FocusSerial.h>
#include <Kaleidoscope-TapDance.h>
#include <Kaleidoscope-DynamicTapDance.h>

KALEIDOSCOPE_INIT_PLUGINS(
  EEPROMSettings,
  Focus,
  TapDance,
  DynamicTapDance
);

void tapDanceAction(uint8_t tap_dance_index, KeyAddr key_addr, uint8_t tap_count, kaleidoscope::plugin::TapDance::ActionType tap_dance_action) {
  DynamicTapDance.dance(tap_dance_index, key_addr, tap_count, tap_dance_action);
}

void setup() {
  Kaleidoscope.setup();

  // 0 is the amount of built-in dances we have.
  // 128 is how much space (in bytes) we reserve for dances.
  DynamicTapDance.setup(0, 128);
}
```

## Plugin methods

The plugin provides a `DynamicTapDance` object, with the following methods and properties available:

### `.setup(builtin_dances, size)`

> Reserves `size` bytes of storage for dynamic dances. This must be called from
> the `setup()` method of your sketch, otherwise dynamic tap-dances will not
> function.
>
> The `builtin_dances` argument tells the plugin how many built-in dances there
> are.

### `.dance(index, key_addr, tap_count, tap_dance_action)`

> Performs a given dance (`index`) made on the key at `key_addr` address, which
> has been tapped `tap_count` times, and the action to perform is
> `tap_dance_action`.
>
> This mirrors the overrideable `tapDanceAction()` method of
> [TapDance][plugin:tapdance], and is intended to be called from therein.

## Focus commands

The plugin provides one Focus command: `tapdance.map`.

### `tapdance.map [dances...]`

> Without arguments, displays all the stored dances. Each dance is terminated by
> an end marker (`0`, aka `Key_NoKey`), and the last dance is followed by an
> additional marker. The plugin will send back the entire dynamic tap-dance
> storage space, even data after the final marker.
>
> With arguments, it replaces the current set of dynamic dances with the newly
> given ones. Dances are terminated by an end marker, and the last dance must be
> terminated by an additional one. It is up to the caller to make sure these
> rules are obeyed.
>
> In both cases, the data sent or expected is a sequence of 16-bit values, a
> memory dump.

## Dependencies

* [Kaleidoscope-EEPROM-Settings](Kaleidoscope-EEPROM-Settings.md)
* [Kaleidoscope-FocusSerial](Kaleidoscope-FocusSerial.md)
* [Kaleidoscope-TapDance](Kaleidoscope-TapDance.md)
