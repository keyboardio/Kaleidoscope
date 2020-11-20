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

// layout: cm
// variant: azerty

#define _HOST_KEYMAP_ASCII(OP) \
   OP('\b', Key_Backspace) /* XKB_KEY_BackSpace, Back space, back char  */ \
   OP('\t', Key_Tab) /* XKB_KEY_Tab */ \
   OP('\r', Key_Enter) /* XKB_KEY_Return, Return, enter  */ \
   OP('\e', Key_Escape) /* XKB_KEY_Escape */ \
   OP(' ', Key_Spacebar) /* U+0020, XKB_KEY_space, SPACE */ \
   OP('!', RALT(Key_Slash)) /* U+0021, XKB_KEY_exclam, EXCLAMATION MARK */ \
   OP('"', Key_3) /* U+0022, XKB_KEY_quotedbl, QUOTATION MARK */ \
   OP('#', RALT(LSHIFT(Key_5))) /* U+0023, XKB_KEY_numbersign, NUMBER SIGN */ \
   OP('$', Key_Backtick) /* U+0024, XKB_KEY_dollar, DOLLAR SIGN */ \
   OP('%', RALT(Key_3)) /* U+0025, XKB_KEY_percent, PERCENT SIGN */ \
   OP('&', Key_1) /* U+0026, XKB_KEY_ampersand, AMPERSAND */ \
   OP('\'', Key_4) /* U+0027, XKB_KEY_apostrophe, APOSTROPHE */ \
   OP('(', Key_5) /* U+0028, XKB_KEY_parenleft, LEFT PARENTHESIS */ \
   OP(')', Key_Minus) /* U+0029, XKB_KEY_parenright, RIGHT PARENTHESIS */ \
   OP('*', Key_KeypadMultiply) /* XKB_KEY_KP_Multiply */ \
   OP('+', Key_KeypadAdd) /* XKB_KEY_KP_Add */ \
   OP(',', Key_M) /* U+002c, XKB_KEY_comma, COMMA */ \
   OP('-', Key_6) /* U+002d, XKB_KEY_minus, HYPHEN-MINUS */ \
   OP('.', LSHIFT(Key_Comma)) /* U+002e, XKB_KEY_period, FULL STOP */ \
   OP('/', Key_KeypadDivide) /* XKB_KEY_KP_Divide */ \
   OP('0', LSHIFT(Key_0)) /* XKB_KEY_KP_0 */ \
   OP('1', LSHIFT(Key_1)) /* XKB_KEY_KP_1 */ \
   OP('2', LSHIFT(Key_2)) /* XKB_KEY_KP_2 */ \
   OP('3', LSHIFT(Key_3)) /* XKB_KEY_KP_3 */ \
   OP('4', LSHIFT(Key_4)) /* XKB_KEY_KP_4 */ \
   OP('5', LSHIFT(Key_5)) /* XKB_KEY_KP_5 */ \
   OP('6', LSHIFT(Key_6)) /* XKB_KEY_KP_6 */ \
   OP('7', LSHIFT(Key_7)) /* XKB_KEY_KP_7 */ \
   OP('8', LSHIFT(Key_8)) /* XKB_KEY_KP_8 */ \
   OP('9', LSHIFT(Key_9)) /* XKB_KEY_KP_9 */ \
   OP(':', Key_Period) /* U+003a, XKB_KEY_colon, COLON */ \
   OP(';', Key_Comma) /* U+003b, XKB_KEY_semicolon, SEMICOLON */ \
   OP('<', Key_NonUsBackslashAndPipe) /* U+003c, XKB_KEY_less, LESS-THAN SIGN */ \
   OP('=', Key_Equals) /* U+003d, XKB_KEY_equal, EQUALS SIGN */ \
   OP('>', LSHIFT(Key_NonUsBackslashAndPipe)) /* U+003e, XKB_KEY_greater, GREATER-THAN SIGN */ \
   OP('?', LSHIFT(Key_M)) /* U+003f, XKB_KEY_question, QUESTION MARK */ \
   OP('@', RALT(Key_0)) /* U+0040, XKB_KEY_at, COMMERCIAL AT */ \
   OP('A', LSHIFT(Key_Q)) /* U+0041, XKB_KEY_A, LATIN CAPITAL LETTER A */ \
   OP('B', LSHIFT(Key_B)) /* U+0042, XKB_KEY_B, LATIN CAPITAL LETTER B */ \
   OP('C', LSHIFT(Key_C)) /* U+0043, XKB_KEY_C, LATIN CAPITAL LETTER C */ \
   OP('D', LSHIFT(Key_D)) /* U+0044, XKB_KEY_D, LATIN CAPITAL LETTER D */ \
   OP('E', LSHIFT(Key_E)) /* U+0045, XKB_KEY_E, LATIN CAPITAL LETTER E */ \
   OP('F', LSHIFT(Key_F)) /* U+0046, XKB_KEY_F, LATIN CAPITAL LETTER F */ \
   OP('G', LSHIFT(Key_G)) /* U+0047, XKB_KEY_G, LATIN CAPITAL LETTER G */ \
   OP('H', LSHIFT(Key_H)) /* U+0048, XKB_KEY_H, LATIN CAPITAL LETTER H */ \
   OP('I', LSHIFT(Key_I)) /* U+0049, XKB_KEY_I, LATIN CAPITAL LETTER I */ \
   OP('J', LSHIFT(Key_J)) /* U+004a, XKB_KEY_J, LATIN CAPITAL LETTER J */ \
   OP('K', LSHIFT(Key_K)) /* U+004b, XKB_KEY_K, LATIN CAPITAL LETTER K */ \
   OP('L', LSHIFT(Key_L)) /* U+004c, XKB_KEY_L, LATIN CAPITAL LETTER L */ \
   OP('M', LSHIFT(Key_Semicolon)) /* U+004d, XKB_KEY_M, LATIN CAPITAL LETTER M */ \
   OP('N', LSHIFT(Key_N)) /* U+004e, XKB_KEY_N, LATIN CAPITAL LETTER N */ \
   OP('O', LSHIFT(Key_O)) /* U+004f, XKB_KEY_O, LATIN CAPITAL LETTER O */ \
   OP('P', LSHIFT(Key_P)) /* U+0050, XKB_KEY_P, LATIN CAPITAL LETTER P */ \
   OP('Q', LSHIFT(Key_A)) /* U+0051, XKB_KEY_Q, LATIN CAPITAL LETTER Q */ \
   OP('R', LSHIFT(Key_R)) /* U+0052, XKB_KEY_R, LATIN CAPITAL LETTER R */ \
   OP('S', LSHIFT(Key_S)) /* U+0053, XKB_KEY_S, LATIN CAPITAL LETTER S */ \
   OP('T', LSHIFT(Key_T)) /* U+0054, XKB_KEY_T, LATIN CAPITAL LETTER T */ \
   OP('U', LSHIFT(Key_U)) /* U+0055, XKB_KEY_U, LATIN CAPITAL LETTER U */ \
   OP('V', LSHIFT(Key_V)) /* U+0056, XKB_KEY_V, LATIN CAPITAL LETTER V */ \
   OP('W', LSHIFT(Key_Z)) /* U+0057, XKB_KEY_W, LATIN CAPITAL LETTER W */ \
   OP('X', LSHIFT(Key_X)) /* U+0058, XKB_KEY_X, LATIN CAPITAL LETTER X */ \
   OP('Y', LSHIFT(Key_Y)) /* U+0059, XKB_KEY_Y, LATIN CAPITAL LETTER Y */ \
   OP('Z', LSHIFT(Key_W)) /* U+005a, XKB_KEY_Z, LATIN CAPITAL LETTER Z */ \
   OP('[', RALT(Key_5)) /* U+005b, XKB_KEY_bracketleft, LEFT SQUARE BRACKET */ \
   OP('\\', RALT(Key_8)) /* U+005c, XKB_KEY_backslash, REVERSE SOLIDUS */ \
   OP(']', RALT(Key_Minus)) /* U+005d, XKB_KEY_bracketright, RIGHT SQUARE BRACKET */ \
   OP('^', RALT(Key_9)) /* U+005e, XKB_KEY_asciicircum, CIRCUMFLEX ACCENT */ \
   OP('_', Key_8) /* U+005f, XKB_KEY_underscore, LOW LINE */ \
   OP('a', Key_Q) /* U+0061, XKB_KEY_a, LATIN SMALL LETTER A */ \
   OP('b', Key_B) /* U+0062, XKB_KEY_b, LATIN SMALL LETTER B */ \
   OP('c', Key_C) /* U+0063, XKB_KEY_c, LATIN SMALL LETTER C */ \
   OP('d', Key_D) /* U+0064, XKB_KEY_d, LATIN SMALL LETTER D */ \
   OP('e', Key_E) /* U+0065, XKB_KEY_e, LATIN SMALL LETTER E */ \
   OP('f', Key_F) /* U+0066, XKB_KEY_f, LATIN SMALL LETTER F */ \
   OP('g', Key_G) /* U+0067, XKB_KEY_g, LATIN SMALL LETTER G */ \
   OP('h', Key_H) /* U+0068, XKB_KEY_h, LATIN SMALL LETTER H */ \
   OP('i', Key_I) /* U+0069, XKB_KEY_i, LATIN SMALL LETTER I */ \
   OP('j', Key_J) /* U+006a, XKB_KEY_j, LATIN SMALL LETTER J */ \
   OP('k', Key_K) /* U+006b, XKB_KEY_k, LATIN SMALL LETTER K */ \
   OP('l', Key_L) /* U+006c, XKB_KEY_l, LATIN SMALL LETTER L */ \
   OP('m', Key_Semicolon) /* U+006d, XKB_KEY_m, LATIN SMALL LETTER M */ \
   OP('n', Key_N) /* U+006e, XKB_KEY_n, LATIN SMALL LETTER N */ \
   OP('o', Key_O) /* U+006f, XKB_KEY_o, LATIN SMALL LETTER O */ \
   OP('p', Key_P) /* U+0070, XKB_KEY_p, LATIN SMALL LETTER P */ \
   OP('q', Key_A) /* U+0071, XKB_KEY_q, LATIN SMALL LETTER Q */ \
   OP('r', Key_R) /* U+0072, XKB_KEY_r, LATIN SMALL LETTER R */ \
   OP('s', Key_S) /* U+0073, XKB_KEY_s, LATIN SMALL LETTER S */ \
   OP('t', Key_T) /* U+0074, XKB_KEY_t, LATIN SMALL LETTER T */ \
   OP('u', Key_U) /* U+0075, XKB_KEY_u, LATIN SMALL LETTER U */ \
   OP('v', Key_V) /* U+0076, XKB_KEY_v, LATIN SMALL LETTER V */ \
   OP('w', Key_Z) /* U+0077, XKB_KEY_w, LATIN SMALL LETTER W */ \
   OP('x', Key_X) /* U+0078, XKB_KEY_x, LATIN SMALL LETTER X */ \
   OP('y', Key_Y) /* U+0079, XKB_KEY_y, LATIN SMALL LETTER Y */ \
   OP('z', Key_W) /* U+007a, XKB_KEY_z, LATIN SMALL LETTER Z */ \
   OP('{', RALT(Key_4)) /* U+007b, XKB_KEY_braceleft, LEFT CURLY BRACKET */ \
   OP('|', RALT(Key_6)) /* U+007c, XKB_KEY_bar, VERTICAL LINE */ \
   OP('}', RALT(Key_Equals)) /* U+007d, XKB_KEY_braceright, RIGHT CURLY BRACKET */ \
   OP('~', RALT(Key_2)) /* U+007e, XKB_KEY_asciitilde, TILDE */ \
   OP('', Key_Delete) /* XKB_KEY_Delete, Delete, rubout  */ \

