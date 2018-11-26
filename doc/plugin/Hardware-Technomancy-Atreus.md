# Kaleidoscope-Hardware-Technomancy-Atreus

This is a plugin for [Kaleidoscope][fw], that adds hardware support for the
[Atreus][atreus]. Supported models include post-2016 PCB variants with an A*
MCU, and the hand-wired variant from [FalbaTech][falba] with a Teensy2.

PCBs prior to 2016, and the legacy teensy2 variants are not supported.

To be able to flash the firmware, one will need the [Teensy Loader
CLI][teensy_cli] tool in addition to Arduino, if using a Teensy. If using an A*
MCU, the additional tool is not required.

 [teensy_cli]: https://www.pjrc.com/teensy/loader_cli.html
 [fw]: https://github.com/keyboardio/Kaleidoscope
 [atreus]: https://atreus.technomancy.us/
 [falba]: https://falba.tech/
