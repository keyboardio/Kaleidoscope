#pragma once

#include <Kaleidoscope.h>

#define LED_MAX_MODES 24

#define LED_TOGGLE   B00000001  // Synthetic, internal

#define Key_LEDEffectNext (Key) { 0,  KEY_FLAGS | SYNTHETIC | IS_INTERNAL | LED_TOGGLE }

namespace kaleidoscope {
/** Base class for LED modes.
 *
 * LED modes are a special kind of plugin, they are in charge of updating LED
 * colors, setting a theme. While it is possible to have other plugins
 * override the mode's colors, the LED mode is the baseline.
 *
 * Most of its functionality is called via @ref LEDControl, with only a few
 * public methods.
 *
 * A LED mode **must** implement at least one of @ref onActivate or @ref
 * update, and possibly @ref refreshAt too.
 */
class LEDMode : public KaleidoscopePlugin {
  friend class LEDControl;
 protected:
  // These methods should only be called by LEDControl.

  /** One-time setup, called at keyboard boot.
   *
   * Any hooks that need registering, any one-time setup that needs to be
   * performed, shall be done here. This is purely for preparation purposes, the
   * LEDs should not be touched yet at this time.
   */
  virtual void setup(void) {}

  /** Function to call whenever the mode is activated.
   *
   * Like @ref setup, this method need not touch LEDs, @ref update will be
   * called right after it. The purpose of this callback is to allow a plugin to
   * do some preparation whenever it is activated, instead of only on boot, or
   * always at each cycle.
   *
   * However, unlike @ref setup, this method can change LED colors, if so
   * desired. Either to provide an initial state, or a static color set. In the
   * latter case, consider implementing @ref refreshAt too, because other
   * plugins may override some of the colors set at activation time, and @ref
   * refreshAt can be used to restore them when needed.
   *
   * Before the callback runs, LEDs will be blanked.
   */
  virtual void onActivate(void) {}

  /** Update the LEDs once per cycle.
   *
   * Usually the brains of the plugin, which updates the LEDs each cycle. It is
   * called after the matrix has been scanned, once per cycle.
   */
  virtual void update(void) {}

  /** Refresh the color of a given key.
   *
   * If we have another plugin that overrides colors set by the active LED mode
   * (either at @onActivate time, or via @ref update), if that plugin wants to
   * restore whatever color the mode would set the key color to, this is the
   * method it will call.
   *
   * @param row is the row coordinate of the key to refresh the color of.
   * @param col is the column coordinate of the key to refresh the color of.
   */
  virtual void refreshAt(byte row, byte col) {}

 public:
  /** Activate the current object as the LED mode.
   */
  void activate(void);

  /** Plugin initialization.
   *
   * Called via `Kaleidoscope.use()`, registers the LED mode, and does the
   * necessary initialization steps. Calls @ref setup at the end.
   */
  void begin(void) final;
};

class LEDControl : public KaleidoscopePlugin {
 public:
  LEDControl(void);

  void begin(void) final;

  static void next_mode(void);
  static void setup(void);
  static void update(void) {
    if (modes[mode])
      modes[mode]->update();
  }
  static void refreshAt(byte row, byte col) {
    if (modes[mode])
      modes[mode]->refreshAt(row, col);
  }
  static void set_mode(uint8_t mode);
  static uint8_t get_mode_index();
  static LEDMode *get_mode();

  static int8_t mode_add(LEDMode *mode);

  static void setCrgbAt(uint8_t i, cRGB crgb);
  static void setCrgbAt(byte row, byte col, cRGB color);
  static cRGB getCrgbAt(uint8_t i);
  static void syncLeds(void);

  static void set_all_leds_to(uint8_t r, uint8_t g, uint8_t b);
  static void set_all_leds_to(cRGB color);

  static void activate(LEDMode *mode);

  static uint16_t syncDelay;

  static bool focusHook(const char *command);

 private:
  static uint32_t syncTimer;
  static LEDMode *modes[LED_MAX_MODES];
  static uint8_t mode;

  static Key eventHandler(Key mappedKey, byte row, byte col, uint8_t keyState);
  static void loopHook(bool postClear);
};
}

extern kaleidoscope::LEDControl LEDControl;

#define FOCUS_HOOK_LEDCONTROL FOCUS_HOOK (LEDControl.focusHook, \
                                          "led.at\n"            \
                                          "led.setAll\n"        \
                                          "led.theme\n"         \
                                          "led.mode")
