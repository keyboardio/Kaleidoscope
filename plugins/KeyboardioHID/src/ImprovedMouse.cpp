/*
  Mouse.cpp

  Copyright (c) 2015, Arduino LLC
  Original code (pre-library): Copyright (c) 2011, Peter Barrett

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "ImprovedMouse.h"

#if defined(_USING_HID)

static const uint8_t _hidReportDescriptor[] PROGMEM = {
 /*  Mouse relative */
    0x05, 0x01,                      /* USAGE_PAGE (Generic Desktop)	  54 */
    0x09, 0x02,                      /* USAGE (Mouse) */
    0xa1, 0x01,                      /* COLLECTION (Application) */
    0x85, HID_REPORTID_MOUSE,				/*     REPORT_ID */

	/* 8 Buttons */
    0x05, 0x09,                      /*     USAGE_PAGE (Button) */
    0x19, 0x01,                      /*     USAGE_MINIMUM (Button 1) */
    0x29, 0x08,                      /*     USAGE_MAXIMUM (Button 8) */
    0x15, 0x00,                      /*     LOGICAL_MINIMUM (0) */
    0x25, 0x01,                      /*     LOGICAL_MAXIMUM (1) */
    0x95, 0x08,                      /*     REPORT_COUNT (8) */
    0x75, 0x01,                      /*     REPORT_SIZE (1) */
    0x81, 0x02,                      /*     INPUT (Data,Var,Abs) */

	/* X, Y, Wheel */
    0x05, 0x01,                      /*     USAGE_PAGE (Generic Desktop) */
    0x09, 0x30,                      /*     USAGE (X) */
    0x09, 0x31,                      /*     USAGE (Y) */
    0x09, 0x38,                      /*     USAGE (Wheel) */
    0x15, 0x81,                      /*     LOGICAL_MINIMUM (-127) */
    0x25, 0x7f,                      /*     LOGICAL_MAXIMUM (127) */
    0x75, 0x08,                      /*     REPORT_SIZE (8) */
    0x95, 0x03,                      /*     REPORT_COUNT (3) */
    0x81, 0x06,                      /*     INPUT (Data,Var,Rel) */

	/* End */
    0xc0                            /* END_COLLECTION */
};

//================================================================================
//================================================================================
//	Mouse

Mouse_::Mouse_(void) : _buttons(0),
HIDDevice((uint8_t*)_hidReportDescriptor, sizeof(_hidReportDescriptor), HID_REPORTID_MOUSE)
{
	// HID Descriptor is appended via the inherited HIDDevice class
}

void Mouse_::begin(void) 
{
    end();
}

void Mouse_::end(void) 
{
    _buttons = 0;
    move(0, 0, 0);
}

void Mouse_::click(uint8_t b)
{
	_buttons = b;
	move(0,0,0);
	_buttons = 0;
	move(0,0,0);
}

void Mouse_::move(signed char x, signed char y, signed char wheel)
{
	HID_MouseReport_Data_t report;
	report.buttons = _buttons;
	report.xAxis = x;
	report.yAxis = y;
	report.wheel = wheel;
#if defined(USE_BOOT_MOUSE_PROTOCOL)
	if(getProtocol() != 1){
	    // Do not send the wheel information.
	    // It should be discarded by the host,
	    // but to be sure we better leave it out.
		SendRawReport(&report, sizeof(report)-1);
	}
	else
#endif
	SendReport(&report, sizeof(report));
}

void Mouse_::buttons(uint8_t b)
{
	if (b != _buttons)
	{
		_buttons = b;
		move(0,0,0);
	}
}

void Mouse_::press(uint8_t b) 
{
	buttons(_buttons | b);
}

void Mouse_::release(uint8_t b)
{
	buttons(_buttons & ~b);
}

bool Mouse_::isPressed(uint8_t b)
{
	if ((b & _buttons) > 0) 
		return true;
	return false;
}

Mouse_ Mouse;

#endif