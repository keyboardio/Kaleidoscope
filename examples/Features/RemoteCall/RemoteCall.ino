
#include "Kaleidoscope.h"
#include "Kaleidoscope-LEDControl.h"

KEYMAPS(
  [0] = KEYMAP_STACKED
        (___,          Key_1, Key_2, Key_3, Key_4, Key_5, ___,
         Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
         Key_PageUp,   Key_A, Key_S, Key_D, Key_F, Key_G,
         Key_PageDown, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,
         Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
         ___,

         ___,  Key_6, Key_7, Key_8,     Key_9,         Key_0,         ___,
         Key_Enter,     Key_Y, Key_U, Key_I,     Key_O,         Key_P,         Key_Equals,
         Key_H, Key_J, Key_K,     Key_L,         Key_Semicolon, Key_Quote,
         Key_RightAlt,  Key_N, Key_M, Key_Comma, Key_Period,    Key_Slash,     Key_Minus,
         Key_RightShift, Key_LeftAlt, Key_Spacebar, Key_RightControl,
         ___)
)


namespace kaleidoscope {
namespace plugin {
class MyPlugin_ : public kaleidoscope::Plugin {

 public:
  void doSomething() { /* do something */ }

  EventHandlerResult onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t key_state) {
    return (EventHandlerResult)(a_ + b_);
  }

  int a_;
  int b_;
};

MyPlugin_ MyPlugin;
}
}


KALEIDOSCOPE_INIT_PLUGINS(
  kaleidoscope::plugin::MyPlugin
)

void setup() {
  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}

void updateMyPlugin() {
  kaleidoscope::plugin::MyPlugin.doSomething();
}

void someFunc() {}
void someFunc1() {}
void someFunc2() {}

uint8_t r = 2, g = 3, b = 4, y = 5;

KALEIDOSCOPE_REMOTE_CALL(

  KRC_ROOT_PACKAGE(Package1,
                   KRC_FUNCTION(setAllLEDs,
                                KRC_ARGUMENTS(
                                  (uint8_t, red, KRC_DESCRIPTION("Red portion.")),
                                  (uint8_t, green, KRC_DESCRIPTION("Green portion.")),
                                  (uint8_t, blue, KRC_DESCRIPTION("Blue portion."))
                                )
                                KRC_RESULTS(
                                  (uint8_t, res, KRC_DESCRIPTION("Results description."))
                                )
                                KRC_FUNCTION_BODY(
                                  auto args = KRC_ACCESS_ARGS();

                                  using namespace kaleidoscope::plugin;
                                  LEDControl::set_all_leds_to(args->red, args->green, args->blue);
                                  LEDControl::syncLeds();

                                  auto results = KRC_ACCESS_RESULTS();

                                  results->res = 42;
                                )
                               )
                  )

  KRC_ROOT_PACKAGE(Level1,
                   KRC_DESCRIPTION("Level1 description")
                   // Default update function of package Level1
                   //
                   KRC_UPDATE_THROUGH_GLOBAL_FUNCTION(::someFunc2)

                   // A package that exports member variables of a plugin. The update
                   // functions are expected to be called after the member variable has been
                   // changed in order for the changes to take effect.
                   //
                   // A plugin package is a package that is build around a plugin object.
                   // The object is used whenever one of the plugin's members is
                   // accessed through KRC_MEMBER.
                   //
                   KRC_PLUGIN(MyPlugin, kaleidoscope::plugin::MyPlugin,
                              KRC_DESCRIPTION("MyPlugin description")
                              KRC_MEMBER(a, a_,
                                  KRC_DESCRIPTION("a issetetdtwdtwdtwdtwset ...")
                                  // No update function. Will use ::someFunc2 specified by package
                                  // Level1.
                                        )
                              KRC_MEMBER(b, b_,
                                  KRC_DESCRIPTION("b is ...")
                                  KRC_UPDATE_THROUGH_GLOBAL_FUNCTION(::updateMyPlugin)
                                        )
                             )

                   // Packages can be nested. Here it is Level1::Level2.
                   //
                   KRC_PACKAGE(Level2,
                               KRC_DESCRIPTION("Level2 description.")
                               KRC_GLOBAL(red, ::r,
                                   KRC_DESCRIPTION("Red color.")
                                   // No update function specified. Will use ::someFunc2 from
                                   // next higher package level Level1.
                                         )
                              )

                   // Packages can be continued at any point.
                   //
                   KRC_PACKAGE(Level2,

                               // Default update function of package Level1::Level2
                               //
                               KRC_UPDATE_THROUGH_GLOBAL_FUNCTION(::someFunc)
                               KRC_GLOBAL(green, ::g,
                                   KRC_DESCRIPTION("Green color")
                                   KRC_UPDATE_THROUGH_GLOBAL_FUNCTION(::someFunc)
                                         )
                               KRC_GLOBAL(blue, ::b,
                                   KRC_DESCRIPTION("Blue color")
                                   // No update function specified. Will use ::someFunc from
                                   // next higher package level Level1::Level2.
                                         )
                               KRC_GLOBAL(yellow, ::y,
                                   KRC_DESCRIPTION("Yellow color")
                                   KRC_NO_UPDATE // Suppresses update function inheritance.
                                   // yellow does not require an update function.
                                         )
                              )
                  )
)
