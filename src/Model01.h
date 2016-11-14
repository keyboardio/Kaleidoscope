#pragma once

#include <Arduino.h>

#define HARDWARE_IMPLEMENTATION Model01
#include "KeyboardioScanner.h"
#include <Wire.h>

#include "keymap_metadata.h"
#include "key_events.h"






class Model01 {
  public:
    Model01(void);
    void led_sync(void);
    void led_set_crgb_at(byte row, byte col, cRGB color);
    void led_set_crgb_at(uint8_t i, cRGB crgb);
    cRGB led_get_crgb_at(uint8_t i);
    cRGB get_key_color(byte row, byte col);

    void scan_matrix(void);
    void read_matrix(void);
    void act_on_matrix_scan(void);
    void setup();
    void enable_high_power_leds(void);
    void enable_scanner_power(void);
    keydata_t leftHandState;
    keydata_t rightHandState;
    keydata_t previousLeftHandState;
    keydata_t previousRightHandState;

  private:
    static KeyboardioScanner leftHand;
    static KeyboardioScanner rightHand;

    static constexpr uint8_t key_led_map[4][16] = {
        {3,4,11,12,19,20,26,27,     36,37,43,44,51,52,59,60},
        {2,5,10,13,18,21,31,28,     35,32,42,45,50,53,58,61},
        {1,6,9,14, 17,22,25,29,     34,38,41,46,49,54,57,62},
        {0,7,8,15,16,23,24,30,      33,39,40,47,48,55,56,63},
    };


};

#define SCANBIT(row,col) (1 << (row * 8 + (7 - col)))

#define R0C0  SCANBIT(0, 0)
#define R0C1  SCANBIT(0, 1)
#define R0C2  SCANBIT(0, 2)
#define R0C3  SCANBIT(0, 3)
#define R0C4  SCANBIT(0, 4)
#define R0C5  SCANBIT(0, 5)
#define R0C6  SCANBIT(0, 6)
#define R0C7  SCANBIT(0, 7)
#define R1C0  SCANBIT(1, 0)
#define R1C1  SCANBIT(1, 1)
#define R1C2  SCANBIT(1, 2)
#define R1C3  SCANBIT(1, 3)
#define R1C4  SCANBIT(1, 4)
#define R1C5  SCANBIT(1, 5)
#define R1C6  SCANBIT(1, 6)
#define R1C7  SCANBIT(1, 7)
#define R2C0  SCANBIT(2, 0)
#define R2C1  SCANBIT(2, 1)
#define R2C2  SCANBIT(2, 2)
#define R2C3  SCANBIT(2, 3)
#define R2C4  SCANBIT(2, 4)
#define R2C5  SCANBIT(2, 5)
#define R2C6  SCANBIT(2, 6)
#define R2C7  SCANBIT(2, 7)
#define R3C0  SCANBIT(3, 0)
#define R3C1  SCANBIT(3, 1)
#define R3C2  SCANBIT(3, 2)
#define R3C3  SCANBIT(3, 3)
#define R3C4  SCANBIT(3, 4)
#define R3C5  SCANBIT(3, 5)
#define R3C6  SCANBIT(3, 6)
#define R3C7  SCANBIT(3, 7)

#define R0C8  SCANBIT(0, 0)
#define R0C9  SCANBIT(0, 1)
#define R0C10 SCANBIT(0, 2)
#define R0C11 SCANBIT(0, 3)
#define R0C12 SCANBIT(0, 4)
#define R0C13 SCANBIT(0, 5)
#define R0C14 SCANBIT(0, 6)
#define R0C15 SCANBIT(0, 7)
#define R1C8  SCANBIT(1, 0)
#define R1C9  SCANBIT(1, 1)
#define R1C10 SCANBIT(1, 2)
#define R1C11 SCANBIT(1, 3)
#define R1C12 SCANBIT(1, 4)
#define R1C13 SCANBIT(1, 5)
#define R1C14 SCANBIT(1, 6)
#define R1C15 SCANBIT(1, 7)
#define R2C8  SCANBIT(2, 0)
#define R2C9  SCANBIT(2, 1)
#define R2C10 SCANBIT(2, 2)
#define R2C11 SCANBIT(2, 3)
#define R2C12 SCANBIT(2, 4)
#define R2C13 SCANBIT(2, 5)
#define R2C14 SCANBIT(2, 6)
#define R2C15 SCANBIT(2, 7)
#define R3C8  SCANBIT(3, 0)
#define R3C9  SCANBIT(3, 1)
#define R3C10 SCANBIT(3, 2)
#define R3C11 SCANBIT(3, 3)
#define R3C12 SCANBIT(3, 4)
#define R3C13 SCANBIT(3, 5)
#define R3C14 SCANBIT(3, 6)
#define R3C15 SCANBIT(3, 7)


#define LED_COUNT 64


#define LED_PGDN 0
#define LED_PGUP 1
#define LED_BACKTICK 2
#define LED_LED 3
#define LED_1 4
#define LED_Q 5
#define LED_A 6
#define LED_Z 7
#define LED_X 8
#define LED_S 9
#define LED_W 10
#define LED_2 11
#define LED_3 12
#define LED_E 13
#define LED_D 14
#define LED_C 15
#define LED_V 16
#define LED_F 17
#define LED_R 18
#define LED_4 19
#define LED_5 20
#define LED_T 21
#define LED_REC_MACRO 22
#define LED_B 23
#define LED_ESC 24
#define LED_TAB 25
#define LED_REC 26
#define LED_L_FN 27
#define LED_L_CTRL 28
#define LED_DEL 29
#define LED_CMD 30
#define LED_L_SHIFT 31
#define LED_R_SHIFT 32
#define LED_ALT 33
#define LED_SPACE 34
#define LED_CTRL 35
#define LED_R_FN 36
#define LED_ANY 37
#define LED_RETURN 38
#define LED_BUTTERFLY 39
#define LED_N 40
#define LED_H 41
#define LED_Y 42
#define LED_6 43
#define LED_7 44
#define LED_U 45
#define LED_J 46
#define LED_M 47
#define LED_COMMA 48
#define LED_K 49
#define LED_I 50
#define LED_8 51
#define LED_9 52
#define LED_O 53
#define LED_L 54
#define LED_PERIOD 55
#define LED_SLASH 56
#define LED_SEMICOLON 57
#define LED_P 58
#define LED_0 59
#define LED_NUM 60
#define LED_EQUALS 61
#define LED_APOSTROPHE 62
#define LED_MINUS 63

