
// Mouse-related methods
//
//
#include "MouseWrapper.h"


MouseWrapper_::MouseWrapper_(void) {
    Mouse.begin();
}

void MouseWrapper_::press_button(uint8_t button) {
    Mouse.press(button);
    end_warping();

}

void MouseWrapper_::release_button(uint8_t button) {
    Mouse.release(button);
}


void MouseWrapper_::warp_jump(long left, long top, long height, long width) {
    long x_center = left + width/2;
    long y_center = top + height/2;
    AbsoluteMouse.moveTo(x_center,y_center);
}





void MouseWrapper_::begin_warping() {
    section_left = WARP_ABS_LEFT;
    section_top = WARP_ABS_TOP;
    next_width = MAX_WARP_WIDTH;
    next_height = MAX_WARP_HEIGHT;
    is_warping = true;
}

void MouseWrapper_::end_warping() {
    is_warping= false;
}

void MouseWrapper_::warp(uint8_t warp_cmd) {
    if (is_warping == false) {
        begin_warping();
    }


    if ( warp_cmd & WARP_END) {
        end_warping();
        return;
    }


    next_width = next_width / 2;
    next_height = next_height/2;

    if (warp_cmd & WARP_UP) {
//    Serial.print(" - up ");
    } else if (warp_cmd & WARP_DOWN) {
//   Serial.print(" - down ");
        section_top  = section_top + next_height;
    }

    if (warp_cmd & WARP_LEFT) {
        //  Serial.print(" - left ");
    } else if (warp_cmd & WARP_RIGHT) {
        // Serial.print(" - right ");
        section_left  = section_left + next_width;
    }

    warp_jump(section_left, section_top, next_height,next_width);

}



double MouseWrapper_::acceleration (double cycles) {
    double accel = (atan((cycles * ACCELERATION_CLIMB_SPEED)-ACCELERATION_RUNWAY) + ATAN_LIMIT) * ACCELERATION_MULTIPLIER;
    if (accel < ACCELERATION_FLOOR) {
        accel = ACCELERATION_FLOOR;
    }
    return accel;
}

void MouseWrapper_::move( int8_t x, int8_t y) {

    if (x != 0 || y != 0) {
        mouseActiveForCycles++;
        double accel = (double) acceleration(mouseActiveForCycles);
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

MouseWrapper_ MouseWrapper;
