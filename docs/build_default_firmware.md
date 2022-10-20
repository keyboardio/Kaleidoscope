# Build and install the latest firmware for your keyboard

# Select your keyboard

1. Open the 'Tools' menu, click on 'Board' and then click on the name of your keyboard. In the screenshot, we picked 'Keyboardio Model 01'. (You may have to scroll through a long list of other boards to get there.)


![](images/arduino-setup/select-board-in-menu.png)

2. Open the 'Tools' menu, click on "Port > ".  If your keyboard is not already selected, click on it to select it. (If there is only one option such as "COM3" try it, it's almost certainly the correct port.)

![](images/arduino-setup/select-port.png)

Next step: [Install the latest firmware on your keyboard](#Install-latest-firmware)

# <a name="Install-latest-firmware" />Install the latest default firmware on your keyboard

To load the firmware, open the Arduino IDE's "File" menu, and click on the "Examples" submenu. 

If you're using a Keyboardio Model 01, Scroll down to 'Model01-Firmware':

![](images/arduino-setup/select-model-01-firmware.png)

If you're using another keyboard, you should find it under Examples -> Kaleidoscope -> Devices -> (Your keyboard maker) -> (Your keyboard)

After you pick your example sketch, Arduino wil open a new window showing the sketch's source code above a black message log section and a green status message bar at the bottom. The message log displays detailed information about what Arduino is doing.

_**Note:**  We recommend that you install the default firmware at least once before you start to make changes. This gives you a chance to ensure that the firmware update process works._

# Build the firmware

Click the check mark icon below "File" to build your firmware. 

![](images/arduino-setup/verify-model-01-firmware.png)

If the firmware builds successfully, Arduino reports "Done Compiling" in the green status bar.

![](images/arduino-setup/verify-ok.png)

If something goes wrong, the status bar turns orange and displays an error message. Additionally, there may be text in the black message log with more details about the error. At this point, it may be helpful to expand the message log so that you can see a bit more about what's going on.


![](images/arduino-setup/verify-failed.png)

# Install the firmware

(If you are updating the firmware on a Keyboardio Model 100 for the first time, at this point, you may need to disconnect the keyboard from your computer, hold down the `Prog` key and plug the keyboard back in to put it into programming mode. Once you plug the keyboard back in, the `Prog` key will glow red. You may then continue with this tutorial.)

If your keyboard has a programming interlock key, you'll need to hold it down now. On the Keyboardio Model 01, this is the `Prog` key. On the Keyboardio Atreus, this is the `Esc` key.

Without releasing that key, click on the "right arrow" button in the sketch window menu bar. This starts the firmware installation process.


![](images/arduino-setup/press-prog.jpg)
![](images/arduino-setup/press-prog-atreus.jpg)
 
![](images/arduino-setup/upload-sketch.png)

If the process is successful, Arduino will tell you that in the status area. Some keyboards may also use LEDs to report their results. For example, the Model 01's LED's flash red across the board as the firmware is installed, and then the "LED" key glows blue. 

On Windows, you may also see the message "the device Model 01 is undergoing additional configuration." 

If you have any trouble flashing your keyboard's firmware, check to see if the issue is addressed on the [Troubleshooting Firmware Upload Issues](https://github.com/keyboardio/Kaleidoscope/wiki/Troubleshooting-Firmware-Upload-Issues) page. Note that if you already have any customized data in the keyboard's EEPROM, any layout differences between the keyboard's original and current firmware may cause issues. See [Using EEPROM](customization/eeprom.md) for ways to correct any problems.

