# Kaleidoscope-MagicCombo

![status][st:stable] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-MagicCombo.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-MagicCombo

 [st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

The `MagicCombo` extension provides a way to perform custom actions when a
particular set of keys are held down together. The functionality assigned to
these keys are not changed, and the custom action triggers as long as all keys
within the set are pressed. The order in which they were pressed do not matter.

This can be used to tie complex actions to key chords.

## Using the extension

To use the extension, we must include the header, create an array of combos we
want to work with, let the plugin know we want to work with those, and then use
a special function to handle the combos:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-MagicCombo.h>

static const KaleidoscopePlugins::MagicCombo::combo_t magic_combos[] PROGMEM = {
  {R1C3 | R2C1 | R2C4 | R2C7, // left hand,
   R0C11 | R1C12 | R2C14      //right hand
  },
  {0, 0}
};

void magicComboActions(uint8_t combo_index, uint32_t left_hand, uint32_t right_hand) {
  switch (combo_index) {
  case 0:
    Serial.println("It's a kind of magic!");
    break;
  }
}

void setup() {
  Serial.begin(9600);

  USE_PLUGINS(&MagicCombo);

  Kaleidoscope.setup();

  MagicCombo.magic_combos = magic_combos;
}
```

The combo list **must** reside in `PROGMEM`, and is a list of tuples. Each
element in the array has two fields: the left hand state, and the right hand
state upon which to trigger the custom action. Both of these are bit fields,
each bit set tells the extension that the key with that index must be held for
the action to trigger. It is recommended to use the `RxCy` macros of the core
`KaleidoscopeFirmware`, and *or* them together to form a bitfield.

The combo list **must** end with an element containing zero values for both the
left and the right halves.

## Extension methods

The extension provides a `MagicCombo` singleton object, with the following
methods and properties:

### `.magic_combos`

> Setting this property lets the plugin know which combinations of key presses
> we are interested in. If any of these are found active, the
> `magicComboActions()` function will be called.

### `.min_interval`

> Restrict the magic action to fire at most once every `minInterval`
> milliseconds.
>
> Defaults to 500.

## Overrideable methods

Whenever an combination is found to be held, the extension will trigger an
action, in each scan cycle until the keys remain held. This is done by calling
the overrideable `magicComboActions` function:

### `magicComboActions(combo_index, left_hand, right_hand)`

> Called whenever a combination is found to be held. The function by default
> does nothing, and it is recommended to override it from within the Sketch.
>
> The first argument will be the index in the combo list, the other two are the
> key states on the left and right halves, respectively.
>
> Plugins that build upon this extensions *should not* override this function,
> but provide helpers that can be called from it. An override should only happen
> in the Sketch.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/keyboardio/Kaleidoscope-MagicCombo/blob/master/examples/MagicCombo/MagicCombo.ino
