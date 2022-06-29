# Kaleidoscope Device API internals

This document is aimed at people interested in working on adding new devices -
or improving support for existing ones - to Kaleidoscope. The APIs detailed here
are a little bit more complex than most of the APIs our plugins provide.
Nevertheless, we hope they're still reasonably easy to use, and this document is
an attempt to explain some of the more intricate parts of it.

## Overview

The core idea of the APIs is that to build up a device, we compose various
components together, by describing their _properties_, and using fairly generic,
templated helper classes with the properties as template parameters.

This way, we can assemble together a device with a given _MCU_, which uses a
particular _Bootloader_, some kind of _Storage_, perhaps some _LEDs_, and it
will more than likely have a _key scanner_ component too.

The base and helper classes provide a lot of the functionality themselves, so
for a device built up from components already supported by Kaleidoscope, the
amount of custom code one has to write will be minimal.

## Component details

### Device

A `Device` is the topmost level component, it is the interface the rest of
Kaleidoscope will work with. The [`kaleidoscope::device::Base`][k:d:Base] class
is the ancestor of _all_ devices, everything derives from this. Devices that use
an `ATmega32U4` MCU we also have the
[`kaleidoscope::device::ATmega32U4Keyboard`][k:d:a32u4] class, which sets up
some of the components that is common to all `ATmega32U4`-based devices (such as
the _MCU_ and the _Storage_).

 [k:d:Base]: ../../src/kaleidoscope/device/Base.h
 [k:d:a32u4]: ../../src/kaleidoscope/device/ATmega32U4Keyboard.h

As hinted at above, a device - or rather, it's `Props` - describe the components
used for the device, such as the MCU, the Bootloader, the Storage driver, LEDs,
and the key scanner. If any of that is unneeded, there's no need to specify them
in `Props` - the defaults are all no-ops.

All devices must also come with a `Props` struct, deriving from [`kaleidoscope::device::BaseProps`][k:d:BaseProps].

 [k:d:BaseProps]: ../../src/kaleidoscope/device/Base.h

As an example, the most basic device we can have, that does nothing, would look
like this:

```c++
class ExampleDevice : public kaleidoscope::device::Base<kaleidoscope::device::BaseProps> {};
```

That's not very useful, though. More often than not, we want to override at
least some of the properties. In some cases, even override some of the
pre-defined methods of the device. See the [base class][k:d:Base] for an
up-to-date list of methods and interfaces it provides. The most often changed
methods are likely to be `setup()` and the constructor, and
`enableHardwareTestMode()` if the device implements a hardware test mode. The
rest are wrappers around the various components described by the `Props`.

In other words, the majority of customisation is in the `Props`, and in what
components the device ends up using.

### MCU

The heart of any device will be the main controller unit, or _MCU_ for short.
The [`kaleidoscope::driver::mcu::Base`][k:d:m:Base] class is the ancestor of our
MCU drivers, including [`kaleidoscope::driver::mcu::ATmega32U4`][k:d:m:a32u4].

 [k:d:m:Base]: ../../src/kaleidoscope/driver/mcu/Base.h
 [k:d:m:a32u4]: ../../src/kaleidoscope/driver/mcu/ATmega32U4.h

The core firmware will use the `detachFromHost()` and `attachToHost()` methods
of the MCU driver, along with `setup()`, but the driver - like any other
driver - is free to have other methods, to be used by individual devices.

For example, the [`ATmega32U4`][k:d:m:a32u4] driver implements a `disableJTAG()`
and a `disableClockDivision()` method, which some of our devices use in their
constructors.

Unlike some other components, the `MCU` component has no properties.

### Bootloader

Another important component of a device is a bootloader. The bootloader is the
thing that allows us to re-program the keyboard without additional hardware
(aptly called a programmer). As such, the [`base class`][k:d:b:Base] has a
single method, `rebootBootloader()`, which our bootloader components implement.

 [k:d:b:Base]: ../../src/kaleidoscope/driver/bootloader/Base.h

Kaleidoscope currently supports [`Caterina`][k:d:b:Caterina],
[`HalfKay`][k:d:b:HalfKay], and [`FLIP`][k:d:b:FLIP] bootloaders. Please consult
them for more information. In many cases, setting up the bootloader in the
device props is all one needs to do.

 [k:d:b:Caterina]: ../../src/kaleidoscope/driver/bootloader/avr/Caterina.h
 [k:d:b:HalfKay]: ../../src/kaleidoscope/driver/bootloader/avr/HalfKay.h
 [k:d:b:FLIP]: ../../src/kaleidoscope/driver/bootloader/avr/FLIP.h

Like the _MCU_ component, the _bootloader_ does not use Props, either.

### Storage

Not nearly as essential for a device is the [`Storage`][k:d:s:Base] component.
Storage is for persistent storage of configuration data, such as key maps,
colormaps, feature toggles, and so on. It's not a required component, but a
recommended one nevertheless. This storage component is what allows apps like
[Chrysalis][chrysalis] to configure some aspects of the keyboard without having
to flash new firmware.

The Storage API resembles the Arduino EEPROM API very closely. In fact, our
[`AVREEPROM`][k:d:s:AVREEPROM] class is but a thin wrapper around that!

 [k:d:s:Base]: ../../src/kaleidoscope/driver/storage/Base.h
 [chrysalis]: https://github.com/keyboardio/Chrysalis
 [k:d:s:AVREEPROM]: ../../src/kaleidoscope/driver/storage/AVREEPROM.h

