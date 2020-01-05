# FirmwareDump

This plugin provides a single Focus command: `firmware.dump`, which dumps the
firmware's executable code. One might rightfully wonder what purpose this serves
when the source code is available, but rest assured, there is one: in case one
wants to temporarily replace their firmware, then put it back on, without having
to carry the HEX file around, this command makes that possible: dump the
contents, turn them into HEX, and it can be re-flashed at any point. We get a
HEX file on-demand, and don't have to carry it around!

The intended primary user of this feature is [Chrysalis][chrysalis].

 [chrysalis]: https://github.com/keyboardio/Chrysalis

## Using the plugin

To use the plugin, include the header, and add it to your list of plugins:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-FocusSerial.h>
#include <Kaleidoscope-FirmwareDump.h>

KALEIDOSCOPE_INIT_PLUGINS(FocusSerial, FirmwareDump);

void setup () {
  Kaleidoscope.setup();
}
```

## Focus commands

The plugin provides a single [Focus][FocusSerial] command:

 [FocusSerial]: FocusSerial.md

### `firmware.dump`

> Dumps the entire firmware (bootloader not included), even the unused parts.

## Dependencies

* [Kaleidoscope-FocusSerial][FocusSerial]
