/*
Copyright (c) 2014-2015 NicoHood
Copyright (c) 2015-2018 Keyboard.io, Inc

See the readme for credit to other people.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

// Include guard
#pragma once

#include <Arduino.h>
#include "HID.h"
#include "HID-Settings.h"
#include "../MouseButtons.h"

typedef union {
    // Mouse report: 8 buttons, position, wheel
    struct {
        uint8_t buttons;
        int8_t xAxis;
        int8_t yAxis;
        int8_t vWheel;
        int8_t hWheel;
    };
} HID_MouseReport_Data_t;


class Mouse_ {
  public:
    Mouse_(void);
    void begin(void);
    void end(void);
    void click(uint8_t b = MOUSE_LEFT);
    void move(signed char x, signed char y, signed char vWheel = 0, signed char hWheel = 0);
    void press(uint8_t b = MOUSE_LEFT);   // press LEFT by default
    void release(uint8_t b = MOUSE_LEFT); // release LEFT by default
    bool isPressed(uint8_t b = MOUSE_LEFT); // check LEFT by default

    /** getReport returns the current report.
     *
     * The current report is the one to be send next time sendReport() is called.
     *
     * @returns A copy of the report.
     */
    const HID_MouseReport_Data_t getReport() {
        return report;
    }
    void sendReport(void);

    void releaseAll(void);

  protected:
    HID_MouseReport_Data_t report;
    HID_MouseReport_Data_t lastReport;

  private:
    void sendReportUnchecked(void);
};
extern Mouse_ Mouse;
