#pragma once
#include <Arduino.h>
#include "KeyboardioHID.h"

#include KALEIDOSCOPE_HARDWARE_H
#include "key_defs.h"
#include "keyswitch_state.h"

extern const Key keymaps[][ROWS][COLS];


// Code can use this macro on injected key events to signal that
// the event isn't tied to a specific physical keyswitch
#define UNKNOWN_KEYSWITCH_LOCATION 255,255

// sending events to the computer
/* The event handling starts with the Scanner calling handle_key_event() for
 * every key in the matrix, and it is the task of this method to figure out what
 * to do, it is the main entry point.
 *
 * This function will iterate through an array of handler functions, and stop as
 * soon as one of them signals that the event has been handled. To make it
 * possible to inject synthetic events, one can call handle_key_event from
 * within a custom handler (making the event handling recursive), with a
 * different keycode.
 *
 * This is useful for example for one-shot modifiers, where we would like to
 * temporarily disable the one-shot functionality, and have them work as a
 * normal modifier instead. In this case, the keymap would contain a key with
 * OSM flags set, and the event handler would remove the OSM flags, and let the
 * system handle the key as it would have, without the OSM flags. So we simply
 * clear the flags, and call handle_key_event again, with the modifier keycode
 * as the first argument. This way, we could insert an event, and have the whole
 * chain re-process it, instead of registering the keycode ourselves with HID
 * ourselves. Injecting allows any and all custom handlers to have a chance,
 * too.
 *
 * For this reason, the handle_key_event receives four arguments: the mapped key
 * (or Key_NoKey if we do not want to override what is in the keymap), the row
 * and column of the key, so we can look up the code for it, and the current and
 * previous state of the key, so we can determine what the event is. The
 * currentState may be flagged INJECTED, which signals that the event was
 * injected, and is not a direct result of a keypress, coming from the scanner.
 */
void handle_key_event(Key mappedKey, byte row, byte col, uint8_t keyState);

// Internal use
void press_key(Key mappedKey);
void release_key(Key mappedKey);
