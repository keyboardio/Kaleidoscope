
static const Key keymaps[LAYERS][ROWS][COLS] = {
        {
                { NoKey,     Key_1,  Key_2, Key_3, Key_4, Key_5,  NoKey, NoKey,  Key_6,  Key_7, Key_8, Key_9,  Key_0, NoKey  },
                { Key_Backtick, Key_Q,  Key_W, Key_E, Key_R, Key_T,  NoKey, NoKey,  Key_Y,  Key_U, Key_I, Key_O, Key_P, Key_Equals},
                { Key_PageUp,   Key_A,  Key_S, Key_D, Key_F, Key_G,  Key_Tab, Key_Return, Key_H,  Key_J, Key_K, Key_L, Key_Semicolon, Key_Quote },
                { Key_PageDn,   Key_Z,  Key_X, Key_C, Key_V, Key_B,  Key_Esc, Key_Return, Key_N,  Key_M, Key_Comma, Key_Period, Key_Slash,  Key_Minus},
                { Key_LGUI,     Key_Backspace, Key_LShift, Key_LCtrl, NextKeymap, NoKey,  NoKey, NoKey, NoKey,  NextKeymap, Key_RCtrl, Key_RShift, Key_Space,  Key_RAlt }
        },
        {
                { NoKey,     Key_1,  Key_2, Key_3, Key_4, Key_5,  NoKey,      NoKey,  Key_6,  Key_7, Key_8, Key_9,  Key_0, NoKey  },
                { Key_Backtick, Key_Q,  mouseBtnL, mouseBtnM, mouseBtnR, Key_T,  NoKey,      NoKey,  Key_LCurlyBracket,  Key_RCurlyBracket, Key_LSquareBracket, Key_RSquareBracket, Key_P, Key_Equals},
                { Key_PageUp,   mouseL, mouseUp, mouseDn, mouseR, Key_G,  Key_Tab,       Key_Return, Key_LArrow, Key_DnArrow,  Key_UpArrow, Key_RArrow, Key_Semicolon, Key_Quote},
                { Key_PageDn,   Key_Z,  mouseBtnL, mouseBtnM, mouseBtnR, Key_B,  Key_Esc,       Key_Return, Key_Pipe,  Key_M, Key_Comma, Key_Period, Key_Backslash,  Key_Minus},
                { Key_LGUI,     Key_Backspace, Key_LShift, Key_LCtrl, NextKeymap, NoKey,    NoKey, NoKey, NoKey,  NextKeymap, Key_RCtrl, Key_RShift, Key_Space,  Key_RAlt }
        },

        {
                { NoKey,     Key_1,  Key_2, Key_3, Key_4, Key_5,  NoKey, NoKey,  Key_6,  Key_7, Key_8, Key_9,  Key_0, NoKey  },
                { Key_Backtick, Key_Q,  Key_W, Key_E, Key_R, Key_T,  NoKey, NoKey,  Key_Y,  Key_U, Key_I, Key_O, Key_P, Key_Equals},
                { Key_PageUp,   Key_A,  Key_S, Key_D, Key_F, Key_G,  Key_Tab, Key_Return, Key_H,  Key_J, Key_K, Key_L, Key_Semicolon, Key_Quote },
                { Key_PageDn,   Key_Z,  Key_X, Key_C, Key_V, Key_B,  Key_Esc, Key_Return, Key_N,  Key_M, Key_Comma, Key_Period, Key_Slash,  Key_Minus},
                { Key_LGUI,     Key_Backspace, Key_LShift, Key_LCtrl, NextKeymap, NoKey,  NoKey, NoKey, NoKey,  NextKeymap, Key_RCtrl, Key_RShift, Key_Space,  Key_RAlt }
        },
        {
                { NoKey,     Key_1,  Key_2, Key_3, Key_4, Key_5,  NoKey,      NoKey,  Key_6,  Key_7, Key_8, Key_9,  Key_0, NoKey  },
                { Key_Backtick, Key_Q,  mouseBtnL, mouseBtnM, mouseBtnR, Key_T,  NoKey,      NoKey,  Key_LCurlyBracket,  Key_RCurlyBracket, Key_LSquareBracket, Key_RSquareBracket, Key_P, Key_Equals},
                { Key_PageUp,   mouseL, mouseUp, mouseDn, mouseR, Key_G,  Key_Tab,       Key_Return, Key_LArrow, Key_DnArrow,  Key_UpArrow, Key_RArrow, Key_Semicolon, Key_Quote},
                { Key_PageDn,   Key_Z,  mouseBtnL, mouseBtnM, mouseBtnR, Key_B,  Key_Esc,       Key_Return, Key_Pipe,  Key_M, Key_Comma, Key_Period, Key_Backslash,  Key_Minus},
                { Key_LGUI,     Key_Backspace, Key_LShift, Key_LCtrl, NextKeymap, NoKey,    NoKey, NoKey, NoKey,  NextKeymap, Key_RCtrl, Key_RShift, Key_Space,  Key_RAlt }
        },


        {
                { NoKey,     Key_1,  Key_2, Key_3, Key_4, Key_5,  NoKey, NoKey,  Key_6,  Key_7, Key_8, Key_9,  Key_0, NoKey  },
                { Key_Backtick, Key_Q,  Key_W, Key_E, Key_R, Key_T,  NoKey, NoKey,  Key_Y,  Key_U, Key_I, Key_O, Key_P, Key_Equals},
                { Key_PageUp,   Key_A,  Key_S, Key_D, Key_F, Key_G,  Key_Tab, Key_Return, Key_H,  Key_J, Key_K, Key_L, Key_Semicolon, Key_Quote },
                { Key_PageDn,   Key_Z,  Key_X, Key_C, Key_V, Key_B,  Key_Esc, Key_Return, Key_N,  Key_M, Key_Comma, Key_Period, Key_Slash,  Key_Minus},
                { Key_LGUI,     Key_Backspace, Key_LShift, Key_LCtrl, NextKeymap, NoKey,  NoKey, NoKey, NoKey,  NextKeymap, Key_RCtrl, Key_RShift, Key_Space,  Key_RAlt }
        },
        {
                { NoKey,     Key_1,  Key_2, Key_3, Key_4, Key_5,  NoKey,      NoKey,  Key_6,  Key_7, Key_8, Key_9,  Key_0, NoKey  },
                { Key_Backtick, Key_Q,  mouseBtnL, mouseBtnM, mouseBtnR, Key_T,  NoKey,      NoKey,  Key_LCurlyBracket,  Key_RCurlyBracket, Key_LSquareBracket, Key_RSquareBracket, Key_P, Key_Equals},
                { Key_PageUp,   mouseL, mouseUp, mouseDn, mouseR, Key_G,  Key_Tab,       Key_Return, Key_LArrow, Key_DnArrow,  Key_UpArrow, Key_RArrow, Key_Semicolon, Key_Quote},
                { Key_PageDn,   Key_Z,  mouseBtnL, mouseBtnM, mouseBtnR, Key_B,  Key_Esc,       Key_Return, Key_Pipe,  Key_M, Key_Comma, Key_Period, Key_Backslash,  Key_Minus},
                { Key_LGUI,     Key_Backspace, Key_LShift, Key_LCtrl, NextKeymap, NoKey,    NoKey, NoKey, NoKey,  NextKeymap, Key_RCtrl, Key_RShift, Key_Space,  Key_RAlt }
        },




};

