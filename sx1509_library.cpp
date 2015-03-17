/* 
	sx1509_library.cpp
		Code file for the SX1509 Arduino library.
	
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
	
	See the header file (sx1509_library.h) for detailed descriptions of each of 
	the sx1509Class methods.
	
	For example uses of these functions, see the Arduino example codes in the
	./examples/ folder.
*/

#include <Wire.h>
#include "Arduino.h"
#include "sx1509_library.h"
#include "sx1509_registers.h"

sx1509Class::sx1509Class(byte address, byte resetPin, byte interruptPin, byte oscillatorPin)
{
	// Store the received parameters into member variables
	deviceAddress =  (uint8_t)address;
	pinInterrupt = interruptPin;
	pinOscillator = oscillatorPin;
	pinReset = resetPin;
}

byte sx1509Class::init(void)
{
	if (pinInterrupt != 255)
	{
		pinMode(pinInterrupt, INPUT_PULLUP);
	}
	
	// Begin I2C
	Wire.begin();
	
	// If the reset pin is connected
	if (pinReset != 255)
		reset(1);
	else
		reset(0);
	
	// Communication test. We'll read from two registers with different
	// default values to verify communication.
	unsigned int testRegisters = 0;
	testRegisters = readWord(REG_INTERRUPT_MASK_A);	// This should return 0xFF00
	// Then read a byte that should be 0x00
	if (testRegisters == 0xFF00)
		return 1;
	else
		return 0;
}

void sx1509Class::reset(bool hardware)
{
	// if hardware bool is set
	if (hardware)
	{
		// Check if bit 2 of REG_MISC is set
		// if so nReset will not issue a POR, we'll need to clear that bit first
		byte regMisc = readByte(REG_MISC);
		if (regMisc & (1<<2))
		{
			regMisc &= ~(1<<2);
			writeByte(REG_MISC, regMisc);
		}
		// Reset the SX1509, the pin is active low
		pinMode(pinReset, OUTPUT);	// set reset pin as output
		digitalWrite(pinReset, LOW);	// pull reset pin low
		delay(1);	// Wait for the pin to settle
		digitalWrite(pinReset, HIGH);	// pull reset pin back high
	}
	else
	{
		// Software reset command sequence:
		writeByte(REG_RESET, 0x12);
		writeByte(REG_RESET, 0x34);
	}
}

void sx1509Class::pinDir(byte pin, byte inOut)
{
	unsigned int tempRegDir = readWord(REG_DIR_B);
	// The SX1509 RegDir registers: REG_DIR_B, REG_DIR_A
	//	0: IO is configured as an output
	//	1: IO is configured as an input
	// Flip inOut, in arduino.h INPUT = 0, OUTPUT = 1
	if (!inOut)	tempRegDir |= (1<<pin);
	else		tempRegDir &= ~(1<<pin);
	
	writeWord(REG_DIR_B, tempRegDir);
}


void sx1509Class::rawWritePin(byte pin, byte highLow)
{
		if (highLow)	
            writeWord(REG_DATA_B, readWord(REG_DATA_B) | (1 <<pin));
		else
		    writeWord(REG_DATA_B, readWord(REG_DATA_B) & ~(1<<pin));
}



void sx1509Class::writePin(byte pin, byte highLow)
{
	
	if ((0xFFFF^readWord(REG_DIR_B))&(1<<pin))	// If the pin is an output, write high/low
	{
		unsigned int tempRegData = readWord(REG_DATA_B);
		if (highLow)	tempRegData |= (1<<pin);
		else			tempRegData &= ~(1<<pin);
		writeWord(REG_DATA_B, tempRegData);
	}
	else	// Otherwise the pin is an input, pull-up/down
	{
		unsigned int tempPullUp = readWord(REG_PULL_UP_B);
		unsigned int tempPullDown = readWord(REG_PULL_DOWN_B);
		
		if (highLow)	// if HIGH, do pull-up, disable pull-down
		{
			tempPullUp |= (1<<pin);
			tempPullDown &= ~(1<<pin);
			writeWord(REG_PULL_UP_B, tempPullUp);
			writeWord(REG_PULL_DOWN_B, tempPullDown);
		}
		else	// If LOW do pull-down, disable pull-up
		{
			tempPullDown |= (1<<pin);
			tempPullUp &= ~(1<<pin);
			writeWord(REG_PULL_UP_B, tempPullUp);
			writeWord(REG_PULL_DOWN_B, tempPullDown);
		}
	}
}

