# Kaleidoscope-LEDControl

This is a plugin for [Kaleidoscope][fw], for controlling the LEDs, and LED
effects. It is also a building block for plugins that control LEDs.

 [fw]: https://github.com/keyboardio/Kaleidoscope

## Using the extension

## Plugin methods

### `.next_mode(void)`

> Activates the next LED mode. Cycles to the first LED mode if the current LED
> mode is the last one.

### `.prev_mode(void)`

> Activates the previous LED mode. Cycles to the last LED mode if the current
> LED mode is the first one.

### `.get_mode()`

> Returns the current LED mode.

### `.get_mode<typename>()`


### `.set_mode(uint8_t mode_id)`

> Activates a LED mode by its index in the firmware. If the index exceeds the
> numer of led modes, the method returns early.

### `.get_mode_index()`

> Returns the index of the currently active LED mode.

### `.refreshAll()`

> If the hardware has LEDs and LEDs are enabled, turn all LEDs off and then
> trigger the current LED mode to refresh.

### `.setCrgbAt(uint8_t led_index, cRGB crgb)`

> Sets the specified LED to the provided color.

### `.setCrgbAt(KeyAddr key_addr, cRGB color)`

> Sets the LED for the specified key to the provided color.

### `.getCrgbAt(uint8_t led_index)`

> Get the LED color of the specified LED.

### `.getCrgbAt(KeyAddr key_addr)`

> Get the LED color of the LED for the specified key.

### `.syncLeds(void)`

> Force an update of all LEDs.

### `.set_all_leds_to(uint8_t r, uint8_t g, uint8_t b)`

> Set all LEDs using the provided rgb values.

### `.set_all_leds_to(cRGB color)`

> Set all LEDs to the specified color.

### `.setSyncInterval(uint8_t interval)`

> Set the interval at which the LEDs should sync, in milliseconds.
>
> Note: LED updates are considered on each cycle of the runtime. Because of
> that, the interval effectively means that _at least_ `interval` milliseconds
> has passed before LEDs are synced.

### `.setBrightness(uint8_t brightness)`

> Set the brightness for all LEDs.

### `.getBrightness()`

> Returns the current brightness of the LEDs as a uint8_t.

### `.onSetup()`

> See [[event-handler-hooks]]

### `.setup(void)`


### `.onKeyEvent(KeyEvent &event)`

> See [[event-handler-hooks]]

### `.afterEachCycle()`

> See [[event-handler-hooks]]

### `.update(void)`

> Triggers the currently active LED mode to update. It is up to the LED mode to
> handle this correctly.

### `.refreshAt(KeyAddr key_addr)`

> Triggers the currently active LED mode to refresh the LED at the specified key
> address.

### `.activate(LEDModeInterface *plugin)`


### `.disable()`

> Turn off all LEDs and disables updating LEDs

### `.enable()`

> Enables updating LEDs and calls `refreshAll()`

### `.isEnabled()`

> Returns a bool value reflecting whether LEDs are currently enabled.
