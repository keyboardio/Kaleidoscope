We're in the midst of refactoring our really bad prototype firmware into a more reasonable production firmware.

Right now, the code is really ugly. It'll be nicer soon. Please don't judge us too harshly.


# Getting Started

Once you have a git checkout, you'll need to pull in the libraries we 
depend on. To do that:

```
$ cd KeyboardioFirmware
$ git submodule sync --recursive
$ git submodule update --init --recursive
$ cd source
$ make
```

<3 jesse

[![Build
Status](https://travis-ci.org/keyboardio/KeyboardioFirmware.svg?branch=master)](https://travis-ci.org/keyboardio/KeyboardioFirmware)