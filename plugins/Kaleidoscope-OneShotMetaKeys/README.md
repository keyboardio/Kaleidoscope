# OneShot Meta Keys

This plugin provides support for two special OneShot keys:
`OneShot_MetaStickyKey` & `OneShot_ActiveStickyKey`, each of which can be used
to make any key on the keyboard (not just modifiers and layer shift keys)
"sticky", so that they remain active even after the key has been released.
These are both `Key` values that can be used as entries in your sketch's keymap.

Any keys made sticky in this way can be released just like OneShot modifier
keys, by tapping them again to cancel the effect.

## The `OneShot_MetaStickyKey`

This special OneShot key behaves like other OneShot keys, but its affect is to
make the next key pressed sticky. Tap `OneShot_MetaStickyKey`, then tap `X`, and
`X` will become sticky. Tap `X` again to deactivate it.

Double-tapping `OneShot_MetaStickyKey` will make it sticky, just like any other
OneShot key.  A third tap will release the key.

## The `OneShot_ActiveStickyKey`

This special key doesn't act like a OneShot key, but instead makes any key(s)
currently held (or otherwise active) sticky. Press (and hold) `X`, tap
`OneShot_ActiveStickyKey`, then release `X`, and `X` will stay active until it
is tapped again to deactivate it.

## Using the plugin

To use the plugin, just include one of the two special OneShot keys somewhere in
your keymap, and add both OneShot and OneShotMetaKeys to your sketch:

```c++
#include <Kaleidoscope-OneShot.h>
#include <Kaleidoscope-OneShotMetaKeys.h>

// somewhere in the keymap...
OneShot_MetaStickyKey, OneShot_ActiveStickyKey

KALEIDOSCOPE_INIT_PLUGINS(OneShot, OneShotMetaKeys);
```

Important note: OneShotMetaKeys _must_ be registered after OneShot in
`KALEIDOSCOPE_INIT_PLUGINS()` in order to function properly.

## Dependencies

* [Kaleidoscope-OneShot](Kaleidoscope-OneShot.md)
* [Kaleidoscope-Ranges](Kaleidoscope-Ranges.md)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: /examples/Keystrokes/OneShotMetaKeys/OneShotMetaKeys.ino
