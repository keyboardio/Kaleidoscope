# ATMegaKeyboard

A lot of custom keyboards are built upon the ATMega MCU (most often an
`atmega32u4`), and the vast majority of them follow a similar architecture, so
much so that the vast majority of code can be lifted out into a base class,
making porting to these keyboards trivial. All we have to do is tell it our
pinout, and we're all done.

## Porting hardware using the ATMegaKeyboard class

The plugin assumes a simple matrix layout, which we can tell it by using the
`ATMEGA_KEYBOARD_CONFIG` macro, which takes two arguments: a list of row and
column pins. In the `.cpp`, simply use `ATMEGA_KEYBOARD_DATA`, with the hardware
object as its argument:

```c++
// Kaleidoscope-Vendor-ExampleHardware.h
#define KALEIDOSCOPE_WITH_ATMEGA_KEYBOARD 1
#include "kaleidoscope/hardware/vendor/ExampleHardware.h"

// kaleidoscope/hardware/vendor/ExampleHardware.h
namespace kaleidoscope {
namespace hardware {
namespace vendor {
class ExampleKeyboard: public ATMegaKeyboard {
 public:
  ExampleKeyboard() {}

  ATMEGA_KEYBOARD_CONFIG(
    ROW_PIN_LIST({PIN_D1, PIN_D2}),
    COL_PIN_LIST({PIN_F6, PIN_F7})
  );

  static constexpr int8_t led_count = 0;
};

#define KEYMAP(  \
   r0c0 ,r0c1    \
  ,r1c0 ,r1c1    \
)                \
 { r0c0 ,r0c1 }, \
 { r1c0 ,r1c1 }

}
}
}

// kaleidoscope/hardware/vendor/ExampleHardware.cpp
namespace kaleidoscope {
namespace hardware {
namespace vendor {

ATMEGA_KEYBOARD_DATA(ExampleHardware);
constexpr int8_t ExampleHardware::led_count;

}
}
}

HARDWARE_IMPLEMENTATION KeyboardHardware;
kaleidoscope::hardware::vendor::ExampleHardware &ExampleHardware = KeyboardHardware;
```

## Overriding methods

For performance and space-saving reasons, the base class does not use virtual
methods. Instead, whenever it calls a method of its own, it will call it through
the `KeyboardHardware` singleton object, which is always an instance of the
exact hardware plugin. Thus, implementing a function in the subclass will shadow
the one in the base class.

This can be used to implement more efficient methods, would it be needed. The
[Atreus][hardware:atreus] port does this, for example, by overriding the generic
`readCols` with a more specialised, faster implementation.

## Further reading

See the [Planck][hardware:planck] and [Atreus][hardware:atreus] ports for an
example of how this class can be used in practice.

 [hardware:planck]: ../../src/kaleidoscope/hardware/olkb/Planck.h
 [hardware:atreus]: ../../src/kaleidoscope/hardware/technomancy/Atreus.h
