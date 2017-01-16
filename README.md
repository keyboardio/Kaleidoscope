# Akela-ShapeShifter

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
#include <Akela-ShapeShifter.h>

static const Akela::ShapeShifter::dictionary_t shapeShiftDictionary[] PROGMEM = {
 {Key_1, Key_4},
 {Key_4, Key_1},
 {Key_NoKey, Key_NoKey},
};

void setup () {
  ShapeShifter.configure (shapeShiftDictionary);
  
  Keyboardio.setup (KEYMAP_SIZE);
  Keyboardio.use (&ShapeShifter, NULL);
}
```

The dictionary is made up of `Key` pairs: the first one is to replace, the
second is the replacement. The dictionary must be closed with a `{Key_NoKey,
Key_NoKey}` pair, and **must** reside in `PROGMEM`.

## Plugin methods

The plugin provides the `ShapeShifter` object, with the following methods:

### `.configure(dictionary)`

> Tells `ShapeShifter` to use the specified dictionary. The dictionary is an
> array of `Akela::ShapeShifter::dictionary_t` elements, which is just a very
> verbose way of saying that its a pair of keys. The first one is the one to
> replace, and the other is to replace it with.
>
> Be aware that the replacement key will be pressed with `Shift` held, so do
> keep that in mind!

### `.on()`

> Turns the shape shifting functionality on. Requires that the plugin
> be [configured](#configuredictionary) first.

### `.off()`

> Turns the shape shifting functionality off. In this case, no transformations
> will be applied, and even if symbols appear in the dictionary, they will be
> ignored until the plugin is turned back on.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/Akela-Plugins/Akela-ShapeShifter/blob/master/examples/ShapeShifter/ShapeShifter.ino
