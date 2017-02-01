# Akela-SpaceCadet

![status][st:stable]

 [st:stable]: https://img.shields.io/badge/stable-✔-black.png?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.png?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.png?style=flat&colorA=dfb317&colorB=494e52

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
#include <Akela-SpaceCadet.h>

void setup () {
  Keyboardio.setup (KEYMAP_SIZE);
  Keyboardio.use (&SpaceCadetShift, NULL);
}
```

This assumes a US QWERTY layout on the host computer, and will use the `9` and
`0` keys for the left and right parens, respectively. To change these keys, use
the [`.configure()`](#configureleft-right) method outlined below.

## Plugin methods

The plugin has a number of methods available on the `SpaceCadetShift` object:

### `.configure(left, right)`

> Used to change the configuration of the plugin, namely, the keys used for the
> left and right parens. These keys will be pressed with `Shift` held, and
> should result in the opening and closing parens.
>
> As an example, assuming a Hungarian QWERTZ layout where the parens are not on
> `9` and `0`, we can use the following little snippet in the `setup` method of
> our Sketch:

```c++
void setup () {
  SpaceCadetShift.configure(Key_8, Key_9);
  Keyboardio.setup (KEYMAP_SIZE);
}
```

### `.on()`

> This method turns the SpaceCadet Shift behaviour on, if it was turned off.

### `.off()`

> Turns the SpaceCadet Shift behaviour off, making the `Shift` keys work as they
> did before, without the additional behaviour.

### `.timeOut`

> The number of loop iterations to wait before considering a held key in
> isolation as its secondary role. That is, we'd have to hold a `Shift` key
> this long, by itself, to trigger the `Shift` role in itself.
>
> Not strictly a method, it is a variable one can assign a new value to.
>
> Defaults to 40.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/keyboardio/Akela-SpaceCadet/blob/master/examples/SpaceCadet/SpaceCadet.ino
