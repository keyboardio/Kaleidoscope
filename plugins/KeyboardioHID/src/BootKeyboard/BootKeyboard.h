/*
Copyright (c) 2014-2015 NicoHood
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
#include "PluggableUSB.h"
#include "HID.h"
#include "HID-Settings.h"
#include "../HID-APIs/KeyboardAPI.h"


typedef union{
	// Low level key report: up to 6 keys and shift, ctrl etc at once
	uint8_t whole8[];
	uint16_t whole16[];
	uint32_t whole32[];
	struct{
		uint8_t modifiers;
		uint8_t reserved;
		KeyboardKeycode keycodes[6];
	};
	uint8_t keys[8];
} HID_KeyboardReport_Data_t;


class BootKeyboard_ : public PluggableUSBModule, public KeyboardAPI
{
public:
    BootKeyboard_(void);

  // Also use the base class functions
  // http://en.cppreference.com/w/cpp/language/using_declaration
  using KeyboardAPI::write;
  using KeyboardAPI::press;
  using KeyboardAPI::release;
  using KeyboardAPI::add;
  using KeyboardAPI::remove;

  // Implement adding/removing key functions
  inline virtual size_t removeAll(void) override;

   int send(void);



    uint8_t getLeds(void);
    uint8_t getProtocol(void);
    void wakeupHost(void);
    
    void setFeatureReport(void* report, int length){
        if(length > 0){
            featureReport = (uint8_t*)report;
            featureLength = length;
            
            // Disable feature report by default
            disableFeatureReport();
        }
    }
    
    int availableFeatureReport(void){
        if(featureLength < 0){
            return featureLength & ~0x8000;
        }
        return 0;
    }
    
    void enableFeatureReport(void){
        featureLength &= ~0x8000;
    }
    
    void disableFeatureReport(void){
        featureLength |= 0x8000;
    }
    

protected:
    HID_KeyboardReport_Data_t _keyReport;

    // Implementation of the PUSBListNode
    int getInterface(uint8_t* interfaceCount);
    int getDescriptor(USBSetup& setup);
    bool setup(USBSetup& setup);
    
    uint8_t epType[1];
    uint8_t protocol;
    uint8_t idle;
    
    uint8_t leds;
    
    uint8_t* featureReport;
    int featureLength;
private:
  inline virtual size_t set(KeyboardKeycode k, bool s) override;
};
extern BootKeyboard_ BootKeyboard;


