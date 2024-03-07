# Kaleidoscope-LEDControl

This is a plugin for [Kaleidoscope][fw], for controlling the LEDs, and LED
effects. It is also a building block for plugins that control LEDs.

 [fw]: https://github.com/keyboardio/Kaleidoscope

## Using the extension

## Plugin methods

### `.next_mode(void)`

> Activates the next LED mode. cycles to the first LED mode if the current LED
> mode is the last one.

### `.prev_mode(void)`

> Activates the previous LED mode. cycles to the last LED mode if the current
> LED mode is the first one.

### `.set_mode(uint8_t mode_id)`
### `.get_mode_index()`
### `.get_mode()`
### `.refreshAll()`
### `.setCrgbAt(uint8_t led_index, cRGB crgb)`
### `.setCrgbAt(KeyAddr key_addr, cRGB color)`
### `.getCrgbAt(uint8_t led_index)`
### `.getCrgbAt(KeyAddr key_addr)`
### `.syncLeds(void)`
### `.set_all_leds_to(uint8_t r, uint8_t g, uint8_t b)`
### `.set_all_leds_to(cRGB color)`
### `.setSyncInterval(uint8_t interval)`
### `.setBrightness(uint8_t brightness)`
### `.getBrightness()`

### `.onSetup()`
### `.setup(void)`
### `.onKeyEvent(KeyEvent &event)`
### `.afterEachCycle()`
### `.update(void)`

> Triggers the currently active LED mode to update. It is up to the LED mode to
> handle this correctly.

### `.refreshAt(KeyAddr key_addr)`

> Triggers the currently active LED mode to refresh the LED at the specified key
> address.

### `.activate(LEDModeInterface *plugin)`
### `.disable()`
### `.enable()`
### `.isEnabled()`
