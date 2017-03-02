# Kaleidoscope-LED-Stalker

![status][st:experimental] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-LED-Stalker.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-LED-Stalker

 [st:stable]: https://img.shields.io/badge/stable-✔-black.png?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.png?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.png?style=flat&colorA=dfb317&colorB=494e52

The `StalkerEffect` plugin provides an interesting new typing experience: the
LEDs light up as you tap keys, and play one of the selected effects: a haunting
trail of ghostly white lights, or a blazing trail of fire.

## Using the plugin

To use the plugin, one needs to include the header, and select the effect.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-LED-Stalker.h>

void setup () {
  Kaleidoscope.setup ();

  StalkerEffect.configure (STALKER (Haunt, {0xff, 0, 0}));
  USE_PLUGINS (&StalkerEffect);
  StalkerEffect.activate ();
}
```

It is recommended to place the activation of the plugin (the `USE_PLUGINS` call)
as early as possible, so the plugin can catch all relevant key presses. The
configuration can happen at any time, but using the `STALKER` macro is highly
recommended.

## Plugin methods

The plugin provides the `StalkerEffect` object, which has the following
method:

### `.configure(effect)`

> Set the effect to use with the plugin. See below for a list.
>
> It is recommended to use the `STALKER` macro to declare the effect itself.

### `.stepLength`

> The length - in milliseconds - of each step of the animation. An animation
> lasts 256 steps.
>
> Not a method itself, but a changeable value.
>
> Defaults to 50.

## Plugin helpers

### `STALKER(effect, params...)`

> Returns an effect, to be used by the `.configure` method of the
> `StalkerEffect` object. Any arguments given to the macro, are also passed on
> to the effect. If the effect takes no arguments, use `NULL`.

## Plugin effects

The plugin provides the following effects:

### `Haunt([color])`

> A ghostly haunt effect, that trails the key taps with a ghostly white color
> (or any other color, if specified).

### `BlazingTrail()`

> A blazing trail of fire will follow our fingers!

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/keyboardio/Kaleidoscope-LED-Stalker/blob/master/examples/LED-Stalker/LED-Stalker.ino
