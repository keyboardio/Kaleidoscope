As the firmware evolves, there are - and will be - APIs that we deprecate, and
eventually remove. We are constantly adding new features and plugins too.

This document lists noteworthy new features for the *current* release, with examples of use. Another section provides a short guide for upgrading from deprecated APIs. For deprecations, their planned removal date is also listed.

If any of this does not make sense to you, or you have trouble updating your .ino sketch or custom plugins, do not hesitate to write us at help@keyboard.io, we can help you fix it.

* [Upgrade notes](#upgrade-notes)
  + [New features](#new-features)
    - [New event handler](#new-event-handler)
    - [Event-driven main loop](#event-driven-main-loop)
    - [Keyboard state array](#keyboard-state-array)
    - [New build system](#new-build-system)
    - [New device API](#new-device-api)
    - [New plugin API](#new-plugin-api)
    - [Bidirectional communication for plugins](#bidirectional-communication-for-plugins)
    - [Consistent timing](#consistent-timing)
  + [Breaking changes](#breaking-changes)
    - [Macros](#macros)
    - [Removed `kaleidoscope-builder`](#removed-kaleidoscope-builder)
    - [OneShot meta keys](#oneshot-meta-keys)
    - [git checkouts aren't compatible with Arduino IDE (GUI)]([#repository-rearchitecture)
    - [Layer system switched to activation-order](#layer-system-switched-to-activation-order)
    - [The `RxCy` macros and peeking into the keyswitch state](#the-rxcy-macros-and-peeking-into-the-keyswitch-state)
    - [HostOS](#hostos)
    - [MagicCombo](#magiccombo)
    - [OneShot](#oneshot)
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

### New event handler

One more `KeyEvent` handler has been added: `afterReportingState(const KeyEvent &event)`.  This handler gets called after HID reports are sent for an event, providing a point for plugins to act after an event has been fully processed by `Runtime.handleKeyEvent()`.

### Event-driven main loop

Kaleidoscope's main loop has been rewritten. It now responds to key toggle-on and toggle-off events, dealing with one event at a time (and possibly more than one in a given cycle). Instead of sending a keyboard HID report at the end of every scan cycle (and letting the HID module suppress duplicates), it now only sends HID reports in response to input events.

Furthermore, there are now two functions for initiating the processing of key events:
- `Runtime.handleKeyswitchEvent()` is the starting point for events that represent physical keyswitches toggling on or off.
- `Runtime.handleKeyEvent()` is the starting point for "artificial" key events. It is also called at the end of `handleKeyswitchEvent()`.
In general, if a plugin needs to generate a key event, it should call `handleKeyEvent()`, not `handleKeyswitchEvent()`.

Each of the above functions calls its own set of plugin event handlers. When those event handlers are all done, event processing continues as `handleKeyEvent()` prepares a new keyboard HID report, then sends it:
- `Runtime.prepareKeyboardReport()` first clears the HID report, then populates it based on the contents of the `live_keys[]` array. Note that the HID report is not cleared until _after_ the new plugin event handlers have been called.
- `Runtime.sendKeyboardReport()`  handles generating extra HID reports required for keys with keyboard modifier flags to avoid certain bugs, then calls a new plugin event handler before finally sending the new HID report.
These functions should rarely, if ever, need to be called by plugins.

#### The `KeyEvent` data type

There is a new `KeyEvent` type that encapsulates all the data relevant to a new key event, and it is used as the parameter for the new event-handling functions.
- `event.addr` contains the `KeyAddr` associated with the event.
- `event.state` contains the state bitfield (`uint8_t`), which can be tested with `keyToggledOn()`/`keyToggledOff()`.
- `event.key` contains a `Key` value, usually looked up from the keymap.
- `event.id` contains a pseudo-unique ID number of type `KeyEventId` (an 8-bit integer), used by certain plugins (see `onKeyswitchEvent()` below).

#### New plugin event handlers
##### `onKeyswitchEvent(KeyEvent &event)`
##### `onKeyEvent(KeyEvent &event)`
##### `onAddToReport(Key key)`
##### `beforeReportingState(const KeyEvent &event)`

#### For end-users

Existing sketches should be mostly backwards-compatible, but some updates will be needed for sketches that use custom code. In particular, users of the Macros plugin are likely to need to make adjustments to the code in the user-defined `macroAction()` function, including that function's signature, the new version of which takes a `KeyEvent` parameter instead of just an event state value. In most cases, this will make the resulting code more straightforward without any loss of functionality.

In addition to Macros, these changes might also affect user-defined code executed by the TapDance, Leader, and Syster plugins. Please see the documentation and examples for the affected plugins for details.

### Keyboard State array

The keymap cache (`Layer_::live_composite_keymap_[]`) has been replaced by a keyboard state array (`kaleidoscope::live_keys[]`). The top-level functions that handle keyswitch events have been updated to treat this new array as a representation of the current state of the keyboard, with corresponding `Key` values for any keys that are active (physically held or activated by a plugin).

#### For end-users

There should be no user-visible changes for anyone who simply uses core plugins. A few functions have been deprecated (`Layer.eventHandler()` & `Layer.updateLiveCompositeKeymap()`), but there are straightforward replacements for both.

#### For developers

The major changes are to the `handleKeyswitchEvent()` function, which has been reorganized in order to update the new keyboard state array with correct values at the appropriate times. In addition to that, two new facilities are available:

##### `EventHandlerResult::ABORT`

This is a new return value available to plugin event handlers, which is similar to `EVENT_CONSUMED` in that it causes the calling hook function to return early (stopping any subsequent handlers from seeing the event), but is treated differently by `handleKeyswitchEvent()`. If a handler returns `EVENT_CONSUMED`, the keyboard state array will still be updated by `handleKeyswitchEvent()`, but if it returns `ABORT`, it will not. In both cases, no further event processing will be done by the built-in event handler.

##### `live_keys[key_addr]`

This is the new facility for checking the value of an entry in the keyboard state array. It is indexed directly by `KeyAddr` values, without the need to convert them to integers first. For example, it could be used in a range-based `for` loop to check for values of interest:

```c++
for (KeyAddr key_addr : KeyAddr::all()) {
  Key key = live_keys[key_addr];
  if (key == Key_LeftShift || key == Key_RightShift) {
    // do something special...
  }
}
```

Additionally, if the `KeyAddr` values are not needed, one can use the iterator from the new `KeyMap` class like so:

```c++
for (Key key : live_keys.all()) {
  if (key == Key_X) {
    // do something special...
  }
}
```

The `live_keys` object's subscript operator can also be used to set values in the keyboard state array:

```c++
live_keys[key_addr] = Key_X;
```

It also comes with several convenience functions which can be used to make the intention of the code clear:

```c++
// Set a value in the keyboard state array to a specified Key value:
live_keys.activate(key_addr, Key_X);

// Set a value to Key_Inactive, deactivating the key:
live_keys.clear(key_addr);

// Set all values in the array to Key_Inactive:
live_keys.clear();)

// Set a value to Key_Masked, masking the key until its next release event:
live_keys.mask(key_addr);
```

In most cases, it won't be necessary for plugins or user sketches to call any of these functions directly, as the built-in event handler functions will manage the keyboard state array automatically.

### New build system

In this release, we replace kaleidoscope-builder with a new Makefile based build system that uses `arduino-cli` instead of of the full Arduino IDE. This means that you can now check out development copies of Kaleidoscope into any directory, using the `KALEIDOSCOPE_DIR` environment variable to point to your installation.

### New device API

We are introducing - or rather, replacing - the older hardware plugins, with a system that's much more composable, more extensible, and will allow us to better support new devices, different MCUs, and so on.

#### For end-users

For end users, this doesn't come with any breaking changes. A few things have been deprecated (`ROWS`, `COLS`, `LED_COUNT`, `KeyboardHardware`), but they still function for the time being.

#### For developers

For those wishing to port Kaleidoscope to devices it doesn't support yet, the new API should make most things considerably easier. Please see the documentation in [device-apis.md](api-reference/device-apis.md).

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
- `onNameQuery`: Used by the [Focus](#bidirecional-communication-for-plugins) plugin, when replying to a `plugins` command. Should either send the plugin name, or not be implemented at all, if the host knowing about the plugin isn't important.
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

  EventHandlerResult onNameQuery() {
    return ::Focus.sendName(F("FocusExampleCommand"));
  }

  EventHandlerResult onFocusEvent(const char *input) {
    const char *cmd = PSTR("example");

    if (::Focus.inputMatchesHelp(input))
      return ::Focus.printHelp(cmd);

    if (!::Focus.inputMatchesCommand(input, cmd))
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

  EventHandlerResult onFocusEvent(const char *input) {
    if (!::Focus.inputMatchesCommand(input, PSTR("example.toggle")))
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

  EventHandlerResult onFocusEvent(const char *input) {
    if (!::Focus.inputMatchesCommand(input, PSTR("optional")))
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

### Sketch preprocssing system

We used to support the ability to amend all compiled sketches by
adding code to
`src/kaleidoscope_internal/sketch_preprocessing/sketch_header.h`
and `src/kaleidoscope_internal/sketch_preprocessing/sketch_footer.h`.
The functionality was never used by Kaleidoscope itself and frequently
pulled the (empty) header files from the wrong copy of Kaleidoscope.
If you need this functionality, please open a GitHub issue.

### Macros

This is a guide to upgrading existing Macros code to use the new version of
Kaleidoscope and the Macros plugin.

#### New `macroAction()` function

There is a new version of the `macroAction()` function, which is the entry point
for user-defined Macros code. The old version takes two integer parameters, with
the following call signature:

```c++
const macro_t* macroAction(uint8_t macro_id, uint8_t key_state)
```

If your sketch has this function, with a `key_state` bitfield parameter, it
might still work as expected, but depending on the specifics of the code that
gets called from it, your macros might not work as expected. Either way, you
should update that function to the new version, which takes a `KeyEvent`
reference as its second parameter:

```c++
const macro_t* macroAction(uint8_t macro_id, KeyEvent &event)
```

For simple macros, it is a simple matter of replacing `key_state` in the body of
the `macroAction()` code with `event.state`. This covers most cases where all
that's done is a call to `Macros.type()`, or a `MACRO()` or `MACRODOWN()`
sequence is returned.


#### Using `MACRO()` and `MACRODOWN()`

The preprocessor macro `MACRODOWN()` has been deprecated, because the event
handler for Macros is no longer called every cycle, but only when a key is
either pressed or released.  Instead of using `return MACRODOWN()`, you should
test for a toggle-on event in `macroAction()` and use `MACRO()` instead.  If you
previously had something like the following in your `macroAction()` function:

```c++
switch(macro_id) {
case MY_MACRO:
  return MACRODOWN(T(X), T(Y), T(Z));
}
```

...you should replace that with:

```c++
switch(macro_id) {
case MY_MACRO:
  if (keyToggledOn(event.state))
    return MACRO(T(X), T(Y), T(Z));
}
```

...or, for a group of macros that should only fire on keypress:

```c++
if (keyToggledOn(event.state)) {
  switch(macro_id) {
  case MY_MACRO:
    return MACRO(T(X), T(Y), T(Z));
  case MY_OTHER_MACRO:
    return MACRO(T(A), T(B), T(C));
  }
}
```

#### Releasing keys with `Macros.release()` or `U()`/`Ur()`/`Uc()`

Macros now operates by manipulating keys on a small supplemental virtual
keyboard when using `Macros.press()` and `Macros.release()` (which are called by
`D()` and `U()`, _et al_, respectively).  This means that it has no built-in
facility for releasing other keys that are held on the keyboard.  For example,
if you had a Macro that removed `shift` keycodes from the HID report in the
past, it won't work.  For example:

```c++
  case KEY_COMMA:
    if (keyToggledOn(event.state)) {
      if (Kaleidoscope.hid().keyboard().wasModifierKeyActive(Key_LeftShift)) {
        return MACRO(U(LeftShift), T(Comma), D(LeftShift));
      } else {
        return MACRO(T(M));
      }
    }
```

In this case, holding a physical `Key_LeftShift` and pressing `M(KEY_COMMA)`
will not cause the held `shift` to be released, and you'll get a `<` instead of
the intended `,` (depending on the OS keymap).  To accomplish this, you'll need
a small plugin like the following in your sketch:

```c++
namespace kaleidoscope {
namespace plugin {

// When activated, this plugin will suppress any `shift` key (including modifier
// combos with `shift` a flag) before it's added to the HID report.
class ShiftBlocker : public Plugin {

 public:
  EventHandlerResult onAddToReport(Key key) {
    if (active_ && key.isKeyboardShift())
      return EventHandlerResult::ABORT;
    return EventHandlerResult::OK;
  }

  void enable() {
    active_ = true;
  }
  void disable() {
    active_ = false;
  }

 private:
  bool active_{false};

};

} // namespace plugin
} // namespace kaleidoscope

kaleidoscope::plugin::ShiftBlocker ShiftBlocker;
```

You may also need to define a function to test for held `shift` keys:

```c++
bool isShiftKeyHeld() {
  for (Key key : kaleidoscope::live_keys.all()) {
    if (key.isKeyboardShift())
      return true;
  }
  return false;
}
```

Then, in your `macroAction()` function:

```c++
  if (keyToggledOn(event.state)) {
    switch (macro_id) {
    case MY_MACRO:
      if (isShiftKeyHeld()) {
        ShiftBlocker.enable();
        Macros.tap(Key_Comma);
        ShiftBlocker.disable();
      } else {
        Macros.tap(Key_M);
      }
      return MACRO_NONE;
    }
  }
```

In many simple cases, such as the above example, an even better solution is to
use the CharShift plugin instead of Macros.

#### Code that calls `handleKeyswitchEvent()` or `pressKey()`

It is very likely that if you have custom code that calls
`handleKeyswitchEvent()` or `pressKey()` directly, it will no longer function
properly after upgrading. To adapt this code to the new `KeyEvent` system
requires a deeper understanding of the changes to Kaleidoscope, but likely
results in much simpler Macros code.

The first thing that is important to understand is that the `macroAction()`
function will now only be called when a Macros `Key` toggles on or off, not once
per cycle while the key is held.  This is because the new event handling code in
Kaleidoscope only calls plugin handlers in those cases, dealing with one event
at a time, in a single pass through the plugin event handlers (rather than one
pass per active key)--and only sends a keyboard HID report in response to those
events, not once per scan cycle.

This means that any Macros code that is meant to keep keycodes in the keyboard
HID report while the Macros key is held needs to be changed. For example, if a
macro contained the following code:

```c++
if (keyIsPressed(key_state)) {
  Runtime.hid().keyboard().pressKey(Key_LeftShift);
}
```

...that wouldn't work quite as expected, because as soon as the next key is
pressed, a new report would be generated without ever calling `macroAction()`,
and therefore that change to the HID report would not take place, effectively
turning off the `shift` modifier immediately before sending the report with the
keycode that it was intended to modify.

Furthermore, that `shift` modifier would never even get sent in the first place,
because the HID report no longer gets cleared at the beginning of every
cycle. Now it doesn't get cleared until _after_ the plugin event handlers get
called (in the case of Macros, that's `onKeyEvent()`, which calls the
user-defined `macroAction()` function), so any changes made to the HID report
from that function will be discarded before it's sent.

Instead of the above, there are two new mechanisms for keeping keys active while
a Macros key is pressed:

##### Alter the `event.key` value

If your macro only needs to keep a single `Key` value active after running some
code, and doesn't need to run any custom code when the key is released, the
simplest thing to do is to override the event's `Key` value:

```c++
if (keyToggledOn(event.state)) {
  // do some macro action(s)
  event.key = Key_LeftShift;
}
```

This will (temporarily) replace the Macros key with the value assigned (in this
case, `Key_LeftShift`), starting immediately after the `macroAction()` function
returns, and lasting until the key is released. This key value can include
modifier flags, or it can be a layer-shift, or any other valid `Key` value
(though it won't get processed by plugins that are initialized before Macros in
`KALEIDOSCOPE_INIT_PLUGINS()`, and Macros itself won't act on the value, if it
gets replaced by a different Macros key).

##### Use the supplemental Macros `Key` array

The Macros plugin now contains a small array of `Key` values that will be
included when building HID reports triggered by subsequent, non-Macros
events. To use it, just call one (or more) of the following methods:

```c++
Macros.press(key);
Macros.release(key);
Macros.tap(key)
```

Each one of these functions generates a new artificial key event, and processes
it (including sending a HID report, if necessary). For `press()` and
`release()`, it also stores the specified key's value in the Macros supplemental
`Key` array. In the case of the `tap()` function, it generates matching press
and release events, but skips storing them, assuming that no plugin will
generate an intervening event. All of the events generated by these functions
will be marked `INJECTED`, which will cause Macros itself (and many other
plugins) to ignore them.

This will allow you to keep multiple `Key` values active while a Macros key is
held, while leaving the Macros key itself active, enabling more custom code to
be called on its release.  Note that whenever a Macros key is released, the
supplemental key array is cleared to minimize the chances of keycodes getting
"stuck". It is still possible to write a macro that will cause values to persist
in this array, however, by combining both a sequence that uses key presses
without matched releases _and_ replacing `event.key` (see above) in the same
macro.

##### Borrow an idle key (not recommended)

It's also possible to "borrow" one (or more) idle keys on the keyboard by
searching the `live_keys[]` array for an empty entry, and generating a new event
with the address of that key. This is not recommended because surprising things
can happen if that key is then pressed and released, but it's still an option
for people who like to live dangerously.

#### Code that calls `sendReport()`

Calling `sendReport()` directly from a macro is now almost always unnecessary.
Instead, a call to `Runtime.handleKeyEvent()` will result in a keyboard HID
report being sent in response to the generated event without needing to make it
explicit.

#### Code that uses `Macros.key_addr`

This variable is deprecated. Instead, using the new `macroAction(id, event)`
function, the address of the Macros key is available via the `event.addr`
variable.

#### Working with other plugins

##### Plugin-specific `Key` values

When the the Macros plugin generates events, it marks the event state as
`INJECTED` in order to prevent unbounded recursion (Macros ignores injected
events).  This causes most other plugins to ignore the event, as well.
Therefore, including a plugin-specific key (e.g. a OneShot modifier such as
`OSM(LeftAlt)`) will most likely be ignored by the target plugin, and will
therefore not have the desired effect. This applies to any calls to
`Macros.play()` (including returning `MACRO()` from `macroAction()`),
`Macros.tap()`, `Macros.press()`, and `Macros.release()`.

##### Physical event plugins

Macros cannot usefully produce events handled by plugins that implement the
`onKeyswitchEvent()` handler, such as Qukeys, TapDance, and Leader.  To make
those plugins work with Macros, it's necessary to have the other plugin produce
a Macros key, not the other way around. A `macroAction()` function must not call
`Runtime.handleKeyswitchEvent()`.

##### OneShot

This is one plugin that you might specifically want to use with a macro,
generally at the end of a sequence.  For example, a macro for ending one
sentence and beginning the next one might print a period followed by a space
(`. `), then a OneShot shift key tap, so that the next character will be
automatically capitalized. The problem, as mentioned before is that the
following won't work:

```c++
MACRO(Tc(Period), Tc(Spacebar), Tr(OSM(LeftShift)))
```

...because OneShot will ignore the `INJECTED` event.  One solution is to change
the value of `event.key`, turning the pressed Macros key into a OneShot
modifier. This will only work if Macros is registered before OneShot in
`KALEIDOSCOPE_INIT_PLUGINS()`:

```c++
const macro_t* macroNewSentence(KeyEvent &event) {
  if (keyToggledOn(event.state)) {
    event.key = OSM(LeftShift);
    return MACRO(Tc(Period), Tc(Spacebar));
  }
  return MACRO_NONE;
}
```

A more robust solution is to explicitly call `Runtime.handleKeyEvent()`, but
this is more complex, because you'll need to prevent the Macros key from
clobbering the OneShot key in the `live_keys[]` array:

```c++
void macroNewSentence(KeyEvent &event) {
  if (keyToggledOn(event.state)) {
    Macros.tap(Key_Period);
    Macros.tap(Key_Spacebar);
    event.key = OSM(LeftShift);
    kaleidoscope::Runtime.handleKeyEvent(event);
    // Last, we invalidate the current event's key address to prevent the Macros
    // key value from clobbering the OneShot shift.
    event.key = Key_NoKey;
    event.addr.clear();
  }
}
```

### Removed `kaleidoscope-builder`

`kaleidoscope-builder` has been removed.

We replaced it with a new Makefile based build system that uses `arduino-cli` instead of of the full Arduino IDE. This means that you can now check out development copies of Kaleidoscope into any directory, using the `KALEIDOSCOPE_DIR` environment variable to point to your installation.

### OneShot meta keys

The special OneShot keys `OneShot_MetaStickyKey` & `OneShot_ActiveStickyKey` are no longer handled by the OneShot plugin directly, but instead by a separate OneShotMetaKeys plugin. If you use these keys in your sketch, you will need to add the new plugin, and register it after OneShot in `KALEIDOSCOPE_INIT_PLUGINS()` for those keys to work properly.

### Repository rearchitecture

To improve build times and to better highlight Kaleidoscope's many plugins, plugins have been move into directories inside the Kaleidoscope directory.

The "breaking change" part of this is that git checkouts of Kaleidoscope are no longer directly compatible with the Arduino IDE, since plugins aren't in a directory the IDE looks in. They are, of course, visible to tools using our commandline build infrastructure / Makefiles.

When we build releases, those plugins are moved into directories inside the arduino platform packages for each architecture to make them visible to the Arduino IDE.

### Layer system switched to activation order

The layer system used to be index-ordered, meaning that we'd look keys up on
layers based on the _index_ of active layers. Kaleidoscope now uses activation
order, which looks up keys based on the order of layer activation.

The following functions have been removed as of **2021-01-01**:

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

 [leader]: plugins/Kaleidoscope-Leader.md

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

### OneShot

Older versions of the plugin were based on `Key` values; OneShot is now based on
`KeyAddr` coordinates instead, in order to improve reliability and
functionality.

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

These functions have been removed as of **2020-12-31**:

- `Qukeys.setTimeout(millis)`
- `Qukeys.setReleaseDelay(millis)`
- `Qukey(layer, row, col, alternate_key)`

### TypingBreaks

Older versions of the plugin used to provide EEPROM storage for the settings only optionally, when it was explicitly enabled via the `TypingBreaks.enableEEPROM()` method. Similarly, the Focus hooks were optional too.

Storing the settable settings in EEPROM makes it depend on `Kaleidoscope-EEPROM-Settings`, which should be initialized before this plugin is.

### Redial

Older versions of the plugin required one to set up `Key_Redial` manually, and let the plugin know about it via `Redial.key`. This is no longer required, as the plugin sets up the redial key itself. As such, `Redial.key` was removed, and `Key_Redial` is defined by the plugin itself. To upgrade, simply remove your definition of `Key_Redial` and the `Redial.key` assignment from your sketch.

### Key masking has been removed

Key masking was a band-aid introduced to avoid accidentally sending unintended keys when key mapping changes between a key being pressed and released. Since the introduction of keymap caching, this is no longer necessary, as long as we can keep the mapping consistent. Users of key masking are encouraged to find ways to use the caching mechanism instead.

As an example, if you had a key event handler that in some cases masked a key, it should now map it to `Key_NoKey` instead, until released.

The masking API has been removed on **2021-01-01**

## Deprecated APIs and their replacements

### Leader plugin

The `Leader.inject()` function is deprecated.  Please call `Runtime.handleKeyEvent()` directly instead.

Direct access to the `Leader.time_out` configuration variable is deprecated.  Please use the `Leader.setTimeout(ms)` function instead.

### Source code and namespace rearrangement

With the move towards a monorepo-based source, some headers have moved to a new location, and plenty of plugins moved to a new namespace (`kaleidoscope::plugin`). This means that the old headers, and some old names are deprecated. The old names no longer work.

The following headers and names have changed:

- `layers.h`, `key_defs_keymaps.h` and `macro_helpers.h` are obsolete, and should not be included in the first place, as `Kaleidoscope.h` will pull them in. In the rare case that one needs them, prefixing them with `kaleidoscope/` is the way to go. Of the various headers provided under the `kaleidoscope/` space, only `kaleidoscope/macro_helpers.h` should be included directly, and only by hardware plugins that can't pull `Kaleidoscope.h` in due to circular dependencies.
- `LED-Off.h`, provided by [LEDControl](plugins/Kaleidoscope-LEDControl.md) is obsolete, the `LEDOff` LED mode is automatically provided by `Kaleidoscope-LEDControl.h`. The `LED-Off.h` includes can be safely removed.
- `LEDUtils.h` is automatically pulled in by `Kaleiodscope-LEDControl.h`, too, and there's no need to directly include it anymore.
- Plugins that implement LED modes should subclass `kaleidoscope::plugin::LEDMode` instead of `kaleidoscope::LEDMode`.
- [GhostInTheFirmware](plugins/Kaleidoscope-GhostInTheFirmware.md) had the `kaleidoscope::GhostInTheFirmware::GhostKey` type replaced by `kaleidoscope::plugin::GhostInTheFirmware::GhostKey`.
- [HostOS](plugins/Kaleidoscope-HostOS.md) no longer provides the `Kaleidoscope/HostOS-select.h` header, and there is no backwards compatibility header either.
- [Leader](plugins/Kaleidoscope-Leader.md) had the `kaleidoscope::Leader::dictionary_t` type replaced by `kaleidoscope::plugin::Leader::dictionary_t`.
- [LED-AlphaSquare](plugins/Kaleidoscope-LED-AlphaSquare.md) used to provide extra symbol graphics in the `kaleidoscope::alpha_square::symbols` namespace. This is now replaced by `kaleidoscope::plugin::alpha_square::symbols`.
- [LEDEffect-SolidColor](plugins/Kaleidoscope-LEDEffect-SolidColor.md) replaced the base class  - `kaleidoscope::LEDSolidColor` - with `kaleidoscope::plugin::LEDSolidColor`.
- [Qukeys](plugins/Kaleidoscope-Qukeys.md) had the `kaleidoscope::Qukey` type replaced by `kaleidoscope::plugin::Qukey`.
- [ShapeShifter](plugins/Kaleidoscope-ShapeShifter.md) had the `kaleidoscope::ShapeShifter::dictionary_t` type replaced by `kaleidoscope::plugin::ShapeShifter::dictionary_t`.
- [SpaceCadet](plugins/Kaleidoscope-SpaceCadet.md) had the `kaleidoscope::SpaceCadet::KeyBinding` type replaced by `kaleidoscope::plugin::SpaceCadet::KeyBinding`.
- [Syster](plugins/Kaleidoscope-Syster.md) had the `kaleidoscope::Syster::action_t` type replaced by `kaleidoscope::plugin::Syster::action_t`.
- [TapDance](plugins/Kaleidoscope-TapDance.md) had the `kaleidoscope::TapDance::ActionType` type replaced by `kaleidoscope::plugin::TapDance::ActionType`.

# Removed APIs

### Removed on 2022-03-03

#### Pre-`KeyEvent` event handler hooks

The old event handler `onKeyswitchEvent(Key &key, KeyAddr addr, uint8_t state)` was removed on **2022-03-03**.  It has been replaced with the new `onKeyEvent(KeyEvent &event)` handler (and, in some special cases the `onKeyswitchEvent(KeyEvent &event)` handler).  Plugins using the deprecated handler will need to be rewritten to use the new one(s).

The old event handler `beforeReportingState()` was removed on **2022-03-03**.  It has been replaced with the new `beforeReportingState(KeyEvent &event)` handler.  However, the new handler will be called only when a report is being sent (generally in response to a key event), not every cycle, like the old one.  It was common practice in the past for plugins to rely on `beforeReportingState()` being called every cycle, so when adapting to the `KeyEvent` API, it's important to check for code that should be moved to `afterEachCycle()` instead.

#### `::handleKeyswitchEvent(Key key, KeyAddr key_addr, uint8_t state)`

The old master function for processing key "events" was removed on **2022-03-03**.  Functions that were calling this function should be rewritten to call `kaleidoscope::Runtime.handleKeyEvent(KeyEvent event)` instead.

#### `Keyboard::pressKey(Key key, bool toggled_on)`

This deprecated function was removed on **2022-03-03**.  Its purpose was to handle rollover events for keys that include modifier flags, and that handling is now done elsewhere.  Any code that called it should now simply call `Keyboard::pressKey(Key key)` instead, dropping the second argument.

#### Old layer key event handler functions

The deprecated `Layer.handleKeymapKeyswitchEvent()` function was removed on **2022-03-03**.  Any code that called it should now call `Layer.handleLayerKeyEvent()` instead, with `event.addr` set to the appropriate `KeyAddr` value if possible, and `KeyAddr::none()` otherwise.

The deprecated `Layer.eventHandler(key, addr, state)` function was removed on **2022-03-03**.  Any code that refers to it should now call call `handleLayerKeyEvent(KeyEvent(addr, state, key))` instead.

#### Keymap cache functions

The deprecated `Layer.updateLiveCompositeKeymap()` function was removed on **2022-03-03**.  Plugin and user code probably shouldn't have been calling this directly, so there's no direct replacement for it.  If a plugin needs to make changes to the `live_keys` structure (equivalent in some circumstances to the old "live composite keymap"), it can call `live_keys.activate(addr, key)`, but there are probably better ways to accomplish this goal (e.g. simply changing the value of `event.key` from an `onKeyEvent(event)` handler).

The deprecated `Layer.lookup(addr)` function was removed on **2022-03-03**.  Please use `Runtime.lookupKey(addr)` instead in most circumstances.  Alternatively, if you need information about the current state of the keymap regardless of any currently active keys (which may have values that override the keymap), use `Layer.lookupOnActiveLayer(addr)` instead.

#### `LEDControl.syncDelay` configuration variable

Direct access to this configuration variable was removed on **2022-03-03**.  Please use `LEDControl.setInterval()` to set the interval between LED updates instead.

#### Obsolete active macros array removed

The deprecated `Macros.active_macro_count` variable was removed on **2022-03-03**.  Any references to it are obsolete, and can simply be removed.

The deprecated `Macros.active_macros[]` array was removed on **2022-03-03**.  Any references to it are obsolete, and can simply be removed.

The deprecated `Macros.addActiveMacroKey()` function was removed on **2022-03-03**.  Any references to it are obsolete, and can simply be removed.

#### Pre-`KeyEvent` Macros API

This is a brief summary of specific elements that were removed.  There is a more comprehensive guide to upgrading existing Macros user code in the [Breaking Changes](#breaking-changes) section, under [Macros](#macros).

Support for deprecated form of the `macroAction(uint8_t macro_id, uint8_t key_state)` function was removed on **2022-03-03**.  This old form must be replaced with the new `macroAction(uint8_t macro_id, KeyEvent &event)` for macros to continue working.

The `Macros.key_addr` public variable was removed on **2022-03-03**.  To get access to the key address of a Macros key event, simply refer to `event.addr` from within the new `macroAction(macro_id, event)` function.

The deprecated `MACRODOWN()` preprocessor macro was removed on **2022-03-03**.  Since most macros are meant to be triggered only by keypress events (not key release), and because `macroAction()` does not get called every cycle for held keys, it's better to simply do one test for `keyToggledOn(event.state)` first, then use `MACRO()` instead.

#### ActiveModColor public variables

The following deprecated `ActiveModColorEffect` public variables were removed on **2022-03-03**.  Please use the following methods instead:

 - For `ActiveModColor.highlight_color`, use `ActiveModColor.setHighlightColor(color)`
 - For `ActiveModColor.oneshot_color`, use `ActiveModColor.setOneShotColor(color)`
 - For `ActiveModColor.sticky_color`, use `ActiveModColor.setStickyColor(color)`

#### OneShot public variables

The following deprecated `OneShot` public variables were removed on **2022-03-03**.  Please use the following methods instead:

 - For `OneShot.time_out`, use `OneShot.setTimeout(ms)`
 - For `OneShot.hold_time_out`, use `OneShot.setHoldTimeout(ms)`
 - For `OneShot.double_tap_time_out`, use `OneShot.setDoubleTapTimeout(ms)`

#### Deprecated OneShot API functions

OneShot was completely rewritten in early 2021, and now is based on `KeyAddr` values (as if it keeps physical keys pressed) rather than `Key` values (with no corresponding physical key location).  This allows it to operate on any `Key` value, not just modifiers and layer shifts.

The deprecated `OneShot.inject(key, key_state)` function was removed on **2022-03-03**.  Its use was very strongly discouraged, and is now unavailable.  See below for alternatives.

The deprecated `OneShot.isActive(key)` function was removed on **2022-03-03**.  There is a somewhat equivalent `OneShot.isActive(KeyAddr addr)` function to use when the address of a key that might be currently held active by OneShot is known.  Any code that needs information about active keys is better served by not querying OneShot specifically.

The deprecated `OneShot.isSticky(key)` function was removed on **2022-03-03**.  There is a somewhat equivalent `OneShot.isStick(KeyAddr addr)` function to use when the address of a key that may be in the one-shot sticky state is known.

The deprecated `OneShot.isPressed()` function was removed on **2022-03-03**.  It was already devoid of functionality, and references to it can be safely removed.

The deprecated `OneShot.isModifierActive(key)` function was removed on **2022-03-03**.  OneShot modifiers are now indistinguishable from other modifier keys, so it is better for client code to do a more general search of `live_keys` or to use another mechanism for tracking this state.

#### `HostPowerManagement.enableWakeup()`

This deprecated function was removed on **2022-03-03**.  The firmware now supports wakeup by default, so any references to it can be safely removed.

#### `EEPROMSettings.version(uint8_t version)`

This deprecated function was removed on **2022-03-03**.  The information stored is not longer intended for user code to set, but instead is used internally.

#### Model01-TestMode plugin

This deprecated plugin was removed on **2022-03-03**.  Please use the more generic HardwareTestMode plugin instead.

### Removed on 2020-10-10

#### Deprecation of the HID facade

With the new Device APIs it became possible to replace the HID facade (the `kaleidoscope::hid` family of functions) with a driver. As such, the old APIs are deprecated, and was removed on 2020-10-10. Please use `Kaleidoscope.hid()` instead.

#### Implementation of type Key internally changed from C++ union to class

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

The [EEPROM-Keymap](plugins/Kaleidoscope-EEPROM-Keymap.md) plugin had its `setup()` method changed, the formerly optional `method` argument is now obsolete and unused. It can be safely removed.

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
