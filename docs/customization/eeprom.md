# Using EEPROM

## Why Use EEPROM?

While we've done our best to make it easy to change how your keyboard works by changing your firmware & re-flashing it, sometimes it would be convenient to be able to make changes without having to go through that rigamarole.
Maybe you'd like to be able to use a GUI like [Chrysalis](https://github.com/keyboardio/Chrysalis) to configure your keyboard layout or LED themes, or maybe your sketch is getting very complicated and you're looking for a way to save program memory.
In either case, you'll want to use EEPROM to store your settings.

## What is EEPROM?

EEPROM stands for "Electronic Erasable Programmable Read-Only Memory" and is one of the three memory mediums your keyboard has.
The other two are RAM, which is used for variables when running your code, and program memory, which is used for storing the program, as well as some other select pieces of data (if you're curious, the bit in your sketch where it says `PROGMEM` indicates that a variable is being stored in program memory instead of RAM).
RAM we want to keep as free as we can, since running our code will need some RAM to work.
While we can put stuff in PROGMEM, your code itself will take up some room there, so it may be useful to store things elsewhere.
EEPROM provides us with another place to store things that can free up RAM and PROGMEM.
Additionally, by leveraging a few plugins, we can store configuration in EEPROM and allow a GUI tool on the connected computer to change settings on the keyboard!

## Move Settings to EEPROM

There are a few important Kaleidoscope plugins for putting settings in EEPROM:

  <!-- - [Kaleidoscope-EEPROM-Keymap-Programmer][] - is this worth mentioning in this context? -->
  - Kaleidoscope-Focus - This plugin is what enables communication between your keyboard and programs running on your computer; all the following plugins require you to be using this if you want to be able to change your settings from the computer without re-flashing.
  - Kaleidoscope-EEPROM-Settings - This is a plugin that doesn't do much by itself, but most of the other EEPROM plugins will need active to be able to make use of EEPROM storage.
  - Kaleidoscope-EEPROM-Keymap - This plugin uses Focus and EEPROM-Settings to allow either overriding or fully replacing the programmed-in keymap without reflashing (by means of a program like Chrysalis running on your computer).
  - Kaleidoscope-Colormap - This plugin allows you to use a computer-side program to set a (static -- i.e. the keys won't change colour over time) LED theme for each layer.

All these plugins have minimal installation that can be found in their respective READMEs.
After following the instructions for each and adding them together, you should be able to download a program that knows how to communicate with the keyboard (i.e. [Chrysalis](https://github.com/keyboardio/Chrysalis) and you can start customizing settings without having to do any more programming!

# Migrating EEPROM contents between firmware changes

When you flash new firmware that adds or removes plugins utilizing EEPROM storage, your configuration stored on the keyboard is likely to break.
This is, because the stored data from the old firmware does not align with the storage layout of the new firmware.

> Don't worry, your config is not gone - flashing the old firmware will provide you a functioning configuration again.

To work around this breakage, the configuration can be extracted via [Focus commands][focus] before flashing - and restored after flashing.
Currently this is not part of the flashing process, but can easily be done with helper scripts [`eeprom-backup.sh`][eeprom_backup] and [`eeprom-restore.sh`][eeprom_restore].

[eeprom_backup]: https://github.com/keyboardio/Kaleidoscope/blob/master/bin/eeprom-backup.sh
[eeprom_restore]: https://github.com/keyboardio/Kaleidoscope/blob/master/bin/eeprom-restore.sh
[focus]: ../plugins/Kaleidoscope-FocusSerial.md