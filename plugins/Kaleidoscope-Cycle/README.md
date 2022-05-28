# Cycle

If you ever wanted a key that works like keys on old cell phones, when you press
a key and it cycles through a number of options in a sequence, then the cycling
key is what you are looking for. It is a bit different than on cell phones of
old, as it is a separate key, that works in combination of other keys: you press
a key, then the cycle key, and the cycle key will replace the previously input
symbol with another. Keep tapping the cycle key, and it will replace symbols
with new ones, in a loop.

## Using the plugin

To use the plugin, we need to include the header, and declare the behaviour
used. Then, we need to place a cycle key or two on the keymap. And finally, we
need to implement the [`cycleAction`][cycleaction] function that gets called
each time the cycling key triggers.

 [cycleaction]: #cycleactionpreviouskey-cyclecount

```c++
#include <Kaleidoscope-Cycle.h>

// Somewhere in the keymap:
Key_Cycle

// later in the Sketch:
void cycleAction(Key previous_key, uint8_t cycle_count) {
  bool is_shifted = previous_key.getFlags() & SHIFT_HELD;
  if (previous_key.getKeyCode() == Key_A.getKeyCode() && is_shifted)
      cycleThrough (LSHIFT(Key_A), LSHIFT(Key_B), LSHIFT(Key_C));
  if (previous_key.getKeyCode() == Key_A.getKeyCode() && !is_shifted)
      cycleThrough (Key_A, Key_B, Key_C);
}

KALEIDOSCOPE_INIT_PLUGINS(Cycle);

void setup() {
  Kaleidoscope.setup();
}
```

## Keymap markup

### `Key_Cycle`

> The key code for the cycle key. There can be as many of this on the keymap, as
> many one wants, but they all behave the same. There is little point in having
> more than one on each side.

## Plugin methods

The plugin provides a `Cycle` object, but to implement the actions, we need to
define a function ([`cycleAction`][cycleaction]) outside of the object. A
handler, of sorts. The object also provides a helper method to replace the
previous symbol with another. The plugin also provides one macro that is
particularly useful, and in most cases, should be used over the `.replace()`
method explained below.

### `cycleThrough(keys...)`

> Cycles through all the possibilities given in `keys` (starting from the
> beginning once it reached the end). This should be used from
> the [`cycleAction`][cycleaction] function, once it is determined what sequence
> to cycle through.
>
> To make the cycling loop complete, the first element of the `keys` list should
> be the one that - when followed by the Cycle key - triggers the action.

### `.replace(key)`

> Deletes the previous symbol (by sending a `Backspace`), and inputs the new
> one. This is used by `cycleThrough()` above, behind the scenes.
>
> The recommended method is to use the macro, but in special circumstances, this
> function can be of direct use as well.

## Overrideable methods

### `cycleAction(previous_key, cycle_count)`

> The heart and soul of the plugin, that must be defined in the Sketch. It will
> be called whenever the cycling key triggers, and the two arguments are the
> last key pressed (not counting repeated taps of the cycling key itself), and
> the number of times the cycling key has been pressed.
>
> It is up to us to decide what to do, and when. But the most common - and
> expected - action is to call `cycleThrough()` with a different sequence for
> each key we want to use together with the cycling key.

## Dependencies

* [Kaleidoscope-Ranges](Kaleidoscope-Ranges.md)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: /examples/Keystrokes/Cycle/Cycle.ino
