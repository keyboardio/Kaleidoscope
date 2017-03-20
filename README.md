# Kaleidoscope-HostOS

![status][st:stable] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-HostOS.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-HostOS

 [st:stable]: https://img.shields.io/badge/stable-✔-black.png?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.png?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.png?style=flat&colorA=dfb317&colorB=494e52

The `HostOS` extension is not all that useful in itself, rather, it is a
building block other plugins and extensions can use to not repeat the same
guesswork and logic. Its primary purpose is to help either detect, or keep track
of the host operating system. The detection part is not the most reliable thing,
mind you.

The goal is to have a single place that remembers the host OS, either detected,
or set by the end-user, in a Sketch, or via a macro, or some other way. This
information can then be reused by other plugins.

See the [Unicode][plugin:unicode] extension for an example about how to use
`HostOS` in practice.

 [plugin:unicode]: https://github.com/keyboardio/Kaleidoscope-Unicode

## Using the extension

The extension provides a `HostOS` singleton object. It can either be a simple
one without auto-detection (the default), or one that will try to detect the
Host OS, using the [FingerprintUSBHost][fprdetect] library. To enable
auto-detection, `KALEIDOSCOPE_HOSTOS_GUESSER` must be defined before including
the `HostOS` library header.

 [fprdetect]: https://github.com/keyboardio/FingerprintUSBHost

```c++
#define KALEIDOSCOPE_HOSTOS_GUESSER 1

#include <Kaleidoscope.h>
#include <Kaleidoscope-HostOS.h>
#include <Kaleidoscope/HostOS-select.h>

void someFunction (void) {
  if (HostOS.os() == Kaleidoscope::HostOS::LINUX) {
    // do something linux-y
  }
  if (HostOS.os() == Kaleidoscope::HostOS::OSX) {
    // do something OSX-y
  }
}

void setup (void) {
  Kaleidoscope.setup (KEYMAP_SIZE);
  Kaleidoscope.use (&HostOS);
}
```

To be able to choose between the two variants, one must also include the
`Kaleidoscope/HostOS-select.h` header.

## Extension methods

The extension provides the following methods on the `HostOS` singleton:

### `.os()`

> Returns the stored type of the Host OS.

### `.os(type)`

> Sets the type of the host OS, overriding any previous value. The type is then
> stored in EEPROM for persistence.

## Dependencies

* [Kaleidoscope-EEPROM-Settings](https://github.com/keyboardio/Kaleidoscope-EEPROM-Settings)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the extension.

 [plugin:example]: https://github.com/keyboardio/Kaleidoscope-HostOS/blob/master/examples/HostOS/HostOS.ino
