# DefaultLEDModeConfig

The `DefaultLEDModeConfig` plugin provides a way to set a default LED mode, the
LED mode the device starts up with active, via Focus.

By default the first LED mode enabled will be the active one, unless set
otherwise in `setup()`. To make this configurable, without having to reorder the
LED modes, this plugin provides the necessary tools to accomplish that.

## Using the plugin

The example below shows how to use the plugin, including setting up a LED mode
other than the first to use as a default in case EEPROM is uninitialized.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-DefaultLEDModeConfig.h>
#include <Kaleidoscope-LEDEffect-Rainbow.h>
#include <Kaleidoscope-FocusSerial.h>

KALEIDOSCOPE_INIT_PLUGINS(EEPROMSettings,
                          LEDControl,
                          LEDOff,
                          LEDRainbowEffect,
                          LEDRainbowWaveEffect,
                          Focus,
                          DefaultLEDModeConfig);

void setup() {
  Kaleidoscope.setup();

  DefaultLEDModeConfig.activateLEDModeIfUnconfigured(
    &LEDRainbowWaveEffect
  );
}
```

## Plugin methods

The plugin provides a singleton object called `DefaultLEDModeConfig`, with a single method:

### `.activateLEDModeIfUnconfigured(&LEDModePlugin)`

> Activates the LED mode pointed to by `&LEDModePlugin` if and only if the
> EEPROM slice of the plugin is unconfigured. This lets us set a default LED
> mode without persisting it into storage, or hard-coding it.

## Focus commands

### `led_mode.default`

> Without arguments, prints the default LED mode's index.
>
> If an argument is given, it must be the index of the LED mode we wish to set
> as the default.

## Dependencies

* [Kaleidoscope-EEPROM-Settings](Kaleidoscope-EEPROM-Settings.md)
* [Kaleidoscope-FocusSerial](Kaleidoscope-FocusSerial.md)
