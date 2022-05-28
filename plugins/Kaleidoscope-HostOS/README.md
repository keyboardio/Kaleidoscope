# HostOS

The `HostOS` extension is not all that useful in itself, rather, it is a
building block other plugins and extensions can use to not repeat the same
guesswork and logic.

The goal is to have a single place that remembers the host OS, whether set by
the end-user in a Sketch, or via a macro, or some other way. This information
can then be reused by other plugins.

See the [Unicode][plugin:unicode] extension for an example about how to use
`HostOS` in practice.

 [plugin:unicode]: Kaleidoscope-Unicode.md

## Using the extension

The extension provides a `HostOS` singleton object.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-HostOS.h>

void someFunction() {
  if (HostOS.os() == kaleidoscope::hostos::LINUX) {
    // do something linux-y
  }
  if (HostOS.os() == kaleidoscope::hostos::MACOS) {
    // do something macOS-y
  }
}

KALEIDOSCOPE_INIT_PLUGINS(HostOS)

void setup() {
  Kaleidoscope.setup ();
}
```

## Extension methods

The extension provides the following methods on the `HostOS` singleton:

### `.os()`

> Returns the stored type of the Host OS.

### `.os(type)`

> Sets the type of the host OS, overriding any previous value. The type is then
> stored in EEPROM for persistence.

## Host OS Values

The OS type (i.e. the return type of `.os()` and the arguments to `.os(type)`) will be one of the following:

   - `kaleidoscope::hostos::LINUX`
   - `kaleidoscope::hostos::MACOS`
   - `kaleidoscope::hostos::WINDOWS`
   - `kaleidoscope::hostos::OTHER`

For compability reasons, `kaleidoscope::hostos::OSX` is an alias to
`kaleidoscope::hostos::MACOS`.

## Focus commands

The plugin provides the `FocusHostOSCommand` object, which, when enabled,
provides the `hostos.type` Focus command.

### `hostos.type [type]`

> Without argument, returns the current OS type set (a numeric value).
>
> With an argument, it sets the OS type.
>
> This command can be used from the host to reliably set the OS type within the firmware.

## Dependencies

* [Kaleidoscope-EEPROM-Settings](Kaleidoscope-EEPROM-Settings.md)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the extension.

 [plugin:example]: /examples/Features/HostOS/HostOS.ino
