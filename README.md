# Kaleidoscope-LED-AlphaSquare

![status][st:experimental] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-LED-AlphaSquare.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-LED-AlphaSquare

 [st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

An alphabet for your per-key LEDs, `AlphaSquare` provides a way to display 4x4
"pixel" symbols on your keyboard. With this building block, one can build some
sweet animations, or just show off - the possibilities are almost endless!

## Using the plugin

To use the plugin, one needs to include the header, and one way or another, call
the `display` method.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-LED-AlphaSquare.h>

void setup() {
  USE_PLUGINS(&AlphaSquare);
  
  Kaleidoscope.setup();

  AlphaSquare.display (Key_A);
}
```

## Plugin methods

The plugin provides the `AlphaSquare` object, which has the following methods:

### `.display(key)`
### `.display(key, row, col)`
### `.display(key, col)`
### `.display(key, row, col, color)`

> Display the symbol for `key` at the given row or column, with pixels set to
> the specified `color`. If `row` is omitted, the first row - `0` is assumed. If
> the column is omitted too, then the third - `2` - column is used by default.
> If the `color` is omitted, the plugin will use the global `.color` property.
>
> The plugin can display the English alphabet, and the numbers from 0 to 9.

### `.display(symbol)`
### `.display(symbol, row, col)`
### `.display(symbol, col)`
### `.display(symbol, row, col, color)`

> Almost the same as the previous function, but instead of a key, it expects a
> 4x4 bitmap.

### `.clear(key)`, `.clear(symbol)`
### `.clear(key, col)`, `.clear(symbol, col)`
### `.clear(key, col, row)`, `.clear(symbol, col, row)`

> Just like the `.display()` counterparts, except these clear the symbol, by
> turning the LED pixels it is made up from, off.

### `.color`

> The color to use to draw the pixels.
>
> Not a method itself, but a changeable value.
>
> Defaults to { 0x80, 0x80, 0x80 }.

## Plugin helpers

### `SYM4x4(...)`

> A helper macro, which can be used to set up custom bitmaps. It expects 16
> values, a 4x4 square of zeroes and ones. Zeroes are transparent pixels, ones
> will be colored.

## Extra symbols

There is a growing number of extra symbols available in the
`kaleidoscope::alpha_square::symbols` namespace. Ok, growing may have been an
exaggeration, there is only one as of this writing:

### `Lambda`

> A lambda (`λ`) symbol.

## Dependencies

* [Kaleidoscope-LEDControl](https://github.com/keyboardio/Kaleidoscope-LEDControl)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/keyboardio/Kaleidoscope-LED-AlphaSquare/blob/master/examples/LED-AlphaSquare/LED-AlphaSquare.ino
