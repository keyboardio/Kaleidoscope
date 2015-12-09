Keyboardio USB HID Driver
=========================

Based on Nico Hood's HID-Project. Please don't contact Nico with issues in this library.


This project went through a lot of phases and has now reached a new Arduino USB-Core
with a lot of new functions like extended HID. It also supports HoodLoader1+2.
The idea is to enable enhanced USB functions to almost all 'standard' Arduino boards.
This is done with slightly different methods for different boards.

###The new 2.1 is ready to test here, need feedback:
https://github.com/NicoHood/HID/tree/Official-Integration

**Arduino IDE 1.5.8 is required for the HID Project.**

**Supported HID devices:**

* Keyboard with Leds out (modifiers + 6 keys pressed at the same time)
* Mouse (5 buttons, move, wheel)
* Media Keys (4 keys for music player, webbrowser and more)
* System Key (for PC standby/shutdown)
* 1 Gamepad (32 buttons, 4 16bit axis, 2 8bit axis, 2 D-Pads)

**Supported Arduinos:**
* Uno
* Mega
* Leonardo
* (Pro)Micro
* Any other 8u2/16u/at90usb162/32u2/32u4 compatible board

| Leonardo/Micro             | Uno/Mega HoodLoader2        | Uno/Mega HoodLoader1                  |
|:---------------------------|:----------------------------|---------------------------------------|
| Extended Arduino USB-Core  | Same Core as Leonardo/Micro | Coded with Lufa                       |
| More HID devices           | Fully reprogrammable 16u2   | No reprogrammable 16u2                |
| Keyboard Led Out report    | Serial0 fully usable        | Main MCU sends HID reports on Serial0 |
| System Wakeup fix (soon)   |                             | Serial Protocol filters HID reports   |
| Serial Control Line access |                             | Built-in ISP function                 |

The HID project contains HID APIs to generate HID reports and functions to send them to the USB Host.
The API (syntax/usage) of the HID Project is always the same for each solution, **you can port code from one device to another very easy**.

* On a Leonardo this function is extended and improved to get more HID devices + some improvements.
* [HoodLoader2](https://github.com/NicoHood/HoodLoader2) is a BootLoader for the 16u2 that let's you use it as standalone MCU with the same USB-Core.
* [HoodLoader1](https://github.com/NicoHood/HoodLoader) is a 16u2 firmware that filters special HW Serial signals from the main MCU and sends HID signals to the USB Host.

**Supported Arduinos (IDE 1.6.6 or higher!):**
* Uno (with [HoodLoader2](https://github.com/NicoHood/HoodLoader2))
* Mega (with [HoodLoader2](https://github.com/NicoHood/HoodLoader2))
* Leonardo
* (Pro)Micro
* Any other 8u2/16u2/at90usb8/162/32u2/32u4 compatible board
* No SAM/ARM support (Due, Zero etc)

**Supported HID devices:**
* Keyboard with Leds out (8 modifiers + 6 keys pressed at the same time, + 1 limited linux consumer key)
* NKRO Keyboard with Leds out (press up to 113 keys at the same time)
* Mouse (5 buttons, move, wheel)
* BootKeyboard/BootMouse BIOS protocol support
* Absolute Mouse
* Consumer/Media Keys (4 keys for music player, web browser and more)
* System Key (for PC standby/shutdown)
* Gamepad (32 buttons, 4 16bit axis, 2 8bit axis, 2 D-Pads)
* RawHID
* Each device is available as single or multi report device (except RawHID)

See the [wiki](https://github.com/NicoHood/HID/wiki/Features) for more information about features etc.

Wiki
====

All documentation moved to the [wiki page](https://github.com/NicoHood/HID/wiki).

An offline snapshot is available in [releases](https://github.com/NicoHood/HID/releases).


