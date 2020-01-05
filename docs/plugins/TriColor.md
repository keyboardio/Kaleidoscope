# TriColor

The `TriColor` effect extension is a part of
the [`LEDEffects`][plugin:ledeffects] library, not a stand-alone base library of
its own. It is used to implement the effects in that library.

 [plugin:ledeffects]: LEDEffects.md

It is a class that can be used to create LED effects that all follow a similar
pattern: alphas and similar in one color; modifiers, special keys, and half the
function keys in another, and `Esc` in a third (this latter being optional). If
we have a color scheme that follows this pattern, the `TriColor` extension can
make it a lot easier to implement it.

## Using the extension

Because the extension is part of the [`LEDEffects`][plugin:ledeffects] library,
we need to include that header:

```c++
#include <Kaleidoscope-LEDEffects.h>
```

Then, we simply create a new instance of the `TriColor` class, with appropriate
colors set for the constructor:

```c++
kaleidoscope::plugin::TriColor BlackAndWhiteEffect (CRGB(0x00, 0x00, 0x00),
                                                    CRGB(0xff, 0xff, 0xff),
                                                    CRGB(0x80, 0x80, 0x80));
```

The first argument is the base color, the second is for modifiers and special
keys, the last one is for the `Esc` key. If the last one is omitted, the
extension will use the modifier color for it.
