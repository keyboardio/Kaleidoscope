# FlashHelper

A number of keyboards supported by Kaleidoscope need a key held during reset to
enter bootloader mode. While held, any action mapped to the key will take
effect, which is something we may want to avoid in this special case. This
plugin is here to help with that: when asked via [Focus](FocusSerial.md), it
will ignore any events on the `Prog` key for a short while (or until asked to
resume).

## Using the plugin

Using the plugin is simple: after including the header, enable the plugin
(preferably before enabling any other plugin that handles key events).

We also need `Focus` enabled, to take full advantage of the plugin.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-FocusSerial.h>
#include <Kaleidoscope-FlashHelper.h>

KALEIDOSCOPE_INIT_PLUGINS(Focus, FlashHelper);

void setup() {
  Kaleidoscope.setup();
}
```

The plugin relies on the device descriptor having set a `Prog` key.

## Plugin methods

The plugin provides the `FlashHelper` object, which has the following methods:

### `.setTimeout(timeout)`
### `.getTimeout()`

Set or get the time (in seconds) to wait before re-enabling the Prog key, after
the helper has been activated.

The timer defaults to **10 seconds**.

### `.activate()`
### `.deactivate()`
### `.isActive()`

Activate, deactivate, or check the status of the flashing helper, respectively.

## Focus commands

The plugin provides two Focus commands: `flash.prepare` and `flash.resume`.

### `flash.prepare`

Prepare for flashing, by disabling the previously configured `Prog` key for a
few seconds, or until explicitly asked to resume. If the keyboard resets
meanwhile, then preparation is automatically concluded.

### `flash.resume`

Resume normal key event handling, and deactivate the helper plugin.

## Dependencies

* [Kaleidoscope-FocusSerial](FocusSerial.md)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting started with the plugin.

  [plugin:example]: ../../examples/Features/FlashHelper/FlashHelper.ino
