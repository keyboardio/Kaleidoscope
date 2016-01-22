
// Mouse-related methods
//
//
#include "Arduino.h"
#include "key_defs.h"
#include "mouse_movement.h"
#include "debouncing.h"
#include "HID-Project.h"

void _warp_jump(long left, long top, long height, long width) {
    long x_center = left + width/2;
    long y_center = top + height/2;
    AbsoluteMouse.moveTo(x_center,y_center);
}



int last_x;
int last_y;

// apparently, the mac discards 15% of the value space for mouse movement.
// need to test this on other platforms
//

#define HALF_WIDTH 16384
#define HALF_HEIGHT 16384



int abs_left = 0;
int abs_top = 0;

int next_width;
int next_height;
int section_top;
int section_left;
boolean is_warping = false;

void begin_warping() {
    section_left = abs_left;
    section_top = abs_top;
    next_width = 32767;
    next_height = 32767;
    is_warping = true;
}

void end_warping() {
    is_warping= false;
}

void warp_mouse(Key ninth) {
    if (is_warping == false) {
        begin_warping();
    }


    if ( ninth.rawKey & MOUSE_END_WARP) {
        end_warping();
        return;
    }


    next_width = next_width / 2;
    next_height = next_height/2;

    if (ninth.rawKey & MOUSE_UP) {
//    Serial.print(" - up ");
    } else if (ninth.rawKey & MOUSE_DN) {
//   Serial.print(" - down ");
        section_top  = section_top + next_height;
    }

    if (ninth.rawKey & MOUSE_L) {
        //  Serial.print(" - left ");
    } else if (ninth.rawKey & MOUSE_R) {
        // Serial.print(" - right ");
        section_left  = section_left + next_width;
    }

    _warp_jump(section_left, section_top, next_height,next_width);

}


// we want the whole s curve, not just the bit
// that's usually above the x and y axes;
#define ATAN_LIMIT 1.57079633
#define ACCELERATION_FLOOR 0.25
#define ACCELERATION_MULTIPLIER 5
#define  ACCELERATION_RUNWAY 5
// Climb speed is how fast we get to max speed
// 1 is "instant"
// 0.05 is just right
// 0.001 is insanely slow

#define ACCELERATION_CLIMB_SPEED  0.05

double mouse_accel (double cycles) {
    double accel = (atan((cycles * ACCELERATION_CLIMB_SPEED)-ACCELERATION_RUNWAY) + ATAN_LIMIT) * ACCELERATION_MULTIPLIER;
    if (accel < ACCELERATION_FLOOR) {
        accel = ACCELERATION_FLOOR;
    }
    return accel;
}

void handle_mouse_movement( char x, char y) {

    if (x != 0 || y != 0) {
        mouseActiveForCycles++;
        double accel = (double) mouse_accel(mouseActiveForCycles);
        float moveX = 0;
        float moveY = 0;
        if (x > 0) {
            moveX = (x * accel) + carriedOverX;
            carriedOverX = moveX - floor(moveX);
        } else if (x < 0) {
            moveX = (x * accel) - carriedOverX;
            carriedOverX = ceil(moveX) - moveX;
        }

        if (y > 0) {
            moveY = (y * accel) + carriedOverY;
            carriedOverY = moveY - floor(moveY);
        } else if (y < 0) {
            moveY = (y * accel) - carriedOverY;
            carriedOverY = ceil(moveY) - moveY;
        }
        end_warping();

        Mouse.move(moveX, moveY, 0);
    } else {
        mouseActiveForCycles = 0;
    }

}


void handle_mouse_key_press(byte switchState, Key mappedKey, char &x, char &y) {

    if (key_is_pressed(switchState)) {
        if (mappedKey.rawKey & MOUSE_UP) {
            y -= 1;
        }
        if (mappedKey.rawKey & MOUSE_DN) {
            y += 1;
        }
        if (mappedKey.rawKey & MOUSE_L) {
            x -= 1;
        }

        if (mappedKey.rawKey & MOUSE_R) {
            x += 1 ;
        }
    }
}

