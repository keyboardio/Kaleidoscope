# Kaleidoscope-FocusSerial

Bidirectional communication for Kaleidoscope. With this plugin enabled, plugins that implement the `onFocusEvent` hook will start responding to Focus commands sent via `Serial`, allowing bidirectional communication between firmware and host.

This plugin is an upgrade of the former [Kaleidoscope-Focus][kaleidoscope:focus] plugin. See the [UPGRADING][upgrading]  section for information about how to transition to the new system.

 [kaleidoscope:focus]: https://github.com/keyboardio/Kaleidoscope-Focus
 [upgrading]: #upgrading-from-focus-to-onfocusevent--focusserial

## Using the plugin

This plugin is **not** meant to be used by the end-user (apart from setting it up to use plugin-provided hooks), but by plugin authors instead. As an end user, you just need to use Focus-enabled plugins like you normally would, and once `FocusSerial` is enabled, their commands will be available too.

Nevertheless, a very simple example is shown below:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-FocusSerial.h>

namespace kaleidoscope {
class FocusTestCommand : public Plugin {
 public:
  FocusTestCommand() {}

  EventHandlerResult onFocusEvent(const char *command) {
    if (strcmp_P(command, PSTR("test")) != 0)
      return EventHandlerResult::OK;

    Serial.println(F("Congratulations, the test command works!"));
    return EventHandlerResult::EVENT_CONSUMED;
  }
};
}

kaleidoscope::FocusTestCommand FocusTestCommand;

KALEIDOSCOPE_INIT_PLUGINS(Focus, FocusTestCommand);

void setup () {
  Kaleidoscope.setup ();
}
```

## Plugin methods

The plugin provides the `Focus` object, with a couple of helper methods aimed at developers. Documenting those is a work in progress for now.

## Wire protocol

`Focus` uses a simple, textual, request-response-based wire protocol.

Each request has to be on one line, anything before the first space is the command part (if there is no space, just a newline, then the whole line will be considered a command), everything after are arguments. The plugin itself only parses until the end of the command part, argument parsing is left to the various hooks. If there is anything left on the line after hooks are done processing, it will be ignored.

Responses can be multi-line, but most aren't. Their content is also up to the hooks, `Focus` does not enforce anything, except a trailing dot and a newline. Responses should end with a dot on its own line.

Apart from these, there are no restrictions on what can go over the wire, but to make the experience consistent, find a few guidelines below:

* Commands should be namespaced, so that the plugin name, or functionality comes first, then the sub-command or property. Such as `led.theme`, or `led.setAll`.
* One should not use setters and getters, but a single property command instead. One, which when called without arguments, will act as a getter, and as a setter otherwise.
* Namespaces should be lowercase, while the commands within them camel-case.
* Do as little work in the hooks as possible. While the protocol is human readable, the expectation is that tools will be used to interact with the keyboard.
* As such, keep formatting to the bare minimum. No fancy table-like responses.
* In general, the output of a getter should be copy-pasteable to a setter.

These are merely guidelines, and there can be - and are - exceptions. Use your discretion when writing Focus hooks.

### Example

In the examples below, `<` denotes what the host sends to the keyboard, `>` what
the keyboard responds.

```
< test
> Congratulations, the test command works!
> .
```

```
< help
> help
> test
> palette
> .
```

```
< palette
> 0 0 0 128 128 128 255 255 255
> .
< palette 0 0 0 128 128 128 255 255 255
> .
```

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting started with the plugin.

  [plugin:example]: https://github.com/keyboardio/Kaleidoscope-FocusSerial/blob/master/examples/FocusSerial/FocusSerial.ino

Upgrading from Focus to onFocusEvent + FocusSerial
==================================================

Upgrading from `Focus` to `onFocusEvent` and `FocusSerial` is a reasonably simple process, the interface is quite similar. Nevertheless, we present a step-by-step guide here, covering two use cases: one where we wish to always provide a Focus command when both our plugin and `FocusSerial` are enabled; and another where we only wish to provide the command when explicitly asked to do so.

## The most trivial example

The biggest difference between `Focus` and `onFocusEvent` is that the former required explicit registering of hooks, while the latter does it automatically: every plugin that implements the `onFocusEvent` method will be part of the system. As a consequence, only plugins are able to supply new commands: there is no explicit registration, thus, no way to inject a command that isn't part of a plugin. This also means that these functions now return `kaleidoscope::EventHandlerResult` instead of `bool`. Lets see a trivial example!

### Focus

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

### onFocusEvent

```c++
namespace kaleidoscope {
class FocusExampleCommand : public Plugin {
 public:
  FocusExampleCommand() {}

  EventHandlerResult onFocusEvent(const char *command) {
    if (strcmp_P(command, PSTR("example")) != 0)
      return EventHandlerResult::OK;

    Serial.println(F("This is an example response. Hello world!"));
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

### Summary

The new version is slightly more verbose for the trivial use case, because we have to wrap it up in an object. But other than that, the changes are minimal, and we don't need to explicitly register it!

Observe that the return values changed: with `Focus`, if we wanted other hooks to have a chance at processing the same command, the hook returned `false`; if we wanted to stop processing, and consider it consumed, it returned `true`. With the new system, this is more descriptive with the `EventHandlerResult::OK` and `EventHandlerResult::EVENT_CONSUMED` return values.

## A stateful example

Perhaps a better example that shows the quality of life improvements the new system brings is the case where the command needs access to either plugin state, or plugin methods. With the former system, the focus hooks needed to be static methods, and needed to be public. This is not necessarily the case now, because `onFocusEvent` is a non-static object method. It has full access to plugin internals!

### Focus

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

### onFocusEvent

```c++
namespace kaleidoscope {
class ExamplePlugin : public Plugin {
 public:
  ExamplePlugin();

  EventHandlerResult onFocusEvent(const char *command) {
    if (strcmp_P(command, PSTR("example.toggle")) != 0)
      return EventHandlerResult::OK;

    example_toggle_ = !example_toggle_;
    ::Focus.printBool(example_toggle_);

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

### Summary

It's just another plugin, with just another event handler method implemented, nothing more. No need to explicitly register the focus hook, no need to provide access to private variables - we can just keep them private.

## Optional commands

Optional commands are something that were perhaps easier with the `Focus` method: one just didn't register them. With `onFocusEvent`, we need to do a bit more than that, and move the command to a separate plugin, if we do not wish to enable it in every case. This adds a bit of overhead, but still less than `Focus` did.

### Focus

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

### onFocusEvent

```c++
namespace kaleidoscope {
class ExampleOptionalCommand : public Plugin {
 public:
  ExampleOptionalCommand() {}

  EventHandlerResult onFocusEvent(const char *command) {
    if (strcmp_P(command, PSTR("optional")) != 0)
      return EventHandlerResult::OK;

    Serial.println(Layer.getLayerState(), BIN);
    return EventHandlerResult::EVENT_CONSUMED;
  }
};
}

KALEIDOSCOPE_INIT_PLUGINS(Focus)

void setup() {
  Kaleidoscope.setup();
}
```

### Summary

The trick here is to move optional commands out into a separate plugin. It's a bit more boilerplate, but not by much.
