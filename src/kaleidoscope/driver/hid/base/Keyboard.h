// -*- mode: c++ -*-
/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2019  Keyboard.io, Inc.
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

#pragma once
#include <Arduino.h>

#include "kaleidoscope/key_defs.h"

namespace kaleidoscope {
namespace driver {
namespace hid {
namespace base {

class NoBootKeyboard {
 public:
  NoBootKeyboard() {}
  void begin() {}

  uint8_t getProtocol() {
    return 1;
  }
  void setProtocol(uint8_t protocol) {}
  void setDefaultProtocol(uint8_t protocol) {}

  void sendReport();

  void press(uint8_t code) {}
  void release(uint8_t code) {}
  void releaseAll() {}

  bool isModifierActive(uint8_t code) {
    return false;
  }
  bool wasModifierActive(uint8_t code) {
    return false;
  }
  bool isAnyModifierActive() {
    return false;
  }
  bool wasAnyModifierActive() {
    return false;
  }

  uint8_t getLeds() {
    return 0;
  }
};

class NoNKROKeyboard {
 public:
  NoNKROKeyboard() {}
  void begin() {}

  void sendReport() {}

  void press(uint8_t code) {}
  void release(uint8_t code) {}
  void releaseAll() {}

  bool isModifierActive(Key key) {
    return false;
  }
  bool wasModifierActive(Key key) {
    return false;
  }
  bool isAnyModifierActive() {
    return false;
  }
  bool wasAnyModifierActive() {
    return false;
  }

  uint8_t getLeds() {
    return 0;
  }
};

class NoConsumerControl {
 public:
  NoConsumerControl() {}
  void begin() {}

  void sendReport() {}
  void releaseAll() {}

  void press(uint8_t code) {}
  void release(uint8_t code) {}
};

class NoSystemControl {
 public:
  NoSystemControl() {}
  void begin() {}

  void press(uint8_t code) {}
  void release() {}
};

struct KeyboardProps {
  typedef NoBootKeyboard BootKeyboard;
  typedef NoNKROKeyboard NKROKeyboard;
  typedef NoConsumerControl ConsumerControl;
  typedef NoSystemControl SystemControl;
};

template <typename _Props>
class Keyboard {
 private:
  typename _Props::BootKeyboard boot_keyboard_;
  typename _Props::NKROKeyboard nkro_keyboard_;
  typename _Props::ConsumerControl consumer_control_;
  typename _Props::SystemControl system_control_;
 public:
  Keyboard() {}

  void setup() __attribute__((noinline)) {
    boot_keyboard_.begin();
    nkro_keyboard_.begin();
    consumer_control_.begin();
    system_control_.begin();
  }

