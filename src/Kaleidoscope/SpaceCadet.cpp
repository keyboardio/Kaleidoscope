/* -*- mode: c++ -*-
 * Kaleidoscope-SpaceCadet -- Space Cadet Shift
 * Copyright (C) 2016, 2017  Gergely Nagy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Modified by Ben Gemperline to support additional keys.
 */

#include <Kaleidoscope-SpaceCadet.h>
#include <kaleidoscope/hid.h>
#include <stdint.h>
#include "SpaceCadet.h"

namespace kaleidoscope {

    //Default constructor
    ModifierKeyMap::ModifierKeyMap(){
    }

    //Constructor with input and output, and assume default timeout
    ModifierKeyMap::ModifierKeyMap(Key input_, Key output_){
        input = input_;
        output = output_;
    }

    //Constructor with all three set
    ModifierKeyMap::ModifierKeyMap(Key input_, Key output_, uint16_t timeout_) {
        input = input_;
        output = output_;
        timeout = timeout_;
    }

    //Space Cadet
    uint8_t SpaceCadet::map_size = 0;
    ModifierKeyMap * SpaceCadet::map;
    uint16_t SpaceCadet::time_out = 1000;

    SpaceCadet::SpaceCadet() {
        //By default, we make one with left shift sending left paren, and right shift sending right paren
        static ModifierKeyMap internalMap[] = {
            {Key_LeftShift,Key_LeftParen,250}
            ,{Key_RightShift,Key_RightParen,250}
            /*,{Key_LeftGui,Key_LeftCurlyBracket,250}
            ,{Key_RightAlt,Key_RightCurlyBracket,250}
            ,{Key_LeftControl,Key_LeftBracket,250}
            ,{Key_RightControl,Key_RightBracket,250}*/
        };

        map = internalMap;
        map_size = sizeof(internalMap)/sizeof(internalMap[0]);
        //setMap(internalMap, sizeof(internalMap));
    }

    SpaceCadet::SpaceCadet(ModifierKeyMap * map_, uint8_t map_size_) {
        //Call the initializer
        setMap(map_, map_size_);
    }

    void SpaceCadet::setMap(ModifierKeyMap * map_, uint8_t map_size_){
        //Set the map
        map = map_;
        //set the map size to be the length of the array
        map_size = map_size_;
    }

    void SpaceCadet::begin() {
        Kaleidoscope.useEventHandlerHook(eventHandlerHook);
    }

    Key SpaceCadet::eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state) {
        //char buffer[50];

        //Serial.print("In eventHandlerHook");
        // If nothing happened, bail out fast.
        if (!keyIsPressed(key_state) && !keyWasPressed(key_state)) {
            return mapped_key;
        }

        // If a key has been just toggled on...
        if (keyToggledOn(key_state)) {

            /*
            sprintf(buffer, "Pressed Key: %u\n", mapped_key.raw );
            Serial.print(buffer);
            */

            if(map_size > 0) {
                //This will only set one key, and if it isn't in our map it clears everything for the non-pressed key
                for (uint8_t i = 0; i < map_size; ++i) {
                    if(mapped_key.raw == map[i].input.raw) {
                        map[i].flagged = true;
                        map[i].start_time = millis();
                        //There was a valid keypress
                        /*sprintf(buffer, "Valid Key: %u\n", mapped_key.raw);
                        Serial.print(buffer);*/

                    } else {
                        map[i].flagged = false;
                        map[i].start_time = 0;
                    }

                    /*
                    sprintf(buffer, "Key: %u\n", map[i].input.raw);
                    Serial.print(buffer);
                    if(map[i].flagged){
                        Serial.print("Flagged\n");
                    } else {
                        Serial.print("Not Flagged\n");
                    }
                    sprintf(buffer, "Start Time: %u\n", map[i].start_time);
                    Serial.print(buffer);
                     */
                }
            }

            // this is all we need to do on keypress, let the next handler do its thing too.
            return mapped_key;
        }

        // if the state is empty, that means that either the shifts weren't pressed,
        // or we used another key in the interim. in both cases, nothing special to do.
        bool valid_key = false;
        bool pressed_key_was_valid = false;
        uint8_t index = 0;
        if(map_size > 0) {
            //Look to see if any are flagged
            for (uint8_t i = 0; i < map_size; ++i) {

                if (map[i].flagged) {
                    valid_key = true;
                    index = i;
                }
                if (map[i].input.raw == mapped_key.raw) {
                    pressed_key_was_valid = true;
                }
            }
        }
        if (!valid_key) {
            return mapped_key;
        }

        //use the map index to find the local timeout for this key
        uint16_t current_timeout = map[index].timeout;
        //If that isn't set, use the global timeout setting.
        if(current_timeout == 0){
            current_timeout = time_out;
        }



        if ((millis() - map[index].start_time) >= current_timeout) {
            // if we timed out, that means we need to keep pressing shift, but won't
            // need the parens in the end.
            map[index].flagged = false;
            map[index].start_time = 0;
            return mapped_key;
        }

        /*
        sprintf(buffer, "Check Index: %u\n", index );
        Serial.print(buffer);

        sprintf(buffer, "Current Timeout: %u\n", current_timeout );
        Serial.print(buffer);

        sprintf(buffer, "Start Time: %u\n", map[index].start_time );
        Serial.print(buffer);

        Serial.print("Made it past timeout check\n");
        */

        // if we have a state, but the key in question is not either of the shifts,
        // return. This can happen when another key is released, and that should not
        // interrupt us.

        if (!pressed_key_was_valid) {
            return mapped_key;
        }

        //Serial.print("Made it validity check\n");


        // if a key toggled off (and that must be one of the shifts at this point),
        // send the parens too (if we were interrupted, we bailed out earlier).
        if (keyToggledOff(key_state)) {
            Key pressed_key = map[index].input;
            Key alternate_key = map[index].output;

            //Don't necessarily need to send the original key
            //handleKeyswitchEvent(pressed_key, row, col, WAS_PRESSED | INJECTED);
            handleKeyswitchEvent(alternate_key, row, col, IS_PRESSED | INJECTED);
            hid::sendKeyboardReport();

            map[index].flagged = false;
            map[index].start_time = 0;
        }

        return mapped_key;
    }

}

kaleidoscope::SpaceCadet SpaceCadet;
