# Software of the Athan Lamp
This was the easiest part, I wrote the first prototype (`/Firmware/AthanTimePrototype.c`) in two days which I was free but could not go to the makerspace to continue milling the PCB. This prototype was finished before I made the first successfull PCB. I followed some guides and papers I found online explaining the astronomical method to calculate athan times from world coordinates and timezone.

After the PCB was finished, I ported the prototype to Arduino IDE and changed the code so that it controls the PCB's peripherals and takes the time input from the RTC module instead of hard coded. See `/Firmware/AthanLamp.ino`.

The file explains itself by comments.

Adafruit's RTC library "RTClib" was used in Arduino IDE.
