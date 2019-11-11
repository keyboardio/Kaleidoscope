/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2018  Keyboard.io, Inc.
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

// Information extracted from xkbcommon-keysyms.h

// Function macro invoked with arguments
//    KEY_SYM_NAME, KEY_SYM, DESC
//
#define XKB_KEY_SYM_NAME__KEY_SYM__DESCRIPTION(OP) \
   OP(XKB_KEY_VoidSymbol, 0xffffff, "Void symbol ") \
   OP(XKB_KEY_BackSpace, 0xff08, "Back space, back char ") \
   OP(XKB_KEY_Tab, 0xff09, "") \
   OP(XKB_KEY_Linefeed, 0xff0a, "Linefeed, LF ") \
   OP(XKB_KEY_Clear, 0xff0b, "") \
   OP(XKB_KEY_Return, 0xff0d, "Return, enter ") \
   OP(XKB_KEY_Pause, 0xff13, "Pause, hold ") \
   OP(XKB_KEY_Scroll_Lock, 0xff14, "") \
   OP(XKB_KEY_Sys_Req, 0xff15, "") \
   OP(XKB_KEY_Escape, 0xff1b, "") \
   OP(XKB_KEY_Delete, 0xffff, "Delete, rubout ") \
   OP(XKB_KEY_Multi_key, 0xff20, "Multi-key character compose ") \
   OP(XKB_KEY_Codeinput, 0xff37, "") \
   OP(XKB_KEY_SingleCandidate, 0xff3c, "") \
   OP(XKB_KEY_MultipleCandidate, 0xff3d, "") \
   OP(XKB_KEY_PreviousCandidate, 0xff3e, "") \
   OP(XKB_KEY_Kanji, 0xff21, "Kanji, Kanji convert ") \
   OP(XKB_KEY_Muhenkan, 0xff22, "Cancel Conversion ") \
   OP(XKB_KEY_Henkan_Mode, 0xff23, "Start/Stop Conversion ") \
   OP(XKB_KEY_Henkan, 0xff23, "Alias for Henkan_Mode ") \
   OP(XKB_KEY_Romaji, 0xff24, "to Romaji ") \
   OP(XKB_KEY_Hiragana, 0xff25, "to Hiragana ") \
   OP(XKB_KEY_Katakana, 0xff26, "to Katakana ") \
   OP(XKB_KEY_Hiragana_Katakana, 0xff27, "Hiragana/Katakana toggle ") \
   OP(XKB_KEY_Zenkaku, 0xff28, "to Zenkaku ") \
   OP(XKB_KEY_Hankaku, 0xff29, "to Hankaku ") \
   OP(XKB_KEY_Zenkaku_Hankaku, 0xff2a, "Zenkaku/Hankaku toggle ") \
   OP(XKB_KEY_Touroku, 0xff2b, "Add to Dictionary ") \
   OP(XKB_KEY_Massyo, 0xff2c, "Delete from Dictionary ") \
   OP(XKB_KEY_Kana_Lock, 0xff2d, "Kana Lock ") \
   OP(XKB_KEY_Kana_Shift, 0xff2e, "Kana Shift ") \
   OP(XKB_KEY_Eisu_Shift, 0xff2f, "Alphanumeric Shift ") \
   OP(XKB_KEY_Eisu_toggle, 0xff30, "Alphanumeric toggle ") \
   OP(XKB_KEY_Kanji_Bangou, 0xff37, "Codeinput ") \
   OP(XKB_KEY_Zen_Koho, 0xff3d, "Multiple/All Candidate(s) ") \
   OP(XKB_KEY_Mae_Koho, 0xff3e, "Previous Candidate ") \
   OP(XKB_KEY_Home, 0xff50, "") \
   OP(XKB_KEY_Left, 0xff51, "Move left, left arrow ") \
   OP(XKB_KEY_Up, 0xff52, "Move up, up arrow ") \
   OP(XKB_KEY_Right, 0xff53, "Move right, right arrow ") \
   OP(XKB_KEY_Down, 0xff54, "Move down, down arrow ") \
   OP(XKB_KEY_Prior, 0xff55, "Prior, previous ") \
   OP(XKB_KEY_Page_Up, 0xff55, "") \
   OP(XKB_KEY_Next, 0xff56, "Next ") \
   OP(XKB_KEY_Page_Down, 0xff56, "") \
   OP(XKB_KEY_End, 0xff57, "EOL ") \
   OP(XKB_KEY_Begin, 0xff58, "BOL ") \
   OP(XKB_KEY_Select, 0xff60, "Select, mark ") \
   OP(XKB_KEY_Print, 0xff61, "") \
   OP(XKB_KEY_Execute, 0xff62, "Execute, run, do ") \
   OP(XKB_KEY_Insert, 0xff63, "Insert, insert here ") \
   OP(XKB_KEY_Undo, 0xff65, "") \
   OP(XKB_KEY_Redo, 0xff66, "Redo, again ") \
   OP(XKB_KEY_Menu, 0xff67, "") \
   OP(XKB_KEY_Find, 0xff68, "Find, search ") \
   OP(XKB_KEY_Cancel, 0xff69, "Cancel, stop, abort, exit ") \
   OP(XKB_KEY_Help, 0xff6a, "Help ") \
   OP(XKB_KEY_Break, 0xff6b, "") \
   OP(XKB_KEY_Mode_switch, 0xff7e, "Character set switch ") \
   OP(XKB_KEY_script_switch, 0xff7e, "Alias for mode_switch ") \
   OP(XKB_KEY_Num_Lock, 0xff7f, "") \
   OP(XKB_KEY_KP_Space, 0xff80, "Space ") \
   OP(XKB_KEY_KP_Tab, 0xff89, "") \
   OP(XKB_KEY_KP_Enter, 0xff8d, "Enter ") \
   OP(XKB_KEY_KP_F1, 0xff91, "PF1, KP_A, ... ") \
   OP(XKB_KEY_KP_F2, 0xff92, "") \
   OP(XKB_KEY_KP_F3, 0xff93, "") \
   OP(XKB_KEY_KP_F4, 0xff94, "") \
   OP(XKB_KEY_KP_Home, 0xff95, "") \
   OP(XKB_KEY_KP_Left, 0xff96, "") \
   OP(XKB_KEY_KP_Up, 0xff97, "") \
   OP(XKB_KEY_KP_Right, 0xff98, "") \
   OP(XKB_KEY_KP_Down, 0xff99, "") \
   OP(XKB_KEY_KP_Prior, 0xff9a, "") \
   OP(XKB_KEY_KP_Page_Up, 0xff9a, "") \
   OP(XKB_KEY_KP_Next, 0xff9b, "") \
   OP(XKB_KEY_KP_Page_Down, 0xff9b, "") \
   OP(XKB_KEY_KP_End, 0xff9c, "") \
   OP(XKB_KEY_KP_Begin, 0xff9d, "") \
   OP(XKB_KEY_KP_Insert, 0xff9e, "") \
   OP(XKB_KEY_KP_Delete, 0xff9f, "") \
   OP(XKB_KEY_KP_Equal, 0xffbd, "Equals ") \
   OP(XKB_KEY_KP_Multiply, 0xffaa, "") \
   OP(XKB_KEY_KP_Add, 0xffab, "") \
   OP(XKB_KEY_KP_Separator, 0xffac, "Separator, often comma ") \
   OP(XKB_KEY_KP_Subtract, 0xffad, "") \
   OP(XKB_KEY_KP_Decimal, 0xffae, "") \
   OP(XKB_KEY_KP_Divide, 0xffaf, "") \
   OP(XKB_KEY_KP_0, 0xffb0, "") \
   OP(XKB_KEY_KP_1, 0xffb1, "") \
   OP(XKB_KEY_KP_2, 0xffb2, "") \
   OP(XKB_KEY_KP_3, 0xffb3, "") \
   OP(XKB_KEY_KP_4, 0xffb4, "") \
   OP(XKB_KEY_KP_5, 0xffb5, "") \
   OP(XKB_KEY_KP_6, 0xffb6, "") \
   OP(XKB_KEY_KP_7, 0xffb7, "") \
   OP(XKB_KEY_KP_8, 0xffb8, "") \
   OP(XKB_KEY_KP_9, 0xffb9, "") \
   OP(XKB_KEY_F1, 0xffbe, "") \
   OP(XKB_KEY_F2, 0xffbf, "") \
   OP(XKB_KEY_F3, 0xffc0, "") \
   OP(XKB_KEY_F4, 0xffc1, "") \
   OP(XKB_KEY_F5, 0xffc2, "") \
   OP(XKB_KEY_F6, 0xffc3, "") \
   OP(XKB_KEY_F7, 0xffc4, "") \
   OP(XKB_KEY_F8, 0xffc5, "") \
   OP(XKB_KEY_F9, 0xffc6, "") \
   OP(XKB_KEY_F10, 0xffc7, "") \
   OP(XKB_KEY_F11, 0xffc8, "") \
   OP(XKB_KEY_L1, 0xffc8, "") \
   OP(XKB_KEY_F12, 0xffc9, "") \
   OP(XKB_KEY_L2, 0xffc9, "") \
   OP(XKB_KEY_F13, 0xffca, "") \
   OP(XKB_KEY_L3, 0xffca, "") \
   OP(XKB_KEY_F14, 0xffcb, "") \
   OP(XKB_KEY_L4, 0xffcb, "") \
   OP(XKB_KEY_F15, 0xffcc, "") \
   OP(XKB_KEY_L5, 0xffcc, "") \
   OP(XKB_KEY_F16, 0xffcd, "") \
   OP(XKB_KEY_L6, 0xffcd, "") \
   OP(XKB_KEY_F17, 0xffce, "") \
   OP(XKB_KEY_L7, 0xffce, "") \
   OP(XKB_KEY_F18, 0xffcf, "") \
   OP(XKB_KEY_L8, 0xffcf, "") \
   OP(XKB_KEY_F19, 0xffd0, "") \
   OP(XKB_KEY_L9, 0xffd0, "") \
   OP(XKB_KEY_F20, 0xffd1, "") \
   OP(XKB_KEY_L10, 0xffd1, "") \
   OP(XKB_KEY_F21, 0xffd2, "") \
   OP(XKB_KEY_R1, 0xffd2, "") \
   OP(XKB_KEY_F22, 0xffd3, "") \
   OP(XKB_KEY_R2, 0xffd3, "") \
   OP(XKB_KEY_F23, 0xffd4, "") \
   OP(XKB_KEY_R3, 0xffd4, "") \
   OP(XKB_KEY_F24, 0xffd5, "") \
   OP(XKB_KEY_R4, 0xffd5, "") \
   OP(XKB_KEY_F25, 0xffd6, "") \
   OP(XKB_KEY_R5, 0xffd6, "") \
   OP(XKB_KEY_F26, 0xffd7, "") \
   OP(XKB_KEY_R6, 0xffd7, "") \
   OP(XKB_KEY_F27, 0xffd8, "") \
   OP(XKB_KEY_R7, 0xffd8, "") \
   OP(XKB_KEY_F28, 0xffd9, "") \
   OP(XKB_KEY_R8, 0xffd9, "") \
   OP(XKB_KEY_F29, 0xffda, "") \
   OP(XKB_KEY_R9, 0xffda, "") \
   OP(XKB_KEY_F30, 0xffdb, "") \
   OP(XKB_KEY_R10, 0xffdb, "") \
   OP(XKB_KEY_F31, 0xffdc, "") \
   OP(XKB_KEY_R11, 0xffdc, "") \
   OP(XKB_KEY_F32, 0xffdd, "") \
   OP(XKB_KEY_R12, 0xffdd, "") \
   OP(XKB_KEY_F33, 0xffde, "") \
   OP(XKB_KEY_R13, 0xffde, "") \
   OP(XKB_KEY_F34, 0xffdf, "") \
   OP(XKB_KEY_R14, 0xffdf, "") \
   OP(XKB_KEY_F35, 0xffe0, "") \
   OP(XKB_KEY_R15, 0xffe0, "") \
   OP(XKB_KEY_Shift_L, 0xffe1, "Left shift ") \
   OP(XKB_KEY_Shift_R, 0xffe2, "Right shift ") \
   OP(XKB_KEY_Control_L, 0xffe3, "Left control ") \
   OP(XKB_KEY_Control_R, 0xffe4, "Right control ") \
   OP(XKB_KEY_Caps_Lock, 0xffe5, "Caps lock ") \
   OP(XKB_KEY_Shift_Lock, 0xffe6, "Shift lock ") \
   OP(XKB_KEY_Meta_L, 0xffe7, "Left meta ") \
   OP(XKB_KEY_Meta_R, 0xffe8, "Right meta ") \
   OP(XKB_KEY_Alt_L, 0xffe9, "Left alt ") \
   OP(XKB_KEY_Alt_R, 0xffea, "Right alt ") \
   OP(XKB_KEY_Super_L, 0xffeb, "Left super ") \
   OP(XKB_KEY_Super_R, 0xffec, "Right super ") \
   OP(XKB_KEY_Hyper_L, 0xffed, "Left hyper ") \
   OP(XKB_KEY_Hyper_R, 0xffee, "Right hyper ") \
   OP(XKB_KEY_ISO_Lock, 0xfe01, "") \
   OP(XKB_KEY_ISO_Level2_Latch, 0xfe02, "") \
   OP(XKB_KEY_ISO_Level3_Shift, 0xfe03, "") \
   OP(XKB_KEY_ISO_Level3_Latch, 0xfe04, "") \
   OP(XKB_KEY_ISO_Level3_Lock, 0xfe05, "") \
   OP(XKB_KEY_ISO_Level5_Shift, 0xfe11, "") \
   OP(XKB_KEY_ISO_Level5_Latch, 0xfe12, "") \
   OP(XKB_KEY_ISO_Level5_Lock, 0xfe13, "") \
   OP(XKB_KEY_ISO_Group_Shift, 0xff7e, "Alias for mode_switch ") \
   OP(XKB_KEY_ISO_Group_Latch, 0xfe06, "") \
   OP(XKB_KEY_ISO_Group_Lock, 0xfe07, "") \
   OP(XKB_KEY_ISO_Next_Group, 0xfe08, "") \
   OP(XKB_KEY_ISO_Next_Group_Lock, 0xfe09, "") \
   OP(XKB_KEY_ISO_Prev_Group, 0xfe0a, "") \
   OP(XKB_KEY_ISO_Prev_Group_Lock, 0xfe0b, "") \
   OP(XKB_KEY_ISO_First_Group, 0xfe0c, "") \
   OP(XKB_KEY_ISO_First_Group_Lock, 0xfe0d, "") \
   OP(XKB_KEY_ISO_Last_Group, 0xfe0e, "") \
   OP(XKB_KEY_ISO_Last_Group_Lock, 0xfe0f, "") \
   OP(XKB_KEY_ISO_Left_Tab, 0xfe20, "") \
   OP(XKB_KEY_ISO_Move_Line_Up, 0xfe21, "") \
   OP(XKB_KEY_ISO_Move_Line_Down, 0xfe22, "") \
   OP(XKB_KEY_ISO_Partial_Line_Up, 0xfe23, "") \
   OP(XKB_KEY_ISO_Partial_Line_Down, 0xfe24, "") \
   OP(XKB_KEY_ISO_Partial_Space_Left, 0xfe25, "") \
   OP(XKB_KEY_ISO_Partial_Space_Right, 0xfe26, "") \
   OP(XKB_KEY_ISO_Set_Margin_Left, 0xfe27, "") \
   OP(XKB_KEY_ISO_Set_Margin_Right, 0xfe28, "") \
   OP(XKB_KEY_ISO_Release_Margin_Left, 0xfe29, "") \
   OP(XKB_KEY_ISO_Release_Margin_Right, 0xfe2a, "") \
   OP(XKB_KEY_ISO_Release_Both_Margins, 0xfe2b, "") \
   OP(XKB_KEY_ISO_Fast_Cursor_Left, 0xfe2c, "") \
   OP(XKB_KEY_ISO_Fast_Cursor_Right, 0xfe2d, "") \
   OP(XKB_KEY_ISO_Fast_Cursor_Up, 0xfe2e, "") \
   OP(XKB_KEY_ISO_Fast_Cursor_Down, 0xfe2f, "") \
   OP(XKB_KEY_ISO_Continuous_Underline, 0xfe30, "") \
   OP(XKB_KEY_ISO_Discontinuous_Underline, 0xfe31, "") \
   OP(XKB_KEY_ISO_Emphasize, 0xfe32, "") \
   OP(XKB_KEY_ISO_Center_Object, 0xfe33, "") \
   OP(XKB_KEY_ISO_Enter, 0xfe34, "") \
   OP(XKB_KEY_dead_grave, 0xfe50, "") \
   OP(XKB_KEY_dead_acute, 0xfe51, "") \
   OP(XKB_KEY_dead_circumflex, 0xfe52, "") \
   OP(XKB_KEY_dead_tilde, 0xfe53, "") \
   OP(XKB_KEY_dead_perispomeni, 0xfe53, "alias for dead_tilde ") \
   OP(XKB_KEY_dead_macron, 0xfe54, "") \
   OP(XKB_KEY_dead_breve, 0xfe55, "") \
   OP(XKB_KEY_dead_abovedot, 0xfe56, "") \
   OP(XKB_KEY_dead_diaeresis, 0xfe57, "") \
   OP(XKB_KEY_dead_abovering, 0xfe58, "") \
   OP(XKB_KEY_dead_doubleacute, 0xfe59, "") \
   OP(XKB_KEY_dead_caron, 0xfe5a, "") \
   OP(XKB_KEY_dead_cedilla, 0xfe5b, "") \
   OP(XKB_KEY_dead_ogonek, 0xfe5c, "") \
   OP(XKB_KEY_dead_iota, 0xfe5d, "") \
   OP(XKB_KEY_dead_voiced_sound, 0xfe5e, "") \
   OP(XKB_KEY_dead_semivoiced_sound, 0xfe5f, "") \
   OP(XKB_KEY_dead_belowdot, 0xfe60, "") \
   OP(XKB_KEY_dead_hook, 0xfe61, "") \
   OP(XKB_KEY_dead_horn, 0xfe62, "") \
   OP(XKB_KEY_dead_stroke, 0xfe63, "") \
   OP(XKB_KEY_dead_abovecomma, 0xfe64, "") \
   OP(XKB_KEY_dead_psili, 0xfe64, "alias for dead_abovecomma ") \
   OP(XKB_KEY_dead_abovereversedcomma, 0xfe65, "") \
   OP(XKB_KEY_dead_dasia, 0xfe65, "alias for dead_abovereversedcomma ") \
   OP(XKB_KEY_dead_doublegrave, 0xfe66, "") \
   OP(XKB_KEY_dead_belowring, 0xfe67, "") \
   OP(XKB_KEY_dead_belowmacron, 0xfe68, "") \
   OP(XKB_KEY_dead_belowcircumflex, 0xfe69, "") \
   OP(XKB_KEY_dead_belowtilde, 0xfe6a, "") \
   OP(XKB_KEY_dead_belowbreve, 0xfe6b, "") \
   OP(XKB_KEY_dead_belowdiaeresis, 0xfe6c, "") \
   OP(XKB_KEY_dead_invertedbreve, 0xfe6d, "") \
   OP(XKB_KEY_dead_belowcomma, 0xfe6e, "") \
   OP(XKB_KEY_dead_currency, 0xfe6f, "") \
   OP(XKB_KEY_dead_lowline, 0xfe90, "") \
   OP(XKB_KEY_dead_aboveverticalline, 0xfe91, "") \
   OP(XKB_KEY_dead_belowverticalline, 0xfe92, "") \
   OP(XKB_KEY_dead_longsolidusoverlay, 0xfe93, "") \
   OP(XKB_KEY_dead_a, 0xfe80, "") \
   OP(XKB_KEY_dead_A, 0xfe81, "") \
   OP(XKB_KEY_dead_e, 0xfe82, "") \
   OP(XKB_KEY_dead_E, 0xfe83, "") \
   OP(XKB_KEY_dead_i, 0xfe84, "") \
   OP(XKB_KEY_dead_I, 0xfe85, "") \
   OP(XKB_KEY_dead_o, 0xfe86, "") \
   OP(XKB_KEY_dead_O, 0xfe87, "") \
   OP(XKB_KEY_dead_u, 0xfe88, "") \
   OP(XKB_KEY_dead_U, 0xfe89, "") \
   OP(XKB_KEY_dead_small_schwa, 0xfe8a, "") \
   OP(XKB_KEY_dead_capital_schwa, 0xfe8b, "") \
   OP(XKB_KEY_dead_greek, 0xfe8c, "") \
   OP(XKB_KEY_First_Virtual_Screen, 0xfed0, "") \
   OP(XKB_KEY_Prev_Virtual_Screen, 0xfed1, "") \
   OP(XKB_KEY_Next_Virtual_Screen, 0xfed2, "") \
   OP(XKB_KEY_Last_Virtual_Screen, 0xfed4, "") \
   OP(XKB_KEY_Terminate_Server, 0xfed5, "") \
   OP(XKB_KEY_AccessX_Enable, 0xfe70, "") \
   OP(XKB_KEY_AccessX_Feedback_Enable, 0xfe71, "") \
   OP(XKB_KEY_RepeatKeys_Enable, 0xfe72, "") \
   OP(XKB_KEY_SlowKeys_Enable, 0xfe73, "") \
   OP(XKB_KEY_BounceKeys_Enable, 0xfe74, "") \
   OP(XKB_KEY_StickyKeys_Enable, 0xfe75, "") \
   OP(XKB_KEY_MouseKeys_Enable, 0xfe76, "") \
   OP(XKB_KEY_MouseKeys_Accel_Enable, 0xfe77, "") \
   OP(XKB_KEY_Overlay1_Enable, 0xfe78, "") \
   OP(XKB_KEY_Overlay2_Enable, 0xfe79, "") \
   OP(XKB_KEY_AudibleBell_Enable, 0xfe7a, "") \
   OP(XKB_KEY_Pointer_Left, 0xfee0, "") \
   OP(XKB_KEY_Pointer_Right, 0xfee1, "") \
   OP(XKB_KEY_Pointer_Up, 0xfee2, "") \
   OP(XKB_KEY_Pointer_Down, 0xfee3, "") \
   OP(XKB_KEY_Pointer_UpLeft, 0xfee4, "") \
   OP(XKB_KEY_Pointer_UpRight, 0xfee5, "") \
   OP(XKB_KEY_Pointer_DownLeft, 0xfee6, "") \
   OP(XKB_KEY_Pointer_DownRight, 0xfee7, "") \
   OP(XKB_KEY_Pointer_Button_Dflt, 0xfee8, "") \
   OP(XKB_KEY_Pointer_Button1, 0xfee9, "") \
   OP(XKB_KEY_Pointer_Button2, 0xfeea, "") \
   OP(XKB_KEY_Pointer_Button3, 0xfeeb, "") \
   OP(XKB_KEY_Pointer_Button4, 0xfeec, "") \
   OP(XKB_KEY_Pointer_Button5, 0xfeed, "") \
   OP(XKB_KEY_Pointer_DblClick_Dflt, 0xfeee, "") \
   OP(XKB_KEY_Pointer_DblClick1, 0xfeef, "") \
   OP(XKB_KEY_Pointer_DblClick2, 0xfef0, "") \
   OP(XKB_KEY_Pointer_DblClick3, 0xfef1, "") \
   OP(XKB_KEY_Pointer_DblClick4, 0xfef2, "") \
   OP(XKB_KEY_Pointer_DblClick5, 0xfef3, "") \
   OP(XKB_KEY_Pointer_Drag_Dflt, 0xfef4, "") \
   OP(XKB_KEY_Pointer_Drag1, 0xfef5, "") \
   OP(XKB_KEY_Pointer_Drag2, 0xfef6, "") \
   OP(XKB_KEY_Pointer_Drag3, 0xfef7, "") \
   OP(XKB_KEY_Pointer_Drag4, 0xfef8, "") \
   OP(XKB_KEY_Pointer_Drag5, 0xfefd, "") \
   OP(XKB_KEY_Pointer_EnableKeys, 0xfef9, "") \
   OP(XKB_KEY_Pointer_Accelerate, 0xfefa, "") \
   OP(XKB_KEY_Pointer_DfltBtnNext, 0xfefb, "") \
   OP(XKB_KEY_Pointer_DfltBtnPrev, 0xfefc, "") \
   OP(XKB_KEY_ch, 0xfea0, "") \
   OP(XKB_KEY_Ch, 0xfea1, "") \
   OP(XKB_KEY_CH, 0xfea2, "") \
   OP(XKB_KEY_c_h, 0xfea3, "") \
   OP(XKB_KEY_C_h, 0xfea4, "") \
   OP(XKB_KEY_C_H, 0xfea5, "") \
   OP(XKB_KEY_3270_Duplicate, 0xfd01, "") \
   OP(XKB_KEY_3270_FieldMark, 0xfd02, "") \
   OP(XKB_KEY_3270_Right2, 0xfd03, "") \
   OP(XKB_KEY_3270_Left2, 0xfd04, "") \
   OP(XKB_KEY_3270_BackTab, 0xfd05, "") \
   OP(XKB_KEY_3270_EraseEOF, 0xfd06, "") \
   OP(XKB_KEY_3270_EraseInput, 0xfd07, "") \
   OP(XKB_KEY_3270_Reset, 0xfd08, "") \
   OP(XKB_KEY_3270_Quit, 0xfd09, "") \
   OP(XKB_KEY_3270_PA1, 0xfd0a, "") \
   OP(XKB_KEY_3270_PA2, 0xfd0b, "") \
   OP(XKB_KEY_3270_PA3, 0xfd0c, "") \
   OP(XKB_KEY_3270_Test, 0xfd0d, "") \
   OP(XKB_KEY_3270_Attn, 0xfd0e, "") \
   OP(XKB_KEY_3270_CursorBlink, 0xfd0f, "") \
   OP(XKB_KEY_3270_AltCursor, 0xfd10, "") \
   OP(XKB_KEY_3270_KeyClick, 0xfd11, "") \
   OP(XKB_KEY_3270_Jump, 0xfd12, "") \
   OP(XKB_KEY_3270_Ident, 0xfd13, "") \
   OP(XKB_KEY_3270_Rule, 0xfd14, "") \
   OP(XKB_KEY_3270_Copy, 0xfd15, "") \
   OP(XKB_KEY_3270_Play, 0xfd16, "") \
   OP(XKB_KEY_3270_Setup, 0xfd17, "") \
   OP(XKB_KEY_3270_Record, 0xfd18, "") \
   OP(XKB_KEY_3270_ChangeScreen, 0xfd19, "") \
   OP(XKB_KEY_3270_DeleteWord, 0xfd1a, "") \
   OP(XKB_KEY_3270_ExSelect, 0xfd1b, "") \
   OP(XKB_KEY_3270_CursorSelect, 0xfd1c, "") \
   OP(XKB_KEY_3270_PrintScreen, 0xfd1d, "") \
   OP(XKB_KEY_3270_Enter, 0xfd1e, "") \
   OP(XKB_KEY_quoteright, 0x0027, "deprecated ") \
   OP(XKB_KEY_quoteleft, 0x0060, "deprecated ") \
   OP(XKB_KEY_Eth, 0x00d0, "deprecated ") \
   OP(XKB_KEY_Thorn, 0x00de, "deprecated ") \
   OP(XKB_KEY_kappa, 0x03a2, "deprecated ") \
   OP(XKB_KEY_kana_middledot, 0x04a5, "deprecated ") \
   OP(XKB_KEY_kana_tu, 0x04af, "deprecated ") \
   OP(XKB_KEY_kana_TI, 0x04c1, "deprecated ") \
   OP(XKB_KEY_kana_TU, 0x04c2, "deprecated ") \
   OP(XKB_KEY_kana_HU, 0x04cc, "deprecated ") \
   OP(XKB_KEY_kana_switch, 0xff7e, "Alias for mode_switch ") \
   OP(XKB_KEY_Arabic_heh, 0x05e7, "deprecated ") \
   OP(XKB_KEY_Arabic_switch, 0xff7e, "Alias for mode_switch ") \
   OP(XKB_KEY_Ukranian_je, 0x06a4, "deprecated ") \
   OP(XKB_KEY_Ukranian_i, 0x06a6, "deprecated ") \
   OP(XKB_KEY_Ukranian_yi, 0x06a7, "deprecated ") \
   OP(XKB_KEY_Serbian_je, 0x06a8, "deprecated ") \
   OP(XKB_KEY_Serbian_lje, 0x06a9, "deprecated ") \
   OP(XKB_KEY_Serbian_nje, 0x06aa, "deprecated ") \
   OP(XKB_KEY_Serbian_dze, 0x06af, "deprecated ") \
   OP(XKB_KEY_Ukranian_JE, 0x06b4, "deprecated ") \
   OP(XKB_KEY_Ukranian_I, 0x06b6, "deprecated ") \
   OP(XKB_KEY_Ukranian_YI, 0x06b7, "deprecated ") \
   OP(XKB_KEY_Serbian_JE, 0x06b8, "deprecated ") \
   OP(XKB_KEY_Serbian_LJE, 0x06b9, "deprecated ") \
   OP(XKB_KEY_Serbian_NJE, 0x06ba, "deprecated ") \
   OP(XKB_KEY_Serbian_DZE, 0x06bf, "deprecated ") \
   OP(XKB_KEY_Greek_IOTAdiaeresis, 0x07a5, "old typo ") \
   OP(XKB_KEY_Greek_switch, 0xff7e, "Alias for mode_switch ") \
   OP(XKB_KEY_topleftsummation, 0x08b1, "") \
   OP(XKB_KEY_botleftsummation, 0x08b2, "") \
   OP(XKB_KEY_topvertsummationconnector, 0x08b3, "") \
   OP(XKB_KEY_botvertsummationconnector, 0x08b4, "") \
   OP(XKB_KEY_toprightsummation, 0x08b5, "") \
   OP(XKB_KEY_botrightsummation, 0x08b6, "") \
   OP(XKB_KEY_rightmiddlesummation, 0x08b7, "") \
   OP(XKB_KEY_blank, 0x09df, "") \
   OP(XKB_KEY_marker, 0x0abf, "") \
   OP(XKB_KEY_trademarkincircle, 0x0acb, "") \
   OP(XKB_KEY_hexagram, 0x0ada, "") \
   OP(XKB_KEY_cursor, 0x0aff, "") \
   OP(XKB_KEY_hebrew_beth, 0x0ce1, "deprecated ") \
   OP(XKB_KEY_hebrew_gimmel, 0x0ce2, "deprecated ") \
   OP(XKB_KEY_hebrew_daleth, 0x0ce3, "deprecated ") \
   OP(XKB_KEY_hebrew_zayin, 0x0ce6, "deprecated ") \
   OP(XKB_KEY_hebrew_het, 0x0ce7, "deprecated ") \
   OP(XKB_KEY_hebrew_teth, 0x0ce8, "deprecated ") \
   OP(XKB_KEY_hebrew_samekh, 0x0cf1, "deprecated ") \
   OP(XKB_KEY_hebrew_finalzadi, 0x0cf5, "deprecated ") \
   OP(XKB_KEY_hebrew_zadi, 0x0cf6, "deprecated ") \
   OP(XKB_KEY_hebrew_kuf, 0x0cf7, "deprecated ") \
   OP(XKB_KEY_hebrew_taf, 0x0cfa, "deprecated ") \
   OP(XKB_KEY_Hebrew_switch, 0xff7e, "Alias for mode_switch ") \
   OP(XKB_KEY_Thai_maihanakat_maitho, 0x0dde, "") \
   OP(XKB_KEY_Hangul, 0xff31, "Hangul start/stop(toggle) ") \
   OP(XKB_KEY_Hangul_Start, 0xff32, "Hangul start ") \
   OP(XKB_KEY_Hangul_End, 0xff33, "Hangul end, English start ") \
   OP(XKB_KEY_Hangul_Hanja, 0xff34, "Start Hangul->Hanja Conversion ") \
   OP(XKB_KEY_Hangul_Jamo, 0xff35, "Hangul Jamo mode ") \
   OP(XKB_KEY_Hangul_Romaja, 0xff36, "Hangul Romaja mode ") \
   OP(XKB_KEY_Hangul_Codeinput, 0xff37, "Hangul code input mode ") \
   OP(XKB_KEY_Hangul_Jeonja, 0xff38, "Jeonja mode ") \
   OP(XKB_KEY_Hangul_Banja, 0xff39, "Banja mode ") \
   OP(XKB_KEY_Hangul_PreHanja, 0xff3a, "Pre Hanja conversion ") \
   OP(XKB_KEY_Hangul_PostHanja, 0xff3b, "Post Hanja conversion ") \
   OP(XKB_KEY_Hangul_SingleCandidate, 0xff3c, "Single candidate ") \
   OP(XKB_KEY_Hangul_MultipleCandidate, 0xff3d, "Multiple candidate ") \
   OP(XKB_KEY_Hangul_PreviousCandidate, 0xff3e, "Previous candidate ") \
   OP(XKB_KEY_Hangul_Special, 0xff3f, "Special symbols ") \
   OP(XKB_KEY_Hangul_switch, 0xff7e, "Alias for mode_switch ") \
   OP(XKB_KEY_Hangul_Kiyeog, 0x0ea1, "") \
   OP(XKB_KEY_Hangul_SsangKiyeog, 0x0ea2, "") \
   OP(XKB_KEY_Hangul_KiyeogSios, 0x0ea3, "") \
   OP(XKB_KEY_Hangul_Nieun, 0x0ea4, "") \
   OP(XKB_KEY_Hangul_NieunJieuj, 0x0ea5, "") \
   OP(XKB_KEY_Hangul_NieunHieuh, 0x0ea6, "") \
   OP(XKB_KEY_Hangul_Dikeud, 0x0ea7, "") \
   OP(XKB_KEY_Hangul_SsangDikeud, 0x0ea8, "") \
   OP(XKB_KEY_Hangul_Rieul, 0x0ea9, "") \
   OP(XKB_KEY_Hangul_RieulKiyeog, 0x0eaa, "") \
   OP(XKB_KEY_Hangul_RieulMieum, 0x0eab, "") \
   OP(XKB_KEY_Hangul_RieulPieub, 0x0eac, "") \
   OP(XKB_KEY_Hangul_RieulSios, 0x0ead, "") \
   OP(XKB_KEY_Hangul_RieulTieut, 0x0eae, "") \
   OP(XKB_KEY_Hangul_RieulPhieuf, 0x0eaf, "") \
   OP(XKB_KEY_Hangul_RieulHieuh, 0x0eb0, "") \
   OP(XKB_KEY_Hangul_Mieum, 0x0eb1, "") \
   OP(XKB_KEY_Hangul_Pieub, 0x0eb2, "") \
   OP(XKB_KEY_Hangul_SsangPieub, 0x0eb3, "") \
   OP(XKB_KEY_Hangul_PieubSios, 0x0eb4, "") \
   OP(XKB_KEY_Hangul_Sios, 0x0eb5, "") \
   OP(XKB_KEY_Hangul_SsangSios, 0x0eb6, "") \
   OP(XKB_KEY_Hangul_Ieung, 0x0eb7, "") \
   OP(XKB_KEY_Hangul_Jieuj, 0x0eb8, "") \
   OP(XKB_KEY_Hangul_SsangJieuj, 0x0eb9, "") \
   OP(XKB_KEY_Hangul_Cieuc, 0x0eba, "") \
   OP(XKB_KEY_Hangul_Khieuq, 0x0ebb, "") \
   OP(XKB_KEY_Hangul_Tieut, 0x0ebc, "") \
   OP(XKB_KEY_Hangul_Phieuf, 0x0ebd, "") \
   OP(XKB_KEY_Hangul_Hieuh, 0x0ebe, "") \
   OP(XKB_KEY_Hangul_A, 0x0ebf, "") \
   OP(XKB_KEY_Hangul_AE, 0x0ec0, "") \
   OP(XKB_KEY_Hangul_YA, 0x0ec1, "") \
   OP(XKB_KEY_Hangul_YAE, 0x0ec2, "") \
   OP(XKB_KEY_Hangul_EO, 0x0ec3, "") \
   OP(XKB_KEY_Hangul_E, 0x0ec4, "") \
   OP(XKB_KEY_Hangul_YEO, 0x0ec5, "") \
   OP(XKB_KEY_Hangul_YE, 0x0ec6, "") \
   OP(XKB_KEY_Hangul_O, 0x0ec7, "") \
   OP(XKB_KEY_Hangul_WA, 0x0ec8, "") \
   OP(XKB_KEY_Hangul_WAE, 0x0ec9, "") \
   OP(XKB_KEY_Hangul_OE, 0x0eca, "") \
   OP(XKB_KEY_Hangul_YO, 0x0ecb, "") \
   OP(XKB_KEY_Hangul_U, 0x0ecc, "") \
   OP(XKB_KEY_Hangul_WEO, 0x0ecd, "") \
   OP(XKB_KEY_Hangul_WE, 0x0ece, "") \
   OP(XKB_KEY_Hangul_WI, 0x0ecf, "") \
   OP(XKB_KEY_Hangul_YU, 0x0ed0, "") \
   OP(XKB_KEY_Hangul_EU, 0x0ed1, "") \
   OP(XKB_KEY_Hangul_YI, 0x0ed2, "") \
   OP(XKB_KEY_Hangul_I, 0x0ed3, "") \
   OP(XKB_KEY_Hangul_J_Kiyeog, 0x0ed4, "") \
   OP(XKB_KEY_Hangul_J_SsangKiyeog, 0x0ed5, "") \
   OP(XKB_KEY_Hangul_J_KiyeogSios, 0x0ed6, "") \
   OP(XKB_KEY_Hangul_J_Nieun, 0x0ed7, "") \
   OP(XKB_KEY_Hangul_J_NieunJieuj, 0x0ed8, "") \
   OP(XKB_KEY_Hangul_J_NieunHieuh, 0x0ed9, "") \
   OP(XKB_KEY_Hangul_J_Dikeud, 0x0eda, "") \
   OP(XKB_KEY_Hangul_J_Rieul, 0x0edb, "") \
   OP(XKB_KEY_Hangul_J_RieulKiyeog, 0x0edc, "") \
   OP(XKB_KEY_Hangul_J_RieulMieum, 0x0edd, "") \
   OP(XKB_KEY_Hangul_J_RieulPieub, 0x0ede, "") \
   OP(XKB_KEY_Hangul_J_RieulSios, 0x0edf, "") \
   OP(XKB_KEY_Hangul_J_RieulTieut, 0x0ee0, "") \
   OP(XKB_KEY_Hangul_J_RieulPhieuf, 0x0ee1, "") \
   OP(XKB_KEY_Hangul_J_RieulHieuh, 0x0ee2, "") \
   OP(XKB_KEY_Hangul_J_Mieum, 0x0ee3, "") \
   OP(XKB_KEY_Hangul_J_Pieub, 0x0ee4, "") \
   OP(XKB_KEY_Hangul_J_PieubSios, 0x0ee5, "") \
   OP(XKB_KEY_Hangul_J_Sios, 0x0ee6, "") \
   OP(XKB_KEY_Hangul_J_SsangSios, 0x0ee7, "") \
   OP(XKB_KEY_Hangul_J_Ieung, 0x0ee8, "") \
   OP(XKB_KEY_Hangul_J_Jieuj, 0x0ee9, "") \
   OP(XKB_KEY_Hangul_J_Cieuc, 0x0eea, "") \
   OP(XKB_KEY_Hangul_J_Khieuq, 0x0eeb, "") \
   OP(XKB_KEY_Hangul_J_Tieut, 0x0eec, "") \
   OP(XKB_KEY_Hangul_J_Phieuf, 0x0eed, "") \
   OP(XKB_KEY_Hangul_J_Hieuh, 0x0eee, "") \
   OP(XKB_KEY_Hangul_RieulYeorinHieuh, 0x0eef, "") \
   OP(XKB_KEY_Hangul_SunkyeongeumMieum, 0x0ef0, "") \
   OP(XKB_KEY_Hangul_SunkyeongeumPieub, 0x0ef1, "") \
   OP(XKB_KEY_Hangul_PanSios, 0x0ef2, "") \
   OP(XKB_KEY_Hangul_KkogjiDalrinIeung, 0x0ef3, "") \
   OP(XKB_KEY_Hangul_SunkyeongeumPhieuf, 0x0ef4, "") \
   OP(XKB_KEY_Hangul_YeorinHieuh, 0x0ef5, "") \
   OP(XKB_KEY_Hangul_AraeA, 0x0ef6, "") \
   OP(XKB_KEY_Hangul_AraeAE, 0x0ef7, "") \
   OP(XKB_KEY_Hangul_J_PanSios, 0x0ef8, "") \
   OP(XKB_KEY_Hangul_J_KkogjiDalrinIeung, 0x0ef9, "") \
   OP(XKB_KEY_Hangul_J_YeorinHieuh, 0x0efa, "") \
   OP(XKB_KEY_braille_dot_1, 0xfff1, "") \
   OP(XKB_KEY_braille_dot_2, 0xfff2, "") \
   OP(XKB_KEY_braille_dot_3, 0xfff3, "") \
   OP(XKB_KEY_braille_dot_4, 0xfff4, "") \
   OP(XKB_KEY_braille_dot_5, 0xfff5, "") \
   OP(XKB_KEY_braille_dot_6, 0xfff6, "") \
   OP(XKB_KEY_braille_dot_7, 0xfff7, "") \
   OP(XKB_KEY_braille_dot_8, 0xfff8, "") \
   OP(XKB_KEY_braille_dot_9, 0xfff9, "") \
   OP(XKB_KEY_braille_dot_10, 0xfffa, "") \
   OP(XKB_KEY_XF86ModeLock, 0x1008FF01, "Mode Switch Lock ") \
   OP(XKB_KEY_XF86MonBrightnessUp, 0x1008FF02, "Monitor/panel brightness ") \
   OP(XKB_KEY_XF86MonBrightnessDown, 0x1008FF03, "Monitor/panel brightness ") \
   OP(XKB_KEY_XF86KbdLightOnOff, 0x1008FF04, "Keyboards may be lit     ") \
   OP(XKB_KEY_XF86KbdBrightnessUp, 0x1008FF05, "Keyboards may be lit     ") \
   OP(XKB_KEY_XF86KbdBrightnessDown, 0x1008FF06, "Keyboards may be lit     ") \
   OP(XKB_KEY_XF86Standby, 0x1008FF10, "System into standby mode   ") \
   OP(XKB_KEY_XF86AudioLowerVolume, 0x1008FF11, "Volume control down        ") \
   OP(XKB_KEY_XF86AudioMute, 0x1008FF12, "Mute sound from the system ") \
   OP(XKB_KEY_XF86AudioRaiseVolume, 0x1008FF13, "Volume control up          ") \
   OP(XKB_KEY_XF86AudioPlay, 0x1008FF14, "Start playing of audio >   ") \
   OP(XKB_KEY_XF86AudioStop, 0x1008FF15, "Stop playing audio         ") \
   OP(XKB_KEY_XF86AudioPrev, 0x1008FF16, "Previous track             ") \
   OP(XKB_KEY_XF86AudioNext, 0x1008FF17, "Next track                 ") \
   OP(XKB_KEY_XF86HomePage, 0x1008FF18, "Display user's home page   ") \
   OP(XKB_KEY_XF86Mail, 0x1008FF19, "Invoke user's mail program ") \
   OP(XKB_KEY_XF86Start, 0x1008FF1A, "Start application          ") \
   OP(XKB_KEY_XF86Search, 0x1008FF1B, "Search                     ") \
   OP(XKB_KEY_XF86AudioRecord, 0x1008FF1C, "Record audio application   ") \
   OP(XKB_KEY_XF86Calculator, 0x1008FF1D, "Invoke calculator program  ") \
   OP(XKB_KEY_XF86Memo, 0x1008FF1E, "Invoke Memo taking program ") \
   OP(XKB_KEY_XF86ToDoList, 0x1008FF1F, "Invoke To Do List program  ") \
   OP(XKB_KEY_XF86Calendar, 0x1008FF20, "Invoke Calendar program    ") \
   OP(XKB_KEY_XF86PowerDown, 0x1008FF21, "Deep sleep the system      ") \
   OP(XKB_KEY_XF86ContrastAdjust, 0x1008FF22, "Adjust screen contrast     ") \
   OP(XKB_KEY_XF86RockerUp, 0x1008FF23, "Rocker switches exist up   ") \
   OP(XKB_KEY_XF86RockerDown, 0x1008FF24, "and down                   ") \
   OP(XKB_KEY_XF86RockerEnter, 0x1008FF25, "and let you press them     ") \
   OP(XKB_KEY_XF86Back, 0x1008FF26, "Like back on a browser     ") \
   OP(XKB_KEY_XF86Forward, 0x1008FF27, "Like forward on a browser  ") \
   OP(XKB_KEY_XF86Stop, 0x1008FF28, "Stop current operation     ") \
   OP(XKB_KEY_XF86Refresh, 0x1008FF29, "Refresh the page           ") \
   OP(XKB_KEY_XF86PowerOff, 0x1008FF2A, "Power off system entirely  ") \
   OP(XKB_KEY_XF86WakeUp, 0x1008FF2B, "Wake up system from sleep  ") \
   OP(XKB_KEY_XF86Eject, 0x1008FF2C, "Eject device (e.g. DVD)    ") \
   OP(XKB_KEY_XF86ScreenSaver, 0x1008FF2D, "Invoke screensaver         ") \
   OP(XKB_KEY_XF86WWW, 0x1008FF2E, "Invoke web browser         ") \
   OP(XKB_KEY_XF86Sleep, 0x1008FF2F, "Put system to sleep        ") \
   OP(XKB_KEY_XF86Favorites, 0x1008FF30, "Show favorite locations    ") \
   OP(XKB_KEY_XF86AudioPause, 0x1008FF31, "Pause audio playing        ") \
   OP(XKB_KEY_XF86AudioMedia, 0x1008FF32, "Launch media collection app ") \
   OP(XKB_KEY_XF86MyComputer, 0x1008FF33, "Display \"My Computer\" window ") \
   OP(XKB_KEY_XF86VendorHome, 0x1008FF34, "Display vendor home web site ") \
   OP(XKB_KEY_XF86LightBulb, 0x1008FF35, "Light bulb keys exist       ") \
   OP(XKB_KEY_XF86Shop, 0x1008FF36, "Display shopping web site   ") \
   OP(XKB_KEY_XF86History, 0x1008FF37, "Show history of web surfing ") \
   OP(XKB_KEY_XF86OpenURL, 0x1008FF38, "Open selected URL           ") \
   OP(XKB_KEY_XF86AddFavorite, 0x1008FF39, "Add URL to favorites list   ") \
   OP(XKB_KEY_XF86HotLinks, 0x1008FF3A, "Show \"hot\" links            ") \
   OP(XKB_KEY_XF86BrightnessAdjust, 0x1008FF3B, "Invoke brightness adj. UI   ") \
   OP(XKB_KEY_XF86Finance, 0x1008FF3C, "Display financial site      ") \
   OP(XKB_KEY_XF86Community, 0x1008FF3D, "Display user's community    ") \
   OP(XKB_KEY_XF86AudioRewind, 0x1008FF3E, "\"rewind\" audio track        ") \
   OP(XKB_KEY_XF86BackForward, 0x1008FF3F, "??? ") \
   OP(XKB_KEY_XF86Launch0, 0x1008FF40, "Launch Application          ") \
   OP(XKB_KEY_XF86Launch1, 0x1008FF41, "Launch Application          ") \
   OP(XKB_KEY_XF86Launch2, 0x1008FF42, "Launch Application          ") \
   OP(XKB_KEY_XF86Launch3, 0x1008FF43, "Launch Application          ") \
   OP(XKB_KEY_XF86Launch4, 0x1008FF44, "Launch Application          ") \
   OP(XKB_KEY_XF86Launch5, 0x1008FF45, "Launch Application          ") \
   OP(XKB_KEY_XF86Launch6, 0x1008FF46, "Launch Application          ") \
   OP(XKB_KEY_XF86Launch7, 0x1008FF47, "Launch Application          ") \
   OP(XKB_KEY_XF86Launch8, 0x1008FF48, "Launch Application          ") \
   OP(XKB_KEY_XF86Launch9, 0x1008FF49, "Launch Application          ") \
   OP(XKB_KEY_XF86LaunchA, 0x1008FF4A, "Launch Application          ") \
   OP(XKB_KEY_XF86LaunchB, 0x1008FF4B, "Launch Application          ") \
   OP(XKB_KEY_XF86LaunchC, 0x1008FF4C, "Launch Application          ") \
   OP(XKB_KEY_XF86LaunchD, 0x1008FF4D, "Launch Application          ") \
   OP(XKB_KEY_XF86LaunchE, 0x1008FF4E, "Launch Application          ") \
   OP(XKB_KEY_XF86LaunchF, 0x1008FF4F, "Launch Application          ") \
   OP(XKB_KEY_XF86ApplicationLeft, 0x1008FF50, "switch to application, left ") \
   OP(XKB_KEY_XF86ApplicationRight, 0x1008FF51, "switch to application, right") \
   OP(XKB_KEY_XF86Book, 0x1008FF52, "Launch bookreader           ") \
   OP(XKB_KEY_XF86CD, 0x1008FF53, "Launch CD/DVD player        ") \
   OP(XKB_KEY_XF86Calculater, 0x1008FF54, "Launch Calculater           ") \
   OP(XKB_KEY_XF86Clear, 0x1008FF55, "Clear window, screen        ") \
   OP(XKB_KEY_XF86Close, 0x1008FF56, "Close window                ") \
   OP(XKB_KEY_XF86Copy, 0x1008FF57, "Copy selection              ") \
   OP(XKB_KEY_XF86Cut, 0x1008FF58, "Cut selection               ") \
   OP(XKB_KEY_XF86Display, 0x1008FF59, "Output switch key           ") \
   OP(XKB_KEY_XF86DOS, 0x1008FF5A, "Launch DOS (emulation)      ") \
   OP(XKB_KEY_XF86Documents, 0x1008FF5B, "Open documents window       ") \
   OP(XKB_KEY_XF86Excel, 0x1008FF5C, "Launch spread sheet         ") \
   OP(XKB_KEY_XF86Explorer, 0x1008FF5D, "Launch file explorer        ") \
   OP(XKB_KEY_XF86Game, 0x1008FF5E, "Launch game                 ") \
   OP(XKB_KEY_XF86Go, 0x1008FF5F, "Go to URL                   ") \
   OP(XKB_KEY_XF86iTouch, 0x1008FF60, "Logitch iTouch- don't use   ") \
   OP(XKB_KEY_XF86LogOff, 0x1008FF61, "Log off system              ") \
   OP(XKB_KEY_XF86Market, 0x1008FF62, "??                          ") \
   OP(XKB_KEY_XF86Meeting, 0x1008FF63, "enter meeting in calendar   ") \
   OP(XKB_KEY_XF86MenuKB, 0x1008FF65, "distingush keyboard from PB ") \
   OP(XKB_KEY_XF86MenuPB, 0x1008FF66, "distinuish PB from keyboard ") \
   OP(XKB_KEY_XF86MySites, 0x1008FF67, "Favourites                  ") \
   OP(XKB_KEY_XF86New, 0x1008FF68, "New (folder, document...    ") \
   OP(XKB_KEY_XF86News, 0x1008FF69, "News                        ") \
   OP(XKB_KEY_XF86OfficeHome, 0x1008FF6A, "Office home (old Staroffice)") \
   OP(XKB_KEY_XF86Open, 0x1008FF6B, "Open                        ") \
   OP(XKB_KEY_XF86Option, 0x1008FF6C, "?? ") \
   OP(XKB_KEY_XF86Paste, 0x1008FF6D, "Paste                       ") \
   OP(XKB_KEY_XF86Phone, 0x1008FF6E, "Launch phone; dial number   ") \
   OP(XKB_KEY_XF86Q, 0x1008FF70, "Compaq's Q - don't use      ") \
   OP(XKB_KEY_XF86Reply, 0x1008FF72, "Reply e.g., mail            ") \
   OP(XKB_KEY_XF86Reload, 0x1008FF73, "Reload web page, file, etc. ") \
   OP(XKB_KEY_XF86RotateWindows, 0x1008FF74, "Rotate windows e.g. xrandr  ") \
   OP(XKB_KEY_XF86RotationPB, 0x1008FF75, "don't use                   ") \
   OP(XKB_KEY_XF86RotationKB, 0x1008FF76, "don't use                   ") \
   OP(XKB_KEY_XF86Save, 0x1008FF77, "Save (file, document, state ") \
   OP(XKB_KEY_XF86ScrollUp, 0x1008FF78, "Scroll window/contents up   ") \
   OP(XKB_KEY_XF86ScrollDown, 0x1008FF79, "Scrool window/contentd down ") \
   OP(XKB_KEY_XF86ScrollClick, 0x1008FF7A, "Use XKB mousekeys instead   ") \
   OP(XKB_KEY_XF86Send, 0x1008FF7B, "Send mail, file, object     ") \
   OP(XKB_KEY_XF86Spell, 0x1008FF7C, "Spell checker               ") \
   OP(XKB_KEY_XF86SplitScreen, 0x1008FF7D, "Split window or screen      ") \
   OP(XKB_KEY_XF86Support, 0x1008FF7E, "Get support (\?\?)            ") \
   OP(XKB_KEY_XF86TaskPane, 0x1008FF7F, "Show tasks ") \
   OP(XKB_KEY_XF86Terminal, 0x1008FF80, "Launch terminal emulator    ") \
   OP(XKB_KEY_XF86Tools, 0x1008FF81, "toolbox of desktop/app.     ") \
   OP(XKB_KEY_XF86Travel, 0x1008FF82, "?? ") \
   OP(XKB_KEY_XF86UserPB, 0x1008FF84, "?? ") \
   OP(XKB_KEY_XF86User1KB, 0x1008FF85, "?? ") \
   OP(XKB_KEY_XF86User2KB, 0x1008FF86, "?? ") \
   OP(XKB_KEY_XF86Video, 0x1008FF87, "Launch video player       ") \
   OP(XKB_KEY_XF86WheelButton, 0x1008FF88, "button from a mouse wheel ") \
   OP(XKB_KEY_XF86Word, 0x1008FF89, "Launch word processor     ") \
   OP(XKB_KEY_XF86Xfer, 0x1008FF8A, "") \
   OP(XKB_KEY_XF86ZoomIn, 0x1008FF8B, "zoom in view, map, etc.   ") \
   OP(XKB_KEY_XF86ZoomOut, 0x1008FF8C, "zoom out view, map, etc.  ") \
   OP(XKB_KEY_XF86Away, 0x1008FF8D, "mark yourself as away     ") \
   OP(XKB_KEY_XF86Messenger, 0x1008FF8E, "as in instant messaging   ") \
   OP(XKB_KEY_XF86WebCam, 0x1008FF8F, "Launch web camera app.    ") \
   OP(XKB_KEY_XF86MailForward, 0x1008FF90, "Forward in mail           ") \
   OP(XKB_KEY_XF86Pictures, 0x1008FF91, "Show pictures             ") \
   OP(XKB_KEY_XF86Music, 0x1008FF92, "Launch music application  ") \
   OP(XKB_KEY_XF86Battery, 0x1008FF93, "Display battery information ") \
   OP(XKB_KEY_XF86Bluetooth, 0x1008FF94, "Enable/disable Bluetooth    ") \
   OP(XKB_KEY_XF86WLAN, 0x1008FF95, "Enable/disable WLAN         ") \
   OP(XKB_KEY_XF86UWB, 0x1008FF96, "Enable/disable UWB	    ") \
   OP(XKB_KEY_XF86AudioForward, 0x1008FF97, "fast-forward audio track    ") \
   OP(XKB_KEY_XF86AudioRepeat, 0x1008FF98, "toggle repeat mode          ") \
   OP(XKB_KEY_XF86AudioRandomPlay, 0x1008FF99, "toggle shuffle mode         ") \
   OP(XKB_KEY_XF86Subtitle, 0x1008FF9A, "cycle through subtitle      ") \
   OP(XKB_KEY_XF86AudioCycleTrack, 0x1008FF9B, "cycle through audio tracks  ") \
   OP(XKB_KEY_XF86CycleAngle, 0x1008FF9C, "cycle through angles        ") \
   OP(XKB_KEY_XF86FrameBack, 0x1008FF9D, "video: go one frame back    ") \
   OP(XKB_KEY_XF86FrameForward, 0x1008FF9E, "video: go one frame forward ") \
   OP(XKB_KEY_XF86Time, 0x1008FF9F, "display, or shows an entry for time seeking ") \
   OP(XKB_KEY_XF86Select, 0x1008FFA0, "Select button on joypads and remotes ") \
   OP(XKB_KEY_XF86View, 0x1008FFA1, "Show a view options/properties ") \
   OP(XKB_KEY_XF86TopMenu, 0x1008FFA2, "Go to a top-level menu in a video ") \
   OP(XKB_KEY_XF86Red, 0x1008FFA3, "Red button                  ") \
   OP(XKB_KEY_XF86Green, 0x1008FFA4, "Green button                ") \
   OP(XKB_KEY_XF86Yellow, 0x1008FFA5, "Yellow button               ") \
   OP(XKB_KEY_XF86Blue, 0x1008FFA6, "Blue button                 ") \
   OP(XKB_KEY_XF86Suspend, 0x1008FFA7, "Sleep to RAM                ") \
   OP(XKB_KEY_XF86Hibernate, 0x1008FFA8, "Sleep to disk               ") \
   OP(XKB_KEY_XF86TouchpadToggle, 0x1008FFA9, "Toggle between touchpad/trackstick ") \
   OP(XKB_KEY_XF86TouchpadOn, 0x1008FFB0, "The touchpad got switched on ") \
   OP(XKB_KEY_XF86TouchpadOff, 0x1008FFB1, "The touchpad got switched off ") \
   OP(XKB_KEY_XF86AudioMicMute, 0x1008FFB2, "Mute the Mic from the system ") \
   OP(XKB_KEY_XF86Keyboard, 0x1008FFB3, "User defined keyboard related action ") \
   OP(XKB_KEY_XF86WWAN, 0x1008FFB4, "Toggle WWAN (LTE, UMTS, etc.) radio ") \
   OP(XKB_KEY_XF86RFKill, 0x1008FFB5, "Toggle radios on/off ") \
   OP(XKB_KEY_XF86AudioPreset, 0x1008FFB6, "Select equalizer preset, e.g. theatre-mode ") \
   OP(XKB_KEY_XF86Switch_VT_1, 0x1008FE01, "") \
   OP(XKB_KEY_XF86Switch_VT_2, 0x1008FE02, "") \
   OP(XKB_KEY_XF86Switch_VT_3, 0x1008FE03, "") \
   OP(XKB_KEY_XF86Switch_VT_4, 0x1008FE04, "") \
   OP(XKB_KEY_XF86Switch_VT_5, 0x1008FE05, "") \
   OP(XKB_KEY_XF86Switch_VT_6, 0x1008FE06, "") \
   OP(XKB_KEY_XF86Switch_VT_7, 0x1008FE07, "") \
   OP(XKB_KEY_XF86Switch_VT_8, 0x1008FE08, "") \
   OP(XKB_KEY_XF86Switch_VT_9, 0x1008FE09, "") \
   OP(XKB_KEY_XF86Switch_VT_10, 0x1008FE0A, "") \
   OP(XKB_KEY_XF86Switch_VT_11, 0x1008FE0B, "") \
   OP(XKB_KEY_XF86Switch_VT_12, 0x1008FE0C, "") \
   OP(XKB_KEY_XF86Ungrab, 0x1008FE20, "force ungrab               ") \
   OP(XKB_KEY_XF86ClearGrab, 0x1008FE21, "kill application with grab ") \
   OP(XKB_KEY_XF86Next_VMode, 0x1008FE22, "next video mode available  ") \
   OP(XKB_KEY_XF86Prev_VMode, 0x1008FE23, "prev. video mode available ") \
   OP(XKB_KEY_XF86LogWindowTree, 0x1008FE24, "print window tree to log   ") \
   OP(XKB_KEY_XF86LogGrabInfo, 0x1008FE25, "print all active grabs to log ") \
   OP(XKB_KEY_SunFA_Grave, 0x1005FF00, "") \
   OP(XKB_KEY_SunFA_Circum, 0x1005FF01, "") \
   OP(XKB_KEY_SunFA_Tilde, 0x1005FF02, "") \
   OP(XKB_KEY_SunFA_Acute, 0x1005FF03, "") \
   OP(XKB_KEY_SunFA_Diaeresis, 0x1005FF04, "") \
   OP(XKB_KEY_SunFA_Cedilla, 0x1005FF05, "") \
   OP(XKB_KEY_SunF36, 0x1005FF10, "Labeled F11 ") \
   OP(XKB_KEY_SunF37, 0x1005FF11, "Labeled F12 ") \
   OP(XKB_KEY_SunSys_Req, 0x1005FF60, "") \
   OP(XKB_KEY_SunPrint_Screen, 0x0000FF61, "Same as XK_Print ") \
   OP(XKB_KEY_SunCompose, 0x0000FF20, "Same as XK_Multi_key ") \
   OP(XKB_KEY_SunAltGraph, 0x0000FF7E, "Same as XK_Mode_switch ") \
   OP(XKB_KEY_SunPageUp, 0x0000FF55, "Same as XK_Prior ") \
   OP(XKB_KEY_SunPageDown, 0x0000FF56, "Same as XK_Next ") \
   OP(XKB_KEY_SunUndo, 0x0000FF65, "Same as XK_Undo ") \
   OP(XKB_KEY_SunAgain, 0x0000FF66, "Same as XK_Redo ") \
   OP(XKB_KEY_SunFind, 0x0000FF68, "Same as XK_Find ") \
   OP(XKB_KEY_SunStop, 0x0000FF69, "Same as XK_Cancel ") \
   OP(XKB_KEY_SunProps, 0x1005FF70, "") \
   OP(XKB_KEY_SunFront, 0x1005FF71, "") \
   OP(XKB_KEY_SunCopy, 0x1005FF72, "") \
   OP(XKB_KEY_SunOpen, 0x1005FF73, "") \
   OP(XKB_KEY_SunPaste, 0x1005FF74, "") \
   OP(XKB_KEY_SunCut, 0x1005FF75, "") \
   OP(XKB_KEY_SunPowerSwitch, 0x1005FF76, "") \
   OP(XKB_KEY_SunAudioLowerVolume, 0x1005FF77, "") \
   OP(XKB_KEY_SunAudioMute, 0x1005FF78, "") \
   OP(XKB_KEY_SunAudioRaiseVolume, 0x1005FF79, "") \
   OP(XKB_KEY_SunVideoDegauss, 0x1005FF7A, "") \
   OP(XKB_KEY_SunVideoLowerBrightness, 0x1005FF7B, "") \
   OP(XKB_KEY_SunVideoRaiseBrightness, 0x1005FF7C, "") \
   OP(XKB_KEY_SunPowerSwitchShift, 0x1005FF7D, "") \
   OP(XKB_KEY_Dring_accent, 0x1000FEB0, "") \
   OP(XKB_KEY_Dcircumflex_accent, 0x1000FE5E, "") \
   OP(XKB_KEY_Dcedilla_accent, 0x1000FE2C, "") \
   OP(XKB_KEY_Dacute_accent, 0x1000FE27, "") \
   OP(XKB_KEY_Dgrave_accent, 0x1000FE60, "") \
   OP(XKB_KEY_Dtilde, 0x1000FE7E, "") \
   OP(XKB_KEY_Ddiaeresis, 0x1000FE22, "") \
   OP(XKB_KEY_DRemove, 0x1000FF00, "Remove ") \
   OP(XKB_KEY_hpClearLine, 0x1000FF6F, "") \
   OP(XKB_KEY_hpInsertLine, 0x1000FF70, "") \
   OP(XKB_KEY_hpDeleteLine, 0x1000FF71, "") \
   OP(XKB_KEY_hpInsertChar, 0x1000FF72, "") \
   OP(XKB_KEY_hpDeleteChar, 0x1000FF73, "") \
   OP(XKB_KEY_hpBackTab, 0x1000FF74, "") \
   OP(XKB_KEY_hpKP_BackTab, 0x1000FF75, "") \
   OP(XKB_KEY_hpModelock1, 0x1000FF48, "") \
   OP(XKB_KEY_hpModelock2, 0x1000FF49, "") \
   OP(XKB_KEY_hpReset, 0x1000FF6C, "") \
   OP(XKB_KEY_hpSystem, 0x1000FF6D, "") \
   OP(XKB_KEY_hpUser, 0x1000FF6E, "") \
   OP(XKB_KEY_hpmute_acute, 0x100000A8, "") \
   OP(XKB_KEY_hpmute_grave, 0x100000A9, "") \
   OP(XKB_KEY_hpmute_asciicircum, 0x100000AA, "") \
   OP(XKB_KEY_hpmute_diaeresis, 0x100000AB, "") \
   OP(XKB_KEY_hpmute_asciitilde, 0x100000AC, "") \
   OP(XKB_KEY_hplira, 0x100000AF, "") \
   OP(XKB_KEY_hpguilder, 0x100000BE, "") \
   OP(XKB_KEY_hpYdiaeresis, 0x100000EE, "") \
   OP(XKB_KEY_hpIO, 0x100000EE, "") \
   OP(XKB_KEY_hplongminus, 0x100000F6, "") \
   OP(XKB_KEY_hpblock, 0x100000FC, "") \
   OP(XKB_KEY_osfCopy, 0x1004FF02, "") \
   OP(XKB_KEY_osfCut, 0x1004FF03, "") \
   OP(XKB_KEY_osfPaste, 0x1004FF04, "") \
   OP(XKB_KEY_osfBackTab, 0x1004FF07, "") \
   OP(XKB_KEY_osfBackSpace, 0x1004FF08, "") \
   OP(XKB_KEY_osfClear, 0x1004FF0B, "") \
   OP(XKB_KEY_osfEscape, 0x1004FF1B, "") \
   OP(XKB_KEY_osfAddMode, 0x1004FF31, "") \
   OP(XKB_KEY_osfPrimaryPaste, 0x1004FF32, "") \
   OP(XKB_KEY_osfQuickPaste, 0x1004FF33, "") \
   OP(XKB_KEY_osfPageLeft, 0x1004FF40, "") \
   OP(XKB_KEY_osfPageUp, 0x1004FF41, "") \
   OP(XKB_KEY_osfPageDown, 0x1004FF42, "") \
   OP(XKB_KEY_osfPageRight, 0x1004FF43, "") \
   OP(XKB_KEY_osfActivate, 0x1004FF44, "") \
   OP(XKB_KEY_osfMenuBar, 0x1004FF45, "") \
   OP(XKB_KEY_osfLeft, 0x1004FF51, "") \
   OP(XKB_KEY_osfUp, 0x1004FF52, "") \
   OP(XKB_KEY_osfRight, 0x1004FF53, "") \
   OP(XKB_KEY_osfDown, 0x1004FF54, "") \
   OP(XKB_KEY_osfEndLine, 0x1004FF57, "") \
   OP(XKB_KEY_osfBeginLine, 0x1004FF58, "") \
   OP(XKB_KEY_osfEndData, 0x1004FF59, "") \
   OP(XKB_KEY_osfBeginData, 0x1004FF5A, "") \
   OP(XKB_KEY_osfPrevMenu, 0x1004FF5B, "") \
   OP(XKB_KEY_osfNextMenu, 0x1004FF5C, "") \
   OP(XKB_KEY_osfPrevField, 0x1004FF5D, "") \
   OP(XKB_KEY_osfNextField, 0x1004FF5E, "") \
   OP(XKB_KEY_osfSelect, 0x1004FF60, "") \
   OP(XKB_KEY_osfInsert, 0x1004FF63, "") \
   OP(XKB_KEY_osfUndo, 0x1004FF65, "") \
   OP(XKB_KEY_osfMenu, 0x1004FF67, "") \
   OP(XKB_KEY_osfCancel, 0x1004FF69, "") \
   OP(XKB_KEY_osfHelp, 0x1004FF6A, "") \
   OP(XKB_KEY_osfSelectAll, 0x1004FF71, "") \
   OP(XKB_KEY_osfDeselectAll, 0x1004FF72, "") \
   OP(XKB_KEY_osfReselect, 0x1004FF73, "") \
   OP(XKB_KEY_osfExtend, 0x1004FF74, "") \
   OP(XKB_KEY_osfRestore, 0x1004FF78, "") \
   OP(XKB_KEY_osfDelete, 0x1004FFFF, "") \
   OP(XKB_KEY_Reset, 0x1000FF6C, "") \
   OP(XKB_KEY_System, 0x1000FF6D, "") \
   OP(XKB_KEY_User, 0x1000FF6E, "") \
   OP(XKB_KEY_ClearLine, 0x1000FF6F, "") \
   OP(XKB_KEY_InsertLine, 0x1000FF70, "") \
   OP(XKB_KEY_DeleteLine, 0x1000FF71, "") \
   OP(XKB_KEY_InsertChar, 0x1000FF72, "") \
   OP(XKB_KEY_DeleteChar, 0x1000FF73, "") \
   OP(XKB_KEY_BackTab, 0x1000FF74, "") \
   OP(XKB_KEY_KP_BackTab, 0x1000FF75, "") \
   OP(XKB_KEY_Ext16bit_L, 0x1000FF76, "") \
   OP(XKB_KEY_Ext16bit_R, 0x1000FF77, "") \
   OP(XKB_KEY_mute_acute, 0x100000a8, "") \
   OP(XKB_KEY_mute_grave, 0x100000a9, "") \
   OP(XKB_KEY_mute_asciicircum, 0x100000aa, "") \
   OP(XKB_KEY_mute_diaeresis, 0x100000ab, "") \
   OP(XKB_KEY_mute_asciitilde, 0x100000ac, "") \
   OP(XKB_KEY_lira, 0x100000af, "") \
   OP(XKB_KEY_guilder, 0x100000be, "") \
   OP(XKB_KEY_IO, 0x100000ee, "") \
   OP(XKB_KEY_longminus, 0x100000f6, "") \
   OP(XKB_KEY_block, 0x100000fc, "")
