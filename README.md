# Akela-GhostInTheFirmware

![status][st:experimental]

 [st:stable]: https://img.shields.io/badge/stable-✔-black.png?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.png?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.png?style=flat&colorA=dfb317&colorB=494e52

Born out of the desire to demo LED effects on the keyboard without having to
touch it by hand (which would obstruct the video), the `GhostInTheFirmware`
plugin allows one to inject events at various delays, by telling it which keys
to press. Unlike macros, these press keys at given positions, as if they were
pressed by someone typing on it - the firmware will not see the difference.

Given a sequence (with delays), the plugin will walk through it once activated,
and hold the key for about a third of the delay amount, then release it.

## Using the plugin

To use the plugin, one needs to include the header, and configure it with a list
of key coordinates and a delay triplet. One also needs a way to trigger starting
the sequence, and a macro is the most convenient way for that.

```c++
#include <Akela-GhostInTheFirmware.h>
#include <Keyboardio-Macros.h>

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  if (macroIndex == 0 && key_toggled_on (keyState))
    GhostInTheFirmware.activate ();

  return MACRO_NONE;
}

static const Akela::GhostInTheFirmware::GhostKey ghostKeys[] PROGMEM = {
  {0, 0, 10},
  {0, 0, 0}
};

void setup () {
  Keyboardio.setup (KEYMAP_SIZE);

  GhostInTheFirmware.configure (ghostKeys);
  Keyboardio.use (&GhostInTheFirmware, &Macros, NULL);
}
```

The plugin won't be doing anything until its `activate()` method is called -
hence the macro.

## Plugin methods

The plugin provides the `GhostInTheFirmware` object, which has the following
method:

### `.configure(sequence)`

> Set the sequence of keys to press. Each element is a triplet of `row`,
> `column`, and a `delay`. Each of these will be pressed in different cycles,
> unlike macros which play back within a single cycle.
>
> The sequence *MUST* reside in `PROGMEM`.

### `.activate()`

> Start playing back the sequence. Best called from a macro.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/keyboardio/Akela-GhostInTheFirmware/blob/master/examples/GhostInTheFirmware/GhostInTheFirmware.ino
