# LayerSelect

The `LayerSelect` plugin implements a new key, with which you can switch layers
like you use modifiers: hold the select key, and press the layer number to move
there. For example, to switch to layer three, one would hold `LayerSelect + 3`.

## Using the plugin

To use the plugin, we need to include the header, and let the firmware know we
want to use it:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-LayerSelect.h>

KALEIDOSCOPE_INIT_PLUGINS(
  LayerSelect
);
```

Then, we need to place the `Key_LayerSelect` key somewhere on our keymap.

## Keymap markup

The plugin provides a single new key: `Key_LayerSelect`, with the functionality
described above.

## Overrideable methods

### `LayerSelect::layerSelect(mapped_key, key_addr)`

> This method decides whether a key - or key address, or a combination of both -
> should be considered a layer trigger, when pressed together with the layer
> select key. It should return the layer to switch to, or
> `LayerSelect::NOT_A_SELECTION` if what we have should not be treated as a
> trigger.
>
> The default implementation treats the numbers between 0 and 9 as triggers,
> targeting the appropriate layer.
