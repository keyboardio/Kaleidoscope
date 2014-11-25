// Do not remove the include below
#include "ArduinoKeyboard.h"
// Copyright 2013 Jesse Vincent <jesse@fsck.com>
// All Rights Reserved. (To be licensed under an opensource license
// before the release of the keyboard.io model 01


/**
 *  TODO:

    add mouse inertia
    add series-of-character macros
    add series of keystroke macros
    use a lower-level USB API
 *
**/


#include "ArduinoKeyboard.h"
#include <EEPROM.h>  // Don't need this for CLI compilation, but do need it in the IDE
#include "debouncing.h"

void setup_matrix()
{
  //blank out the matrix.
  for (byte col = 0; col < COLS; col++) {
    for (byte row = 0; row < ROWS; row++) {
      matrixState[row][col] = 0;
    }
  }
}
void reset_matrix()
{
  for (byte col = 0; col < COLS; col++) {
    for (byte row = 0; row < ROWS; row++) {
      matrixState[row][col] <<= 1;
    }
  }
}



void set_keymap(Key keymapEntry, byte matrixStateEntry) {
  if (keymapEntry.flags & SWITCH_TO_KEYMAP) {
    // this logic sucks. there is a better way TODO this
    if (! (keymapEntry.flags ^ ( MOMENTARY | SWITCH_TO_KEYMAP))) {
      if (key_is_pressed(matrixStateEntry)) {
        if ( keymapEntry.rawKey == KEYMAP_NEXT) {
          active_keymap++;
        } else if ( keymapEntry.rawKey == KEYMAP_PREVIOUS) {
          active_keymap--;
        } else {
          active_keymap = keymapEntry.rawKey;
        }
      }
    } else if (! (keymapEntry.flags ^ (  SWITCH_TO_KEYMAP))) {
      // switch keymap and stay there
      if (key_toggled_on(matrixStateEntry)) {
        active_keymap = primary_keymap = keymapEntry.rawKey;
        save_primary_keymap(primary_keymap);
#ifdef DEBUG_SERIAL
        Serial.print("keymap is now:");
        Serial.print(active_keymap);
#endif
      }
    }
  }
}

void scan_matrix()
{
  //scan the Keyboard matrix looking for connections
  for (byte row = 0; row < ROWS; row++) {
    digitalWrite(rowPins[row], LOW);
    for (byte col = 0; col < COLS; col++) {
      //If we see an electrical connection on I->J,

      if (digitalRead(colPins[col])) {
        matrixState[row][col] |= 0; 
      } else {
        matrixState[row][col] |= 1; 
      }
      // while we're inspecting the electrical matrix, we look
      // to see if the Key being held is a firmware level
      // metakey, so we can act on it, lest we only discover
      // that we should be looking at a seconary Keymap halfway
      // through the matrix scan


      set_keymap(keymaps[active_keymap][row][col], matrixState[row][col]);

    }
    digitalWrite(rowPins[row], HIGH);
  }
}

// Command mode
//


void command_reboot_bootloader() {
    Keyboard.println("Rebooting to bootloader");
    Serial.end();


    // Set the magic bits to get a Caterina-based device
    // to reboot into the bootloader and stay there, rather
    // than run move onward 
    //
    // These values are the same as those defined in
    // Caterina.c
    
    uint16_t bootKey = 0x7777;
    uint16_t *const bootKeyPtr = (uint16_t *)0x0800;

    // Stash the magic key
    *bootKeyPtr = bootKey;

    // Set a watchdog timer
    wdt_enable(WDTO_120MS);

    while(1) {} // This infinite loop ensures nothing else
                // happens before the watchdog reboots us
}

void command_plugh() {
     commandMode = !commandMode;
    if (commandMode) {
        Keyboard.println("");
        Keyboard.println("Entering command mode!");

    } else {
        Keyboard.println("Leaving command mode!");
        Keyboard.println("");
        }
  }

void setup_command_mode() {
    commandBufferSize=0;
    commandMode = false;
    commandPromptPrinted = false;
}
boolean command_ends_in_return() {
  if (
  commandBuffer[commandBufferSize-1] == KEY_ENTER ||
  commandBuffer[commandBufferSize-1] == KEY_RETURN ) {
  return true;
  } else {
  return false;
  }
}

