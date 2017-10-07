<h1>Keyboardio Model 01 Customization</h1>

<h2>Introduction</h2>

Welcome to your Keyboardio!

<h3>Audience</h3>

This document is intended for folks who are new to Arduino programming or perhaps to programming in general, and want a quick tutorial on how to get started customizing your keyboardio. It assumes you are reasonably clever and can follow instructions. It will give you some background on keyboards in general, and the keyboardio in specific, Then it will take you through setting up the Arduino Integrated Development Environment (IDE) on your computer, and finally making simple changes to your firmware.

When you have worked through this document you will be able to modify the definitions of the keys on your keyboardio, enabling you to do tasks such as swapping BACKSPACE and SPACE. You will also be able to turn on and off the various standard LED pattern modules.

If you're already familiar with Arduino and/or prefer to work on the command line of your computer I encourage you to look at the Model 01 git repository.

If you already know how to do this:

```
mkdir -p hardware/keyboardio

## then clone the hardware definitions to make them available to the arduino environment:

git clone --recursive https://github.com/keyboardio/Arduino-Boards.git hardware/keyboardio/avr
```

then you will probably find this document tedious. Perhaps glance at the next section that gives an overview of how the firmware is set up for a high level understanding, and the Appendix with keymap definitions may be a useful reference.

This is only a start-up guide. The Keyboardio team hopes you will move on to making bigger and better improvements to your keyboardio, and will share them with the community. There is an active forum full of helpful people here:

https://community.keyboard.io/

where you will find both experienced folks and others who are new to programming. Your questions and triumphs will be welcomed and celebrated if you feel like sharing them, and we sincerely hope you will ask questions if things aren't working for you. Keyboardio was built so people like you can experiment with customizing your environment, and then share what you learn with other folks who will appreciate your work and be inspired by it.

<h3>Keyboardio Firmware Overview</h3>

Keyboards work by receiving your keypresses and translating them into "keycodes".  Those keycodes are passed to your computer's operating system, which can interpret them and turn into characters and control codes. Those characters and codes are passed on to the active application on your computer. Some examples:

 * Press the "A" key while editing a document, and "a" will be inserted where your cursor is currently positioned.

 * Hold down "shift" and press the "A" key while editing a document, and "A" will be inserted where your cursor is currently positioned.

 * Press the "up arrow" key, and the cursor will move up a line in the document.

In order to change what happens when you press a key on your keyboardio, you will need to edit the firmware configuration file to change the keycode it sends to your computer's operating system when you press a given key. To say that a bit more succinctly, you will edit a keymap. 

_**NOTE:** There's an Appendix at the end of this document which lists all the available keycodes, and what they do._

<h4>Editing Keymaps</h4>

The keyboardio ships with three keymaps installed: QWERTY, FUNCTION, and NUMPAD. The laminated sheet that came with your keyboard shows the QWERTY layer in black, the FUNCTION layer in blue, and the NUMPAD layer in red. The back side of the sheet has a blank layout you can write on with a grease pencil or a whiteboard marker to represent a changed layout.

The default layer is numbered zero, and the zero-layer that ships on the keyboardio is called "QWERTY". 

_**NOTE:** If you want to use a different layout, such as Dvorak and Colemark, you will find those keymaps and instructions to swap out the default layer in an Appendix._ 

The next layer up is 1, which is FUNCTION. 2 is NUMPAD. The FUNCTION layer is activated when one of the FN palm keys is held down. NUMPAD gets toggled on and off with the NUM key in the upper right of the right half of the keyboardio.

Imagine the default layer as the base of a stack, and the higher layers as overlays. Keys on the upper layers can be transparent, in which case the default layer "shines through."

For example: if NUMPAD is toggled on, all the keys on the left half of the keymap (and some keys on the right side) are transparent. If you press a key on the left half, the keycode corresponding to that key on the QWERTY map will be generated, unless you are also holding down function, in which case you'll send the relevant keycode from the FUNCTION keymap.

<h4>Obvious Keys to Re-map</h4>

The ANY key and butterfly key were intentionally left without important "jobs" in the layout, so there are convenient "empty" positions on the physical keyboard to play with. 

The LED key is another key you may wish to re-map to a less central place on the keyboard, and move something you use more into that spot. For instance, it could get added to an empty spot in the FUNCTION map.

