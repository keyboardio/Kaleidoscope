# Qukeys

## Concept

This Kaleidoscope plugin allows you to overload keys on your keyboard so that
they produce one keycode (i.e. symbol) when tapped, and a different keycode --
most likely a modifier (e.g. `shift` or `alt`) -- when held. The name is a play
on the term _qubit_; a qukey is a "quantum key". When it is first pressed it is
in a superposition of states until some event determines which state it ends up
in. While a qukey is in this indeterminate state, its key press event and any
subsequent key presses are delayed until something determines the qukey's
ultimate state.

Most likely, what determines the qukey's state (_primary_ or _alternate_) is the
release of a key; if the qukey is released before a subsequent key, it will take
on its primary value (most likely a printable character), but if the subsequent
key is released first, it will take on its alternate value (usually a modifier).

Qukeys is designed to make it practical to use these overloaded keys on the home
row, where similar designs have historically been problematic. For some typists
(particularly those who are accustomed to rolling over from modifiers to
modified keys, rather than deliberately holding the modifier until the
subsequent key has been released), this may still not work perfectly with
Qukeys, but some people have reported good results with home-row qukeys.


## Setup

- Include the header file:
```
#include <Kaleidoscope-Qukeys.h>
```
- Use the plugin in the `KALEIDOSCOPE_INIT_PLUGINS` macro:
```
KALEIDOSCOPE_INIT_PLUGINS(Qukeys);
```

- Define some `Qukeys` of the format `Qukey(layer, key_addr, alternate_key)`.
  Layers and key addresses are all zero-indexed, in key addresses rows are top to bottom and
  columns are left to right:

