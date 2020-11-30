# EEPROM-Keymap-Programmer

Inspired by a similar feature on other keyboards, the `EEPROM-Keymap-Programmer`
plugin implements an on-device keymap re-arrangement / re-coding system. There
are two modes of operation: in one, we need to press a key we want to change,
then another to copy from. In the other, we press a key to change, and then
input a key code (terminated by any non-number key).

## The two modes of operation

It is worth looking at the two separately, to better understand how they work,
and what they accomplish:

### Copy mode

In `COPY` mode, the plugin will use both the built-in, default keymap, and the
override stored in `EEPROM`. When we select a key to override, we need to tap
another, which will be used as the source. The source key's code will be looked
up from the built-in keymap. For example, lets say we want to swap `A` and `B`
for some odd reason. We can do this by triggering the keymap programmer mode,
then tapping `A` to select it as the destination, then `B` as the source. The
plugin will look up the keycode in the built-in keymap for the key in `B`'s
location, and replace the location of `A` in the override with it. Next, we
press the `B` key to select it as the destination, and we press the key that
used to be `A` (but is now `B` too) to select it as a source. Because source
keys are looked up in the built-in keymap, the plugin will find it is `A`.

Obviously, this method only works if we have a built-in keymap, and it does not
support copying from another layer. It is merely a way to rearrange simple
things, like alphanumerics.

### Code mode

In `CODE` mode, instead of selecting a source key, we need to enter a code:
press numbers to input the code, and any non-number key to end the sequence.
Thus, when entering keymap programmer mode, and selecting, say, the `A` key,
then tapping `5 SPACE` will set the key to `B` (which has the keycode of `5`).

This allows us to use keycodes not present on the built-in keymap, at the
expense of having to know the keycode, and allowing no mistakes.

## Using the plugin

Adding the functionality of the plugin to a Sketch is easier the usage explained
above, though it requires that the [EEPROM-Keymap][plugin:eeprom-keymap] plugin
is also used, and set up appropriately.

Once the prerequisites are dealt with, all we need to do is to use the plugin,
and find a way to trigger entering the keymap programmer mode. One such way is
to use a macro, as in the example below:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-EEPROM-Keymap.h>
#include <Kaleidoscope-EEPROM-Keymap-Programmer.h>
#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-Macros.h>

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  if (macroIndex == 0 && keyToggledOff(keyState)) {
    EEPROMKeymapProgrammer.activate();
  }

  return MACRO_NONE;
}

KALEIDOSCOPE_INIT_PLUGINS(EEPROMSettings,
                          EEPROMKeymapProgrammer,
                          EEPROMKeymap,
                          Macros);

void setup() {
  Kaleidoscope.setup();

  Layer.getKey = EEPROMKeymap.getKey;

  EEPROMKeymap.max_layers(1);
  EEPROMSettings.seal();
}
```

The plugin should be used as early as possible, otherwise other plugins that
hook into the event system may start processing events before the programmer can
take over.

## Plugin methods

The plugin provides the `EEPROMKeymapProgrammer` object, which has the following
methods and properties:

### `.activate()`

> Activates the keymap programmer. This is the function one needs to call from -
> say - a macro, to enter the edit state.

### `.mode`

> Set this property to the mode to use for editing: either
> `kaleidoscope::EEPROMKeymapProgrammer::COPY`, or
> `kaleidoscope::EEPROMKeymapProgrammer::CODE`.
>
> Defaults to `kaleidoscope::EEPROMKeymapProgrammer::CODE`.

## Focus commands

The plugin provides a single `Focus` hook: `FOCUS_HOOK_KEYMAP_PROGRAMMER`, which
in turn provides the following command:

### `keymap.toggleProgrammer`

> Toggles the programmer mode on or off.

## Dependencies

* [Kaleidoscope-EEPROM-Keymap][plugin:eeprom-keymap]

  [plugin:eeprom-keymap]: EEPROM-Keymap.md

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

  [plugin:example]: /examples/Features/EEPROM/EEPROM-Keymap-Programmer/EEPROM-Keymap-Programmer.ino
