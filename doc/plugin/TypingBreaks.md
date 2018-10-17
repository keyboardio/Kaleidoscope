# Kaleidoscope-TypingBreaks

Typing on the keyboard for an extended period of time may lead to injuries,
which is why it is highly recommended to take frequent breaks from the
keyboard - and from the computer as well. But sometimes - more often than one
would wish to admit - we tend to forget about this, and plow through, at the
cost of hand's health.

No more.

With the `TypingBreaks` plugin, we can instruct the keyboard to lock itself up
after some time, or after a number of key presses. It will stay locked for a few
minutes (or whatever amount we told it to), forcing us to take a break.

## Using the plugin

The plugin comes with reasonable defaults (see below), and can be used out of
the box, without any further configuration:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-TypingBreaks.h>

KALEIDOSCOPE_INIT_PLUGINS(EEPROMSettings, TypingBreaks);

void setup (void) {
  Kaleidoscope.setup ();

  TypingBreaks.settings.idle_time_limit = 60;
}
```

## Plugin methods

The plugin provides a single object, `TypingBreaks`, with the following
properties. All times are in seconds.

### `.settings.idle_time_limit`

> The amount of time that can pass between two pressed keys, before the plugin
> considers it a new session, and starts all timers and counters over.
>
> Defaults to 300 seconds (5 minutes).

### `.settings.lock_time_out`

> The length of the session, after which the keyboard will be locked.
>
> Defaults to 2700 seconds (45 minutes).

### `.settings.lock_length`

> The length until the keyboard lock is held. Any key pressed while the lock is
> active, will be discarded.
>
> Defaults to 300 seconds (5 minutes).

### `.settings.left_hand_max_keys`

> It is possible to lock the keyboard after a number of keys pressed, too. If
> this happens sooner than the timeout, the keyboard will still be locked.
>
> This property controls how many keys can be pressed on the left side.
>
> Defaults to 0 (off).

### `.settings.right_hand_max_keys`

> It is possible to lock the keyboard after a number of keys pressed, too. If
> this happens sooner than the timeout, the keyboard will still be locked.
>
> This property controls how many keys can be pressed on the right side.
>
> Defaults to 0 (off).

## Focus commands

### `typingbreaks.idleTimeLimit [limit]`

> Get or set the `.settings.idle_time_limit` property.

### `typingbreaks.lockTimeOut [time_out]`

> Get or set the `.settings.lock_time_out` property.

### `typingbreaks.lockLength [length]`

> Get or set the `.settings.lock_length` property.

### `typingbreaks.leftMaxKeys [max]`

> Get or set the `.settings.left_hand_max_keys` property.

### `typingbreaks.rightMaxKeys [max]`

> Get or set the `.settings.right_hand_max_keys` property.

## Dependencies

* [Kaleidoscope-EEPROM-Settings](EEPROM-Settings.md)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: ../../examples/TypingBreaks/TypingBreaks.ino

## Upgrading

Older versions of the plugin used to provide EEPROM storage for the settings
only optionally, when it was explicitly enabled via the
`TypingBreaks.enableEEPROM()` method. Similarly, the Focus hooks were optional
too.

Both of them are unconditionally enabled now, because they add so much to the
plugin. This means that any calls to `TypingBreaks.enableEEPROM()` can be safely
removed, the method is a no-op by now.

Storing the settable settings in EEPROM makes it depend on
`Kaleidoscope-EEPROM-Settings`, which should be initialized before this plugin
is.
