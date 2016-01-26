
#pragma once


// Warping
double mouseActiveForCycles = 0;
float carriedOverX = 0;
float carriedOverY = 0;


// apparently, the mac discards 15% of the value space for mouse movement.
// need to test this on other platforms

#define HALF_WIDTH 16384
#define HALF_HEIGHT 16384



int abs_left = 0;
int abs_top = 0;

int next_width;
int next_height;
int section_top;
int section_left;
boolean is_warping = false;


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
