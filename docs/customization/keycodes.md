# What can go on your keymap

Eventually there should be a helpful table here with good definitions for the common codes. In the meantime, you can check these files for all the codes the Keyboardio supports:

 - Most of the common keyboard key codes are here: 
   
   [key_defs_keyboard.h](https://github.com/keyboardio/Kaleidoscope/blob/master/src/kaleidoscope/key_defs_keyboard.h)

 - Key codes for system tasks like shutting down, switching windows, and moving through menus are here: 
    
   [key_defs_sysctl.h](https://github.com/keyboardio/Kaleidoscope/blob/master/src/kaleidoscope/key_defs_sysctl.h)

 - A wide range of key codes for controlling consumer electronics, most of which are probably not relevant, are in this file:  

    [key_defs_consumerctl.h](https://github.com/keyboardio/Kaleidoscope/blob/master/src/kaleidoscope/key_defs_consumerctl.h)

## In-keymap chorded keys

In addition, the keys in `key_defs_keyboard.h` can be augmented with modifier macros: `LCTRL()`, `LSHIFT()`, `LALT()`, `LGUI()` and `RALT()` to add chorded keys to your keymap. For example `LCTRL(LALT(Key_Delete))` can be used to add control-alt-delete as a single key to your keymap, should you wish. The innermost bracket must be of the standard format as taken from the above key definitions, and all other modifiers must be from the aforementioned list, and in that format. This does allow you to create single keys for multiple modifiers, e.g. `LCTRL(LALT(LSHIFT(Key_LeftGui)))`, when held, would have the effect of all left modifiers at once. These modifier macros only work for standard keys! When applied to any key provided by a plugin, they will have no effect.
