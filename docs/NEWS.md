# Kaleidoscope v2.0

> Currently at 1.99.8, in development!

See [UPGRADING.md](UPGRADING.md) for more detailed instructions about upgrading from earlier versions (even earlier betas). This is just a list of noteworthy changes.

* [New features](#new-features)
* [New hardware support](#new-hardware-support)
* [New plugins](#new-plugins)
* [Breaking changes](#breaking-changes)
* [Bugfixes](#bugfixes)

## New features

### ModLayer keys

There is a new type of built-in key that activates both a layer shift and a
keyboard modifier simultaneously, and keeps both active until the key is
released.  Basically, it's a combination of a single modifier key (any one of
the standard eight) and a `ShiftToLayer(N)` key (for any layer in the range
0-31).

A ModLayer key `key` will return `true` for the test functions
`key.isKeyboardModifier()`, `key.isLayerKey()`, and `key.isLayerShift()`.  As
such, it can be turned into a OneShot key by either
`OneShot.enableAutoModifiers()` or `OneShot.enableAutoLayers()`.

An additional umbrella test function has also been added: `key.isMomentary()`,
which returns `true` for any key that is either a keyboard modifier or a layer
shift (including ModLayer keys).

### Layer changes updated

Layer change key handling has been updated to be more consistent with activation
ordering.  In most common cases there will be no obvious difference; layer move,
lock, and shift keys still generally function the same way.  The details now
vary by type in the edge cases, however.

Layer lock keys (i.e. `LockLayer(N)`) will change slightly.  Whenever a layer
lock key toggles on, it will put the target layer on the top of the active layer
stack, unless that layer is already at the top of the stack, in which case it
will be deactivated.  This means that if layers `A`, `B`, and `C` are on the
stack, with `A` on top, pressing `LockLayer(B)` will move layer `B` to the top
of the stack, above `A`, rather than deactivating it.  As a result, if you use
layers that have no transparent entries, every press of a layer lock key will
result in a user-visible change.

Layer shift keys will now work independently of locked layers.  This means that
if layers `A`, `B` and `C` are (locked) on the stack, with `A` on top, pressing
`ShiftToLayer(B)` will keep a temporary version of layer `B` on top, but when
that layer shift key is released, the layer stack will return to the same state
it had been in before the layer shift key was pressed, with layers `A`, `B`, and
`C` still active, and in the same order.  However, since it is assumed that
users won't forget about keys that they are holding, pressing a second layer
shift key for the same target layer will not result in promoting that shifted
layer to the top of the stack.

A consequence of this is that releasing a layer shift key will no longer
deactivate a layer that is locked, either from pressing a `LockLayer()` or
`MoveToLayer()` key.  This allows users to configure keymaps such that layer `N`
could be reached by holding `ShiftToLayer(N)`, then kept active and on top by
tapping `LockLayer(N)` or `MoveToLayer(N)`, which could be mapped on Layer `N`.
That layer would continue to stay active after the release of the
`ShiftToLayer(N)` key.

### OneShot public functions

The OneShot plugin now allows other plugins to control the OneShot state of
individual keys, by calling one of the following:

- `OneShot.setPending(key_addr)`: Put the key at `key_addr` in the "pending"
  OneShot state.  This will make that key act like any other OneShot key until
  it is cancelled by a subsequent keypress.  Once a key is in this state,
  OneShot will manage it from that point on, including making the key "sticky"
  if it is double-tapped.
- `OneShot.setSticky(key_addr)`: Put the key at `key_addr` in the "sticky"
  OneShot state.  The key will be released by OneShot when it is tapped again.
- `OneShot.setOneShot(key_addr)`: Put the key at `key_addr` in the "one-shot"
  state.  This is normally the state OneShot key will be in after it has been
  tapped.  Calling `setPending()` is more likely to be useful.
- `OneShot.clear(key_addr)`: Clear the OneShot state of the key at `key_addr`.

Note: Any plugin that calls one of these OneShot methods must either be
registered in `KALEIDOSCOPE_INIT_PLUGINS()` after OneShot, or it must add the
`INJECTED` bit to the keyswitch state of the event (i.e. `event.state |=
INJECTED`) to prevent OneShot from prematurely advancing keys to the next
OneShot state.

### SpaceCadet "no-delay" mode

SpaceCadet can now be enabled in "no-delay" mode, wherein the primary (modifier)
value of the key will be sent to the host immediately when the key is pressed.
If the SpaceCadet key is released before the timeout, the modifier is released,
and then the alternate (symbol) value is sent.  To activate "no-delay" mode, call `SpaceCadet.enableWithoutDelay()`.

### New Qukeys features

#### Tap-repeat

It is now possible to get the "tap" value of a qukey to repeat (as if that key
for that character was simply being held down on a normal keyboard) by tapping
the qukey, then quickly pressing and holding it down. The result on the OS will
be as if the key was pressed and held just once, so that users of macOS apps
that use the Cocoa input system can get the menu for characters with diacritics
without an extra character in the output.

The maximum interval between the two keypresses that will trigger a tap repeat
can be configured via the `Qukeys.setMaxIntervalForTapRepeat(ms)` function,
where the argument specifies the number of milliseconds Qukeys will wait after a
qukey is tapped for it to be pressed a second time. If it is, but the qukey is
released within that same interval from the first tap's release, it will be
treated as a double-tap, and both taps will be sent to the OS.

### New OneShot features

#### Auto-OneShot modifiers & layers

OneShot can now treat modifiers and layer-shift keys as automatic OneShot
keys. This includes modifiers with other modifier flags applied, so it is now
very simple to turn `Key_Meh` or `Key_Hyper` into a OneShot key. The feature is
controlled by the following new functions:

- `OneShot.toggleAutoModifiers()`: Turn auto-OneShot modifiers on or off.
- `OneShot.toggleAutoLayers()`: Turn auto-OneShot layer shifts on or off.
- `OneShot.toggleAutoOneShot()`: Both of the above.

There are also `enable` and `disable` versions of these functions.

Note, it is still possible to define a modifier key in the keymap that will not
automatically become a OneShot key when pressed, by applying modifier flags to
`Key_NoKey` (e.g. `LSHIFT(Key_NoKey)`).

#### Two new special OneShot keys

OneShot can now also turn _any_ key into a sticky key, using either of two
special `Key` values that can be inserted in the keymap.

##### `OneShot_MetaStickyKey`

This is a special OneShot key (it behaves like other OneShot keys), but its
effect is to make any key pressed while it is active sticky. Press
`OneShot_MetaStickyKey`, then press `X`, and `X` will become sticky. Sticky
keys can be deactivated just like other OneShot keys, by pressing them
again. This works for any key value, so use it with caution.

##### `OneShot_ActiveStickyKey`

Like `OneShot_ActiveStickyKey`, this key makes other keys sticky, but rather than
affecting a subsequent key, it affects any keys already held when it is
pressed. Press `X`, press `OneShot_ActiveStickyKey`, and release `X`, and `X`
will be sticky until it is pressed again to deactivate it. Again, it works on
any key value, so use with caution.

#### LED-ActiveModColor highlighting

With the updates to OneShot, LED-ActiveModColor now recognizes and highlights
OneShot keys in three different states (along with normal modifiers):

- one-shot (a key that's active after release, but will time out)
- sticky (a key that will stay active indefinitely after release)
- normal (a key that will stay active only while physically held; also applies
  to normal modifier keys)

The colors of theses three highlights are controlled by the properties
`ActiveModColorEffect.oneshot_color`, `ActiveModColorEffect.sticky_color`, and
`ActiveModColorEffect.highlight_color`, respectively.

### Better protection against unintended modifiers from Qukeys

Qukeys has two new configuration options for preventing unintended modifiers in
the output, particularly when typing fast:

- `Qukeys.setMinimumHoldTime(ms)` sets the minimum duration of a qukey press
  required for it to be eligible to take on its alternate (modifier) value.
- `Qukeys.setMinimumPriorInterval(ms)` sets the minimum interval between the
  previous printable (letters, numbers, and punctuation) key press and the press
  of the qukey required to make the qukey eligible to take on its alternate
  (modifier) value.

### KALEIDOSCOPE_API_VERSION bump

`KALEIDOSCOPE_API_VERSION` has been bumped to **2** due to the plugin API
changes mentioned below. It does not mean that version two of the API is final,
though. The bump is there so plugins can check it, and make compile-time
decisions based on it. Such as whether to compile for the version one, or for
the version two API.

The API version will remain the same, even if we introduce breaking changes -
until a stable release is made from the v2 branch. From that point onwards, the
API version will change with further breaking changes.

### New device API

A new hardware device API was introduced in November 2019, replacing the old
system. It was designed to be more composable, more future proof than the old
system. All hardware plugins under Keyboardio control have been updated to use
the new APIs.

See [UPGRADING.md](UPGRADING.md#new-device-api) for more information.

### New plugin API

A new plugin API was introduced in May 2018, which replaces the old system. The new system is hopefully easier to use and develop for:

 - It does not require one to register / use hooks anymore. Implementing the interface provided by `kaleidoscope::Plugin` is all that is required.
 - The new system has more hook points, and the method names are much more clear now.

Plugins under Keyboardio control have all been updated to use the new API, and they no longer support the older one.

See [UPGRADING.md](UPGRADING.md#new-plugin-api) for more information.

### Transition to a monorepo

We heard a lot of complaints that while the plugin architecture of Kaleidoscope is great, having so many plugins scattered around in dozens of repositories is a huge barrier of entry for potential contributors, and a significant pain point for end-users to update. For these reasons and more, we merged almost all plugins into the Kaleidoscope repository.

While at first it may seem that this is a move towards a monolithic architecture, rest assured, it is not. The plugin APIs are still a core part of Kaleidoscope, it isn't going anywhere. We merely rearranged the sources, is all. Nothing else changes.

Some headers and names did change, however, see [UPGRADING.md](UPGRADING.md#source-code-and-namespace-rearrangement) for more information.

### Bidirectional communication for plugins

The bi-directional communication protocol formerly implemented by `Kaleidoscope-Focus` has been partially pulled into core, using the new plugin system mentioned above. The new system makes it a lot easier for both end-users and developers to use the feature.

See [UPGRADING.md](UPGRADING.md#bidirectional-communication-for-plugins) for more information.

### Consistent timing

Numerous plugins use timers, most of them directly calling `millis()`. This has the disadvantage that calls within a main loop cycle will be inconsistent, which makes timing synchronization across plugins hard. The newly introduced `Kaleidoscope.millisAtCycleStart()` function helps dealing with this issue.

See [UPGRADING.md](UPGRADING.md#consistent-timing) for more information.

### USB detach / attach

It is now possible to detach, and re-attach the USB link from/to the host, without resetting the device. The intent of this feature (as implemented by the `Kaleidoscope.detachFromHost()` and `Kaleidoscope.attachToHost()` methods) is to allow configuration changes without rebooting.

See the [Kaleidoscope-USB-Quirks][plugin:USB-Quirks] plugin for a use-case.

 [plugin:USB-Quirks]: hAttps://github.com/keyboardio/Kaleidoscope-USB-Quirks

### Finer stickability controls for OneShot

The [OneShot plugin](plugins/Kaleidoscope-OneShot.md) gained finer stickability controls, one can now control whether the double-tap stickiness is enabled on a per-key basis. See [UPGRADING.md](UPGRADING.md#finer-oneshot-stickability-control) for more information.

### A way to slow down Unicode input

In certain cases we need to delay the unicode input sequence, otherwise the host is unable to process the input properly. For this reason, the [Unicode](plugins/Kaleidoscope-Unicode.md) gained an `.input_delay()` method that lets us do just that. It still defaults to no delay.

### Better support for modifiers in the Cycle plugin

The [Cycle](plugins/Kaleidoscope-Cycle.md) plugin has much better support for cycling through keys with modifiers applied to them, such as `LSHIFT(Key_A)`. Please see the documentation and the updated example for more information.

### More control over when to send reports during Macro playback

There are situations where one would like to disable sending a report after each and every step of a macro, and rather have direct control over when reports are sent. The new `WITH_EXPLICIT_REPORT`, `WITH_IMPLICIT_REPORT` and `SEND_REPORT` steps help with that. Please see the [Macros](plugins/Kaleidoscope-Macros.md) documentation for more information.

### LED-ActiveModColor can be asked to not highlight normal modifiers

The plugin was intended to work with OneShot primarily, and that's where it is most useful. To make it less surprising, and more suitable to include it in default-like firmware, we made it possible to ask it not to highlight normal modifiers. Please see the [LED-ActiveModColor](plugins/Kaleidoscope-LED-ActiveModColor.md) documentation for more information.

### Events now trigger on layer changes

Changing layers now triggers the `onLayerChange` event - but only if there was real change (thus, calling `Layer.on(SOME_LAYER)` multiple times in a row will only trigger one event). This event was introduced to help plugins that depend on layer state schedule their work better.

### Hyper and Meh keys

To make it easier to create custom shortcuts, that do not interfere with system ones, an old trick is to use many modifiers. To make this easier, `Ctrl+Shift+Alt` is commonly abbreviated as `Meh`, while `Ctrl+Shift+Alt+Gui` is often called `Hyper`. To support this, we offer the `Key_Meh` and `Key_Hyper` aliases, along with `MEH(k)` and `HYPER(k)` to go with them.

## `keymap` internals are now a one dimensional array

Historically, Kaleidoscope used the dimensional array `keymaps` to map between logical key position and hardware key position. `keymaps` has been replaced with `keymaps_linear`, which moves the keymap to a simple array. This makes it easier to support new features in Kaleidoscope and simplifies some code


## `PER_KEY_DATA` macros

New `PER_KEY_DATA` and `PER_KEY_DATA_STACKED` macros are available (when defined by a hardware implementation). These macros make it easier to build features like `KEYMAPS` that track some data about each key on a keyboard.

## New hardware support

Kaleidoscope has been ported to the following devices:

- [Atreus](hardware-devices/Hardware-Technomancy-Atreus.md): All known variants of the original Atreus are supported. From the Legacy Teensy variant, through the pre-2016 PCB with an A* MCU, the post-2016 PCB, and FalbaTech's handwired one too. Apart from the legacy Teensy variant, the other support both the A* or a Teensy as an MCU.
- [ErgoDox](hardware-devices/Hardware-EZ-ErgoDox.md): Originally developed to support the ErgoDox EZ, but all other compatible hardware is supported, such as the original ErgoDox and anything else wired like it, like some Dactyls.
- [Planck](hardware-devices/Hardware-OLKB-Planck.md): AVR-based Plancks, and anything else wired similarly should be supported, as long as they use a Teensy.
- [Splitography](hardware-devices/Hardware-SOFTHRUF-Splitography.md): Initial support for the Splitography Steno keyboard.

For more information, please see the hardware plugins' documentation.

To make it easier to port Kaleidoscope, we introduced the `ATMegaKeyboard` base class. For any board that's based on the ATMega MCU and a simple matrix, this might be a good foundation to develop the hardware plugin upon.

## New plugins

### CharShift

The [CharShift](plugins/Kaleidoscope-CharShift.md) plugin allows independent assignment of symbols to keys depending on whether or not a `shift` key is held.

### AutoShift

The [AutoShift](plugins/Kaleidoscope-AutoShift.md) plugin provides an alternative way to get shifted symbols, by long-pressing keys instead of using a separate `shift` key.

### DynamicMacros

The [DynamicMacros](plugins/Kaleidoscope-DynamicMacros.md) plugin provides a way to use and update macros via the Focus API, through Chrysalis.

### IdleLEDs

The [IdleLEDs](plugins/Kaleidoscope-IdleLEDs.md) plugin is a simple, yet, useful one: it will turn the keyboard LEDs off after a period of inactivity, and back on upon the next key event.

### LEDActiveLayerColor

The [LEDActiveLayerColor][plugins/Kaleidoscope-LEDActiveLayerColor.md] plugin makes it possible to set the color of all LEDs to the same color, depending on which layer is active topmost.

### LED-Wavepool

We integrated the [LEDWavepool](plugins/Kaleidoscope-LED-Wavepool.md) plugin by [ToyKeeper][wavepool:origin], with a few updates and new features added.

 [wavepool:origin]: https://github.com/ToyKeeper/Kaleidoscope-LED-Wavepool

### Turbo

The [Turbo](plugins/Kaleidoscope-Turbo.md) plugin provides a way to send keystrokes in very quick succession while holding down a key.

### WinKeyToggle

The [WinKeyToggle](plugins/Kaleidoscope-WinKeyToggle.md) plugin assists with toggling the Windows key on and off - a little something for those of us who game under Windows and are tired of accidentally popping up the start menu.

### FirmwareDump

The [FirmwareDump](plugins/Kaleidoscope-FirmwareDump.md) plugin makes it possible to dump one's firmware over Focus.

## Breaking changes

### Implementation of type Key internally changed from C++ union to class

Type `Key` was originally implemented as a C++ union. For technical reasons
it had to be converted to a C++ class. This implies that the double usage
of the original union, holding either raw data (member `raw`) or key code/key flags
data (members `keyCode` and `flags`) is no more possible.

Direct use of member `raw` will
emit a diagnostic compiler message but will cause the firmware linking
process to fail. For a deprecation
periode `keyCode` and `flags` keep on being supported but will cause
deprecation warnings during compile.

Please see the [relevant upgrade notes](UPGRADING.md##implementation-of-type-key-internally-changed-from-union-to-class)
for information about how to upgrade legacy code.

### `LEDControl.paused` has been deprecated

The `.paused` property of `LEDControl` has been deprecated in favour of the new
`LEDControl.disable()` and `LEDControl.enable()` methods. These two will turn
off or refresh the LEDs, respectively, along with disabling or re-enabling
future updates and syncs.

### The `NumPad` plugin no longer toggles `NumLock`

The `NumPad` plugin used to toggle `NumLock` when switching to the NumPad layer. This caused issues on OSX where `NumLock` is interpreted as `Clear`. For this reason, the plugin no longer does this. As a consequence, everyone's encouraged to update their keymap so that the numpad layer uses normal number keys instead of the keypad numbers. See [Model01-Firmware#79](https://github.com/keyboardio/Model01-Firmware/pull/79) for an example about how to do this.

### The `RxCy` macros and peeking into the keyswitch state

The `RxCy` macros changed from being indexes into a per-hand bitmap to being an index across the whole keyboard. This mostly affected the [MagicCombo](plugins/Kaleidoscope-MagicCombo.md) plugin.

Please see the [relevant upgrade notes](UPGRADING.md#the-rxcy-macros-and-peeking-into-the-keyswitch-state) for more information.

### The `Redial` plugin had a breaking API change

The [Redial](plugins/Kaleidoscope-Redial.md) plugin was simplified, one no longer needs to define `Key_Redial` on their own, the plugin defines it itself. See the [upgrade notes](UPGRADING.md#Redial) for more information about how to upgrade.

### Color palette storage has changed

The [LED-Palette-Theme](plugins/Kaleidoscope-LED-Palette-Theme.md) had to be changed to store the palette colors in reverse. This change had to be made in order to not default to a bright white palette, that would draw so much power that most operating systems would disconnect the keyboard due to excessive power usage. With inverting the colors, we now default to a black palette instead. This sadly breaks existing palettes, and you will have to re-set the colors.

We also changed when we reserve space for the palette in EEPROM: we used to do it as soon as possible, but that made it impossible to go from a firmware that does not use the plugin to one that does, and still have a compatible EEPROM layout. We now reserve space as late as possible. This breaks existing EEPROM layouts however.

### EEPROM-Keymap changed Focus commands

The [EEPROMKeymap](plugins/Kaleidoscope-EEPROM-Keymap.md) plugin was changed to treat built-in (default) and EEPROM-stored (custom) layers separately, because that's less surprising, and easier to work with from Chrysalis. The old `keymap.map` and `keymap.roLayers` commands are gone, the new `keymap.default` and `keymap.custom` commands should be used instead.

### EEPROMSettings' version() setter has been deprecated

We're repurposing the `version` setting: instead of it being something end-users
can set, we'll be using it internally to track changes made to
[EEPROMSettings](plugins/Kaleidoscope-EEPROM-Settings.md) itself, with the goal of
allowing external tools to aid in migrations. The setting wasn't widely used -
if at all -, which is why we chose to repurpose it instead of adding a new
field.

### Key masking has been deprecated

Key masking was a band-aid introduced to avoid accidentally sending unintended keys when key mapping changes between a key being pressed and released. Since the introduction of keymap caching, this is no longer necessary, as long as we can keep the mapping consistent. Users of key masking are encouraged to find ways to use the caching mechanism instead.

## Bugfixes

We fixed way too many issues to list here, so we're going to narrow it down to the most important, most visible ones.

### Support for BIOSes, EFI, login prompts, etc

Keyboards report keys pressed to the host via either of two protocols: the boot protocol, or the report protocol. The boot protocol is the simpler, and it is what older BIOSes, EFI, and certain OS login prompts (or hard disk password prompts and the like) require. Until recently, the firmware wasn't able to provide this protocol, only the more advanced report one, which is required for N-key roll-over.

We now support the boot protocol, and on operating systems that fully conform to the USB specification, this works automatically. For all others, one can implement a way to force one mode or the other. See the [factory firmware][Model01-Firmware] for an example how to achieve this.

 [Model01-Firmware]: https://github.com/keyboardio/Model01-Firmware
