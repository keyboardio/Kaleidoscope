The Kaleidoscope Command-Line Build System
==========================================

The Kaleidoscope command-line build system are a set of tools and scripts that
help build, flash, and otherwise work with one's own sketches, examples, and
whatnot - from the command-line, without starting the graphical Arduino IDE.
It's not meant to be a replacement for neither the Arduino IDE, nor
arduino-builder. Rather, it builds on top of Arduino, and is meant to be used
when the GUI is not desired. It's a supplemental set of tools, not a replacement
for any other. You are not required to use it, if the IDE - or any other
option - works for you.

## For end-users

Fortunately, you do not need to know much about how the system works. It's a
kind of magic under the hood, and that's ok. The easiest to get started with it
is to copy of [extras/Makefile.example](../extras/Makefile.example) to your
sketch as `Makefile`, as-is. You will not need to modify this file.

What we'll do, is create another file, `rules.mk`. This will be included by the
main makefile, and this is where we can set a few things up.

The only strictly required setting is `BOARD`, which tells the system which
board to build the firmware for. The currently supported boards are:

* `atreus`, for Technomancy's Atreus and compatible variants.
* `ergodox`, for the original ErgoDox and compatible variants (such as the
  ErgoDox EZ).
* `model01`, for the Keyboard.io Model01.
* `planck`, for OLKB's Planck (AVR variants).
* `splitography`, for SOFT/HRUF's Splitography.

> ```makefile
> BOARD = model01
> ```

This assumes that your sketch has the same name as the directory it resides in,
and the sketch itself is at the root of the repository. If the name of the
sketch is different from the directory name, we need to set `SKETCH` too. It's
safe to just set it anyway:

> ```makefile
> SKETCH = MySketch
> ```

In case the sketch lives in a subdirectory, we also need to set
`SKETCH_SOURCEDIR` to that directory:

> ```makefile
> SKETCH_SOURCEDIR = src
> ```

The build system tries to auto-detect where the board hardware package is
installed. If you installed it to some special place, outside of where Arduino
would find it automatically, then `BOARD_HARDWARE_PATH` needs to be set too:

> ```makefile
> BOARD_HARDWARE_PATH = ./lib/hardware
> ```

If we want to use third-party plugins or Arduino libraries, then we can set
`ARDUINO_BUILDER_ARGS`, which will be passed down to `arduino-builder` as-is,
and as such, we can use it to tell the builder where to find additional
libraries - assuming they're not on the default search path. For example, if we
put them into a `lib` subdirectory of our own sketch, the following would make
the builder find the libraries:

> ```makefile
> ARDUINO_BUILDER_ARGS = -libraries lib
> ```

And in case we want to set C pre-processor macros, or any other flags, the
`LOCAL_CFLAGS` variable is just for this purpose:

> ```makefile
> LOCAL_CFLAGS = -DKALEIDOSCOPE_HIDADAPTOR_DISABLE_KEYBOARD_BOOT_PROTOCOL=1
> ```

## For board developers

When adding a new board, we will need to add a new makefile snippet to
`makefiles/boards/` in [Kaleidoscope-Build-Tools][build-tools]. This will
duplicate some of the information added to `boards.txt`, for a number of
reasons, but primarily because this allows us to do things smarter than Arduino
would be able to.

 [build-tools]: https://github.com/keyboardio/Kaleidoscope-Build-Tools

Name the file `yourboard.mk`, where `yourboard` is the same thing you want your
users to use as their `BOARD`. At a minimum, we'll have to specify a few
variables, namely:

* `VID`, `PID`: The vendor and product IDs of your board, in hex.
* `BOOTLOADER_VID`, `BOOTLOADER_PID`: The vendor and product IDs of your board
  in bootloader mode, in hex. These need to be specified even if they're the
  same as `VID` and `PID`.
* `MCU`: The main controller unit's type. It's primarily used by the flash
  tools. Currently, this will almost always be `atmega32u4`.
* `PROG_SIZE_MAX`, `DATA_SIZE_MAX`: The maximum size of program and data
  sections a user sketch can use. The former often depends on the size of the
  bootloader used, the latter is usually the same for the same MCU. To figure
  out `PROG_SIZE_MAX`, take the maximum size of flash memory on the MCU, and
  subtract the size of your bootloader.

Once these are set up, we want to include common bits, and a makefile snippet
for the flashing application to be used with the board:

> ```makefile
> include ${MAKEFILE_RULES_DIR}/boards/common.mk
> include ${MAKEFILE_RULES_DIR}/flasher/FLASHER_OF_CHOICE.mk
> ```

The following flashers are currently available:

* `avrdude`
* `dfu-programmer`
* `teensy_cli`

These should all do the right thing, and you only need to include the preferred
one.

Next, we need to set up a few goals to do the flashing:

* `flash/prepare` should print a message telling the user what to do to put the
  keyboard in bootloader mode, and wait for confirmation (usually `Enter`).
* `flash` is usually an empty target, that depends on `flash/FLASHER_OF_CHOICE`.
  The reason the included snippet does not define `flash` directly is to support
  use-cases where the same board could come with different MCUs, bootloaders,
  and thus, require different ways to flash. This architecture makes it possible
  to choose between multiple flashers.

In many cases, this is all we will need. For more complicated cases, please see
the existing board snippets for tips, tricks and hints, and do not hesitate to
ask!
