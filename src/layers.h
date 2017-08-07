#pragma once

#include <Arduino.h>
#include "key_defs.h"
#include KALEIDOSCOPE_HARDWARE_H

class Layer_ {
 public:
  Layer_(void);

  static Key lookup(byte row, byte col) {
    return keyMap[row][col];
  };
  static void on(uint8_t layer);
  static void off(uint8_t layer);
  static void move(uint8_t layer);

  static uint8_t top(void);
  static void next(void);
  static void previous(void);

  static boolean isOn(uint8_t layer);

  static void defaultLayer(uint8_t layer);
  static uint8_t defaultLayer(void);

  static uint32_t getLayerState(void);

  static Key eventHandler(Key mappedKey, byte row, byte col, uint8_t keyState);

  static Key(*getKey)(uint8_t layer, byte row, byte col);

  static Key getKeyFromPROGMEM(uint8_t layer, byte row, byte col);

  static void updateKeyCache(byte row, byte col);

  static bool repeat_first_press;

 private:
  static uint8_t highestLayer;
  static Key keyMap[ROWS][COLS];
};

Key layer_getKey(uint8_t layer, uint8_t r, uint8_t c);

extern Layer_ Layer;
