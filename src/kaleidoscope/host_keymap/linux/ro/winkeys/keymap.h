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

// layout: ro
// variant: winkeys

#define _HOST_KEYMAP_ASCII(OP) \
   OP('\b', Key_Backspace) /* XKB_KEY_BackSpace, Back space, back char  */ \
   OP('\t', Key_Tab) /* XKB_KEY_Tab */ \
   OP('\r', Key_Enter) /* XKB_KEY_Return, Return, enter  */ \
   OP('\e', Key_Escape) /* XKB_KEY_Escape */ \
   OP(' ', Key_Spacebar) /* U+0020, XKB_KEY_space, SPACE */ \
   OP('!', LSHIFT(Key_1)) /* U+0021, XKB_KEY_exclam, EXCLAMATION MARK */ \
   OP('"', LSHIFT(Key_2)) /* U+0022, XKB_KEY_quotedbl, QUOTATION MARK */ \
   OP('#', LSHIFT(Key_3)) /* U+0023, XKB_KEY_numbersign, NUMBER SIGN */ \
   OP('$', RALT(Key_Semicolon)) /* U+0024, XKB_KEY_dollar, DOLLAR SIGN */ \
   OP('%', LSHIFT(Key_5)) /* U+0025, XKB_KEY_percent, PERCENT SIGN */ \
   OP('&', LSHIFT(Key_6)) /* U+0026, XKB_KEY_ampersand, AMPERSAND */ \
   OP('\'', Key_Equals) /* U+0027, XKB_KEY_apostrophe, APOSTROPHE */ \
   OP('(', LSHIFT(Key_8)) /* U+0028, XKB_KEY_parenleft, LEFT PARENTHESIS */ \
   OP(')', LSHIFT(Key_9)) /* U+0029, XKB_KEY_parenright, RIGHT PARENTHESIS */ \
   OP('*', Key_KeypadMultiply) /* XKB_KEY_KP_Multiply */ \
   OP('+', Key_Minus) /* U+002b, XKB_KEY_plus, PLUS SIGN */ \
   OP(',', Key_Comma) /* U+002c, XKB_KEY_comma, COMMA */ \
   OP('-', Key_Slash) /* U+002d, XKB_KEY_minus, HYPHEN-MINUS */ \
   OP('.', Key_Period) /* U+002e, XKB_KEY_period, FULL STOP */ \
   OP('/', Key_KeypadDivide) /* XKB_KEY_KP_Divide */ \
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
   OP(':', LSHIFT(Key_Period)) /* U+003a, XKB_KEY_colon, COLON */ \
   OP(';', LSHIFT(Key_Comma)) /* U+003b, XKB_KEY_semicolon, SEMICOLON */ \
   OP('<', Key_NonUsBackslashAndPipe) /* U+003c, XKB_KEY_less, LESS-THAN SIGN */ \
   OP('=', LSHIFT(Key_0)) /* U+003d, XKB_KEY_equal, EQUALS SIGN */ \
   OP('>', RALT(Key_Period)) /* U+003e, XKB_KEY_greater, GREATER-THAN SIGN */ \
   OP('?', LSHIFT(Key_Minus)) /* U+003f, XKB_KEY_question, QUESTION MARK */ \
   OP('@', RALT(Key_2)) /* U+0040, XKB_KEY_at, COMMERCIAL AT */ \
   OP('A', LSHIFT(Key_A)) /* U+0041, XKB_KEY_A, LATIN CAPITAL LETTER A */ \
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
   OP('M', LSHIFT(Key_M)) /* U+004d, XKB_KEY_M, LATIN CAPITAL LETTER M */ \
   OP('N', LSHIFT(Key_N)) /* U+004e, XKB_KEY_N, LATIN CAPITAL LETTER N */ \
   OP('O', LSHIFT(Key_O)) /* U+004f, XKB_KEY_O, LATIN CAPITAL LETTER O */ \
   OP('P', LSHIFT(Key_P)) /* U+0050, XKB_KEY_P, LATIN CAPITAL LETTER P */ \
   OP('Q', LSHIFT(Key_Q)) /* U+0051, XKB_KEY_Q, LATIN CAPITAL LETTER Q */ \
   OP('R', LSHIFT(Key_R)) /* U+0052, XKB_KEY_R, LATIN CAPITAL LETTER R */ \
   OP('S', LSHIFT(Key_S)) /* U+0053, XKB_KEY_S, LATIN CAPITAL LETTER S */ \
   OP('T', LSHIFT(Key_T)) /* U+0054, XKB_KEY_T, LATIN CAPITAL LETTER T */ \
   OP('U', LSHIFT(Key_U)) /* U+0055, XKB_KEY_U, LATIN CAPITAL LETTER U */ \
   OP('V', LSHIFT(Key_V)) /* U+0056, XKB_KEY_V, LATIN CAPITAL LETTER V */ \
   OP('W', LSHIFT(Key_W)) /* U+0057, XKB_KEY_W, LATIN CAPITAL LETTER W */ \
   OP('X', LSHIFT(Key_X)) /* U+0058, XKB_KEY_X, LATIN CAPITAL LETTER X */ \
   OP('Y', LSHIFT(Key_Z)) /* U+0059, XKB_KEY_Y, LATIN CAPITAL LETTER Y */ \
   OP('Z', LSHIFT(Key_Y)) /* U+005a, XKB_KEY_Z, LATIN CAPITAL LETTER Z */ \
   OP('[', RALT(Key_8)) /* U+005b, XKB_KEY_bracketleft, LEFT SQUARE BRACKET */ \
   OP('\\', RALT(Key_Minus)) /* U+005c, XKB_KEY_backslash, REVERSE SOLIDUS */ \
   OP(']', Key_Backtick) /* U+005d, XKB_KEY_bracketright, RIGHT SQUARE BRACKET */ \
   OP('^', RALT(Key_3)) /* U+005e, XKB_KEY_asciicircum, CIRCUMFLEX ACCENT */ \
   OP('_', LSHIFT(Key_Slash)) /* U+005f, XKB_KEY_underscore, LOW LINE */ \
   OP('`', RALT(Key_7)) /* U+0060, XKB_KEY_grave, GRAVE ACCENT */ \
   OP('a', Key_A) /* U+0061, XKB_KEY_a, LATIN SMALL LETTER A */ \
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
   OP('m', Key_M) /* U+006d, XKB_KEY_m, LATIN SMALL LETTER M */ \
   OP('n', Key_N) /* U+006e, XKB_KEY_n, LATIN SMALL LETTER N */ \
   OP('o', Key_O) /* U+006f, XKB_KEY_o, LATIN SMALL LETTER O */ \
   OP('p', Key_P) /* U+0070, XKB_KEY_p, LATIN SMALL LETTER P */ \
   OP('q', Key_Q) /* U+0071, XKB_KEY_q, LATIN SMALL LETTER Q */ \
   OP('r', Key_R) /* U+0072, XKB_KEY_r, LATIN SMALL LETTER R */ \
   OP('s', Key_S) /* U+0073, XKB_KEY_s, LATIN SMALL LETTER S */ \
   OP('t', Key_T) /* U+0074, XKB_KEY_t, LATIN SMALL LETTER T */ \
   OP('u', Key_U) /* U+0075, XKB_KEY_u, LATIN SMALL LETTER U */ \
   OP('v', Key_V) /* U+0076, XKB_KEY_v, LATIN SMALL LETTER V */ \
   OP('w', Key_W) /* U+0077, XKB_KEY_w, LATIN SMALL LETTER W */ \
   OP('x', Key_X) /* U+0078, XKB_KEY_x, LATIN SMALL LETTER X */ \
   OP('y', Key_Z) /* U+0079, XKB_KEY_y, LATIN SMALL LETTER Y */ \
   OP('z', Key_Y) /* U+007a, XKB_KEY_z, LATIN SMALL LETTER Z */ \
   OP('{', RALT(Key_B)) /* U+007b, XKB_KEY_braceleft, LEFT CURLY BRACKET */ \
   OP('|', RALT(Key_W)) /* U+007c, XKB_KEY_bar, VERTICAL LINE */ \
   OP('}', RALT(Key_0)) /* U+007d, XKB_KEY_braceright, RIGHT CURLY BRACKET */ \
   OP('~', RALT(Key_1)) /* U+007e, XKB_KEY_asciitilde, TILDE */ \
   OP('', Key_Delete) /* XKB_KEY_Delete, Delete, rubout  */ \

