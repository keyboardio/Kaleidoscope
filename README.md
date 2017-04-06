# Kaleidoscope-EEPROM-Settings

![status][st:experimental] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-EEPROM-Settings.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-EEPROM-Settings

 [st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

TODO

## Using the plugin

TODO

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-EEPROM-Settings.h>

void setup () {
  Kaleidoscope.setup ();
  
  USE_PLUGINS (&EEPROMSettings);

  // TODO
}
```

## Plugin methods

The plugin provides the `EEPROMSettings` object, which has the following methods:

**TODO**

## Dependencies

* [Kaleidoscope-Focus](https://github.com/keyboardio/Kaleidoscope-Focus)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

  [plugin:example]: https://github.com/keyboardio/Kaleidoscope-EEPROM-Settings/blob/master/examples/EEPROM-Settings/EEPROM-Settings.ino