byte sx1509Class::rawReadPin(byte pin)
{
		return (readWord(REG_DATA_B) & (1<<pin));
}



void sx1509Class::fetchPinStates() {
    pinData = readWord(REG_DATA_B);  
    
}
void sx1509Class::sendPinStates() {
   writeWord(REG_DATA_B,pinData);  
    
}



byte sx1509Class::readPrefetchedPin(byte pin) {
   return (pinData & (1 << pin)); 
}

void sx1509Class::updatePinState(byte pin, byte highLow)
{
		if (highLow)	
                    pinData =  pinData | (1 <<pin);
		else
		    pinData = pinData & ~(1<<pin);
}




byte sx1509Class::readPin(byte pin)
{
	
	if (readWord(REG_DIR_B) & (1<<pin))	// If the pin is an input
	{
		unsigned int tempRegData = readWord(REG_DATA_B);
		if (tempRegData & (1<<pin))
			return 1;
	}
	
	return 0;
}





void sx1509Class::ledDriverInit(byte pin, byte freq, bool log)
{
	unsigned int tempWord;
	byte tempByte;
	// Disable input buffer
	// Writing a 1 to the pin bit will disable that pins input buffer
	tempWord = readWord(REG_INPUT_DISABLE_B);
	tempWord |= (1<<pin);
	writeWord(REG_INPUT_DISABLE_B, tempWord);
	
	// Disable pull-up
	// Writing a 0 to the pin bit will disable that pull-up resistor
	tempWord = readWord(REG_PULL_UP_B);
	tempWord &= ~(1<<pin);
	writeWord(REG_PULL_UP_B, tempWord);
	
	// Enable open-drain
	// Writing a 1 to the pin bit will enable open drain on that pin
	tempWord = readWord(REG_OPEN_DRAIN_B);
	tempWord |= (1<<pin);
	writeWord(REG_OPEN_DRAIN_B, tempWord);
	
	// Set direction to output (REG_DIR_B)
	pinDir(pin, OUTPUT);
	
	// Enable oscillator (REG_CLOCK)
	tempByte = readByte(REG_CLOCK);
	tempByte |= (1<<6);	// Internal 2MHz oscillator part 1 (set bit 6)
	tempByte &= ~(1<<5);	// Internal 2MHz oscillator part 2 (clear bit 5)
	writeByte(REG_CLOCK, tempByte);
	
	// Configure LED driver clock and mode (REG_MISC)
	tempByte = readByte(REG_MISC);
	if (log)
	{
		tempByte |= (1<<7);	// set logarithmic mode bank B
		tempByte |= (1<<3);	// set logarithmic mode bank A
	}
	else
	{
		tempByte &= ~(1<<7);	// set linear mode bank B
		tempByte &= ~(1<<3);	// set linear mode bank A
	}
	if (freq == 0)	// don't want it to be 0, that'll disable all led drivers
		freq = 1;
	freq = (freq & 0x07) << 4;	// freq should only be 3 bits from 6:4
	tempByte |= freq;
	writeByte(REG_MISC, tempByte);
	
	// Enable LED driver operation (REG_LED_DRIVER_ENABLE)
	tempWord = readWord(REG_LED_DRIVER_ENABLE_B);
	tempWord |= (1<<pin);
	writeWord(REG_LED_DRIVER_ENABLE_B, tempWord);
	
	// Set REG_DATA bit low ~ LED driver started
	tempWord = readWord(REG_DATA_B);
	tempWord &= ~(1<<pin);
	writeWord(REG_DATA_B, tempWord);
}

void sx1509Class::pwm(byte pin, byte iOn)
{
	// Write the on intensity of pin
	// Linear mode: Ion = iOn
	// Log mode: Ion = f(iOn)
	writeByte(REG_I_ON[pin], iOn);
}

