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


# Upgrading old Macros code

This is a guide to upgrading existing Macros code to use the new version of
Kaleidoscope and the Macros plugin.

## New `macroAction()` function

There is a new version of the `macroAction()` function, which is the entry point
for user-defined Macros code. The old version takes two integer parameters, with
the following call signature:

```c++
const macro_t* macroAction(uint8_t macro_id, uint8_t key_state)
```

If your sketch has this function, with a `key_state` bitfield parameter, it
might still work as expected, but depending on the specifics of the code that
gets called from it, your macros might not work as expected. Either way, you
should update that function to the new version, which takes a `KeyEvent`
reference as its second parameter:

```c++
const macro_t* macroAction(uint8_t macro_id, KeyEvent &event)
```

For simple macros, it is a simple matter of replacing `key_state` in the body of
the `macroAction()` code with `event.state`. This covers most cases where all
that's done is a call to `Macros.type()`, or a `MACRO()` or `MACRODOWN()`
sequence is returned.


## Using `MACRO()` and `MACRODOWN()`

The preprocessor macro `MACRODOWN()` has been deprecated, because the event
handler for Macros is no longer called every cycle, but only when a key is
either pressed or released.  Instead of using `return MACRODOWN()`, you should
test for a toggle-on event in `macroAction()` and use `MACRO()` instead.  If you
previously had something like the following in your `macroAction()` function:

```c++
switch(macro_id) {
case MY_MACRO:
  return MACRODOWN(T(X), T(Y), T(Z));
}
```

...you should replace that with:

```c++
switch(macro_id) {
case MY_MACRO:
  if (keyToggledOn(event.state))
    return MACRO(T(X), T(Y), T(Z));
}
```

...or, for a group of macros that should only fire on keypress:

```c++
if (keyToggledOn(event.state)) {
  switch(macro_id) {
  case MY_MACRO:
    return MACRO(T(X), T(Y), T(Z));
  case MY_OTHER_MACRO:
    return MACRO(T(A), T(B), T(C));
  }
}
```

## Releasing keys with `Macros.release()` or `U()`/`Ur()`/`Uc()`

Macros now operates by manipulating keys on a small supplemental virtual
keyboard when using `Macros.press()` and `Macros.release()` (which are called by
`D()` and `U()`, _et al_, respectively).  This means that it has no built-in
facility for releasing other keys that are held on the keyboard.  For example,
if you had a Macro that removed `shift` keycodes from the HID report in the
past, it won't work.  For example:

```c++
  case KEY_COMMA:
    if (keyToggledOn(event.state)) {
      if (Kaleidoscope.hid().keyboard().wasModifierKeyActive(Key_LeftShift)) {
        return MACRO(U(LeftShift), T(Comma), D(LeftShift));
      } else {
        return MACRO(T(M));
      }
    }
```

In this case, holding a physical `Key_LeftShift` and pressing `M(KEY_COMMA)`
will not cause the held `shift` to be released, and you'll get a `<` instead of
the intended `,` (depending on the OS keymap).  To accomplish this, you'll need
a small plugin like the following in your sketch:

```c++
namespace kaleidoscope {
namespace plugin {

// When activated, this plugin will suppress any `shift` key (including modifier
// combos with `shift` a flag) before it's added to the HID report.
class ShiftBlocker : public Plugin {

 public:
  EventHandlerResult onAddToReport(Key key) {
    if (active_ && key.isKeyboardShift())
      return EventHandlerResult::ABORT;
    return EventHandlerResult::OK;
  }

  void enable() {
    active_ = true;
  }
  void disable() {
    active_ = false;
  }

 private:
  bool active_{false};

};

} // namespace plugin
} // namespace kaleidoscope

kaleidoscope::plugin::ShiftBlocker ShiftBlocker;
```

You may also need to define a function to test for held `shift` keys:

```c++
bool isShiftKeyHeld() {
  for (Key key : kaleidoscope:live_keys.all()) {
    if (key.isKeyboardShift())
      return true;
  }
  return false;
}
```

Then, in your `macroAction()` function:

```c++
  if (keyToggledOn(event.state)) {
    switch (macro_id) {
    case MY_MACRO:
      if (isShiftKeyHeld()) {
        ShiftBlocker.enable();
        Macros.tap(Key_Comma);
        ShiftBlocker.disable();
      } else {
        Macros.tap(Key_M);
      }
      return MACRO_NONE;
    }
  }
```

In many simple cases, such as the above example, an even better solution is to
use the CharShift plugin instead of Macros.


## Code that calls `handleKeyswitchEvent()` or `pressKey()`

It is very likely that if you have custom code that calls
`handleKeyswitchEvent()` or `pressKey()` directly, it will no longer function
properly after upgrading. To adapt this code to the new `KeyEvent` system
requires a deeper understanding of the changes to Kaleidoscope, but likely
results in much simpler Macros code.

The first thing that is important to understand is that the `macroAction()`
function will now only be called when a Macros `Key` toggles on or off, not once
per cycle while the key is held.  This is because the new event handling code in
Kaleidoscope only calls plugin handlers in those cases, dealing with one event
at a time, in a single pass through the plugin event handlers (rather than one
pass per active key)--and only sends a keyboard HID report in response to those
events, not once per scan cycle.

This means that any Macros code that is meant to keep keycodes in the keyboard
HID report while the Macros key is held needs to be changed. For example, if a
macro contained the following code:

```c++
if (keyIsPressed(key_state)) {
  Runtime.hid().keyboard().pressKey(Key_LeftShift);
}
```

...that wouldn't work quite as expected, because as soon as the next key is
pressed, a new report would be generated without ever calling `macroAction()`,
and therefore that change to the HID report would not take place, effectively
turning off the `shift` modifier immediately before sending the report with the
keycode that it was intended to modify.

