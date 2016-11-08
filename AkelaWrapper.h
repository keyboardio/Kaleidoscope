#pragma once

#include <Akela.h>
#include <Akela-TapDance.h>

namespace Keyboardio {
  class KeyMap : public Akela::KeyMap::Abstract {
  public:
    KeyMap () {};

    virtual uint16_t lookup (uint8_t index) {
      uint8_t col = index % 16;
      uint8_t row = index / 16 - col;
      return keymaps[temporary_keymap][row][col].keyCode;
    }
  };

  class HID : public Akela::AbstractHID {
  public:
    HID () {};

    virtual void press (uint8_t code) {
      Keyboard.press (code);
    }

    virtual void release (uint8_t code) {
      Keyboard.release (code);
    }

    virtual void sendReport () {
      Keyboard.sendReport ();
    }

    virtual bool isModifierActive (uint8_t code) {
      return false;
    }
  };

  class AkelaInterface : public Akela::TapDance::Component::OneShotMod {
  public:
    KeyMap keyMap;
    HID hid;

    AkelaInterface () {};
  };
};
