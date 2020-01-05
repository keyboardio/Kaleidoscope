# Ranges

## kaleidoscope::ranges enum

This plugin defines the `ranges` enum that many plugins use.

To explain its purpose, first a brief digression to explain how Kaleidoscope implements keys.

Keys in Kaleidoscope are defined as follows:

```c++
// in Kaleidoscope/src/key_defs.h
typedef union Key_ {

  struct {
    uint8_t keyCode;
    uint8_t flags;
  };
  uint16_t raw;
// bunch of operator overloads elided...
} Key;
```
That is, a key is a 16-bit integer that, by default, has 8 bits for the keyCode and 8 bits for flags.
For normal keypresses, this is straightforward: the 8-bit keyCode is the normal HID code corresponding to a normal keyboard key and the flags are used to indicate the modifiers being held.

However, many plugins want to be able to make key presses do something special and need some way of indicating that a key is not just an ordinary key.
To do this, we take advantage of the fact that our flags field is 8 bits, but there are only five normal modifiers (control, shift, GUI, right alt, and left alt).
<!-- question: does bit six have any signicance? it seems to be unused
--> Therefore, we can use a bit to indicate that something special is happening with a given key:
In this case, by setting the high bit of the flags field, we indicate that this is a reserved key and isn't going to be interpreted normally.

This way, a plugin can make a key be sent with the high bit of the flags field set, then it is free to use the rest of the 16 bits as it sees fit and be assured that it won't conflict with the built-in keys.

However, there is a problem with this:
Many plugins will want to do this, so how can they be sure that the key codes they make won't be interpreted by another plugin?
Thus, we come to the purpose of this enum: 
The `range` enum gives the ranges of possible raw key values that each plugin will use; by referring to it, plugins can be sure none of them will step on each others' toes.

**In summary, the values in the enum here gives the possible raw keycode values that the various plugins will inject; if you're trying to make a plugin that will be injecting special key events, you should probably add yourself to the enum here.**
