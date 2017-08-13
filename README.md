# Kaleidoscope-LEDEffect-BootGreeting

![status][st:stable]

 [st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

If you want to have your keyboard signal when it turns on, but you don't want to
use any more complicated LED modes, this plugin is for you. It will make the
`LEDEffectNext` key on your keymap slowly breathe for about ten seconds after
plugging the keyboard in (without blocking the normal functionality of the
keyboard, of course).

## Using the plugin

To use the plugin, include the header, and tell `Kaleidoscope` to use the plugin:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-LEDEffect-BootGreeting.h>

void setup() {
  Kaleidoscope.use(&BootGreetingEffect, &LEDOff);

  Kaleidoscope.setup();
}
```

The plugin provides no methods or properties, the above is all it can do.

## Dependencies

* [Kaleidoscope-LEDControl](https://github.com/keyboardio/Kaleidoscope-LEDControl)
