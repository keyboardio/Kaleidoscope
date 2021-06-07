/* -*- mode: c++ -*-
 * Kaleidoscope-ModLayer -- Combination modifer/layer shift keys
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

#pragma once

#include <Kaleidoscope-Ranges.h>

#include "kaleidoscope/plugin.h"
#include "kaleidoscope/event_handler_result.h"
#include "kaleidoscope/key_defs.h"
#include "kaleidoscope/KeyEvent.h"

namespace kaleidoscope {
namespace plugin {

// =============================================================================
/// Kaleidoscope plugin for combo modifier/layer shift keys
///
class ModLayer : public Plugin {

 public:
  // ---------------------------------------------------------------------------
  // Event handlers
  EventHandlerResult onNameQuery();
  EventHandlerResult onKeyEvent(KeyEvent &event);
  EventHandlerResult onAddToReport(Key key);

 private:
  // ---------------------------------------------------------------------------
  /// Determines which keys are processed by this plugin
  static bool isModLayerKey(Key key) {
    return (key >= ranges::ML_FIRST && key <= ranges::ML_LAST);
  }

  // ---------------------------------------------------------------------------
  /// Get the encoded layer
  static Key getLayerKey(Key key) {
    uint8_t raw_code = key.getRaw() - ranges::ML_FIRST;
    uint8_t layer = raw_code / 8;
    return ShiftToLayer(layer);
  }

  // ---------------------------------------------------------------------------
  /// Get the encoded modifier
  static Key getModifierKey(Key key) {
    uint8_t raw_code = key.getRaw() - ranges::ML_FIRST;
    uint8_t mod = raw_code % 8;
    return Key(Key_LeftControl.getRaw() + mod);
  }

};

// =============================================================================
/// Generate a ModLayer key (unchecked)
constexpr Key modLayerKey(Key modifier, uint8_t layer) {
  uint8_t mod = modifier.getRaw() - Key_LeftControl.getRaw();
  uint8_t raw_code = mod + (layer * 8);
  return Key(ranges::ML_FIRST + raw_code);
}

} // namespace plugin
} // namespace kaleidoscope

// Shorthand for keymap entry (e.g. `ML(LeftAlt, 3)`)
#define ML(mod, layer) kaleidoscope::plugin::modLayerKey(Key_ ## mod, layer)

extern kaleidoscope::plugin::ModLayer ModLayer;
