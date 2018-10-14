# Kaleidoscope-HostOS

The `HostOS` extension is not all that useful in itself, rather, it is a
building block other plugins and extensions can use to not repeat the same
guesswork and logic.

The goal is to have a single place that remembers the host OS, whether set by
the end-user in a Sketch, or via a macro, or some other way. This information
can then be reused by other plugins.

See the [Unicode][plugin:unicode] extension for an example about how to use
`HostOS` in practice.

 [plugin:unicode]: https://github.com/keyboardio/Kaleidoscope-Unicode

## Using the extension

The extension provides a `HostOS` singleton object.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-HostOS.h>

void someFunction(void) {
  if (HostOS.os() == kaleidoscope::hostos::LINUX) {
    // do something linux-y
  }
  if (HostOS.os() == kaleidoscope::hostos::OSX) {
    // do something OSX-y
  }
}

KALEIDOSCOPE_INIT_PLUGINS(HostOS)

void setup(void) {
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
   - `kaleidoscope::hostos::OSX`
   - `kaleidoscope::hostos::WINDOWS`
   - `kaleidoscope::hostos::OTHER`

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

* [Kaleidoscope-EEPROM-Settings](https://github.com/keyboardio/Kaleidoscope-EEPROM-Settings)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the extension.

 [plugin:example]: https://github.com/keyboardio/Kaleidoscope-HostOS/blob/master/examples/HostOS/HostOS.ino

## Upgrading

Prior versions of `HostOS` used to include a way to auto-detect the host
operating system. This code was brittle, unreliable, and rather big too. For
these reasons, this functionality was removed. The `autoDetect()` method is now
a no-op, and is deprecated. The `Kaleidoscope/HostOS-select.h` header is
similarly obsolete. Both of these produce deprecation messages now, and will be
removed by 2019-01-14.

Furthermore, `HostOS` now depends on `Kaleidoscope-EEPROM-Settings`, that plugin
should be initialized first.
