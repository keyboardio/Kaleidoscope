/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2019  Keyboard.io, Inc.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "kaleidoscope/host_keymap/linux.h"

// This file was autogenerated by host_keymap.
// Its content is based on information provided by the XKB X11 system
// collected with the extraordinary libxkbcommon.

// layout: gr
// variant: sun_type6

#define _HOST_KEYMAP_ASCII(OP) \
   OP('\b', Key_Backspace) /* XKB_KEY_BackSpace, Back space, back char  */ \
   OP('\t', Key_Tab) /* XKB_KEY_Tab */ \
   OP('\r', Key_Enter) /* XKB_KEY_Return, Return, enter  */ \
   OP('\e', Key_Escape) /* XKB_KEY_Escape */ \
   OP(' ', Key_Spacebar) /* U+0020, XKB_KEY_space, SPACE */ \
   OP('!', LSHIFT(Key_1)) /* U+0021, XKB_KEY_exclam, EXCLAMATION MARK */ \
   OP('"', LSHIFT(Key_Quote)) /* U+0022, XKB_KEY_quotedbl, QUOTATION MARK */ \
   OP('#', LSHIFT(Key_3)) /* U+0023, XKB_KEY_numbersign, NUMBER SIGN */ \
   OP('$', LSHIFT(Key_4)) /* U+0024, XKB_KEY_dollar, DOLLAR SIGN */ \
   OP('%', LSHIFT(Key_5)) /* U+0025, XKB_KEY_percent, PERCENT SIGN */ \
   OP('&', LSHIFT(Key_7)) /* U+0026, XKB_KEY_ampersand, AMPERSAND */ \
   OP('\'', Key_Quote) /* U+0027, XKB_KEY_apostrophe, APOSTROPHE */ \
   OP('(', LSHIFT(Key_9)) /* U+0028, XKB_KEY_parenleft, LEFT PARENTHESIS */ \
   OP(')', LSHIFT(Key_0)) /* U+0029, XKB_KEY_parenright, RIGHT PARENTHESIS */ \
   OP('*', Key_KeypadMultiply) /* XKB_KEY_KP_Multiply */ \
   OP('+', Key_KeypadAdd) /* XKB_KEY_KP_Add */ \
   OP(',', Key_Comma) /* U+002c, XKB_KEY_comma, COMMA */ \
   OP('-', Key_Minus) /* U+002d, XKB_KEY_minus, HYPHEN-MINUS */ \
   OP('.', Key_Period) /* U+002e, XKB_KEY_period, FULL STOP */ \
   OP('/', Key_Slash) /* U+002f, XKB_KEY_slash, SOLIDUS */ \
   OP('0', Key_0) /* U+0030, XKB_KEY_0, DIGIT ZERO */ \
   OP('1', Key_1) /* U+0031, XKB_KEY_1, DIGIT ONE */ \
   OP('2', Key_2) /* U+0032, XKB_KEY_2, DIGIT TWO */ \
   OP('3', Key_3) /* U+0033, XKB_KEY_3, DIGIT THREE */ \
   OP('4', Key_4) /* U+0034, XKB_KEY_4, DIGIT FOUR */ \
   OP('5', Key_5) /* U+0035, XKB_KEY_5, DIGIT FIVE */ \
   OP('6', Key_6) /* U+0036, XKB_KEY_6, DIGIT SIX */ \
   OP('7', Key_7) /* U+0037, XKB_KEY_7, DIGIT SEVEN */ \
   OP('8', Key_8) /* U+0038, XKB_KEY_8, DIGIT EIGHT */ \
   OP('9', Key_9) /* U+0039, XKB_KEY_9, DIGIT NINE */ \
   OP(':', LSHIFT(Key_Q)) /* U+003a, XKB_KEY_colon, COLON */ \
   OP(';', Key_Q) /* U+003b, XKB_KEY_semicolon, SEMICOLON */ \
   OP('<', LSHIFT(Key_Comma)) /* U+003c, XKB_KEY_less, LESS-THAN SIGN */ \
   OP('=', Key_Equals) /* U+003d, XKB_KEY_equal, EQUALS SIGN */ \
   OP('>', LSHIFT(Key_Period)) /* U+003e, XKB_KEY_greater, GREATER-THAN SIGN */ \
   OP('?', LSHIFT(Key_Slash)) /* U+003f, XKB_KEY_question, QUESTION MARK */ \
   OP('@', LSHIFT(Key_2)) /* U+0040, XKB_KEY_at, COMMERCIAL AT */ \
   OP('[', Key_LeftBracket) /* U+005b, XKB_KEY_bracketleft, LEFT SQUARE BRACKET */ \
   OP('\\', Key_NonUsPound) /* U+005c, XKB_KEY_backslash, REVERSE SOLIDUS */ \
   OP(']', Key_RightBracket) /* U+005d, XKB_KEY_bracketright, RIGHT SQUARE BRACKET */ \
   OP('^', LSHIFT(Key_6)) /* U+005e, XKB_KEY_asciicircum, CIRCUMFLEX ACCENT */ \
   OP('_', LSHIFT(Key_Minus)) /* U+005f, XKB_KEY_underscore, LOW LINE */ \
   OP('`', Key_Backtick) /* U+0060, XKB_KEY_grave, GRAVE ACCENT */ \
   OP('{', LSHIFT(Key_LeftBracket)) /* U+007b, XKB_KEY_braceleft, LEFT CURLY BRACKET */ \
   OP('|', LSHIFT(Key_NonUsPound)) /* U+007c, XKB_KEY_bar, VERTICAL LINE */ \
   OP('}', LSHIFT(Key_RightBracket)) /* U+007d, XKB_KEY_braceright, RIGHT CURLY BRACKET */ \
   OP('~', LSHIFT(Key_Backtick)) /* U+007e, XKB_KEY_asciitilde, TILDE */ \
   OP('', Key_Delete) /* XKB_KEY_Delete, Delete, rubout  */ \

