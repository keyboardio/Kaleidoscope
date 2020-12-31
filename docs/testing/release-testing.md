# Release testing

Before a new release of Kaleidoscope, the following test process should be run through on all supported operating systems.

Always run all of the [automated tests](automated-testing.md) to verify there are no regressions.

(As of August 2017, this whole thing really applies to Model01-Firmware, but we hope to generalize it to Kaleidoscope)

# Tested operating systems

* The latest stable Ubuntu Linux release running X11. (We _should_ eventually be testing both X11 and Wayland)
* The latest stable release of macOS
* An older Mac OS X release TBD. (There were major USB stack changes in 10.9 or so)
* Windows 10
* Windows 7
* The current release of ChromeOS
* A currentish android tablet that supports USB Host
* an iOS device (once we fix the usb connection issue to limit power draw)

# Test process

## Basic testing
1. Plug the keyboard in
1. Make sure the host OS doesn't throw an error
1. Make sure the LED in the top left doesn't glow red
1. Make sure the LED in the top-right corner of the left side breathes blue for ~10s
1. Bring up some sort of notepad app or text editor

## Basic testing, part 2

1. Test typing of shifted and unshifted letters and numbers with and without key repeat
1. Test typing of fn-shifted characters: []{}|\ with and without key repeat
1. Test that 'Any' key generates a random letter or number and that key repeat works
1. Test fn-hjkl to move the cursor
1. Test Fn-WASD to move the mouse
1. Test Fn-RFV for the three mouse buttons
1. Test Fn-BGTabEsc for mouse warp
1. Test that LeftFn+RightFn + hjkl move the cursor
1. Verify that leftfn+rightfn do not light up the numpad

## NKRO

1. Open the platform's native key event viewer
   (If not available, visit https://www.microsoft.com/appliedsciences/KeyboardGhostingDemo.mspx in a browser)
1. Press as many keys as your fingers will let you
1. Verify that the keymap reports all the keys you're pressing


## Test media keys

1. Fn-Any: previous track
1. Fn-Y: next-track
1. Fn-Enter: play/pause
1. Fn-Butterfly: Windows 'menu' key
1. Fn-n: mute
1. Fn-m: volume down
1. Fn-,: volume up

## Test numlock

1. Tap "Num"
1. Verify that the numpad lights up red
1. Verify that the num key is breathing blue
1. Verify that numpad keys generate numbers
1. Tap the Num key
1. Verify that the numpad keys stop being lit up
1 Verify that 'jkl' don't generate numbers.

## Test LED Effects 

1. Tap the LED key
1. Verify that there is a rainbow effect
1. Tap the LED key a few more times and verify that other LED effects show up
1. Verify that you can still type.

## Second connection
1. Unplug the keyboard
1. Plug the keyboard back in
1. Make sure you can still type

## Programming
1. If the OS has a way to show serial port devices, verify that the keyboard's serial port shows up.
1. If you can run stty, as you can on linux and macos, make sure you can tickle the serial port at 1200 bps.
    Linux: stty -F /dev/ttyACM0 1200
    Mac: 
1. If you tickle the serial port without holding down the prog key, verify that the Prog key does not light up red
1. If you hold down the prog key before tickling the serial port, verify that the Prog key's LED lights up red.
1. Unplug the keyboard
1. While holding down prog, plug the keyboard in
1. Verify that the prog key is glowing red.
1. Unplug the keyboard
1. Plug the keyboard in
1. Verify that the prog key is not glowing red.

# If the current platform supports the Arduino IDE (Win/Lin/Mac)
1. use the Arduino IDE to reflash the current version of the software.
1. Verify that you can type a few keys
1. Verify that the LED key toggles between LED effects

