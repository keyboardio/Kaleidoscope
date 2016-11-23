We're in the midst of refactoring our really bad prototype firmware into a more reasonable production firmware.

Right now, the code is really ugly. It'll be nicer soon. Please don't judge us too harshly.


# Getting Started

Pick a directory to work in, you'll need to clone multiple repositories. We'll assume you picked `$HOME/keyboardio`, if you chose another adapt the commands below accordingly.

## Setup the Arduino IDE

Setup the Arduino IDE on your system. Make sure you install at least version 1.6, since older version may not support all required features.

* On Linux, follow the instructions [on the wiki](https://github.com/keyboardio/KeyboardioFirmware/wiki/Arduino-Setup-Linux).
* On macOS, install using [homebrew](http://brew.sh/) [cask](https://caskroom.github.io/) with `brew cask install arduino` or download the application from [the official website](https://www.arduino.cc/en/Main/Software) and move it to your `/Applications` folder.

## Install the Keyboardio Hardware Definitions

```sh
# you'll need to know your arduino sketchbook directory

# on macOS the default is
SKETCHBOOK_DIR=$HOME/Documents/Arduino
# on Linux the default is
SKETCHBOOK_DIR=$HOME/Arduino

# go to your keyboardio directory
cd $HOME/keyboardio
# then clone the hardware definitions
git clone --recursive https://github.com/keyboardio/Arduino-Boards.git
# and make them available to the arduino environment
mkdir -p $SKETCHBOOK_DIR/hardware/keyboardio
ln -s $HOME/keyboardio/Arduino-Boards $SKETCHBOOK_DIR/hardware/keyboardio/avr
```

## Clone and Build the Keyboardio Firmware

```sh
# go to your keyboardio directory
cd $HOME/keyboardio
# then clone the firmware repository
git clone https://github.com/keyboardio/KeyboardioFirmware.git
# and build your firmware!
cd KeyboardioFirmware
make
```

<3 jesse

[![Build
Status](https://travis-ci.org/keyboardio/KeyboardioFirmware.svg?branch=master)](https://travis-ci.org/keyboardio/KeyboardioFirmware)
