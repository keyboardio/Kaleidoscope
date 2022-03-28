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

#define System_PowerDown                        SYSTEM_KEY(HID_SYSTEM_POWER_DOWN, HID_TYPE_OSC)
#define System_Sleep                            SYSTEM_KEY(HID_SYSTEM_SLEEP, HID_TYPE_OSC)
#define System_WakeUp                           SYSTEM_KEY(HID_SYSTEM_WAKE_UP, HID_TYPE_OSC)
#define System_ContextMenu                      SYSTEM_KEY(HID_SYSTEM_CONTEXT_MENU, HID_TYPE_OSC)
#define System_MainMenu                         SYSTEM_KEY(HID_SYSTEM_MAIN_MENU, HID_TYPE_OSC)
#define System_AppMenu                          SYSTEM_KEY(HID_SYSTEM_APP_MENU, HID_TYPE_OSC)
#define System_MenuHelp                         SYSTEM_KEY(HID_SYSTEM_MENU_HELP, HID_TYPE_OSC)
#define System_MenuExit                         SYSTEM_KEY(HID_SYSTEM_MENU_EXIT, HID_TYPE_OSC)
#define System_MenuSelect                       SYSTEM_KEY(HID_SYSTEM_MENU_SELECT, HID_TYPE_OSC)
#define System_MenuRight                        SYSTEM_KEY(HID_SYSTEM_MENU_RIGHT, HID_TYPE_RTC)
#define System_MenuLeft                         SYSTEM_KEY(HID_SYSTEM_MENU_LEFT, HID_TYPE_RTC)
#define System_MenuUp                           SYSTEM_KEY(HID_SYSTEM_MENU_UP, HID_TYPE_RTC)
#define System_MenuDown                         SYSTEM_KEY(HID_SYSTEM_MENU_DOWN, HID_TYPE_RTC)
#define System_ColdRestart                      SYSTEM_KEY(HID_SYSTEM_COLD_RESTART, HID_TYPE_OSC)
#define System_WarmRestart                      SYSTEM_KEY(HID_SYSTEM_WARM_RESTART, HID_TYPE_OSC)
#define System_DPadUp                           SYSTEM_KEY(HID_D_PAD_UP, HID_TYPE_OOC)
#define System_DPadDown                         SYSTEM_KEY(HID_D_PAD_DOWN, HID_TYPE_OOC)
#define System_DPadRight                        SYSTEM_KEY(HID_D_PAD_RIGHT, HID_TYPE_OOC)
#define System_DPadLeft                         SYSTEM_KEY(HID_D_PAD_LEFT, HID_TYPE_OOC)

#define System_IndexTrigger                     SYSTEM_KEY(HID_INDEX_TRIGGER, HID_TYPE_MC)
#define System_PalmTrigger                      SYSTEM_KEY(HID_PALM_TRIGGER, HID_TYPE_MC)
#define System_Thumbstick                       SYSTEM_KEY(HID_THUMBSTICK, HID_TYPE_CP)
#define System_SystemFunctionShift              SYSTEM_KEY(HID_SYSTEM_FUNCTION_SHIFT, HID_TYPE_MC)
#define System_SystemFunctionShiftLock          SYSTEM_KEY(HID_SYSTEM_FUNCTION_SHIFT_LOCK, HID_TYPE_OOC)
#define System_SystemFunctionShiftLockIndicator SYSTEM_KEY(HID_SYSTEM_FUNCTION_SHIFT_LOCK_INDICATOR, HID_TYPE_DV)
#define System_DismissNotification              SYSTEM_KEY(HID_DISMISS_NOTIFICATION, HID_TYPE_OSC)
#define System_DoNotDisturb                     SYSTEM_KEY(HID_DO_NOT_DISTURB, HID_TYPE_OOC)

#define System_Dock                             SYSTEM_KEY(HID_SYSTEM_DOCK, HID_TYPE_OSC)
#define System_Undock                           SYSTEM_KEY(HID_SYSTEM_UNDOCK, HID_TYPE_OSC)
#define System_Setup                            SYSTEM_KEY(HID_SYSTEM_SETUP, HID_TYPE_OSC)
#define System_Break                            SYSTEM_KEY(HID_SYSTEM_BREAK, HID_TYPE_OSC)
#define System_DebuggerBreak                    SYSTEM_KEY(HID_SYSTEM_DEBUGGER_BREAK, HID_TYPE_OSC)
#define System_ApplicationBreak                 SYSTEM_KEY(HID_APPLICATION_BREAK, HID_TYPE_OSC)
#define System_ApplicationDebuggerBreak         SYSTEM_KEY(HID_APPLICATION_DEBUGGER_BREAK, HID_TYPE_OSC)
#define System_SpeakerMute                      SYSTEM_KEY(HID_SYSTEM_SPEAKER_MUTE, HID_TYPE_OSC)
#define System_Hibernate                        SYSTEM_KEY(HID_SYSTEM_HIBERNATE, HID_TYPE_OSC)

#define System_DisplayInvert                    SYSTEM_KEY(HID_SYSTEM_DISPLAY_INVERT, HID_TYPE_OSC)
#define System_DisplayInternal                  SYSTEM_KEY(HID_SYSTEM_DISPLAY_INTERNAL, HID_TYPE_OSC)
#define System_DisplayExternal                  SYSTEM_KEY(HID_SYSTEM_DISPLAY_EXTERNAL, HID_TYPE_OSC)
#define System_DisplayBoth                      SYSTEM_KEY(HID_SYSTEM_DISPLAY_BOTH, HID_TYPE_OSC)
#define System_DisplayDual                      SYSTEM_KEY(HID_SYSTEM_DISPLAY_DUAL, HID_TYPE_OSC)
#define System_DisplayToggleIntSlashExt         SYSTEM_KEY(HID_SYSTEM_DISPLAY_TOGGLE_INT_SLASH_EXT, HID_TYPE_OSC)
#define System_DisplaySwapPrimarySlashSecondary SYSTEM_KEY(HID_SYSTEM_DISPLAY_SWAP_PRIMARY_SLASH_SECONDARY, HID_TYPE_OSC)
#define System_DisplayLCDAutoscale              SYSTEM_KEY(HID_SYSTEM_DISPLAY_LCD_AUTOSCALE, HID_TYPE_OSC)
