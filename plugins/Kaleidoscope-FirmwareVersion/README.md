# FirmwareVersion

Implements a new focus command - version - that simply prints the version set up
at compile time.

## Using the plugin

To use the plugin, first define the version to be printed, then include the
header, and activate the plugin.

```c++
#define KALEIDOSCOPE_FIRMWARE_VERSION "0.1.2"

#include <Kaleidoscope.h>
#include <Kaleidoscope-FirmwareVersion.h>
#include <Kaleidoscope-FocusSerial.h>

KALEIDOSCOPE_INIT_PLUGINS(Focus,
                          FirmwareVersion);

void setup () {
  Kaleidoscope.setup ();
}
```

## Focus commands

The plugin provides a single Focus command: `version`.

### `version`

> Prints the version configured at build time.

## Dependencies

* [Kaleidoscope-FocusSerial](Kaleidoscope-FocusSerial.md)