- For the Keyboardio Model 01, key coordinates refer to [this header
  file](https://github.com/keyboardio/Kaleidoscope/blob/master/plugins/Kaleidoscope-Hardware-Keyboardio-Model01/src/kaleidoscope/device/keyboardio/Model01.h#L153).
  
- For the Keyboardio Model 100, key coordinates refer to [this header
  file](https://github.com/keyboardio/Kaleidoscope/blob/master/plugins/Kaleidoscope-Hardware-Keyboardio-Model100/src/kaleidoscope/device/keyboardio/Model100.h#L175).

```
QUKEYS(
    // left-side modifiers
    kaleidoscope::plugin::Qukey(0, KeyAddr(2, 1), Key_LeftGui),      // A
    kaleidoscope::plugin::Qukey(0, KeyAddr(2, 2), Key_LeftAlt),      // S
    kaleidoscope::plugin::Qukey(0, KeyAddr(2, 3), Key_LeftControl),  // D
    kaleidoscope::plugin::Qukey(0, KeyAddr(2, 4), Key_LeftShift),    // F
    // left-side layer shifts
    kaleidoscope::plugin::Qukey(0, KeyAddr(3, 3), ShiftToLayer(NUMPAD)),    // C
    kaleidoscope::plugin::Qukey(0, KeyAddr(3, 4), ShiftToLayer(FUNCTION)),  // V
```

`Qukeys` will work best if it's the first plugin in the `INIT()` list, because when typing
overlap occurs, it will (temporarily) mask keys and block them from being processed by
other plugins. If those other plugins handle the keypress events first, it may not work as
expected. It doesn't _need_ to be first, but if it's `INIT()`'d after another plugin that
handles typing events, especially one that sends extra keyboard HID reports, it is more
likely to generate errors and out-of-order events.


## Configuration

### `.setHoldTimeout(timeout)`

> Sets the time (in milliseconds) after which a qukey held on its own will take
> on its alternate state. Note: this is not the primary determining factor for a
> qukey's state. It is not necessary to wait this long before pressing a key
> that should be modified by the qukey's alternate value. The primary function
> of this timeout is so that a qukey can be used as a modifier for an separate
> pointing device (i.e. `shift` + `click`).
>
> Defaults to `250`.

### `.setMaxIntervalForTapRepeat(timeout)`

> Sets the time (in milliseconds) that limits the tap-repeat window. If the same
> qukey is pressed, released, and pressed again within this timeframe, then
> held, Qukeys will turn it into a single press and hold event, using the
> primary key value (which cannot otherwise be held). If the second press is
> also a tap, and the two _release_ events occur within the same timeframe, it
> will instead be treated as a double tap (of the primary key value).
>
> To effectively shut off the tap-repeat feature, set this value to `0`. The
> maximum value is `255`; anything higher than `250` could result in key repeat
> being triggered on the host before Qukeys determines whether it's a tap-repeat
> or a double-tap sequence, because most systems delay the key repeat by 500 ms.
>
> Defaults to `200`.

### `.setOverlapThreshold(percentage)`

> This sets a variable that allows the user to roll over from a qukey to a
> subsequent key (i.e. the qukey is released first), and still get the qukey's
> alternate (modifier) state.
>
> The `percentage` parameter should be between `1` and `100` (`75` means 75%),
> and represents the fraction of the _subsequent_ key press's duration that
> overlaps with the qukey's press. If the subsequent key is released soon enough
> after the qukey is released, the percentage overlap will be high, and the
> qukey will take on its alternate (modifier) value. If, on the other hand, the
> subsequent key is held longer after the qukey is released, the qukey will take
> on its primary (non-modifier) value.
>
> Setting `percentage` to 100% turns off the grace period, so you can’t reliably
> get either output if you release the two keys simultaneously. That means the
> subsequent key must be released before the qukey for the release-order
> condition to trigger making the qukey take on its alternate state.
>
> Setting `percentage` to a low value (e.g. `30`) will result in a longer grace
> period. If you're getting primary values when you intended modifiers, try
> decreasing this setting. If, on the other hand, you start getting modifiers
> when you intend primary values, try increasing this setting. If you're getting
> both, the only solution is to change your typing habits, unfortunately.
>
> Defaults to `80`.

### `.setMinimumHoldTime(min_hold_time)`

> Sets the minimum amount of time (in milliseconds) a qukey must be held before
> it is allowed to resolve to its alternate `Key` value. Use this if you find
> that you're getting unintended alternate values (i.e. modifiers) while typing
> on home-row qukeys, despite setting the overlap threshold (see above) to
> 100%. It may mean that you'll need to slow down when using Qukeys to get
> modifiers, however.
>
> Defaults to `50` (milliseconds).

### `.setMinimumPriorInterval(min_interval)`

> Sets the minimum amount of time (in milliseconds) that must pass between the
> press event of a prior (non-modifier) key and the press of a qukey required to
> make that qukey eligible to take on it's alternate state. This is another
> measure that can be taken to prevent unintended modifiers while typing fast.
>
> Defaults to `75` (milliseconds).

### `.activate()`
### `.deactivate()`
### `.toggle()`

> Activate/deactivate `Qukeys` plugin.

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

DualUse keys are more limited than `Qukey` definitions, which can contain any
valid `Key` value for both the primary and alternate keys, but they take up less
space in program memory, and are just as functional for typical definitions.


### SpaceCadet Emulation

It is possible to define a `Qukey` on a key with a _primary_ value that is a
modifier. In this case, the qukey is treated specially, and the _primary_ value
is used when the key is held, rather than the alternate value. The _alternate_
value is only used if the qukey is tapped on its own, without rolling over to
any other key. This is a reasonable facsimile of the behaviour of the SpaceCadet
plugin, and is much more suitable for keys that are mainly used as modifiers,
with an additional "tap" feature.

In addition to working this way on keyboard modifiers (`shift`, `control`, _et
al_), this works for keys that are primarily layer shift keys
(e.g. `ShiftToLayer(N)`).

As an added bonus, if Qukeys is deactivated, such a key reverts to being a
modifier, because that's what's in the keymap.


### The Wildcard Layer

There is a special value (`Qukeys::layer_wildcard`) that can be used in place of
the layer number in the definition of a `Qukey`. This will define a qukey with
the given alternate value on all layers, regardless of what the primary value is
for that key on the top currently active layer.


### Tap-Repeat Behaviour

If a qukey is tapped, then immediately pressed and held, Qukeys will turn that
sequence of events into a single press and hold of the primary key value
(whereas merely holding the key yeilds the alternate value). This is
particularly useful on macOS apps that use Apple's Cocoa input system, where
holding a key gives the user access to a menu of accented characters, rather
than merely repeating the same character until the key is released.


## Design & Implementation

When a qukey is pressed, it doesn't immediately add a corresponding keycode to
the HID report; it adds that key to a queue, and waits until one of three things
happens:

1. the qukey is released
1. a subsequently-pressed key is released
1. a time limit is reached

Until one of those conditions is met, all subsequent keypresses are simply added
to the queue, and no new reports are sent to the host. Once a condition is met,
the qukey is flushed from the queue, and so are any subsequent keypresses (up
to, but not including, the next qukey that is still pressed).

Basically, if you hold the qukey, then press and release some other key, you'll
get the alternate keycode (probably a modifier) for the qukey, even if you don't
wait for a timeout. If you're typing quickly, and there's some overlap between
two keypresses, you won't get the alternate keycode, and the keys will be
reported in the order that they were pressed -- as long as the keys are released
in the same order they were pressed.

The time limit is mainly there so that a qukey can be used as a modifier (in its
alternate state) with a second input device (e.g. a mouse). It can be quite
short (200ms is probably short enough) -- as long as your "taps" while typing
are shorter than the time limit, you won't get any unintended alternate
keycodes.

## Further reading

The [example][plugin:example] can help to learn how to use this plugin.

 [plugin:example]: /examples/Keystrokes/Qukeys/Qukeys.ino
