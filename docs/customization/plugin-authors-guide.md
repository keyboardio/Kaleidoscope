# How to write a Kaleidoscope plugin

This is a brief guide intended for those who want to write custom Kaleidoscope plugins.  It covers basic things you'll need to know about how Kaleidoscope calls plugin event handlers, and how it will respond to actions taken by those plugins.

## What can a plugin do?

There are many things that Kaleidoscope plugins are capable of, from LED effects, serial communication with the host, altering HID reports, and interacting with other plugins.  It's useful to break these capabilities down into some broad categories, based on the types of input a plugin can respond to.

- Key events (key switches toggling on and off)
- Focus commands (sent to the keyboard from software on the host via the serial port)
- LED updates
- Keymap layer changes
- Timers

## An example plugin

To make a Kaleidoscope plugin, we create a subclass of the `kaleidoscope::Plugin` class, usually in the `kaleidoscope::plugin` namespace:

```c++
namespace kaleidoscope {
namespace plugin {

class MyPlugin : public Plugin {};

} // namespace kaleidoscope
} // namespace plugin
```

This code can be placed in a separate C++ source file, but it's simplest to just define it right in the sketch's \*.ino file for now.

By convention, we create a singleton object named like the plugin's class in the global namespace.  This is typical of Arduino code.

```c++
kaleidoscope::plugin::MyPlugin MyPlugin;
```

Next, in order to connect that plugin to the Kaleidoscope event handler system, we need to register it in the call to the preprocessor macro `KALEIDOSCOPE_INIT_PLUGINS()` in the sketch:

```c++
KALEIDOSCOPE_INIT_PLUGINS(MyPlugin, OtherPlugin);
```

To make our plugin do anything useful, we need to add [[event-handler-hooks]] to it.  This is how Kaleidoscope delivers input events to its registered plugins.  Here's an example:

```c++
class MyPlugin : public Plugin {
 public:
  EventHandlerResult onKeyEvent(KeyEvent &event);
};
```

