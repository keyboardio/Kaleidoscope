/* -*- mode: c++ -*-
 * Kaleidoscope-LeaderPrefix -- Prefix arg for Leader plugin
 * Copyright (C) 2021  Keyboard.io, Inc
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

#include <Kaleidoscope.h>
#include <Kaleidoscope-Leader.h>
#include <Kaleidoscope-MacroSupport.h>

#include <Kaleidoscope-Ranges.h>
#include "kaleidoscope/KeyEventTracker.h"
#include "kaleidoscope/LiveKeys.h"
#include "kaleidoscope/plugin.h"

// clang-format off
KEYMAPS(
  [0] = KEYMAP_STACKED
  (Key_NoKey,    Key_1, Key_2, Key_3, Key_4, Key_5, Key_NoKey,
   Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
   Key_PageUp,   Key_A, Key_S, Key_D, Key_F, Key_G,
   Key_PageDown, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,

   Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
   LEAD(0),

   Key_skip,  Key_6, Key_7, Key_8,     Key_9,      Key_0,         Key_skip,
   Key_Enter, Key_Y, Key_U, Key_I,     Key_O,      Key_P,         Key_Equals,
              Key_H, Key_J, Key_K,     Key_L,      Key_Semicolon, Key_Quote,
   Key_skip,  Key_N, Key_M, Key_Comma, Key_Period, Key_Slash,     Key_Minus,

   Key_RightShift, Key_RightAlt, Key_Spacebar, Key_RightControl,
   LEAD(0)),
)
// clang-format on

namespace kaleidoscope {
namespace plugin {

// =============================================================================
/// Plugin to supply a numeric prefix argument to Leader key functions
///
/// This plugin lets the user type a numeric prefix after a Leader key is
/// pressed, but before the rest of the Leader sequence is begun, storing the
/// "prefix argument" and making it available to functions called from the
/// leader dictionary.  LeaderPrefix allows us to define keys other than the
/// ones on the number row to be interpreted as the "digit" keys, because
/// whatever we use will need to be accessed without a layer change.
class LeaderPrefix : public Plugin {
 public:
  // We need to define `onKeyswitchEvent()` instead of `onKeyEvent()` because we
  // need to intercept events before Leader sees them, and the Leader plugin
  // uses the former.
  EventHandlerResult onKeyswitchEvent(KeyEvent &event) {
    // Every `onKeyswitchEvent()` function should begin with this to prevent
    // re-processing events that it has already seen.
    if (event_tracker_.shouldIgnore(event))
      return EventHandlerResult::OK;

    // `Active` means that we're actively building the prefix argument.  If the
    // plugin is not active, we're looking for a Leader key toggling on.
    if (!active_) {
      if (keyToggledOn(event.state) && isLeaderKey(event.key)) {
        // A Leader key toggled on, so we set our state to "active", and set the
        // arg value to zero.
        active_     = true;
        leader_arg_ = 0;
      }
      // Whether or not the plugin just became active, there's nothing more to
      // do for this event.
      return EventHandlerResult::OK;
    }

    // The plugin is "active", so we're looking for a "digit" key that just
    // toggled on.
    if (keyToggledOn(event.state)) {
      // We search our array of digit keys to find one that matches the event.
      // These "digit keys" are defined by their `KeyAddr` because they're
      // probably independent of keymap and layer, and because a `KeyAddr` only
      // takes one byte, whereas a `Key` takes two.
      for (uint8_t i{0}; i < 10; ++i) {
        if (digit_addrs_[i] == event.addr) {
          // We found a match, which means that one of our "digit keys" toggled
          // on.  If this happens more than once, the user is typing a number
          // with multiple digits, so we multiply the current value by ten
          // before adding the new digit to the total.
          leader_arg_ *= 10;
          leader_arg_ += i;
          // Next, we mask the key that was just pressed, so that nothing will
          // happen when it is released.
          live_keys.mask(event.addr);
          // We return `ABORT` so that no other plugins (i.e. Leader) will see
          // this keypress event.
          return EventHandlerResult::ABORT;
        }
      }
    }
    // No match was found, so the key that toggled on was not one of our "digit
    // keys".  Presumably, this is the first key in the Leader sequence that is
    // being typed.  We leave the prefix argument at its current value so that
    // it will still be set when the sequence is finished, and allow the event
    // to pass through to the next plugin (i.e. Leader).
    active_ = false;
    return EventHandlerResult::OK;
  }

  uint16_t arg() const {
    return leader_arg_;
  }

 private:
  // The "digit keys": these are the keys on the number row of the Model01.
  KeyAddr digit_addrs_[10] = {
    KeyAddr(0, 14),
    KeyAddr(0, 1),
    KeyAddr(0, 2),
    KeyAddr(0, 3),
    KeyAddr(0, 4),
    KeyAddr(0, 5),
    KeyAddr(0, 10),
    KeyAddr(0, 11),
    KeyAddr(0, 12),
    KeyAddr(0, 13),
  };

  // This event tracker is necessary to prevent re-processing events.  Any
  // plugin that defines `onKeyswitchEvent()` should use one.
  KeyEventTracker event_tracker_;

  // The current state of the plugin.  It determines whether we're looking for a
  // Leader keypress or building a prefix argument.
  bool active_{false};

  // The prefix argument itself.
  uint16_t leader_arg_{0};

  // Leader should probably provide this test, but since it doesn't, we add it
  // here to determine if a key is a Leader key.
  bool isLeaderKey(Key key) {
    return (key >= ranges::LEAD_FIRST && key <= ranges::LEAD_LAST);
  }
};

}  // namespace plugin
}  // namespace kaleidoscope

// This creates our plugin object.
kaleidoscope::plugin::LeaderPrefix LeaderPrefix;

auto &serial_port = Kaleidoscope.serialPort();

static void leaderTestX(uint8_t seq_index) {
  serial_port.println(F("leaderTestX"));
}

static void leaderTestXX(uint8_t seq_index) {
  serial_port.println(F("leaderTestXX"));
}

// This demonstrates how to use the prefix argument in a Leader function.  In
// this case, our function just types as many `x` characters as specified by the
// prefix arg.
void leaderTestPrefix(uint8_t seq_index) {
  // Read the prefix argument into a temporary variable:
  uint8_t prefix_arg = LeaderPrefix.arg();
  // Use a Macros helper function to tap the `X` key repeatedly.
  while (prefix_arg-- > 0)
    MacroSupport.tap(Key_X);
}

static const kaleidoscope::plugin::Leader::dictionary_t leader_dictionary[] PROGMEM =
  LEADER_DICT({LEADER_SEQ(LEAD(0), Key_X), leaderTestX},
              {LEADER_SEQ(LEAD(0), Key_X, Key_X), leaderTestXX},
              {LEADER_SEQ(LEAD(0), Key_Z), leaderTestPrefix});

// The order matters here; LeaderPrefix won't work unless it precedes Leader in
// this list.  If there are other plugins in the list, these two should ideally
// be next to each other, but that's not necessary.
KALEIDOSCOPE_INIT_PLUGINS(LeaderPrefix, Leader);

void setup() {
  Kaleidoscope.setup();

  Leader.dictionary = leader_dictionary;
}

void loop() {
  Kaleidoscope.loop();
}
