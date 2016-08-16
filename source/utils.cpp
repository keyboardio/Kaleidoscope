#include "utils.h"

void reboot_bootloader() {
    // Set the magic bits to get a Caterina-based device
    // to reboot into the bootloader and stay there, rather
    // than run move onward
    //
    // These values are the same as those defined in
    // Caterina.c

    uint16_t bootKey = 0x7777;
    uint16_t *const bootKeyPtr = (uint16_t *)0x0800;

    // Stash the magic key
    *bootKeyPtr = bootKey;

    // Set a watchdog timer
    wdt_enable(WDTO_120MS);

    while (1) {} // This infinite loop ensures nothing else
    // happens before the watchdog reboots us
}