  void sendReport() __attribute__((noinline)) {
    // Before sending the report, we add any modifier flags that are currently
    // allowed, based on the latest keypress:
    pressModifiers(requested_modifier_flags & modifier_flag_mask);

    // If a key has been toggled on in this cycle, we might need to send an extra
    // HID report to the host, because that key might have the same keycode as
    // another key that was already in the report on the previous cycle. For
    // example, a user could have two `Key_E` keys in their keymap, in order to
    // avoid repeating the same key with one finger. Or one might have a
    // `LCTRL(Key_S)` and a plain `Key_S`, and have a reason to press them in
    // rapid succession. In order to make this work, we need to call `release()` &
    // `sendReport()` to send a release event to the host so that its normal
    // repeat-rate-limiting behaviour won't effectively mask the second keypress.
    // Then we call `press()` to add the keycode back in before sending the normal
    // report.
    //
    // In most cases, this won't result in any difference from the previous report
    // (because the newly-toggled-on keycode won't be in the previous report), so
    // no extra report will be sent (because we suppress duplicate reports in
    // KeyboardioHID). If there is a difference in the modifiers byte, an extra
    // report would be sent later, regardless (also in KeyboardioHID).
    //
    // Furthermore, we need to send a report without the keycode for the
    // newly-toggled-on key, but with any masked modifiers from flags removed. For
    // example, if we roll over from `LSHIFT(Key_A)` to `Key_B`, we need to first
    // send a report without the `shift`, then a second report with the `B`. If
    // both of those bits are changed in the same report, at least one major OS
    // processes the `B` keypress first, and we end up with `AB` instead of `Ab`
    // in the output.

    if (boot_keyboard_.getProtocol() == HID_BOOT_PROTOCOL) {
      if (last_keycode_toggled_on) {
        boot_keyboard_.release(last_keycode_toggled_on);
        boot_keyboard_.sendReport();
        boot_keyboard_.press(last_keycode_toggled_on);
        last_keycode_toggled_on = 0;
      }
      boot_keyboard_.sendReport();
      return;
    }

    if (last_keycode_toggled_on) {
      // It would be good if KeyboardioHID's Keyboard object offered a way to
      // compare the modifiers bytes of the current and previous key reports. That
      // would allow us to only send these extra reports when either
      // `last_keycode_toggled_on` was already held, or the modifiers byte
      // changed. Likewise for BootKeyboard above.
      nkro_keyboard_.release(last_keycode_toggled_on);
      nkro_keyboard_.sendReport();
      nkro_keyboard_.press(last_keycode_toggled_on);
      last_keycode_toggled_on = 0;
    }

    nkro_keyboard_.sendReport();
    consumer_control_.sendReport();
  }
  void releaseAllKeys() __attribute__((noinline)) {
    resetModifierTracking();
    if (boot_keyboard_.getProtocol() == HID_BOOT_PROTOCOL) {
      boot_keyboard_.releaseAll();
    } else {
      nkro_keyboard_.releaseAll();
      consumer_control_.releaseAll();
    }
  }
  void pressConsumerControl(Key mapped_key) {
    consumer_control_.press(CONSUMER(mapped_key));
  }
  void releaseConsumerControl(Key mapped_key) {
    consumer_control_.release(CONSUMER(mapped_key));
  }
  void pressSystemControl(Key mapped_key) {
    uint8_t keycode = mapped_key.getKeyCode();
    system_control_.press(keycode);
    last_system_control_keycode_ = keycode;
  }
  void releaseSystemControl(Key mapped_key) {
    uint8_t keycode = mapped_key.getKeyCode();
    if (keycode == last_system_control_keycode_) {
      system_control_.release();
    }
  }

  // pressKey takes a Key, as well as optional boolean 'toggledOn' which defaults
  // to 'true'

  // If toggled_on is not set to false, this routine adds the modifier flags on
  // this key to the bitmask of modifiers that are allowed to be added to the
  // upcoming report. We do this so that when we roll over from a key with a
  // modifier flag to one without it, that modifier flag won't affect the new
  // keypress.

  // If the key we're processing is a modifier key, any modifier flags attached to
  // it are added directly to the report along with the modifier from its keycode
  // byte.
  //
  // (A 'modifier key' is one of the eight modifier keys defined by the HID
  // standard: left and right variants of Control, Shift, Alt, and GUI.)

  // Eventually it calls pressRawKey.

  void pressKey(Key pressed_key, boolean toggled_on = true) __attribute__((noinline)) {
    if (toggled_on) {
      // If two keys are toggled on during the same USB report, we would ideally
      // send an extra USB report to help the host handle each key correctly, but
      // this is problematic.

      // If we simply allow modifiers associated with the second newly-pressed
      // key, it is possible to drop a modifier before the report is sent.
      // Instead, we send modifiers associated with any newly-pressed keys.

      // The downside of this behavior is that in cases where the user presses
      // down keys with conflicting modifiers at the exact same moment, they may
      // get unexpected behavior.

      // If this is the first 'new' keycode being pressed in this cycle, reset the
      // bitmask of modifiers we're willing to attach to USB HID keyboard reports
      if (!last_keycode_toggled_on) {
        modifier_flag_mask = 0;
      }

      // Add any modifiers attached to this key to the bitmask of modifiers we're
      // willing to attach to USB HID keyboard reports
      modifier_flag_mask |= pressed_key.getFlags();

      if (!isModifierKey(pressed_key)) {
        last_keycode_toggled_on = pressed_key.getKeyCode();
      }
    }


    if (isModifierKey(pressed_key)) {
      // If the key is a modifier key with additional modifiers attached to it as
      // flags (as one might when creating a 'Hyper' key or a "Control Alt" key,
      // we assume that all those modifiers are intended to modify other keys
      // pressed while this key is held, so they are never masked out.
      pressModifiers(pressed_key.getFlags());
    } else {
      // If, instead, the modifiers are attached to a 'printable' or non-modifier
      // key, we assume that they're not intended to modify other keys, so we add
      // them to requested_modifier_flags, and only allow them to affect the report if
      // the most recent keypress includes those modifiers.
      requestModifiers(pressed_key.getFlags());
    }

    pressRawKey(pressed_key);
  }

