# Kaleidoscope-EEPROM-Keymap-Programmer

![status][st:experimental] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-EEPROM-Keymap-Programmer.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-EEPROM-Keymap-Programmer

 [st:stable]: https://img.shields.io/badge/stable-✔-black.png?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.png?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.png?style=flat&colorA=dfb317&colorB=494e52

TODO

## Using the plugin

TODO

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-EEPROM-Keymap.h>
#include <Kaleidoscope-EEPROM-Keymap-Programmer.h>

void setup () {
  Kaleidoscope.setup ();
  
  USE_PLUGINS (&EEPROMKeymapProgrammer, &EEPROMKeymap);

  // TODO
}
```

## Plugin methods

The plugin provides the `EEPROMKeymapProgrammer` object, which has the following methods:

**TODO**

## Dependencies

* [Kaleidoscope-EEPROM-Keymap](https://github.com/keyboardio/Kaleidoscope-EEPROM-Keymap)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

  [plugin:example]: https://github.com/keyboardio/Kaleidoscope-EEPROM-Keymap-Programmer/blob/master/examples/EEPROM-Keymap-Programmer/EEPROM-Keymap-Programmer.ino
