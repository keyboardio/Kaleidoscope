# Technomancy Atreus

This is a plugin for [Kaleidoscope][fw], that adds hardware support for the
[Atreus][atreus]. Supports both the pre- and post-2016 variants with an A* MCU,
the hand-wired variant from [FalbaTech][falba] with a Teensy2, and the legacy
teensy2 variant too.

To select which one to build for, you can either use the Arduino IDE, and select
the appropriate Pinout and CPU, or, if using `kaleidoscope-builder`, you can add
a `LOCAL_CFLAGS` setting to your Makefile

For the post-2016 variant (the default, also used by the handwired variant from
FalbaTech), if you want to explicitly select it, add
`-DKALEIDOSCOPE_HARDWARE_ATREUS_PINOUT_ASTAR=1`. For the pre-2016 variant, add
`-DKALEIDOSCOPE_HARDWARE_ATREUS_PINOUT_ASTAR_DOWN`. For the legacy teensy2 variant, add `-DKALEIDOSCOPE_HARDWARE_ATREUS_PINOUT_LEGACY_TEENSY2=1`.

To be able to flash the firmware, one will need the [Teensy Loader
CLI][teensy_cli] tool in addition to Arduino, if using a Teensy. If using an A*
MCU, the additional tool is not required.

 [teensy_cli]: https://www.pjrc.com/teensy/loader_cli.html
 [fw]: https://github.com/keyboardio/Kaleidoscope
 [atreus]: https://atreus.technomancy.us/
 [falba]: https://falba.tech/
