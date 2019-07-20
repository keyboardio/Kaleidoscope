#include "Kaleidoscope-MouseKeys.h"
/*
 * keygroups.h provides functions that can be used at compile-time to evaluate whether a
 * Key constant is in a certain major grouping. This is used by plugins like FunctionalColor
 * to color keys by the group that they are in.
 */ 

//Alphabetical keys A-Z
inline constexpr bool isAlpha(const Key& k) {
  return ( k.flags == KEY_FLAGS && (k.keyCode >= (Key_A).keyCode) && (k.keyCode <= (Key_Z).keyCode) )?true:false;
}

//Number keys 1-9, 0
inline constexpr bool isNumber(const Key& k) {
  return ( k.flags == KEY_FLAGS && (k.keyCode >= (Key_1).keyCode) && (k.keyCode <= (Key_0).keyCode) )?true:false;
}

// enter, escape, delete, tab, spacebar, all have no group

//minus, equals, brackets, backslash, pound, semicolon, quote, grave accent, comma, period, slash
inline constexpr bool isPunctuation(const Key& k) {
  return ( (k.keyCode == (Key_LeftCurlyBracket).keyCode && k.flags == (Key_LeftCurlyBracket).flags) ||
  	(k.keyCode == (Key_RightCurlyBracket).keyCode && k.flags == (Key_RightCurlyBracket).flags) ||
  	(k.keyCode == (Key_LeftParen).keyCode && k.flags == (Key_LeftParen).flags) ||
  	(k.keyCode == (Key_RightParen).keyCode && k.flags == (Key_RightParen).flags) ||
  	(k.keyCode == (Key_Pipe).keyCode && k.flags == (Key_Pipe).flags) ||
  	(k.flags == KEY_FLAGS &&
  	(k.keyCode == (Key_International1).keyCode || //Brazilian forward-slash (/) and question-mark (?) key
  	k.keyCode == (Key_NonUsBackslashAndPipe).keyCode ||
  	((k.keyCode >= (Key_Minus).keyCode) && (k.keyCode <= (Key_Slash).keyCode))) )
  	)?true:false;
}

// capslock has no group
//F1 through F24, which are split into two groups
inline constexpr bool isFunction(const Key& k) {
	return ( k.flags == KEY_FLAGS &&
	  (((k.keyCode >= (Key_F1).keyCode) && (k.keyCode <= (Key_F12).keyCode)) ||
	  ((k.keyCode >= (Key_F13).keyCode) && (k.keyCode <= (Key_F24).keyCode)))
	  )?true:false;
}

// printscreen, scrolllock, pause, insert

//nav cluster: insert, home, end, delete_forward, pageup, pagedown
// I don't know if insert and delete "fit" here, but they do get clustered together on traditional keyboards
inline constexpr bool isNavigation(const Key& k) {
	return (k.flags == KEY_FLAGS && (k.keyCode >= (Key_Insert).keyCode) && (k.keyCode <= (Key_PageDown).keyCode))?true:false;
}

//right, left, down, up
inline constexpr bool isArrow(const Key& k) {
  return (k.flags == KEY_FLAGS && (k.keyCode >= (Key_RightArrow).keyCode) && (k.keyCode <= (Key_UpArrow).keyCode))?true:false;
}

//All keypad keys including numbers, and mathemetical operators
//There's another group of more obscure keypad keys that are unused so they don't interfere with consumer keys
// ((k.keyCode >= (Key_Keypad00).keyCode) && (k.keyCode <= (Key_KeypadHexadecimal).keyCode))
inline constexpr bool isKeypad(const Key& k) {
  return ( k.flags == KEY_FLAGS && (
  	k.keyCode == (Key_KeypadEquals).keyCode ||
  	k.keyCode == (Key_KeypadComma).keyCode ||
  	k.keyCode == (Key_KeypadEqualSign).keyCode ||
  	((k.keyCode >= (Key_KeypadNumLock).keyCode) && (k.keyCode <= (Key_KeypadDot).keyCode)) ||
  	((k.keyCode >= (Key_Keypad00).keyCode) && (k.keyCode <= (Key_KeypadHexadecimal).keyCode))
  	)
  	)?true:false;
}

