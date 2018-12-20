# Kaleidoscope

Flexible firmware for Arduino-powered keyboards.

This package contains the "core" of Kaleidoscope and an [example firmware "Sketch."](https://github.com/keyboardio/Kaleidoscope/blob/master/examples/Kaleidoscope/Kaleidoscope.ino)

If you're just getting started with the Keyboardio Model 01, the [introductory docs are here](https://github.com/keyboardio/Kaleidoscope/wiki/Keyboardio-Model-01-Introduction) and the source for the basic firmware package is here: https://github.com/keyboardio/Model01-Firmware. It's probably a good idea to start there, learn how to modify your keymap and maybe turn some modules on or off, and then come back to the full repository when you have more complex changes in mind.

# Getting Started

Pick a directory to work in, you'll need to clone multiple repositories. We'll assume you picked `$HOME/kaleidoscope`, if you chose another adapt the commands below accordingly.

## Setup the Arduino IDE

Setup the Arduino IDE on your system. Make sure you install at least version 1.6, since older version may not support all required features.

* On Linux, follow the instructions [on the wiki](https://github.com/keyboardio/Kaleidoscope/wiki/Install-Arduino-support-on-Linux).
* On macOS, install using [homebrew](http://brew.sh/) [cask](https://caskroom.github.io/) with `brew cask install arduino` or download the application from [the official website](https://www.arduino.cc/en/Main/Software) and move it to your `/Applications` folder.

## Install the Kaleidoscope Hardware Definitions

```sh
# you'll need to know your arduino sketchbook directory

# on macOS the default is
SKETCHBOOK_DIR=$HOME/Documents/Arduino
# on Linux the default is
SKETCHBOOK_DIR=$HOME/Arduino

# go to your kaleidoscope directory
cd $HOME/kaleidoscope
# then clone the hardware definitions
git clone --recursive https://github.com/keyboardio/Kaleidoscope-Bundle-Keyboardio.git
# and make them available to the arduino environment
mkdir -p $SKETCHBOOK_DIR/hardware
ln -s $HOME/kaleidoscope/Kaleidoscope-Bundle-Keyboardio $SKETCHBOOK_DIR/hardware/keyboardio
```

## Build the Kaleidoscope Firmware for your keyboard

```sh
# go to your keyboardio directory
cd $HOME/kaleidoscope/Kaleidoscope-Bundle-Keyboardio/avr/libraries/Model01-Firmware
# and build your firmware!
make
```

<3 jesse

[![Build
Status](https://travis-ci.org/keyboardio/Kaleidoscope.svg?branch=master)](https://travis-ci.org/keyboardio/Kaleidoscope)
