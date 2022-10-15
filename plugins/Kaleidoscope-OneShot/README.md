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

## Using One-Shot keys

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
[LED-ActiveModColor](Kaleidoscope-LED-ActiveModColor.md) will help you
understand what state your one-shot is in; when a one-shot key is active, it
will have a yellow LED highlight; when sticky, a red highlight. When it is in a
"held" state, but will be deactivated when released like any non-one-shot key,
it will have a white highlight. (These colors are configurable.)

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

To enable configuring the plugin via [Focus][focus] (including via
[Chrysalis][chrysalis]), one will also need the `OneShotConfig` plugin enabled
in addition.

 [focus]: Kaleidoscope-FocusSerial.md
 [chrysalis]: https://github.com/keyboardio/Chrysalis

## Keymap markup

There are two macros the plugin provides:

#### `OSM(mod)`

> A macro that takes a single argument, the name of the modifier: `LeftControl`,
> `LeftShift`, `LeftAlt`, `LeftGui` or their right-side variant. When marked up
> with this macro, the modifier will act as a one-shot modifier.

#### `OSL(layer)`

> Takes a layer number as argument, and sets up the key to act as a one-shot
> layer key.
>
> Please note that while `Kaleidoscope` supports more, one-shot layers are
> limited to 8 layers only.

In addition, there is a special key:

#### `Key_MetaSticky`

> A key that behaves like a one-shot key, but while active, it makes
> other keys that are pressed become sticky, just like double-tapped
> one-shot keys.

## Plugin methods

The plugin provides one object, `OneShot`, which implements both one-shot
modifiers and one-shot layer keys. It has the following methods:

### Configuration methods: Timeouts

#### `.setTimeout(timeout)`

> OneShot keys will remain active after they're pressed for `timeout`
> milliseconds (or until a subsequent non-oneshot key is pressed). The
> default value is 2500 (2.5 seconds).

#### `.setHoldTimeout(timeout)`

> If a one-shot key is held for longer than `timeout` milliseconds, it
> will behave like a normal key, and won't remain active after it is
> released. The default value is 250 (1/4 seconds).

#### `.setDoubleTapTimeout(timeout)`

> If a one-shot key is double-tapped (pressed twice in a row) in less
> than `timeout` milliseconds, it wil become sticky, and will remain
> active until it is pressed a third time. The default value is -1,
> which indicates that it should use the same timeout as
> `.setTimeout()`.

### Configuration methods: Stickability

#### `.enableStickability(key...)`
#### `.disableStickability(key...)`

> Enables/disables stickability for all keys listed. The keys should
> all be OneShot keys, modifier keys, or layer-shift keys, as
> specified on the keymap. For example:
> `OneShot.enableStickability(OSM(LeftShift), OSL(1), Key_RightGUI)`.
> `OneShot.disableStickability(OSM(RighttAlt), OSL(2), ShiftToLayer(4))`.
>
> By default, all OneShot keys are stickable.

#### `.enableStickabilityForModifiers()`
#### `.enableStickabilityForLayers()`
#### `.disableStickabilityForModifiers()`
#### `.disableStickabilityForLayers()`

> Enables/disables stickability for all modifiers and layers,
> respectively. These are convenience methods for cases where one
> wants to enable stickability for a group of one-shot keys.

### Configuration methods: Automatic one-shot keys

#### `.enableAutoModifiers()`
#### `.disableAutoModifiers()`
#### `.toggleAutoModifiers()`

> Enables/disables/toggles auto-oneshot functionality for modifier
> keys. When enabled, all normal modifier keys, including those with
> other modifier flags added to them (e.g. `LSHIFT(Key_LeftAlt)`,
> `Key_Meh`) will be automatically treated as one-shot keys, in
> addition to dedicated ones like `OSM(LeftGui)`.

#### `.enableAutoLayers()`
#### `.disableAutoLayers()`
#### `.toggleAutoLayers()`

> Enables/disables/toggles auto-oneshot functionality for layer shift
> keys (see above).

#### `.enableAutoOneShot()`
#### `.disableAutoOneShot()`
#### `.toggleAutoOneShot()`

> Enables/disables/toggles auto-oneshot functionality for all
> modifiers and layer shift keys.

### Test methods

#### `.isActive(key_addr)`

> Returns `true` if the key at `key_addr` is in an active one-shot
> state. Note that if a key is still being held, but will be not
> remain active after it is released, it is not considered to be in a
> one-shot state, even if it had been earlier.

#### `.isTemporary(key_addr)`