#define _HOST_KEYMAP_UNICODE(OP) \
   OP(L'\b', Key_Backspace) /* XKB_KEY_BackSpace, Back space, back char  */ \
   OP(L'\t', Key_Tab) /* XKB_KEY_Tab */ \
   OP(L'\r', Key_Enter) /* XKB_KEY_Return, Return, enter  */ \
   OP(L'\e', Key_Escape) /* XKB_KEY_Escape */ \
   OP(L' ', Key_Spacebar) /* U+0020, XKB_KEY_space, SPACE */ \
   OP(L'!', LSHIFT(Key_1)) /* U+0021, XKB_KEY_exclam, EXCLAMATION MARK */ \
   OP(L'"', LSHIFT(Key_Quote)) /* U+0022, XKB_KEY_quotedbl, QUOTATION MARK */ \
   OP(L'#', LSHIFT(Key_3)) /* U+0023, XKB_KEY_numbersign, NUMBER SIGN */ \
   OP(L'$', LSHIFT(Key_4)) /* U+0024, XKB_KEY_dollar, DOLLAR SIGN */ \
   OP(L'%', LSHIFT(Key_5)) /* U+0025, XKB_KEY_percent, PERCENT SIGN */ \
   OP(L'&', LSHIFT(Key_7)) /* U+0026, XKB_KEY_ampersand, AMPERSAND */ \
   OP(L'\'', Key_Quote) /* U+0027, XKB_KEY_apostrophe, APOSTROPHE */ \
   OP(L'(', LSHIFT(Key_9)) /* U+0028, XKB_KEY_parenleft, LEFT PARENTHESIS */ \
   OP(L')', LSHIFT(Key_0)) /* U+0029, XKB_KEY_parenright, RIGHT PARENTHESIS */ \
   OP(L'*', Key_KeypadMultiply) /* XKB_KEY_KP_Multiply */ \
   OP(L'+', Key_KeypadAdd) /* XKB_KEY_KP_Add */ \
   OP(L',', Key_Comma) /* U+002c, XKB_KEY_comma, COMMA */ \
   OP(L'-', Key_Minus) /* U+002d, XKB_KEY_minus, HYPHEN-MINUS */ \
   OP(L'.', Key_Period) /* U+002e, XKB_KEY_period, FULL STOP */ \
   OP(L'/', Key_Slash) /* U+002f, XKB_KEY_slash, SOLIDUS */ \
   OP(L'0', Key_0) /* U+0030, XKB_KEY_0, DIGIT ZERO */ \
   OP(L'1', Key_1) /* U+0031, XKB_KEY_1, DIGIT ONE */ \
   OP(L'2', Key_2) /* U+0032, XKB_KEY_2, DIGIT TWO */ \
   OP(L'3', Key_3) /* U+0033, XKB_KEY_3, DIGIT THREE */ \
   OP(L'4', Key_4) /* U+0034, XKB_KEY_4, DIGIT FOUR */ \
   OP(L'5', Key_5) /* U+0035, XKB_KEY_5, DIGIT FIVE */ \
   OP(L'6', Key_6) /* U+0036, XKB_KEY_6, DIGIT SIX */ \
   OP(L'7', Key_7) /* U+0037, XKB_KEY_7, DIGIT SEVEN */ \
   OP(L'8', Key_8) /* U+0038, XKB_KEY_8, DIGIT EIGHT */ \
   OP(L'9', Key_9) /* U+0039, XKB_KEY_9, DIGIT NINE */ \
   OP(L':', LSHIFT(Key_Q)) /* U+003a, XKB_KEY_colon, COLON */ \
   OP(L';', Key_Q) /* U+003b, XKB_KEY_semicolon, SEMICOLON */ \
   OP(L'<', LSHIFT(Key_Comma)) /* U+003c, XKB_KEY_less, LESS-THAN SIGN */ \
   OP(L'=', Key_Equals) /* U+003d, XKB_KEY_equal, EQUALS SIGN */ \
   OP(L'>', LSHIFT(Key_Period)) /* U+003e, XKB_KEY_greater, GREATER-THAN SIGN */ \
   OP(L'?', LSHIFT(Key_Slash)) /* U+003f, XKB_KEY_question, QUESTION MARK */ \
   OP(L'@', LSHIFT(Key_2)) /* U+0040, XKB_KEY_at, COMMERCIAL AT */ \
   OP(L'[', Key_LeftBracket) /* U+005b, XKB_KEY_bracketleft, LEFT SQUARE BRACKET */ \
   OP(L'\\', Key_NonUsPound) /* U+005c, XKB_KEY_backslash, REVERSE SOLIDUS */ \
   OP(L']', Key_RightBracket) /* U+005d, XKB_KEY_bracketright, RIGHT SQUARE BRACKET */ \
   OP(L'^', LSHIFT(Key_6)) /* U+005e, XKB_KEY_asciicircum, CIRCUMFLEX ACCENT */ \
   OP(L'_', LSHIFT(Key_Minus)) /* U+005f, XKB_KEY_underscore, LOW LINE */ \
   OP(L'`', Key_Backtick) /* U+0060, XKB_KEY_grave, GRAVE ACCENT */ \
   OP(L'{', LSHIFT(Key_LeftBracket)) /* U+007b, XKB_KEY_braceleft, LEFT CURLY BRACKET */ \
   OP(L'|', LSHIFT(Key_NonUsPound)) /* U+007c, XKB_KEY_bar, VERTICAL LINE */ \
   OP(L'}', LSHIFT(Key_RightBracket)) /* U+007d, XKB_KEY_braceright, RIGHT CURLY BRACKET */ \
   OP(L'~', LSHIFT(Key_Backtick)) /* U+007e, XKB_KEY_asciitilde, TILDE */ \
   OP(L'', Key_Delete) /* XKB_KEY_Delete, Delete, rubout  */ \
   OP(L'£', RALT(Key_3)) /* U+00a3, XKB_KEY_sterling, POUND SIGN */ \
   OP(L'¦', RALT(Key_NonUsPound)) /* U+00a6, XKB_KEY_brokenbar, BROKEN BAR */ \
   OP(L'©', RALT(Key_C)) /* U+00a9, XKB_KEY_copyright, COPYRIGHT SIGN */ \
   OP(L'«', Key_NonUsBackslashAndPipe) /* U+00ab, XKB_KEY_guillemotleft, LEFT-POINTING DOUBLE ANGLE QUOTATION MARK */ \
   OP(L'¬', RALT(Key_6)) /* U+00ac, XKB_KEY_notsign, NOT SIGN */ \
   OP(L'®', RALT(Key_R)) /* U+00ae, XKB_KEY_registered, REGISTERED SIGN */ \
   OP(L'°', RALT(Key_0)) /* U+00b0, XKB_KEY_degree, DEGREE SIGN */ \
   OP(L'±', RALT(Key_Minus)) /* U+00b1, XKB_KEY_plusminus, PLUS-MINUS SIGN */ \
   OP(L'·', RALT(Key_Q)) /* U+00b7, XKB_KEY_periodcentered, MIDDLE DOT */ \
   OP(L'»', RALT(Key_RightBracket)) /* U+00bb, XKB_KEY_guillemotright, RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK */ \
   OP(L'½', RALT(Key_Backtick)) /* U+00bd, XKB_KEY_onehalf, VULGAR FRACTION ONE HALF */ \
   OP(L'Α', LSHIFT(Key_A)) /* U+0391, XKB_KEY_Greek_ALPHA, GREEK CAPITAL LETTER ALPHA */ \
   OP(L'Β', LSHIFT(Key_B)) /* U+0392, XKB_KEY_Greek_BETA, GREEK CAPITAL LETTER BETA */ \
   OP(L'Γ', LSHIFT(Key_G)) /* U+0393, XKB_KEY_Greek_GAMMA, GREEK CAPITAL LETTER GAMMA */ \
   OP(L'Δ', LSHIFT(Key_D)) /* U+0394, XKB_KEY_Greek_DELTA, GREEK CAPITAL LETTER DELTA */ \
   OP(L'Ε', LSHIFT(Key_E)) /* U+0395, XKB_KEY_Greek_EPSILON, GREEK CAPITAL LETTER EPSILON */ \
   OP(L'Ζ', LSHIFT(Key_Z)) /* U+0396, XKB_KEY_Greek_ZETA, GREEK CAPITAL LETTER ZETA */ \
   OP(L'Η', LSHIFT(Key_H)) /* U+0397, XKB_KEY_Greek_ETA, GREEK CAPITAL LETTER ETA */ \
   OP(L'Θ', LSHIFT(Key_U)) /* U+0398, XKB_KEY_Greek_THETA, GREEK CAPITAL LETTER THETA */ \
   OP(L'Ι', LSHIFT(Key_I)) /* U+0399, XKB_KEY_Greek_IOTA, GREEK CAPITAL LETTER IOTA */ \
   OP(L'Κ', LSHIFT(Key_K)) /* U+039a, XKB_KEY_Greek_KAPPA, GREEK CAPITAL LETTER KAPPA */ \
   OP(L'Λ', LSHIFT(Key_L)) /* U+039b, XKB_KEY_Greek_LAMDA, GREEK CAPITAL LETTER LAMDA */ \
   OP(L'Μ', LSHIFT(Key_M)) /* U+039c, XKB_KEY_Greek_MU, GREEK CAPITAL LETTER MU */ \
   OP(L'Ν', LSHIFT(Key_N)) /* U+039d, XKB_KEY_Greek_NU, GREEK CAPITAL LETTER NU */ \
   OP(L'Ξ', LSHIFT(Key_J)) /* U+039e, XKB_KEY_Greek_XI, GREEK CAPITAL LETTER XI */ \
   OP(L'Ο', LSHIFT(Key_O)) /* U+039f, XKB_KEY_Greek_OMICRON, GREEK CAPITAL LETTER OMICRON */ \
   OP(L'Π', LSHIFT(Key_P)) /* U+03a0, XKB_KEY_Greek_PI, GREEK CAPITAL LETTER PI */ \
   OP(L'Ρ', LSHIFT(Key_R)) /* U+03a1, XKB_KEY_Greek_RHO, GREEK CAPITAL LETTER RHO */ \
   OP(L'Σ', RALT(Key_W)) /* U+03a3, XKB_KEY_Greek_SIGMA, GREEK CAPITAL LETTER SIGMA */ \
   OP(L'Τ', LSHIFT(Key_T)) /* U+03a4, XKB_KEY_Greek_TAU, GREEK CAPITAL LETTER TAU */ \
   OP(L'Υ', LSHIFT(Key_Y)) /* U+03a5, XKB_KEY_Greek_UPSILON, GREEK CAPITAL LETTER UPSILON */ \
   OP(L'Φ', LSHIFT(Key_F)) /* U+03a6, XKB_KEY_Greek_PHI, GREEK CAPITAL LETTER PHI */ \
   OP(L'Χ', LSHIFT(Key_X)) /* U+03a7, XKB_KEY_Greek_CHI, GREEK CAPITAL LETTER CHI */ \
   OP(L'Ψ', LSHIFT(Key_C)) /* U+03a8, XKB_KEY_Greek_PSI, GREEK CAPITAL LETTER PSI */ \
   OP(L'Ω', LSHIFT(Key_V)) /* U+03a9, XKB_KEY_Greek_OMEGA, GREEK CAPITAL LETTER OMEGA */ \
   OP(L'α', Key_A) /* U+03b1, XKB_KEY_Greek_alpha, GREEK SMALL LETTER ALPHA */ \
   OP(L'β', Key_B) /* U+03b2, XKB_KEY_Greek_beta, GREEK SMALL LETTER BETA */ \
   OP(L'γ', Key_G) /* U+03b3, XKB_KEY_Greek_gamma, GREEK SMALL LETTER GAMMA */ \
   OP(L'δ', Key_D) /* U+03b4, XKB_KEY_Greek_delta, GREEK SMALL LETTER DELTA */ \
   OP(L'ε', Key_E) /* U+03b5, XKB_KEY_Greek_epsilon, GREEK SMALL LETTER EPSILON */ \
   OP(L'ζ', Key_Z) /* U+03b6, XKB_KEY_Greek_zeta, GREEK SMALL LETTER ZETA */ \
   OP(L'η', Key_H) /* U+03b7, XKB_KEY_Greek_eta, GREEK SMALL LETTER ETA */ \
   OP(L'θ', Key_U) /* U+03b8, XKB_KEY_Greek_theta, GREEK SMALL LETTER THETA */ \
   OP(L'ι', Key_I) /* U+03b9, XKB_KEY_Greek_iota, GREEK SMALL LETTER IOTA */ \
   OP(L'κ', Key_K) /* U+03ba, XKB_KEY_Greek_kappa, GREEK SMALL LETTER KAPPA */ \
   OP(L'λ', Key_L) /* U+03bb, XKB_KEY_Greek_lamda, GREEK SMALL LETTER LAMDA */ \
   OP(L'μ', Key_M) /* U+03bc, XKB_KEY_Greek_mu, GREEK SMALL LETTER MU */ \
   OP(L'ν', Key_N) /* U+03bd, XKB_KEY_Greek_nu, GREEK SMALL LETTER NU */ \
   OP(L'ξ', Key_J) /* U+03be, XKB_KEY_Greek_xi, GREEK SMALL LETTER XI */ \
   OP(L'ο', Key_O) /* U+03bf, XKB_KEY_Greek_omicron, GREEK SMALL LETTER OMICRON */ \
   OP(L'π', Key_P) /* U+03c0, XKB_KEY_Greek_pi, GREEK SMALL LETTER PI */ \
   OP(L'ρ', Key_R) /* U+03c1, XKB_KEY_Greek_rho, GREEK SMALL LETTER RHO */ \
   OP(L'ς', Key_W) /* U+03c2, XKB_KEY_Greek_finalsmallsigma, GREEK SMALL LETTER FINAL SIGMA */ \
   OP(L'σ', Key_S) /* U+03c3, XKB_KEY_Greek_sigma, GREEK SMALL LETTER SIGMA */ \
   OP(L'τ', Key_T) /* U+03c4, XKB_KEY_Greek_tau, GREEK SMALL LETTER TAU */ \
   OP(L'υ', Key_Y) /* U+03c5, XKB_KEY_Greek_upsilon, GREEK SMALL LETTER UPSILON */ \
   OP(L'φ', Key_F) /* U+03c6, XKB_KEY_Greek_phi, GREEK SMALL LETTER PHI */ \
   OP(L'χ', Key_X) /* U+03c7, XKB_KEY_Greek_chi, GREEK SMALL LETTER CHI */ \
   OP(L'ψ', Key_C) /* U+03c8, XKB_KEY_Greek_psi, GREEK SMALL LETTER PSI */ \
   OP(L'ω', Key_V) /* U+03c9, XKB_KEY_Greek_omega, GREEK SMALL LETTER OMEGA */ \
   OP(L'€', RALT(Key_4)) /* U+20ac, XKB_KEY_EuroSign, EURO SIGN */ \

