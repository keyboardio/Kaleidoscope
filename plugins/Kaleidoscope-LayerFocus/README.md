# LayerFocus

The `LayerFocus` plugin exposes a number of layer-related commands via
[Focus][plugin:focus], to allow controlling layers from the host side.

 [plugin:focus]: FocusSerial.md

## Using the plugin

To use the plugin, we need to include the header, and let the firmware know we
want to use it:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-FocusSerial.h>
#include <Kaleidoscope-LayerFocus.h>

KALEIDOSCOPE_INIT_PLUGINS(
  Focus,
  LayerFocus
);
```

## Focus commands

The plugin provides the following Focus commands:

### `layer.activate N` / `layer.deactivate N` / `layer.isActive N`

> Activates, deactivates, or queries the state of layer `N`.

### `layer.moveTo N`

> Moves to layer `N`, deactivating all other layers in the process.

### `layer.state [STATE...]`

> Without arguments, display the state of all layers, from lower to higher. Each
> active layer will be represented by `1`, while inactive layers will be
> represented by `0`.
>
> With arguments, override the state of layers with the `STATE` given.

## Dependencies

* [Kaleidoscope-FocusSerial](FocusSerial.md)
