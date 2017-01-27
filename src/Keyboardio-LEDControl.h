#pragma once

#include <KeyboardioFirmware.h>

#define LED_MAX_MODES 24

#define LED_TOGGLE   B00000001 // Synthetic, internal

#define Key_LEDEffectNext (Key) { 0,  KEY_FLAGS | SYNTHETIC | IS_INTERNAL | LED_TOGGLE }

class LEDMode : public KeyboardioPlugin {
 public:
  virtual void begin (void);
  virtual void setup (void) {};
  virtual void init (void) {};
  virtual void update (void) {};
  virtual void activate (void);
};

class LEDControl_ : public KeyboardioPlugin {
  public:
    LEDControl_(void);

    virtual void begin(void) final;

    static void next_mode(void);
    static void setup(void);
    static void update(void);
    static void set_mode(uint8_t mode);
    static uint8_t get_mode();

    static int8_t mode_add (LEDMode *mode);

    static void led_set_crgb_at(uint8_t i, cRGB crgb);
    static void led_set_crgb_at(byte row, byte col, cRGB color);
    static cRGB led_get_crgb_at(uint8_t i);
    static void led_sync(void);

    static void set_all_leds_to(uint8_t r, uint8_t g, uint8_t b);
    static void set_all_leds_to(cRGB color);

    static void activate (LEDMode *mode);

 private:
    static LEDMode *modes[LED_MAX_MODES];
    static uint8_t previousMode, mode;

    static Key eventHandler(Key mappedKey, byte row, byte col, uint8_t keyState);
    static void loopHook (bool postClear);
};

extern LEDControl_ LEDControl;
