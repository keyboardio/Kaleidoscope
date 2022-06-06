# Macros

Macros are a standard feature on many keyboards and Kaleidoscope-powered ones
are no exceptions. Macros are a way to have a single key-press do a whole lot of
things under the hood: conventionally, macros play back a key sequence, but with
Kaleidoscope, there is much more we can do. Nevertheless, playing back a
sequence of events is still the primary use of macros.

Playing back a sequence means that when we press a macro key, we can have it
play pretty much any sequence. It can type some text for us, or invoke a
complicated shortcut - the possibilities are endless!

In Kaleidoscope, macros are implemented via this plugin. You can define upto 256 macros.

## Using the plugin

To use the plugin, we need to include the header, initialize the plugins with
`KALEIDOSCOPE_INIT_PLUGINS()`, place macros on the keymap, and create a special
handler function (`macroAction()`) that will determine what happens when macro
keys are pressed. It is best illustrated with an example:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-Macros.h>

// Give a name to the macros!
enum {
  MACRO_MODEL01,
  MACRO_HELLO,
  MACRO_SPECIAL,
};

// Somewhere in the keymap:
M(MACRO_MODEL01), M(MACRO_HELLO), M(MACRO_SPECIAL)

// later in the Sketch:
const macro_t *macroAction(uint8_t macro_id, KeyEvent &event) {
  switch (macro_id) {
  case MACRO_MODEL01:
    if (keyToggledOn(event.state)) {
      return MACRO(I(25),
                   D(LeftShift), T(M), U(LeftShift), T(O), T(D), T(E), T(L),
                   T(Spacebar),
                   W(100),
                   T(0), T(1) );
    }
    break;
  case MACRO_HELLO:
    if (keyToggledOn(event.state)) {
      return Macros.type(PSTR("Hello "), PSTR("world!"));
    }
    break;
  case MACRO_SPECIAL:
    if (keyToggledOn(event.state)) {
      // Do something special
    }
    break;
  }
  return MACRO_NONE;
}

KALEIDOSCOPE_INIT_PLUGINS(Macros);

