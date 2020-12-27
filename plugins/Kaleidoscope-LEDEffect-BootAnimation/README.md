# LEDEffect-BootAnimation

With this plugin enabled, the keyboard will play a little boot animation when
starting up (this animation does not inhibit typing, you can still use the
keyboard while the animation plays).

## Using the plugin

To use the plugin, include the header, and tell `Kaleidoscope` to use the plugin:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LEDEffect-BootAnimation.h>

KALEIDOSCOPE_INIT_PLUGINS(LEDControl,
                          BootAnimationEffect
                          LEDOff);

void setup() {
  Kaleidoscope.setup();
}
```

## Plugin properties

The plugin provides the `BootAnimationEffect` object, with the following
properties:

### `.timeout`

> This property specifies the timeout (in milliseconds) each step of the
> animation is displayed.
>
> Defaults to `1000` ms, or one second.

### `.color`

> This property sets the color the animation is played with.
>
> The default is a red color.

## Dependencies

* [Kaleidoscope-LEDControl](LEDControl.md)
