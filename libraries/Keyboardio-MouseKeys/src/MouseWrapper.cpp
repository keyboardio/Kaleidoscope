
// Mouse-related methods
//
//
#include "MouseWrapper.h"


MouseWrapper_::MouseWrapper_(void) {
    Mouse.begin();
    AbsoluteMouse.begin();
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


// cubic wave function based on code from FastLED
uint8_t MouseWrapper_::acceleration(uint8_t cycles) {
    uint8_t i = cycles;

    if( i & 0x80) {
        i = 255 - i;
    }

    i = i << 1;

    uint8_t ii = (i*i) >> 8;
    uint8_t iii = (ii*i) >> 8;

    i =  (( (3 * (uint16_t)(ii)) - ( 2 * (uint16_t)(iii))) / 2) + ACCELERATION_FLOOR;

    if ( i > ACCELERATION_CEIL) {
	i = ACCELERATION_CEIL;
    }
    return i;
}


void MouseWrapper_::move( int8_t x, int8_t y) {
    int16_t moveX =0;
    int16_t moveY = 0;
    if (x != 0 ) {
    	if (mouseActiveForCyclesX < 255) { mouseActiveForCyclesX++;}
        moveX = (x * acceleration(mouseActiveForCyclesX));
    }
    if (y != 0) {
    	if (mouseActiveForCyclesY < 255) { mouseActiveForCyclesY++;}
        moveY = (y * acceleration(mouseActiveForCyclesY));

    }
	
        end_warping();
        Mouse.move(moveX, moveY, 0);

}

MouseWrapper_ MouseWrapper;
