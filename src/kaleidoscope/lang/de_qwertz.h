#pragma once

// Alt gr
#define ALTGR RALT

/* German Umlaute */
#define Key_de_SZ Key_Minus
#define Key_de_AE Key_Quote
#define Key_de_UE Key_LeftBracket
#define Key_de_OE Key_Semicolon

/* These are deviating from the us layout */
#define Key_de_Circumflex Key_Backtick /* ^ and ° */
#define Key_de_Circle LSHIFT(Key_de_Circumflex) /* ° */

#define Key_de_ExlamationMark LSHIFT(Key_de_1)
#define Key_de_DoubleQuote LSHIFT(Key_de_2)
#define Key_de_Paragraph LSHIFT(Key_de_3)
#define Key_de_Dollar LSHIFT(Key_de_4)
#define Key_de_Percent LSHIFT(Key_de_5)
#define Key_de_Ampersand LSHIFT(Key_de_6)
#define Key_de_Slash LSHIFT(Key_de_7)
#define Key_de_LeftParen LSHIFT(Key_de_8)
#define Key_de_RightParen LSHIFT(Key_de_9)
#define Key_de_Equals LSHIFT(Key_de_0)
#define Key_de_QuestionMark LSHIFT(Key_de_SZ)
#define Key_de_LeftCurly ALTGR(Key_de_7)
#define Key_de_RightCurly ALTGR(Key_de_0)
#define Key_de_LeftBracket ALTGR(Key_de_8)
#define Key_de_RightBracket ALTGR(Key_de_9)
#define Key_de_Backslash ALTGR(Key_de_SZ)

#define Key_de_Tick Key_Equals /* Tick ´ and Backtick ` */
#define Key_de_Backtick LSHIFT(Key_de_Tick)

#define Key_de_At ALTGR(Key_de_Q)
#define Key_de_Euro ALTGR(Key_de_E)

#define Key_de_Plus Key_RightBracket /* Plus + Asterisk * and Tilde ~ */
#define Key_de_Asterisk LSHIFT(Key_de_Plus) /* * */
#define Key_de_Tilde ALTGR(Key_de_Plus) /* ~ */

#define Key_de_Hash Key_Backslash /* # and ' */
#define Key_de_Quote LSHIFT(Key_de_Hash)

#define Key_de_LessThan Key_NonUsBackslashAndPipe /* < and > and | */
#define Key_de_GreaterThan LSHIFT(Key_de_LessThan)
#define Key_de_Pipe ALTGR(Key_de_LessThan)

#define Key_de_Minus Key_Slash /* - and _ */
#define Key_de_Underscore LSHIFT(Key_de_Minus)

/* the default keys are just aliased to have them look the same */
#define Key_de_NoEvent Key_NoEvent
#define Key_de_ErrorRollover Key_ErrorRollover
#define Key_de_PostFail Key_PostFail
#define Key_de_ErrorUndefined Key_ErrorUndefined
#define Key_de_A Key_A
#define Key_de_B Key_B
#define Key_de_C Key_C
#define Key_de_D Key_D
#define Key_de_E Key_E
#define Key_de_F Key_F
#define Key_de_G Key_G
#define Key_de_H Key_H
#define Key_de_I Key_I
#define Key_de_J Key_J
#define Key_de_K Key_K
#define Key_de_L Key_L
#define Key_de_M Key_M
#define Key_de_N Key_N
#define Key_de_O Key_O
#define Key_de_P Key_P
#define Key_de_Q Key_Q
#define Key_de_R Key_R
#define Key_de_S Key_S
#define Key_de_T Key_T
#define Key_de_U Key_U
#define Key_de_V Key_V
#define Key_de_W Key_W
#define Key_de_X Key_X

/* These two are inverted in the german layout */
#define Key_de_Y Key_Z
#define Key_de_Z Key_Y

#define Key_de_1 Key_1
#define Key_de_2 Key_2
#define Key_de_3 Key_3
#define Key_de_4 Key_4
#define Key_de_5 Key_5
#define Key_de_6 Key_6
#define Key_de_7 Key_7
#define Key_de_8 Key_8
#define Key_de_9 Key_9
#define Key_de_0 Key_0
#define Key_de_Enter Key_Enter
#define Key_de_Escape Key_Escape
#define Key_de_Backspace Key_Backspace
#define Key_de_Tab Key_Tab
#define Key_de_Spacebar Key_Spacebar
#define Key_de_Comma Key_Comma
#define Key_de_Semicolon LSHIFT(Key_de_Comma)
#define Key_de_Period Key_Period
#define Key_de_Colon LSHIFT(Key_de_Period)