void sx1509Class::blink(byte pin, byte tOn, byte tOff, 
						byte offIntensity, byte onIntensity,
						byte tRise, byte tFall)
{
	// Keep parameters within their limits:
	tOn &= 0x1F;	// tOn should be a 5-bit value
	tOff &= 0x1F;	// tOff should be a 5-bit value
	offIntensity &= 0x07;
	// Write the time on
	// 1-15:  TON = 64 * tOn * (255/ClkX)
	// 16-31: TON = 512 * tOn * (255/ClkX)
	writeByte(REG_T_ON[pin], tOn);
	
	
	// Write the time/intensity off register
	// 1-15:  TOFF = 64 * tOff * (255/ClkX)
	// 16-31: TOFF = 512 * tOff * (255/ClkX)
	// linear Mode - IOff = 4 * offIntensity
	// log mode - Ioff = f(4 * offIntensity)
	writeByte(REG_OFF[pin], (tOff<<3) | offIntensity);
	
	// Write the on intensity:
	writeByte(REG_I_ON[pin], onIntensity);
	
	// Prepare tRise and tFall
	tRise &= 0x1F;	// tRise is a 5-bit value
	tFall &= 0x1F;	// tFall is a 5-bit value
	
	
	// Write regTRise
	// 0: Off
	// 1-15:  TRise =      (regIOn - (4 * offIntensity)) * tRise * (255/ClkX)
	// 16-31: TRise = 16 * (regIOn - (4 * offIntensity)) * tRise * (255/ClkX)
	if (REG_T_RISE[pin] != 0xFF)
		writeByte(REG_T_RISE[pin], tRise);
	// Write regTFall
	// 0: off
	// 1-15:  TFall =      (regIOn - (4 * offIntensity)) * tFall * (255/ClkX)
	// 16-31: TFall = 16 * (regIOn - (4 * offIntensity)) * tFall * (255/ClkX)
	if (REG_T_FALL[pin] != 0xFF)
		writeByte(REG_T_FALL[pin], tFall);
}

void sx1509Class::keypad(byte rows, byte columns, byte sleepTime, byte scanTime)
{
	unsigned int tempWord;
	byte tempByte;
	
	// Set regDir 0:7 outputs, 8:15 inputs:
	tempWord = readWord(REG_DIR_B);
	for (int i=0; i<rows; i++)
		tempWord &= ~(1<<i);
	for (int i=8; i<(columns * 2); i++)
		tempWord |= (1<<i);
	writeWord(REG_DIR_B, tempWord);
	
	// Set regOpenDrain on 0:7:
	tempByte = readByte(REG_OPEN_DRAIN_A);
	for (int i=0; i<rows; i++)
		tempByte |= (1<<i);
	writeByte(REG_OPEN_DRAIN_A, tempByte);
	
	// Set regPullUp on 8:15:
	tempByte = readByte(REG_PULL_UP_B);
	for (int i=0; i<columns; i++)
		tempByte |= (1<<i);
	writeByte(REG_PULL_UP_B, tempByte);
	
	// Enable and configure debouncing on 8:15:
	tempByte = readByte(REG_DEBOUNCE_ENABLE_B);
	for (int i=0; i<columns; i++)
		tempByte |= (1<<i);
	writeByte(REG_DEBOUNCE_ENABLE_B, tempByte);
	writeByte(REG_DEBOUNCE_CONFIG, (scanTime & 0b111));	// Debounce must be less than scan time
	
	// RegKeyConfig1 sets the auto sleep time and scan time per row
	sleepTime = (sleepTime & 0b111)<<4;	
	scanTime &= 0b111;	// Scan time is bits 2:0
	tempByte = sleepTime | scanTime;
	writeByte(REG_KEY_CONFIG_1, tempByte);
	
	// RegKeyConfig2 tells the SX1509 how many rows and columns we've got going
	rows = (rows - 1) & 0b111;	// 0 = off, 0b001 = 2 rows, 0b111 = 8 rows, etc.
	columns = (columns - 1) & 0b111;	// 0b000 = 1 column, ob111 = 8 columns, etc.
	writeByte(REG_KEY_CONFIG_2, (rows << 3) | columns);
}

unsigned int sx1509Class::readKeyData()
{
	return (0xFFFF ^ readWord(REG_KEY_DATA_1));
}

void sx1509Class::sync(void)
{
	// First check if nReset functionality is set
	byte regMisc = readByte(REG_MISC);
	if (!(regMisc & 0x04))
	{
		regMisc |= (1<<2);
		writeByte(REG_MISC, regMisc);
	}
	
	// Toggle nReset pin to sync LED timers
	pinMode(pinReset, OUTPUT);	// set reset pin as output
	digitalWrite(pinReset, LOW);	// pull reset pin low
	delay(1);	// Wait for the pin to settle
	digitalWrite(pinReset, HIGH);	// pull reset pin back high	
	
	// Return nReset to POR functionality
	writeByte(REG_MISC, (regMisc & ~(1<<2)));
}