  // pressRawKey takes a Key object and calles KeyboardioHID's ".press" method
  // with its keycode. It does no processing of any flags or modifiers on the key
  void pressRawKey(Key pressed_key) {
    if (boot_keyboard_.getProtocol() == HID_BOOT_PROTOCOL) {
      boot_keyboard_.press(pressed_key.getKeyCode());
      return;
    }

    nkro_keyboard_.press(pressed_key.getKeyCode());
  }

  void releaseRawKey(Key released_key) {
    if (boot_keyboard_.getProtocol() == HID_BOOT_PROTOCOL) {
      boot_keyboard_.release(released_key.getKeyCode());
      return;
    }

    nkro_keyboard_.release(released_key.getKeyCode());
  }

  void releaseKey(Key released_key) {
    // Remove any modifiers attached to this key from the bitmask of modifiers we're
    // willing to attach to USB HID keyboard reports
    modifier_flag_mask &= ~(released_key.getFlags());

    if (!isModifierKey(released_key)) {

      // TODO(anyone): this code is incomplete, but is better than nothing
      // If we're toggling off the most recently toggled on key, clear
      // last_keycode_toggled_on
      if (last_keycode_toggled_on == released_key.getKeyCode()) {
        last_keycode_toggled_on = 0;
      }

      // If the modifiers are attached to a 'printable' or non-modifier
      // key, we need to clean up after the key press which would have requested
      // the modifiers be pressed if the most recent keypress includes those modifiers.
      cancelModifierRequest(released_key.getFlags());
    }

    releaseModifiers(released_key.getFlags());
    releaseRawKey(released_key);
  }

  boolean isModifierKeyActive(Key modifier_key) {
    if (boot_keyboard_.getProtocol() == HID_BOOT_PROTOCOL) {
      return boot_keyboard_.isModifierActive(modifier_key.getKeyCode());
    }

    return nkro_keyboard_.isModifierActive(modifier_key.getKeyCode());
  }

  boolean wasModifierKeyActive(Key modifier_key) {
    if (boot_keyboard_.getProtocol() == HID_BOOT_PROTOCOL) {
      return boot_keyboard_.wasModifierActive(modifier_key.getKeyCode());
    }

    return nkro_keyboard_.wasModifierActive(modifier_key.getKeyCode());
  }

  boolean isAnyModifierKeyActive() {
    if (boot_keyboard_.getProtocol() == HID_BOOT_PROTOCOL) {
      return boot_keyboard_.isAnyModifierActive();
    }

    return nkro_keyboard_.isAnyModifierActive();
  }

  boolean wasAnyModifierKeyActive() {
    if (boot_keyboard_.getProtocol() == HID_BOOT_PROTOCOL) {
      return boot_keyboard_.wasAnyModifierActive();
    }

    return nkro_keyboard_.wasAnyModifierActive();
  }

  uint8_t getKeyboardLEDs() {
    if (boot_keyboard_.getProtocol() == HID_BOOT_PROTOCOL) {
      return boot_keyboard_.getLeds();
    }

    return nkro_keyboard_.getLeds();
  }

  uint8_t getProtocol() {
    return boot_keyboard_.getProtocol();
  }
  void setProtocol(uint8_t protocol) {
    boot_keyboard_.setProtocol(protocol);
  }
  void setDefaultProtocol(uint8_t protocol) {
    boot_keyboard_.setDefaultProtocol(protocol);
  }

 private:
  // To prevent premature release of a System Control key when rolling
  // over from one to another, we record the last System Control
  // keycode that was pressed. It's initialized to zero, which should
  // not be a valid System Control keycode.
  uint8_t last_system_control_keycode_ = 0;

