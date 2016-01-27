
// Mouse-related methods
//
//
#include "Arduino.h"
#include "key_defs.h"
#include "debouncing.h"
#include "HID-Project.h"
#include "mouse_movement.h"


// Warping
static double mouseActiveForCycles = 0;
static float carriedOverX = 0;
static float carriedOverY = 0;


static int abs_left = 0;
static int abs_top = 0;
static int next_width;
static int next_height;
static int section_top;
static int section_left;
static boolean is_warping = false;


void _warp_jump(long left, long top, long height, long width) {
    long x_center = left + width/2;
    long y_center = top + height/2;
    AbsoluteMouse.moveTo(x_center,y_center);
}





void begin_warping() {
    section_left = abs_left;
    section_top = abs_top;
    next_width = MAX_WARP_WIDTH;
    next_height = MAX_WARP_HEIGHT;
    is_warping = true;
}

void end_warping() {
    is_warping= false;
}

void warp_mouse(Key quadrant) {
    if (is_warping == false) {
        begin_warping();
    }


    if ( quadrant.rawKey & MOUSE_END_WARP) {
        end_warping();
        return;
    }


    next_width = next_width / 2;
    next_height = next_height/2;

    if (quadrant.rawKey & MOUSE_UP) {
//    Serial.print(" - up ");
    } else if (quadrant.rawKey & MOUSE_DN) {
//   Serial.print(" - down ");
        section_top  = section_top + next_height;
    }

    if (quadrant.rawKey & MOUSE_L) {
        //  Serial.print(" - left ");
    } else if (quadrant.rawKey & MOUSE_R) {
        // Serial.print(" - right ");
        section_left  = section_left + next_width;
    }

    _warp_jump(section_left, section_top, next_height,next_width);

}



double mouse_accel (double cycles) {
    double accel = (atan((cycles * ACCELERATION_CLIMB_SPEED)-ACCELERATION_RUNWAY) + ATAN_LIMIT) * ACCELERATION_MULTIPLIER;
    if (accel < ACCELERATION_FLOOR) {
        accel = ACCELERATION_FLOOR;
    }
    return accel;
}

void move_mouse( int8_t x, int8_t y) {

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


void handle_mouse_key_press(byte switchState, Key mappedKey) {
    if (key_is_pressed(switchState)) {
        if (mappedKey.rawKey & MOUSE_UP) {
            move_mouse(0,-1);
        }
        if (mappedKey.rawKey & MOUSE_DN) {
            move_mouse(0,1);
        }
        if (mappedKey.rawKey & MOUSE_L) {
            move_mouse(-1,0);
        }

        if (mappedKey.rawKey & MOUSE_R) {
            move_mouse(1,0);
        }
    }
}

