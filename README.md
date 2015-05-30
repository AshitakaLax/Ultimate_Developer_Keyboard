Ultimate_Developer_Keyboard
===========================

Ultimate left single hand keyboard for Developing, Macros, and gaming. Code for MCU, Code for Windows service program, keyboard Hardware schematics, Keyboard Plastic templates.


### GOALS

While there are many great keyboards out there, I'm not aware of any that can support all of these.

1. Lower Cost (Many commercial keyboards have a low price)
2. Modularity (Nearly No keyboard is modular, but there are many nice features with modularity)
3. Open Source (Parallelized work is always faster, This is a hobby for me)
4. Flexible (If I want to have 64 keys I can, If I want 105 keys, I can, If I want 1000 keys I can), if I want it to be wireless, it can be,
5. Powerful (I don't want to be limited by the hardware as to what It can support).
6. Energy Flexible ( If I want to attach a fan, and LED lamp, I can. If I want to run off a Coin cell battery I can.)

I naturally can see the conflicting attributes in the list above. But with enough fore site, and structure, It should be possible.



### ROAD MAP

These are the core features with the new version of the keyboard.

1. Layout Modular components(Keys, brains, Communications)
2. Create the Bare bones keyboard(pretty much copy 64keys, and Ergodox, directly to a USB connection)
3. Support direct I/O connection from one board to the other(like 64 keys)
4. Have keyboard Right hand keyboard run off coin cell battery, and be wireless





This Project is broken down into 3 sections

1. Keyboard Software - this is the embedded code on a custom dev board(Atmel Sam4E), with hopes to move to a smaller chip to save cost.
2. Windows Software - this will be the smart portion of the system. It will determine the current IDE, and switch to it's custom keyboard layout.
3. Keyboard Hardware - This is all of the documentation for the hardware of this system, schematics, STL's, etc...


This system will be developed with Industrial quality tools, mainly because I like my time.

Keyboard Software Tools - Atmel Studio 6.2
Windows Software Tools - Visual Studio 2010 Ultimate
Keyboard Hardware Tools - (PCB)Altium Designer ,(Plastics) TBD

Scratch Notes

right side board structure.

SAM L21E(32 pin chip 25 IO) PL0
coin cell battery ~250mAh rechargeable
cpu pwr use 25 uA/Mhz (since speed isn't a factor) 1.2 mA when active
//since we can sleep, with a specific wakeup pin we can use
sleep/standby is 25uAmp.
NRF24L01+
tx is 7 mA
power down mode 4.2uA

standby sum of power
4.2uA
25uA

This right hand design is a asynchronous design to send keys as soon as they are pressed.
 and as soon as they aren't pressed
 





