/** This source file instantiates the default USB HID based EventDispatcher
 * implementation in the case that the hardware header has not defined it.
 * The goal is to remove this once all hardware implementations are
 * taking care of registering this for themselves. */
#include KALEIDOSCOPE_HARDWARE_H
#ifndef HARDWARE_EVENT_DISPATCHER

#include "HIDEventDispatcher.h"
HIDEventDispatcher defaultHIDDispatcher;

#endif
