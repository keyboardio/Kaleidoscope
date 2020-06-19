# Installing the Arduino IDE

If you'd like to customize your keyboard's layout or functionality, the most robust and flexible option is to use the Arduino IDE. 

Arduino is one of the world's most widely used (and user friendly) platforms for programming "embedded" devices like the chip inside your keyboard. 

The first step is to install the Arduino "Integrated development environment" or IDE:

* [Install Arduino on macOS](#Arduino-macOS)
* [Install Arduino on Linux](#Arduino-Linux)
* [Install Arduino on Windows](#Arduino-Windows)
* [Install Arduino on FreeBSD](#Arduino-FreeBSD)


## <a name="Arduino-macOS"/>Install Arduino on macOS

The Arduino system has been designed to be accessible to people at all skill levels, and Keyboardio is built on top of the Arduino platform because we share that goal. There are many ways to set up your system to work with the Keyboardio firmware, the most typical is to use the Arduino Integrated Development Environment (IDE); an application that gives some visual context to the code you want to send to your Arduino device. This is the easiest process for folks who are new to Arduino, or to programming generally. If you follow the instructions below step by step you should be fine. :-)

<h4>Step One: Set up the Arduino IDE</h4>

 1. Download the Arduino IDE install package from https://www.arduino.cc/en/Main/Software

As of this writing, the latest version is v1.8.8, which you can download from https://www.arduino.cc/download_handler.php?f=/arduino-1.8.8-macosx.zip

 2. Double-click "arduino-1.8.x-macos.zip" to uncompress the Arduino IDE. 

 3. Move Arduino.app from your `Downloads` folder to your Applications folder.

 4. Double-click on Arduino.app to start it.

Next step: [Add keyboard support to Arduino](Add-keyboard-support-to-Arduino)


# <a name="Arduino-Linux"/>Install on Linux

The Arduino system has been designed to be accessible to people at all skill levels, and Keyboardio is built on top of the Arduino platform because we share that goal. There are many ways to set up your system to work with the Keyboardio firmware, the most typical is to use the Arduino Integrated Development Environment (IDE); an application that gives some visual context to the code you want to send to your Arduino device. This is the easiest process for folks who are new to Arduino, or to programming generally. If you follow the instructions below step by step you should be fine. :-)

<h4>Step One: Set up the Arduino IDE</h4>

1. Install version 1.8.8 or newer of the Arduino IDE from http://arduino.cc/download.  Unfortunately, the version packaged in Ubuntu is too ancient to support Arduino's new way of doing 3rd-party hardware.

2. Assuming you're untarring in the download directory:

    ```sh
    $ cd ~/Downloads
    $ tar xvf arduino-1.8.8-linux64.tar.xz
    $ sudo mv arduino-1.8.8 /usr/local/arduino
    $ cd /usr/local/arduino
    $ sudo ./install.sh
    ```
3. On Ubuntu, you will have to prevent ModemManager from attempting to grab the keyboard virtual serial port when it goes into bootloader mode:

    ```sh
    $ wget https://raw.githubusercontent.com/keyboardio/Kaleidoscope/master/etc/99-kaleidoscope.rules
    $ sudo cp 99-kaleidoscope.rules /etc/udev/rules.d
    $ sudo /etc/init.d/udev reload
    ```

4. Then disconnect and reconnect the keyboard for that change to take effect.

5. To let Arduino talk to the device over the serial port, you'll need to be in the appropriate group.  On Ubuntu and some other Linux distributions, the group you need is the 'dialout' group.

    From a shell:

    ```sh
    $ groups 
    ```

    If you see dialout in the list, you're good to go. If you don't, you'll have to add yourself and then get Linux to recognize the change in your current shell:

    ```sh
    $ sudo usermod -a -G dialout $USER
    $ newgrp dialout   # or su - $USER, or log out and in again
    ```

    On Arch linux, Manjaro linux, and probably other Arch derivatives, the group for the device access is `uucp`. To add yourself to this group if necessary, use

    ```shq
    $ sudo usermod -a -G uucp $USER
    $ newgrp uucp   # or su - $USER, or log out and in again
    ```

6. You may have to tweak the `ARDUINO_PATH` (put this line in your shell-rc)

    ```sh
    export ARDUINO_PATH=/usr/local/arduino
    ```

Next step: [Add keyboard support to Arduino](Add-keyboard-support-to-Arduino)

# <a name="Arduino-Windows"/>Install on Windows

The Arduino system has been designed to be accessible to people at all skill levels, and Keyboardio is built on top of the Arduino platform because we share that goal. There are many ways to set up your system to work with the Keyboardio firmware, the most typical is to use the Arduino Integrated Development Environment (IDE); an application that gives some visual context to the code you want to send to your Arduino device. This is the easiest process for folks who are new to Arduino, or to programming generally. If you follow the instructions below step by step you should be fine. :-)

_**Note:** This tutorial has been written using Windows 10._


<h4>Prerequisite: Install USB Driver (Win 7, 8, & 8.1)</h4>

For Windows 7, 8, and 8.1, please follow the instructions on the [Installing USB Driver on Windows](Installing-USB-Driver-on-Windows) page before proceeding with the instructions on this page.

<h4>Step One: Set up the Arduino IDE</h4>

1. Download the Arduino IDE installation package from https://www.arduino.cc/en/Main/Software

As of this writing, the latest version is v1.8.8, which you can download from this URL:

https://www.arduino.cc/download_handler.php?f=/arduino-1.8.8-windows.exe

_**Note:** Some users have had difficulties with the Windows store version of the Arduino IDE. Please use the downloadable installation package._

2. Open the installation package and follow the prompts to install the Arduino IDE.

Next step: [Add keyboard support to Arduino](Add-keyboard-support-to-Arduino)

## <a name="Arduino-FreeBSD"/>Install on FreeBSD

The Arduino system has been designed to be accessible to people at all skill levels, and Keyboardio is built on top of the Arduino platform because we share that goal. There are many ways to set up your system to work with the Keyboardio firmware, the most typical is to use the Arduino Integrated Development Environment (IDE); an application that gives some visual context to the code you want to send to your Arduino device. This is the easiest process for folks who are new to Arduino, or to programming generally. If you follow the instructions below step by step you should be fine. :-)

<h4>Step One: installing support packages</h4>

1. Install the following packages required by the build system: `bash`, `gmake`, `perl5`, `avrdude`, and `arduino18`.

   ```sh
   $ sudo pkg install bash gmake perl5 avrdude arduino18
   ```

2. Flashing firmware as non-root.

   a. If you want to flash your firmware as non-root, ensure your user has write access to the appropriate USB devices in devfs. By default, the devices are owned by `root`:`operator`, so put yourself in the `operator` group. You will also need to add yourself to the `dialer` group to use the modem device:

      ```sh
      $ sudo pw groupmod operator -m $USER
      $ sudo pw groupmod dialer -m $USER
      ```

   b. Add devfs rules for write access for operator to USB devices:

      ```sh
      $ cat << EOM >> /etc/devfs.rules
      #
      # Allow operators access to usb devices.
      #
      [operator_usb=5]
      add path usbctl mode 0660 group operator
      add path 'usb/*' mode 0660 group operator
      add path 'ugen*' mode 0660 group operator
      EOM
      ```

   c. Update `/etc/rc.conf` to use the new devfs rule as the system rule:

      ```sh
      $ sudo sysrc devfs_system_ruleset=operator_usb
      ```

   d. Restart devfs:

      ```sh
      $ sudo service devfs restart
      ```

