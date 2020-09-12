/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2020  Keyboard.io, Inc.
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

// System control mappings

#define System_PowerDown Key(HID_SYSTEM_POWER_DOWN, SYNTHETIC | IS_SYSCTL)
#define System_Sleep Key(HID_SYSTEM_SLEEP, SYNTHETIC | IS_SYSCTL)
#define System_WakeUp Key(HID_SYSTEM_WAKE_UP, SYNTHETIC | IS_SYSCTL)
#define System_ContextMenu Key(HID_SYSTEM_CONTEXT_MENU, SYNTHETIC | IS_SYSCTL)
#define System_MainMenu Key(HID_SYSTEM_MAIN_MENU, SYNTHETIC | IS_SYSCTL)
#define System_AppMenu Key(HID_SYSTEM_APP_MENU, SYNTHETIC | IS_SYSCTL)
#define System_MenuHelp Key(HID_SYSTEM_MENU_HELP, SYNTHETIC | IS_SYSCTL)
#define System_MenuExit Key(HID_SYSTEM_MENU_EXIT, SYNTHETIC | IS_SYSCTL)
#define System_MenuSelect Key(HID_SYSTEM_MENU_SELECT, SYNTHETIC | IS_SYSCTL)
#define System_MenuRight Key(HID_SYSTEM_MENU_RIGHT, SYNTHETIC | IS_SYSCTL)
#define System_MenuLeft Key(HID_SYSTEM_MENU_LEFT, SYNTHETIC | IS_SYSCTL)
#define System_MenuUp Key(HID_SYSTEM_MENU_UP, SYNTHETIC | IS_SYSCTL)
#define System_MenuDown Key(HID_SYSTEM_MENU_DOWN, SYNTHETIC | IS_SYSCTL)
#define System_ColdRestart Key(HID_SYSTEM_COLD_RESTART, SYNTHETIC | IS_SYSCTL)
#define System_WarmRestart Key(HID_SYSTEM_WARM_RESTART, SYNTHETIC | IS_SYSCTL)
#define System_DPadUp Key(HID_D_PAD_UP, SYNTHETIC | IS_SYSCTL)
#define System_DPadDown Key(HID_D_PAD_DOWN, SYNTHETIC | IS_SYSCTL)
#define System_DPadRight Key(HID_D_PAD_RIGHT, SYNTHETIC | IS_SYSCTL)
#define System_DPadLeft Key(HID_D_PAD_LEFT, SYNTHETIC | IS_SYSCTL)

#define System_IndexTrigger Key(HID_INDEX_TRIGGER, SYNTHETIC | IS_SYSCTL)
#define System_PalmTrigger Key(HID_PALM_TRIGGER, SYNTHETIC | IS_SYSCTL)
#define System_Thumbstick Key(HID_THUMBSTICK, SYNTHETIC | IS_SYSCTL)
#define System_SystemFunctionShift Key(HID_SYSTEM_FUNCTION_SHIFT, SYNTHETIC | IS_SYSCTL)
#define System_SystemFunctionShiftLock Key(HID_SYSTEM_FUNCTION_SHIFT_LOCK, SYNTHETIC | IS_SYSCTL)
#define System_SystemFunctionShiftLockIndicator Key(HID_SYSTEM_FUNCTION_SHIFT_LOCK_INDICATOR, SYNTHETIC | IS_SYSCTL)
#define System_DismissNotification Key(HID_DISMISS_NOTIFICATION, SYNTHETIC | IS_SYSCTL)
#define System_DoNotDisturb Key(HID_DO_NOT_DISTURB, SYNTHETIC | IS_SYSCTL)

#define System_Dock Key(HID_SYSTEM_DOCK, SYNTHETIC | IS_SYSCTL)
#define System_Undock Key(HID_SYSTEM_UNDOCK, SYNTHETIC | IS_SYSCTL)
#define System_Setup Key(HID_SYSTEM_SETUP, SYNTHETIC | IS_SYSCTL)
#define System_Break Key(HID_SYSTEM_BREAK, SYNTHETIC | IS_SYSCTL)
#define System_DebuggerBreak Key(HID_SYSTEM_DEBUGGER_BREAK, SYNTHETIC | IS_SYSCTL)
#define System_ApplicationBreak Key(HID_APPLICATION_BREAK, SYNTHETIC | IS_SYSCTL)
#define System_ApplicationDebuggerBreak Key(HID_APPLICATION_DEBUGGER_BREAK, HID_TYPE_OSC)
#define System_SpeakerMute Key(HID_SYSTEM_SPEAKER_MUTE, SYNTHETIC | IS_SYSCTL)
#define System_Hibernate Key(HID_SYSTEM_HIBERNATE, SYNTHETIC | IS_SYSCTL)

#define System_DisplayInvert Key(HID_SYSTEM_DISPLAY_INVERT, SYNTHETIC | IS_SYSCTL)
#define System_DisplayInternal Key(HID_SYSTEM_DISPLAY_INTERNAL, SYNTHETIC | IS_SYSCTL)
#define System_DisplayExternal Key(HID_SYSTEM_DISPLAY_EXTERNAL, SYNTHETIC | IS_SYSCTL)
#define System_DisplayBoth Key(HID_SYSTEM_DISPLAY_BOTH, SYNTHETIC | IS_SYSCTL)
#define System_DisplayDual Key(HID_SYSTEM_DISPLAY_DUAL, SYNTHETIC | IS_SYSCTL)
#define System_DisplayToggleIntSlashExt Key(HID_SYSTEM_DISPLAY_TOGGLE_INT_SLASH_EXT, SYNTHETIC | IS_SYSCTL)
#define System_DisplaySwapPrimarySlashSecondary Key(HID_SYSTEM_DISPLAY_SWAP_PRIMARY_SLASH_SECONDARY, SYNTHETIC | IS_SYSCTL)
#define System_DisplayLCDAutoscale Key(HID_SYSTEM_DISPLAY_LCD_AUTOSCALE, SYNTHETIC | IS_SYSCTL)
