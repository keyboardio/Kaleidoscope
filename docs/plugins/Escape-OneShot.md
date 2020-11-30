# Escape-OneShot

Turn the `Esc` key into a special key, that can cancel any active `OneShot`
effect - or act as the normal `Esc` key if none are active, or if any of them
are still held. For those times when one accidentally presses a one-shot key, or
change their minds.

## Using the plugin

To use the plugin, one needs to include the header, and activate it. No further
configuration is necessary.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-OneShot.h>
#include <Kaleidoscope-Escape-OneShot.h>

KALEIDOSCOPE_INIT_PLUGINS(OneShot,
                          EscapeOneShot);

void setup () {
  Kaleidoscope.setup ();
}
```

The plugin only makes sense when using one-shot keys.

## Plugin methods

The plugin provides the `EscapeOneShot` object, which has no public methods.

## Dependencies

* [Kaleidoscope-OneShot](OneShot.md)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: /examples/Keystrokes/Escape-OneShot/Escape-OneShot.ino
