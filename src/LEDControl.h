#pragma once

#include <Arduino.h>
#include "KeyboardConfig.h"
#include "plugin.h"

#define LED_MAX_MODES 24

class LEDMode : public KeyboardioPlugin {
 public:
  virtual void begin (void);
  virtual void setup (void) {};
  virtual void init (void) {};
  virtual void update (void) {};
  virtual void activate (void);
};

class LEDControl_ {
  public:
    LEDControl_(void);
    void next_mode(void);
    void setup(void);
    void update(void);
    void set_mode(uint8_t mode);
    uint8_t get_mode();

    int8_t mode_add (LEDMode *mode);

    void set_all_leds_to(uint8_t r, uint8_t g, uint8_t b);
    void set_all_leds_to(cRGB color);

    void activate (LEDMode *mode);

 private:
    LEDMode *modes[LED_MAX_MODES];
    uint8_t previousMode, mode;
};

extern LEDControl_ LEDControl;
