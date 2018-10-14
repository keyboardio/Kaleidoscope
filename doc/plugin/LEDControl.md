# Kaleidoscope-LEDControl

This is a plugin for [Kaleidoscope][fw], for controlling the LEDs, and LED
effects.

 [fw]: https://github.com/keyboardio/Kaleidoscope

## Upgrading

The `LEDUtils.h` and `LED-Off.h` headers were moved, and need a
`kaleidoscope/plugin/` prefix now. The old headers still work, but are
deprecated, and will emit a warning. Both of them are included by
`<Kaleidoscope-LEDControl.h>` by default, so instead of migrating to the new
paths, one might wish to simply drop them instead.

The compatibility headers will be removed by 2019-01-14.

Furthermore, to implement LED modes, one should use
`kaleidoscope::plugin::LEDMode` as a base class now, instead of the former
`kaleidoscope::LEDMode`. There is a backwards compatible typedef, but like the
headers, it will be removed by 2019-01-14.