boolean is_command_buffer(byte* myCommand) {
    if (!command_ends_in_return()) {
    return false;
    }
    int i = 0;
    do {
        if (commandBuffer[i] != myCommand[i]) {
            return false;
        }
    } while (myCommand[++i] != NULL);
    return true;
}


void process_command_buffer(){
  if (!command_ends_in_return()) {
  return;
  }



  // This is the only command we might want to execute when 
  // we're not in command mode, as it's the only way to toggle 
  // command mode on
  static byte cmd_plugh[] = {KEY_P,KEY_L,KEY_U,KEY_G,KEY_H,NULL};
  if (is_command_buffer(cmd_plugh)) {
     command_plugh();
  }

  // if we've toggled command mode off, get out of here.
  if (!commandMode) {
    commandBufferSize=0;
    return; 
  }
  
    // Handle all the other commands here
    static byte cmd_reboot_bootloader[] = { KEY_B, KEY_O, KEY_O, KEY_T, KEY_L, KEY_O, KEY_A, KEY_D, KEY_E, KEY_R, NULL};
    static byte cmd_version[] = { KEY_V, KEY_E, KEY_R, KEY_S, KEY_I, KEY_O, KEY_N, NULL};

    if(is_command_buffer(cmd_reboot_bootloader)) {
        command_reboot_bootloader();
    } else if (is_command_buffer(cmd_version)) {
        Keyboard.println("");
        Keyboard.print("This is Keyboardio Firmware ");
        Keyboard.println(VERSION);
    }


   if (!commandPromptPrinted ){
    Keyboard.print(">>> ");
    commandPromptPrinted = true;
    commandBufferSize=0;
  }
}


void setup()
{
   wdt_disable();

  //usbMaxPower = 100;
  Keyboard.begin();
  Mouse.begin();
  setup_command_mode();
  setup_matrix();
  setup_pins();
  Serial.begin(9600);
  primary_keymap = load_primary_keymap();
}

String myApp;

void loop()
{
  if(Serial.available()) {
   myApp = Serial.readString(); 
   myApp.trim();
  }
  active_keymap = primary_keymap;
  scan_matrix();
  send_key_events();
  reset_matrix();
  reset_key_report();
}

















void save_primary_keymap(byte keymap)
{
  EEPROM.write(EEPROM_KEYMAP_LOCATION, keymap);
}

byte load_primary_keymap()
{
  byte keymap =  EEPROM.read(EEPROM_KEYMAP_LOCATION);
  if (keymap >= KEYMAPS ) {
    return 0; // undefined positions get saved as 255
  }
  return keymap;
}




// Debugging Reporting
//

void report_matrix()
{
#ifdef DEBUG_SERIAL
  if (reporting_counter++ % 100 == 0 ) {
    for (byte row = 0; row < ROWS; row++) {
      for (byte col = 0; col < COLS; col++) {
        Serial.print(matrixState[row][col], HEX);
        Serial.print(", ");

      }
      Serial.println("");
    }
    Serial.println("");
  }
#endif
}

void report(byte row, byte col, boolean value)
{
#ifdef DEBUG_SERIAL
  Serial.print("Detected a change on ");
  Serial.print(col);
  Serial.print(" ");
  Serial.print(row);
  Serial.print(" to ");
  Serial.print(value);
  Serial.println(".");
#endif
}

//
// Key Reports
//

void release_keys_not_being_pressed()
{
  // we use charsReportedLastTime to figure out what we might
  // not be holding anymore and can now release. this is
  // destructive to charsReportedLastTime

  for (byte i = 0; i < KEYS_HELD_BUFFER; i++) {
    // for each key we were holding as of the end of the last cycle
    // see if we're still holding it
    // if we're not, call an explicit Release

    if (charsReportedLastTime[i] != 0x00) {
      // if there _was_ a character in this slot, go check the
      // currently held characters
      for (byte j = 0; j < KEYS_HELD_BUFFER; j++) {
        if (charsReportedLastTime[i] == charsBeingReported[j]) {
          // if's still held, we don't need to do anything.
          charsReportedLastTime[i] = 0x00;
          break;
        }
      }
    }
  }
  for (byte i = 0; i < KEYS_HELD_BUFFER; i++) {
    if (charsReportedLastTime[i] != 0x00) {
      Keyboard.release(charsReportedLastTime[i]);
    }
  }
}

