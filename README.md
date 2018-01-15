# Kaleidoscope-Qukeys

![status][st:experimental] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/gedankenlab/Kaleidoscope-Qukeys.svg?branch=master
 [travis:status]: https://travis-ci.org/gedankenlab/Kaleidoscope-Qukeys

 [st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

## Concept

This Kaleidoscope plugin allows you to overload keys on your keyboard so that they produce
one keycode (i.e. symbol) when tapped, and a different keycode -- most likely a modifier
(e.g. `shift` or `alt`) -- when held.


## Setup

- Clone the module -- In your sketch directory (e.g. `Model01-Firmware/`:
```
git submodule add Kaleidoscope-Qukeys https://github.com/gedankenlab/Kaleidoscope-Qukeys.git
```
- Include the header file:
```
#include <Kaleidoscope-Qukeys.h>
```
- Use the plugin in the `setup()` function:
```
Kaleidoscope.use(&Qukeys);
```
- Define some `Qukeys`:
```
QUKEYS(
  kaleidoscope::Qukey(0, 2, 1, Key_LeftGui),      // A/cmd
  kaleidoscope::Qukey(0, 2, 2, Key_LeftAlt),      // S/alt
  kaleidoscope::Qukey(0, 2, 3, Key_LeftControl),  // D/ctrl
  kaleidoscope::Qukey(0, 2, 4, Key_LeftShift)     // F/shift
)
```

`Qukeys` will work best if it's the first plugin in the `use()` list, because when typing
overlap occurs, it will (temporarily) mask keys and block them from being processed by
other plugins. If those other plugins handle the keypress events first, it may not work as
expected. It doesn't _need_ to be first, but if it's `use()`'d after another plugin that
handles typing events, especially one that sends extra keyboard HID reports, it is more
likely to generate errors and out-of-order events.


## Configuration

- set timeout

- activate/deactivate `Qukeys`

- see the
  [example](https://github.com/gedankenlab/Kaleidoscope-Qukeys/blob/master/examples/Qukeys/Qukeys.ino)
  for a way to turn `Qukeys` on and off, using Kaleidoscope-Macros


## Design & Implementation

When a `Qukey` is pressed, it doesn't immediately add a corresponding keycode to the HID
report; it adds that key to a queue, and waits until one of three things happens:

1. a time limit is reached

2. the `Qukey` is released

3. a subsequently-pressed key is released

Until one of those conditions is met, all subsequent keypresses are simply added to the
queue, and no new reports are sent to the host. Once a condition is met, the `Qukey` is
flushed from the queue, and so are any subsequent keypresses (up to, but not including,
the next `Qukey` that is still pressed).

Basically, if you hold the `Qukey`, then press and release some other key, you'll get the
alternate keycode (probably a modifier) for the `Qukey`, even if you don't wait for a
timeout. If you're typing quickly, and there's some overlap between two keypresses, you
won't get the alternate keycode, and the keys will be reported in the order that they were
pressed -- as long as the keys are released in the same order they were pressed.

The time limit is mainly there so that a `Qukey` can be used as a modifier (in its
alternate state) with a second input device (e.g. a mouse). It can be quite short (200ms
is probably short enough) -- as long as your "taps" while typing are shorter than the time
limit, you won't get any unintended alternate keycodes.
