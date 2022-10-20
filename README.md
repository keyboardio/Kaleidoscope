# Kaleidoscope


Flexible firmware for Arduino-powered keyboards.

This package contains the "core" of Kaleidoscope and a number of [example firmware "Sketches"](https://github.com/keyboardio/Kaleidoscope/tree/master/examples).

If you're just getting started with the Keyboardio Model 01, the [introductory docs are here](https://github.com/keyboardio/Kaleidoscope/wiki/Keyboardio-Model-01-Introduction) and the source for the basic firmware package is here: https://github.com/keyboardio/Model01-Firmware. It's probably a good idea to start there, learn how to modify your keymap and maybe turn some modules on or off, and then come back to the full repository when you have more complex changes in mind. (The firmware for all other devices is inside examples/Devices in this Kaleidoscope repo.)

# Getting Started


If you want to use Kaleidoscope to customize or compile a "sketch" to power a supported keyboard, the fastest way to get started is to use the Arduino IDE. You can find [setup instructions](https://kaleidoscope.readthedocs.io/en/latest/quick_start.html) on [kaleidoscope.readthedocs.io](https://kaleidoscope.readthedocs.io)

If you prefer to work from the command line or intend to work on Kaleidscope itself, please follow the instructions below. It's important to note that the Arduino IDE needs the source code laid out in a slightly different arrangement than you'll find in this repository. If you want to use the Arduino IDE, you should follow [these instructions](https://kaleidoscope.readthedocs.io/en/latest/quick_start.html) instead. Even if you're using the command line, you may want to refer to the docs for troubleshooting steps.

# Use git to check out a copy of Kaleidoscope

1. Open a commandline shell and navigate to where you want to check out Kaleidoscope.

For the purposes of this tutorial, we'll assume your checkout is in a directory called `kaleidoscope` inside a directory called `git` in your home directory.

```sh
$ mkdir ${HOME}/git
$ cd ${HOME}/git
```

2. Download the latest version of Kaleidoscope

```sh
$ git clone https://github.com/keyboardio/Kaleidoscope
```

3.  Ask Kaleidoscope to install the compiler toolchain, arduino-cli and platform support

```sh
$ cd ${HOME}/git/Kaleidoscope
$ make setup
```

4.  Tell your shell where to find your Kaleidoscope installation. This example is for bash. If you're using another shell, consult the shell's documentation for instructions about how to set an environment variable

```sh
$ export KALEIDOSCOPE_DIR=${HOME}/git/Kaleidoscope

$ echo "export KALEIDOSCOPE_DIR=${HOME}/git/Kaleidoscope" >> ${HOME}/.bash_profile
```

5.  Build the Kaleidoscope Firmware for your keyboard

(This part assumes you're building firmware for the Keyboardio Atreus)

```sh
$ cd examples/Devices/Keyboardio/Atreus

$ make compile
```

5.  Install your firmware

```sh
$ make flash

```

<3 jesse