This will result in `MyPlugin.onKeyEvent()` being called (along with other plugins' `onKeyEvent()` methods) when Kaleidoscope detects a key state change.  This function returns one of three `EventHandlerResult` values:

- `EventHandlerResult::OK` indicates that Kaleidoscope should proceed on to the event handler for the next plugin in the chain.
- `EventHandlerResult::ABORT` indicates that Kaleidoscope should stop processing immediately, and treat the event as if it didn't happen.
- `EventHandlerResult::EVENT_CONSUMED` stops event processing like `ABORT`, but records that the key is being held.

The `onKeyEvent()` method takes one argument: a reference to a `KeyEvent` object, which is a simple container for these essential bits of information:

- `event.addr` — the physical location of the keyswitch, if any
- `event.state` — a bitfield containing information on the current and previous state of the keyswitch (from which we can find out if it just toggled on or toggled off)
- `event.key` — a 16-bit `Key` value containing the contents looked up from the sketch's current keymap (if the key just toggled on) or the current live value of the key (if the key just toggled off)

Because the `KeyEvent` parameter is passed by (mutable) reference, our plugin's `onKeyEvent()` method can alter the components of the event, causing subsequent plugins (and, eventually, Kaleidoscope itself) to treat it as if it was a different event.  In practice, except in very rare cases, the only member of a `KeyEvent` that a plugin should alter is `event.key`.  Here's a very simple `onKeyEvent()` handler that changes all `X` keys into `Y` keys:

```c++
EventHandlerResult onKeyEvent(KeyEvent &event) {
  if (event.key == Key_X)
    event.key = Key_Y;
  return EventHandlerResult::OK;
}
```

### The difference between `ABORT` & `EVENT_CONSUMED`

Here's a plugin that will suppress all `X` key events:

```c++
EventHandlerResult onKeyEvent(KeyEvent &event) {
  if (event.key == Key_X)
    return EventHandlerResult::ABORT;
  return EventHandlerResult::OK;
}
```

Here's an almost identical plugin that has an odd failure mode:

```c++
EventHandlerResult onKeyEvent(KeyEvent &event) {
  if (event.key == Key_X)
    return EventHandlerResult::EVENT_CONSUMED;
  return EventHandlerResult::OK;
}
```

In this case, when an `X` key is pressed, no Keyboard HID report will be generated and sent to the host, but the key will still be recorded by Kaleidoscope as "live".  If we hold that key down and press a `Y` key, we will suddenly see both `x` _and_ `y` in the output on the host.  This is because returning `ABORT` suppresses the key event entirely, as if it never happened, whereas `EVENT_CONSUMED` signals to Kaleidoscope that the key should still become "live", but that no further processing is necessary.  In this case, since we want to suppress all `X` keys entirely, we should return `ABORT`.

### A complete in-sketch plugin

Here's an example of a very simple plugin, defined as it would be in a firmware sketch (e.g. a `*.ino` file):

```c++
namespace kaleidoscope {
namespace plugin {

class KillX : public Plugin {
 public:
  EventHandlerResult onKeyEvent(KeyEvent &event) {
    if (event.key == Key_X)
      return EventHandlerResult::ABORT;
    return EventHandlerResult::OK;
  }
};

} // namespace kaleidoscope
} // namespace plugin

kaleidoscope::plugin::KillX;
```

On its own, this plugin won't have any effect unless we register it later in the sketch like this:

```c++
KALEIDOSCOPE_INIT_PLUGINS(KillX);
```

Note: `KALEIDOSCOPE_INIT_PLUGINS()` should only appear once in a sketch, with a list of all the plugins to be registered.

## Plugin registration order

Obviously, the `KillX` plugin isn't very useful.  But more important, it's got a potential problem.  Suppose we had another plugin defined, like so:

```c++
namespace kaleidoscope {
namespace plugin {

class YtoX : public Plugin {
 public:
  EventHandlerResult onKeyEvent(KeyEvent &event) {
    if (event.key == Key_Y)
      event.key = Key_X;
    return EventHandlerResult::OK;
  }
};

} // namespace kaleidoscope
} // namespace plugin

kaleidoscope::plugin::YtoX;
```

`YtoX` changes any `Y` key to an `X` key.  These two plugins both work fine on their own, but when we put them together, we get some undesirable behavior.  Let's try it this way first:

```c++
KALEIDOSCOPE_INIT_PLUGINS(YtoX, KillX);
```

This registers both plugins' event handlers with Kaleidoscope, in order, so for each `KeyEvent` generated in response to a keyswitch toggling on or off, `YtoX.onKeyEvent(event)` will get called first, then `KillX.onKeyEvent(event)` will get called.

If we press `X`, the `YtoX` plugin will effectively ignore the event, allowing it to pass through to `KillX`, which will abort the event.

If we press `Y`, `YtoX.onKeyEvent()` will change `event.key` from `Key_Y` to `Key_X`.  Then, `KillX.onKeyEvent()` will abort the event.  As a result, both `X` and `Y` keys will be suppressed by the combination of the two plugins.

---

Now, let's try the same two plugins in the other order:

```c++
KALEIDOSCOPE_INIT_PLUGINS(KillX, YtoX);
```

If we press `X`, its keypress event will get aborted by `KillX.onKeyEvent()`, and that key will not become live, so when it gets released, the event generated won't have the value `Key_X`, but will instead by `Key_Inactive`, which will not result in anything happening, either from the plugins or from Kaleidoscope itself.

Things get interesting if we press and release `Y`, though.  First, `KillX.onKeyEvent()` will simply return `OK`, allowing `YtoX.onKeyEvent()` to change `event.key` from `Key_Y` to `Key_X`, causing that `Key_X` to become live, and sending its keycode to the host in the Keyboard USB HID report.  That's all as expected, but then we release the key, and that's were it goes wrong.

`KillX.onKeyEvent()` doesn't distinguish between presses and releases.  When a key toggles off, rather than looking up that key's value in the keymap, Kaleidoscope takes it from the live keys array.  That means that `event.key` will be `Key_X` when `KillX.onKeyEvent()` is called, which will result in that event being aborted.  And when an event is aborted, the key's entry in the live keys array doesn't get updated, so Kaleidoscope will treat it as if the key is still held after release.  Thus, far from preventing the keycode for `X` getting to the host, it keeps that key pressed forever!  The `X` key becomes "stuck on" because the plugin suppresses both key _presses_ and key _releases_.

### Differentiating between press and release events

There is a solution to this problem, which is to have `KillX` suppress `Key_X` toggle-on events, but not toggle-off events:

```c++
EventHandlerResult KillX::onKeyEvent(KeyEvent &event) {
  if (event.key == Key_X && keyToggledOn(event.state))
    return EventHandlerResult::ABORT;
  return EventHandlerResult::OK;
}
```

Kaleidoscope provides `keyToggledOn()` and `keyToggledOff()` functions that operate on the `event.state` bitfield, allowing plugins to differentiate between the two different event states.  With this new version of the `KillX` plugin, it won't keep an `X` key live, but it will stop one from _becoming_ live.

Our two plugins still yield results that depend on registration order in `KALEIDOSCOPE_INIT_PLUGINS()`, but the bug where the `X` key becomes "stuck on" is gone.

It is very common for plugins to only act on key toggle-on events, or to respond differently to toggle-on and toggle-off events.

## Timers

Another thing that many plugins need to do is handle timeouts.  For example, the OneShot plugin keeps certain keys live for a period of time after those keys are released.  Kaleidoscope provides some infrastructure to help us keep track of time, starting with the `afterEachCycle()` "event" handler function.

The `onKeyEvent()` handlers only get called in response to keyswitches toggling on and off (or as a result of plugins calling `Runtime.handleKeyEvent()`).  If the user isn't actively typing for a period, its `onKeyEvent()` handler won't get called at all, so it's not very useful to check timers in that function.  Instead, if we need to know if a timer has expired, we need to do it in a function that gets called regularly, regardless of input.  The `afterEachCycle()` handler gets called once per cycle, guaranteed.

This is what an `afterEachCycle()` handler looks like:

```c++
EventHandlerResult afterEachCycle() {
  return EventHandlerResult::OK;
}
```

It returns an `EventHandlerResult`, like other event handlers, but this one's return value is ignored by Kaleidoscope; returning `ABORT` or `EVENT_CONSUMED` has no effect on other plugins.

In addition to this, we need a way to keep track of time.  For this, Kaleidoscope provides the function `Runtime.millisAtCycleStart()`, which returns an unsigned integer representing the number of milliseconds that have elapsed since the keyboard started.  It's a 32-bit integer, so it won't overflow until about one month has elapsed, but we usually want to use as few bytes of RAM as possible on our MCU, so most timers store only as many bytes as needed, usually a `uint16_t`, which overflows after about one minute, or even a `uint8_t`, which is good for up to a quarter of a second.

We need to use an integer type that's at least as big as the longest timeout we expect to be used, but integer overflow can still give us the wrong answer if we check it by naïvely comparing the current time to the time at expiration, so Kaleidoscope provides a timeout-checking service that's handles the integer overflow properly: `Runtime.hasTimeExpired(start_time, timeout)`.  To use it, your plugin should store a timestamp when the timer begins, using `Runtime.millisAtCycleStart()` (usually set in response to an event in `onKeyEvent()`).  Then, in its `afterEachCycle()` call `hasTimeExpired()`:

```c++
namespace kaleidoscope {
namespace plugin {

class MyPlugin : public Plugin {
 public:
  constexpr uint16_t timeout = 500;

  EventHandlerResult onKeyEvent(KeyEvent &event) {
    if (event.key == Key_X && keyToggledOn(event.state)) {
      start_time_ = Runtime.millisAtCycleStart();
      timer_running_ = true;
    }
    return EventHandlerResult::OK;
  }

  EventHandlerResult afterEachCycle() {
    if (Runtime.hasTimeExpired(start_time_, timeout)) {
      timer_running_ = false;
      // do something...
    }
    return EventHandlerResult::OK;
  }

 private:
  bool timer_running_ = false;
  uint16_t start_time_;
};

} // namespace kaleidoscope
} // namespace plugin

kaleidoscope::plugin::MyPlugin;
```

In the above example, the private member variable `start_time_` and the constant `timeout` are the same type of unsigned integer (`uint16_t`), and we've used the additional boolean `timer_running_` to keep from checking for timeouts when `start_time_` isn't valid.  This plugin does something (unspecified) 500 milliseconds after a `Key_X` toggles on.

## Creating additional events

Another thing we might want a plugin to do is generate "extra" events that don't correspond to physical state changes.  An example of this is the Macros plugin, which might turn a single keypress into a series of HID reports sent to the host.  Let's build a simple plugin to illustrate how this is done, by making a key type a string of characters, rather than a single one.

For the sake of simplicity, let's make the key `H` result in the string `Hi!` being typed (from the point of view of the host computer).  To do this, we'll make a plugin with an `onKeyEvent()` handler (because we want it to respond to a particular keypress event), which will call `Runtime.handleKeyEvent()` to generate new events sent to the host.

The first thing we need to understand to do this is how to use the `KeyEvent()` constructor to create a new `KeyEvent` object.  For example:

```c++
KeyEvent event = KeyEvent(KeyAddr::none(), IS_PRESSED, Key_H);
```

This creates a `KeyEvent` where `event.addr` is an invalid address that doesn't correspond to a physical keyswitch, `event.state` has only the `IS_PRESSED` bit set, but not `WAS_PRESSED`, which corresponds to a key toggle-on event, and `event.key` is set to `Key_H`.

We can then cause Kaleidoscope to process this event, including calling plugin handlers, by calling `Runtime.handleKeyEvent(event)`:

```c++
  EventHandlerResult onKeyEvent(KeyEvent &event) {
    if (event.key == Key_H && keyToggledOn(event.state)) {

      // Create and send the `H` (shift + h)
      KeyEvent new_event = KeyEvent(KeyAddr::none(), IS_PRESSED, LSHIFT(Key_H));
      Runtime.handleKeyEvent(new_event);

      // Change the key value and send `i`
      new_event.key = Key_I;
      Runtime.handleKeyEvent(new_event);

      // Change the key value and send `!` (shift + 1)
      new_event.key = LSHIFT(Key_1);
      Runtime.handleKeyEvent(new_event);

      return EventHandlerResult::ABORT;
    }
    return EventHandlerResult::OK;
  }
```

A few shortcuts were taken with this plugin that are worth pointing out.  First, you may have noticed that we didn't send any key _release_ events, just three presses.  This works, but there's a small chance that it could cause problems for some plugin that's trying to match key presses and releases.  To be nice (or pedantic, if you will), we could also send the matching release events, but this is probably not necessary in this case, because we've used an invalid key address (`KeyAddr::none()`) for these generated events.  This means that Kaleidoscope will not be recording these events as held keys.  If we had used valid key addresses (corresponding to physical keyswitches) instead, it would be more important to send matching release events to keep keys from getting "stuck" on.  For example, we could just use the address of the `H` key that was pressed:

```c++
  EventHandlerResult onKeyEvent(KeyEvent &event) {
    if (event.key == Key_H && keyToggledOn(event.state)) {

      KeyEvent new_event = KeyEvent(event.addr, IS_PRESSED, LSHIFT(Key_H));
      Runtime.handleKeyEvent(new_event);

      new_event.key = Key_I;
      Runtime.handleKeyEvent(new_event);

      new_event.key = LSHIFT(Key_1);
      Runtime.handleKeyEvent(new_event);

      return EventHandlerResult::ABORT;
    }
    return EventHandlerResult::OK;
  }
```

This new version has the curious property that if the `H` key is held long enough, it will result in repeating `!!!!` characters on the host, until the key is released, which will clear it.  In fact, instead of creating a whole new `KeyEvent` object, we could further simplify this plugin by simply modifying the `event` object that we already have, instead:

```c++
  EventHandlerResult onKeyEvent(KeyEvent &event) {
    if (event.key == Key_H && keyToggledOn(event.state)) {
      event.key = LSHIFT(Key_H);
      Runtime.handleKeyEvent(event);

      event.key = Key_I;
      Runtime.handleKeyEvent(event);

      event.key = LSHIFT(Key_1);
    }
    return EventHandlerResult::OK;
  }
```

Note that, with this version, we've only sent two extra events, then changed the `event.key` value, and returned `OK` instead of `ABORT`.  This is basically the same as the above pluging that turned `Y` into `X`, but with two extra events sent first.

As one extra precaution, it would be wise to mark the generated event(s) as "injected" to let other plugins know that these events should be ignored.  This is a convention that is used by many existing Kaleidoscope plugins.  We do this by setting the `INJECTED` bit in the `event.state` variable:

```c++
  EventHandlerResult onKeyEvent(KeyEvent &event) {
    if (event.key == Key_H && keyToggledOn(event.state)) {
      event.state |= INJECTED;

      event.key = LSHIFT(Key_H);
      Runtime.handleKeyEvent(event);

      event.key = Key_I;
      Runtime.handleKeyEvent(event);

      event.key = LSHIFT(Key_1);
    }
    return EventHandlerResult::OK;
  }
```

If we wanted to be especially careful, we could also add the corresponding release events:

```c++
  EventHandlerResult onKeyEvent(KeyEvent &event) {
    if (event.key == Key_H && keyToggledOn(event.state)) {
      event.key = LSHIFT(Key_H);
      event.state = INJECTED | IS_PRESSED;
      Runtime.handleKeyEvent(event);
      event.state = INJECTED | WAS_PRESSED;
      Runtime.handleKeyEvent(event);

      event.key = Key_I;
      event.state = INJECTED | IS_PRESSED;
      Runtime.handleKeyEvent(event);
      event.state = INJECTED | WAS_PRESSED;
      Runtime.handleKeyEvent(event);

      event.key = LSHIFT(Key_1);
      event.state = INJECTED | IS_PRESSED;
    }
    return EventHandlerResult::OK;
  }
```

### Avoiding infinite loops

One very important consideration for any plugin that calls `Runtime.handleKeyEvent()` from an `onKeyEvent()` handler is recursion.  `Runtime.handleKeyEvent()` will call all plugins' `onKeyEvent()` handlers, including the one that generated the event.  Therefore, we need to take some measures to short-circuit the resulting recursive call so that our plugin doesn't cause an infinite loop.

Suppose the example plugin above was changed to type the string `hi!` instead of `Hi!`.  When sending the first generated event, with `event.key` set to `Key_H`, our plugin would recognize that event as one that should be acted on, and make another call to `Runtime.handleKeyEvent()`, which would again call `MyPlugin.onKeyEvent()`, and so on until the MCU ran out of memory on the stack.

The simplest mechanism used by many plugins that mark their generated events "injected" is to simply ignore all generated events:

```c++
  EventHandlerResult onKeyEvent(KeyEvent &event) {
    if ((event.state & INJECTED) != 0)
      return EventHandlerResult::OK;

    if (event.key == Key_H && keyToggledOn(event.state)) {
      event.state |= INJECTED;

      event.key = LSHIFT(Key_H);
      Runtime.handleKeyEvent(event);

      event.key = Key_I;
      Runtime.handleKeyEvent(event);

      event.key = LSHIFT(Key_1);
    }
    return EventHandlerResult::OK;
  }
```

There are other techniques to avoid inifinite loops, employed by plugins whose injected events should be processed by other plugins, but since most of those will be using the `onKeyswitchEvent()` handler instead of `onKeyEvent()`, we'll cover that later in this guide.

## Physical keyswitch events

Most plugins that respond to key events can do their work using the `onKeyEvent()` handler, but in some cases, it's necessary to use the `onKeyswitchEvent()` handler instead.  These event handlers are strictly intended for physical keyswitch events, and plugins that implement the `onKeyswitchEvent()` handler must abide by certain rules in order to work well with each other.  As a result, such a plugin is a bit more complex, but there are helper mechanisms to make things easier:

```c++
#include "kaleidoscope/KeyEventTracker.h"

namespace kaleidoscope {
namespace plugin {

class MyKeyswitchPlugin : public Plugin {
 public:
  EventHandlerResult onKeyswitchEvent(KeyEvent &event) {
    if (event_tracker_.shouldIgnore(event))
      return EventHandlerResult::OK;
    // Plugin logic goes here...
    return EventHandlerResult::OK;
  }
 private:
  KeyEventTracker event_tracker_;
};

} // namespace kaleidoscope
} // namespace plugin

kaleidoscope::plugin::MyKeyswitchPlugin;
```

We've just added a `KeyEventTracker` object to our plugin, and made the first line of its `onKeyswitchEvent()` handler call that event tracker's `shouldIgnore()` method, returning `OK` if it returns `true` (thereby ignoring the event).  Every plugin that implements `onKeyswitchEvent()` should follow this template to avoid plugin interaction bugs, including possible infinite loops.

The main reason for this event tracker mechanism is that plugins with `onKeyswitchEvent()` handlers often delay events because some aspect of those events (usually `event.key`) needs to be determined by subsequent events or timeouts.  To do this, event information is stored, and the event is later regenerated by the plugin, which calls `Runtime.handleKeyswitchEvent()` so that the other `onKeyswitchEvent()` handlers can process it.

We need to prevent infinite loops, but simply marking the regenerated event `INJECTED` is no good, because it would prevent the other plugins from acting on it, so we instead keep track of a monotonically increasing event id number and use the `KeyEventTracker` helper class to ignore events that our plugin has already recieved, so that when the plugin regenerates an event with the same event id, it (and all the plugins before it) can ignore that event, but the subsequent plugins, which haven't seen that event yet, will recongize it as new and process the event accordingly.

### Regenerating stored events

When a plugin that implements `onKeyswitchEvent()` regenerates a stored event later so that it can be processed by the next plugin in the chain, it must use the correct event id value (the same one used by the original event).  This is an object of type `EventId`, and is retrieved by calling `event.id()` (unlike the other properties of a `KeyEvent` object the event id is not directly accessible).

```c++
KeyEventId stored_id = event.id();
```

When reconstructing an event to allow it to proceed, we then use the four-argument version of the `KeyEvent` constructor:

```c++
KeyEvent event = KeyEvent(addr, state, key, stored_id);
```

In the above, `addr` and `state` are usually also the same as the original event's values, and `key` is most often the thing that changes.  If your plugin wants a keymap lookup to take place, the value `Key_Undefined` can be used instead of explicitly doing the lookup itself.

## Controlling LEDs

## HID reports

## Layer changes
