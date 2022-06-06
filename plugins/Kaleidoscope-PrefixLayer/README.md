# PrefixLayer

The `PrefixLayer` plugin allows you to easily create a keyboard layer designed
for use with programs that use a prefix key, such as tmux or screen. When a key
in a prefix layer is pressed, the prefix is injected first, then the key in
that layer is pressed.

## Using the plugin

You will need to define a keymap layer and configure the plugin to use that
layer with a prefix key. You can then include the plugin's header and set the
`.prefix_layers` property.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-PrefixLayer.h>

enum {
  PRIMARY,
  TMUX,
};  // layers

static const kaleidoscope::plugin::PrefixLayer::Entry prefix_layers[] PROGMEM = {
  kaleidoscope::plugin::PrefixLayer::Entry(TMUX, LCTRL(Key_B)),
};

KALEIDOSCOPE_INIT_PLUGINS(LEDControl, PrefixLayer);

void setup() {
  Kaleidoscope.setup();
  PrefixLayer.prefix_layers = prefix_layers;
  PrefixLayer.prefix_layers_length = 1;
}
```

## Plugin methods

The plugin provides a `PrefixLayer` object, which has the following methods
and properties:

### `.prefix_layers`

> A `kaleidoscope::plugin::PrefixLayer::Entry` array that maps layers to prefix
> keys. The `Entry` constructor accepts `Entry(layer_number, prefix_key)`. This
> array must be stored in `PROGMEM` as shown above.
>
> Defaults to an empty array.

### `.prefix_layers_length`

> Length of the `prefix_layers` array.
>
> Defaults to *0*

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: /examples/Keystrokes/PrefixLayer/PrefixLayer.ino
