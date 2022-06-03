/* -*- mode: c++ -*-
 * Kaleidoscope-Syster -- Symbolic input system
 * Copyright (C) 2017-2021  Keyboard.io, Inc
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

#include "kaleidoscope/plugin/Syster.h"

#include <Arduino.h>                   // for F, __FlashStringHelper
#include <Kaleidoscope-FocusSerial.h>  // for Focus, FocusSerial
#include <stdint.h>                    // for int8_t

#include "kaleidoscope/KeyAddr.h"               // for KeyAddr
#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/Runtime.h"               // for Runtime, Runtime_
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/key_defs.h"              // for Key, Key_Backspace, Key_1, Key_A, Key_0
#include "kaleidoscope/keyswitch_state.h"       // for INJECTED, WAS_PRESSED, keyToggledOn, IS_P...

namespace kaleidoscope {
namespace plugin {

// --- api ---
void Syster::reset() {
  symbol_pos_ = 0;
  symbol_[0]  = 0;
  is_active_  = false;
}

bool Syster::is_active() {
  return is_active_;
}

// --- hooks ---
EventHandlerResult Syster::onNameQuery() {
  return ::Focus.sendName(F("Syster"));
}

EventHandlerResult Syster::onKeyEvent(KeyEvent &event) {
  if (!is_active_) {
    // If Syster isn't actively matching an input sequence, we're only looking
    // for the special Syster `Key` value; anything else gets passed through
    // immediately.
    if (event.key != Key_Syster)
      return EventHandlerResult::OK;

    // It's a Syster Key; activate the plugin as soon as it toggles on, so we
    // process rollover correctly.
    if (keyToggledOn(event.state)) {
      is_active_ = true;
      systerAction(StartAction, nullptr);
    }
    return EventHandlerResult::EVENT_CONSUMED;
  }

  // Always ignore events marked as artificially injected (it might actually be
  // better to drop this, but it's not really clear).
  if (keyIsInjected(event.state))
    return EventHandlerResult::OK;

  // If a Syster key gets pressed while we're reading an input sequence, ignore
  // it. This could be turned into a "reset" where we erase the abandoned input.
  if (event.key == Key_Syster) {
    return EventHandlerResult::EVENT_CONSUMED;
  }

  // If the user presses a backspace key while at the beginning of the input
  // string, suppress it to prevent erasing past the start of the sequence.
  // Again, this could be changed to do a reset.
  if (event.key == Key_Backspace && symbol_pos_ == 0) {
    return EventHandlerResult::ABORT;
  }

  // We only pay attention to key press events while parsing input.  If the user
  // holds a key down long enough to trigger repeating characters in the OS,
  // we'll end up erasing fewer characters than we should.  This could be
  // addressed by immediately sending the corresponding release event, but
  // that's probably too much trouble to be worthwhile.
  if (keyToggledOn(event.state)) {
    // Pressing the spacebar ends the input sequence.
    if (event.key == Key_Spacebar) {
      // First, we erase all the typed characters in the symbol sequence.
      eraseChars(symbol_pos_);

      // Next, we call the user-defined end action.
      systerAction(EndAction, nullptr);

      // Then we null-terminate the `symbol_` string, and call the user-defined
      // symbol action.
      symbol_[symbol_pos_] = 0;
      systerAction(SymbolAction, symbol_);

      // Finally, we're done, so we reset, deactivating Syster until the next
      // press of a Syster key.
      reset();

      // Returning ABORT here stops the spacebar from activating. Alternatively,
      // we could remove this return statement, and instead allow the spacebar
      // to take effect, resulting in a space in the output, which would follow
      // any symbols produced by the symbol action.
      return EventHandlerResult::ABORT;

    } else if (event.key == Key_Backspace) {
      // If the user erases any typos, we keep the Syster symbol string in sync
      // with what's on the screen.  Again, this doesn't account for a key held
      // long enough to trigger repeat in the OS.
      if (symbol_pos_ > 0)
        --symbol_pos_;

    } else {
      // An ordinary keypress, with Syster active.  We add its corresponding
      // character to the symbol string.
      const char c = keyToChar(event.key);
      if (c)
        symbol_[symbol_pos_++] = c;
    }
  }

  return EventHandlerResult::OK;
}

}  // namespace plugin

void eraseChars(int8_t n) {
  // For the `event.addr`, we could track the address of the Syster key, but it
  // might be on a layer that's no longer active by the time this gets
  // called. We could search the active keymap for an existing `Key_Backspace`,
  // but there might not be one. We could hijack the first idle key we find in
  // the keymap, but we probably don't need to. Even if some other plugin reacts
  // by inserting another event between these two, it's very unlikely that will
  // cause a user-visible error.
  auto event = KeyEvent(KeyAddr::none(), INJECTED, Key_Backspace);
  while (n > 0) {
    event.state = IS_PRESSED | INJECTED;
    Runtime.handleKeyEvent(event);
    event.state = WAS_PRESSED | INJECTED;
    Runtime.handleKeyEvent(event);
    --n;
  }
  Runtime.handleKeyEvent(event);
  // Change the event from a press to a release, but use the same id. This does
  // come with a small risk that another plugin will be tracking events, but
  // also ignoring event ids that it has seen before, but it's more likely to
  // avoid an error than to cause one.
  event.state = WAS_PRESSED | INJECTED;
  Runtime.handleKeyEvent(event);
}

}  // namespace kaleidoscope


__attribute__((weak)) const char keyToChar(Key key) {
  if (key.getFlags() != 0)
    return 0;

  switch (key.getKeyCode()) {
  case Key_A.getKeyCode()... Key_Z.getKeyCode():
    return 'a' + (key.getKeyCode() - Key_A.getKeyCode());
  case Key_1.getKeyCode()... Key_9.getKeyCode():
    return '1' + (key.getKeyCode() - Key_1.getKeyCode());
  case Key_0.getKeyCode():
    return '0';
  }

  return 0;
}

__attribute__((weak)) void systerAction(kaleidoscope::plugin::Syster::action_t action, const char *symbol) {
}

kaleidoscope::plugin::Syster Syster;
