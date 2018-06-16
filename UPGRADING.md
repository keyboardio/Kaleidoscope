Kaleidoscope upgrade notes
==========================

Noteworthy features
-------------------

### New plugin API

A new plugin API was introduced in May 2018, which replaces the old system
(which still continues to work, but see below). The new system is hopefully
easier to use and develop for:

 - It does not require one to register / use hooks anymore. Implementing the
   interface provided by `kaleidoscope::Plugin` is all that is required.
 - The new system has more hook points, and the method names are much more clear
   now.

### Kaleidoscope.millisAtCycleStart()

Many plugins use timers, and most of them will call `millis()`, which isn't
wrong, but isn't the most efficient either. While `millis()` isn't terribly
expensive, it's not cheap either. For most cases, we do not need an exact timer,
and one updated once per cycle is enough - which is what `.millisAtCycleStart()`
is. Having a timer that is consistent throughout the whole cycle may also be
beneficial.

While `millis()` should continue to work forever, plugins and user code should,
as a general rule, use `Kaleidoscope.millisAtCycleStart()` rather than
`millis()`.

### Kaleidoscope.detachFromHost() and Kaleidoscope.attachToHost()

These two require changes to the Hardware plugins, as they wrap functionality
provided by it. The intent of these methods is to allow one to apply some
configuration changes between detach and attach - because we only sever the link
between device and host, power remains connected, and the device does not
reboot. A quick way to re-initialize the endpoints.

See the [Kaleidoscope-USB-Quriks][plugin:USB-Quriks] plugin for a use-case.

 [plugin:USB-Quirks]: https://github.com/keyboardio/Kaleidoscope-USB-Quirks

### The `RxCy` macros and peeking into the keyswitch state

The `RxCy` macros changed from being indexes into a per-hand bitmap to being an
index across the whole keyboard. This means they can no longer be `or`-ed
together to check against the keyswitch state of a given hand. Instead, the
`kaleidoscope::hid::getKeyswitchStateAtPosition()` method can be used to check
the state of a keyswitch at a given row and column; or at a given index.

### KALEIDOSCOPE_API_VERSION bump

`KALEIDOSCOPE_API_VERSION` has been bumped to **2** due to the plugin API
changes. It does not mean that version two of the API is final, though. The bump
is there so plugins can check it, and make compile-time decisions based on it.
Such as whether to compile for the version one, or for the version two API.

The API version will remain the same, even if we introduce breaking changes -
until a stable release is made from the v2 branch. From that point onwards, the
API version will change with further breaking changes.

Deprecated APIs and their replacements
--------------------------------------

As the firmware evolves, there are - and will be - APIs that we deprecate, and
eventually remove. This document is a short guide that lists the deprecations -
along with when it happened, and when the functionality will be removed -,
suggested upgrade paths, and any other information that may be useful.

If any of this does not make sense to you, or you have trouble updating your
.ino sketch, do not hesitate to write us at help@keyboard.io, we can help you
fix it.

### Sheduled for removal by 2018-07-12

We aim at making a new release by mid-July, and APIs we deprecate now, will be
removed shortly before the major release. We may deprecate further APIs during
the next month (until mid-June), and those deprecations will share the same
removal date. We will try our best to minimize deprecations, and do them as soon
as possible, to give everyone at least a month to prepare and update.

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
  EventHandlerResult onKeyswitchEvent(Key &mapped_key, byte row, byte col, uint8_t key_state);
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

Deprecated and removed APIs
---------------------------

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
