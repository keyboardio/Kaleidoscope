// -*- mode: c++ -*-

#include <stdint.h>  // for uint16_t, uint8_t

#include "pins_and_ports.h"  // IWYU pragma: keep
#include <avr/wdt.h>

extern "C" {
#include "adns5050.h"
}

#define BOOTLOADER_RESET_KEY 0xB007B007
static uint32_t reset_key __attribute__((section(".noinit")));

/*
 * This function runs before main(), and jumps to the bootloader after a reset
 * initiated by .resetDevice().
 */
static void _bootloader_jump_after_watchdog_reset()
    __attribute__((used, naked, section(".init3")));
static void _bootloader_jump_after_watchdog_reset() {
  if ((MCUSR & (1 << WDRF)) && reset_key == BOOTLOADER_RESET_KEY) {
    reset_key = 0;

    ((void (*)(void))(((FLASHEND + 1L) - 4096) >> 1))();
  }
}

void rebootBootloader() {
  reset_key = BOOTLOADER_RESET_KEY;
  wdt_enable(WDTO_250MS);
  while (true) {}  // This infinite loop ensures nothing else
  // happens before the watchdog reboots us
}

static constexpr uint8_t direct_pins[5] = {PIN_D4, PIN_D2, PIN_E6, PIN_B6, PIN_D7};
static constexpr uint8_t unused_pins[12] = {PIN_B5, PIN_C7, PIN_D0, PIN_D1, PIN_D3, PIN_D5, PIN_D6, PIN_F1, PIN_F3, PIN_F5, PIN_F6, PIN_F7};

void mcuSetup() {
  wdt_disable();

  // disable jtag
  MCUCR |= (1 << JTD);
  MCUCR |= (1 << JTD);

  // disable clock division
  CLKPR = (1 << CLKPCE);
  CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);
}

void buttonSetup() {
  // Initialize keyscanner

  for (uint8_t i = 0; i < sizeof(unused_pins); i++) {
    DDR_OUTPUT(unused_pins[i]);
    DISABLE_PULLUP(unused_pins[i]);
  }

  for (uint8_t i = 0; i < sizeof(direct_pins); i++) {
    DDR_INPUT(direct_pins[i]);
    ENABLE_PULLUP(direct_pins[i]);
  }
}

void setup() {
  mcuSetup();
  Serial.begin(9600);
  buttonSetup();

  adns5050_init();
  adns5050_sync();

  while (!Serial);

  uint8_t pid = adns5050_read_reg(0x00);
  uint8_t rid = adns5050_read_reg(0x01);
  uint8_t pid2 = adns5050_read_reg(0x3e);

  Serial.print("pid=");
  Serial.print(pid, HEX);
  Serial.print("; rid=");
  Serial.print(rid, HEX);
  Serial.print("; pid2=");
  Serial.println(pid2);
}

void handleSerial() {
  if (Serial.available() == 0) return;

  uint8_t b = Serial.read();

  if (b == 'r') rebootBootloader();

  Serial.flush();
}

void loop() {
  handleSerial();

  //report_adns5050_t r = adns505

  delay(250);
}
