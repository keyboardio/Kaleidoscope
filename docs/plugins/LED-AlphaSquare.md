# LED-AlphaSquare

An alphabet for your per-key LEDs, `AlphaSquare` provides a way to display 4x4
"pixel" symbols on your keyboard. With this building block, one can build some
sweet animations, or just show off - the possibilities are almost endless!

## Using the plugin

To use the plugin, one needs to include the header in their Sketch, tell the
firmware to `use` the plugin, and one way or another, call the `display` method.
This can be done from a macro, or via the `AlphaSquareEffect` LED mode.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LED-AlphaSquare.h>

KALEIDOSCOPE_INIT_PLUGINS(LEDControl,
                          AlphaSquare,
                          AlphaSquareEffect);

void setup() {
  Kaleidoscope.setup();

  AlphaSquare.display (Key_A);
}
```

## Plugin methods

The plugin provides the `AlphaSquare` object, which has its methods and
properties listed below, and an `AlphaSquareEffect` LED mode, which has no
methods or properties other than those provided by all LED modes.

### `.display(key)`
### `.display(key, col)`
### `.display(key, key_addr)`
### `.display(key, key_addr, color)`

> Display the symbol for `key` at the given led address, with pixels set to
> the specified `color`. If only `col` is provided, the first row - `0` is assumed. If
> the column is omitted, then the third column - `2` - is used.
> If the `color` is omitted, the plugin will use the global `.color` property.
>
> The plugin can display the English alphabet, and the numbers from 0 to 9. The
> symbol will be drawn with the top-left corner at the given position.
>
> Please consult the appropriate hardware library of your keyboard to see how
> keys are laid out in rows and columns.

### `.display(symbol)`
### `.display(symbol, col)`
### `.display(symbol, key_addr)`
### `.display(symbol, key_addr, color)`

> As the previous function, but instead of a key, it expects a 4x4 bitmap in
> the form of a 16-bit unsigned integer, where the low bit is the top-right
> corner, the second-lowest bit is to the right of that, and so on.
>
> The `SYM4x4` macro can be used to simplify creating these bitmaps.

### `.clear(key)`, `.clear(symbol)`
### `.clear(key, col)`, `.clear(symbol, col)`
### `.clear(key, key_addr)`, `.clear(symbol, key_addr)`

> Just like the `.display()` counterparts, except these clear the symbol, by
> turning the LED pixels it is made up from off.

### `.color`

> The color to use to draw the pixels.
>
> Defaults to `{ 0x80, 0x80, 0x80 }` (light gray).

## Plugin helpers

### `SYM4x4(...)`

> A helper macro, which can be used to set up custom bitmaps. It expects 16
> values, a 4x4 square of zeroes and ones. Zeroes are transparent pixels, ones
> will be colored.

## Extra symbols

There is a growing number of pre-defined symbols available in the
`kaleidoscope::plugin::alpha_square::symbols` namespace. Ok, growing may have
been an exaggeration, there is only one as of this writing:

### `Lambda`

> A lambda (`λ`) symbol.

## Dependencies

* [Kaleidoscope-LEDControl](LEDControl.md)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: /examples/LEDs/LED-AlphaSquare/LED-AlphaSquare.ino