#define _HOST_KEYMAP_UNICODE(OP) \
   OP(L'\b', Key_Backspace) /* XKB_KEY_BackSpace, Back space, back char  */ \
   OP(L'\t', Key_Tab) /* XKB_KEY_Tab */ \
   OP(L'\r', Key_Enter) /* XKB_KEY_Return, Return, enter  */ \
   OP(L'\e', Key_Escape) /* XKB_KEY_Escape */ \
   OP(L' ', Key_Spacebar) /* U+0020, XKB_KEY_space, SPACE */ \
   OP(L'!', RALT(Key_Slash)) /* U+0021, XKB_KEY_exclam, EXCLAMATION MARK */ \
   OP(L'"', Key_3) /* U+0022, XKB_KEY_quotedbl, QUOTATION MARK */ \
   OP(L'#', RALT(LSHIFT(Key_5))) /* U+0023, XKB_KEY_numbersign, NUMBER SIGN */ \
   OP(L'$', Key_Backtick) /* U+0024, XKB_KEY_dollar, DOLLAR SIGN */ \
   OP(L'%', RALT(Key_3)) /* U+0025, XKB_KEY_percent, PERCENT SIGN */ \
   OP(L'&', Key_1) /* U+0026, XKB_KEY_ampersand, AMPERSAND */ \
   OP(L'\'', Key_4) /* U+0027, XKB_KEY_apostrophe, APOSTROPHE */ \
   OP(L'(', Key_5) /* U+0028, XKB_KEY_parenleft, LEFT PARENTHESIS */ \
   OP(L')', Key_Minus) /* U+0029, XKB_KEY_parenright, RIGHT PARENTHESIS */ \
   OP(L'*', Key_KeypadMultiply) /* XKB_KEY_KP_Multiply */ \
   OP(L'+', Key_KeypadAdd) /* XKB_KEY_KP_Add */ \
   OP(L',', Key_M) /* U+002c, XKB_KEY_comma, COMMA */ \
   OP(L'-', Key_6) /* U+002d, XKB_KEY_minus, HYPHEN-MINUS */ \
   OP(L'.', LSHIFT(Key_Comma)) /* U+002e, XKB_KEY_period, FULL STOP */ \
   OP(L'/', Key_KeypadDivide) /* XKB_KEY_KP_Divide */ \
   OP(L'0', LSHIFT(Key_0)) /* XKB_KEY_KP_0 */ \
   OP(L'1', LSHIFT(Key_1)) /* XKB_KEY_KP_1 */ \
   OP(L'2', LSHIFT(Key_2)) /* XKB_KEY_KP_2 */ \
   OP(L'3', LSHIFT(Key_3)) /* XKB_KEY_KP_3 */ \
   OP(L'4', LSHIFT(Key_4)) /* XKB_KEY_KP_4 */ \
   OP(L'5', LSHIFT(Key_5)) /* XKB_KEY_KP_5 */ \
   OP(L'6', LSHIFT(Key_6)) /* XKB_KEY_KP_6 */ \
   OP(L'7', LSHIFT(Key_7)) /* XKB_KEY_KP_7 */ \
   OP(L'8', LSHIFT(Key_8)) /* XKB_KEY_KP_8 */ \
   OP(L'9', LSHIFT(Key_9)) /* XKB_KEY_KP_9 */ \
   OP(L':', Key_Period) /* U+003a, XKB_KEY_colon, COLON */ \
   OP(L';', Key_Comma) /* U+003b, XKB_KEY_semicolon, SEMICOLON */ \
   OP(L'<', Key_NonUsBackslashAndPipe) /* U+003c, XKB_KEY_less, LESS-THAN SIGN */ \
   OP(L'=', Key_Equals) /* U+003d, XKB_KEY_equal, EQUALS SIGN */ \
   OP(L'>', LSHIFT(Key_NonUsBackslashAndPipe)) /* U+003e, XKB_KEY_greater, GREATER-THAN SIGN */ \
   OP(L'?', LSHIFT(Key_M)) /* U+003f, XKB_KEY_question, QUESTION MARK */ \
   OP(L'@', RALT(Key_0)) /* U+0040, XKB_KEY_at, COMMERCIAL AT */ \
   OP(L'A', LSHIFT(Key_Q)) /* U+0041, XKB_KEY_A, LATIN CAPITAL LETTER A */ \
   OP(L'B', LSHIFT(Key_B)) /* U+0042, XKB_KEY_B, LATIN CAPITAL LETTER B */ \
   OP(L'C', LSHIFT(Key_C)) /* U+0043, XKB_KEY_C, LATIN CAPITAL LETTER C */ \
   OP(L'D', LSHIFT(Key_D)) /* U+0044, XKB_KEY_D, LATIN CAPITAL LETTER D */ \
   OP(L'E', LSHIFT(Key_E)) /* U+0045, XKB_KEY_E, LATIN CAPITAL LETTER E */ \
   OP(L'F', LSHIFT(Key_F)) /* U+0046, XKB_KEY_F, LATIN CAPITAL LETTER F */ \
   OP(L'G', LSHIFT(Key_G)) /* U+0047, XKB_KEY_G, LATIN CAPITAL LETTER G */ \
   OP(L'H', LSHIFT(Key_H)) /* U+0048, XKB_KEY_H, LATIN CAPITAL LETTER H */ \
   OP(L'I', LSHIFT(Key_I)) /* U+0049, XKB_KEY_I, LATIN CAPITAL LETTER I */ \
   OP(L'J', LSHIFT(Key_J)) /* U+004a, XKB_KEY_J, LATIN CAPITAL LETTER J */ \
   OP(L'K', LSHIFT(Key_K)) /* U+004b, XKB_KEY_K, LATIN CAPITAL LETTER K */ \
   OP(L'L', LSHIFT(Key_L)) /* U+004c, XKB_KEY_L, LATIN CAPITAL LETTER L */ \
   OP(L'M', LSHIFT(Key_Semicolon)) /* U+004d, XKB_KEY_M, LATIN CAPITAL LETTER M */ \
   OP(L'N', LSHIFT(Key_N)) /* U+004e, XKB_KEY_N, LATIN CAPITAL LETTER N */ \
   OP(L'O', LSHIFT(Key_O)) /* U+004f, XKB_KEY_O, LATIN CAPITAL LETTER O */ \
   OP(L'P', LSHIFT(Key_P)) /* U+0050, XKB_KEY_P, LATIN CAPITAL LETTER P */ \
   OP(L'Q', LSHIFT(Key_A)) /* U+0051, XKB_KEY_Q, LATIN CAPITAL LETTER Q */ \
   OP(L'R', LSHIFT(Key_R)) /* U+0052, XKB_KEY_R, LATIN CAPITAL LETTER R */ \
   OP(L'S', LSHIFT(Key_S)) /* U+0053, XKB_KEY_S, LATIN CAPITAL LETTER S */ \
   OP(L'T', LSHIFT(Key_T)) /* U+0054, XKB_KEY_T, LATIN CAPITAL LETTER T */ \
   OP(L'U', LSHIFT(Key_U)) /* U+0055, XKB_KEY_U, LATIN CAPITAL LETTER U */ \
   OP(L'V', LSHIFT(Key_V)) /* U+0056, XKB_KEY_V, LATIN CAPITAL LETTER V */ \
   OP(L'W', LSHIFT(Key_Z)) /* U+0057, XKB_KEY_W, LATIN CAPITAL LETTER W */ \
   OP(L'X', LSHIFT(Key_X)) /* U+0058, XKB_KEY_X, LATIN CAPITAL LETTER X */ \
   OP(L'Y', LSHIFT(Key_Y)) /* U+0059, XKB_KEY_Y, LATIN CAPITAL LETTER Y */ \
   OP(L'Z', LSHIFT(Key_W)) /* U+005a, XKB_KEY_Z, LATIN CAPITAL LETTER Z */ \
   OP(L'[', RALT(Key_5)) /* U+005b, XKB_KEY_bracketleft, LEFT SQUARE BRACKET */ \
   OP(L'\\', RALT(Key_8)) /* U+005c, XKB_KEY_backslash, REVERSE SOLIDUS */ \
   OP(L']', RALT(Key_Minus)) /* U+005d, XKB_KEY_bracketright, RIGHT SQUARE BRACKET */ \
   OP(L'^', RALT(Key_9)) /* U+005e, XKB_KEY_asciicircum, CIRCUMFLEX ACCENT */ \
   OP(L'_', Key_8) /* U+005f, XKB_KEY_underscore, LOW LINE */ \
   OP(L'a', Key_Q) /* U+0061, XKB_KEY_a, LATIN SMALL LETTER A */ \
   OP(L'b', Key_B) /* U+0062, XKB_KEY_b, LATIN SMALL LETTER B */ \
   OP(L'c', Key_C) /* U+0063, XKB_KEY_c, LATIN SMALL LETTER C */ \
   OP(L'd', Key_D) /* U+0064, XKB_KEY_d, LATIN SMALL LETTER D */ \
   OP(L'e', Key_E) /* U+0065, XKB_KEY_e, LATIN SMALL LETTER E */ \
   OP(L'f', Key_F) /* U+0066, XKB_KEY_f, LATIN SMALL LETTER F */ \
   OP(L'g', Key_G) /* U+0067, XKB_KEY_g, LATIN SMALL LETTER G */ \
   OP(L'h', Key_H) /* U+0068, XKB_KEY_h, LATIN SMALL LETTER H */ \
   OP(L'i', Key_I) /* U+0069, XKB_KEY_i, LATIN SMALL LETTER I */ \
   OP(L'j', Key_J) /* U+006a, XKB_KEY_j, LATIN SMALL LETTER J */ \
   OP(L'k', Key_K) /* U+006b, XKB_KEY_k, LATIN SMALL LETTER K */ \
   OP(L'l', Key_L) /* U+006c, XKB_KEY_l, LATIN SMALL LETTER L */ \
   OP(L'm', Key_Semicolon) /* U+006d, XKB_KEY_m, LATIN SMALL LETTER M */ \
   OP(L'n', Key_N) /* U+006e, XKB_KEY_n, LATIN SMALL LETTER N */ \
   OP(L'o', Key_O) /* U+006f, XKB_KEY_o, LATIN SMALL LETTER O */ \
   OP(L'p', Key_P) /* U+0070, XKB_KEY_p, LATIN SMALL LETTER P */ \
   OP(L'q', Key_A) /* U+0071, XKB_KEY_q, LATIN SMALL LETTER Q */ \
   OP(L'r', Key_R) /* U+0072, XKB_KEY_r, LATIN SMALL LETTER R */ \
   OP(L's', Key_S) /* U+0073, XKB_KEY_s, LATIN SMALL LETTER S */ \
   OP(L't', Key_T) /* U+0074, XKB_KEY_t, LATIN SMALL LETTER T */ \
   OP(L'u', Key_U) /* U+0075, XKB_KEY_u, LATIN SMALL LETTER U */ \
   OP(L'v', Key_V) /* U+0076, XKB_KEY_v, LATIN SMALL LETTER V */ \
   OP(L'w', Key_Z) /* U+0077, XKB_KEY_w, LATIN SMALL LETTER W */ \
   OP(L'x', Key_X) /* U+0078, XKB_KEY_x, LATIN SMALL LETTER X */ \
   OP(L'y', Key_Y) /* U+0079, XKB_KEY_y, LATIN SMALL LETTER Y */ \
   OP(L'z', Key_W) /* U+007a, XKB_KEY_z, LATIN SMALL LETTER Z */ \
   OP(L'{', RALT(Key_4)) /* U+007b, XKB_KEY_braceleft, LEFT CURLY BRACKET */ \
   OP(L'|', RALT(Key_6)) /* U+007c, XKB_KEY_bar, VERTICAL LINE */ \
   OP(L'}', RALT(Key_Equals)) /* U+007d, XKB_KEY_braceright, RIGHT CURLY BRACKET */ \
   OP(L'~', RALT(Key_2)) /* U+007e, XKB_KEY_asciitilde, TILDE */ \
   OP(L'', Key_Delete) /* XKB_KEY_Delete, Delete, rubout  */ \
   OP(L'£', RALT(LSHIFT(Key_L))) /* U+00a3, XKB_KEY_sterling, POUND SIGN */ \
   OP(L'¤', RALT(LSHIFT(Key_NonUsPound))) /* U+00a4, XKB_KEY_currency, CURRENCY SIGN */ \
   OP(L'§', LSHIFT(Key_Slash)) /* U+00a7, XKB_KEY_section, SECTION SIGN */ \
   OP(L'©', RALT(LSHIFT(Key_C))) /* U+00a9, XKB_KEY_copyright, COPYRIGHT SIGN */ \
   OP(L'«', RALT(Key_NonUsBackslashAndPipe)) /* U+00ab, XKB_KEY_guillemotleft, LEFT-POINTING DOUBLE ANGLE QUOTATION MARK */ \
   OP(L'®', RALT(LSHIFT(Key_R))) /* U+00ae, XKB_KEY_registered, REGISTERED SIGN */ \
   OP(L'°', LSHIFT(Key_Minus)) /* U+00b0, XKB_KEY_degree, DEGREE SIGN */ \
   OP(L'±', RALT(LSHIFT(Key_Equals))) /* U+00b1, XKB_KEY_plusminus, PLUS-MINUS SIGN */ \
   OP(L'²', RALT(Key_Backtick)) /* U+00b2, XKB_KEY_twosuperior, SUPERSCRIPT TWO */ \
   OP(L'³', RALT(LSHIFT(Key_Backtick))) /* U+00b3, XKB_KEY_threesuperior, SUPERSCRIPT THREE */ \
   OP(L'µ', RALT(LSHIFT(Key_Semicolon))) /* U+00b5, XKB_KEY_mu, MICRO SIGN */ \
   OP(L'¹', RALT(Key_1)) /* U+00b9, XKB_KEY_onesuperior, SUPERSCRIPT ONE */ \
   OP(L'»', RALT(LSHIFT(Key_NonUsBackslashAndPipe))) /* U+00bb, XKB_KEY_guillemotright, RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK */ \
   OP(L'Æ', RALT(LSHIFT(Key_F))) /* U+00c6, XKB_KEY_AE, LATIN CAPITAL LETTER AE */ \
   OP(L'×', RALT(LSHIFT(Key_M))) /* U+00d7, XKB_KEY_multiply, MULTIPLICATION SIGN */ \
   OP(L'Ø', RALT(LSHIFT(Key_P))) /* U+00d8, XKB_KEY_Oslash, LATIN CAPITAL LETTER O WITH STROKE */ \
   OP(L'à', Key_0) /* U+00e0, XKB_KEY_agrave, LATIN SMALL LETTER A WITH GRAVE */ \
   OP(L'æ', RALT(Key_F)) /* U+00e6, XKB_KEY_ae, LATIN SMALL LETTER AE */ \
   OP(L'ç', Key_9) /* U+00e7, XKB_KEY_ccedilla, LATIN SMALL LETTER C WITH CEDILLA */ \
   OP(L'è', Key_7) /* U+00e8, XKB_KEY_egrave, LATIN SMALL LETTER E WITH GRAVE */ \
   OP(L'é', Key_2) /* U+00e9, XKB_KEY_eacute, LATIN SMALL LETTER E WITH ACUTE */ \
   OP(L'ø', RALT(Key_P)) /* U+00f8, XKB_KEY_oslash, LATIN SMALL LETTER O WITH STROKE */ \
   OP(L'ù', RALT(Key_Semicolon)) /* U+00f9, XKB_KEY_ugrave, LATIN SMALL LETTER U WITH GRAVE */ \
   OP(L'Ŋ', RALT(LSHIFT(Key_N))) \
   OP(L'ŋ', RALT(Key_N)) \
   OP(L'Œ', RALT(LSHIFT(Key_T))) \
   OP(L'œ', RALT(Key_T)) \
   OP(L'Ɓ', RALT(LSHIFT(Key_B))) \
   OP(L'Ɔ', RALT(LSHIFT(Key_O))) \
   OP(L'Ɗ', RALT(LSHIFT(Key_D))) \
   OP(L'Ə', RALT(LSHIFT(Key_E))) /* U+018f, XKB_KEY_SCHWA, LATIN CAPITAL LETTER SCHWA */ \
   OP(L'Ɛ', RALT(LSHIFT(Key_Q))) \
   OP(L'Ɨ', RALT(LSHIFT(Key_I))) \
   OP(L'Ƴ', RALT(LSHIFT(Key_Y))) \
   OP(L'ƴ', RALT(Key_Y)) \
   OP(L'Ʉ', RALT(LSHIFT(Key_U))) \
   OP(L'ɑ', RALT(Key_H)) \
   OP(L'ɓ', RALT(Key_B)) \
   OP(L'ɔ', RALT(Key_O)) \
   OP(L'ɗ', RALT(Key_D)) \
   OP(L'ə', RALT(Key_E)) /* U+0259, XKB_KEY_schwa, LATIN SMALL LETTER SCHWA */ \
   OP(L'ɛ', RALT(Key_Q)) \
   OP(L'ɨ', RALT(Key_I)) \
   OP(L'ʉ', RALT(Key_U)) \
   OP(L'ʼ', RALT(Key_G)) \
   OP(L'̀', Key_Quote) \
   OP(L'́', Key_LeftBracket) \
   OP(L'̂', LSHIFT(Key_Quote)) \
   OP(L'̃', LSHIFT(Key_RightBracket)) \
   OP(L'̄', RALT(Key_RightBracket)) \
   OP(L'̌', LSHIFT(Key_LeftBracket)) \
   OP(L'̍', RALT(LSHIFT(Key_RightBracket))) \
   OP(L'̣', RALT(Key_Period)) \
   OP(L'̧', RALT(Key_C)) \
   OP(L'̰', Key_RightBracket) \
   OP(L'᷄', RALT(LSHIFT(Key_Quote))) \
   OP(L'᷅', RALT(Key_Quote)) \
   OP(L'᷆', Key_NonUsPound) \
   OP(L'᷇', LSHIFT(Key_NonUsPound)) \
   OP(L'Ẅ', RALT(LSHIFT(Key_Z))) /* U+1e84, XKB_KEY_Wdiaeresis, LATIN CAPITAL LETTER W WITH DIAERESIS */ \
   OP(L'ẅ', RALT(Key_Z)) /* U+1e85, XKB_KEY_wdiaeresis, LATIN SMALL LETTER W WITH DIAERESIS */ \
   OP(L'–', RALT(LSHIFT(Key_A))) \
   OP(L'—', RALT(Key_A)) \
   OP(L'‘', RALT(LSHIFT(Key_1))) \
   OP(L'’', RALT(LSHIFT(Key_2))) \
   OP(L'“', RALT(Key_S)) \
   OP(L'”', RALT(LSHIFT(Key_S))) \
   OP(L'†', RALT(LSHIFT(Key_W))) \
   OP(L'…', RALT(Key_W)) \
   OP(L'‹', RALT(Key_X)) \
   OP(L'›', RALT(LSHIFT(Key_X))) \
   OP(L'€', LSHIFT(Key_Backtick)) \
   OP(L'⃖', RALT(LSHIFT(Key_8))) \
   OP(L'⃗', RALT(LSHIFT(Key_0))) \
   OP(L'™', RALT(LSHIFT(Key_J))) \
   OP(L'←', RALT(Key_J)) /* U+2190, XKB_KEY_leftarrow, LEFTWARDS ARROW */ \
   OP(L'↑', RALT(LSHIFT(Key_K))) /* U+2191, XKB_KEY_uparrow, UPWARDS ARROW */ \
   OP(L'→', RALT(Key_L)) /* U+2192, XKB_KEY_rightarrow, RIGHTWARDS ARROW */ \
   OP(L'↓', RALT(Key_K)) /* U+2193, XKB_KEY_downarrow, DOWNWARDS ARROW */ \
   OP(L'◌', RALT(Key_R)) \
   OP(L'Ɑ', RALT(LSHIFT(Key_H))) \
   OP(L'Ꞌ', RALT(LSHIFT(Key_G))) \
   OP(L'ꞌ', RALT(Key_V)) \

