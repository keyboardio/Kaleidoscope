
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
