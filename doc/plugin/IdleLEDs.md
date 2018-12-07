# Kaleidoscope-IdleLEDs

Having LED effects on the keyboard can be exceptionally helpful. However, having
the effects - or lights, in general - on all the time, even when the keyboard is
otherwise idle, is perhaps not the best. When one leaves the keyboard, locks the
computer, what use are the LED effects then?

One could turn them off manually, but... that's too easy to forget, and why do
something the firmware could do for us anyway? What if the LEDs turned
themselves off after some configurable idle time? Say, if one did not press any
keys for the past ten minutes, just shut 'em off.

This is exactly what the `IdleLEDs` plugin does.

## Using the plugin

The plugin comes with reasonable defaults (see below), and can be used out of
the box, without any further configuration:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LEDEffect-Rainbow.h>
#include <Kaleidoscope-IdleLEDs.h>

KALEIDOSCOPE_INIT_PLUGINS(LEDControl, IdleLEDs, LEDEffectRainbowWave);

void setup (void) {
  Kaleidoscope.setup ();
}
```

Because the plugin needs to know about key events, it is best to make it one of
the first plugins, so it can catch all of them, before any other plugin would
have a chance to consume key events.

## Plugin properties

The plugin provides a single object, `IdleLEDs`, with the following
properties. All times are in seconds.

### `idle_time_limit`

> The amount of time that can pass without a single key being pressed, before
> the plugin considers the keyboard idle, and turns the LEDs off.
>
> Defaults to 600 seconds (10 minutes).

## Dependencies

* [Kaleidoscope-LEDControl](LEDControl.md)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: ../../examples/LEDs/IdleLEDs/IdleLEDs.ino
