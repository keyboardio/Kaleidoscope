/* Kaleidoscope-LEDMode - LED modes for Kaleidoscope
 * Copyright (C) 2017-2018  Keyboard.io, Inc.
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

#include "kaleidoscope/KeyAddr.h"                        // for KeyAddr
#include "kaleidoscope/event_handler_result.h"           // for EventHandlerResult, EventHandlerResult...
#include "kaleidoscope/plugin.h"                         // for Plugin
#include "kaleidoscope/plugin/AccessTransientLEDMode.h"  // IWYU pragma: keep
#include "kaleidoscope/plugin/LEDModeInterface.h"        // for LEDModeInterface

namespace kaleidoscope {

namespace internal {
// Forward declaration
class LEDModeManager;

}  // namespace internal

namespace plugin {

/** Base class for LED modes.
*
* LED modes are a special kind of plugin, they are in charge of updating LED
* colors, setting a theme. While it is possible to have other plugins
* override the mode's colors, the LED mode is the baseline.
*
* Most of its functionality is called via @ref LEDControl, with only a few
* public methods.
*
* A LED mode **must** implement at least one of @ref onActivate or @ref
* update, and possibly @ref refreshAt too.
*/
class LEDMode : public kaleidoscope::Plugin,
                public LEDModeInterface {
  friend class LEDControl;
  friend class kaleidoscope::internal::LEDModeManager;

 protected:
  // These methods should only be called by LEDControl.

  /** One-time setup, called at keyboard boot.
   *
   * Any hooks that need registering, any one-time setup that needs to be
   * performed, shall be done here. This is purely for preparation purposes, the
   * LEDs should not be touched yet at this time.
   */
  virtual void setup(void) {}

  /** Function to call whenever the mode is activated.
   *
   * Like @ref setup, this method need not touch LEDs, @ref update will be
   * called right after it. The purpose of this callback is to allow a plugin to
   * do some preparation whenever it is activated, instead of only on boot, or
   * always at each cycle.
   *
   * However, unlike @ref setup, this method can change LED colors, if so
   * desired. Either to provide an initial state, or a static color set. In the
   * latter case, consider implementing @ref refreshAt too, because other
   * plugins may override some of the colors set at activation time, and @ref
   * refreshAt can be used to restore them when needed.
   *
   * Before the callback runs, LEDs will be blanked.
   */
  virtual void onActivate(void) {}

  /** Update the LEDs once per cycle.
   *
   * Usually the brains of the plugin, which updates the LEDs each cycle. It is
   * called after the matrix has been scanned, once per cycle.
   */
  virtual void update(void) {}

  /** Refresh the color of a given key.
   *
   * If we have another plugin that overrides colors set by the active LED mode
   * (either at @onActivate time, or via @ref update), if that plugin wants to
   * restore whatever color the mode would set the key color to, this is the
   * method it will call.
   *
   * @param key_addr is the matrix coordinate of the key to refresh the color of.
   */
  virtual void refreshAt(KeyAddr key_addr) {}

 public:
  /** Plugin initialization.
   *
   * Called via `Runtime.use()`, registers the LED mode, and does the
   * necessary initialization steps. Calls @ref setup at the end.
   */
  kaleidoscope::EventHandlerResult onSetup() {
    setup();

    return EventHandlerResult::OK;
  }
};

}  // namespace plugin
}  // namespace kaleidoscope
