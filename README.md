TextToMorse-LDD
===============

This is a Linux Device Driver, Includes the Kernel Module and converts text into a blinking mores code led. Developed originally for the raspberry pi.

How To Run
-------
(Make sure you have root permission to run this code. This means either A, that you log in to root or B, you use Sudo Before all commads.)

####FIRST STEP - Compiling The Kernal Module Code

cd into km folder and use the command "make"

####Second Step - Compiling The User Level Code
cd into ul folder and use command "make"

####Third Step - Running the kernal module
#####Running the script
- "chmod +x runMe.sh" (Must be in main directory)
- "./runme.sh"

#####Doing it manualy
- "mknod /dev/text2Morse c 60 0" (Can do this from file location)
- "insmod text2Morse.ko" (Must be in km directory)

