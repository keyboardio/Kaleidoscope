/* -*- mode: c++ -*-
 * Basic -- A very basic Kaleidoscope example
 * Copyright (C) 2018  Keyboard.io, Inc.
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

#include "Kaleidoscope.h"


// This example demonstrates how a plugin can gather information about
// the keymap at compile time, e.g. to adapt its behavior, safe resources, ...

// clang-format off
KEYMAPS(
  [0] = KEYMAP_STACKED
  (
   Key_NoKey,    Key_1, Key_1, Key_1, Key_4, Key_5, Key_NoKey,
   Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
   Key_PageUp,   Key_A, Key_S, Key_D, Key_F, Key_G,
   Key_PageDown, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,

   Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
   Key_NoKey,

   Key_skip,  Key_6, Key_7, Key_8,     Key_9,      Key_0,         Key_skip,
   Key_Enter, Key_Y, Key_U, Key_I,     Key_O,      Key_P,         Key_Equals,
              Key_H, Key_J, Key_K,     Key_L,      Key_Semicolon, Key_Quote,
   Key_skip,  Key_N, Key_M, Key_Comma, Key_Period, Key_Slash,     Key_Minus,

   Key_RightShift, Key_RightAlt, Key_Spacebar, Key_RightControl,
   Key_NoKey
  ),
)
// clang-format on

using namespace kaleidoscope::sketch_exploration;  // NOLINT(build/namespaces)

class BPlugin : public kaleidoscope::Plugin {};
class CPlugin : public kaleidoscope::Plugin {};

// A simple plugin that defines just one hook.
//
class APlugin : public kaleidoscope::Plugin {

 public:
  APlugin()
    : has_key_1_{false} {}

  template<typename _Sketch>
  kaleidoscope::EventHandlerResult exploreSketch() {

    // Static keymap exploration

    typedef typename _Sketch::StaticKeymap K;

    // Important: Always make sure to call _Sketch::StaticKeymap's methods
    //            in a constexpr context. This is done by
    //            passing their value to a constexpr temporary variable.

    constexpr uint8_t n_key_1 = K::collect(NumKeysEqual{Key_1});
    static_assert(n_key_1 == 3, "Error determining key count");

    constexpr bool has_key_1 = K::collect(HasKey{Key_1});
    static_assert(has_key_1, "Error querying key existence");
    has_key_1_ = has_key_1;  // Assign the temporary that was computed
    // at compile time.

    constexpr Key max_key = K::collect(MaxKeyRaw{});
    static_assert(max_key.getRaw() > 0, "");

    static_assert(K::getKey(0 /*layer*/, KeyAddr{2, 3}) == Key_D,
                  "Key lookup failed");

    constexpr auto n_layers   = K::nLayers();
    constexpr auto layer_size = K::layerSize();

    // Plugin exploration
    //
    // Use macros ENTRY_TYPE, ENRTY_IS_LAST, PLUGIN_POSITION,
    // PLUGIN_IS_REGISTERED and NUM_OCCURRENCES to retreive information
    // about the plugins that are registered in the sketch.

    typedef typename _Sketch::Plugins P;

    static_assert(std::is_same<ENTRY_TYPE(P, 0), APlugin>::value, "");
    static_assert(std::is_same<ENTRY_TYPE(P, 1), BPlugin>::value, "");

    static_assert(P::size == 3, "");

    static_assert(!ENRTY_IS_LAST(P, 0), "");
    static_assert(!ENRTY_IS_LAST(P, 1), "");
    static_assert(ENRTY_IS_LAST(P, 2), "");

    static_assert(PLUGIN_POSITION(P, APlugin) == 0, "");
    static_assert(PLUGIN_POSITION(P, BPlugin) == 1, "");
    static_assert(PLUGIN_POSITION(P, CPlugin) == -1, "");

    static_assert(PLUGIN_IS_REGISTERED(P, APlugin) == true, "");
    static_assert(PLUGIN_IS_REGISTERED(P, BPlugin) == true, "");
    static_assert(PLUGIN_IS_REGISTERED(P, CPlugin) == false, "");

    static_assert(NUM_OCCURRENCES(P, APlugin) == 2, "");
    static_assert(NUM_OCCURRENCES(P, BPlugin) == 1, "");
    static_assert(NUM_OCCURRENCES(P, CPlugin) == 0, "");

    return kaleidoscope::EventHandlerResult::OK;
  }

 private:
  bool has_key_1_;
};

APlugin a_plugin1, a_plugin2;
BPlugin b_plugin;

KALEIDOSCOPE_INIT_PLUGINS(
  a_plugin1,
  b_plugin,
  a_plugin2)

void setup() {
  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}