The `Storage` component does use Props, one that describes the length - or
size - of it. We provide an [`ATmega32U4EEPROMProps`][k:d:s:a32u4props] helper,
which is preconfigured for the 1k EEPROM size of the ATmega32U4.

 [k:d:s:a32u4props]: ../../src/kaleidoscope/driver/storage/ATmega32U4EEPROMProps.h

### LEDs

[`kaleidoscope::driver::led::Base`][k:d:l:Base]

 [k:d:l:Base]: ../../src/kaleidoscope/driver/led/Base.h

### Keyscanner

[`kaleidoscope::driver::keyscanner::Base`][k:d:ks:Base]

 [k:d:ks:Base]: ../../src/kaleidoscope/driver/keyscanner/Base.h

## Helpers

[`kaleidoscope::device::ATmega32U4Keyboard`][k:d:a32u4k]
[`kaleidoscope::driver::keyscanner::ATmega`][k:d:ks:atm]

 [k:d:a32u4k]: ../../src/kaleidoscope/device/ATmega32U4Keyboard.h
 [k:d:ks:atm]: ../../src/kaleidoscope/driver/keyscanner/ATmega.h

## Putting it all together

To put things into perspective, and show a simple example, we'll build an
imaginary mini keypad: `ATmega32U4` with `Caterina` as bootloader, no LEDs, and
four keys only.

### `ImaginaryKeypad.h`

```c++
#pragma once

#ifdef ARDUINO_AVR_IMAGINARY_KEYPAD

#include <Arduino.h>
#include "kaleidoscope/driver/keyscanner/ATmega.h"
#include "kaleidoscope/driver/bootloader/avr/Caterina.h"
#include "kaleidoscope/device/ATmega32U4Keyboard.h"

namespace kaleidoscope {
namespace device {
namespace imaginary {

struct KeypadProps : kaleidoscope::device::ATmega32U4KeyboardProps {
  struct KeyScannerProps : public kaleidoscope::driver::keyscanner::ATmegaProps {
    static constexpr uint8_t matrix_rows = 2;
    static constexpr uint8_t matrix_columns = 2;
    typedef MatrixAddr<matrix_rows, matrix_columns> KeyAddr;
    static constexpr uint8_t matrix_row_pins[matrix_rows] = {PIN_D0, PIN_D1};
    static constexpr uint8_t matrix_col_pins[matrix_columns] = {PIN_C0, PIN_C1};
  };

  typedef kaleidoscope::driver::keyscanner::ATmega<KeyScannerProps> KeyScanner;
  typedef kaleidoscope::driver::bootloader::avr::Caterina Bootloader;
  static constexpr const char *short_name = "imaginary-keypad";
};

class Keypad: public kaleidoscope::device::ATmega32U4Keyboard<KeypadProps> {};

#define PER_KEY_DATA(dflt, \
  R0C0, R0C1,              \
  R1C0, R1C1               \
)                          \
  R0C0, R0C1, R1C0, R1C1

}
}

EXPORT_DEVICE(kaleidoscope::device::imaginary::Keypad);

}
#endif
```

### `ImaginaryKeypad.cpp`

```c++
#ifdef ARDUINO_AVR_IMAGINARY_KEYPAD

#include <Kaleidoscope.h>

// Here, we set up aliases to the device's KeyScanner and KeyScannerProps in the
// global namespace within the scope of this file. We'll use these aliases to
// simplify some template initialization code below.
using KeyScannerProps = typename kaleidoscope::device::imaginary::KeypadProps::KeyScannerProps;
using KeyScanner = typename kaleidoscope::device::imaginary::KeypadProps::KeyScanner;

namespace kaleidoscope {
namespace device {
namespace imaginary {

// `KeyScannerProps` here refers to the alias set up above. We do not need to
// prefix the `matrix_rows` and `matrix_columns` names within the array
// declaration, because those are resolved within the context of the class, so
// the `matrix_rows` in `KeyScannerProps::matrix_row_pins[matrix_rows]` gets
// resolved as `KeyScannerProps::matrix_rows`.
const uint8_t KeyScannerProps::matrix_rows;
const uint8_t KeyScannerProps::matrix_columns;
constexpr uint8_t KeyScannerProps::matrix_row_pins[matrix_rows];
constexpr uint8_t KeyScannerProps::matrix_col_pins[matrix_columns];

// `KeyScanner` here refers to the alias set up above, just like in the
// `KeyScannerProps` case above.
template<> KeyScanner::row_state_t KeyScanner::matrix_state_[KeyScannerProps::matrix_rows] = {};

// We set up the TIMER1 interrupt vector here. Due to dependency reasons, this
// cannot be in a header-only driver, and must be placed here.
//
// Timer1 is responsible for setting a property on the KeyScanner, which will
// tell it to do a scan. We use this to make sure that scans happen at roughly
// the intervals we want. We do the scan outside of the interrupt scope for
// practical reasons: guarding every codepath against interrupts that can be
// reached from the scan is far too tedious, for very little gain.
ISR(TIMER1_OVF_vect) {
  Runtime.device().keyScanner().do_scan_ = true;
}

}
}
}
#endif
```

That's it.
