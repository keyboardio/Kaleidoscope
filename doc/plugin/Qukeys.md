# Kaleidoscope-Qukeys

## Concept

This Kaleidoscope plugin allows you to overload keys on your keyboard so that they produce
one keycode (i.e. symbol) when tapped, and a different keycode -- most likely a modifier
(e.g. `shift` or `alt`) -- when held.


## Setup

- Include the header file:
```
#include <Kaleidoscope-Qukeys.h>
```
- Use the plugin in the `KALEIDOSCOPE_INIT_PLUGINS` macro:
```
KALEIDOSCOPE_INIT_PLUGINS(Qukeys);
```

- Define some `Qukeys` of the format `Qukey(layer, row, col, alt_keycode)`
  (layers, rows and columns are all zero-indexed, rows are top to bottom and
  columns are left to right):

- For the Keyboardio Model 01, key coordinates refer to [this header
  file](https://github.com/keyboardio/Kaleidoscope-Hardware-Model01/blob/f469015346535cb864a340bf8eb317d268943248/src/Kaleidoscope-Hardware-Model01.h#L267-L279).

```
QUKEYS(
  //                  l, r, c, alt_keycode
  kaleidoscope::plugin::Qukey(0, 2, 1, Key_LeftGui),      // A/cmd
  kaleidoscope::plugin::Qukey(0, 2, 2, Key_LeftAlt),      // S/alt
  kaleidoscope::plugin::Qukey(0, 2, 3, Key_LeftControl),  // D/ctrl
  kaleidoscope::plugin::Qukey(0, 2, 4, Key_LeftShift),    // F/shift
  kaleidoscope::plugin::Qukey(0, 1, 14, Key_LeftShift),   // P/shift
  kaleidoscope::plugin::Qukey(0, 3, 15, Key_LeftShift)    // Minus/shift
)
```

`Qukeys` will work best if it's the first plugin in the `INIT()` list, because when typing
overlap occurs, it will (temporarily) mask keys and block them from being processed by
other plugins. If those other plugins handle the keypress events first, it may not work as
expected. It doesn't _need_ to be first, but if it's `INIT()`'d after another plugin that
handles typing events, especially one that sends extra keyboard HID reports, it is more
likely to generate errors and out-of-order events.


## Configuration

### `.setTimeout(time_limit)`

> Sets the time length in milliseconds which determines if a key has been tapped or held.
>
> Defaults to 250.

### `.setReleaseDelay(release_delay)`

> Sets the time length in milliseconds to artificially delay the release of the Qukey.
>
> This is to accommodate users who are in the habit of releasing modifiers and the keys
> they modify (almost) simultaneously, since the Qukey may be detected as released
> *slightly* before the other key, which would not trigger the desired alternate keycode.
>
> It is best to keep this a very small value such as 20 to avoid over-extending the
> modifier to further keystrokes.
>
> Defaults to 0.

### `.activate()`
### `.deactivate()`
### `.toggle()`

> activate/deactivate `Qukeys`

### DualUse key definitions

In addition to normal `Qukeys` described above, Kaleidoscope-Qukeys also treats
DualUse keys in the keymap as `Qukeys`. This makes `Qukeys` a drop-in replacement
for the `DualUse` plugin, without the need to edit the keymap.

The plugin provides a number of macros one can use in keymap definitions:

#### `CTL_T(key)`

> A key that acts as the *left* `Control` when held, or used in conjunction with
> other keys, but as `key` when tapped in isolation. The `key` argument must be
> a plain old key, and can't have any modifiers or anything else applied.

#### `ALT_T(key)`

> A key that acts as the *left* `Alt` when held, or used in conjunction with
> other keys, but as `key` when tapped in isolation. The `key` argument must be
> a plain old key, and can't have any modifiers or anything else applied.

#### `SFT_T(key)`

> A key that acts as the *left* `Shift` when held, or used in conjunction with
> other keys, but as `key` when tapped in isolation. The `key` argument must be
> a plain old key, and can't have any modifiers or anything else applied.

#### `GUI_T(key)`

> A key that acts as the *left* `GUI` when held, or used in conjunction with
> other keys, but as `key` when tapped in isolation. The `key` argument must be
> a plain old key, and can't have any modifiers or anything else applied.

#### `MT(mod, key)`

> A key that acts as `mod` when held, or used in conjunction with other keys,
> but as `key` when tapped in isolation. The `key` argument must be a plain old
> key, and can't have any modifiers or anything else applied. The `mod` argument
> can be any of the modifiers, *left* or *right* alike.

#### `LT(layer, key)`

> A key that momentarily switches to `layer` when held, or used in conjunction
> with other keys, but as `key` when tapped in isolation. The `key` argument
> must be a plain old key, and can't have any modifiers or anything else
> applied.

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

## Further reading

The [example][plugin:example] can help to learn how to use this plugin.

 [plugin:example]: ../../examples/Qukeys/Qukeys.ino
