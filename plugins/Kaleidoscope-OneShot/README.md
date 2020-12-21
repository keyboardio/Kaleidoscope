# OneShot

One-shots are a new kind of behaviour for your standard modifier and momentary
layer keys: instead of having to hold them while pressing other keys, they can
be tapped and released, and will remain active until any other key is pressed
subject to a time-out.

In short, they turn `Shift, A` into `Shift+A`, and `Fn, 1` to `Fn+1`. The main
advantage is that this allows us to place the modifiers and layer keys to
positions that would otherwise be awkward when chording. Nevertheless, they
still act as normal when held, that behaviour is not lost.

Furthermore, if a one-shot key is double-tapped ie tapped two times in quick
succession, it becomes sticky, and remains active until disabled with a third tap.
This can be useful when one needs to input a number of keys with the modifier or
layer active, and does not wish to hold the key down. If this "stickability"
feature is undesirable, it can be unset (and later again set) for individual
modifiers/layers. If stickability is unset, double-tapping a one-shot modifier
will just restart the timer.

To make multi-modifier, or multi-layer shortcuts possible, one-shot keys remain
active if another one-shot of the same type is tapped, so `Ctrl, Alt, b` becomes
`Ctrl+Alt+b`, and `L1, L2, c` is turned into `L1+L2+c`. Furthermore, modifiers
and other layer keys do not cancel the one-shot effect, either.

## Using One-Shot Keys

To enter one-shot mode, tap _quickly_ on a one-shot key. The next
normal (non-one-shot) key you press will have the modifier applied,
and then the modifier will automatically turn off. If the Shift key is
a one-shot modifier, then hitting `Shift, a, b` will give you `Ab`,
_if you hit shift quickly._

Longish keypresses do not activate one-shot mode. If you press `Shift,
a, b`, as above, but hold the Shift key a bit longer, you'll get `ab`.

To enter sticky mode, _tap twice quickly_ on a one-shot key. The
modifier will now stay on until you press it again. Continuing the
`Shift` example, tapping `Shift, Shift` _quickly_ and then `a, b, c,
Shift, d, e, f` will give you `ABCdef`.

This can be a bit tricky; combining this plugin with
[LED-ActiveModColor](LED-ActiveModColor.md)
will help you understand what state your one-shot is in; when a
one-shot key is active, it will have a white LED highlight; when
sticky, a red highlight. (These colors are configurable.)


## Using the plugin

After adding one-shot keys to the keymap, all one needs to do, is enable the
plugin:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-OneShot.h>

// somewhere in the keymap...
OSM(LeftControl), OSL(_FN)

KALEIDOSCOPE_INIT_PLUGINS(OneShot);

void setup() {
  Kaleidoscope.setup();
}
```

## Keymap markup

There are two macros the plugin provides:

### `OSM(mod)`

> A macro that takes a single argument, the name of the modifier: `LeftControl`,
> `LeftShift`, `LeftAlt`, `LeftGui` or their right-side variant. When marked up
> with this macro, the modifier will act as a one-shot modifier.

### `OSL(layer)`

> Takes a layer number as argument, and sets up the key to act as a one-shot
> layer key.
>
> Please note that while `Kaleidoscope` supports more, one-shot layers are
> limited to 8 layers only.

## Plugin methods

The plugin provides one object, `OneShot`, which implements both one-shot
modifiers and one-shot layer keys. It has the following methods:

### `.isActive()`

> Returns if any one-shot key is in flight. This makes it possible to
> differentiate between having a modifier or layer active, versus having them
> active only until after the next key getting pressed. And this, in turn, is
> useful for macros that need to fiddle with either modifier or layer state: if
> one-shots are not active, they need not restore the original state.

### `.isPressed()`

> Returns true if any one-shot key is still held.

### `.isSticky(key)`

> Returns if the key is currently sticky.

### `.isModifierActive(key)`

> Returns if the modifier `key` has a one-shot state active. Use this together
> with `Kaleidoscope.hid().keyboard().isModifierKeyActive` to catch cases where
> a one-shot modifier is active, but not registered yet.

### `.cancel([with_stickies])`

> The `cancel()` method can be used to cancel any pending one-shot effects,
> useful when one changed their minds, and does not wish to wait for the
> timeout.
>
> The optional `with_stickies` argument, if set to `true`, will also cancel
> sticky one-shot effects. If omitted, it defaults to `false`, and not canceling
> stickies.

### `.inject(key, keyState)`

> Simulates a key event, specifically designed to inject one-shot keys into the
> event loop. The primary purpose of this method is to make it easier to trigger
> multiple one-shots at the same time.
>
> See the example sketch for more information about its use.

### `.enableStickability(key...)`

> Enables stickability for all keys listed. The keys should all be OneShot keys,
> as if specified on the keymap. For example:
>   `OneShot.enableStickability(OSM(LeftShift), OSL(1))`.
>
> By default, all oneshot keys are stickable.

### `.enableStickabilityForModifiers()`
### `.enableStickabilityForLayers()`

> Enables stickability for all modifiers and layers, respectively. These are
> convenience methods for cases where one wants to enable stickability for a
> group of one-shot keys.

### `.disableStickability(key...)`

> Disables stickability for all keys listed. The keys should all be OneShot keys,
> as if specified on the keymap. For example:
>   `OneShot.disableStickability(OSM(LeftShift), OSL(1))`.
>
> By default, all oneshot keys are stickable.

### `.disableStickabilityForModifiers()`
### `.disableStickabilityForLayers()`

> Disables stickability for all modifiers and layers, respectively. These are
> convenience methods for cases where one wants to disable stickability for a
> group of one-shot keys.

## Plugin properties

Along with the methods listed above, the `OneShot` object has the following
properties too:

### `.time_out`

> Set this property to the number of milliseconds to wait before timing out and
> cancelling the one-shot effect (unless interrupted or cancelled before by any
> other means).
>
> Defaults to 2500.

### `.hold_time_out`

> Set this property to the number of milliseconds to wait before considering a
> held one-shot key as intentionally held. In this case, the one-shot effect
> will not trigger when the key is released. In other words, holding a one-shot
> key at least this long, and then releasing it, will not trigger the one-shot
> effect.
>
> Defaults to 200.

### `.double_tap_time_out`

> Set this property to the number of milliseconds within which a second
> uninterrupted tap of the same one-shot key will be treated as a sticky-tap.
> Only takes effect when `.double_tap_sticky` is set.
>
>
> Setting the property to `-1` will make the double-tap timeout use `.time_out`
> for its calculations.
>
> Defaults to -1.

## Dependencies

* [Kaleidoscope-Ranges](Ranges.md)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: /examples/Keystrokes/OneShot/OneShot.ino
