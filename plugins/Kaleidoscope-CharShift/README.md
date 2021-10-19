# CharShift

CharShift allows you to independently assign symbols to shifted and unshifted
positions of keymap entries.  Either or both symbols can be ones that normally
requires the `shift` modifier, and either or both symbols can be ones normally
produced without it.

For example you can configure your keyboard so that a single key produces `,`
when pressed unshifted, but `;` when pressed with `shift` held.  Or `(`
unshifted, and `[` shifted.  Or `+`/`*` — all without changing your OS keyboard
layout.

## Using the plugin

Using the plugin with its defaults is as simple as including the header, and
enabling the plugin:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-CharShift.h>

KALEIDOSCOPE_INIT_PLUGINS(CharShift);
```

Further configuration is required, of course; see below.

Note: CharShift should be registered in `KALEIDOSCOPE_INIT_PLUGINS()` after any
plugin that changes the event's `Key` value to that of an CharShift key.

## Configuring CharShift keys

To use CharShift, we must first define `KeyPair` objects, which can then be
referenced by entries in the keymap.  This is easiest to do by using the
`CS_KEYS()` preprocessor macro in the sketch's `setup()` function, as follows:

```c++
void setup() {
  Kaleidoscope.setup();
  CS_KEYS(
    kaleidoscope::plugin::CharShift::KeyPair(Key_Comma, Key_Semicolon),                   // `,`/`;`
    kaleidoscope::plugin::CharShift::KeyPair(Key_Period, LSHIFT(Key_Semicolon)),          // `.`/`:`
    kaleidoscope::plugin::CharShift::KeyPair(LSHIFT(Key_9), Key_LeftBracket),             // `(`/`[`
    kaleidoscope::plugin::CharShift::KeyPair(LSHIFT(Key_Comma), LSHIFT(Key_LeftBracket)), // `<`/`{`
  );
}
```

The first argument to the `KeyPair()` constructor is the value for when the key is
pressed without `shift` held, the second is what you'll get if a `shift`
modifier is being held when the key toggles on.  If that second ("upper") value
doesn't have the `shift` modifier flag (i.e. `LSHIFT()`) applied to it, the held
`shift` modifier will be suppressed when the key is pressed, allowing the
"unshifted" symbol to be produced.

These `KeyPair`s can be referred to in the sketch's keymap by using the `CS()`
preprocessor macro, which takes an integer argument, referring to items in the
`CS_KEYS()` array, starting with zero.  With the example above, an entry of
`CS(2)` will output `(` when pressed without `shift`, and `[` if `shift` is
being held.

## Adding CharShift keys in Chrysalis

As of this writing, CharShift keys can't be defined in Chrysalis; they can only
be defined in a custom sketch (see above).  This doesn't mean that you can't use
them in Chrysalis-defined keymaps, however.  To add a CharShift key in
Chrysalis, select `Custom key code`, and add the offset `53631` to the index number of the
CharShift key.

In other words, where you would use `CS(2)` in a Kaleidoscope sketch, you would
need to use `53633` (`53631 + 2`) as the custom key code in Chrysalis.  Any
CharShift keys referenced in this way still need to be defined in a custom
Kaleidoscope sketch (see above), but they can still be used in a Chrysalis
keymap.

In general, the formula for the Chrysalis custom key code corresponding to the
CharShift key with index `N` is:

`CS(N)` ⟹ `53631 + N`

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: /examples/Keystrokes/CharShift/CharShift.ino
