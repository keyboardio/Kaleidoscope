# Kaleidoscope-TypingBreaks

![status][st:experimental] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-TypingBreaks.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-TypingBreaks

 [st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

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
#include <Kaleidoscope-TypingBreaks.h>

void setup (void) {
  Kaleidoscope.use (&TypingBreaks);

  Kaleidoscope.setup ();
}
```

## Plugin methods

The plugin provides a single object, `TypingBreaks`, with the following
properties. All times are in milliseconds.

### `.settings.idle_time_limit`

> The amount of time that can pass between two pressed keys, before the plugin
> considers it a new session, and starts all timers and counters over.
>
> Defaults to 10000 (10 seconds).

### `.settings.lock_time_out`

> The length of the session, after which the keyboard will be locked.
>
> Defaults to 2700000 (45 minutes).

### `.settings.lock_length`

> The length until the keyboard lock is held. Any key pressed while the lock is
> active, will be discarded.
>
> Defaults to 300000 (15 minutes).

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

The plugin provides a single `Focus` hook, `FOCUS_HOOK_TYPINGBREAKS`, which in
turn make a few commands available. All of these return the respective setting's
value when called without arguments, or set them if called with some.

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

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/keyboardio/Kaleidoscope-TypingBreaks/blob/master/examples/TypingBreaks/TypingBreaks.ino