void sx1509Class::debounceConfig(byte configValue)
{
	// First make sure clock is configured
	byte tempByte = readByte(REG_MISC);
	if ((tempByte & 0x70) == 0)
	{
		tempByte |= (1<<4);	// Just default to no divider if not set
		writeByte(REG_MISC, tempByte);
	}
	tempByte = readByte(REG_CLOCK);
	if ((tempByte & 0x60) == 0)
	{
		tempByte |= (1<<6);	// default to internal osc.
		writeByte(REG_CLOCK, tempByte);
	}
	
	configValue &= 0b111;	// 3-bit value
	writeByte(REG_DEBOUNCE_CONFIG, configValue);
}

void sx1509Class::debounceEnable(byte pin)
{
    if (pin<8) { 
	    unsigned int debounceEnable = readWord(REG_DEBOUNCE_ENABLE_A);
	    debounceEnable |= (1<<pin);
	    writeWord(REG_DEBOUNCE_ENABLE_A, debounceEnable);
    } else {
	    unsigned int debounceEnable = readWord(REG_DEBOUNCE_ENABLE_B);
	    debounceEnable |= (1<<pin);
	    writeWord(REG_DEBOUNCE_ENABLE_B, debounceEnable);
    }
}

void sx1509Class::enableInterrupt(byte pin, byte riseFall)
{
	// Set REG_INTERRUPT_MASK
	unsigned int tempWord = readWord(REG_INTERRUPT_MASK_B);
	tempWord &= ~(1<<pin);	// 0 = event on IO will trigger interrupt
	writeWord(REG_INTERRUPT_MASK_B, tempWord);
	
	byte sensitivity = 0;
	switch (riseFall)
	{
	case CHANGE:
		sensitivity = 0b11;
		break;
	case FALLING:
		sensitivity = 0b10;
		break;
	case RISING:
		sensitivity = 0b01;
		break;
	}
	
	// Set REG_SENSE_XXX
	// Sensitivity is set as follows:
	// 00: None
	// 01: Rising
	// 10: Falling
	// 11: Both
	byte pinMask = (pin & 0x07) * 2;
	byte senseRegister;
	
	// Need to select between two words. One for bank A, one for B.
	if (pin >= 8)	senseRegister = REG_SENSE_HIGH_B;
	else			senseRegister = REG_SENSE_HIGH_A;
	
	tempWord = readWord(senseRegister);
	tempWord &= ~(0b11<<pinMask);	// Mask out the bits we want to write
	tempWord |= (sensitivity<<pinMask);	// Add our new bits
	writeWord(senseRegister, tempWord);
}

unsigned int sx1509Class::interruptSource(void)
{
	unsigned int intSource = readWord(REG_INTERRUPT_SOURCE_B);
	writeWord(REG_INTERRUPT_SOURCE_B, 0xFFFF);	// Clear interrupts
	return intSource;
}

void sx1509Class::configClock(byte oscSource, byte oscPinFunction, byte oscFreqOut, byte oscDivider)
{
	// RegClock constructed as follows:
	//	6:5 - Oscillator frequency souce
	//		00: off, 01: external input, 10: internal 2MHz, 1: reserved
	//	4 - OSCIO pin function
	//		0: input, 1 ouptut
	//	3:0 - Frequency of oscout pin
	//		0: LOW, 0xF: high, else fOSCOUT = FoSC/(2^(RegClock[3:0]-1))
	oscSource = (oscSource & 0b11)<<5;		// 2-bit value, bits 6:5
	oscPinFunction = (oscPinFunction & 1)<<4;	// 1-bit value bit 4
	oscFreqOut = (oscFreqOut & 0b1111);	// 4-bit value, bits 3:0
	byte regClock = oscSource | oscPinFunction | oscFreqOut;
	writeByte(REG_CLOCK, regClock);
	
	// Config RegMisc[6:4] with oscDivider
	// 0: off, else ClkX = fOSC / (2^(RegMisc[6:4] -1))
	oscDivider = (oscDivider & 0b111)<<4;	// 3-bit value, bits 6:4
	byte regMisc = readByte(REG_MISC);
	regMisc &= ~(0b111<<4);
	regMisc |= oscDivider;
	writeByte(REG_MISC, regMisc);
}

