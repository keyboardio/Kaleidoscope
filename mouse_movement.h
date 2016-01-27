
#pragma once

#include "Arduino.h"
#include "key_defs.h"
#include "matrix_state.h"
#include "HID-Project.h"
// Mouse-related 'public' API methods

double mouse_accel (double cycles);
void move_mouse( int8_t x, int8_t y);
void begin_warping();
void end_warping();
void warp_mouse(Key key);




// apparently, the mac discards 15% of the value space for mouse movement.
// need to test this on other platforms

#define MAX_WARP_WIDTH 32767
#define MAX_WARP_HEIGHT 32767


// Mouse acceleration

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
