# Upgrading Macros code

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
