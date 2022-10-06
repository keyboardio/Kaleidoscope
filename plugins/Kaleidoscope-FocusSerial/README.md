# FocusSerial

Bidirectional communication for Kaleidoscope. With this plugin enabled, plugins that implement the `onFocusEvent` hook will start responding to Focus commands sent via `Serial`, allowing bidirectional communication between firmware and host.

This plugin is an upgrade of the former [Kaleidoscope-Focus][kaleidoscope:focus] plugin. See the [UPGRADING.md][upgrading] document for information about how to transition to the new system.

 [kaleidoscope:focus]: (Kaleidoscope-FocusSerial.md)
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
  EventHandlerResult onNameQuery() {
    return ::Focus.sendName(F("FocusTestCommand"));
  }

  EventHandlerResult onFocusEvent(const char *input) {
    const char *cmd = PSTR("test");

    if (::Focus.inputMatchesHelp(input))
      return ::Focus.printHelp(cmd);

    if (!::Focus.inputMatchesCommand(input, cmd))
      return EventHandlerResult::OK;

    ::Focus.send(F("Congratulations, the test command works!"));
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

The plugin provides the `Focus` object, with a couple of helper methods aimed at developers. Terminating the response with a dot on its own line is handled implicitly by `FocusSerial`, one does not need to do that explicitly.

### `.inputMatchesHelp(input)`

Returns `true` if the given `input` matches the `help` command. To be used at the top of `onFocusEvent()`, followed by `.printHelp(...)`.

### `.printHelp(...)`

Given a series of strings (stored in `PROGMEM`, via `PSTR()`), prints them one per line. Assumes it is run as part of handling the `help` command. Returns `EventHandlerResult::OK`.

### `.inputMatchesCommand(input, command)`

Returns `true` if the `input` matches the expected `command`, false otherwise. A convenience function over `strcmp_P()`.

### `.send(...)`
### `.sendRaw(...)`

Sends a list of variables over the wire. The difference between `.send()` and `.sendRaw()` is that the former puts a space between each variable, the latter does not. If one just wants to send a list of things, use the former. If one needs more control over the formatting, use the latter. In most cases, `.send()` is the recommended method to use.

Both of them take a variable number of arguments, of almost any type: all built-in types can be sent, `cRGB`, `Key` and `bool` too in addition. For colors, `.send()` will write them as an `R G B` sequence; `Key` objects will be sent as the raw 16-bit keycode; and `bool` will be sent as either the string `true`, or `false`.

### `.sendName(F("..."))`

To be used with the `onNameQuery()` hook, this sends the plugin name given,
followed by a newline, and returns `EventHandlerResult::OK`, so that
`onNameQuery()` hooks can be implemented in a single line with the help of this
function.

### `.read(variable)`

Depending on the type of the variable passed by reference, reads a 8 or 16-bit unsigned integer, a `Key`, or a `cRGB` color from the wire, into the variable passed as the argument.

### `.peek()`

Returns the next character on the wire, without reading it. Subsequent reads will include the peeked-at byte too.

### `.isEOL()`

Returns whether we're at the end of the request line.

### `.COMMENT`

When sending something to the host that is not a response to a request, prefix the response lines with this.

### `.SEPARATOR`

To be used when using `.sendRaw`, when one needs complete control over where separators are inserted into the response.

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
* Messages sent to the host, without a request, should be prefixed with a hash mark (`Focus.COMMENT`).

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

- The [`focus-send` script][focus-send] in the Kaleidoscope repo make use of this protocol.

- Starting from the [example][plugin:example] is the recommended way of getting started with the plugin.

  [plugin:example]: /examples/Features/FocusSerial/FocusSerial.ino
  [focus-send]: https://github.com/keyboardio/Kaleidoscope/blob/master/bin/focus-send