# LED-Stalker

The `StalkerEffect` plugin provides an interesting new typing experience: the
LEDs light up as you tap keys and play one of the selected effects: a haunting
trail of ghostly white lights, or a blazing trail of fire.

## Using the plugin

To use the plugin, one needs to include the header and select the effect.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LED-Stalker.h>

KALEIDOSCOPE_INIT_PLUGINS(LEDControl, StalkerEffect);

void setup (){
  Kaleidoscope.setup();

  StalkerEffect.variant = STALKER(Haunt, (CRGB(0, 128, 0)));
  StalkerEffect.activate();
}
```

It is recommended to place the activation of the plugin (the `Kaleidoscope.use`
call) as early as possible, so the plugin can catch all relevant key presses.
The configuration can happen at any time and should use the `STALKER` macro to
do so.

## Plugin methods

The plugin provides the `StalkerEffect` object, which has the following
properties:

### `.variant`

> Set the effect to use with the plugin. See below for a list.
>
> It is recommended to use the `STALKER` macro to declare the effect itself.

### `.step_length`

> The length - in milliseconds - of each step of the animation. An animation
> lasts 256 steps.
>
> Defaults to 50.

### `.inactive-color`

> The color to use when a key hasn't been pressed recently.
>
> Defaults to `(cRGB) { 0, 0, 0 }`

## Plugin helpers

### `STALKER(effect, params)`

> Returns an effect, to be used to assign a value the `.variant` property of the
> `StalkerEffect` object. Any arguments given to the macro are passed on
> to the effect. If the effect takes no arguments, use an empty `params` list.

## Plugin effects

The plugin provides the following effects:

### `Haunt([color])`

> A ghostly haunt effect, that trails the key taps with a ghostly white color
> (or any other color, if specified). Use the `CRGB(r,g,b)` macro to specify the
> color, if you want something else than the ghostly white.

### `BlazingTrail()`

> A blazing trail of fire will follow our fingers!

### `Rainbow()`

> Leave a rainbow behind, where your fingers has been!

## Dependencies

* [Kaleidoscope-LEDControl](LEDControl.md)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: /examples/LEDs/LED-Stalker/LED-Stalker.ino
