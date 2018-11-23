# Kaleidoscope-FocusSerial

Bidirectional communication for Kaleidoscope. With this plugin enabled, plugins that implement the `onFocusEvent` hook will start responding to Focus commands sent via `Serial`, allowing bidirectional communication between firmware and host.

This plugin is an upgrade of the former [Kaleidoscope-Focus][kaleidoscope:focus] plugin. See the [UPGRADING.md][upgrading] document for information about how to transition to the new system.

 [kaleidoscope:focus]: https://github.com/keyboardio/Kaleidoscope-Focus
 [upgrading]: ../../UPGRADING.md#bidirectional-communication-for-plugins

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

  [plugin:example]: ../../examples/FocusSerial/FocusSerial.ino
