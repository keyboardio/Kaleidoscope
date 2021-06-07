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

#include "kaleidoscope/plugin/ModLayer.h"
#include <Kaleidoscope-FocusSerial.h>

#include "kaleidoscope/keyswitch_state.h"
#include "kaleidoscope/layers.h"
#include "kaleidoscope/Runtime.h"

namespace kaleidoscope {
namespace plugin {

// =============================================================================
// Event handler hook functions

// -----------------------------------------------------------------------------
EventHandlerResult ModLayer::onNameQuery() {
  return ::Focus.sendName(F("ModLayer"));
}

// -----------------------------------------------------------------------------
EventHandlerResult ModLayer::onKeyEvent(KeyEvent &event) {
  if (!isModLayerKey(event.key))
    return EventHandlerResult::OK;

  Layer.handleLayerKeyEvent(
    KeyEvent(event.addr, event.state, getLayerKey(event.key)));

  return EventHandlerResult::OK;
}

// -----------------------------------------------------------------------------
EventHandlerResult ModLayer::onAddToReport(Key key) {
  if (!isModLayerKey(key))
    return EventHandlerResult::OK;

  Runtime.hid().keyboard().pressKey(getModifierKey(key));

  return EventHandlerResult::OK;
}

} // namespace plugin
} // namespace kaleidoscope

kaleidoscope::plugin::ModLayer ModLayer;
