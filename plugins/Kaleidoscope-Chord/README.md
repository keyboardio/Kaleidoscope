# Chord

## Concept

This Kaleidoscope plugin allows you to define a chord of keys on your keyboard
which, when pressed simultaneously, produce a single keycode. This differs from
[MagicCombo](https://github.com/keyboardio/Kaleidoscope/tree/master/plugins/Kaleidoscope-MagicCombo)
in that the individual keys making up a chord are suppressed, producing only
the singular result.


## Setup

- Include the header file:
```
#include <Kaleidoscope-Chord.h>
```
- Use the plugin in the `KALEIDOSCOPE_INIT_PLUGINS` macro:
```
KALEIDOSCOPE_INIT_PLUGINS(Chord);
```

And define some chords in `setup` such as:

```
CHORDS(
  CHORD(Key_J, Key_K), Key_Escape,
  CHORD(Key_D, Key_F), Key_LeftShift,
  CHORD(Key_S, Key_D), TOPSY(Semicolon),
  CHORD(Key_S, Key_D, Key_F), Key_Spacebar,
)
```

As can be seen from the example, chords can be overlapping or subsets of each
other, and can result in regular keys, modifier keys or special keys (such as a
[TopsyTurvey](https://github.com/keyboardio/Kaleidoscope/tree/master/plugins/Kaleidoscope-TopsyTurvy)
key). The resulting key will be held for as long as the last key pressed in the
chord is held.

## Configuration

### `.setTimeout(timeout)`

> Sets the time (in milliseconds) after which a key or set of keys that could be
> part of a larger chord is pressed before the pressed keys are resolved. It's
> generally not necessary to explicitly wait for this timeout, since as soon as
> a key is pressed that could not be part of a chord with existing key presses,
> the existing keys will resolve. For instance, with the example above, pressing
> and holding S, D, L in quick succession would result in a held Shift + L. It's
> only if you wanted to type Shift + F, that you'd need to add a pause (S, D,
> wait for timeout, F), since otherwise it would be interpreted as a space.
>
> Defaults to `50`.

## Further reading

The [example](/examples/Keystrokes/Chord/Chord.ino) can help to learn how to use this plugin.
