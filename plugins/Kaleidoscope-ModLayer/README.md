# ModLayer

The ModLayer plugin allows you to define keys that combine the properties of a
keyboard modifier key with a layer shift key.  Holding a Modlayer key will both
shift to the target layer and activate a keyboard modifier until that key is
released.

## Using the plugin

Using the plugin with its defaults is as simple as including the header, and
enabling the plugin:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-ModLayer.h>

KALEIDOSCOPE_INIT_PLUGINS(ModLayer);
```

Then, you can add entries to your keymap for special ModLayer keys, using the
`ML(modifier, layer)` preprocessor macro.  For example:

```c++
// right alt, plus shift to layer 3:
ML(RightAlt, 3)
```

The first argument to `ML()` is the name of a modifier key, without the `Key_`
prefix to make it somewhat shorter.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: /examples/Keystrokes/ModLayer/ModLayer.ino
