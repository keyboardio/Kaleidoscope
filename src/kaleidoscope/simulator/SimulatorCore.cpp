/* -*- mode: c++ -*-
 * kaleidoscope::simulator - Testing API for Kaleidoscope
 * Copyright (C) 2019  noseglasses (shinynoseglasses@gmail.com)
 * Copyright (C) 2020  Keyboard.io, Inc
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef KALEIDOSCOPE_VIRTUAL_BUILD

#include "kaleidoscope/simulator/SimulatorCore.h"

#include "Kaleidoscope.h"

#undef min
#undef max

#include <map>

namespace kaleidoscope {
namespace simulator {

std::map<uint8_t, const char*> hid_code_to_string = {
  { 0x04, "A   " },  // HID_KEYBOARD_A_AND_A
  { 0x05, "B   " },  // HID_KEYBOARD_B_AND_B
  { 0x06, "C   " },  // HID_KEYBOARD_C_AND_C
  { 0x07, "D   " },  // HID_KEYBOARD_D_AND_D
  { 0x08, "E   " },  // HID_KEYBOARD_E_AND_E
  { 0x09, "F   " },  // HID_KEYBOARD_F_AND_F
  { 0x0A, "G   " },  // HID_KEYBOARD_G_AND_G
  { 0x0B, "H   " },  // HID_KEYBOARD_H_AND_H
  { 0x0C, "I   " },  // HID_KEYBOARD_I_AND_I
  { 0x0D, "J   " },  // HID_KEYBOARD_J_AND_J
  { 0x0E, "K   " },  // HID_KEYBOARD_K_AND_K
  { 0x0F, "L   " },  // HID_KEYBOARD_L_AND_L
  { 0x10, "M   " },  // HID_KEYBOARD_M_AND_M
  { 0x11, "N   " },  // HID_KEYBOARD_N_AND_N
  { 0x12, "O   " },  // HID_KEYBOARD_O_AND_O
  { 0x13, "P   " },  // HID_KEYBOARD_P_AND_P
  { 0x14, "Q   " },  // HID_KEYBOARD_Q_AND_Q
  { 0x15, "R   " },  // HID_KEYBOARD_R_AND_R
  { 0x16, "S   " },  // HID_KEYBOARD_S_AND_S
  { 0x17, "T   " },  // HID_KEYBOARD_T_AND_T
  { 0x18, "U   " },  // HID_KEYBOARD_U_AND_U
  { 0x19, "V   " },  // HID_KEYBOARD_V_AND_V
  { 0x1A, "W   " },  // HID_KEYBOARD_W_AND_W
  { 0x1B, "X   " },  // HID_KEYBOARD_X_AND_X
  { 0x1C, "Y   " },  // HID_KEYBOARD_Y_AND_Y
  { 0x1D, "Z   " },  // HID_KEYBOARD_Z_AND_Z
  { 0x1E, "1 ! " },  // HID_KEYBOARD_1_AND_EXCLAMATION_POINT
  { 0x1F, "2 @ " },  // HID_KEYBOARD_2_AND_AT
  { 0x20, "3 # " },  // HID_KEYBOARD_3_AND_POUND
  { 0x21, "4 $ " },  // HID_KEYBOARD_4_AND_DOLLAR
  { 0x22, "5 % " },  // HID_KEYBOARD_5_AND_PERCENT
  { 0x23, "6 ^ " },  // HID_KEYBOARD_6_AND_CARAT
  { 0x24, "7 & " },  // HID_KEYBOARD_7_AND_AMPERSAND
  { 0x25, "8 * " },  // HID_KEYBOARD_8_AND_ASTERISK
  { 0x26, "9 ( " },  // HID_KEYBOARD_9_AND_LEFT_PAREN
  { 0x27, "0 ) " },  // HID_KEYBOARD_0_AND_RIGHT_PAREN
  { 0x28, "Entr" },  // HID_KEYBOARD_ENTER	{ 0x28	 // (MARKED AS ENTER_SLASH_RETURN)
  { 0x29, "Esc " },  // HID_KEYBOARD_ESCAPE
  { 0x2A, "Del " },  // HID_KEYBOARD_DELETE	{ 0x2A	// (BACKSPACE)
  { 0x2B, "Tab " },  // HID_KEYBOARD_TAB
  { 0x2C, "Spce" },  // HID_KEYBOARD_SPACEBAR
  { 0x2D, "- _ " },  // HID_KEYBOARD_MINUS_AND_UNDERSCORE	{ 0x2D	 // (UNDERSCORE)
  { 0x2E, "= + " },  // HID_KEYBOARD_EQUALS_AND_PLUS
  { 0x2F, "[ { " },  // HID_KEYBOARD_LEFT_BRACKET_AND_LEFT_CURLY_BRACE
  { 0x30, "] } " },  // HID_KEYBOARD_RIGHT_BRACKET_AND_RIGHT_CURLY_BRACE
  { 0x31, "\\ | " },  // HID_KEYBOARD_BACKSLASH_AND_PIPE
  { 0x32, "   ~ " },  // HID_KEYBOARD_NON_US_POUND_AND_TILDE
  { 0x33, "; , " },  // HID_KEYBOARD_SEMICOLON_AND_COLON
  { 0x34, "\' \" " },  // HID_KEYBOARD_QUOTE_AND_DOUBLEQUOTE
  { 0x35, "` ~ " },  // HID_KEYBOARD_GRAVE_ACCENT_AND_TILDE
  { 0x36, ", < " },  // HID_KEYBOARD_COMMA_AND_LESS_THAN
  { 0x37, ". > " },  // HID_KEYBOARD_PERIOD_AND_GREATER_THAN
  { 0x38, "/ ? " },  // HID_KEYBOARD_SLASH_AND_QUESTION_MARK
  { 0x39, "C.L." },  // HID_KEYBOARD_CAPS_LOCK
  { 0x3A, "F1  " },  // HID_KEYBOARD_F1
  { 0x3B, "F2  " },  // HID_KEYBOARD_F2
  { 0x3C, "F3  " },  // HID_KEYBOARD_F3
  { 0x3D, "F4  " },  // HID_KEYBOARD_F4
  { 0x3E, "F5  " },  // HID_KEYBOARD_F5
  { 0x3F, "F6  " },  // HID_KEYBOARD_F6
  { 0x40, "F7  " },  // HID_KEYBOARD_F7
  { 0x41, "F8  " },  // HID_KEYBOARD_F8
  { 0x42, "F9  " },  // HID_KEYBOARD_F9
  { 0x43, "F10 " },  // HID_KEYBOARD_F10
  { 0x44, "F11 " },  // HID_KEYBOARD_F11
  { 0x45, "F12 " },  // HID_KEYBOARD_F12
  { 0x46, "PRTS" },  // HID_KEYBOARD_PRINTSCREEN
  { 0x47, "ScLk" },  // HID_KEYBOARD_SCROLL_LOCK
  { 0x48, "Pse " },  // HID_KEYBOARD_PAUSE
  { 0x49, "Isrt" },  // HID_KEYBOARD_INSERT
  { 0x4A, "Home" },  // HID_KEYBOARD_HOME
  { 0x4B, "PgUp" },  // HID_KEYBOARD_PAGE_UP
  { 0x4C, "Del " },  // HID_KEYBOARD_DELETE_FORWARD
  { 0x4D, "End " },  // HID_KEYBOARD_END
  { 0x4E, "PgDn" },  // HID_KEYBOARD_PAGE_DOWN
  { 0x4F, "→   " },  // HID_KEYBOARD_RIGHT_ARROW
  { 0x50, "←   " },  // HID_KEYBOARD_LEFT_ARROW
  { 0x51, "↓   " },  // HID_KEYBOARD_DOWN_ARROW
  { 0x52, "↑   " },  // HID_KEYBOARD_UP_ARROW
  { 0x53, "NlCl" },  // HID_KEYPAD_NUM_LOCK_AND_CLEAR
  { 0x54, "/   " },  // HID_KEYPAD_DIVIDE
  { 0x55, "*   " },  // HID_KEYPAD_MULTIPLY
  { 0x56, "-   " },  // HID_KEYPAD_SUBTRACT
  { 0x57, "+   " },  // HID_KEYPAD_ADD
  { 0x58, "Entr" },  // HID_KEYPAD_ENTER
  { 0x59, "1 Ed" },  // HID_KEYPAD_1_AND_END
  { 0x5A, "2 ↓ " },  // HID_KEYPAD_2_AND_DOWN_ARROW
  { 0x5B, "3 PD" },  // HID_KEYPAD_3_AND_PAGE_DOWN
  { 0x5C, "4 ← " },  // HID_KEYPAD_4_AND_LEFT_ARROW
  { 0x5D, "5   " },  // HID_KEYPAD_5
  { 0x5E, "6 → " },  // HID_KEYPAD_6_AND_RIGHT_ARROW
  { 0x5F, "7 Hm" },  // HID_KEYPAD_7_AND_HOME
  { 0x60, "8 ↑ " },  // HID_KEYPAD_8_AND_UP_ARROW
  { 0x61, "9 PU" },  // HID_KEYPAD_9_AND_PAGE_UP
  { 0x62, "0 IN" },  // HID_KEYPAD_0_AND_INSERT
  { 0x63, ". DL" },  // HID_KEYPAD_PERIOD_AND_DELETE
  { 0x64, "\\ | " },  // HID_KEYBOARD_NON_US_BACKSLASH_AND_PIPE
  //{ 0x65 }, // HID_KEYBOARD_APPLICATION
  //{ 0x66 }, // HID_KEYBOARD_POWER
  { 0x67, "=   " },  // HID_KEYPAD_EQUALS
  { 0x68, "F13 " },  // HID_KEYBOARD_F13
  { 0x69, "F14 " },  // HID_KEYBOARD_F14
  { 0x6A, "F15 " },  // HID_KEYBOARD_F15
  { 0x6B, "F16 " },  // HID_KEYBOARD_F16
  { 0x6C, "F17 " },  // HID_KEYBOARD_F17
  { 0x6D, "F18 " },  // HID_KEYBOARD_F18
  { 0x6E, "F19 " },  // HID_KEYBOARD_F19
  { 0x6F, "F20 " },  // HID_KEYBOARD_F20
  { 0x70, "F21 " },  // HID_KEYBOARD_F21
  { 0x71, "F22 " },  // HID_KEYBOARD_F22
  { 0x72, "F23 " },  // HID_KEYBOARD_F23
  { 0x73, "F24 " },  // HID_KEYBOARD_F24
  //{ 0x74 ,  }, // HID_KEYBOARD_EXECUTE
  //{ 0x75 ,  }, // HID_KEYBOARD_HELP
  //{ 0x76 ,  }, // HID_KEYBOARD_MENU
  //{ 0x77 ,  }, // HID_KEYBOARD_SELECT
  //{ 0x78 ,  }, // HID_KEYBOARD_STOP
  //{ 0x79 ,  }, // HID_KEYBOARD_AGAIN
  //{ 0x7A ,  }, // HID_KEYBOARD_UNDO
  //{ 0x7B ,  }, // HID_KEYBOARD_CUT
  //{ 0x7C ,  }, // HID_KEYBOARD_COPY
  //{ 0x7D ,  }, // HID_KEYBOARD_PASTE
  //{ 0x7E ,  }, // HID_KEYBOARD_FIND
  //{ 0x7F ,  }, // HID_KEYBOARD_MUTE
  //{ 0x80 ,  }, // HID_KEYBOARD_VOLUME_UP
  //{ 0x81 ,  }, // HID_KEYBOARD_VOLUME_DOWN
  //{ 0x82 ,  }, // HID_KEYBOARD_LOCKING_CAPS_LOCK
  //{ 0x83 ,  }, // HID_KEYBOARD_LOCKING_NUM_LOCK
  //{ 0x84 ,  }, // HID_KEYBOARD_LOCKING_SCROLL_LOCK
  { 0x85, ",   " },  // HID_KEYPAD_COMMA
  { 0x86, "=   " }  // HID_KEYPAD_EQUAL_SIGN
};

unsigned long millis = 0;

void SimulatorCore::init() {
  kaleidoscope::hid::initializeKeyboard();
}

void SimulatorCore::getKeyMatrixDimensions(uint8_t &rows, uint8_t &cols) const {
  rows = kaleidoscope::Device::KeyScanner::matrix_rows;
  cols = kaleidoscope::Device::KeyScanner::matrix_columns;
}

void SimulatorCore::pressKey(uint8_t row, uint8_t col) {
  Kaleidoscope.device().keyScanner().setKeystate(KeyAddr{row, col},
      kaleidoscope::Device::Props::KeyScanner::KeyState::Pressed);
}

void SimulatorCore::releaseKey(uint8_t row, uint8_t col) {
  Kaleidoscope.device().keyScanner().setKeystate(KeyAddr{row, col},
      kaleidoscope::Device::Props::KeyScanner::KeyState::NotPressed);
}

void SimulatorCore::tapKey(uint8_t row, uint8_t col) {
  Kaleidoscope.device().keyScanner().setKeystate(KeyAddr{row, col},
      kaleidoscope::Device::Props::KeyScanner::KeyState::Tap);
}

bool SimulatorCore::isKeyPressed(uint8_t row, uint8_t col) const {
  return Kaleidoscope.device().keyScanner().getKeystate(KeyAddr{row, col})
         == kaleidoscope::Device::Props::KeyScanner::KeyState::Pressed;
}

uint8_t SimulatorCore::getNumLEDs() const {
  return kaleidoscope::Device::Props::LEDDriverProps::led_count;
}

void SimulatorCore::getCurrentKeyLEDColor(uint8_t key_offset,
    uint8_t &red, uint8_t &green, uint8_t &blue) const {
  auto led_id = Kaleidoscope.device().getLedIndex(key_offset);

  auto color = Kaleidoscope.device().getCrgbAt(led_id);

  red = color.r;
  green = color.g;
  blue = color.b;
}

void SimulatorCore::getCurrentKeyLabel(uint8_t row, uint8_t col,
                                       std::string &label_string) const {
  auto key = Layer.lookupOnActiveLayer(KeyAddr{row, col});

  if (key.getFlags() == KEY_FLAGS) {

    // Map the keycode to a string that matches the key
    //
    auto it = hid_code_to_string.find(key.getKeyCode());
    if (it != hid_code_to_string.end()) {
      label_string = it->second;
    }
  }
}

void SimulatorCore::setTime(uint32_t time) {
  millis = time;
}

#define FOR_ALL_KEYBOARD(FUNC) \
   FUNC(A) \
   FUNC(B) \
   FUNC(C) \
   FUNC(D) \
   FUNC(E) \
   FUNC(F) \
   FUNC(G) \
   FUNC(H) \
   FUNC(I) \
   FUNC(J) \
   FUNC(K) \
   FUNC(L) \
   FUNC(M) \
   FUNC(N) \
   FUNC(O) \
   FUNC(P) \
   FUNC(Q) \
   FUNC(R) \
   FUNC(S) \
   FUNC(T) \
   FUNC(U) \
   FUNC(V) \
   FUNC(W) \
   FUNC(X) \
   FUNC(Y) \
   FUNC(Z) \
   FUNC(1) \
   FUNC(2) \
   FUNC(3) \
   FUNC(4) \
   FUNC(5) \
   FUNC(6) \
   FUNC(7) \
   FUNC(8) \
   FUNC(9) \
   FUNC(0) \
   FUNC(Enter) \
   FUNC(Escape) \
   FUNC(Backspace) \
   FUNC(Tab) \
   FUNC(Spacebar) \
   FUNC(Minus) \
   FUNC(Equals) \
   FUNC(LeftBracket) \
   FUNC(RightBracket) \
   FUNC(Backslash) \
   FUNC(NonUsPound) \
   FUNC(Semicolon) \
   FUNC(Quote) \
   FUNC(Backtick) \
   FUNC(Comma) \
   FUNC(Period) \
   FUNC(Slash) \
   FUNC(CapsLock) \
   FUNC(F1) \
   FUNC(F2) \
   FUNC(F3) \
   FUNC(F4) \
   FUNC(F5) \
   FUNC(F6) \
   FUNC(F7) \
   FUNC(F8) \
   FUNC(F9) \
   FUNC(F10) \
   FUNC(F11) \
   FUNC(F12) \
   FUNC(PrintScreen) \
   FUNC(ScrollLock) \
   FUNC(Pause) \
   FUNC(Insert) \
   FUNC(Home) \
   FUNC(PageUp) \
   FUNC(Delete) \
   FUNC(End) \
   FUNC(PageDown) \
   FUNC(RightArrow) \
   FUNC(LeftArrow) \
   FUNC(DownArrow) \
   FUNC(UpArrow) \
   FUNC(KeypadNumLock) \
   FUNC(KeypadDivide) \
   FUNC(KeypadMultiply) \
   FUNC(KeypadSubtract) \
   FUNC(KeypadAdd) \
   FUNC(KeypadEnter) \
   FUNC(Keypad1) \
   FUNC(Keypad2) \
   FUNC(Keypad3) \
   FUNC(Keypad4) \
   FUNC(Keypad5) \
   FUNC(Keypad6) \
   FUNC(Keypad7) \
   FUNC(Keypad8) \
   FUNC(Keypad9) \
   FUNC(Keypad0) \
   FUNC(KeypadDot) \
   FUNC(NonUsBackslashAndPipe) \
   FUNC(PcApplication) \
   FUNC(Power) \
   FUNC(KeypadEquals) \
   FUNC(F13) \
   FUNC(F14) \
   FUNC(F15) \
   FUNC(F16) \
   FUNC(F17) \
   FUNC(F18) \
   FUNC(F19) \
   FUNC(F20) \
   FUNC(F21) \
   FUNC(F22) \
   FUNC(F23) \
   FUNC(F24) \
   FUNC(Execute) \
   FUNC(Help) \
   FUNC(Menu) \
   FUNC(Select) \
   FUNC(Stop) \
   FUNC(Again) \
   FUNC(Undo) \
   FUNC(Cut) \
   FUNC(Copy) \
   FUNC(Paste) \
   FUNC(Find) \
   FUNC(Mute) \
   FUNC(VolumeUp) \
   FUNC(VolumeDown) \
   FUNC(LockingCapsLock) \
   FUNC(LockingNumLock) \
   FUNC(LockingScrollLock) \
   FUNC(KeypadComma) \
   FUNC(KeypadEqualSign) \
   FUNC(International1) \
   FUNC(International2) \
   FUNC(International3) \
   FUNC(International4) \
   FUNC(International5) \
   FUNC(International6) \
   FUNC(International7) \
   FUNC(International8) \
   FUNC(International9) \
   FUNC(Lang1) \
   FUNC(Lang2) \
   FUNC(Lang3) \
   FUNC(Lang4) \
   FUNC(Lang5) \
   FUNC(Lang6) \
   FUNC(Lang7) \
   FUNC(Lang8) \
   FUNC(Lang9) \
   FUNC(AlternateErase) \
   FUNC(Sysreq) \
   FUNC(Cancel) \
   FUNC(Clear) \
   FUNC(Prior) \
   FUNC(Return) \
   FUNC(Separator) \
   FUNC(Out) \
   FUNC(Oper) \
   FUNC(ClearSlashAgain) \
   FUNC(CrselSlashProps) \
   FUNC(Exsel) \
   FUNC(Keypad00) \
   FUNC(Keypad000) \
   FUNC(ThousandsSeparator) \
   FUNC(DecimalSeparator) \
   FUNC(CurrencyUnit) \
   FUNC(CurrencySubunit) \
   FUNC(KeypadLeftParen) \
   FUNC(KeypadRightParen) \
   FUNC(KeypadLeftCurlyBrace) \
   FUNC(KeypadRightCurlyBrace) \
   FUNC(KeypadTab) \
   FUNC(KeypadBackspace) \
   FUNC(KeypadA) \
   FUNC(KeypadB) \
   FUNC(KeypadC) \
   FUNC(KeypadD) \
   FUNC(KeypadE) \
   FUNC(KeypadF) \
   FUNC(KeypadXor) \
   FUNC(KeypadCarat) \
   FUNC(KeypadPercent) \
   FUNC(KeypadLessThan) \
   FUNC(KeypadGreaterThan) \
   FUNC(KeypadAmpersand) \
   FUNC(KeypadDoubleampersand) \
   FUNC(KeypadPipe) \
   FUNC(KeypadDoublepipe) \
   FUNC(KeypadColon) \
   FUNC(KeypadPoundSign) \
   FUNC(KeypadSpace) \
   FUNC(KeypadAtSign) \
   FUNC(KeypadExclamationPoint) \
   FUNC(KeypadMemoryStore) \
   FUNC(KeypadMemoryRecall) \
   FUNC(KeypadMemoryClear) \
   FUNC(KeypadMemoryAdd) \
   FUNC(KeypadMemorySubtract) \
   FUNC(KeypadMemoryMultiply) \
   FUNC(KeypadMemoryDivide) \
   FUNC(KeypadPlusSlashMinus) \
   FUNC(KeypadClear) \
   FUNC(KeypadClearEntry) \
   FUNC(KeypadBinary) \
   FUNC(KeypadOctal) \
   FUNC(KeypadDecimal) \
   FUNC(KeypadHexadecimal) \
   FUNC(LeftControl) \
   FUNC(LeftShift) \
   FUNC(LeftAlt) \
   FUNC(LeftGui) \
   FUNC(RightControl) \
   FUNC(RightShift) \
   FUNC(RightAlt) \
   FUNC(RightGui)

const char *SimulatorCore::keycodeToName(uint8_t keycode) const {

  switch (keycode) {

#define DEFINE_KEY_CASE(KEY) \
      case (Key_##KEY).getKeyCode(): \
         return #KEY; \
         break;

    FOR_ALL_KEYBOARD(DEFINE_KEY_CASE)
  }

  return "";
}

void SimulatorCore::loop() {
  ::loop();
}

} // namespace simulator
} // namespace kaleidoscope

unsigned long millis(void) {
  return kaleidoscope::simulator::millis;
}

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
