# flatlight

__What is Flatlight?__

This Project is intended for private use. A powerful light, that nicely fits into your pockets because it's flat like a phone ;)
Rechargeable - Temperaturcontrolled - Magnetic - Versatile
It's Modular Design allows for the LED side or the µController side to be exchanged quickly and easily.
The Battery is commonly available and exchangeable.

I personaly build the complete Flatlight myself, from bare copper PCB to the finished device, including printing the transparencys, exposing, developing, etching, cleaning filing, placing the components and toasting the board on my gas stove.
For more information take a look at my youtube channel, the link is down below.


__Building the Flatlight__

You should find everything you need in the KiCAD Folder to get started Hardwarewise.
I tried my best to add Comments in the Schematics. 
There is also a Sample Layout file which you may use and modify.
Also there are ready made SVGs so you can just print your transparency and start etching.
And you can watch how to build a Flatlight on my youtube channel.


__Code for the Flatlight__

It started out as a C++ Project in Eclipse, but i found that pretty difficult to set up and handle, so i switched to arduino.
The Arduino Programm is for the Flatlight with an ATMega 8 µController. The Software and Hardware in this Project fit together, if you modify your layout or the PINs in the Schematic, make sure to adapt the Code to that changes.
When I have the Hardware in my hands i usually hook it to a USB ISP, in my case an USBASP v2.0 and:
- Set the fuses on a new ATMega chip.
- Use the programmer and ISP of your Choice, i usually export as HEX file and flash with avrdude.
done.


__Something by the Way__

I'm nowhere near being a good Programmer, the code works for me although it is not very elegant.
There might be a Video where i build a complete Flatlight, so you can see how it is intended to be.
Until then you could take a look into my channel : https://www.youtube.com/channel/UCBQ4cEZi6Pdw4T7Wq-Gdzzg 
And yeah, I'm also nowhere near being a good actor, cameraman, etc. etc.

- The bare Material for one Flatlight is around 17 €uros for me..

- The build Time from the point where i have printed transparencys to the point where im ready to pour epoxy on everything is around 6 Hours when completely building the PCB at home. (etching and so on)


HAVE FUN !!!!

-Nico
