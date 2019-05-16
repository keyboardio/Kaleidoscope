# Kaleidoscope-TapDance

Tap-dance keys are general purpose, multi-use keys, which trigger a different
action based on the number of times they were tapped in sequence. As an example
to make this clearer, one can have a key that inputs `A` when tapped once,
inputs `B` when tapped twice, and lights up the keyboard in Christmas colors
when tapped a third time.

This behaviour is most useful in cases where we have a number of things we
perform rarely, where tapping a single key repeatedly is not counter-productive.
Such cases include - for example - multimedia forward / backward keys: forward
on single tap, backward on double. Of course, one could use modifiers to achieve
a similar effect, but that's two keys to use, this is only one. We can also hide
some destructive functionality behind a number of taps: reset the keyboard after
4 taps, and light up LEDs in increasingly frightful colors until then.

## How does it work?

To not interfere with normal typing, tap-dance keys have two ways to decide when
to call an action: they either get interrupted, or they time out. Every time a
tap-dance key is pressed, the timer resets, so one does not have to finish the
whole tapping sequence within a short time limit. The tap-dance counter
continues incrementing until one of these cases happen.

When a tap-dance key is pressed and released, and nothing is pressed on the
keyboard until the timeout is reached, then the key will time out, and trigger
an action. Which action, depends on the number of times it has been tapped up
until this point.

When a tap-dance key is pressed and released, and another key is hit before the
timer expires, then the tap-dance key will trigger an action first, perform it,
and only then will the firmware continue handling the interrupting key press.
This is to preserve the order of keys pressed.

In both of these cases, the [`tapDanceAction`][tdaction] will be called, with
`tapDanceIndex` set to the index of the tap-dance action (as set in the keymap),
the `tapCount`, and `tapDanceAction` set to either
`kaleidoscope::plugin::TapDance::Interrupt`, or
`kaleidoscope::plugin::TapDance::Timeout`. If we continue holding the key, then
as long as it is held, the same function will be called with `tapDanceAction`
set to `kaleidoscope::plugin::TapDance::Hold`. When the key is released, after
either an `Interrupt` or `Timeout` action was triggered, the function will be
called with `tapDanceAction` set to `kaleidoscope::plugin::TapDance::Release`.

These actions allow us to create sophisticated tap-dance setups, where one can
tap a key twice and hold it, and have it repeat, for example.

There is one additional value the `tapDanceAction` parameter can take:
`kaleidoscope::plugin::TapDance::Tap`. It is called with this argument for each
and every tap, even if no action is to be triggered yet. This is so that we can
have a way to do some side-effects, like light up LEDs to show progress, and so
on.

## Using the plugin

To use the plugin, we need to include the header, and declare the behaviour
used. Then, we need to place tap-dance keys on the keymap. And finally, we need
to implement the [`tapDanceAction`][tdaction] function that gets called each
time an action is to be performed.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-TapDance.h>

// Somewhere in the keymap:
TD(0)

// later in the Sketch:
void tapDanceAction(uint8_t tap_dance_index, byte row, byte col, uint8_t tap_count,
                    kaleidoscope::plugin::TapDance::ActionType tap_dance_action) {
  switch (tap_dance_index) {
  case 0:
    return tapDanceActionKeys(tap_count, tap_dance_action,
                              Consumer_ScanNextTrack, Consumer_ScanPreviousTrack);
  }
}

KALEIDOSCOPE_INIT_PLUGINS(TapDance);

void setup() {
  Kaleidoscope.setup ();
}
```

## Keymap markup

### `TD(id)`

> A key that acts as a tap-dance key. The actions performed depend on the
> implementation for the `id` index within the [`tapDanceActions`][tdactions]
> function.
>
> The `id` parameter here is what will be used as `tap_dance_index` in the
> handler function.

 [tdaction]: #tapdanceactiontapdanceindex-tapcount-tapdanceaction

## Plugin methods

The plugin provides a `TapDance` object, but to implement the actions, we need
to define a function ([`tapDanceAction`][tdaction]) outside of the object. A
handler, of sorts. Nevertheless, the plugin provides one macro that is
particularly useful: `tapDanceActionKeys`. Apart from that, it provides one
property only:

### `.time_out`

> The number of loop iterations to wait before a tap-dance sequence times out.
> Once the sequence timed out, the action for it will trigger, even without an
> interruptor. Defaults to 5, and the timer resets with every tap of the same

### `tapDanceActionKeys(tap_count, tap_dance_action, keys...)`

> Sets up an action where for each subsequent tap, a different key will be
> chosen from the list of keys supplied in the `keys...` argument.
>
> If we have `Key_A` and `Key_B` in the list, then, if tapped once, this
> function will input `A`, but when tapped twice, will input `B`.
>
> When all our actions are just different keys, this is a very handy macro to
> use.
>
> The `tap_count` and `tap_dance_actions` parameters should be the same as the
> similarly named parameters of the `tapDanceAction` function.

### `tapDanceAction(tap_dance_index, key_addr, tap_count, tap_dance_action)`

> The heart of the tap-dance plugin is the handler method. This is called every
> time any kind of tap-dance action is to be performed. See the
> *[How does it work?](#how-does-it-work)* section for details about when and
> how this function is called.
>
> The `tap_dance_index` and `tap_count` parameters help us choose which action
> to perform. The `row` and `col` parameters tell us where the tap-dance key is
> on the keyboard.

## Dependencies

* [Kaleidoscope-Ranges](Ranges.md)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: ../../examples/Keystrokes/TapDance/TapDance.ino
