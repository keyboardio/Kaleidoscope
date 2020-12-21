# LEDEffect-Chase

A simple LED effect where one color chases another across the keyboard and back,
over and over again. Playful colors they are.

## Using the extension

To use the plugin, include the header, and tell the firmware to use it:

```c++
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LEDEffect-Chase.h>

KALEIDOSCOPE_INIT_PLUGINS(LEDControl,
                          LEDEffect-Chase);

void setup() {
  Kaleidoscope.setup();
}
```

## Plugin methods

The plugin provides the `LEDChaseEffect` object, which has the following methods
outside of those provided by all LED modes:

### `.update_delay([delay])`

> Accessor for the update delay, the time between each step of the animation.
> When called without an argument, returns the current setting. When called with
> one, sets it.
>
> Defaults to 150 (milliseconds).

### `.distance([pixels])`

> Accessor for the distance between the two pixels. When called without an
> argument, returns the current setting. When called with one, sets it.
>
> Defaults to 5.

## Dependencies

* [Kaleidoscope-LEDControl](LEDControl.md)
