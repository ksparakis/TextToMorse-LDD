TextToMorse-LDD
===============

This is a Linux Device Driver, Includes the Kernel Module and converts text into a blinking mores code led. Developed originally for the raspberry pi.

Hardware Setup
---
(Note I used a raspberry pi model B, but should work for other similar devices)
using a bread board or any other type of connector
connect the led to "GPIO PIN 17" (Depending on your device or need you can change this)
and to the ground.

How To Run
-------
(Make sure you have root permission to run this code. This means either A, that you log in to root or B, you use Sudo Before all commads.)

####First Step - Compiling The Kernal Module Code

cd into km directory and use the command "make"

####Second Step - Compiling The User Level Code
cd into ul directory and use command "make"

####Third Step - Running the kernal module
#####Running the script
- "chmod +x runMe.sh" (Must be in main directory)
- "./runme.sh"

#####Doing it manualy
- "mknod /dev/text2Morse c 60 0" (Can do this from file location)
- "insmod text2Morse.ko" (Must be in km directory)

####Step Four - Running the user level code
- Move into the ul directory
- execute ./morseUL "Text To Be converted to blinking morse"
