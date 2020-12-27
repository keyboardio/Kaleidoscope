# ShapeShifter

`ShapeShifter` is a plugin that makes it considerably easier to change what
symbol is input when a key is pressed together with `Shift`. If one wants to
rearrange the symbols on the number row for example, without modifying the
layout on the operating system side, this plugin is where one can turn to.

What it does, is very simple: if any key in its dictionary is found pressed
while `Shift` is held, it will press another key instead of the one triggering
the event. For example, if it sees `Shift + 1` pressed together, which normally
results in a `!`, it will press `4` instead of `1`, inputting `$`.

## Using the plugin

To use the plugin, one needs to include the header, create a dictionary, and
configure the provided `ShapeShifter` object to use the dictionary:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-ShapeShifter.h>

static const kaleidoscope::plugin::ShapeShifter::dictionary_t shape_shift_dictionary[] PROGMEM = {
 {Key_1, Key_4},
 {Key_4, Key_1},
 {Key_NoKey, Key_NoKey},
};

KALEIDOSCOPE_INIT_PLUGINS(ShapeShifter);

void setup() {
  Kaleidoscope.setup();

  ShapeShifter.dictionary = shape_shift_dictionary;
}
```

The dictionary is made up of `Key` pairs: the first one is to replace, the
second is the replacement. The dictionary must be closed with a `{Key_NoKey,
Key_NoKey}` pair, and **must** reside in `PROGMEM`.

## Plugin methods

The plugin provides the `ShapeShifter` object, with the following methods and
properties:

### `.dictionary`

> Set this property to the dictionary `ShapeShifter` should use. The dictionary
> is an array of `kaleidoscope::ShapeShifter::dictionary_t` elements, which is
> just a very verbose way of saying that its a pair of keys. The first one is
> the one to replace, and the other is to replace it with.
>
> Be aware that the replacement key will be pressed with `Shift` held, so do
> keep that in mind!

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: /examples/Keystrokes/ShapeShifter/ShapeShifter.ino
