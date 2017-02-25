# Kaleidoscope-EEPROM-Keymap

![status][st:experimental] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-EEPROM-Keymap.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-EEPROM-Keymap

 [st:stable]: https://img.shields.io/badge/stable-✔-black.png?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.png?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.png?style=flat&colorA=dfb317&colorB=494e52

TODO

## Using the plugin

TODO

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-EEPROM-Keymap.h>

void setup () {
  Kaleidoscope.setup ();
  
  USE_PLUGINS (&EEPROMKeymap);

  // TODO
}
```

## Plugin methods

The plugin provides the `EEPROMKeymap` object, which has the following methods:

**TODO**

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

  [plugin:example]: https://github.com/keyboardio/Kaleidoscope-EEPROM-Keymap/blob/master/examples/EEPROM-Keymap/EEPROM-Keymap.ino
