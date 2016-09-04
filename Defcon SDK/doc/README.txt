Defcon 24 Car Hacking Village Badge Overview
Copyright 2016 Specialized Solutions LLC
nhoch <at> specializedsolutionsllc <dot> com if you have any questions/suggestions/problems. 

-----------------------------------------------------------------------------------
Intro
-----------------------------------------------------------------------------------
This file shows how to use the badge to make PAWN scripts.  I recomend you take a look at the other PAWN documents in this folder and also the tests and default scripts to learn more about PAWN.

The badge this year is much better than last year's.  My goal is to out-do the previous year's badge by a mile, yet keep the price point the same.  The badge allows me to apply the things that I have picked up over the last year using a very aggressive schedule (this proves to me that I have picked it up :-)).  Just like last year, the badge has gone from "nothing" to "for sale at DefCon" in about 4 weeks.  

The badge this year supports dual CAN channels with full gateway support.  Fully-controllable pass-through (pass messages from channel 0 to channel 1, or channel 1 to channel 0), the ability to intercept a message and either modify it contents or "eat it", and/or change the ID of a message and pass the data through (either altered or unaltered) are supported.  These abilities are a must for serious vehicle hacking, and the badge implementation of PAWN makes this super easy.

The badge also sports a 128x128 color LCD which is fully accessible via the PAWN scripts.  In fact, all of the graphics the default demo script has are implemented in PAWN.

The badge has 4 WS2812B RGB LEDs that are also fully accessible via the PAWN scripts.

Demos such as displaying values on the LCD that are obtained via CAN, or controlling LED colors via CAN data are very possible.

The badge this year also gets rid of using UART to program scripts and pass data in/out of PAWN scripts.  Now USB is used to both enhance I/O speeds but also to allow for easy reprogramming (last year you had to have an FTDI USB to UART adapter).  This change will allow anyone to tinker with PAWN, provided they have a micro-USB cable (everyone should have one of these, right?). The full source code for the tools used to load scripts onto the badge are provided in the SDK.  Modifying these tools would allow for you to easily make advanced applications including your own CAN development tools.

A full native SDK will also be provided if you want to roll-your-own stuff that runs on the hardware instead of in PAWN.  Just be sure to bring your ARM SWD adapter (ST-Link/Segger/etc.).

The badge is actually a mini version of another QCM unit that is much more powerful. More CAN message handlers, more timers, more script space, and power management (i.e. sleep/wakeup) are a few of the things the full product contains.  Basically, the full product is designed to be installed in an automotive/industrial environment permanently, as opposed to just being a development tool.  It has a much better power supply and has been temperature tested.  My clients have used this more advanced unit to facilitate engine swaps (i.e. GM engine in a Chrysler vehicle), cluster swaps (i.e. Mfg X gauges in Mfg Y's vehicles), and other various custom jobs.

Many other QCM modules have been developed that support other functionality like BTLE, WiFi, LIN, SWCAN, J1850, advanced analog I/O etc.  LIN/SWCAN/J1850 modules all allow easy gateway functionality between LIN/SWCAN/J1850 and CAN. Contact me at nhoch <at> specializedsolutionsllc <dot> com if you would like to know more!

<shameless plug>
I make a living contracting my hardware and software services.  I also sell QCM devices and supply finished electronic devices for a few of my clients (clients that do not want to facilitate building the units themselves). If you see something that you think would be awesome in your product, need a product developed/produced, need embedded software assitance, or just need a friend, contact me.  I have done many custom devices/embedded software projects for automotive/other vehicles, industrial, appliance, sporting goods, and consumer electronics.  I am also friendly (at least that's what others tell me).
</shameless plug>

-----------------------------------------------------------------------------------
Making a Script
-----------------------------------------------------------------------------------
I recomend taking one of the examples and modifying it to do what you want.  You can use any text editor to edit PAWN. I use Notepad++ with the PAWN language add-on that can be found in this foler under the "Misc" directory.

See the file titled "qcm_bdg.inc" in the include subdirectory for all of the QCM-specific APIs that can be used.  You can look at the other .inc files to see the PAWN native calls that are supported as well.

-----------------------------------------------------------------------------------
Compiling a Script
-----------------------------------------------------------------------------------
The SDK is setup so that the PAWN compiler can easily find the include files and include them by default.  I recommend making a subdirectory in the SDK folder for your project.

To compile the script, invoke the pawn compiler on your script:

SDK\YourDir> ..\bin\pawncc yourscipt.p

This will produce a .amx file.

Now connect the badge to a PC using a micro USB cable.  On Windows 8 and up, the badge support driverless installation, so it should just work.  For windows 7 or other platforms, see the readme about the USB interface.

Start the .NET utility named QCM.exe in the bin subdirectory of the SDK.  It should say that the device is connected.  Now click on Load Script and point it to the resulting .amx file you made.  It should proceed to load and start.  You should hear USB disconnect and reconnect if you have your speakers turned on on Windows.

The QCM.exe utility can also be used to send text to the badge for scripts (see the @host_rx callback).  It will also display anything output using printf and qcm_host_send() on the badge.

If you are using qcm_host_send() to send binary data, you may want to make your own version of QCM.exe - I have included the full .NET/C# source for it in the "QCM USB Example" subdirectory.  You will need Visual Studio 2015
to build it (I use the community edition, which is free).

-----------------------------------------------------------------------------------
Badge Functionality
-----------------------------------------------------------------------------------
If only the bottom front LED is flashing red, then this means that a script is not loaded onto the device.

You can hold the left switch down when applying power to skip loading the script or force bootloader entry for software update.  To do this:

1. Hold down switch when applying power - LED will turn solid red.
2. Keep switch held down until led turns green.  If you release the switch within one second, the script will not be loaded and the red LED will flash until a power cycle or a script is loaded.
3. If you keep holding the switch down until the LED turns back to red, then release within one second, the unit will enter the bootloader.  This will be evident by a red/red/green/green pattern being presented on the LED.

-----------------------------------------------------------------------------------
Software Update
-----------------------------------------------------------------------------------
Enter the bootloader using the steps outlined above.  Then use the Load Firmware button on QCM.exe to load any updates supplied by Specialized Solutions.