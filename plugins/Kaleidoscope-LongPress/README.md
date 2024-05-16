# LongPress

LongPress allows you to type different characters when long-pressing a key
rather than tapping it.

It is derived from the AutoShift plugin and integrates its functionality. When
using LongPress it supersedes the AutoShift plugin. They conflict with each
other and should not be used together.


## Setup

To use the plugin put the following into your .ino file:

```c++
#include <Kaleidoscope-LongPress.h>

KALEIDOSCOPE_INIT_PLUGINS(LongPress);
```

## Configuration

To do anything useful some configuration is necessary.

### Long-press mappings

To define the keys that should behave differently on long-press include
a definition like the following:

```c++
LONGPRESS(
    kaleidoscope::plugin::LongPressKey(kaleidoscope::plugin::longpress::ALL_LAYERS,
                                       KeyAddr(1, 1),     Key_Q),                    // Long-press the key on QWERTY position of “q” to enter a “q” on all layers
    kaleidoscope::plugin::LongPressKey(QWERTY,
                                       KeyAddr(2, 4),     LCTRL(Key_C)),             // Long-press the key below the index finger to enter “Ctrl-C“ on the QWERTY layer
    kaleidoscope::plugin::LongPressKey(kaleidoscope::plugin::longpress::ALL_LAYERS,
                                       Key_T,             RALT(Key_T)),              // Long-press “t” to enter a “þ” on all layers
)
```

As can be seen in the example long-presses can be configured on either `KeyAddr`
or `Key`, even in combination. Which variant is preferred is based on the use
case.

For example for mirroring the numbers in the number row (produce a “0” on long
pressing “1”, produce a “9” on long pressing “2”, etc.) the best approach is to
configure these on the `KeyAddr`. However to always generate an “ä” when “a” is
long-pressed, regardless of where the “a” is mapped on (and whether it is mapped
to different physical keys, probably on different layers), configuring it on the
`Key` may be preferable.

Be aware, however, that the order of the entries in LONGPRESS matters! Ealier
definitions take precedence over later ones. Usually it is best to define
long-presses on `KeyAddr` first and long-presses on `Key` afterwards as that is
the least surprising behaviour in case of conflicting mappings.

Another thing that can be seen in the example above is that long presses can be
restricted to a single layer (the second one is restricted to the QWERTY
layer). To apply the mapping to all layers, use the constant
`kaleidoscope::plugin::longpress::ALL_LAYERS`[^1] as can be seen the first and the
third mapping in the example above.

### Enabling and disabling LongPress

The following methods are provided for enabling / disabling the plugin altogether:

- `LongPress.enable()` to enable the plugin (after loading the plugin is enabled by default).
- `LongPress.disable()` to disable the plugin.
- `LongPress.toggle()` to switch the plugin between enabled and disabled state.
- `LongPress.enabled()` to check whether the plugin is currently enabled.

### Setting the long-press delay

To set the amount of time (in milliseconds) the LongPress plugin will wait
until it executes the long-press behaviour use `LongPress.setTimeout(timeout)`.

The default is 175.

### Auto-Shifting

One of the most common use cases for Long-Presses is auto-shifting of the
generated character. This use case has special support to avoid having to
configure every single key.

By default no auto-shifting behaviour is applied. To set this behaviour to some
certain sets of keys use one of the following methods:

- `LongPress.setAutoshiftEnabled(categories)` to activate auto-shifting for exactly the given categories.
  To set multiple categories combine them using `|` (bitwise or), e.g.: `LongPress.setAutoshiftEnabled(LongPress.letterKeys() | LongPress.numberKeys())`.
- `LongPress.enableAutoshift(category)` to add a single category to be auto-shifted.
- `LongPress.disableAutoshift(category)` to remove a single category from the auto-shifted ones.
- `LongPress.isAutoshiftEnabled(category)` to check whether auto-shifting is enabled for the given category.
- `LongPress.enabledAutoShiftCategories()` to get an array of the categories for which auto-shifting is enabled.

These are the predefined categories for auto-shifting:

- `LongPress.noKeys()`: Can be used with `LongPress.setAutoshiftEnabled()` to remove all categories from being auto-shifted.
- `LongPress.letterKeys`: All letter keys.
- `LongPress.numberKeys`: All number keys (in the number row, not the numeric keypad).
- `LongPress.symbolKeys`: Other printable symbols.
- `LongPress.arrowKeys`: Navigational arrow keys.
- `LongPress.functionKeys`: All function keys (F1 – F24).
- `LongPress.printableKeys`: Letters, numbers and symbols.
- `LongPress.allKeys`: All non-modifier USB keyboard keys.

If the above categories are not sufficient for your auto-shifting needs, it is
possible to get even finer-grained control of which keys are affected by
auto-shifting, by overriding the `isAutoShiftable()` method in your sketch. For
example, to make LongPress only auto-shift keys `A` and `Z`, include the following
code in your sketch:

```c++
bool LongPress::isAutoShiftable(Key key) {
  if (key == Key_A || key == key_Z)
    return true;
  return false;
}
```

As you can see, this method takes a `Key` as its input and returns either
`true` (for keys eligible to be auto-shifted) or `false` (for keys to be left
alone).

In contrast to the explict configuration of long-presses via `LongPressKey`,
such auto-shift behaviour always applies to all layers.

## Conflicts with other plugins

Care should be taken when using the plugin together with the Qukeys, SpaceCadet
and Chords plugins. Most of the time they conflict with each other and when
using one of these plugins together with LongPress it should be avoided to
configure them on the same keys.

In any case the LongPress plugin should be defined as the last one of these.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

[plugin:example]: /examples/Keystrokes/LongPress/LongPress.ino


[^1] The constant `kaleidoscope::plugin::longpress::ALL_LAYERS` is a bit long
and unwieldy. Before integrating this plugin there were some discussions about
whether that is acceptable. Therefore people using that plugin to apply
mappings to all layers are kindly requested to provide some feedback about
their usage and whether they are annoyed by that long constant name or not.