> Returns `true` if the key at `key_addr` is in a temporary one-shot
> state. Such a key will eventually time out or get deactivated by a
> subsequent key press.

#### `.isSticky(key_addr)`

> Returns `true` if the key at `key_addr` is in a permanent one-shot
> state. Such a key will not be deactivated by subsequent keypresses,
> nor will it time out. It will only be deactivated by pressing it one
> more time, or by being cancelled by the `cancel()` method (see
> below).

#### `.isActive()`

> Returns `true` if there are any active one-shot keys. Note: it
> returns `false` if there are no currently active one-shot keys, but
> there are keys that were at one time in a one-shot state, but are
> still being held after that state has been cancelled.

#### `.isSticky()`

> Returns `true` if there are any sticky one-shot keys.

#### `.isStickable(key)`

> Returns `true` if a key of the specified value can be made sticky by
> double-tapping.

#### `.isModifier(key)`

> Returns `true` if the specified key is a modifier key. This does not
> include OneShot modifiers (e.g. `OSM(LeftShift)`), but it does
> include modifiers with additional modifier flags (e.g. `Key_Meh`,
> `LCTRL(Key_RightGui)`).

#### `.isLayerShift(key)`

> Returns `true` if the specified key is a layer-shift key
> (e.g. `ShiftToLayer(2)`). OneShot layer keys (e.g. `OSL(5)` are not
> included).

#### `.isOneShotKey(key)`

> Returns `true` if the specified key is a OneShot modifier or
> layer-shift key (e.g. `OSM(LeftAlt)`, `OSL(3)`).

### Other methods

#### `.cancel([with_stickies])`

> Immediately deactivates the one-shot status of any _temporary_
> one-shot keys. Any keys still being physically held will continue to
> function as normal modifier/layer-shift keys.
>
> If `with_stickies` is `true` (the default is `false`), _sticky_
> one-shot keys will also be deactivated, in the same way.

### Deprecated methods

The following methods have been deprecated, and should no longer be
used, if possible. These functions made more sense when OneShot was
based on `Key` values; it has since be rewritten to be based on
`KeyAddr` values.

#### `.inject(key, key_state)`

> Finds an idle key on the keyboard, and turns it into a one-shot
> key. When OneShot was based on `Key` values, this made more sense,
> as it didn't need a valid `KeyAddr` to work. Since the main purpose
> of this method was to enable the triggering of multiple one-shot
> modifiers with a single key, it is much better to use automatic
> one-shot modifiers, if possible, because then it's not necessary to
> use a Macro to configure.

#### `.isModifierActive(key)`

> Returns `true` if a keymap cache entry with the current value of
> `key` is active (one-shot, sticky, or held). This should be a
> function that is not specific to OneShot.

#### `.isActive(key)`

> Returns `true` if a keymap cache entry with the current value of
> `key` is in an active one-shot state. Please use
> `.isActive(key_addr)` instead.

#### `.isSticky(key)`

> Returns `true` if a keymap cache entry with the current value of
> `key` is in a sticky one-shot state. Please use
> `.isSticky(key_addr)` instead.

#### `.isPressed()`

> Returns `false`. OneShot doesn't need to keep track of whether or
> not a one-shot key is still pressed any more. This function was
> mainly used by LED-ActiveModColor, which no longer needs it.

## Focus commands

When the `OneShotConfig` plugin is enabled, the following Focus commands become
available:

### `.timeout`
### `.hold_timeout`
### `.double_tap_timeout`

> These correspond to the `.setTimeout()`, `.setHoldTimeout()`, and
> `.setDoubleTapTimeout()` methods, and can be used to query or set the
> respective timeout value. When used without an argument, the command will
> print the current timeout value. When used with one, it will update it.

### `.auto_modifiers`
### `.auto_layers`

> Corresponds to the `.enableAutoModifiers()` and `.enableAutoLayers()` methods.
> Used without an argument, the command will print the current status of the
> setting, otherwise it will update it.
>
> A value of `1` means the setting is enabled, a value of `0` means it is disabled.

### `.stickable_keys`

> Can be used to query or set the bitmap used for controlling the stickability
> of the oneshot modifier and layer keys. Constructing the bitmap is
> complicated, and is best done through Chrysalis.

## Dependencies

* [Kaleidoscope-Ranges](Kaleidoscope-Ranges.md)

If the `OneShotConfig` plugin is enabled, additional dependencies are:

* [Kaleidoscope-EEPROM-Settings](Kaleidoscope-EEPROM-Settings.md)
* [Kaleidoscope-FocusSerial][focus]

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: /examples/Keystrokes/OneShot/OneShot.ino
