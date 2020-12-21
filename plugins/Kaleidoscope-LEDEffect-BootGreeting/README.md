# LEDEffect-BootGreeting

If you want to have your keyboard signal when it turns on, but you don't want to
use any more complicated LED modes, this plugin is for you. It will make the
`LEDEffectNext` key on your keymap slowly breathe for about ten seconds after
plugging the keyboard in (without blocking the normal functionality of the
keyboard, of course).

## Using the plugin

To use the plugin, include the header, and tell `Kaleidoscope` to use the plugin:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LEDEffect-BootGreeting.h>

KALEIDOSCOPE_INIT_PLUGINS(LEDControl,
                          BootGreetingEffect
                          LEDOff);

void setup() {
  Kaleidoscope.setup();
}
```

You may also set optional parameters.

### Specify by search key
```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LEDEffect-BootGreeting.h>

KALEIDOSCOPE_INIT_PLUGINS(LEDControl,
                          BootGreetingEffect
                          LEDOff);

void setup() {
  Kaleidoscope.setup();

  BootGreetingEffect.search_key = Key_M;
}
```

### Specify by position
```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LEDEffect-BootGreeting.h>

KALEIDOSCOPE_INIT_PLUGINS(LEDControl,
                          BootGreetingEffect
                          LEDOff);

void setup() {
  Kaleidoscope.setup();

  //Butterfly key
  BootGreetingEffect.key_col = 7;
  BootGreetingEffect.key_row = 3;
}
```

### Specify longer timeout
```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LEDEffect-BootGreeting.h>

KALEIDOSCOPE_INIT_PLUGINS(LEDControl,
                          BootGreetingEffect
                          LEDOff);

void setup() {
  Kaleidoscope.setup();

  //Butterfly key
  BootGreetingEffect.timeout = 15000;
}
```

### Specify different color
```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LEDEffect-BootGreeting.h>

KALEIDOSCOPE_INIT_PLUGINS(LEDControl,
                          BootGreetingEffect
                          LEDOff);

void setup() {
  Kaleidoscope.setup();

  //Butterfly key
  BootGreetingEffect.hue = 90;

  Kaleidoscope.setup();
}
```

##   Plugin methods

The plugin provides the `BootGreetingEffect` object, with the following methods and
properties:

### `.search_key`

> Set the key in the current keymap that should be activated with the pulsing
> LED on startup.  The plugin will search from the top left to the bottom right
> of the keyboard, row by row, to find this key.  The first matching key will
> be selected.
>
> Defaults to `Key_LEDEffectNext`

### `.key_row`

> This is an optional override to explicitly set the selected key by exact row
> and column.  This number is 0-indexed, so the top row is 0, the second row is
> 1, etc.  Must set `.key_col` property for this feature to be enabled.

### `.key_col`

> This is an optional override to explicitly set the selected key by exact row
> and column.  This number is 0-indexed, so the left-most column is 0, the
> second column is 1, etc.  Must set `.key_row` property for this feature to
> be enabled.

### `.timeout`

> This property specifies the timeout (in milliseconds) for the effect to last.
> When the keyboard is first connected, the pulsing LED effect will last for
> this duration before turning off.
>
> Defaults to `9200` ms.

### `.hue`

> This property sets the color hue that the LED pulsing effect.
>
> The default is `170`, which is a blue color.

## Dependencies

* [Kaleidoscope-LEDControl](LEDControl.md)
