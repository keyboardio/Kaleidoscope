This is an example that exercises, I think, all of the different core behaviors of Kaleidoscope's TapDance keys:

```
void tapDanceAction(uint8_t tap_dance_index, 
                    KeyAddr key_addr, 
                    uint8_t tap_count,
                    kaleidoscope::plugin::TapDance::ActionType tap_dance_action) {
  switch (tap_dance_index) {
  case 0:
        // kaleidoscope::plugin::TapDance::Timeout, if the tap-dance key has been released when its timeout expires.
        if (tap_dance_action == kaleidoscope::plugin::TapDance::ActionType::Timeout) {
          tapDanceActionKeys(tap_count, tap_dance_action, Key_Q, Key_W, Key_E);

        }
        // kaleidoscope::plugin::TapDance::Hold, if the tap-dance key is still being held when its timeout expires.
        else if ( tap_dance_action == kaleidoscope::plugin::TapDance::ActionType::Hold) {
          tapDanceActionKeys(tap_count, tap_dance_action, Key_A, Key_S, Key_D);
        }
        // kaleidoscope::plugin::TapDance::Interrupt, if another key is pressed before the tap-dance key’s timeout expires.
        else if (tap_dance_action == kaleidoscope::plugin::TapDance::ActionType::Interrupt) {
          tapDanceActionKeys(tap_count, tap_dance_action, Key_Z, Key_X, Key_C);
        }
  }
}
```

TapDance here counts taps + what's going on when it's done counting:
Was the tapdance key released before the timer expired, Was it still being held down when the timer expired, was another key hit before you released the tapdance key. So three quick taps will get you an 'E', A hold will get you an "A", A tap and hold will get you an "S", two taps and a hold will get you a "D"
Hold until some other key is hit will get you a Z, etc.
The other things you'd need to do are "#include "Kaleidoscope-TapDance.h"" at the top of your sketch, add "TapDance" to the list of "KALEIDOSCOPE_INIT_PLUGINS", and stick a "TD(0)" somewhere in your keymap.
