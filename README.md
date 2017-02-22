# Kaleidoscope-GhostInTheFirmware

![status][st:experimental] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-GhostInTheFirmware.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-GhostInTheFirmware

 [st:stable]: https://img.shields.io/badge/stable-✔-black.png?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.png?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.png?style=flat&colorA=dfb317&colorB=494e52

Born out of the desire to demo LED effects on the keyboard without having to
touch it by hand (which would obstruct the video), the `GhostInTheFirmware`
plugin allows one to inject events at various delays, by telling it which keys
to press. Unlike macros, these press keys at given positions, as if they were
pressed by someone typing on it - the firmware will not see the difference.

Given a sequence (with press- and delay times), the plugin will walk through it
once activated, and hold the key for the specified amount, release it, and move
on to the next after the delay time.

## Using the plugin

To use the plugin, one needs to include the header, and configure it with a list
of key coordinates, a press time, and a delay time quartett. One also needs a
way to trigger starting the sequence, and a macro is the most convenient way for
that.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-GhostInTheFirmware.h>
#include <Kaleidoscope-Macros.h>

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  if (macroIndex == 0 && key_toggled_on (keyState))
    GhostInTheFirmware.activate ();

  return MACRO_NONE;
}

static const KaleidoscopePlugins::GhostInTheFirmware::GhostKey ghostKeys[] PROGMEM = {
  {0, 0, 200, 50},
  {0, 0, 0}
};

void setup () {
  Kaleidoscope.setup (KEYMAP_SIZE);

  GhostInTheFirmware.configure (ghostKeys);
  Kaleidoscope.use (&GhostInTheFirmware, &Macros, NULL);
}
```

The plugin won't be doing anything until its `activate()` method is called -
hence the macro.

## Plugin methods

The plugin provides the `GhostInTheFirmware` object, which has the following
method:

### `.configure(sequence)`

> Set the sequence of keys to press. Each element is a quartett of `row`,
> `column`, a `pressTime`, and a `delay`. Each of these will be pressed in
> different cycles, unlike macros which play back within a single cycle.
>
> The key at `row`, `column` will be held for `pressTime` milliseconds, and
> after an additional `delay` milliseconds, the plugin will move on to the next
> entry in the sequence.
>
> The sequence *MUST* reside in `PROGMEM`.

### `.activate()`

> Start playing back the sequence. Best called from a macro.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/keyboardio/Kaleidoscope-GhostInTheFirmware/blob/master/examples/GhostInTheFirmware/GhostInTheFirmware.ino
