# Kaleidoscope

Flexible firmware for Arduino-powered keyboards.

This package contains the "core" of Kaleidoscope and a number of [example firmware "Sketches"](https://github.com/keyboardio/Kaleidoscope/tree/master/examples).

If you're just getting started with the Keyboardio Model 01, the [introductory docs are here](https://github.com/keyboardio/Kaleidoscope/wiki/Keyboardio-Model-01-Introduction) and the source for the basic firmware package is here: https://github.com/keyboardio/Model01-Firmware. It's probably a good idea to start there, learn how to modify your keymap and maybe turn some modules on or off, and then come back to the full repository when you have more complex changes in mind.

# Getting Started

## Setup the Arduino IDE

Setup the Arduino IDE on your system. Make sure you install at least version 1.8.6, since older version may not support all required features.

* On Linux, follow the instructions [on the wiki](https://github.com/keyboardio/Kaleidoscope/wiki/Install-Arduino-support-on-Linux).
* On macOS, install using [homebrew](http://brew.sh/) [cask](https://caskroom.github.io/) with `brew cask install arduino` or download the application from [the official website](https://www.arduino.cc/en/Main/Software) and move it to your `/Applications` folder.

## Get into the right directory

### macOS
```sh
mkdir -p $HOME/Documents/Arduino/hardware
cd $HOME/Documents/Arduino/hardware
```

### Linux

```sh
mkdir -p $HOME/Arduino/hardware
cd $HOME/Arduino/hardware
```

### Windows (Assuming cmd)

```sh
mkdir %userprofile%\Documents\Arduino\hardware
cd %userprofile%\Documents\Arduino\hardware
```


### Install the libraries and hardware definitions

## Clone the hardware definitions

Because git for Windows doesn't always have symlink support enabled by default, you may need to enable it for Kaleidoscope. To do that, add '-c core.symlinks=true' to your commandline. On all other platforms, that option is safe, but not necessary, as symlinks should work by default.

```sh
git clone -c core.symlinks=true --recursive https://github.com/keyboardio/Kaleidoscope-Bundle-Keyboardio.git keyboardio

```

## Build the Kaleidoscope Firmware for your keyboard

(This part assumes you're building firmware for the Keyboardio Model 01)

```sh
# Go to your device firmware directory
cd keyboardio/avr/libraries/Model01-Firmware

# Build your firmware!
make


# Install your firmware
make flash

```

<3 jesse

[![Build
Status](https://travis-ci.org/keyboardio/Kaleidoscope.svg?branch=master)](https://travis-ci.org/keyboardio/Kaleidoscope)
