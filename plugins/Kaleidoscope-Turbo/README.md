# Turbo

The `Turbo` plugin provides an extra key one can place on their keymap. While the
key is pressed or toggled, pressing other keys will generate quick repeated
inputs independent of the OS key repetition mechanics.

## Using the plugin

To use the plugin, simply include the header and enable the plugin and place
`Key_Turbo` somewhere on your keymap. You may add additionally configure
specific behaviors of the plugin as shown:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-Turbo.h>
#include <Kaleidoscope-LEDControl.h>

// somewhere in the keymap...
Key_Turbo

KALEIDOSCOPE_INIT_PLUGINS(LEDControl, Turbo);

void setup() {
    Kaleidoscope.setup();

    Turbo.interval(30);
    Turbo.sticky(true);
    Turbo.flash(true);
    Turbo.flashInterval(80);
    Turbo.activeColor(CRGB(0x64, 0x96, 0xed));
}
```

## Plugin properties

The `Turbo` object has the following user-configurable properties:

### `.interval([uint16_t])`

> This property adjusts the timing between simulated keypresses. If you set this
> too low, some programs might not like it. The default repeat rate for X11 is 25.
>
> Defaults to 10

### `.flashInterval([uint16_t])`

> This property adjusts the timing between the on/off states of the key LED.
>
> Defaults to 69

### `.sticky([bool])`
> This method makes the Turbo functionality sticky, so it remains in effect not only while
> it is held, but after it is released too, until it is toggled off with another tap. Without
> arguments, the method enables the sticky functionality. Passing a boolean argument
> sets stickiness to the given value.
>
> Defaults to `false`.

### `.flash([bool])`

> This property indicates whether the key should flash when enabled or remain a solid
> color.
>
> Defaults to true.


### `.activeColor([cRGB])`

> This property indicates the color the key should become when enabled.
>
> Defaults to `CRGB(160, 0, 0)` (same as solidRed in default firmware).

## Dependencies

* [Kaleidoscope-LEDControl](Kaleidoscope-LEDControl.md)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

  [plugin:example]: /examples/Keystrokes/Turbo/Turbo.ino