void setup() {
  Kaleidoscope.setup ();
}
```

## Keymap markup

### `M(id)`

> Places a macro key on the keymap, with the `id` number (0 to 255) as identifier. Whenever this key
> has to be handled, the `macroAction` overrideable function will be called,
> with the identifier and key state as arguments.
>
> It is recommended to give a *name* to macro ids, by using an `enum`.

## Plugin methods

The plugin provides a `Macros` object, with the following methods and properties available:

### `.play(macro)`

> Plays back a macro, where a macro is a sequence created with the `MACRO()`
> helper discussed below. This method will be used by the plugin to play back
> the result of the `macroAction()` method, but is used rarely otherwise.
>
> The `macro` argument must be a sequence created with the `MACRO()` helper! For example:
>
> Macros.play(MACRO(D(LeftControl), D(LeftAlt), D(Spacebar), U(LeftControl), U(LeftAlt), U(Spacebar)));

### `.type(strings...)`

> In cases where we only want to type some strings, it is far more convenient to
> use this method: we do not have to use the `MACRO()` helper, but just give
> this one a set of strings, and it will type them for us on the keyboard. We
> can use as many strings as we want, and all of them will be typed in order.
>
> Each string is limited to a sequence of printable ASCII characters. No
> international symbols, or unicode, or anything like it: just plain ASCII.
>
> Each of `strings` arguments must also reside in program memory, and the
> easiest way to do that is to wrap the string in a `PSTR()` helper. See the
> program code at the beginning of this documentation for an example!

### `.press(key)`/`.release(key)`

> Used in `Macros.play()`, these methods press virtual keys in a small
> supplemental `Key` array for the purpose of keeping keys active for complex
> macro sequences where it's important to have overlapping key presses.
>
> `Macros.press(key)` sends a key press event, and will keep that virtual key
> active until either `Macros.release(key)` is called, or a Macros key is
> released. If you use `Macros.press(key)` in a macro, but also change the value
> of `event.key`, you will need to make sure to also call `Macros.release(key)`
> at some point to prevent that key from getting "stuck" on.

### `.clear()`

> Releases all virtual keys held by macros. This both empties the supplemental
> `Key` array (see above) and sends a release event for each key stored there.

### `.tap(key)`

> Sends an immediate press and release event for `key` with no delay, using an
> invalid key address.


## Macro helpers

Macros need to be able to simulate key down and key up events for any key - even
keys that may not be on the keymap otherwise. For this reason and others, we
need to define them in a special way, using the `MACRO` helper.

### `MACRO(steps...)`

> Defines a macro, that is built up from `steps` (explained below). The plugin
> will iterate through the sequence, and re-play the steps in order.
>
> Note: In older versions of the Macros plugin, the sequence of steps had to end
> with a special step called END.  This is no longer required.  Existing macros
> that end with END will still work correctly, but new code should not use END;
> usage of END is deprecated.

## `MACRO` steps

Macro steps can be divided into the following groups:

### Delays

* `I(millis)`: Sets the interval between steps to `millis`. By default, there is
  no delay between steps, and they are played back as fast as possible. Useful
  when we want to see the macro being typed, or need to slow it down, to allow
  the host to process it.
* `W(millis)`: Waits for `millis` milliseconds. For dramatic effects.

### Key events

Key event steps have three variants: one that prefixes its argument with `Key_`,
one that does not, and a third that allows for a more compact - but also more
limited - representation. The first are the `D`, `U`, and `T` variants, the
second are `Dr`, `Ur`, and `Tr`, and the last variant are `Dc`, `Uc`, and `Tc`.
In most cases, one is likely use normal keys for the steps, so the `D`, `U`, and
`T` steps apply the `Key_` prefix. This allows us to write `MACRO(T(X))` instead
of `MACRO(Tr(Key_X))` - making the macro definition shorter, and more readable.

The "raw" variants (`Dr`/`Ur`/`Tr`) use the full name of the `Key` object,
without adding the `Key_` prefix to the argument given. `Tr(Key_X)` is the same
as `T(X)`.

The "compact" variants (`Dc`/`Uc`/`Tc`) prefix the argument with `Key_` too,
but unlike `D`, `U`, and `T`, they ignore the `flags` component of the key, and
as such, are limited to ordinary keys. Mouse keys, consumer- or system keys are
not supported by this compact representation.

* `D(key)`, `Dr(key)`, `Dc(key)`: Simulates a key being pressed (pushed down).
* `U(key)`, `Ur(key)`, `Uc(key)`: Simulates a key being released (going up).
* `T(key)`, `Tr(key)`, `Tc(key)`: Simulates a key being tapped (pressed first, then released).

### Key sequences

One often used case for macros is to type longer sequences of text. In these
cases, assembling the macro step by step using the events described above is
verbose both in source code, and compiled. For this reason, the plugin provides
two other actions, both of which take a sequence of keys, and will tap all of
them in order.

* `SEQ(K(key1), K(key2), ...)`: Simulates all the given keys being tapped in
  order, with the currently active interval waited between them. The keys need
  to be specified by their full name.
* `SEQc(Kc(key1), Kc(key2), ...)`: Same as `SEQ()`, but the keys are prefixed
  with `Key_`, and they ignore the `flags` component of a key, and as such, are
  limited to ordinary keys.

## Overrideable functions

### `macroAction(uint8_t macro_id, KeyEvent &event)`

> The `macroAction` method is the brain of the macro support in Kaleidoscope:
> this function tells the plugin what sequence to play when given a macro index
> and a key state.
>
> It should return a macro sequence, or `MACRO_NONE` if nothing is to be played
> back.

## Limitations

Due to technical and practical reasons, `Macros.type()` assumes a QWERTY layout
on the host side, and so do all other parts that work with keycodes. If your
operating system is set to a different layout, the strings and keycodes will
need to be adjusted accordingly.

## Dependencies

* [Kaleidoscope-MacroSupport](Kaleidoscope-MacroSupport.md)
