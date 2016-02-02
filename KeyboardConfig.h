#pragma once 



#include "implementation/Model01Beta.h"


// These global proxy functions are a cheap hack to avoid 
// a circular dependency between the keyboard hardware class
// and the 'userspace' LED implementation. If my C++ were stronger, there woudl
// certainly be a better way -JV 2016-02-01
void led_set_crgb_at(uint8_t i, cRGB crgb);
void led_sync(void);