#define _HOST_KEYMAP_UNICODE(OP) \
   OP(L'\b', Key_Backspace) /* XKB_KEY_BackSpace, Back space, back char  */ \
   OP(L'\t', Key_Tab) /* XKB_KEY_Tab */ \
   OP(L'\r', Key_Enter) /* XKB_KEY_Return, Return, enter  */ \
   OP(L'\e', Key_Escape) /* XKB_KEY_Escape */ \
   OP(L' ', Key_Spacebar) /* U+0020, XKB_KEY_space, SPACE */ \
   OP(L'!', LSHIFT(Key_1)) /* U+0021, XKB_KEY_exclam, EXCLAMATION MARK */ \
   OP(L'"', LSHIFT(Key_2)) /* U+0022, XKB_KEY_quotedbl, QUOTATION MARK */ \
   OP(L'#', LSHIFT(Key_3)) /* U+0023, XKB_KEY_numbersign, NUMBER SIGN */ \
   OP(L'$', RALT(Key_Semicolon)) /* U+0024, XKB_KEY_dollar, DOLLAR SIGN */ \
   OP(L'%', LSHIFT(Key_5)) /* U+0025, XKB_KEY_percent, PERCENT SIGN */ \
   OP(L'&', LSHIFT(Key_6)) /* U+0026, XKB_KEY_ampersand, AMPERSAND */ \
   OP(L'\'', Key_Equals) /* U+0027, XKB_KEY_apostrophe, APOSTROPHE */ \
   OP(L'(', LSHIFT(Key_8)) /* U+0028, XKB_KEY_parenleft, LEFT PARENTHESIS */ \
   OP(L')', LSHIFT(Key_9)) /* U+0029, XKB_KEY_parenright, RIGHT PARENTHESIS */ \
   OP(L'*', Key_KeypadMultiply) /* XKB_KEY_KP_Multiply */ \
   OP(L'+', Key_Minus) /* U+002b, XKB_KEY_plus, PLUS SIGN */ \
   OP(L',', Key_Comma) /* U+002c, XKB_KEY_comma, COMMA */ \
   OP(L'-', Key_Slash) /* U+002d, XKB_KEY_minus, HYPHEN-MINUS */ \
   OP(L'.', Key_Period) /* U+002e, XKB_KEY_period, FULL STOP */ \
   OP(L'/', Key_KeypadDivide) /* XKB_KEY_KP_Divide */ \
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
   OP(L':', LSHIFT(Key_Period)) /* U+003a, XKB_KEY_colon, COLON */ \
   OP(L';', LSHIFT(Key_Comma)) /* U+003b, XKB_KEY_semicolon, SEMICOLON */ \
   OP(L'<', Key_NonUsBackslashAndPipe) /* U+003c, XKB_KEY_less, LESS-THAN SIGN */ \
   OP(L'=', LSHIFT(Key_0)) /* U+003d, XKB_KEY_equal, EQUALS SIGN */ \
   OP(L'>', RALT(Key_Period)) /* U+003e, XKB_KEY_greater, GREATER-THAN SIGN */ \
   OP(L'?', LSHIFT(Key_Minus)) /* U+003f, XKB_KEY_question, QUESTION MARK */ \
   OP(L'@', RALT(Key_2)) /* U+0040, XKB_KEY_at, COMMERCIAL AT */ \
   OP(L'A', LSHIFT(Key_A)) /* U+0041, XKB_KEY_A, LATIN CAPITAL LETTER A */ \
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
   OP(L'M', LSHIFT(Key_M)) /* U+004d, XKB_KEY_M, LATIN CAPITAL LETTER M */ \
   OP(L'N', LSHIFT(Key_N)) /* U+004e, XKB_KEY_N, LATIN CAPITAL LETTER N */ \
   OP(L'O', LSHIFT(Key_O)) /* U+004f, XKB_KEY_O, LATIN CAPITAL LETTER O */ \
   OP(L'P', LSHIFT(Key_P)) /* U+0050, XKB_KEY_P, LATIN CAPITAL LETTER P */ \
   OP(L'Q', LSHIFT(Key_Q)) /* U+0051, XKB_KEY_Q, LATIN CAPITAL LETTER Q */ \
   OP(L'R', LSHIFT(Key_R)) /* U+0052, XKB_KEY_R, LATIN CAPITAL LETTER R */ \
   OP(L'S', LSHIFT(Key_S)) /* U+0053, XKB_KEY_S, LATIN CAPITAL LETTER S */ \
   OP(L'T', LSHIFT(Key_T)) /* U+0054, XKB_KEY_T, LATIN CAPITAL LETTER T */ \
   OP(L'U', LSHIFT(Key_U)) /* U+0055, XKB_KEY_U, LATIN CAPITAL LETTER U */ \
   OP(L'V', LSHIFT(Key_V)) /* U+0056, XKB_KEY_V, LATIN CAPITAL LETTER V */ \
   OP(L'W', LSHIFT(Key_W)) /* U+0057, XKB_KEY_W, LATIN CAPITAL LETTER W */ \
   OP(L'X', LSHIFT(Key_X)) /* U+0058, XKB_KEY_X, LATIN CAPITAL LETTER X */ \
   OP(L'Y', LSHIFT(Key_Z)) /* U+0059, XKB_KEY_Y, LATIN CAPITAL LETTER Y */ \
   OP(L'Z', LSHIFT(Key_Y)) /* U+005a, XKB_KEY_Z, LATIN CAPITAL LETTER Z */ \
   OP(L'[', RALT(Key_8)) /* U+005b, XKB_KEY_bracketleft, LEFT SQUARE BRACKET */ \
   OP(L'\\', RALT(Key_Minus)) /* U+005c, XKB_KEY_backslash, REVERSE SOLIDUS */ \
   OP(L']', Key_Backtick) /* U+005d, XKB_KEY_bracketright, RIGHT SQUARE BRACKET */ \
   OP(L'^', RALT(Key_3)) /* U+005e, XKB_KEY_asciicircum, CIRCUMFLEX ACCENT */ \
   OP(L'_', LSHIFT(Key_Slash)) /* U+005f, XKB_KEY_underscore, LOW LINE */ \
   OP(L'`', RALT(Key_7)) /* U+0060, XKB_KEY_grave, GRAVE ACCENT */ \
   OP(L'a', Key_A) /* U+0061, XKB_KEY_a, LATIN SMALL LETTER A */ \
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
   OP(L'm', Key_M) /* U+006d, XKB_KEY_m, LATIN SMALL LETTER M */ \
   OP(L'n', Key_N) /* U+006e, XKB_KEY_n, LATIN SMALL LETTER N */ \
   OP(L'o', Key_O) /* U+006f, XKB_KEY_o, LATIN SMALL LETTER O */ \
   OP(L'p', Key_P) /* U+0070, XKB_KEY_p, LATIN SMALL LETTER P */ \
   OP(L'q', Key_Q) /* U+0071, XKB_KEY_q, LATIN SMALL LETTER Q */ \
   OP(L'r', Key_R) /* U+0072, XKB_KEY_r, LATIN SMALL LETTER R */ \
   OP(L's', Key_S) /* U+0073, XKB_KEY_s, LATIN SMALL LETTER S */ \
   OP(L't', Key_T) /* U+0074, XKB_KEY_t, LATIN SMALL LETTER T */ \
   OP(L'u', Key_U) /* U+0075, XKB_KEY_u, LATIN SMALL LETTER U */ \
   OP(L'v', Key_V) /* U+0076, XKB_KEY_v, LATIN SMALL LETTER V */ \
   OP(L'w', Key_W) /* U+0077, XKB_KEY_w, LATIN SMALL LETTER W */ \
   OP(L'x', Key_X) /* U+0078, XKB_KEY_x, LATIN SMALL LETTER X */ \
   OP(L'y', Key_Z) /* U+0079, XKB_KEY_y, LATIN SMALL LETTER Y */ \
   OP(L'z', Key_Y) /* U+007a, XKB_KEY_z, LATIN SMALL LETTER Z */ \
   OP(L'{', RALT(Key_B)) /* U+007b, XKB_KEY_braceleft, LEFT CURLY BRACKET */ \
   OP(L'|', RALT(Key_W)) /* U+007c, XKB_KEY_bar, VERTICAL LINE */ \
   OP(L'}', RALT(Key_0)) /* U+007d, XKB_KEY_braceright, RIGHT CURLY BRACKET */ \
   OP(L'~', RALT(Key_1)) /* U+007e, XKB_KEY_asciitilde, TILDE */ \
   OP(L'', Key_Delete) /* XKB_KEY_Delete, Delete, rubout  */ \
   OP(L'¢', RALT(Key_C)) /* U+00a2, XKB_KEY_cent, CENT SIGN */ \
   OP(L'¤', LSHIFT(Key_4)) /* U+00a4, XKB_KEY_currency, CURRENCY SIGN */ \
   OP(L'¥', RALT(LSHIFT(Key_Y))) /* U+00a5, XKB_KEY_yen, YEN SIGN */ \
   OP(L'¦', RALT(LSHIFT(Key_NonUsBackslashAndPipe))) /* U+00a6, XKB_KEY_brokenbar, BROKEN BAR */ \
   OP(L'§', RALT(Key_M)) /* U+00a7, XKB_KEY_section, SECTION SIGN */ \
   OP(L'©', RALT(LSHIFT(Key_C))) /* U+00a9, XKB_KEY_copyright, COPYRIGHT SIGN */ \
   OP(L'ª', RALT(LSHIFT(Key_F))) /* U+00aa, XKB_KEY_ordfeminine, FEMININE ORDINAL INDICATOR */ \
   OP(L'«', RALT(Key_Z)) /* U+00ab, XKB_KEY_guillemotleft, LEFT-POINTING DOUBLE ANGLE QUOTATION MARK */ \
   OP(L'¬', RALT(Key_Backtick)) /* U+00ac, XKB_KEY_notsign, NOT SIGN */ \
   OP(L'®', RALT(LSHIFT(Key_R))) /* U+00ae, XKB_KEY_registered, REGISTERED SIGN */ \
   OP(L'°', RALT(Key_5)) /* U+00b0, XKB_KEY_degree, DEGREE SIGN */ \
   OP(L'±', RALT(LSHIFT(Key_9))) /* U+00b1, XKB_KEY_plusminus, PLUS-MINUS SIGN */ \
   OP(L'¶', RALT(Key_R)) /* U+00b6, XKB_KEY_paragraph, PILCROW SIGN */ \
   OP(L'»', RALT(Key_X)) /* U+00bb, XKB_KEY_guillemotright, RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK */ \
   OP(L'¼', RALT(Key_4)) /* U+00bc, XKB_KEY_onequarter, VULGAR FRACTION ONE QUARTER */ \
   OP(L'¾', RALT(Key_6)) /* U+00be, XKB_KEY_threequarters, VULGAR FRACTION THREE QUARTERS */ \
   OP(L'¿', RALT(LSHIFT(Key_Minus))) /* U+00bf, XKB_KEY_questiondown, INVERTED QUESTION MARK */ \
   OP(L'Â', LSHIFT(Key_NonUsPound)) /* U+00c2, XKB_KEY_Acircumflex, LATIN CAPITAL LETTER A WITH CIRCUMFLEX */ \
   OP(L'Æ', RALT(LSHIFT(Key_A))) /* U+00c6, XKB_KEY_AE, LATIN CAPITAL LETTER AE */ \
   OP(L'Î', LSHIFT(Key_RightBracket)) /* U+00ce, XKB_KEY_Icircumflex, LATIN CAPITAL LETTER I WITH CIRCUMFLEX */ \
   OP(L'Ð', RALT(LSHIFT(Key_D))) /* U+00d0, XKB_KEY_ETH, LATIN CAPITAL LETTER ETH */ \
   OP(L'×', RALT(Key_RightBracket)) /* U+00d7, XKB_KEY_multiply, MULTIPLICATION SIGN */ \
   OP(L'Ø', RALT(LSHIFT(Key_O))) /* U+00d8, XKB_KEY_Oslash, LATIN CAPITAL LETTER O WITH STROKE */ \
   OP(L'Þ', RALT(LSHIFT(Key_P))) /* U+00de, XKB_KEY_THORN, LATIN CAPITAL LETTER THORN */ \
   OP(L'ß', RALT(Key_S)) /* U+00df, XKB_KEY_ssharp, LATIN SMALL LETTER SHARP S */ \
   OP(L'â', Key_NonUsPound) /* U+00e2, XKB_KEY_acircumflex, LATIN SMALL LETTER A WITH CIRCUMFLEX */ \
   OP(L'æ', RALT(Key_A)) /* U+00e6, XKB_KEY_ae, LATIN SMALL LETTER AE */ \
   OP(L'î', Key_RightBracket) /* U+00ee, XKB_KEY_icircumflex, LATIN SMALL LETTER I WITH CIRCUMFLEX */ \
   OP(L'ð', RALT(Key_D)) /* U+00f0, XKB_KEY_eth, LATIN SMALL LETTER ETH */ \
   OP(L'÷', RALT(Key_LeftBracket)) /* U+00f7, XKB_KEY_division, DIVISION SIGN */ \
   OP(L'ø', RALT(Key_O)) /* U+00f8, XKB_KEY_oslash, LATIN SMALL LETTER O WITH STROKE */ \
   OP(L'þ', RALT(Key_P)) /* U+00fe, XKB_KEY_thorn, LATIN SMALL LETTER THORN */ \
   OP(L'Ă', LSHIFT(Key_LeftBracket)) /* U+0102, XKB_KEY_Abreve, LATIN CAPITAL LETTER A WITH BREVE */ \
   OP(L'ă', Key_LeftBracket) /* U+0103, XKB_KEY_abreve, LATIN SMALL LETTER A WITH BREVE */ \
   OP(L'đ', RALT(Key_F)) /* U+0111, XKB_KEY_dstroke, LATIN SMALL LETTER D WITH STROKE */ \
   OP(L'Ħ', RALT(LSHIFT(Key_H))) /* U+0126, XKB_KEY_Hstroke, LATIN CAPITAL LETTER H WITH STROKE */ \
   OP(L'ħ', RALT(Key_H)) /* U+0127, XKB_KEY_hstroke, LATIN SMALL LETTER H WITH STROKE */ \
   OP(L'ı', RALT(LSHIFT(Key_I))) /* U+0131, XKB_KEY_idotless, LATIN SMALL LETTER DOTLESS I */ \
   OP(L'ĸ', RALT(Key_K)) /* U+0138, XKB_KEY_kra, LATIN SMALL LETTER KRA */ \
   OP(L'Ł', RALT(LSHIFT(Key_L))) /* U+0141, XKB_KEY_Lstroke, LATIN CAPITAL LETTER L WITH STROKE */ \
   OP(L'ł', RALT(Key_L)) /* U+0142, XKB_KEY_lstroke, LATIN SMALL LETTER L WITH STROKE */ \
   OP(L'Ŋ', RALT(LSHIFT(Key_G))) /* U+014a, XKB_KEY_ENG, LATIN CAPITAL LETTER ENG */ \
   OP(L'ŋ', RALT(Key_G)) /* U+014b, XKB_KEY_eng, LATIN SMALL LETTER ENG */ \
   OP(L'Ş', LSHIFT(Key_Semicolon)) /* U+015e, XKB_KEY_Scedilla, LATIN CAPITAL LETTER S WITH CEDILLA */ \
   OP(L'ş', Key_Semicolon) /* U+015f, XKB_KEY_scedilla, LATIN SMALL LETTER S WITH CEDILLA */ \
   OP(L'Ţ', LSHIFT(Key_Quote)) /* U+0162, XKB_KEY_Tcedilla, LATIN CAPITAL LETTER T WITH CEDILLA */ \
   OP(L'ţ', Key_Quote) /* U+0163, XKB_KEY_tcedilla, LATIN SMALL LETTER T WITH CEDILLA */ \
   OP(L'Ŧ', RALT(LSHIFT(Key_T))) /* U+0166, XKB_KEY_Tslash, LATIN CAPITAL LETTER T WITH STROKE */ \
   OP(L'ŧ', RALT(Key_T)) /* U+0167, XKB_KEY_tslash, LATIN SMALL LETTER T WITH STROKE */ \
   OP(L'‘', RALT(LSHIFT(Key_V))) /* U+2018, XKB_KEY_leftsinglequotemark, LEFT SINGLE QUOTATION MARK */ \
   OP(L'“', RALT(Key_V)) /* U+201c, XKB_KEY_leftdoublequotemark, LEFT DOUBLE QUOTATION MARK */ \
   OP(L'€', RALT(Key_E)) /* U+20ac, XKB_KEY_EuroSign, EURO SIGN */ \
   OP(L'™', RALT(LSHIFT(Key_8))) /* U+2122, XKB_KEY_trademark, TRADE MARK SIGN */ \
   OP(L'⅝', RALT(LSHIFT(Key_6))) /* U+215d, XKB_KEY_fiveeighths, VULGAR FRACTION FIVE EIGHTHS */ \
   OP(L'←', RALT(Key_Y)) /* U+2190, XKB_KEY_leftarrow, LEFTWARDS ARROW */ \
   OP(L'↑', RALT(LSHIFT(Key_U))) /* U+2191, XKB_KEY_uparrow, UPWARDS ARROW */ \
   OP(L'→', RALT(Key_I)) /* U+2192, XKB_KEY_rightarrow, RIGHTWARDS ARROW */ \
   OP(L'↓', RALT(Key_U)) /* U+2193, XKB_KEY_downarrow, DOWNWARDS ARROW */ \

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
   OP(XKB_KEY_A, LSHIFT(Key_A)) \
   OP(XKB_KEY_AE, RALT(LSHIFT(Key_A))) \
   OP(XKB_KEY_Abreve, LSHIFT(Key_LeftBracket)) \
   OP(XKB_KEY_Acircumflex, LSHIFT(Key_NonUsPound)) \
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
   OP(XKB_KEY_ENG, RALT(LSHIFT(Key_G))) \
   OP(XKB_KEY_ETH, RALT(LSHIFT(Key_D))) /* XKB_KEY_Eth, deprecated  */ \
   OP(XKB_KEY_End, Key_End) /* XKB_KEY_End, EOL  */ \
   OP(XKB_KEY_Escape, Key_Escape) /* XKB_KEY_Escape */ \
   OP(XKB_KEY_EuroSign, RALT(Key_E)) \
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
   OP(XKB_KEY_Hstroke, RALT(LSHIFT(Key_H))) \
   OP(XKB_KEY_I, LSHIFT(Key_I)) \
   OP(XKB_KEY_ISO_Left_Tab, LSHIFT(Key_Tab)) /* XKB_KEY_ISO_Left_Tab */ \
   OP(XKB_KEY_ISO_Level3_Shift, Key_RightAlt) /* XKB_KEY_ISO_Level3_Shift */ \
   OP(XKB_KEY_Icircumflex, LSHIFT(Key_RightBracket)) \
   OP(XKB_KEY_Insert, Key_Insert) /* XKB_KEY_Insert, Insert, insert here  */ \
   OP(XKB_KEY_J, LSHIFT(Key_J)) \
   OP(XKB_KEY_K, LSHIFT(Key_K)) \
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
   OP(XKB_KEY_KP_Separator, LSHIFT(Key_KeypadDot)) /* XKB_KEY_KP_Separator, Separator, often comma  */ \
   OP(XKB_KEY_KP_Subtract, Key_KeypadSubtract) /* XKB_KEY_KP_Subtract */ \
   OP(XKB_KEY_KP_Up, Key_Keypad8) /* XKB_KEY_KP_Up */ \
   OP(XKB_KEY_L, LSHIFT(Key_L)) \
   OP(XKB_KEY_Left, Key_LeftArrow) /* XKB_KEY_Left, Move left, left arrow  */ \
   OP(XKB_KEY_Lstroke, RALT(LSHIFT(Key_L))) \
   OP(XKB_KEY_M, LSHIFT(Key_M)) \
   OP(XKB_KEY_Menu, Key_PcApplication) /* XKB_KEY_Menu */ \
   OP(XKB_KEY_Meta_L, LSHIFT(Key_LeftAlt)) /* XKB_KEY_Meta_L, Left meta  */ \
   OP(XKB_KEY_N, LSHIFT(Key_N)) \
   OP(XKB_KEY_Next, Key_PageDown) /* XKB_KEY_Next, Next  */ \
   OP(XKB_KEY_Num_Lock, Key_KeypadNumLock) /* XKB_KEY_Num_Lock */ \
   OP(XKB_KEY_O, LSHIFT(Key_O)) \
   OP(XKB_KEY_Oslash, RALT(LSHIFT(Key_O))) \
   OP(XKB_KEY_P, LSHIFT(Key_P)) \
   OP(XKB_KEY_Pause, Key_Pause) /* XKB_KEY_Pause, Pause, hold  */ \
   OP(XKB_KEY_Print, Key_PrintScreen) /* XKB_KEY_Print */ \
   OP(XKB_KEY_Prior, Key_PageUp) /* XKB_KEY_Prior, Prior, previous  */ \
   OP(XKB_KEY_Q, LSHIFT(Key_Q)) \
   OP(XKB_KEY_R, LSHIFT(Key_R)) \
   OP(XKB_KEY_Return, Key_Enter) /* XKB_KEY_Return, Return, enter  */ \
   OP(XKB_KEY_Right, Key_RightArrow) /* XKB_KEY_Right, Move right, right arrow  */ \
   OP(XKB_KEY_S, LSHIFT(Key_S)) \
   OP(XKB_KEY_Scedilla, LSHIFT(Key_Semicolon)) \
   OP(XKB_KEY_Scroll_Lock, Key_ScrollLock) /* XKB_KEY_Scroll_Lock */ \
   OP(XKB_KEY_Shift_L, Key_LeftShift) /* XKB_KEY_Shift_L, Left shift  */ \
   OP(XKB_KEY_Shift_R, Key_RightShift) /* XKB_KEY_Shift_R, Right shift  */ \
   OP(XKB_KEY_Super_L, Key_LeftGui) /* XKB_KEY_Super_L, Left super  */ \
   OP(XKB_KEY_Super_R, Key_RightGui) /* XKB_KEY_Super_R, Right super  */ \
   OP(XKB_KEY_Sys_Req, LALT(Key_PrintScreen)) /* XKB_KEY_Sys_Req */ \
   OP(XKB_KEY_T, LSHIFT(Key_T)) \
   OP(XKB_KEY_THORN, RALT(LSHIFT(Key_P))) /* XKB_KEY_Thorn, deprecated  */ \
   OP(XKB_KEY_Tab, Key_Tab) /* XKB_KEY_Tab */ \
   OP(XKB_KEY_Tcedilla, LSHIFT(Key_Quote)) \
   OP(XKB_KEY_Tslash, RALT(LSHIFT(Key_T))) \
   OP(XKB_KEY_U, LSHIFT(Key_U)) \
   OP(XKB_KEY_Up, Key_UpArrow) /* XKB_KEY_Up, Move up, up arrow  */ \
   OP(XKB_KEY_V, LSHIFT(Key_V)) \
   OP(XKB_KEY_W, LSHIFT(Key_W)) \
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
   OP(XKB_KEY_Y, LSHIFT(Key_Z)) \
   OP(XKB_KEY_Z, LSHIFT(Key_Y)) \
   OP(XKB_KEY_a, Key_A) \
   OP(XKB_KEY_abreve, Key_LeftBracket) \
   OP(XKB_KEY_acircumflex, Key_NonUsPound) \
   OP(XKB_KEY_ae, RALT(Key_A)) \
   OP(XKB_KEY_ampersand, LSHIFT(Key_6)) \
   OP(XKB_KEY_apostrophe, Key_Equals) /* XKB_KEY_quoteright, deprecated  */ \
   OP(XKB_KEY_asciicircum, RALT(Key_3)) \
   OP(XKB_KEY_asciitilde, RALT(Key_1)) \
   OP(XKB_KEY_asterisk, LSHIFT(Key_Equals)) \
   OP(XKB_KEY_at, RALT(Key_2)) \
   OP(XKB_KEY_b, Key_B) \
   OP(XKB_KEY_backslash, RALT(Key_Minus)) \
   OP(XKB_KEY_bar, RALT(Key_W)) \
   OP(XKB_KEY_braceleft, RALT(Key_B)) \
   OP(XKB_KEY_braceright, RALT(Key_0)) \
   OP(XKB_KEY_bracketleft, RALT(Key_8)) \
   OP(XKB_KEY_bracketright, Key_Backtick) \
   OP(XKB_KEY_brokenbar, RALT(LSHIFT(Key_NonUsBackslashAndPipe))) \
   OP(XKB_KEY_c, Key_C) \
   OP(XKB_KEY_cent, RALT(Key_C)) \
   OP(XKB_KEY_colon, LSHIFT(Key_Period)) \
   OP(XKB_KEY_comma, Key_Comma) \
   OP(XKB_KEY_copyright, RALT(LSHIFT(Key_C))) \
   OP(XKB_KEY_currency, LSHIFT(Key_4)) \
   OP(XKB_KEY_d, Key_D) \
   OP(XKB_KEY_dead_abovedot, RALT(LSHIFT(Key_Slash))) /* XKB_KEY_dead_abovedot */ \
   OP(XKB_KEY_dead_belowdot, RALT(Key_Slash)) /* XKB_KEY_dead_belowdot */ \
   OP(XKB_KEY_dead_breve, RALT(LSHIFT(Key_NonUsPound))) /* XKB_KEY_dead_breve */ \
   OP(XKB_KEY_dead_cedilla, RALT(Key_Equals)) /* XKB_KEY_dead_cedilla */ \
   OP(XKB_KEY_dead_grave, RALT(Key_NonUsPound)) /* XKB_KEY_dead_grave */ \
   OP(XKB_KEY_dead_hook, RALT(Key_J)) /* XKB_KEY_dead_hook */ \
   OP(XKB_KEY_dead_horn, RALT(LSHIFT(Key_J))) /* XKB_KEY_dead_horn */ \
   OP(XKB_KEY_dead_ogonek, RALT(LSHIFT(Key_Equals))) /* XKB_KEY_dead_ogonek */ \
   OP(XKB_KEY_degree, RALT(Key_5)) \
   OP(XKB_KEY_division, RALT(Key_LeftBracket)) \
   OP(XKB_KEY_dollar, RALT(Key_Semicolon)) \
   OP(XKB_KEY_downarrow, RALT(Key_U)) \
   OP(XKB_KEY_dstroke, RALT(Key_F)) \
   OP(XKB_KEY_e, Key_E) \
   OP(XKB_KEY_eng, RALT(Key_G)) \
   OP(XKB_KEY_equal, LSHIFT(Key_0)) \
   OP(XKB_KEY_eth, RALT(Key_D)) \
   OP(XKB_KEY_exclam, LSHIFT(Key_1)) \
   OP(XKB_KEY_f, Key_F) \
   OP(XKB_KEY_fiveeighths, RALT(LSHIFT(Key_6))) \
   OP(XKB_KEY_g, Key_G) \
   OP(XKB_KEY_grave, RALT(Key_7)) /* XKB_KEY_quoteleft, deprecated  */ \
   OP(XKB_KEY_greater, RALT(Key_Period)) \
   OP(XKB_KEY_guillemotleft, RALT(Key_Z)) \
   OP(XKB_KEY_guillemotright, RALT(Key_X)) \
   OP(XKB_KEY_h, Key_H) \
   OP(XKB_KEY_hstroke, RALT(Key_H)) \
   OP(XKB_KEY_i, Key_I) \
   OP(XKB_KEY_icircumflex, Key_RightBracket) \
   OP(XKB_KEY_idotless, RALT(LSHIFT(Key_I))) \
   OP(XKB_KEY_j, Key_J) \
   OP(XKB_KEY_k, Key_K) \
   OP(XKB_KEY_kra, RALT(Key_K)) /* XKB_KEY_kappa, deprecated  */ \
   OP(XKB_KEY_l, Key_L) \
   OP(XKB_KEY_leftarrow, RALT(Key_Y)) \
   OP(XKB_KEY_leftdoublequotemark, RALT(Key_V)) \
   OP(XKB_KEY_leftsinglequotemark, RALT(LSHIFT(Key_V))) \
   OP(XKB_KEY_less, Key_NonUsBackslashAndPipe) \
   OP(XKB_KEY_lstroke, RALT(Key_L)) \
   OP(XKB_KEY_m, Key_M) \
   OP(XKB_KEY_minus, Key_Slash) \
   OP(XKB_KEY_multiply, RALT(Key_RightBracket)) \
   OP(XKB_KEY_n, Key_N) \
   OP(XKB_KEY_notsign, RALT(Key_Backtick)) \
   OP(XKB_KEY_numbersign, LSHIFT(Key_3)) \
   OP(XKB_KEY_o, Key_O) \
   OP(XKB_KEY_onequarter, RALT(Key_4)) \
   OP(XKB_KEY_ordfeminine, RALT(LSHIFT(Key_F))) \
   OP(XKB_KEY_oslash, RALT(Key_O)) \
   OP(XKB_KEY_p, Key_P) \
   OP(XKB_KEY_paragraph, RALT(Key_R)) \
   OP(XKB_KEY_parenleft, LSHIFT(Key_8)) \
   OP(XKB_KEY_parenright, LSHIFT(Key_9)) \
   OP(XKB_KEY_percent, LSHIFT(Key_5)) \
   OP(XKB_KEY_period, Key_Period) \
   OP(XKB_KEY_plus, Key_Minus) \
   OP(XKB_KEY_plusminus, RALT(LSHIFT(Key_9))) \
   OP(XKB_KEY_q, Key_Q) \
   OP(XKB_KEY_question, LSHIFT(Key_Minus)) \
   OP(XKB_KEY_questiondown, RALT(LSHIFT(Key_Minus))) \
   OP(XKB_KEY_quotedbl, LSHIFT(Key_2)) \
   OP(XKB_KEY_r, Key_R) \
   OP(XKB_KEY_registered, RALT(LSHIFT(Key_R))) \
   OP(XKB_KEY_rightarrow, RALT(Key_I)) \
   OP(XKB_KEY_s, Key_S) \
   OP(XKB_KEY_scedilla, Key_Semicolon) \
   OP(XKB_KEY_section, RALT(Key_M)) \
   OP(XKB_KEY_semicolon, LSHIFT(Key_Comma)) \
   OP(XKB_KEY_slash, LSHIFT(Key_7)) \
   OP(XKB_KEY_space, Key_Spacebar) \
   OP(XKB_KEY_ssharp, RALT(Key_S)) \
   OP(XKB_KEY_t, Key_T) \
   OP(XKB_KEY_tcedilla, Key_Quote) \
   OP(XKB_KEY_thorn, RALT(Key_P)) \
   OP(XKB_KEY_threequarters, RALT(Key_6)) \
   OP(XKB_KEY_trademark, RALT(LSHIFT(Key_8))) \
   OP(XKB_KEY_tslash, RALT(Key_T)) \
   OP(XKB_KEY_u, Key_U) \
   OP(XKB_KEY_underscore, LSHIFT(Key_Slash)) \
   OP(XKB_KEY_uparrow, RALT(LSHIFT(Key_U))) \
   OP(XKB_KEY_v, Key_V) \
   OP(XKB_KEY_w, Key_W) \
   OP(XKB_KEY_x, Key_X) \
   OP(XKB_KEY_y, Key_Z) \
   OP(XKB_KEY_yen, RALT(LSHIFT(Key_Y))) \
   OP(XKB_KEY_z, Key_Y) \


namespace kaleidoscope {
namespace host_keymap {
namespace linux {
namespace ro {
namespace winkeys {

HOST_KEYMAP_LINUX(
  _HOST_KEYMAP_ASCII,
  _HOST_KEYMAP_UNICODE,
  _HOST_KEYMAP_NON_PRINTABLE
)

} // namespace winkeys
} // namespace ro
} // namespace linux
} // namespace host_keymap
} // namespace kaleidoscope

#undef _HOST_KEYMAP_ASCII
#undef _HOST_KEYMAP_UNICODE
#undef _HOST_KEYMAP_NON_PRINTABLE