#define _HOST_KEYMAP_NON_PRINTABLE(OP) \
   OP(XKB_KEY_A, LSHIFT(Key_Q)) \
   OP(XKB_KEY_AE, RALT(LSHIFT(Key_F))) \
   OP(XKB_KEY_Alt_L, Key_LeftAlt) /* XKB_KEY_Alt_L, Left alt  */ \
   OP(XKB_KEY_B, LSHIFT(Key_B)) \
   OP(XKB_KEY_BackSpace, Key_Backspace) /* XKB_KEY_BackSpace, Back space, back char  */ \
   OP(XKB_KEY_Break, LCTRL(Key_Pause)) /* XKB_KEY_Break */ \
   OP(XKB_KEY_C, LSHIFT(Key_C)) \
   OP(XKB_KEY_Caps_Lock, Key_CapsLock) /* XKB_KEY_Caps_Lock, Caps lock  */ \
   OP(XKB_KEY_Control_L, Key_LeftControl) /* XKB_KEY_Control_L, Left control  */ \
   OP(XKB_KEY_Control_R, Key_RightControl) /* XKB_KEY_Control_R, Right control  */ \
   OP(XKB_KEY_D, LSHIFT(Key_D)) \
   OP(XKB_KEY_Delete, Key_Delete) /* XKB_KEY_Delete, Delete, rubout  */ \
   OP(XKB_KEY_Down, Key_DownArrow) /* XKB_KEY_Down, Move down, down arrow  */ \
   OP(XKB_KEY_E, LSHIFT(Key_E)) \
   OP(XKB_KEY_End, Key_End) /* XKB_KEY_End, EOL  */ \
   OP(XKB_KEY_Escape, Key_Escape) /* XKB_KEY_Escape */ \
   OP(XKB_KEY_F, LSHIFT(Key_F)) \
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
   OP(XKB_KEY_G, LSHIFT(Key_G)) \
   OP(XKB_KEY_H, LSHIFT(Key_H)) \
   OP(XKB_KEY_Home, Key_Home) /* XKB_KEY_Home */ \
   OP(XKB_KEY_I, LSHIFT(Key_I)) \
   OP(XKB_KEY_ISO_Left_Tab, LSHIFT(Key_Tab)) /* XKB_KEY_ISO_Left_Tab */ \
   OP(XKB_KEY_ISO_Level3_Latch, Key_Slash) /* XKB_KEY_ISO_Level3_Latch */ \
   OP(XKB_KEY_ISO_Level3_Shift, Key_RightAlt) /* XKB_KEY_ISO_Level3_Shift */ \
   OP(XKB_KEY_Insert, Key_Insert) /* XKB_KEY_Insert, Insert, insert here  */ \
   OP(XKB_KEY_J, LSHIFT(Key_J)) \
   OP(XKB_KEY_K, LSHIFT(Key_K)) \
   OP(XKB_KEY_KP_0, LSHIFT(Key_0)) /* XKB_KEY_KP_0 */ \
   OP(XKB_KEY_KP_1, LSHIFT(Key_1)) /* XKB_KEY_KP_1 */ \
   OP(XKB_KEY_KP_2, LSHIFT(Key_2)) /* XKB_KEY_KP_2 */ \
   OP(XKB_KEY_KP_3, LSHIFT(Key_3)) /* XKB_KEY_KP_3 */ \
   OP(XKB_KEY_KP_4, LSHIFT(Key_4)) /* XKB_KEY_KP_4 */ \
   OP(XKB_KEY_KP_5, LSHIFT(Key_5)) /* XKB_KEY_KP_5 */ \
   OP(XKB_KEY_KP_6, LSHIFT(Key_6)) /* XKB_KEY_KP_6 */ \
   OP(XKB_KEY_KP_7, LSHIFT(Key_7)) /* XKB_KEY_KP_7 */ \
   OP(XKB_KEY_KP_8, LSHIFT(Key_8)) /* XKB_KEY_KP_8 */ \
   OP(XKB_KEY_KP_9, LSHIFT(Key_9)) /* XKB_KEY_KP_9 */ \
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
   OP(XKB_KEY_L, LSHIFT(Key_L)) \
   OP(XKB_KEY_Left, Key_LeftArrow) /* XKB_KEY_Left, Move left, left arrow  */ \
   OP(XKB_KEY_M, LSHIFT(Key_Semicolon)) \
   OP(XKB_KEY_Menu, Key_PcApplication) /* XKB_KEY_Menu */ \
   OP(XKB_KEY_Meta_L, LSHIFT(Key_LeftAlt)) /* XKB_KEY_Meta_L, Left meta  */ \
   OP(XKB_KEY_N, LSHIFT(Key_N)) \
   OP(XKB_KEY_Next, Key_PageDown) /* XKB_KEY_Next, Next  */ \
   OP(XKB_KEY_Num_Lock, Key_KeypadNumLock) /* XKB_KEY_Num_Lock */ \
   OP(XKB_KEY_O, LSHIFT(Key_O)) \
   OP(XKB_KEY_Oslash, RALT(LSHIFT(Key_P))) \
   OP(XKB_KEY_P, LSHIFT(Key_P)) \
   OP(XKB_KEY_Pause, Key_Pause) /* XKB_KEY_Pause, Pause, hold  */ \
   OP(XKB_KEY_Print, Key_PrintScreen) /* XKB_KEY_Print */ \
   OP(XKB_KEY_Prior, Key_PageUp) /* XKB_KEY_Prior, Prior, previous  */ \
   OP(XKB_KEY_Q, LSHIFT(Key_A)) \
   OP(XKB_KEY_R, LSHIFT(Key_R)) \
   OP(XKB_KEY_Return, Key_Enter) /* XKB_KEY_Return, Return, enter  */ \
   OP(XKB_KEY_Right, Key_RightArrow) /* XKB_KEY_Right, Move right, right arrow  */ \
   OP(XKB_KEY_S, LSHIFT(Key_S)) \
   OP(XKB_KEY_SCHWA, RALT(LSHIFT(Key_E))) \
   OP(XKB_KEY_Scroll_Lock, Key_ScrollLock) /* XKB_KEY_Scroll_Lock */ \
   OP(XKB_KEY_Shift_L, Key_LeftShift) /* XKB_KEY_Shift_L, Left shift  */ \
   OP(XKB_KEY_Shift_R, Key_RightShift) /* XKB_KEY_Shift_R, Right shift  */ \
   OP(XKB_KEY_Super_L, Key_LeftGui) /* XKB_KEY_Super_L, Left super  */ \
   OP(XKB_KEY_Super_R, Key_RightGui) /* XKB_KEY_Super_R, Right super  */ \
   OP(XKB_KEY_Sys_Req, LALT(Key_PrintScreen)) /* XKB_KEY_Sys_Req */ \
   OP(XKB_KEY_T, LSHIFT(Key_T)) \
   OP(XKB_KEY_Tab, Key_Tab) /* XKB_KEY_Tab */ \
   OP(XKB_KEY_U, LSHIFT(Key_U)) \
   OP(XKB_KEY_Up, Key_UpArrow) /* XKB_KEY_Up, Move up, up arrow  */ \
   OP(XKB_KEY_V, LSHIFT(Key_V)) \
   OP(XKB_KEY_W, LSHIFT(Key_Z)) \
   OP(XKB_KEY_Wdiaeresis, RALT(LSHIFT(Key_Z))) \
   OP(XKB_KEY_X, LSHIFT(Key_X)) \
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
   OP(XKB_KEY_Y, LSHIFT(Key_Y)) \
   OP(XKB_KEY_Z, LSHIFT(Key_W)) \
   OP(XKB_KEY_a, Key_Q) \
   OP(XKB_KEY_ae, RALT(Key_F)) \
   OP(XKB_KEY_agrave, Key_0) \
   OP(XKB_KEY_ampersand, Key_1) \
   OP(XKB_KEY_apostrophe, Key_4) /* XKB_KEY_quoteright, deprecated  */ \
   OP(XKB_KEY_asciicircum, RALT(Key_9)) \
   OP(XKB_KEY_asciitilde, RALT(Key_2)) \
   OP(XKB_KEY_at, RALT(Key_0)) \
   OP(XKB_KEY_b, Key_B) \
   OP(XKB_KEY_backslash, RALT(Key_8)) \
   OP(XKB_KEY_bar, RALT(Key_6)) \
   OP(XKB_KEY_braceleft, RALT(Key_4)) \
   OP(XKB_KEY_braceright, RALT(Key_Equals)) \
   OP(XKB_KEY_bracketleft, RALT(Key_5)) \
   OP(XKB_KEY_bracketright, RALT(Key_Minus)) \
   OP(XKB_KEY_c, Key_C) \
   OP(XKB_KEY_ccedilla, Key_9) \
   OP(XKB_KEY_colon, Key_Period) \
   OP(XKB_KEY_comma, Key_M) \
   OP(XKB_KEY_copyright, RALT(LSHIFT(Key_C))) \
   OP(XKB_KEY_currency, RALT(LSHIFT(Key_NonUsPound))) \
   OP(XKB_KEY_d, Key_D) \
   OP(XKB_KEY_dead_acute, RALT(Key_Comma)) /* XKB_KEY_dead_acute */ \
   OP(XKB_KEY_dead_circumflex, RALT(Key_LeftBracket)) /* XKB_KEY_dead_circumflex */ \
   OP(XKB_KEY_dead_diaeresis, RALT(LSHIFT(Key_LeftBracket))) /* XKB_KEY_dead_diaeresis */ \
   OP(XKB_KEY_dead_grave, RALT(LSHIFT(Key_Comma))) /* XKB_KEY_dead_grave */ \
   OP(XKB_KEY_degree, LSHIFT(Key_Minus)) \
   OP(XKB_KEY_dollar, Key_Backtick) \
   OP(XKB_KEY_downarrow, RALT(Key_K)) \
   OP(XKB_KEY_e, Key_E) \
   OP(XKB_KEY_eacute, Key_2) \
   OP(XKB_KEY_egrave, Key_7) \
   OP(XKB_KEY_equal, Key_Equals) \
   OP(XKB_KEY_exclam, RALT(Key_Slash)) \
   OP(XKB_KEY_f, Key_F) \
   OP(XKB_KEY_g, Key_G) \
   OP(XKB_KEY_greater, LSHIFT(Key_NonUsBackslashAndPipe)) \
   OP(XKB_KEY_guillemotleft, RALT(Key_NonUsBackslashAndPipe)) \
   OP(XKB_KEY_guillemotright, RALT(LSHIFT(Key_NonUsBackslashAndPipe))) \
   OP(XKB_KEY_h, Key_H) \
   OP(XKB_KEY_i, Key_I) \
   OP(XKB_KEY_j, Key_J) \
   OP(XKB_KEY_k, Key_K) \
   OP(XKB_KEY_l, Key_L) \
   OP(XKB_KEY_leftarrow, RALT(Key_J)) \
   OP(XKB_KEY_less, Key_NonUsBackslashAndPipe) \
   OP(XKB_KEY_m, Key_Semicolon) \
   OP(XKB_KEY_minus, Key_6) \
   OP(XKB_KEY_mu, RALT(LSHIFT(Key_Semicolon))) \
   OP(XKB_KEY_multiply, RALT(LSHIFT(Key_M))) \
   OP(XKB_KEY_n, Key_N) \
   OP(XKB_KEY_numbersign, RALT(LSHIFT(Key_5))) \
   OP(XKB_KEY_o, Key_O) \
   OP(XKB_KEY_onesuperior, RALT(Key_1)) \
   OP(XKB_KEY_oslash, RALT(Key_P)) \
   OP(XKB_KEY_p, Key_P) \
   OP(XKB_KEY_parenleft, Key_5) \
   OP(XKB_KEY_parenright, Key_Minus) \
   OP(XKB_KEY_percent, RALT(Key_3)) \
   OP(XKB_KEY_period, LSHIFT(Key_Comma)) \
   OP(XKB_KEY_plus, LSHIFT(Key_Equals)) \
   OP(XKB_KEY_plusminus, RALT(LSHIFT(Key_Equals))) \
   OP(XKB_KEY_q, Key_A) \
   OP(XKB_KEY_question, LSHIFT(Key_M)) \
   OP(XKB_KEY_quotedbl, Key_3) \
   OP(XKB_KEY_r, Key_R) \
   OP(XKB_KEY_registered, RALT(LSHIFT(Key_R))) \
   OP(XKB_KEY_rightarrow, RALT(Key_L)) \
   OP(XKB_KEY_s, Key_S) \
   OP(XKB_KEY_schwa, RALT(Key_E)) \
   OP(XKB_KEY_section, LSHIFT(Key_Slash)) \
   OP(XKB_KEY_semicolon, Key_Comma) \
   OP(XKB_KEY_slash, LSHIFT(Key_Period)) \
   OP(XKB_KEY_space, Key_Spacebar) \
   OP(XKB_KEY_sterling, RALT(LSHIFT(Key_L))) \
   OP(XKB_KEY_t, Key_T) \
   OP(XKB_KEY_threesuperior, RALT(LSHIFT(Key_Backtick))) \
   OP(XKB_KEY_twosuperior, RALT(Key_Backtick)) \
   OP(XKB_KEY_u, Key_U) \
   OP(XKB_KEY_ugrave, RALT(Key_Semicolon)) \
   OP(XKB_KEY_underscore, Key_8) \
   OP(XKB_KEY_uparrow, RALT(LSHIFT(Key_K))) \
   OP(XKB_KEY_v, Key_V) \
   OP(XKB_KEY_w, Key_Z) \
   OP(XKB_KEY_wdiaeresis, RALT(Key_Z)) \
   OP(XKB_KEY_x, Key_X) \
   OP(XKB_KEY_y, Key_Y) \
   OP(XKB_KEY_z, Key_W) \


namespace kaleidoscope {
namespace host_keymap {
namespace linux {
namespace cm {
namespace azerty {

HOST_KEYMAP_LINUX(
  _HOST_KEYMAP_ASCII,
  _HOST_KEYMAP_UNICODE,
  _HOST_KEYMAP_NON_PRINTABLE
)

} // namespace azerty
} // namespace cm
} // namespace linux
} // namespace host_keymap
} // namespace kaleidoscope

#undef _HOST_KEYMAP_ASCII
#undef _HOST_KEYMAP_UNICODE
#undef _HOST_KEYMAP_NON_PRINTABLE

