# Kaleidoscope-LEDControl

This is a plugin for [Kaleidoscope][fw], for controlling the LEDs, and LED
effects.

 [fw]: https://github.com/keyboardio/Kaleidoscope

## Upgrading

The `LEDUtils.h` and `LED-Off.h` headers are now included by default when using
`<Kaleidoscope-LEDControl.h>`, and their explicit use is therefore deprecated.
The includes can be safely removed.

Compatibility headers are in place for both, but will be removed by 2019-01-14.

Furthermore, to implement LED modes, one should use
`kaleidoscope::plugin::LEDMode` as a base class now, instead of the former
`kaleidoscope::LEDMode`. There is a backwards compatible typedef, but like the
headers, it will be removed by 2019-01-14.
