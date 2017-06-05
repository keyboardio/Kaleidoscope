# Kaleidoscope-SpaceCadet

![status][st:stable] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-SpaceCadet.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-SpaceCadet

 [st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

[Space Cadet][space-cadet] Shift is a way to make it more convenient to input
parens - those `(` and `)` things -, symbols that a lot of programming languages
use frequently. If you are working with Lisp, you are using these all the time.

What it does, is that it turns your left and right `Shift` keys into parens if
you tap and release them, without pressing any other key while holding them.
Therefore, to input, say, `(print foo)`, you don't need to press `Shift`, hold
it, and press `9` to get a `(`, you simply press and release `Shift`, and
continue writing. You use it as if you had a dedicated key for parens!

But if you wish to write capital letters, you hold it, as usual, and you will
not see any parens when you release it. You can also hold it for a longer time,
and it still would act as a `Shift`, without the parens inserted on release:
this is useful when you want to augment some mouse action with `Shift`, to
select text, for example.

 [space-cadet]: https://en.wikipedia.org/wiki/Space-cadet_keyboard

## Using the plugin

Using the plugin with its defaults is as simple as including the header, and
enabling the plugin:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-SpaceCadet.h>

void setup() {
  USE_PLUGINS(&SpaceCadetShift);

  Kaleidoscope.setup();
}
```

This assumes a US QWERTY layout on the host computer, and will use the `9` and
`0` keys for the left and right parens, respectively. To change these keys, use
the `.opening_paren` and `.closing_paren` properties outlined below.

## Plugin methods

The plugin provides the `SpaceCadetShift` object, with the following methods and
properties:

### `.opening_paren`

> Set this property to the key that - when shifted - will result in an opening paren.
>
> Defaults to `Key_9`.

### `.closing_paren`

> Set this property to the key that - when shifted - will result in a closing paren.
>
> Defaults to `Key_0`.

### `.time_out`

> Set this property to the number of milliseconds to wait before considering a
> held key in isolation as its secondary role. That is, we'd have to hold a
> `Shift` key this long, by itself, to trigger the `Shift` role in itself.
>
> Defaults to 1000.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/keyboardio/Kaleidoscope-SpaceCadet/blob/master/examples/SpaceCadet/SpaceCadet.ino
