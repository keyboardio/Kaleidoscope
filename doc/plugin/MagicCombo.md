# Kaleidoscope-MagicCombo

The `MagicCombo` extension provides a way to perform custom actions when a
particular set of keys are held down together. The functionality assigned to
these keys are not changed, and the custom action triggers as long as all keys
within the set are pressed. The order in which they were pressed do not matter.

This can be used to tie complex actions to key chords.

## Using the extension

To use the extension, we must include the header, create actions for the magic
combos we want to trigger, and set up a mapping:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-Macros.h>
#include <Kaleidoscope-MagicCombo.h>

enum { KIND_OF_MAGIC };

void kindOfMagic(uint8_t combo_index) {
  Macros.type(PSTR("It's a kind of magic!"));
}

USE_MAGIC_COMBOS(
[KIND_OF_MAGIC] = {
  .action = kindOfMagic,
  .keys = {R3C6, R3C9} // Left Fn + Right Fn
});

KALEIDOSCOPE_INIT_PLUGINS(MagicCombo, Macros);

void setup() {
  Kaleidoscope.setup();
}
```

It is recommended to use the `RxCy` macros of the core firmware to set the keys
that are part of a combination.

## Plugin properties

The extension provides a `MagicCombo` singleton object, with the following
property:

### `.min_interval`

> Restrict the magic action to fire at most once every `min_interval`
> milliseconds.
>
> Defaults to 500.

## Plugin callbacks

Whenever a combination is found to be held, the plugin will trigger the
specified action, which is just a regular method with a single `uint8_t`
argument: the index of the magic combo. This function will be called repeatedly
(every `min_interval` milliseconds) while the combination is held.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

`RxCy` coordinates for a Model01:

![rxcy layout](../model01_coordinates.png)

 [plugin:example]: ../../examples/MagicCombo/MagicCombo.ino

## Upgrading

To make `MagicCombo` more portable, and easier to use, we had to break the API
previously provided, there was no way to maintain backwards compatibility. This
document is an attempt at guiding you through the process of migrating from the
earlier API to the current one.

Migration should be a straightforward process, but if you get stuck, please feel
free to [open an issue][gh:issues], or start a thread on the [forums][forums],
and we'll help you with it.

 [gh:issues]: https://github.com/keyboardio/Kaleidoscope/issues
 [forums]: https://community.keyboard.io/

## The old API

```c++
void magicComboActions(uint8_t combo_index, uint32_t left_hand, uint32_t right_hand) {
  switch (combo_index) {
  case 0:
    Macros.type(PSTR("It's a kind of magic!"));
    break;
  }
}

static const kaleidoscope::MagicCombo::combo_t magic_combos[] PROGMEM = {
  {
    R3C6,  // left palm key
    R3C9   // right palm key
  },
  {0, 0}
};

void setup() {
  Kaleidoscope.setup();

  MagicCombo.magic_combos = magic_combos;
}
```

Previously, we used a global, overrideable function (`magicComboActions`) to run
the actions of all magic combos, similar to how macros are set up to work.
Unlike macros, magic combos can't be defined in the keymap, due to technical
reasons, so we had to use a separate list - `magic_combos` in our example. We
also needed to tell `MagicCombo` to use this list, which is what we've done in
`setup()`.

## The new API

```c++
void kindOfMagic(uint8_t combo_index) {
  Macros.type(PSTR("It's a kind of magic!"));
}

USE_MAGIC_COMBOS({
  .action = kindOfMagic,
  .keys = {R3C6, R3C9} // Left Fn + Right Fn
});
```

The new API is much shorter, and is inspired by the way the [Leader][leader]
plugin works: instead of having a list, and a dispatching function like
`magicComboActions`, we include the action method in the list too!

 [leader]: Leader.md

We also don't make a difference between left- and right-hand anymore, you can
just list keys for either in the same list. This will be very handy for
non-split keyboards.

## Migration

First of all, we'll need to split up `magicComboActions` into separate
functions. Each function should have a unique name, but their shape is always
the same:

```c++
void someFunction(uint8_t combo_index) {
 // Do some action here
}
```

Copy the body of each `case` statement of `magicComboActions`, and copy them one
by one into appropriately named functions of the above shape. You can name your
functions anything you want, the only constraint is that they need to be valid
C++ function names. The plugin itself does nothing with the name, we'll
reference them later in the `USE_MAGIC_COMBOS` helper macro.

Once `magicComboActions` is split up, we need to migrate the `magic_combos` list
to the new format. That list had to be terminated by a `{0, 0}` entry, the new
method does not require such a sentinel at the end.

For each entry in `magic_combos`, add an entry to `USE_MAGIC_COMBOS`, with the
following structure:

```c++
{.action = theActionFunction,
 .keys = { /* list of keys */ }}
```

The list of keys are the same `RxCy` constants you used for `magic_combos`, with
the left- and right hands combined. The action, `theActionFunction`, is the
function you extracted the magic combo action to. It's the function that has the
same body as the `case` statement in `magicComboActions` had.

And this is all there is to it.

If your actions made use of the `left_hand` or `right_hand` arguments of
`magicComboActions`, the same information is still available. But that's a bit
more involved to get to, out of scope for this simple migration guide. Please
open an issue, or ask for help on the forums, and we'll help you.
