# Heatmap

The `Heatmap` plugin provides a LED effect, that displays a heatmap on the
keyboard. The LEDs under each key will have a color according to how much use
they see. Fewer used keys will have deep blue colors, that gradually turns
lighter, then green, to yellow, and finally red for the most used keys. The
heatmap is not updated on every key press, but periodically. It's precision is
also an approximation, and not a hundred percent exact. Nevertheless, it is a
reasonable estimate.

## Using the plugin

The plugin comes with reasonable defaults pre-configured, all one needs to do is
include the header, and make sure the plugin is in use:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-Heatmap.h>

static const cRGB heat_colors[] PROGMEM = {
  {  0,   0,   0}, // black
  {255,  25,  25}, // blue
  { 25, 255,  25}, // green
  { 25,  25, 255}  // red
};

KALEIDOSCOPE_INIT_PLUGINS(LEDControl, HeatmapEffect);

void setup() {
  Kaleidoscope.setup ();

  HeatmapEffect.heat_colors = heat_colors;
  HeatmapEffect.heat_colors_length = 4;
}
```

This sets up the heatmap to update every second (by default). It also registers
a new LED effect, which means that if you have not set up any other effects,
then Heatmap will likely be the default. You may not want that, so setting up
at least one other LED effect, such as `LEDOff` is highly recommended.

## Plugin methods

The plugin provides a `HeatmapEffect` object, which has the following methods
and properties:

### `.activate()`

> When called, immediately activates the Heatmap effect. Mostly useful in the
> `setup()` method of the Sketch, or in macros that are meant to switch to the
> heatmap effect, no matter where we are in the list.

### `.update_delay`

> The number of milliseconds to wait between updating the heatmap. Updating the
> heatmap incurs a significant performance penalty, and should not be done too
> often. Doing it too rarely, on the other hand, make it much less useful. One
> has to strike a reasonable balance.
>
> Defaults to *1000*.

### `.heat_colors`

> A cRGB array describing the gradian of colors that will be used, from colder
> to hoter keys.
> E.g. with `heat_colors = {{100, 0,  0}, {0, 100,  0}, {0,  0, 100}}`, a key
> with a temperature of 0.8 (0=coldest, 1=hotest), will end up with a color
> `{0, 40, 60}`.
>
> Defaults to `{{0, 0, 0}, {25, 255, 25}, {25, 255, 255}, {25, 25, 255}}`
> (black, green, yellow, red)

### `.heat_colors_length`

> Length of the `heat_colors` array.
>
> Defaults to *4*

## Dependencies

* [Kaleidoscope-LEDControl](LEDControl.md)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: /examples/LEDs/Heatmap/Heatmap.ino