#define Key_de_CapsLock Key_CapsLock
#define Key_de_F1 Key_F1
#define Key_de_F2 Key_F2
#define Key_de_F3 Key_F3
#define Key_de_F4 Key_F4
#define Key_de_F5 Key_F5
#define Key_de_F6 Key_F6
#define Key_de_F7 Key_F7
#define Key_de_F8 Key_F8
#define Key_de_F9 Key_F9
#define Key_de_F10 Key_F10
#define Key_de_F11 Key_F11
#define Key_de_F12 Key_F12

#define Key_de_PrintScreen Key_PrintScreen
#define Key_de_ScrollLock Key_ScrollLock
#define Key_de_Pause Key_Pause
#define Key_de_Insert Key_Insert
#define Key_de_Home Key_Home
#define Key_de_PageUp Key_PageUp
#define Key_de_Delete Key_Delete
#define Key_de_End Key_End
#define Key_de_PageDown Key_PageDown
#define Key_de_RightArrow Key_RightArrow
#define Key_de_LeftArrow Key_LeftArrow
#define Key_de_DownArrow Key_DownArrow
#define Key_de_UpArrow Key_UpArrow

#define Key_de_KeypadNumLock Key_KeypadNumLock
#define Key_de_KeypadDivide Key_KeypadDivide
#define Key_de_KeypadMultiply Key_KeypadMultiply
#define Key_de_KeypadSubtract Key_KeypadSubtract
#define Key_de_KeypadAdd Key_KeypadAdd
#define Key_de_KeypadEnter Key_KeypadEnter
#define Key_de_Keypad1 Key_Keypad1
#define Key_de_Keypad2 Key_Keypad2
#define Key_de_Keypad3 Key_Keypad3
#define Key_de_Keypad4 Key_Keypad4
#define Key_de_Keypad5 Key_Keypad5
#define Key_de_Keypad6 Key_Keypad6
#define Key_de_Keypad7 Key_Keypad7
#define Key_de_Keypad8 Key_Keypad8
#define Key_de_Keypad9 Key_Keypad9
#define Key_de_Keypad0 Key_Keypad0
#define Key_de_KeypadDot Key_KeypadDot

#define Key_de_PcApplication Key_PcApplication
#define Key_de_Power Key_Power

#define Key_de_KeypadEquals Key_KeypadEquals

#define Key_de_F13 Key_F13
#define Key_de_F14 Key_F14
#define Key_de_F15 Key_F15
#define Key_de_F16 Key_F16
#define Key_de_F17 Key_F17
#define Key_de_F18 Key_F18
#define Key_de_F19 Key_F19
#define Key_de_F20 Key_F20
#define Key_de_F21 Key_F21
#define Key_de_F22 Key_F22
#define Key_de_F23 Key_F23
#define Key_de_F24 Key_F24

#define Key_de_Execute Key_Execute
#define Key_de_Help Key_Help
#define Key_de_Menu Key_Menu
#define Key_de_Select Key_Select
#define Key_de_Stop Key_Stop
#define Key_de_Again Key_Again
#define Key_de_Undo Key_Undo
#define Key_de_Cut Key_Cut
#define Key_de_Copy Key_Copy
#define Key_de_Paste Key_Paste
#define Key_de_Find Key_Find

#define Key_de_Mute Key_Mute
#define Key_de_VolumeUp Key_VolumeUp
#define Key_de_VolumeDown Key_VolumeDown

#define Key_de_LockingCapsLock Key_LockingCapsLock
#define Key_de_LockingNumLock Key_LockingNumLock
#define Key_de_LockingScrollLock Key_LockingScrollLock

#define Key_de_KeypadComma Key_KeypadComma
#define Key_de_KeypadEqualSign Key_KeypadEqualSign

#define Key_de_International1 Key_International1
#define Key_de_International2 Key_International2
#define Key_de_International3 Key_International3
#define Key_de_International4 Key_International4
#define Key_de_International5 Key_International5
#define Key_de_International6 Key_International6
#define Key_de_International7 Key_International7
#define Key_de_International8 Key_International8
#define Key_de_International9 Key_International9
#define Key_de_Lang1 Key_Lang1
#define Key_de_Lang2 Key_Lang2
#define Key_de_Lang3 Key_Lang3
#define Key_de_Lang4 Key_Lang4
#define Key_de_Lang5 Key_Lang5
#define Key_de_Lang6 Key_Lang6
#define Key_de_Lang7 Key_Lang7
#define Key_de_Lang8 Key_Lang8
#define Key_de_Lang9 Key_Lang9

