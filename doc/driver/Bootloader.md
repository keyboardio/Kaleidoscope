# kaleidoscope::driver::bootloader

We rarely have to work with or care about the bootloader from the user firmware,
but there's one scenario when we do: if we want to reset the device, and put it
into bootloader (programmable) mode, we need to do that in a bootloader-specific
manner.

This driver provides a number of helpers that implement the reset functionality
for various bootloaders.

## Using the driver

To use the driver, we need to include the appropriate header, from the hardware plugin of
our keyboard:

```c++
#include <kaleidoscope/driver/bootloader/avr/Caterina.h>
```

Next, we create a (private) member variable, based on the bootloader of our choice:

```c++
kaleidoscope::driver::bootloader::avr::Caterina bootloader_;
```

And then we can implement a thin wrapper for the bootloader's `resetDevice()` method:

```c++
void resetDevice() {
  bootloader_.resetDevice();
}
```

## Methods provided by all bootloader drivers

### `.resetDevice()`

> Resets the device, and forces it into bootloader (programmable) mode.

## List of bootloaders

All of the drivers below live below the `kaleidoscope::driver::bootloader`
namespace.

## `avr::Caterina`:

Used by many (most?) arduino MCUs. Provided by
`kaleidoscope/driver/bootloader/avr/Caterina.h`.

### `avr::HalfKay`

Used by the Teensy2. Provided by `kaleidoscope/driver/bootloader/avr/HalfKay.h`.

### `avr::FLIP`

Used by the ATMega32U4 MCUs by default, unless another bootloader has been
flashed on them. Provided by `kaleidoscope/driver/bootloader/avr/FLIP.h`.

For this driver to work, one also needs to define the
`KALEIDOSCOPE_BOOTLOADER_FLIP` macro before including the driver header, for
technical reasons.

## Notes

When using the `ATMegaKeyboard` base class, there's no need to provide a
`resetDevice()` method, the base class does that for us. In this case, all that
is required is declaring a `bootloader_` protected member, of the appropriate
bootloader type.
