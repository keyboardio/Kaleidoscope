# Escape-OneShot

Turn the `Esc` key into a special key, that can cancel any active `OneShot`
effect - or act as the normal `Esc` key if none are active, or if any of them
are still held. For those times when one accidentally presses a one-shot key, or
change their minds.

Additionally, the special `Key_OneShotCancel` key will also count as a oneshot
cancel key, would one want a dedicated key for the purpose.

## Using the plugin

To use the plugin, one needs to include the header, and activate it. No further
configuration is necessary.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-OneShot.h>
#include <Kaleidoscope-Escape-OneShot.h>

KALEIDOSCOPE_INIT_PLUGINS(OneShot,
                          EscapeOneShot);

void setup () {
  Kaleidoscope.setup ();
}
```

If one wishes to configure the plugin at run-time via [Focus][plugin:focus], the
optional `EscapeOneShotConfig` plugin must also be enabled:

 [plugin:focus]: Kaleidoscope-FocusSerial.md

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-FocusSerial.h>
#include <Kaleidoscope-OneShot.h>
#include <Kaleidoscope-Escape-OneShot.h>

KALEIDOSCOPE_INIT_PLUGINS(EEPROMSettings,
                          Focus,
                          OneShot,
                          EscapeOneShot,
                          EscapeOneShotConfig);

void setup () {
  Kaleidoscope.setup ();
}
```

The plugin only makes sense when using one-shot keys.

## Plugin methods

The plugin provides the `EscapeOneShot` object, which has the following public
configuration methods:

### `.setCancelKey(key)`

> Changes the `Key` value that will trigger deactivation of one-shot
> (including sticky) keys. The default is to use `Key_Escape` (the
> normal `Esc` key), but if you would rather have a dedicated key (so
> that you can use `Key_Escape` in combination with one-shot
> modifiers), there is the special `Key_OneShotCancel`, which will not
> have any side effects.

### `.getCancelKey(key)`

> Returns the `Key` value that will trigger deactivation of one-shot (including
> sticky) keys.

## Focus commands

The plugin provides a single Focus command: `escape_oneshot.cancel_key`.

### `escape_oneshot.cancel_key [keycode]`

> Without an argument, returns the raw 16-bit keycode of the cancel key set.
>
> With an argument - a raw 16-bit keycode -, sets the cancel key to the one
> corresponding to the given code.

## Dependencies

* [Kaleidoscope-OneShot](Kaleidoscope-OneShot.md)

### Optional dependencies

* [Kaleidoscope-EEPROM-Settings](Kaleidoscope-EEPROM-Settings.md)
* [Kaleidoscope-FocusSerial](Kaleidoscope-FocusSerial.md)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: /examples/Keystrokes/Escape-OneShot/Escape-OneShot.ino
