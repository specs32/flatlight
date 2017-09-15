# flatlight

__What is Flatlight?__


A (relatively) powerful light, that nicely fits into your pockets because it's flat like a phone ;)

This Project is intended for educational and private use. (Probably to show how not to do a few things^^)

I designed the "Flatlight" myself, searching for circuits online, creating the schematics and layout with KiCAD.
I  built a few prototypes from bare copper PCB to the finished device, including printing the transparencys, exposing, developing, etching, cleaning, filing, placing the components and toasting the board on my old camping gas stove.

And as you can see im not much of a programmer ...

I got a lot of positive feedback from everyone, so I thought its time to share ! :)

__Building the Flatlight__

You should find everything you need in the KiCAD Folder to get started Hardwarewise.
I tried my best to add Comments in the Schematics. 
There is also a Sample Layout file which you may use and modify.
Also there are ready made SVGs so you can just print your transparency and start etching.
You might wanna take a look how to build a Flatlight on my youtube channel.

<B> The Hardware is Licensed under CC-BY-SA 4.0 please include this on your Silkscreen or TopCopper</B>

Both Licenses can be foud in the docs folder


__Code for the Flatlight__

It started out as a C++ Project in Eclipse, but i found that pretty difficult to set up and handle, so i switched to arduino.
The Arduino Programm is for the Flatlight with an ATMega 8 µController. The Software and Hardware in this Project fit together, if you modify your layout or the PINs in the Schematic, make sure to adapt the Code to that changes.
When I have the Hardware in my hands i usually hook it to a USB ISP, in my case an USBASP v2.0 and:

- Set the fuses on a new ATMega chip. (help is in the docs)
- Use the programmer and ISP of your Choice, i usually export as HEX file and flash with avrdude.

<B>The Software is Licensed under the GNU Public License 3.0, please include that in your Code</B>

Both Licenses can be foud in the docs folder

Thanks to everyone who supported me  !!!

HAVE FUN !!!

-Nico

PS: thanks and credits to cpldcpu for the nice tiny library !