#define _HOST_KEYMAP_NON_PRINTABLE(OP) \
   OP(XKB_KEY_0, Key_0) \
   OP(XKB_KEY_1, Key_1) \
   OP(XKB_KEY_2, Key_2) \
   OP(XKB_KEY_3, Key_3) \
   OP(XKB_KEY_4, Key_4) \
   OP(XKB_KEY_5, Key_5) \
   OP(XKB_KEY_6, Key_6) \
   OP(XKB_KEY_7, Key_7) \
   OP(XKB_KEY_8, Key_8) \
   OP(XKB_KEY_9, Key_9) \
   OP(XKB_KEY_Alt_L, Key_LeftAlt) /* XKB_KEY_Alt_L, Left alt  */ \
   OP(XKB_KEY_BackSpace, Key_Backspace) /* XKB_KEY_BackSpace, Back space, back char  */ \
   OP(XKB_KEY_Break, LCTRL(Key_Pause)) /* XKB_KEY_Break */ \
   OP(XKB_KEY_Caps_Lock, Key_CapsLock) /* XKB_KEY_Caps_Lock, Caps lock  */ \
   OP(XKB_KEY_Control_L, Key_LeftControl) /* XKB_KEY_Control_L, Left control  */ \
   OP(XKB_KEY_Control_R, Key_RightControl) /* XKB_KEY_Control_R, Right control  */ \
   OP(XKB_KEY_Delete, Key_Delete) /* XKB_KEY_Delete, Delete, rubout  */ \
   OP(XKB_KEY_Down, Key_DownArrow) /* XKB_KEY_Down, Move down, down arrow  */ \
   OP(XKB_KEY_End, Key_End) /* XKB_KEY_End, EOL  */ \
   OP(XKB_KEY_Escape, Key_Escape) /* XKB_KEY_Escape */ \
   OP(XKB_KEY_EuroSign, RALT(Key_4)) \
   OP(XKB_KEY_F1, Key_F1) /* XKB_KEY_F1 */ \
   OP(XKB_KEY_F10, Key_F10) /* XKB_KEY_F10 */ \
   OP(XKB_KEY_F11, Key_F11) /* XKB_KEY_F11 */ \
   OP(XKB_KEY_F12, Key_F12) /* XKB_KEY_F12 */ \
   OP(XKB_KEY_F2, Key_F2) /* XKB_KEY_F2 */ \
   OP(XKB_KEY_F3, Key_F3) /* XKB_KEY_F3 */ \
   OP(XKB_KEY_F4, Key_F4) /* XKB_KEY_F4 */ \
   OP(XKB_KEY_F5, Key_F5) /* XKB_KEY_F5 */ \
   OP(XKB_KEY_F6, Key_F6) /* XKB_KEY_F6 */ \
   OP(XKB_KEY_F7, Key_F7) /* XKB_KEY_F7 */ \
   OP(XKB_KEY_F8, Key_F8) /* XKB_KEY_F8 */ \
   OP(XKB_KEY_F9, Key_F9) /* XKB_KEY_F9 */ \
   OP(XKB_KEY_Greek_ALPHA, LSHIFT(Key_A)) \
   OP(XKB_KEY_Greek_BETA, LSHIFT(Key_B)) \
   OP(XKB_KEY_Greek_CHI, LSHIFT(Key_X)) \
   OP(XKB_KEY_Greek_DELTA, LSHIFT(Key_D)) \
   OP(XKB_KEY_Greek_EPSILON, LSHIFT(Key_E)) \
   OP(XKB_KEY_Greek_ETA, LSHIFT(Key_H)) \
   OP(XKB_KEY_Greek_GAMMA, LSHIFT(Key_G)) \
   OP(XKB_KEY_Greek_IOTA, LSHIFT(Key_I)) \
   OP(XKB_KEY_Greek_KAPPA, LSHIFT(Key_K)) \
   OP(XKB_KEY_Greek_LAMDA, LSHIFT(Key_L)) \
   OP(XKB_KEY_Greek_MU, LSHIFT(Key_M)) \
   OP(XKB_KEY_Greek_NU, LSHIFT(Key_N)) \
   OP(XKB_KEY_Greek_OMEGA, LSHIFT(Key_V)) \
   OP(XKB_KEY_Greek_OMICRON, LSHIFT(Key_O)) \
   OP(XKB_KEY_Greek_PHI, LSHIFT(Key_F)) \
   OP(XKB_KEY_Greek_PI, LSHIFT(Key_P)) \
   OP(XKB_KEY_Greek_PSI, LSHIFT(Key_C)) \
   OP(XKB_KEY_Greek_RHO, LSHIFT(Key_R)) \
   OP(XKB_KEY_Greek_SIGMA, RALT(Key_W)) \
   OP(XKB_KEY_Greek_TAU, LSHIFT(Key_T)) \
   OP(XKB_KEY_Greek_THETA, LSHIFT(Key_U)) \
   OP(XKB_KEY_Greek_UPSILON, LSHIFT(Key_Y)) \
   OP(XKB_KEY_Greek_XI, LSHIFT(Key_J)) \
   OP(XKB_KEY_Greek_ZETA, LSHIFT(Key_Z)) \
   OP(XKB_KEY_Greek_alpha, Key_A) \
   OP(XKB_KEY_Greek_beta, Key_B) \
   OP(XKB_KEY_Greek_chi, Key_X) \
   OP(XKB_KEY_Greek_delta, Key_D) \
   OP(XKB_KEY_Greek_epsilon, Key_E) \
   OP(XKB_KEY_Greek_eta, Key_H) \
   OP(XKB_KEY_Greek_finalsmallsigma, Key_W) \
   OP(XKB_KEY_Greek_gamma, Key_G) \
   OP(XKB_KEY_Greek_iota, Key_I) \
   OP(XKB_KEY_Greek_kappa, Key_K) \
   OP(XKB_KEY_Greek_lamda, Key_L) \
   OP(XKB_KEY_Greek_mu, Key_M) \
   OP(XKB_KEY_Greek_nu, Key_N) \
   OP(XKB_KEY_Greek_omega, Key_V) \
   OP(XKB_KEY_Greek_omicron, Key_O) \
   OP(XKB_KEY_Greek_phi, Key_F) \
   OP(XKB_KEY_Greek_pi, Key_P) \
   OP(XKB_KEY_Greek_psi, Key_C) \
   OP(XKB_KEY_Greek_rho, Key_R) \
   OP(XKB_KEY_Greek_sigma, Key_S) \
   OP(XKB_KEY_Greek_tau, Key_T) \
   OP(XKB_KEY_Greek_theta, Key_U) \
   OP(XKB_KEY_Greek_upsilon, Key_Y) \
   OP(XKB_KEY_Greek_xi, Key_J) \
   OP(XKB_KEY_Greek_zeta, Key_Z) \
   OP(XKB_KEY_Home, Key_Home) /* XKB_KEY_Home */ \
   OP(XKB_KEY_ISO_Left_Tab, LSHIFT(Key_Tab)) /* XKB_KEY_ISO_Left_Tab */ \
   OP(XKB_KEY_ISO_Level3_Shift, Key_RightAlt) /* XKB_KEY_ISO_Level3_Shift */ \
   OP(XKB_KEY_Insert, Key_Insert) /* XKB_KEY_Insert, Insert, insert here  */ \
   OP(XKB_KEY_KP_0, LSHIFT(Key_Keypad0)) /* XKB_KEY_KP_0 */ \
   OP(XKB_KEY_KP_1, LSHIFT(Key_Keypad1)) /* XKB_KEY_KP_1 */ \
   OP(XKB_KEY_KP_2, LSHIFT(Key_Keypad2)) /* XKB_KEY_KP_2 */ \
   OP(XKB_KEY_KP_3, LSHIFT(Key_Keypad3)) /* XKB_KEY_KP_3 */ \
   OP(XKB_KEY_KP_4, LSHIFT(Key_Keypad4)) /* XKB_KEY_KP_4 */ \
   OP(XKB_KEY_KP_5, LSHIFT(Key_Keypad5)) /* XKB_KEY_KP_5 */ \
   OP(XKB_KEY_KP_6, LSHIFT(Key_Keypad6)) /* XKB_KEY_KP_6 */ \
   OP(XKB_KEY_KP_7, LSHIFT(Key_Keypad7)) /* XKB_KEY_KP_7 */ \
   OP(XKB_KEY_KP_8, LSHIFT(Key_Keypad8)) /* XKB_KEY_KP_8 */ \
   OP(XKB_KEY_KP_9, LSHIFT(Key_Keypad9)) /* XKB_KEY_KP_9 */ \
   OP(XKB_KEY_KP_Add, Key_KeypadAdd) /* XKB_KEY_KP_Add */ \
   OP(XKB_KEY_KP_Begin, Key_Keypad5) /* XKB_KEY_KP_Begin */ \
   OP(XKB_KEY_KP_Decimal, LSHIFT(Key_KeypadDot)) /* XKB_KEY_KP_Decimal */ \
   OP(XKB_KEY_KP_Delete, Key_KeypadDot) /* XKB_KEY_KP_Delete */ \
   OP(XKB_KEY_KP_Divide, Key_KeypadDivide) /* XKB_KEY_KP_Divide */ \
   OP(XKB_KEY_KP_Down, Key_Keypad2) /* XKB_KEY_KP_Down */ \
   OP(XKB_KEY_KP_End, Key_Keypad1) /* XKB_KEY_KP_End */ \
   OP(XKB_KEY_KP_Enter, Key_KeypadEnter) /* XKB_KEY_KP_Enter, Enter  */ \
   OP(XKB_KEY_KP_Home, Key_Keypad7) /* XKB_KEY_KP_Home */ \
   OP(XKB_KEY_KP_Insert, Key_Keypad0) /* XKB_KEY_KP_Insert */ \
   OP(XKB_KEY_KP_Left, Key_Keypad4) /* XKB_KEY_KP_Left */ \
   OP(XKB_KEY_KP_Multiply, Key_KeypadMultiply) /* XKB_KEY_KP_Multiply */ \
   OP(XKB_KEY_KP_Next, Key_Keypad3) /* XKB_KEY_KP_Next */ \
   OP(XKB_KEY_KP_Prior, Key_Keypad9) /* XKB_KEY_KP_Prior */ \
   OP(XKB_KEY_KP_Right, Key_Keypad6) /* XKB_KEY_KP_Right */ \
   OP(XKB_KEY_KP_Subtract, Key_KeypadSubtract) /* XKB_KEY_KP_Subtract */ \
   OP(XKB_KEY_KP_Up, Key_Keypad8) /* XKB_KEY_KP_Up */ \
   OP(XKB_KEY_Left, Key_LeftArrow) /* XKB_KEY_Left, Move left, left arrow  */ \
   OP(XKB_KEY_Meta_L, LSHIFT(Key_LeftAlt)) /* XKB_KEY_Meta_L, Left meta  */ \
   OP(XKB_KEY_Multi_key, Key_PcApplication) /* XKB_KEY_Multi_key, Multi-key character compose  */ \
   OP(XKB_KEY_Next, Key_PageDown) /* XKB_KEY_Next, Next  */ \
   OP(XKB_KEY_Num_Lock, Key_KeypadNumLock) /* XKB_KEY_Num_Lock */ \
   OP(XKB_KEY_Pause, Key_Pause) /* XKB_KEY_Pause, Pause, hold  */ \
   OP(XKB_KEY_Print, Key_PrintScreen) /* XKB_KEY_Print */ \
   OP(XKB_KEY_Prior, Key_PageUp) /* XKB_KEY_Prior, Prior, previous  */ \
   OP(XKB_KEY_Return, Key_Enter) /* XKB_KEY_Return, Return, enter  */ \
   OP(XKB_KEY_Right, Key_RightArrow) /* XKB_KEY_Right, Move right, right arrow  */ \
   OP(XKB_KEY_Scroll_Lock, Key_ScrollLock) /* XKB_KEY_Scroll_Lock */ \
   OP(XKB_KEY_Shift_L, Key_LeftShift) /* XKB_KEY_Shift_L, Left shift  */ \
   OP(XKB_KEY_Shift_R, Key_RightShift) /* XKB_KEY_Shift_R, Right shift  */ \
   OP(XKB_KEY_Super_L, Key_LeftGui) /* XKB_KEY_Super_L, Left super  */ \
   OP(XKB_KEY_Super_R, Key_RightGui) /* XKB_KEY_Super_R, Right super  */ \
   OP(XKB_KEY_Sys_Req, LALT(Key_PrintScreen)) /* XKB_KEY_Sys_Req */ \
   OP(XKB_KEY_Tab, Key_Tab) /* XKB_KEY_Tab */ \
   OP(XKB_KEY_Up, Key_UpArrow) /* XKB_KEY_Up, Move up, up arrow  */ \
   OP(XKB_KEY_XF86ClearGrab, LCTRL(LALT(Key_KeypadMultiply))) /* XKB_KEY_XF86ClearGrab, kill application with grab  */ \
   OP(XKB_KEY_XF86Next_VMode, LCTRL(LALT(Key_KeypadAdd))) /* XKB_KEY_XF86Next_VMode, next video mode available   */ \
   OP(XKB_KEY_XF86Prev_VMode, LCTRL(LALT(Key_KeypadSubtract))) /* XKB_KEY_XF86Prev_VMode, prev. video mode available  */ \
   OP(XKB_KEY_XF86Switch_VT_1, LCTRL(LALT(Key_F1))) /* XKB_KEY_XF86Switch_VT_1 */ \
   OP(XKB_KEY_XF86Switch_VT_10, LCTRL(LALT(Key_F10))) /* XKB_KEY_XF86Switch_VT_10 */ \
   OP(XKB_KEY_XF86Switch_VT_11, LCTRL(LALT(Key_F11))) /* XKB_KEY_XF86Switch_VT_11 */ \
   OP(XKB_KEY_XF86Switch_VT_12, LCTRL(LALT(Key_F12))) /* XKB_KEY_XF86Switch_VT_12 */ \
   OP(XKB_KEY_XF86Switch_VT_2, LCTRL(LALT(Key_F2))) /* XKB_KEY_XF86Switch_VT_2 */ \
   OP(XKB_KEY_XF86Switch_VT_3, LCTRL(LALT(Key_F3))) /* XKB_KEY_XF86Switch_VT_3 */ \
   OP(XKB_KEY_XF86Switch_VT_4, LCTRL(LALT(Key_F4))) /* XKB_KEY_XF86Switch_VT_4 */ \
   OP(XKB_KEY_XF86Switch_VT_5, LCTRL(LALT(Key_F5))) /* XKB_KEY_XF86Switch_VT_5 */ \
   OP(XKB_KEY_XF86Switch_VT_6, LCTRL(LALT(Key_F6))) /* XKB_KEY_XF86Switch_VT_6 */ \
   OP(XKB_KEY_XF86Switch_VT_7, LCTRL(LALT(Key_F7))) /* XKB_KEY_XF86Switch_VT_7 */ \
   OP(XKB_KEY_XF86Switch_VT_8, LCTRL(LALT(Key_F8))) /* XKB_KEY_XF86Switch_VT_8 */ \
   OP(XKB_KEY_XF86Switch_VT_9, LCTRL(LALT(Key_F9))) /* XKB_KEY_XF86Switch_VT_9 */ \
   OP(XKB_KEY_XF86Ungrab, LCTRL(LALT(Key_KeypadDivide))) /* XKB_KEY_XF86Ungrab, force ungrab                */ \
   OP(XKB_KEY_ampersand, LSHIFT(Key_7)) \
   OP(XKB_KEY_apostrophe, Key_Quote) /* XKB_KEY_quoteright, deprecated  */ \
   OP(XKB_KEY_asciicircum, LSHIFT(Key_6)) \
   OP(XKB_KEY_asciitilde, LSHIFT(Key_Backtick)) \
   OP(XKB_KEY_asterisk, LSHIFT(Key_8)) \
   OP(XKB_KEY_at, LSHIFT(Key_2)) \
   OP(XKB_KEY_backslash, Key_NonUsPound) \
   OP(XKB_KEY_bar, LSHIFT(Key_NonUsPound)) \
   OP(XKB_KEY_braceleft, LSHIFT(Key_LeftBracket)) \
   OP(XKB_KEY_braceright, LSHIFT(Key_RightBracket)) \
   OP(XKB_KEY_bracketleft, Key_LeftBracket) \
   OP(XKB_KEY_bracketright, Key_RightBracket) \
   OP(XKB_KEY_brokenbar, RALT(Key_NonUsPound)) \
   OP(XKB_KEY_colon, LSHIFT(Key_Q)) \
   OP(XKB_KEY_comma, Key_Comma) \
   OP(XKB_KEY_copyright, RALT(Key_C)) \
   OP(XKB_KEY_dead_acute, Key_Semicolon) /* XKB_KEY_dead_acute */ \
   OP(XKB_KEY_dead_diaeresis, LSHIFT(Key_Semicolon)) /* XKB_KEY_dead_diaeresis */ \
   OP(XKB_KEY_degree, RALT(Key_0)) \
   OP(XKB_KEY_dollar, LSHIFT(Key_4)) \
   OP(XKB_KEY_equal, Key_Equals) \
   OP(XKB_KEY_exclam, LSHIFT(Key_1)) \
   OP(XKB_KEY_grave, Key_Backtick) /* XKB_KEY_quoteleft, deprecated  */ \
   OP(XKB_KEY_greater, LSHIFT(Key_Period)) \
   OP(XKB_KEY_guillemotleft, Key_NonUsBackslashAndPipe) \
   OP(XKB_KEY_guillemotright, RALT(Key_RightBracket)) \
   OP(XKB_KEY_less, LSHIFT(Key_Comma)) \
   OP(XKB_KEY_minus, Key_Minus) \
   OP(XKB_KEY_notsign, RALT(Key_6)) \
   OP(XKB_KEY_numbersign, LSHIFT(Key_3)) \
   OP(XKB_KEY_onehalf, RALT(Key_Backtick)) \
   OP(XKB_KEY_parenleft, LSHIFT(Key_9)) \
   OP(XKB_KEY_parenright, LSHIFT(Key_0)) \
   OP(XKB_KEY_percent, LSHIFT(Key_5)) \
   OP(XKB_KEY_period, Key_Period) \
   OP(XKB_KEY_periodcentered, RALT(Key_Q)) \
   OP(XKB_KEY_plus, LSHIFT(Key_Equals)) \
   OP(XKB_KEY_plusminus, RALT(Key_Minus)) \
   OP(XKB_KEY_question, LSHIFT(Key_Slash)) \
   OP(XKB_KEY_quotedbl, LSHIFT(Key_Quote)) \
   OP(XKB_KEY_registered, RALT(Key_R)) \
   OP(XKB_KEY_semicolon, Key_Q) \
   OP(XKB_KEY_slash, Key_Slash) \
   OP(XKB_KEY_space, Key_Spacebar) \
   OP(XKB_KEY_sterling, RALT(Key_3)) \
   OP(XKB_KEY_underscore, LSHIFT(Key_Minus)) \


namespace kaleidoscope {
namespace host_keymap {
namespace linux {
namespace gr {
namespace sun_type6 {

HOST_KEYMAP_LINUX(
  _HOST_KEYMAP_ASCII,
  _HOST_KEYMAP_UNICODE,
  _HOST_KEYMAP_NON_PRINTABLE
)

} // namespace sun_type6
} // namespace gr
} // namespace linux
} // namespace host_keymap
} // namespace kaleidoscope

#undef _HOST_KEYMAP_ASCII
#undef _HOST_KEYMAP_UNICODE
#undef _HOST_KEYMAP_NON_PRINTABLE