Furthermore, that `shift` modifier would never even get sent in the first place,
because the HID report no longer gets cleared at the beginning of every
cycle. Now it doesn't get cleared until _after_ the plugin event handlers get
called (in the case of Macros, that's `onKeyEvent()`, which calls the
user-defined `macroAction()` function), so any changes made to the HID report
from that function will be discarded before it's sent.

Instead of the above, there are two new mechanisms for keeping keys active while
a Macros key is pressed:

### Alter the `event.key` value

If your macro only needs to keep a single `Key` value active after running some
code, and doesn't need to run any custom code when the key is released, the
simplest thing to do is to override the event's `Key` value:

```c++
if (keyToggledOn(event.state)) {
  // do some macro action(s)
  event.key = Key_LeftShift;
}
```

This will (temporarily) replace the Macros key with the value assigned (in this
case, `Key_LeftShift`), starting immediately after the `macroAction()` function
returns, and lasting until the key is released. This key value can include
modifier flags, or it can be a layer-shift, or any other valid `Key` value
(though it won't get processed by plugins that are initialized before Macros in
`KALEIDOSCOPE_INIT_PLUGINS()`, and Macros itself won't act on the value, if it
gets replaced by a different Macros key).

### Use the supplemental Macros `Key` array

The Macros plugin now contains a small array of `Key` values that will be
included when building HID reports triggered by subsequent, non-Macros
events. To use it, just call one (or more) of the following methods:

```c++
Macros.press(key);
Macros.release(key);
Macros.tap(key)
```

Each one of these functions generates a new artificial key event, and processes
it (including sending a HID report, if necessary). For `press()` and
`release()`, it also stores the specified key's value in the Macros supplemental
`Key` array. In the case of the `tap()` function, it generates matching press
and release events, but skips storing them, assuming that no plugin will
generate an intervening event. All of the events generated by these functions
will be marked `INJECTED`, which will cause Macros itself (and many other
plugins) to ignore them.

This will allow you to keep multiple `Key` values active while a Macros key is
held, while leaving the Macros key itself active, enabling more custom code to
be called on its release.  Note that whenever a Macros key is released, the
supplemental key array is cleared to minimize the chances of keycodes getting
"stuck". It is still possible to write a macro that will cause values to persist
in this array, however, by combining both a sequence that uses key presses
without matched releases _and_ replacing `event.key` (see above) in the same
macro.

### Borrow an idle key (not recommended)

It's also possible to "borrow" one (or more) idle keys on the keyboard by
searching the `live_keys[]` array for an empty entry, and generating a new event
with the address of that key. This is not recommended because surprising things
can happen if that key is then pressed and released, but it's still an option
for people who like to live dangerously.


## Code that calls `sendReport()`

Calling `sendReport()` directly from a macro is now almost always unnecessary.
Instead, a call to `Runtime.handleKeyEvent()` will result in a keyboard HID
report being sent in response to the generated event without needing to make it
explicit.


## Code that uses `Macros.key_addr`

This variable is deprecated. Instead, using the new `macroAction(id, event)`
function, the address of the Macros key is available via the `event.addr`
variable.


## Working with other plugins

### Plugin-specific `Key` values

When the the Macros plugin generates events, it marks the event state as
`INJECTED` in order to prevent unbounded recursion (Macros ignores injected
events).  This causes most other plugins to ignore the event, as well.
Therefore, including a plugin-specific key (e.g. a OneShot modifier such as
`OSM(LeftAlt)`) will most likely be ignored by the target plugin, and will
therefore not have the desired effect. This applies to any calls to
`Macros.play()` (including returning `MACRO()` from `macroAction()`),
`Macros.tap()`, `Macros.press()`, and `Macros.release()`.

### Physical event plugins

Macros cannot usefully produce events handled by plugins that implement the
`onKeyswitchEvent()` handler, such as Qukeys, TapDance, and Leader.  To make
those plugins work with Macros, it's necessary to have the other plugin produce
a Macros key, not the other way around. A `macroAction()` function must not call
`Runtime.handleKeyswitchEvent()`.

### OneShot

This is one plugin that you might specifically want to use with a macro,
generally at the end of a sequence.  For example, a macro for ending one
sentence and beginning the next one might print a period followed by a space
(`. `), then a OneShot shift key tap, so that the next character will be
automatically capitalized. The problem, as mentioned before is that the
following won't work:

```c++
MACRO(Tc(Period), Tc(Spacebar), Tr(OSM(LeftShift)))
```

...because OneShot will ignore the `INJECTED` event.  One solution is to change
the value of `event.key`, turning the pressed Macros key into a OneShot
modifier. This will only work if Macros is registered before OneShot in
`KALEIDOSCOPE_INIT_PLUGINS()`:

```c++
const macro_t* macroNewSentence(KeyEvent &event) {
  if (keyToggledOn(event.state)) {
    event.key = OSM(LeftShift);
    return MACRO(Tc(Period), Tc(Spacebar));
  }
  return MACRO_NONE;
}
```

A more robust solution is to explicitly call `Runtime.handleKeyEvent()`, but
this is more complex, because you'll need to prevent the Macros key from
clobbering the OneShot key in the `live_keys[]` array:

```c++
void macroNewSentence(KeyEvent &event) {
  if (keyToggledOn(event.state)) {
    Macros.tap(Key_Period);
    Macros.tap(Key_Spacebar);
    event.key = OSM(LeftShift);
    kaleidoscope::Runtime.handleKeyEvent(event);
    // Last, we invalidate the current event's key address to prevent the Macros
    // key value from clobbering the OneShot shift.
    event.key = Key_NoKey;
    event.addr.clear();
  }
}
```
