#pragma once
// switch debouncing and status

#include <Arduino.h>

#define INJECTED    B10000000
#define IS_PRESSED  B00000010
#define WAS_PRESSED B00000001

/* keyIsPressed(): This is true if the key is pressed during this scan cycle.
 * This will be true for several consecutive cycles even for a single tap of
 *   the key.
 * Use this for events which should fire every scan cycle the key is held.
 * If you want an event which fires only once when a key is pressed, use
 *   keyToggledOn() or keyToggledOff() (defined below).
 */
#define keyIsPressed(keyState)  ((keyState) & IS_PRESSED)

/* keyWasPressed(): This is true if the key was pressed during the previous
     scan cycle, regardless of whether it is pressed or not in this scan cycle.
 */
#define keyWasPressed(keyState) ((keyState) & WAS_PRESSED)

/* keyToggledOn(): This is true if the key is newly pressed during this scan
 *   cycle, i.e. was not pressed in the previous scan cycle but is now.
 * Use this for events which should fire exactly once per keypress, on a
 *   "key-down" event.
 */
#define keyToggledOn(keyState)  (keyIsPressed(keyState) && ! keyWasPressed(keyState))

/* keyToggledOff(): This is true if the key is newly not-pressed during this
 *   scan cycle, i.e. is not pressed now but was in the previous scan cycle.
 * Use this for events which should fire exactly once per keypress, on a
 *   "key-up" event.
 */
#define keyToggledOff(keyState) (keyWasPressed(keyState) && ! keyIsPressed(keyState))


// Deprecated - Remove once the core has transitioned

inline uint8_t __attribute__((deprecated("Use keyWasPressed instead"))) key_was_pressed(uint8_t keyState) {
  return keyWasPressed(keyState);
}
inline uint8_t __attribute__((deprecated("Use keyIsPressed instead"))) key_is_pressed(uint8_t keyState) {
  return keyIsPressed(keyState);
}
inline uint8_t __attribute__((deprecated("Use keyToggledOn instead"))) key_toggled_on(uint8_t keyState) {
  return keyToggledOn(keyState);
}
inline uint8_t __attribute__((deprecated("Use keyToggledOff instead"))) key_toggled_off(uint8_t keyState) {
  return keyToggledOff(keyState);
}
