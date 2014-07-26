#define KEYMAP_QWERTY_PIERS { /* Generated keymap for QWERTY_PIERS */ 	{Key_skip, Key_1, Key_2, Key_3, Key_4, Key_5, Key_skip, Key_LCtrl, Key_RCtrl, Key_skip, Key_6, Key_7, Key_8, Key_9, Key_0, Key_skip},\
	{Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab, Key_Return, Key_Backspace, Key_Space, Key_Y, Key_U, Key_I, Key_O, Key_P, Key_Equals},\
	{Key_PageUp, Key_A, Key_S, Key_D, Key_F, Key_G, Key_Esc, Key_LShift, Key_RShift, Key_Keymap2_Momentary, Key_H, Key_J, Key_K, Key_L, Key_Semicolon, Key_Quote},\
	{Key_PageDn, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_KeymapNext_Momentary, Key_LGUI, Key_RAlt, Key_KeymapNext_Momentary, Key_N, Key_M, Key_Comma, Key_Period, Key_Slash, Key_Minus},\
},
#define KEYMAPS 2
// This is a total waste. we should only ever include any given keymap once
static const Key keymaps[KEYMAPS][ROWS][COLS] = {
	KEYMAP_QWERTY_PIERS
	KEYMAP_QWERTY_PIERS




};