#define Key_de_AlternateErase Key_AlternateErase
#define Key_de_Sysreq Key_Sysreq
#define Key_de_Cancel Key_Cancel
#define Key_de_Clear Key_Clear
#define Key_de_Prior Key_Prior
#define Key_de_Return Key_Return
#define Key_de_Separator Key_Separator
#define Key_de_Out Key_Out
#define Key_de_Oper Key_Oper
#define Key_de_ClearSlashAgain Key_ClearSlashAgain
#define Key_de_CrselSlashProps Key_CrselSlashProps
#define Key_de_Exsel Key_Exsel

// Reserved	0xA5-AF
#define Key_de_Keypad00 Key_Keypad00
#define Key_de_Keypad000 Key_Keypad000
#define Key_de_ThousandsSeparator Key_ThousandsSeparator
#define Key_de_DecimalSeparator Key_DecimalSeparator
#define Key_de_CurrencyUnit Key_CurrencyUnit
#define Key_de_CurrencySubunit Key_CurrencySubunit
#define Key_de_KeypadLeftParen Key_KeypadLeftParen
#define Key_de_KeypadRightParen Key_KeypadRightParen
#define Key_de_KeypadLeftCurlyBrace Key_KeypadLeftCurlyBrace
#define Key_de_KeypadRightCurlyBrace Key_KeypadRightCurlyBrace
#define Key_de_KeypadTab Key_KeypadTab
#define Key_de_KeypadBackspace Key_KeypadBackspace
#define Key_de_KeypadA Key_KeypadA
#define Key_de_KeypadB Key_KeypadB
#define Key_de_KeypadC Key_KeypadC
#define Key_de_KeypadD Key_KeypadD
#define Key_de_KeypadE Key_KeypadE
#define Key_de_KeypadF Key_KeypadF
#define Key_de_KeypadXor Key_KeypadXor
#define Key_de_KeypadCarat Key_KeypadCarat
#define Key_de_KeypadPercent Key_KeypadPercent
#define Key_de_KeypadLessThan Key_KeypadLessThan
#define Key_de_KeypadGreaterThan Key_KeypadGreaterThan
#define Key_de_KeypadAmpersand Key_KeypadAmpersand
#define Key_de_KeypadDoubleampersand Key_KeypadDoubleampersand
#define Key_de_KeypadPipe Key_KeypadPipe
#define Key_de_KeypadDoublepipe Key_KeypadDoublepipe
#define Key_de_KeypadColon Key_KeypadColon
#define Key_de_KeypadPoundSign Key_KeypadPoundSign
#define Key_de_KeypadSpace Key_KeypadSpace
#define Key_de_KeypadAtSign Key_KeypadAtSign
#define Key_de_KeypadExclamationPoint Key_KeypadExclamationPoint
#define Key_de_KeypadMemoryStore Key_KeypadMemoryStore
#define Key_de_KeypadMemoryRecall Key_KeypadMemoryRecall
#define Key_de_KeypadMemoryClear Key_KeypadMemoryClear
#define Key_de_KeypadMemoryAdd Key_KeypadMemoryAdd
#define Key_de_KeypadMemorySubtract Key_KeypadMemorySubtract
#define Key_de_KeypadMemoryMultiply Key_KeypadMemoryMultiply
#define Key_de_KeypadMemoryDivide Key_KeypadMemoryDivide
#define Key_de_KeypadPlusSlashMinus Key_KeypadPlusSlashMinus
#define Key_de_KeypadClear Key_KeypadClear
#define Key_de_KeypadClearEntry Key_KeypadClearEntry
#define Key_de_KeypadBinary Key_KeypadBinary
#define Key_de_KeypadOctal Key_KeypadOctal
#define Key_de_KeypadDecimal Key_KeypadDecimal
#define Key_de_KeypadHexadecimal Key_KeypadHexadecimal

#define Key_de_LeftControl Key_LeftControl
#define Key_de_LeftShift Key_LeftShift
#define Key_de_LeftAlt Key_LeftAlt
#define Key_de_LeftGui Key_LeftGui
#define Key_de_RightControl Key_RightControl
#define Key_de_RightShift Key_RightShift
#define Key_de_RightAlt Key_RightAlt
#define Key_de_RightGui Key_RightGui
