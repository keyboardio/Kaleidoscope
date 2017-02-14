# Kaleidoscope

Flexible firmware for Arduino-powered keyboards

# Getting Started

Pick a directory to work in, you'll need to clone multiple repositories. We'll assume you picked `$HOME/kaleidoscope`, if you chose another adapt the commands below accordingly.

## Setup the Arduino IDE

Setup the Arduino IDE on your system. Make sure you install at least version 1.6, since older version may not support all required features.

* On Linux, follow the instructions [on the wiki](https://github.com/keyboardio/Kaleidoscope/wiki/Arduino-Setup-Linux).
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
git clone --recursive https://github.com/keyboardio/Arduino-Boards.git
# and make them available to the arduino environment
mkdir -p $SKETCHBOOK_DIR/hardware/kaleidoscope
ln -s $HOME/kaleidoscope/Arduino-Boards $SKETCHBOOK_DIR/hardware/kaleidoscope/avr
```

## Clone and Build the Kaleidoscope Firmware

```sh
# go to your keyboardio directory
cd $HOME/kaleidoscope
# then clone the firmware repository
git clone https://github.com/keyboardio/Kaleidoscope.git
# and build your firmware!
cd Kaleidoscope
make
```

<3 jesse

[![Build
Status](https://travis-ci.org/keyboardio/Kaleidoscope.svg?branch=master)](https://travis-ci.org/keyboardio/Kaleidoscope)
