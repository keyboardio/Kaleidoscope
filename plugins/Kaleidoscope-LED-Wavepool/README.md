# LED-Wavepool

The `WavepoolEffect` plugin makes waves of light splash out from each keypress.
When idle, it will also simulate gentle rainfall on the keyboard.

## Using the plugin

To use the plugin, one needs to include the header and select the effect.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LED-Wavepool.h>

KALEIDOSCOPE_INIT_PLUGINS(LEDControl, WavepoolEffect);

void setup (){
  Kaleidoscope.setup();

  WavepoolEffect.idle_timeout = 5000;  // 5 seconds
  WavepoolEffect.activate();
}
```

It is recommended to place the activation of the plugin as early as possible, so
the plugin can catch all relevant key presses.

## Plugin properties

The plugin provides the `WavepoolEffect` object, which has the following
properties:

### `.idle_timeout`

> When to keys are being pressed, light will periodically splash across
> the keyboard.  This value sets the delay in ms before that starts.
>
> To disable the idle animation entirely, set this to 0.
>
> Default is 5000 (5 seconds).

### `.ripple_hue`

> The Hue of the ripple animation. If set, the light splashing across the
> keyboard will use this value instead of all colors of the rainbow.
>
> Setting it to the special value of `WavepoolEffect.rainbow_hue` will cause the
> plugin to use all colors again.
>
> Defaults to `WavepoolEffect.rainbow_hue`.

## Dependencies

* [Kaleidoscope-LEDControl](LEDControl.md)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]:  /examples/LEDS/LED-Wavepool/LED-Wavepool.ino