Similarly, the PROG key is avaliable. It sends a special signal to the keyboardio when you hold it down that allows the firmware to be over-written, but is free for mapping. Since it's in the usual place for ESC, you may wish to make the PROG position in the default keymap an extra ESC key. 

The eight thumb keycaps were deliberately all made the same shape so they can be swapped. If you think you would like to rearrange the locations of these keys, move them in the keymaps and rearrange the keycaps!

<h4> Limitations to Configuring Keyboardio Layout</h4>

There are some limitations to what the Keyboardio can be configured to do that are related to how keyboards and operating systems work.

Firstly, the operating system has control of what happens when you hold down "shift". It makes sense that pressing "A" generates the character "a", and "shift + A" generates the character "A".

What makes less sense is that "shift + 4" will always generate the character "$", no matter where you have the "4" key mapped on the keyboard. It is _possible_ to make "shift + 4" create a character other than "$" in the keyboardio firmware by enabling a plugin called ShapeShifter:

https://github.com/keyboardio/Kaleidoscope-ShapeShifter

But it's probably better to consider that an advanced option, and try a few more straightforward things first. 

If you want to change the shifted character on a key, the change will need to be made in the operating system. Look below for the link to the Keyboardio forum; there are discussions on the forums about how to do that, and friendly folks there who can answer questions.

Another thing to be aware of is the prog key. It sends a special signal to the keyboardio when you hold it down that allows the firmware to be over-written. 

[comment]:<> (@jesse @algernon are there more limitations that should be documented?)

<h2>Other Resources</h2>

 * Keyboardio forum:
    https://community.keyboard.io/

 * Git repository for the Model 01 basic firmware: 
 	https://github.com/keyboardio/Model01-Firmware

 * Git repository for Kaleidoscope, which contains all the keyboardio modules:
	https://github.com/keyboardio/Kaleidoscope

 * Git repository for Calliope, the GUI tool for configuring the keyboardio that is still under development:
	https://github.com/Lepidopterarium/Chrysalis


<h2>Install Arduino support</h2>

The Arduino system has been designed to be accessible to people at all skill levels, and Keyboardio is built on top of the Arduino platform because we share that goal. There are many ways to set up your system to work with the keyboardio firmware. This is the easiest process for folks who are new to Arduino, or to programming generally. If you follow the instructions below step by step you should be fine. :-)

<h4>Step One: Set up the Arduino IDE</h4>

Download the Arduino IDE install package from:

 https://www.arduino.cc/en/Main/Software

Click on your operating system name to download the correct version.

_**Note:** This tutorial has been written using the Mac version._

Install the Arduino IDE. On a mac the application is called "Arduino.app". You probably want to put it in your Applications directory. 

<h4>Step Two: Install keyboardio support into the IDE</h4>

It will open a default sketch; just ignore that. (If you close the default sketch window the application will close.)

Open the “Arduino” menu and click on “Preferences”

At the bottom of the "Settings" tab is the 'Additional Board Manager URLs` box. Paste this into it: 

https://raw.githubusercontent.com/keyboardio/boardsmanager/master/package\_keyboardio\_index.json

and click ‘OK’ to close the dialog

Open the ‘Tools’ menu, click on ‘Board’ and then click on ‘Boards Manager’

Type ‘Keyboardio’ into the search box. You will see an entry that says "keyboardio by Keyboardio" Click on it to select it, and then click ‘Install’. Once the install completes, click "Close".

Open the ‘Tools’ menu again, click on ‘Board’ and then click on ‘Keyboardio Model 01’ – You may have to scroll through a long list of other boards to get there.

Plug in the keyboardio.

Open the Tools menu again, and scroll down to "Port > ". Select the Keyboardio. (It may already be seleted.)

<h4>Step Three: Build the default firmware and load it on the Keyboardio</h4>

Click on the “File” menu, click on the “Examples” submenu. Click on ‘Model01-Firmware’

While holding down the prog key, click on the right arrow in the sketch window menu bar to compile compile and install.

The Keyboardio LED's will flash across the board as the firmware is installed, and then the "LED" key will glow blue. 

Congrats, you've just installed the latest firmware!!

<h2>Edit Keymap</h2>

<h2>Build and Update firmware</h2>

<h2>Edit LED patterns</h2>

<h2>Create Macros</h2>

<h2>Ideas to Explore</h2>

<h2>Next Steps</h2>

<h1>Appendix 1: Default layouts</h1>

<h1>Appendix 2: Keycode translations</h1>
