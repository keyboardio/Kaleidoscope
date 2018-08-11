# Kaleidoscope-Macros

![status][st:stable] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-Macros.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-Macros

 [st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

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

To use the plugin, we need to include the header, tell the firmware to `use` the
plugin, place macros on the keymap, and create a special handler function
(`macroAction`) that will tell the plugin what shall happen when macro keys are
pressed. It is best illustrated with an example:

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
const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  switch (macroIndex) {
  case MACRO_MODEL01:
    return MACRODOWN(I(25),
                     D(LeftShift), T(M), U(LeftShift), T(O), T(D), T(E), T(L),
                     T(Spacebar),
                     W(100),
                     T(0), T(1) );
  case MACRO_HELLO:
    if (keyToggledOn(keyState)) {
      return Macros.type(PSTR("Hello "), PSTR("world!"));
    }
    break;
  case MACRO_SPECIAL:
    if (keyToggledOn(keyState)) {
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
> The `macro` argument must be a sequence created with the `MACRO()` helper!

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

### `.row`, `.col`

> The `row` and `col` properties describe the physical position a macro was
> triggered from if it was triggered by a key. The playback functions
> do not use these properties, but they are available, would one want to create
> a macro that needs to know which key triggered it.
>
> When the macro was not triggered by a key the value of these properties are
> unspecified.

## Macro helpers

Macros need to be able to simulate key down and key up events for any key - even
keys that may not be on the keymap otherwise. For this reason and others, we
need to define them in a special way, using the `MACRO` helper (or its
`MACRODOWN()` variant, see below):

### `MACRO(steps...)`

> Defines a macro, that is built up from `steps` (explained below). The plugin
> will iterate through the sequence, and re-play the steps in order.
>
> Note: In older versions of the Macros plugin, the sequence of steps had to end
> with a special step called END.  This is no longer required.  Existing macros
> that end with END will still work correctly, but new code should not use END;
> usage of END is deprecated.

### `MACRODOWN(steps...)`

> The same as the `MACRO()` helper above, but it will create a special sequence,
> where the steps are only played back when the triggering key was just pressed.
> That is, the macro will not be performed when the key is released, or held, or
> not pressed at all.
>
> Use this over `MACRO()` when you only want to perform an action when the key
> actuates, and no action should be taken when it is held, released, or when it
> is not pressed at all. For a lot of macros that emit a sequence without any
> other side effects, `MACRODOWN()` is usually the better choice.
>
> Can only be used from the `macroAction()` overrideable method.

## `MACRO` steps

Macro steps can be divided into two groups:

### Delays

* `I(millis)`: Sets the interval between steps to `millis`. By default, there is
  no delay between steps, and they are played back as fast as possible. Useful
  when we want to see the macro being typed, or need to slow it down, to allow
  the host to process it.
* `W(millis)`: Waits for `millis` milliseconds. For dramatic effects.

### Key events

Key event steps have two variants: one that prefixes its argument with `Key_`,
and one that does not. The latter are the `Dr`, `Ur`, and `Tr` variants. In most
cases, one is likely to use normal keys for the steps, so the `D`, `U`, and `T`
steps apply the `Key_` prefix. This allows us to write `MACRO(T(X))`
instead of `MACRO(Tr(Key_X))` - making the macro definition shorter, and
more readable.

* `D(key)`, `Dr(key)`: Simulates a key being pressed (pushed down).
* `U(key)`, `Ur(key)`: Simulates a key being released (going up).
* `T(key)`, `Tr(key)`: Simulates a key being tapped (pressed first, then released).

## Overrideable methods

### `macroAction(macroIndex, keyState)`

> The `macroAction` method is the brain of the macro support in Kaleidoscope:
> this function tells the plugin what sequence to play when given a macro index
> and a key state.
>
> It should return a macro sequence, or `MACRO_NONE` if nothing is to be played
> back.