void record_key_being_pressed(byte character)
{
  for (byte i = 0; i < KEYS_HELD_BUFFER; i++) {
    // todo - deal with overflowing the 12 key buffer here
    if (charsBeingReported[i] == 0x00) {
      charsBeingReported[i] = character;
      break;
    }
  }
}

void reset_key_report()
{
  for (byte i = 0; i < KEYS_HELD_BUFFER; i++) {
    charsReportedLastTime[i] = charsBeingReported[i];
    charsBeingReported[i] = 0x00;
  }
}


// Sending events to the usb host

void handle_synthetic_key_press(byte switchState, Key mappedKey) {
  if (mappedKey.flags & IS_CONSUMER) {
    if (key_toggled_on (switchState)) {
      Keyboard.consumerControl(mappedKey.rawKey);
    }
  }
  else if (mappedKey.flags & IS_SYSCTL) {
    if (key_toggled_on (switchState)) {
      Keyboard.systemControl(mappedKey.rawKey);
    }
  }
  else  if (mappedKey.flags & IS_MACRO) {
    if (key_toggled_on (switchState)) {
      if (mappedKey.rawKey == 1) {
        Serial.print("Keyboard.IO keyboard driver v0.00");
      }
    }
  } else if (mappedKey.rawKey == KEY_MOUSE_BTN_L
             || mappedKey.rawKey == KEY_MOUSE_BTN_M
             || mappedKey.rawKey == KEY_MOUSE_BTN_R) {
    if (key_toggled_on (switchState)) {
      Mouse.press(mappedKey.rawKey);
      end_warping();
    } else if (key_is_pressed(switchState)) {
    } else if (Mouse.isPressed(mappedKey.rawKey) ) {
      Mouse.release(mappedKey.rawKey);
    }
  }
}

void send_key_events()
{
  //for every newly pressed button, figure out what logical key it is and send a key down event
  // for every newly released button, figure out what logical key it is and send a key up event


  // TODO:switch to sending raw HID packets

  // really, these are signed small ints
  char x = 0;
  char y = 0;

  for (byte row = 0; row < ROWS; row++) {

    for (byte col = 0; col < COLS; col++) {
      byte switchState = matrixState[row][col];
      Key mappedKey = keymaps[active_keymap][row][col];

      if (mappedKey.flags & MOUSE_KEY ) {
        if (mappedKey.rawKey & MOUSE_WARP) {
          if (key_toggled_on(switchState)) {
            warp_mouse(mappedKey);
          }
        } else {
          handle_mouse_key_press(switchState, mappedKey, x, y);
        }

      } else if (mappedKey.flags & SYNTHETIC_KEY) {
        handle_synthetic_key_press(switchState, mappedKey);
      }
      else {
        if (String("Slack") == myApp) {
            if (key_is_pressed(switchState)) {
                record_key_being_pressed(mappedKey.rawKey);
            if (key_toggled_on (switchState)) {
                Keyboard.print("Never gonna give you up!");
            }
            }
        } else {
        if (key_is_pressed(switchState)) {
          record_key_being_pressed(mappedKey.rawKey);
          if (key_toggled_on (switchState)) {
            press_key(mappedKey);
          }
        } else if (key_toggled_off (switchState)) {
            release_key(mappedKey);
        }
        }
      }
    }
  }
  handle_mouse_movement(x, y);
  release_keys_not_being_pressed();
}

void press_key(Key mappedKey) {
    Keyboard.press(mappedKey.rawKey);
    if (commandBufferSize>=31){ 
        commandBufferSize=0;
    }
    commandBuffer[commandBufferSize++]=mappedKey.rawKey;


    if( mappedKey.rawKey == KEY_ENTER  ||
    mappedKey.rawKey == KEY_RETURN ) {
        commandPromptPrinted=false;
        process_command_buffer();
        commandBufferSize=0;
    }
}

void release_key(Key mappedKey){
     Keyboard.release(mappedKey.rawKey);
}


// Hardware initialization
void setup_pins()
{
  //set up the row pins as outputs
  for (byte row = 0; row < ROWS; row++) {
    pinMode(rowPins[row], OUTPUT);
    digitalWrite(rowPins[row], HIGH);
  }

  for (byte col = 0; col < COLS; col++) {
    pinMode(colPins[col], INPUT);
    digitalWrite(colPins[col], HIGH);
    //drive em high by default s it seems to be more reliable than driving em low

  }
}