// System keys like Pause/Break, Print Screen, and scroll lock
inline constexpr bool isSystem(const Key& k) {
  return ( k.flags == KEY_FLAGS && (
  	k.keyCode == (Key_PrintScreen).keyCode ||
  	k.keyCode == (Key_ScrollLock).keyCode ||
  	k.keyCode == (Key_Pause).keyCode
  	)
  	)?true:false;
}

//Media keys used on some keyboards with extra buttons, as well as all the consumer control keys
//includes, execute, help, menu, select, stop, again, undo, cut, copy, paste, find, mute, volup, voldn
// #define HID_CONSUMER_PLAY	0xB0	// HID type OOC
// #define HID_CONSUMER_PAUSE	0xB1	// HID type OOC
// #define HID_CONSUMER_RECORD	0xB2	// HID type OOC
// #define HID_CONSUMER_FAST_FORWARD	0xB3	// HID type OOC
// #define HID_CONSUMER_REWIND	0xB4	// HID type OOC
// #define HID_CONSUMER_SCAN_NEXT_TRACK	0xB5	// HID type OSC
// #define HID_CONSUMER_SCAN_PREVIOUS_TRACK	0xB6	// HID type OSC
// #define HID_CONSUMER_STOP	0xB7	// HID type OSC
// #define HID_CONSUMER_EJECT	0xB8	// HID type OSC
// #define HID_CONSUMER_RANDOM_PLAY	0xB9	// HID type OOC

// #define HID_CONSUMER_PLAY_SLASH_PAUSE	0xCD	// HID type OSC

// //Mute happens to be the same code as left alt. Hmm.
// #define HID_CONSUMER_MUTE	0xE2	// HID type OOC

// #define HID_CONSUMER_VOLUME_INCREMENT	0xE9	// HID type RTC
// #define HID_CONSUMER_VOLUME_DECREMENT	0xEA	// HID type RTC
inline constexpr bool isMedia(const Key& k) {
  return (
  	(k.flags == KEY_FLAGS && (k.keyCode >= (Key_Execute).keyCode) && (k.keyCode <= (Key_VolumeDown).keyCode)) ||
	(k.keyCode == (Consumer_ScanPreviousTrack).keyCode && k.flags == (Consumer_ScanPreviousTrack).flags)||
	(k.keyCode == (Consumer_ScanNextTrack).keyCode && k.flags == (Consumer_ScanNextTrack).flags)||
	(k.keyCode == (Consumer_PlaySlashPause).keyCode && k.flags == (Consumer_PlaySlashPause).flags)||
	(k.keyCode == (Consumer_Play).keyCode && k.flags == (Consumer_Play).flags)||
	(k.keyCode == (Consumer_Pause).keyCode && k.flags == (Consumer_Pause).flags)||
	(k.keyCode == (Consumer_Stop).keyCode && k.flags == (Consumer_Stop).flags)||
	(k.flags == (Consumer_VolumeIncrement).flags && k.keyCode == (Consumer_VolumeIncrement).keyCode) ||
	(k.flags == (Consumer_VolumeDecrement).flags && k.keyCode == (Consumer_VolumeDecrement).keyCode) ||
	(k.keyCode == (Consumer_Mute).keyCode && k.flags == (Consumer_Mute).flags)
  	)?true:false;
}

// Lang keys used primarily for Asian language character set switching
inline constexpr bool isLang(const Key& k) {
	return (k.flags == KEY_FLAGS &&
		(k.keyCode >= (Key_Lang1).keyCode) && (k.keyCode <= (Key_Lang9).keyCode) )?true:false;
}

//Here I will have some groups that have subgroups...
//Control, shift, alt, gui (windows/command), and application (menu) key
inline constexpr bool isModifier(const Key& k) {
	return ( k.flags == KEY_FLAGS &&
		(k.keyCode == (Key_PcApplication).keyCode ||
		((k.keyCode >= (Key_LeftControl).keyCode) && (k.keyCode <= (Key_RightGui).keyCode))) )?true:false;
}

