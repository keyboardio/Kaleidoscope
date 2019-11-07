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

Next, we set up the device `Properties` so that it includes the override for our
bootloader:

```c++
struct OurBoardProps : kaleidoscope::device::BaseProps {
  typedef kaleidoscope::driver::bootloader::avr::Caterina Bootloader;
};
```

The base classes will do all the rest.

## Methods provided by all bootloader drivers

### `.rebootBootloader()`

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
`KALEIDOSCOPE_BOOTLOADER_FLIP_WORKAROUND` macro before including the driver
header, for technical reasons.
