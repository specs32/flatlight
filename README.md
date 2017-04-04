# flatlight

__What is Flatlight?__

When the Phone of a friend died there was a lipo left, from installing lights for someone 
there were some LED Strips left - i found a small switch in a box, i had a soldering iron and some wires.

Thats how it started... but it has grown.

This Project is intended for educational and private use.

A powerful light, that nicely fits into your pockets because it's flat like a phone ;)

Rechargeable - Temperaturcontrolled - Magnetic - Versatile

It's Modular Design allows for the LED side or the µController side to be exchanged quickly and easily.
The Battery is commonly available and exchangeable.

Personally I build the complete Flatlight myself, from bare copper PCB to the finished device, including printing the transparencys, exposing, developing, etching, cleaning, filing, placing the components and toasting the board on my gas stove.
For more information take a look at my youtube channel, the link is down below.


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

and done.

<B>The Software is Licensed under the GNU Public License 3.0, please include that in your Code</B>

Both Licenses can be foud in the docs folder


__Something by the Way__

- This is my first git project, im sure there are errors, please tell me and I'll try and fix them.
- I'm nowhere near being a good Programmer, the code works for me although it is not very elegant.
- There might be a Video where i build a complete Flatlight, so you can see how it is intended to be. Until then you could take a look into my channel : https://www.youtube.com/channel/UCBQ4cEZi6Pdw4T7Wq-Gdzzg 
- And yeah, I'm also nowhere near being a good actor, cameraman, etc. etc.
- The bare Material for one Flatlight is around 17 €uros for me..
- The build Time from the point where i have printed transparencys to the point where im ready to pour epoxy on everything is around 6 Hours when completely building the PCB at home. (etching and so on)
- If you want a Flatlight but do not have the tools or material to make one I might build one for you if you like just leave me a message.

HAVE FUN !!!!


-Nico