// readByte(byte registerAddress)
//	This function reads a single byte located at the registerAddress register.
//	- deviceAddress should already be set by the constructor.
//	- Return value is the byte read from registerAddress
//		- Currently returns 0 if communication has timed out
byte sx1509Class::readByte(byte registerAddress)
{
	unsigned int timeout = RECEIVE_TIMEOUT_VALUE;

	Wire.beginTransmission(deviceAddress);
	Wire.write(registerAddress);
	Wire.endTransmission();
	Wire.requestFrom(deviceAddress, (byte) 1);

	while ((Wire.available() < 1) && (timeout != 0))
		timeout--;
		
	if (timeout == 0)
		return 0;

	return Wire.read();

}

// readWord(byte registerAddress)
//	This function will read a two-byte word beginning at registerAddress
//	- A 16-bit unsigned int will be returned.
//		- The msb of the return value will contain the value read from registerAddress
//		- The lsb of the return value will contain the value read from registerAddress + 1
unsigned int sx1509Class::readWord(byte registerAddress)
{
	unsigned int timeout = RECEIVE_TIMEOUT_VALUE * 2;

	Wire.beginTransmission(deviceAddress);
	Wire.write(registerAddress);
	Wire.endTransmission(true);
	Wire.requestFrom(deviceAddress, (uint8_t) 2, (uint8_t)true);

	while ((Wire.available() < 2) && (timeout != 0))
		timeout--;
		
	if (timeout == 0)
		return 0;
	
	return  ((Wire.read() & 0x00FF) << 8 | (Wire.read() & 0x00FF));
}

// readBytes(byte firstRegisterAddress, byte * destination, byte length)
//	This function reads a series of bytes incrementing from a given address
//	- firstRegsiterAddress is the first address to be read
//	- destination is an array of bytes where the read values will be stored into
//	- length is the number of bytes to be read
//	- No return value.
void sx1509Class::readBytes(byte firstRegisterAddress, byte * destination, byte length)
{
	byte readValue;

	Wire.beginTransmission(deviceAddress);
	Wire.write(firstRegisterAddress);
	Wire.endTransmission();
	Wire.requestFrom(deviceAddress, length);
	
	while (Wire.available() < length)
		;
	
	for (int i=0; i<length; i++)
	{
		destination[i] = Wire.read();
	}
}

// writeByte(byte registerAddress, byte writeValue)
//	This function writes a single byte to a single register on the SX509.
//	- writeValue is written to registerAddress
//	- deviceAddres should already be set from the constructor
//	- No return value.
void sx1509Class::writeByte(byte registerAddress, byte writeValue)
{
	Wire.beginTransmission(deviceAddress);
	Wire.write(registerAddress);
	Wire.write(writeValue);
	Wire.endTransmission();
}

// writeWord(byte registerAddress, ungisnged int writeValue)
//	This function writes a two-byte word to registerAddress and registerAddress + 1
//	- the upper byte of writeValue is written to registerAddress
//		- the lower byte of writeValue is written to registerAddress + 1
//	- No return value.
void sx1509Class::writeWord(byte registerAddress, unsigned int writeValue)
{
	Wire.beginTransmission(deviceAddress);
	Wire.write(registerAddress);
	Wire.write((writeValue & 0xFF00) >> 8);
	Wire.write(writeValue & 0x00FF);
	Wire.endTransmission();	
}

// writeBytes(byte firstRegisterAddress, byte * writeArray, byte length)
//	This function writes an array of bytes, beggining at a specific adddress
//	- firstRegisterAddress is the initial register to be written.
//		- All writes following will be at incremental register addresses.
//	- writeArray should be an array of byte values to be written.
//	- length should be the number of bytes to be written.
//	- no return value.
void sx1509Class::writeBytes(byte firstRegisterAddress, byte * writeArray, byte length)
{
	Wire.beginTransmission(deviceAddress);
	Wire.write(firstRegisterAddress);
	for (int i=0; i<length; i++)
	{
		Wire.write(writeArray[i]);
	}
	Wire.endTransmission();
}