  // modifier_flag_mask is a bitmask of modifiers that we found attached to
  // keys that were newly pressed down during the most recent cycle with any new
  // keypresses.

  // This is used to determine which modifier flags will be allowed to be added to
  // the current keyboard HID report. It gets set during any cycle where one or
  // more new keys is toggled on and presists until the next cycle with a newly
  // detected keypress.

  uint8_t modifier_flag_mask = 0;

  // The functions in this namespace are primarily to solve the problem of
  // rollover from a key with a modifier flag (e.g. `LSHIFT(Key_T)`) to one
  // without (e.g. `Key_H`), which used to result in the mod flag being applied to
  // keys other than the one with the flag. By using `modifier_flag_mask`, we can
  // mask out any modifier flags that aren't attached to modifier keys or keys
  // pressed or held in the most recent cycle, mitigating the rollover problem,
  //  and getting the intended `The` instead of `THe`.

  // requested_modifier_flags is bitmap of the modifiers attached to any non-modifier
  // key found to be pressed during the most recent cycle. For example, it would
  // include modifiers attached to Key_A, but not modifiers attached to
  // Key_LeftControl

  uint8_t requested_modifier_flags = 0;

  // last_keycode_toggled_on is the keycode of the key most recently toggled on
  // for this report.  This is set when a keypress is first detected and cleared
  // after the report is sent. If multiple keys are toggled on during a single
  // cycle, this contains the most recently handled one.

  uint8_t last_keycode_toggled_on = 0;

  void resetModifierTracking(void) {
    last_keycode_toggled_on = 0;
    requested_modifier_flags = 0;
  }

  // isModifierKey takes a Key and returns true if the key is a
  // keyboard key corresponding to a modifier like Control, Alt or Shift
  // TODO(anyone): This function should be lifted to the Kaleidoscope core, somewhere.

  bool isModifierKey(Key key) {
    // If it's not a keyboard key, return false
    if (key.getFlags() & (SYNTHETIC | RESERVED)) return false;

    return (key.getKeyCode() >= HID_KEYBOARD_FIRST_MODIFIER &&
            key.getKeyCode() <= HID_KEYBOARD_LAST_MODIFIER);
  }

  // requestModifiers takes a bitmap of modifiers that might apply
  // to the next USB HID report and adds them to a bitmap of all such modifiers.

  void requestModifiers(byte flags) {
    requested_modifier_flags |= flags;
  }

  // cancelModifierRequest takes a bitmap of modifiers that should no longer apply
  // to the next USB HID report and removes them from the bitmap of all such modifiers.

  void cancelModifierRequest(byte flags) {
    requested_modifier_flags &= ~flags;
  }

  // pressModifiers takes a bitmap of modifier keys that must be included in
  // the upcoming USB HID report and passes them through to KeyboardioHID
  // immediately

  void pressModifiers(byte flags) {
    if (flags & SHIFT_HELD) {
      pressRawKey(Key_LeftShift);
    }
    if (flags & CTRL_HELD) {
      pressRawKey(Key_LeftControl);
    }
    if (flags & LALT_HELD) {
      pressRawKey(Key_LeftAlt);
    }
    if (flags & RALT_HELD) {
      pressRawKey(Key_RightAlt);
    }
    if (flags & GUI_HELD) {
      pressRawKey(Key_LeftGui);
    }
  }

  // releaseModifiers takes a bitmap of modifier keys that must not be included in
  // the upcoming USB HID report and passes them through to KeyboardioHID
  // immediately

  void releaseModifiers(byte flags) {
    if (flags & SHIFT_HELD) {
      releaseRawKey(Key_LeftShift);
    }
    if (flags & CTRL_HELD) {
      releaseRawKey(Key_LeftControl);
    }
    if (flags & LALT_HELD) {
      releaseRawKey(Key_LeftAlt);
    }
    if (flags & RALT_HELD) {
      releaseRawKey(Key_RightAlt);
    }
    if (flags & GUI_HELD) {
      releaseRawKey(Key_LeftGui);
    }
  }

};

}
}
}
}