//Keys that are subgroups of Modifier keys
inline constexpr bool isShift(const Key& k) {
	return ( k.flags == KEY_FLAGS &&
		(k.keyCode == (Key_LeftShift).keyCode || (k.keyCode == (Key_RightShift).keyCode) ) )?true:false;
}

inline constexpr bool isControl(const Key& k) {
	return ( k.flags == KEY_FLAGS &&
		(k.keyCode == (Key_LeftControl).keyCode || (k.keyCode == (Key_RightControl).keyCode) ) )?true:false;
}

inline constexpr bool isGui(const Key& k) {
	return ( k.flags == KEY_FLAGS &&
		(k.keyCode == (Key_LeftGui).keyCode || (k.keyCode == (Key_RightGui).keyCode) ) )?true:false;
}

inline constexpr bool isAlt(const Key& k) {
	return ( k.flags == KEY_FLAGS &&
		(k.keyCode == (Key_LeftAlt).keyCode || (k.keyCode == (Key_RightAlt).keyCode) ) )?true:false;
}

// The groups below require Kaleidoscope-MouseKeys.h. Perhaps compiler flags could be used
// to skip this final section if MouseKeys was not included as many users may not need this.
// All mouse keys. This technique seems to get some false positives (prog & led keys)
inline constexpr bool isMouseMove(const Key& k) {
	return ( ((Key_mouseUpL).flags == k.flags && (Key_mouseUpL).keyCode == k.keyCode) ||
		 ((Key_mouseUp).flags == k.flags && (Key_mouseUp).keyCode == k.keyCode) ||
		 ((Key_mouseUpR).flags == k.flags && (Key_mouseUpR).keyCode == k.keyCode) ||
		 ((Key_mouseL).flags == k.flags && (Key_mouseL).keyCode == k.keyCode) ||
		 ((Key_mouseR).flags == k.flags && (Key_mouseR).keyCode == k.keyCode) ||
		 ((Key_mouseDnL).flags == k.flags && (Key_mouseDnL).keyCode == k.keyCode) ||
		 ((Key_mouseDn).flags == k.flags && (Key_mouseDn).keyCode == k.keyCode) ||
		 ((Key_mouseDnR).flags == k.flags && (Key_mouseDnR).keyCode == k.keyCode)
		)?true:false;
}

// Mouse keys that are for the mouse wheel. L & R scroll wheel don't seem to work.
inline constexpr bool isMouseWheel(const Key& k) {
	return ( ((Key_mouseScrollUp).flags == k.flags && (Key_mouseScrollUp).keyCode == k.keyCode) ||
		 ((Key_mouseScrollDn).flags == k.flags && (Key_mouseScrollDn).keyCode == k.keyCode)
		)?true:false;
}

// Mouse keys that are for the mouse buttons
inline constexpr bool isMouseButton(const Key& k) {
	return ( ((Key_mouseBtnL).flags == k.flags && (Key_mouseBtnL).keyCode == k.keyCode) ||
		 ((Key_mouseBtnM).flags == k.flags && (Key_mouseBtnM).keyCode == k.keyCode) ||
		 ((Key_mouseBtnR).flags == k.flags && (Key_mouseBtnR).keyCode == k.keyCode)
		)?true:false;
}

// Mouse keys that warp the mouse pointer
inline constexpr bool isMouseWarp(const Key& k) {
	return ( ((Key_mouseWarpNW).flags == k.flags && (Key_mouseWarpNW).keyCode == k.keyCode) ||
		 ((Key_mouseWarpNE).flags == k.flags && (Key_mouseWarpNE).keyCode == k.keyCode) ||
		 ((Key_mouseWarpSW).flags == k.flags && (Key_mouseWarpSW).keyCode == k.keyCode) ||
		 ((Key_mouseWarpSE).flags == k.flags && (Key_mouseWarpSE).keyCode == k.keyCode) ||
		 ((Key_mouseWarpEnd).flags == k.flags && (Key_mouseWarpEnd).keyCode == k.keyCode)
		)?true:false;
}

inline constexpr bool isMouse(const Key& k) {
	return (isMouseMove(k) || isMouseWheel(k) || isMouseButton(k) || isMouseWarp(k));
}
