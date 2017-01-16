# Akela-MagicCombo

![status][st:stable]

 [st:stable]: https://img.shields.io/badge/stable-✔-black.png?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.png?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.png?style=flat&colorA=dfb317&colorB=494e52

The `MagicCombo` extension provides a way to perform custom actions when a
particular set of keys are held down together. The functionality assigned to
these keys are not changed, and the custom action triggers as long as all keys
within the set are pressed. The order in which they were pressed do not matter.

This can be used to tie complex actions to key chords.

## Using the extension

To use the extension, we must include the header, create a dictionary, and
configure the plugin to use it:

```c++
#include <Akela-MagicCombo.h>

static const Akela::MagicCombo::dictionary_t dictionary[] PROGMEM = {
  {R1C3 | R2C1 | R2C4 | R2C7, // left hand,
   R0C11 | R1C12 | R2C14      //right hand
  },
  {0, 0}
};

void setup (void) {
  MagicCombo.configure (dictionary);

  Keyboardio.setup (KEYMAP_SIZE);
  Keyboardio.use (&MagicCombo, NULL);
}
```

The dictionary **must** reside in `PROGMEM`, and is a list of tuples. Each
element in the array has two fields: the left hand state, and the right hand
state upon which to trigger the custom action. Both of these are bit fields,
each bit set tells the extension that the key with that index must be held for
the action to trigger. It is recommended to use the `RxCy` macros of the core
`KeyboardioFirmware`, and *or* them together to form a bitfield.

The dictionary **must** end with an element containing zero values for both the
left and the right halves.

## Extension methods

The extension provides a `MagicCombo` singleton object, with the following method:

### `.configure(dictionary[, timeout])`

> Configures the extension to use the supplied dictionary, and restrict it to
> fire at most once every `timeout` cycles.
>
> If the timeout is not specified, it defaults to `DEFAULT_TIMEOUT`, which in
> turn is 40 cycles.

## Overrideable methods

Whenever an combination is found to be held, the extension will trigger an
action, in each scan cycle until the keys remain held. This is done by calling
the overrideable `magicComboActions` function:

### `magicComboActions(comboIndex, leftHand, rightHand)`

> Called whenever a combination is found to be held. The function by default
> does nothing, and it is recommended to override it from within the Sketch.
>
> The first argument will be the index in the dictionary, the other two are the
> key states on the left and right halves, respectively.
>
> Plugins that build upon this extensions *should not* override this function,
> but provide helpers that can be called from it. An override should only happen
> in the Sketch.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/Akela-Plugins/Akela-MagicCombo/blob/master/examples/MagicCombo/MagicCombo.ino
