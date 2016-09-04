Defcon 24 Car Hacking Village Badge Native SDK Overview
Copyright 2016 Specialized Solutions LLC
nhoch <at> specializedsolutionsllc <dot> com if you have any problems. 

-----------------------------------------------------------------------------------
Intro
-----------------------------------------------------------------------------------
This file gives an overview of how to use the native SDK to develop C/C++/ASM code for the badge.  What you can do with this SDK:

1. Use the included libraries to make your own stuff
2. Use ST's Cubesuite to make device driver libraries and more

What is not there:

1. The PAWN environment that ships with the badge is not included (PAWN itself is not closed source, and I have not modified it.  Everything else surrounding it is closed source for now - please try to convince me to open it up)
2. See the file title "Specialized Solutions LLC Licensing Agreement.txt" for more let downs :-(

-----------------------------------------------------------------------------------
Required Tools
-----------------------------------------------------------------------------------
You will need a compiler / IDE.  I use emBitz which can be found here (http://www.emblocks.org/web/),  and it will load the project files I have included.  But you can also use GCC and your own build/edit environment if you would like.  emBitz just uses GCC under the hood.

You will also need a way to load code onto the STM32F205RE part that is included on the badge (I didn't skimp on the micro this year - this is a very capable Cortex-M3 processor with 512k of flash and 128k of RAM).

You can use any JTAG probe that can do single-wire debugging.  I have used both a Segger J-Link (expensive) and ST's ST-Link (cheap).  emBitz supports both natively and very well (not slow and crappy (hello? no reset button?) like how Eclipse does ARM-based debugging).

The badge uses a custom 0.1" pitch, single-row, 5-pin header for the debug interface.  The pinout is:

Pin 1 - 3.3 volts
Pin 2 - GND
Pin 3 - TMS / SWDIO
Pin 4 - TCK / SWCLK
Pin 5 - Reset (active low)

When connecting these pins to the standard 20-pin, dual-row, ARM debug header found on almost all ARM JTAG probes, the pins are:

Pin 1 On Badge --> Pin 1 on ARM header
Pin 2 On Badge --> Pin 4 (and others) on ARM header
Pin 3 On Badge --> Pin 7 on ARM header
Pin 4 On Badge --> Pin 9 on ARM header
Pin 5 On Badge --> Pin 15 on ARM header

-----------------------------------------------------------------------------------
Software Overview
-----------------------------------------------------------------------------------
I have included the stock setup for the STM32F205RE that emBitz creates for you.  I have also included ST's "legacy" USB stack for this part that you will need to import into the project and build if you want to use USB.  I have also included ST's "legacy" perpheral library that has lots of examples of how to use the peripherals.

ST has abandoned the legacy libraries because they are now pushing Cubesuite.  If I were you, I would give Cubesuite a shot if you need things like USB/file system drivers/FreeRTOS/etc.  Cubesuite can automate the building of this code for you.  Supposedly Cubesuite can output something that is like the legacy libraries now, but I do not think that this feature is available on all families (like the STM32F2's)

I use the legacy libraries because they are small and fast.  They just get it done without the bloat and I can have them work just like how I want them to (ok, maybe I have some control issues).

Because it is tough to get the LCD up and going, I have included a file to show you what needs to be initialized on the LCD and how to write pixels to it.  You will need to port this code over to use whatever SPI libraries you put in place and/or use.

I have also included an example on how to setup CAN so that it can do something useful.  This peripheral can be a little bit tricky to get the filters working correctly for both channels.

I have also included some code to control the WS2812B RGB LEDs on the badge.

-----------------------------------------------------------------------------------
Hardware Overview
-----------------------------------------------------------------------------------
I have included a schematic of the badge so you can know what you are doing.  I have also included data sheets for some of the more esoteric parts (like the LCD), because it is really hard to find them on the web.

I am hoping that with the debugging headers that I have provided, you are able to interface the badge to many other things.  A lot of the peripherals can be remapped to the debug headers.

