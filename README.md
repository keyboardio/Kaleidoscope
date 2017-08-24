# Kaleidoscope-SpaceCadet

![status][st:stable] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-SpaceCadet.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-SpaceCadet

 [st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

[Space Cadet][space-cadet] is a way to make it more convenient to input
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

After getting used to the Space Cadet style of typing, you may wish to enable
this sort of functionality on other keys, as well.  Fortunately, the Space Cadet
plugin is configurable and extensible to support adding symbols to other keys,
as well.  Along with `(` on your left `Shift` key, you with to add other such
programming mainstays as `{` to your `cmd` key and `[` to your left `Control`
key (and, of course, the matching bookend symbols on the right).  You can do
whatever you wish to do, so feel free to experiment with different combinations
and discover what works best for you!

 [space-cadet]: https://en.wikipedia.org/wiki/Space-cadet_keyboard

## Using the plugin

Using the plugin with its defaults is as simple as including the header, and
enabling the plugin:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-SpaceCadet.h>

void setup() {
  Kaleidoscope.use(&SpaceCadet);

  Kaleidoscope.setup();
}
```

This assumes a US QWERTY layout on the host computer, though the plugin sends
the correct keymap code for each symbol.  Because the mapping is entirely 
configurable, though, you may switch out keys at your leisure.

If you wish to enable additional modifier keys (or disable the default behavior
for the shift and parentheses combinations), configuration is as simple as
passing a new keymap into the SpaceCadet object, as shown below:


```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-SpaceCadet.h>

void setup() {
  Kaleidoscope.use(&SpaceCadet);

  //Set the keymap with a 250ms timeout per-key
  static kaleidoscope::ModifierKeyMap spacecadetmap[] = {
    {Key_LeftShift, Key_LeftParen, 250}
    ,{Key_RightShift, Key_RightParen, 250}
    ,{Key_LeftGui, Key_LeftCurlyBracket, 250}
    ,{Key_RightAlt, Key_RightCurlyBracket, 250}
    ,{Key_LeftAlt, Key_RightCurlyBracket, 250}
    ,{Key_LeftControl, Key_LeftBracket, 250}
    ,{Key_RightControl, Key_RightBracket, 250}
  };
  //Tell SpaceCadet to use the map  
  SpaceCadet.setMap(spacecadetmap, sizeof(spacecadetmap)/sizeof(spacecadetmap[0]));

  Kaleidoscope.setup();
}
```

##   Plugin methods

The plugin provides the `SpaceCadet` object, with the following methods and
properties:

### `.setMap()`

> Set the key map.  Takes two arguments: the key map and the number of mappings
> in the map array.  The key map is an array of `kaleidoscope::ModifierKeyMap`
> objects, and the size is by default set dynamically by the line
> `sizeof(spacecadetmap)/sizeof(spacecadetmap[0])`
>
> Defaults to mapping left `shift` to `(` and right `shift` to `)`.

### `kaleidoscope::ModifierKeyMap`

> An object consisting of the key that is pressed, the key that should be sent
> in its place, and the timeout (in milliseconds) until the key press is
> considered to be a "held" key press.  The third parameter, the timeout, is
> optional and may be set per-key or left out entirely (or set to `0`) to use 
> the default timeout value.

### `.time_out`

> Set this property to the number of milliseconds to wait before considering a
> held key in isolation as its secondary role. That is, we'd have to hold a
> `Shift` key this long, by itself, to trigger the `Shift` role in itself. This
> timeout setting can be overridden by an individual key in the keymap, but if
> it is omitted or set to `0` in the key map, the global timeout will be used.
>
> Defaults to 1000.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/keyboardio/Kaleidoscope-SpaceCadet/blob/master/examples/SpaceCadet/SpaceCadet.ino
