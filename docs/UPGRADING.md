As the firmware evolves, there are - and will be - APIs that we deprecate, and
eventually remove. We are constantly adding new features and plugins too.

This document lists noteworthy new features for the *current* release, with examples of use. Another section provides a short guide for upgrading from deprecated APIs. For deprecations, their planned removal date is also listed.

If any of this does not make sense to you, or you have trouble updating your .ino sketch or custom plugins, do not hesitate to write us at help@keyboard.io, we can help you fix it.

* [Upgrade notes](#upgrade-notes)
  + [New features](#new-features)
    - [New device API](#new-device-api)
    - [New plugin API](#new-plugin-api)
    - [Bidirectional communication for plugins](#bidirectional-communication-for-plugins)
    - [Consistent timing](#consistent-timing)
  + [Breaking changes](#breaking-changes)
    - [Layer system switched to activation-order](#layer-system-switched-to-activation-order)
    - [The `RxCy` macros and peeking into the keyswitch state](#the-rxcy-macros-and-peeking-into-the-keyswitch-state)
    - [HostOS](#hostos)
    - [MagicCombo](#magiccombo)
    - [Qukeys](#qukeys)
    - [TypingBreaks](#typingbreaks)
    - [Redial](#redial)
    - [Key mapping has been deprecated](#key-mapping-has-been-deprecated)
  + [Deprecated APIs and their replacements](#deprecated-apis-and-their-replacements)
    - [Source code and namespace rearrangement](#source-code-and-namespace-rearrangement)
* [Removed APIs](#removed-apis)

# Upgrade notes

As a matter of policy, we try hard to give you at least 60 days notice before we permanently remove or break
any API we've included in a release. Typically, this means that any code that uses the old API will emit a warning when compiled with a newer version of Kaleidoscope. In all cases, this document should explain how to update your code to use the new API.


## New features

### New device API

We are introducing - or rather, replacing - the older hardware plugins, with a system that's much more composable, more extensible, and will allow us to better support new devices, different MCUs, and so on.

#### For end-users

For end users, this doesn't come with any breaking changes. A few things have been deprecated (`ROWS`, `COLS`, `LED_COUNT`, `KeyboardHardware`), but they still function for the time being.

#### For developers

For those wishing to port Kaleidoscope to devices it doesn't support yet, the new API should make most things considerably easier. Please see the (work in progress) documentation in [doc/device-apis.md](doc/device-apis.md).

The old symbols and APIs are no longer available.

### New plugin API

#### For end-users

With the next version of `Kaleidoscope`, we are introducing a new plugin API. It's more efficient, smaller, and uses less resources than the previous one, while being more extensible, and a lot easier to use as well. But all of this matters little when one's not all that interested in writing plugins. However, the new plugin API comes with breaking changes, and one will need to update their own sketch too.

To achieve all of the above, we had to change how plugins are initialized. Instead of using `Kaleidoscope.use()` in the `setup()` method of one's sketch, the plugins must now be initialized with `KALEIDOSCOPE_INIT_PLUGINS()`, outside of the `setup()` method. While `use()` was expecting pointers (`&Plugin`), `_INIT_PLUGINS()` expects references (`Plugin`).

The conversion should be simple, and all of the official plugins have been updated already to use the new API, so they're safe to use this way. Some third-party plugins may still use the older API, they will need to be updated.

To make things clear, here's an example of how to migrate from the old way to the new:

```c++
// Old way
void setup() {
  Kaleidoscope.use(&LEDControl,
                   &Macros,
                   &OneShot,
                   &MouseKeys,
                   &LEDOff,
                   &LEDRainbowEffect);
  Kaleidoscope.setup();
```

```c++
// New way
KALEIDOSCOPE_INIT_PLUGINS(LEDControl,
                          Macros,
                          OneShot,
                          MouseKeys,
                          LEDOff,
                          LEDRainbowEffect);
void setup() {
  Kaleidoscope.setup();
}
```

One thing to keep in mind is that with the old interface, plugins were able to automatically pull in their dependencies. This is not possible with the new interface, and one is required to initialize dependencies manually. Please consult the documentation of the plugins you use, to discover their dependencies - if any - and add them to the list if need be. You only need to add each dependency once.

#### For developers

Developing plugins should be considerably simpler now, there is no need to register hooks, just implement the parts of the `kaleidoscope::Plugin` interface that make sense for a particular plugin.

In practice, this boils down to implementing one or more of the following hook points:

- `onSetup()`: Called once during device bootup, at the end of the `setup()` method. It takes no arguments, and must return `kaleidoscope::EventHandlerResult::OK`.
- `beforeEachCycle()`: Called once, at the beginning of each cycle of the main loop. This is similar to the old "loop hook" with its `post_clear` argument set to false. Takes no arguments, must return `kaleidoscope::EventHandlerResult::OK`.
- `onKeyswitchEvent`: Called for every non-idle key event. This replaces the old "event handler hook". It takes a key reference, a key address, and a key state. The key reference can be updated to change the key being processed, so that any plugin that processes it further, will see the updated key. Can return `kaleidoscope::EventHandlerResult::OK` to let other plugins process the event further, or `kaleidoscope::EventHandlerResult::EVENT_CONSUMED` to stop processing.
- `onFocusEvent`: Used to implement [bi-directional communication](#bidirectional-communication-for-plugins). This is called whenever the firmware receives a command from the host. The only argument is the command name. Can return `kaleidoscope::EventHandlerResult::OK` to let other plugins process the event further, or `kaleidoscope::EventHandlerResult::EVENT_CONSUMED` to stop processing.
- `beforeReportingState`: Called without arguments, just before sending the keyboard and mouse reports to the host. Must return `kaleidoscope::EventHandlerResult::OK`.
- `afterEachCycle`: Called without arguments at the very end of each cycle. This is the replacement for the "loop hook" with its `post_clear` argument set.

### Bidirectional communication for plugins

#### For end-users

Whereas one would have used `Focus.addHook()` to register a new focus command, with the new architecture, one needs to add the object implementing the command to their list of plugins in `KALEIDOSCOPE_INIT_PLUGINS()`. A number of plugins that used to provide optional Focus commands now provide them by default. Some still provide optional ones, and we must transition to the new way.

For example, where one would have written the following before:

```c++
Focus.addHook(FOCUS_HOOK_LEDCONTROL);
```

...we need to add the appropriate object to the plugin list:

```c++
KALEIDOSCOPE_INIT_PLUGINS(LEDControl,
                          FocusLEDCommand)
```

#### For developers

Upgrading from `Focus` to `onFocusEvent` and `FocusSerial` is a reasonably simple process, the interface is quite similar. Nevertheless, we present a step-by-step guide here, covering two use cases: one where we wish to always provide a Focus command when both our plugin and `FocusSerial` are enabled; and another where we only wish to provide the command when explicitly asked to do so.

##### The most trivial example

The biggest difference between `Focus` and `onFocusEvent` is that the former required explicit registering of hooks, while the latter does it automatically: every plugin that implements the `onFocusEvent` method will be part of the system. As a consequence, only plugins are able to supply new commands: there is no explicit registration, thus, no way to inject a command that isn't part of a plugin. This also means that these functions now return `kaleidoscope::EventHandlerResult` instead of `bool`. Furthermore, with `FocusSerial`, all communication is expected to go through it, instead of using `Serial` directly. Lets see a trivial example!

###### Focus

```c++
bool exampleFocusHook(const char *command) {
  if (strcmp_P(command, PSTR("example")) != 0)
    return false;

  Serial.println(F("This is an example response. Hello world!"));

  return true;
}

KALEIDOSCOPE_INIT_PLUGINS(Focus)

void setup() {
  Serial.begin(9600);
  Kaleidoscope.setup();
  Focus.addHook(FOCUS_HOOK(exampleFocusHook, "example"));
}
```

###### onFocusEvent

```c++
namespace kaleidoscope {
class FocusExampleCommand : public Plugin {
 public:
  FocusExampleCommand() {}

  EventHandlerResult onFocusEvent(const char *command) {
    if (strcmp_P(command, PSTR("example")) != 0)
      return EventHandlerResult::OK;

    ::Focus.send(F("This is an example response. Hello world!"));
    return EventHandlerResult::EVENT_CONSUMED;
  }
};
}

kaleidoscope::FocusExampleCommand FocusExampleCommand;

KALEIDOSCOPE_INIT_PLUGINS(Focus, FocusExampleCommand);

void setup() {
  Kaleidoscope.setup();
}
```

###### Summary

The new version is slightly more verbose for the trivial use case, because we have to wrap it up in an object. But other than that, the changes are minimal, and we don't need to explicitly register it!

Observe that the return values changed: with `Focus`, if we wanted other hooks to have a chance at processing the same command, the hook returned `false`; if we wanted to stop processing, and consider it consumed, it returned `true`. With the new system, this is more descriptive with the `EventHandlerResult::OK` and `EventHandlerResult::EVENT_CONSUMED` return values.

##### A stateful example

Perhaps a better example that shows the quality of life improvements the new system brings is the case where the command needs access to either plugin state, or plugin methods. With the former system, the focus hooks needed to be static methods, and needed to be public. This is not necessarily the case now, because `onFocusEvent` is a non-static object method. It has full access to plugin internals!

###### Focus

```c++
namespace kaleidoscope {
class ExamplePlugin : public Plugin {
 public:
  ExamplePlugin();

  static bool exampleToggle() {
    example_toggle_ = !example_toggle_;
    return example_toggle_;
  }

  static bool focusHook(const char *command) {
    if (strcmp_P(command, PSTR("example.toggle")) != 0)
      return false;

    ::Focus.printBool(exampleToggle());
    return true;
  }

 private:
  static bool example_toggle_;
};
}

kaleidoscope::ExamplePlugin ExamplePlugin;

KALEIDOSCOPE_PLUGIN_INIT(Focus, ExamplePlugin)

void setup() {
  Serial.begin(9600);
  Kaleidoscope.setup();

  Focus.addHook(FOCUS_HOOK(ExamplePlugin.focusHook, "example.toggle"));
}
```

###### onFocusEvent

```c++
namespace kaleidoscope {
class ExamplePlugin : public Plugin {
 public:
  ExamplePlugin();

  EventHandlerResult onFocusEvent(const char *command) {
    if (strcmp_P(command, PSTR("example.toggle")) != 0)
      return EventHandlerResult::OK;

    example_toggle_ = !example_toggle_;
    ::Focus.send(example_toggle_);

    return EventHandlerResult::EVENT_CONSUMED;
  }

 private:
  static bool example_toggle_;
};
}

kaleidoscope::ExamplePlugin ExamplePlugin;

KALEIDOSCOPE_PLUGIN_INIT(Focus, ExamplePlugin)

void setup() {
  Kaleidoscope.setup();
}
```

###### Summary

It's just another plugin, with just another event handler method implemented, nothing more. No need to explicitly register the focus hook, no need to provide access to private variables - we can just keep them private.

##### Optional commands

Optional commands are something that were perhaps easier with the `Focus` method: one just didn't register them. With `onFocusEvent`, we need to do a bit more than that, and move the command to a separate plugin, if we do not wish to enable it in every case. This adds a bit of overhead, but still less than `Focus` did.

###### Focus

```c++
bool exampleOptionalHook(const char *command) {
  if (strcmp_P(command, PSTR("optional")) != 0)
    return false;

  Serial.println(Layer.getLayerState(), BIN);
  return true;
}

KALEIDOSCOPE_INIT_PLUGINS(Focus)

void setup() {
  Kaleidoscope.setup();
}
```

Do note that we do not register the `exampleOptionalHook` here! As such, because it is unused code, it will get optimized out during compilation. While this is a simplistic example, the optional hook might have been part of a class, that provides other hooks.

###### onFocusEvent

```c++
namespace kaleidoscope {
class ExampleOptionalCommand : public Plugin {
 public:
  ExampleOptionalCommand() {}

  EventHandlerResult onFocusEvent(const char *command) {
    if (strcmp_P(command, PSTR("optional")) != 0)
      return EventHandlerResult::OK;

    ::Focus.send(Layer.getLayerState());
    return EventHandlerResult::EVENT_CONSUMED;
  }
};
}

KALEIDOSCOPE_INIT_PLUGINS(Focus)

void setup() {
  Kaleidoscope.setup();
}
```

###### Summary

The trick here is to move optional commands out into a separate plugin. It's a bit more boilerplate, but not by much.

### Consistent timing

As an end-user, there's nothing one needs to do. Consistent timing helpers are primarily for plugin use.

As a developer, one can continue using `millis()`, but migrating to `Kaleidoscope.millisAtCycleStart()` is recommended. The new method will return the same value for the duration of the main loop cycle, making time-based synchronization between plugins a lot easier.

## Breaking changes

### Layer system switched to activation order

The layer system used to be index-ordered, meaning that we'd look keys up on
layers based on the _index_ of active layers. Kaleidoscope now uses activation
order, which looks up keys based on the order of layer activation.

This means that the following functions are deprecated, and will be removed by **2020-12-31**:

- `Layer.top()`, which used to return the topmost layer index. Use
  `Layer.mostRecent()` instead, which returns the most recently activated layer.
  Until removed, the old function will return the most recent layer.
- `Layer.deactivateTop()`, which used to return the topmost layer index. Use
  `Layer.deactivateMostRecent()` instead. The old function will deactivate the
  most recent layer.
- `Layer.getLayerState()`, which used to return a bitmap of the active layers.
  With activation-order, a simple bitmap is not enough. For now, we still return
  the bitmap, but without the ordering, it is almost useless. Use
  `Layer.forEachActiveLayer()` to walk the active layers in order (from least
  recent to most).

#### For end-users

This is a breaking change only if your code accesses the member `raw` of
type `Key` directly, for instance in a construct like

```cpp
Key k;
k.raw = Key_A.raw;
```

This can easily be fixed by replacing read access to `Key::raw` with `Key::getRaw()`
and write access with `Key::setRaw(...)`.

```cpp
Key k;
k.setRaw(Key_A.getRaw());
```

Moreover, the compiler will still emit warnings in places of the code where
members `keyCode` and `flags` of the original type `Key` are used, like e.g.

```cpp
Key k;
k.keyCode = Key_A.keyCode;
k.flags = Key_A.flags;
```

These warnings can be also resolved by using the appropriate accessor methods
`Key::getKeyCode()`/`Key::setKeyCode()` and `Key::getFlags()`/`Key::setKlags()`
instead.

```cpp
Key k;
k.setKeyCode(Key_A.getKeyCode());
k.setFlags(Key_A.getFlags());
```

### The `RxCy` macros and peeking into the keyswitch state

The `RxCy` macros changed from being indexes into a per-hand bitmap to being an
index across the whole keyboard. This means they can no longer be `or`-ed
together to check against the keyswitch state of a given hand. Instead, the
`kaleidoscope::hid::getKeyswitchStateAtPosition()` method can be used to check
the state of a keyswitch at a given row and column; or at a given index.

### HostOS

Prior versions of `HostOS` used to include a way to auto-detect the host
operating system. This code was brittle, unreliable, and rather big too. For
these reasons, this functionality was removed.
Furthermore, `HostOS` now depends on `Kaleidoscope-EEPROM-Settings`, that plugin
should be initialized first.

### MagicCombo

To make `MagicCombo` more portable, and easier to use, we had to break the API previously provided, there was no way to maintain backwards compatibility. This document is an attempt at guiding you through the process of migrating from the earlier API to the current one.

Migration should be a straightforward process, but if you get stuck, please feel free to [open an issue][gh:issues], or start a thread on the [forums][forums], and we'll help you with it.

 [gh:issues]: https://github.com/keyboardio/Kaleidoscope/issues
 [forums]: https://community.keyboard.io/

#### The old API

```c++
void magicComboActions(uint8_t combo_index, uint32_t left_hand, uint32_t right_hand) {
  switch (combo_index) {
  case 0:
    Macros.type(PSTR("It's a kind of magic!"));
    break;
  }
}

static const kaleidoscope::MagicCombo::combo_t magic_combos[] PROGMEM = {
  {
    R3C6,  // left palm key
    R3C9   // right palm key
  },
  {0, 0}
};

void setup() {
  Kaleidoscope.setup();

  MagicCombo.magic_combos = magic_combos;
}
```

Previously, we used a global, overrideable function (`magicComboActions`) to run
the actions of all magic combos, similar to how macros are set up to work.
Unlike macros, magic combos can't be defined in the keymap, due to technical
reasons, so we had to use a separate list - `magic_combos` in our example. We
also needed to tell `MagicCombo` to use this list, which is what we've done in
`setup()`.

#### The new API

```c++
void kindOfMagic(uint8_t combo_index) {
  Macros.type(PSTR("It's a kind of magic!"));
}

USE_MAGIC_COMBOS({
  .action = kindOfMagic,
  .keys = {R3C6, R3C9} // Left Fn + Right Fn
});
```

The new API is much shorter, and is inspired by the way the [Leader][leader]
plugin works: instead of having a list, and a dispatching function like
`magicComboActions`, we include the action method in the list too!

 [leader]: plugins/Leader.md

We also don't make a difference between left- and right-hand anymore, you can
just list keys for either in the same list. This will be very handy for
non-split keyboards.

#### Migration

First of all, we'll need to split up `magicComboActions` into separate
functions. Each function should have a unique name, but their shape is always
the same:

```c++
void someFunction(uint8_t combo_index) {
 // Do some action here
}
```

Copy the body of each `case` statement of `magicComboActions`, and copy them one
by one into appropriately named functions of the above shape. You can name your
functions anything you want, the only constraint is that they need to be valid
C++ function names. The plugin itself does nothing with the name, we'll
reference them later in the `USE_MAGIC_COMBOS` helper macro.

Once `magicComboActions` is split up, we need to migrate the `magic_combos` list
to the new format. That list had to be terminated by a `{0, 0}` entry, the new
method does not require such a sentinel at the end.

For each entry in `magic_combos`, add an entry to `USE_MAGIC_COMBOS`, with the
following structure:

```c++
{.action = theActionFunction,
 .keys = { /* list of keys */ }}
```

The list of keys are the same `RxCy` constants you used for `magic_combos`, with
the left- and right hands combined. The action, `theActionFunction`, is the
function you extracted the magic combo action to. It's the function that has the
same body as the `case` statement in `magicComboActions` had.

And this is all there is to it.

If your actions made use of the `left_hand` or `right_hand` arguments of
`magicComboActions`, the same information is still available. But that's a bit
more involved to get to, out of scope for this simple migration guide. Please
open an issue, or ask for help on the forums, and we'll help you.

### Qukeys

Older versions of the plugin used `row` and `col` indexing for defining `Qukey`
objects. This has since been replaced with a single `KeyAddr` parameter in the
constructor.

Older versions of the plugin used a single timeout, configured via a
`setTimeout()` method. For clarity, that method has been renamed to
`setHoldTimeout()`.

Older versions of the plugin used a configurable "release delay" value to give
the user control over how Qukeys determined which value to assign to a qukey
involved in rollover, via the `setReleaseDelay()` method. That release delay has
been replaced with a better "overlap percentage" strategy, which makes the
decision based on the percentage of the subsequent keypress's duration overlaps
with the qukey's press. The configuration method is now `setOverlapThreshold()`,
which accepts a value between 0 and 100 (interpreted as a percentage). User who
used higher values for `setReleaseDelay()` will want a lower values for
`setOverlapThreshold()`.

These functions have been deprecated since 2019-08-22, and will be removed by **2020-12-31**:

- `Qukeys.setTimeout(millis)`
- `Qukeys.setReleaseDelay(millis)`
- `Qukey(layer, row, col, alternate_key)`

### TypingBreaks

Older versions of the plugin used to provide EEPROM storage for the settings only optionally, when it was explicitly enabled via the `TypingBreaks.enableEEPROM()` method. Similarly, the Focus hooks were optional too.

Storing the settable settings in EEPROM makes it depend on `Kaleidoscope-EEPROM-Settings`, which should be initialized before this plugin is.

### Redial

Older versions of the plugin required one to set up `Key_Redial` manually, and let the plugin know about it via `Redial.key`. This is no longer required, as the plugin sets up the redial key itself. As such, `Redial.key` was removed, and `Key_Redial` is defined by the plugin itself. To upgrade, simply remove your definition of `Key_Redial` and the `Redial.key` assignment from your sketch.

### Key masking has been deprecated

Key masking was a band-aid introduced to avoid accidentally sending unintended keys when key mapping changes between a key being pressed and released. Since the introduction of keymap caching, this is no longer necessary, as long as we can keep the mapping consistent. Users of key masking are encouraged to find ways to use the caching mechanism instead.

As an example, if you had a key event handler that in some cases masked a key, it should now map it to `Key_NoKey` instead, until released.

The masking API has been deprecated, and is scheduled to be removed after **2020-11-25**.

## Deprecated APIs and their replacements

### Source code and namespace rearrangement

With the move towards a monorepo-based source, some headers have moved to a new location, and plenty of plugins moved to a new namespace (`kaleidoscope::plugin`). This means that the old headers, and some old names are deprecated. The old names no longer work.

The following headers and names have changed:

- `layers.h`, `key_defs_keymaps.h` and `macro_helpers.h` are obsolete, and should not be included in the first place, as `Kaleidoscope.h` will pull them in. In the rare case that one needs them, prefixing them with `kaleidoscope/` is the way to go. Of the various headers provided under the `kaleidoscope/` space, only `kaleidoscope/macro_helpers.h` should be included directly, and only by hardware plugins that can't pull `Kaleidoscope.h` in due to circular dependencies.
- `LED-Off.h`, provided by [LEDControl](plugins/LEDControl.md) is obsolete, the `LEDOff` LED mode is automatically provided by `Kaleidoscope-LEDControl.h`. The `LED-Off.h` includes can be safely removed.
- `LEDUtils.h` is automatically pulled in by `Kaleiodscope-LEDControl.h`, too, and there's no need to directly include it anymore.
- Plugins that implement LED modes should subclass `kaleidoscope::plugin::LEDMode` instead of `kaleidoscope::LEDMode`.
- [GhostInTheFirmware](plugins/GhostInTheFirmware.md) had the `kaleidoscope::GhostInTheFirmware::GhostKey` type replaced by `kaleidoscope::plugin::GhostInTheFirmware::GhostKey`.
- [HostOS](plugins/HostOS.md) no longer provides the `Kaleidoscope/HostOS-select.h` header, and there is no backwards compatibility header either.
- [Leader](plugins/Leader.md) had the `kaleidoscope::Leader::dictionary_t` type replaced by `kaleidoscope::plugin::Leader::dictionary_t`.
- [LED-AlphaSquare](plugins/LED-AlphaSquare.md) used to provide extra symbol graphics in the `kaleidoscope::alpha_square::symbols` namespace. This is now replaced by `kaleidoscope::plugin::alpha_square::symbols`.
- [LEDEffect-SolidColor](plugins/LEDEffect-SolidColor.md) replaced the base class  - `kaleidoscope::LEDSolidColor` - with `kaleidoscope::plugin::LEDSolidColor`.
- [Qukeys](plugins/Qukeys.md) had the `kaleidoscope::Qukey` type replaced by `kaleidoscope::plugin::Qukey`.
- [ShapeShifter](plugins/ShateShifter.md) had the `kaleidoscope::ShapeShifter::dictionary_t` type replaced by `kaleidoscope::plugin::ShapeShifter::dictionary_t`.
- [SpaceCadet](plugins/SpaceCadet.md) had the `kaleidoscope::SpaceCadet::KeyBinding` type replaced by `kaleidoscope::plugin::SpaceCadet::KeyBinding`.
- [Syster](plugins/Syster.md) had the `kaleidoscope::Syster::action_t` type replaced by `kaleidoscope::plugin::Syster::action_t`.
- [TapDance](plugins/TapDance.md) had the `kaleidoscope::TapDance::ActionType` type replaced by `kaleidoscope::plugin::TapDance::ActionType`.

# Removed APIs

### Removed on 2020-10-10

### Deprecation of the HID facade

With the new Device APIs it became possible to replace the HID facade (the `kaleidoscope::hid` family of functions) with a driver. As such, the old APIs are deprecated, and was removed on 2020-10-10. Please use `Kaleidoscope.hid()` instead.

### Implementation of type Key internally changed from C++ union to class

The deprecated functions were removed on 2020-10-10.

### Removed on 2020-06-16

#### The old device API

After the introduction of the new device API, the old APIs (`ROWS`, `COLS`, `LED_COUNT`, `KeyboardHardware`, the old `Hardware` base class, etc) were removed on **2020-06-16**.

#### `LEDControl.mode_add()`

Since March of 2019, this method has been deprecated, and turned into a no-op. While no removal date was posted at the time, after more than a year of deprecation, it has been removed on **2020-06-16**.

#### `LEDControl.paused`

Wherever we used `LEDControl.paused`, we'll need to use one of
`LEDControl.disable()`, `LEDControl.enable()`, or `LEDControl.isEnabled()`
instead. `LEDControl.paused`  has been removed on **2020-06-16**.

Keep in mind that `.enable()` and `.disable()` do more than what `paused` did:
they will refresh and turn off LEDs too, respectively.

A few examples to show how to transition to the new APIs follow, old use first, new second.

```c++
if (someCondition) {
  LEDControl.set_all_leds_to({0, 0, 0});
  LEDControl.syncLeds();
  LEDControl.paused = true;
} else if (someOtherCondition) {
  LEDControl.paused = false;
  LEDControl.refreshAll();
}

if (LEDControl.paused) {
 // do things...
}
```

```c++
if (someCondition) {
  LEDControl.disable();
} else if (someOtherCondition) {
  LEDControl.enable();
}
if (!LEDControl.isEnabled()) {
  // do things...
}
```

#### Class/global instance Kaleidoscope_/Kaleidoscope renamed to kaleidoscope::Runtime_/kaleidoscope::Runtime

After the renaming, Kaleidoscope core should be using `kaleidoscope::Runtime`.
The former `Kaleidoscope` global symbol is to be used by sketches only - and
only  to not diverge too much from the Arduino naming style.

The deprecated `Kaleidoscope_` class has been removed on **2020-06-16**.

#### Transition to linear indexing

Row/col based indexing was replaced by linear indexing throughout the whole firmware. A compatibility layer of functions was introduced that allows the firmware to remain backwards compatible, however, these functions are deprecated and will be removed in future versions of the firmware.

Also a new version of the onKeyswitchEvent-handler has been introduced.

The deprecated row/col based indexing APIs have been removed on **2020-06-16**.

### Removed on 2020-01-06

#### EEPROMKeymap mode

The [EEPROM-Keymap](plugins/EEPROM-Keymap.md) plugin had its `setup()` method changed, the formerly optional `method` argument is now obsolete and unused. It can be safely removed.

##### keymaps array and KEYMAPS and KEYMAPS_STACKED macros

The `keymaps` array has been replaced with a `keymaps_linear` array. This new array treats each layer as a simple one dimensional array of keys, rather than a two dimensional array of arrays of rows. At the same time, the `KEYMAPS` and `KEYMAPS_STACKED` macros that were previously defined in each hardware implmentation class have been replaced with `PER_KEY_DATA` and `PER_KEY_DATA_STACKED` macros in each hardware class. This change should be invisible to users, but will require changes by any plugin that accessed the 'keymaps' variable directly.

Code like `key.raw = pgm_read_word(&(keymaps[layer][row][col])); return key;` should be changed to look like this: `return keyFromKeymap(layer, row, col);`

### Removed on 2019-01-18

#### Removal of Layer.defaultLayer

The `Layer.defaultLayer()` method has been deprecated, because it wasn't widely used, nor tested well, and needlessly complicated the layering logic. If one wants to set a default layer, which the keyboard switches to when booting up, `EEPROMSettings.default_layer()` may be of use.

`Layer.defaultLayer` has since been removed.

#### More clarity in Layer method names

A number of methods on the `Layer` object have been renamed, to make their intent clearer:

- `Layer.on()` and `Layer.off()` became `Layer.activate()` and `Layer.decativate()`, repsectively.
- `Layer.next()` and `Layer.previous()` became `Layer.activateNext()` and `Layer.deactivateTop()`.
- `Layer.isOn` became `Layer.isActive()`.

The goal was to have a method name that is a verb, because these are actions we do. The old names have since been removed.

### Removed on 2019-01-17

#### Compat headers following the source code and namespace rearrangement

With the move towards a monorepo-based source, some headers have moved to a new location, and plenty of plugins moved to a new namespace (`kaleidoscope::plugin`). This means that the old headers, and some old names are deprecated. The old names no longer work.

#### HostOS.autoDetect()

The `autoDetect()` method has been formerly deprecated, and is now removed.

#### The old MagicCombo API

We've changed the API of the MagicCombo plugin, and while it provided a helpful error message for a while when trying to use the old API, it no longer does so, the error message has been removed.

#### TypingBreaks.enableEEPROM()

`TypingBreaks.enableEEPROM()` has been previously deprecated, and turned into a no-op, and is now removed.

#### `OneShot.double_tap_sticky` and `OneShot.double_tap_layer_sticky`

These were deprecated in favour of a better, finer grained API, and are now removed.

### Removed on 2018-08-20

We aim at making a new release by mid-July, and APIs we deprecate now, will be
removed shortly after the major release, before the next point release. We may
deprecate further APIs during the next month (until mid-June), and those
deprecations will share the same removal date. We will try our best to minimize
deprecations, and do them as soon as possible, to give everyone at least a month
to prepare and update.

#### Kaleidoscope.use()

Deprecated in May 2018, this method is part of the old plugin API, replaced by
`KALEIDOSCOPE_INIT_PLUGINS`. To upgrade, you need to modify your .ino sketch
file, and replace the text `Kaleidoscope.use` with `KALEIDOSCOPE_INIT_PLUGINS`,
then remove the `&` from all of the plugins inside it, and finally, move it
outside of `setup()`.

If your current sketch looks like this:

```c++
void setup() {
  Kaleidoscope.use(&Plugin1, &Plugin2);
  Kaleidoscope.setup();
}
```

You should change it so that it looks like this instead:

```c++
KALEIDOSCOPE_INIT_PLUGINS(Plugin1, Plugin2);

void setup() {
  Kaleidoscope.setup();
}
```

#### The old-style (v1) plugin API

This includes using `KaleidoscopePlugin`, `Kaleidoscope.useEventHandlerHook`,
`Kaleidoscope.replaceEventHandlerHook`, `Kaleidoscope.appendEventHandlerHook`,
`Kaleidoscope.useLoopHook`, `Kaleidoscope.replaceLoopHook`,
`Kaleidoscope.appendLoopHook`. They were deprecated in May 2017.

Their replacement is the new plugin API:

```c++
namespace kaleidoscope {

enum class EventHandlerResult {
  OK,
  EVENT_CONSUMED,
  ERROR,
};

class Plugin {
public:
  EventHandlerResult onSetup();
  EventHandlerResult beforeEachCycle();
  EventHandlerResult onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t key_state);
  EventHandlerResult beforeReportingState();
  EventHandlerResult afterEachCycle();
};

}
```

Plugins are supposed to implement this new API, and then be initialised via
`KALEIDOSCOPE_INIT_PLUGINS`.

#### Consumer_SNapshot

A key with a typo in its name, which was left in place after fixing the typo, so
as to not break any code that may be using it already, however unlikely.

### Removed on 2018-06-10 (originally scheduled for 2018-05-27)

These APIs and functions have been deprecated for a long time, and as far as we
can tell, aren't used by any third party or user code anymore. They were removed
as of the June 10th, 2018.

#### Kaleidoscope.setup(KEYMAP_SIZE)

The `Kaleidoscope.setup()` method is still around, and is **not** deprecated,
but the variant of it that takes a keymap size is, and has been since October
2017.

Instead, one should use the argument-less `Kaleidoscope.setup()`, and the new
`KEYMAP()` macros to define a keymap.

#### event_handler_hook_use, loop_hook_use, and USE_PLUGINS

Deprecated in October 2017, these are old aliases that should no longer be in
use. They were replaced by `Kaleidoscope.useEventHandlerHook`,
`Kaleidoscope.useLoopHook`, and `Kaleidoscope.use`, respectively.

The replacements themselves are also deprecated - see below -, but their removal
will come at a later date.

#### MOMENTARY_OFFSET

Deprecated in October 2017, replaced by `LAYER_SHIFT_OFFSET`.

This symbol was meant to be used by plugins, not user code, and as far as we
know, no third party plugin ever used it.

#### key_was_pressed, key_is_pressed, key_toggled_on, key_toggled_off

Deprecated in July 2017, replaced by `keyWasPressed`, `keyIsPressed`,
`keyToggledOn`, and `keyToggledOff`, respectively.
