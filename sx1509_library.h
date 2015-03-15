/*
	sx1509_library.h
		Header file for the SX1509 Arduino library.
	
	by: Jim Lindblom
		SparkFun Electronics
	date: December 13, 2012
	
	license: Beerware. Feel free to use it, with or without attribution, in
		your own projects. If you find it helpful, buy me a beer next time you
		see me at the local pub.
	
	In here you'll find the Arduino code used to interface with the SX1509 I2C
	16 I/O expander. There are functions to take advantage of everything the
	SX1509 provides - input/output setting, writing pins high/low, reading 
	the input value of pins, LED driver utilities (blink, breath, pwm), and
	keypad engine utilites.
	
	This file includes detailed descriptions of each of the sx1509Class's
	public methods.
	
	For example uses of these functions, see the Arduino example codes in the
	./examples/ folder.
*/

#include "Arduino.h"

#ifndef sx1509_library_H
#define sx1509_library_H

#define RECEIVE_TIMEOUT_VALUE 1000	// Timeout for I2C receive

// These are used for setting LED driver to linear or log mode:
#define LINEAR		0	
#define LOGARITHMIC	1

// These are used for clock config:
#define INTERNAL_CLOCK	2
#define EXTERNAL_CLOCK	1
	
class sx1509Class
{
private:	// These private functions are not available to Arduino sketches.
			// If you need to read or write directly to registers, consider
			// putting the writeByte, readByte functions in the public section
	byte deviceAddress; // I2C Address of SX1509
// Pin definitions:
	byte pinInterrupt;
	byte pinOscillator;
	byte pinReset;
// Read Functions:
	byte readByte(byte registerAddress);
	unsigned int readWord(byte registerAddress);
	void readBytes(byte firstRegisterAddress, byte * destination, byte length);
// Write functions:
	void writeByte(byte registerAddress, byte writeValue);
	void writeWord(byte registerAddress, unsigned int writeValue);
	void writeBytes(byte firstRegisterAddress, byte * writeArray, byte length);

public:
// -----------------------------------------------------------------------------
// Constructor - sx1509Class: This function sets up the pins connected to the 
//		SX1509, and sets up the private deviceAddress variable.
//
// Inputs:
//		- address: should be the 7-bit address of the SX1509. This should be  
//		 one of four values - 0x3E, 0x3F, 0x70, 0x71 - all depending on what the
//		 ADDR0 and ADDR1 pins ar se to. This variable is required.
//		- resetPin: This is the Arduino pin tied to the SX1509 RST pin. This
//		 pin is optional. If not declared, the library will attempt to
//		 software reset the SX1509.
//		- interruptPin: This is the Arduino pin tied to the SX1509 active-low
//		 interrupt output. Only necessary if you're planning on using
//		 the interrupt capabilities.
//		- oscillatorPin: This is the Arduino pin tied to the SX1509's OSCIO 
//		 pin. This pin can be an output or an input. This parameter is optional.
// -----------------------------------------------------------------------------
	sx1509Class(byte address, byte resetPin = 255, 
				byte interruptPin = 255, byte oscillatorPin = 255);

// -----------------------------------------------------------------------------
// init(void): This function initializes the SX1509. It begins the Wire 
//  	library, resets the IC, and tries to read some registers to prove it's 
//		connected.
// 
// Output: Returns a 1 if communication is successful, 0 on error.
// -----------------------------------------------------------------------------
	byte init(void);

// -----------------------------------------------------------------------------
// reset(bool hardware): This function resets the SX1509 - either a hardware 
//		reset or software. A hardware reset (hardware parameter = 1) pulls the 
//		reset line low, pausing, then pulling the reset line high. A software 
//		reset writes a 0x12 then 0x34 to the REG_RESET as outlined in the
//		datasheet.
//
//  Input:
//	 	- hardware: 0 executes a software reset, 1 executes a hardware reset
// -----------------------------------------------------------------------------
	void reset(bool hardware);

// -----------------------------------------------------------------------------
// pinDir(byte pin, byte inOut): This function sets one of the SX1509's 16 
//		outputs to either an INPUT or OUTPUT.
//
//	Inputs:
//	 	- pin: should be a value between 0 and 15
//	 	- inOut: The Arduino INPUT and OUTPUT constants should be used for the 
//		 inOut parameter. They do what they say!
// -----------------------------------------------------------------------------
	void pinDir(byte pin, byte inOut);
	
// -----------------------------------------------------------------------------
// writePin(byte pin, byte highLow): This function writes a pin to either high 
//		or low if it's configured as an OUTPUT. If the pin is configured as an 
//		INPUT, this method will activate either the PULL-UP	or PULL-DOWN
//		resistor (HIGH or LOW respectively).
//
//	Inputs:
//		- pin: The SX1509 pin number. Should be a value between 0 and 15.
//		- highLow: should be Arduino's defined HIGH or LOW constants.
// -----------------------------------------------------------------------------
	void writePin(byte pin, byte highLow);
	void rawWritePin(byte pin, byte highLow);

// -----------------------------------------------------------------------------
// readPin(byte pin): This function reads the HIGH/LOW status of a pin.
//		The pin should be configured as an INPUT, using the pinDir function.
//
//	Inputs:
//	 	- pin: The SX1509 pin to be read. should be a value between 0 and 15.
//  Outputs:
//		This function returns a 1 if HIGH, 0 if LOW
// -----------------------------------------------------------------------------
	byte readPin(byte pin);
	byte rawReadPin(byte pin);
	
// -----------------------------------------------------------------------------
// ledDriverInit(byte pin, byte freq, bool log): This function initializes LED 
//		driving on a pin. It must be called if you want to use the pwm or blink 
//		functions on that pin.
//	
//	Inputs:
//		- pin: The SX1509 pin connected to an LED. Should be 0-15.
// 		- freq: decides ClkX, and should be a value between 1-7
//			- ClkX = 2MHz / (2^(freq - 1)
//			- freq defaults to 1, which makes ClkX = 2MHz
//		- log: selects either linear or logarithmic mode on the LED drivers
//			- log defaults to 0, linear mode
//			- currently log sets both bank A and B to the same mode
//	Note: this function automatically decides to use the internal 2MHz osc.
// -----------------------------------------------------------------------------
	void ledDriverInit(byte pin, byte freq = 1, bool log = 0);
	
// -----------------------------------------------------------------------------
// pwm(byte pin, byte iOn):	This function can be used to control the intensity 
//		of an output pin connected to an LED.
//
//	Inputs:
//		- pin: The SX1509 pin connecte to an LED.Should be 0-15.
//		- iOn: should be a 0-255 value setting the intensity of the LED
//			- 0 is completely off, 255 is 100% on.
//
//	Note: ledDriverInit should be called on the pin before calling this.
// -----------------------------------------------------------------------------
	void pwm(byte pin, byte iOn);
	
// -----------------------------------------------------------------------------
// blink(byte pin, byte tOn, byte tOff, byte offIntensity, byte tRise, byte 
//		tFall):  blink performs both the blink and breath LED driver functions.
//
// 	Inputs:
//  	- pin: the SX1509 pin (0-15) you want to set blinking/breathing.
//		- tOn: the amount of time the pin is HIGH
//			- This value should be between 1 and 31. 0 is off.
//		- tOff: the amount of time the pin is at offIntensity
//			- This value should be between 1 and 31. 0 is off.
//		- offIntensity: How dim the LED is during the off period.
//			- This value should be between 0 and 7. 0 is completely off.
//		- onIntensity: How bright the LED will be when completely on.
//			- This value can be between 0 (0%) and 255 (100%).
//		- tRise: This sets the time the LED takes to fade in.
//			- This value should be between 1 and 31. 0 is off.
//			- This value is used with tFall to make the LED breath.
//		- tFall: This sets the time the LED takes to fade out.
//			- This value should be between 1 and 31. 0 is off.
// 	 Notes: 
//		- The breathable pins are 4, 5, 6, 7, 12, 13, 14, 15 only. If tRise and 
//			tFall are set on 0-3 or 8-11 those pins will still only blink.
// 		- ledDriverInit should be called on the pin to be blinked before this.
// -----------------------------------------------------------------------------
	void blink(byte pin, byte tOn, byte toff, byte offIntensity = 0, 
			   byte onIntensity = 255, byte tRise = 0, byte tFall = 0);

// -----------------------------------------------------------------------------
// keypad(byte rows, byte columns, byte sleepTime, byte scanTime): This function
//		will initialize the keypad function on the SX1509.
//
//	Inputs:
//		- rows: The number of rows in the button matrix.
//			- This value must be between 1 and 7. 0 will turn it off.
//			- eg: 1 = 2 rows, 2 = 3 rows, 7 = 8 rows, etc.
//		- columns: The number of columns in the button matrix
//			- This value should be between 0 and 7.
//			- 0 = 1 column, 7 = 8 columns, etc.
//		- sleepTime: Sets the auto-sleep time of the keypad engine. 3-bit value:
//				0 : OFF
//				1 : 128ms x 2MHz/fOSC
//				2 : 256ms x 2MHz/fOSC
//				3 : 512ms x 2MHz/fOSC
//				4 : 1sec x 2MHz/fOSC
//				5 : 2sec x 2MHz/fOSC
//				6 : 4sec x 2MHz/fOSC
//				7 : 8sec x 2MHz/fOSC
//			- scanTime: Sets the scan time per row. Must be set above debounce 
//				time. 3-bit value:
//				0 : 1ms x 2MHz/fOSC
//				1 : 2ms x 2MHz/fOSC
//				2 : 4ms x 2MHz/fOSC
//				3 : 8ms x 2MHz/fOSC
//				4 : 16ms x 2MHz/fOSC
//				5 : 32ms x 2MHz/fOSC
//				6 : 64ms x 2MHz/fOSC
//				7 : 128ms x 2MHz/fOSC
// -----------------------------------------------------------------------------
	void keypad(byte rows, byte columns, byte sleepTime = 0, byte scanTime = 0);

// -----------------------------------------------------------------------------
// readKeyData(): This function returns a 16-bit value containing the status of
//		keypad engine.
//
//	Output:
//		A 16-bit value is returned. The lower 8 bits represent the up-to 8 rows,
//		while the MSB represents the up-to 8 columns. Bit-values of 1 indicate a
//		button in that row or column is being pressed. As such, at least two
//		bits should be set.
// -----------------------------------------------------------------------------
	unsigned int readKeyData();
	
// -----------------------------------------------------------------------------
// sync(void): this function resets the PWM/Blink/Fade counters, syncing any 
//		blinking LEDs. Bit 2 of REG_MISC is set, which alters the functionality 
//		of the nReset pin. The nReset pin is toggled low->high, which should 
//		reset all LED counters. Bit 2 of REG_MISC is again cleared, returning 
//		nReset pin to POR functionality
// -----------------------------------------------------------------------------
	void sync(void);
	
// -----------------------------------------------------------------------------
// debounceConfig(byte configValue): This method configures the debounce time of 
//		every input.
//
//	Input:
//		- configValue: A 3-bit value configuring the debounce time.
//			000: 0.5ms * 2MHz/fOSC
//			001: 1ms * 2MHz/fOSC
//			010: 2ms * 2MHz/fOSC
//			011: 4ms * 2MHz/fOSC
//			100: 8ms * 2MHz/fOSC
//			101: 16ms * 2MHz/fOSC
//			110: 32ms * 2MHz/fOSC
//			111: 64ms * 2MHz/fOSC
//
//	Note: fOSC is set with the configClock function. It defaults to 2MHz.
// -----------------------------------------------------------------------------
	void debounceConfig(byte configVaule);

// -----------------------------------------------------------------------------
// debounceEnable(byte pin): This method enables debounce on SX1509 input pin.
//
//	Input:
//		- pin: The SX1509 pin to be debounced. Should be between 0 and 15.	
//
//	Note: debounceConfig() should be called before this, to configure the clock
//		and other debounce parameters.
// -----------------------------------------------------------------------------
	void debounceEnable(byte pin);

// -----------------------------------------------------------------------------	
// enableInterrupt(byte pin, byte riseFall): This function sets up an interrupt 
//		on a pin. Interrupts can occur on all SX1509 pins, and can be generated
//		on rising, falling, or both.
//
//	Inputs: 
//		-pin: SX1509 input pin that will generate an input. Should be 0-15.
//		-riseFall: Configures if you want an interrupt generated on rise fall or
//			both. For this param, send the pin-change values previously defined 
//			by Arduino:
//			#define CHANGE 1	<-Both
//			#define FALLING 2	<- Falling
//			#define RISING 3	<- Rising
//	
//	Note: This function does not set up a pin as an input, or configure	its 
//		pull-up/down resistors! Do that before (or after).
// -----------------------------------------------------------------------------
	void enableInterrupt(byte pin, byte riseFall);
	
// -----------------------------------------------------------------------------
// interruptSource(void): Returns an unsigned int representing which pin caused 
//		an interrupt.
//
//	Output: 16-bit value, with a single bit set representing the pin(s) that
//		generated an interrupt. E.g. a return value of	0x0104 would mean pins 8
//		and 3 (bits 8 and 3) have generated an interrupt.
//
//	Note: This function also clears all interrupts
// -----------------------------------------------------------------------------
	unsigned int interruptSource(void);

// -----------------------------------------------------------------------------
// configClock(byte oscSource, byte oscPinFunction, byte oscFreqOut, 
//		byte oscDivider): This function configures the oscillator source/speed
//		and the clock, which is used to drive LEDs and time debounces.
//
//	Inputs:
//	- oscSource: Choose either internal 2MHz oscillator or an external signal 
//		applied to the OSCIO pin.
//		- INTERNAL_CLOCK and EXTERNAL_CLOCK are defined in the header file.
//			Use those.
//		- This value defaults to internal.
//	- oscPinFunction: Allows you to set OSCIO as an input or output.
//		- You can use Arduino's INPUT, OUTPUT defines for this value
//		- This value defaults to input
//	- oscFreqOut: If oscio is configured as an output, this will set the output 
//		frequency
//		- This should be a 4-bit value. 0=0%, 0xF=100%, else 
//			fOSCOut = FOSC / (2^(RegClock[3:0]-1))
//		- This value defaults to 0.
//	- oscDivider: Sets the clock divider in REG_MISC.
//		- ClkX = fOSC / (2^(RegMisc[6:4] -1))
//		- This value defaults to 1.
// -----------------------------------------------------------------------------
	void configClock(byte oscSource = 2, byte oscPinFunction = 0, 
					 byte oscFreqOut = 0, byte oscDivider = 1);
};

#endif	// SX1509_library_H
