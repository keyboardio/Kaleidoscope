# Akela-LED-Stalker

![status][st:experimental]

 [st:stable]: https://img.shields.io/badge/stable-✔-black.png?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.png?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.png?style=flat&colorA=dfb317&colorB=494e52

A haunting effect, where the lights follow your fingers as you keep typing.
Always behind, always watching, always stalking the fingertips...

The plugin simply lights up the LED below keys you press, and fades them away
soon after, producing a haunting trail effect.

## Using the plugin

To use the plugin, one needs to include the header, and activate the effect. It
is also possible to use a custom color instead of the white-ish default.

```c++
#include <Akela-LED-Stalker.h>

void setup () {
  Keyboardio.setup (KEYMAP_SIZE);

  Keyboardio.use (&StalkerEffect, NULL);
  
  StalkerEffect.configure ({0x00, 0xff, 0xff});
}
```

It is recommended to place the activation of the plugin (the `Keyboardio.use`
call) as early as possible, so the plugin can catch all relevant key presses.

## Plugin methods

The plugin provides the `StalkerEffect` object, which has the following
method:

### `.configure(color)`

> Set the color to use for highlighting pressed keys. If unset, will use the
> default white-ish color.
