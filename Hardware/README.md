# Hardware of the Athan Lamp
This is the part that taught me the most, consumed the most time and energy, and which I enjoyed the most.

This is one of the first big electronics projects which I made.

## Planning the hardware
I began by planning the project and brainstorming it, I wanted a 12V LED strip inside the mosque so it can act as a lamp, and I wanted a loud speaker so it can be heard around the house, and I wanted an external flash chip to store high quality athan audios. In the end I made the following decisions because of some technical and financial limitations:
* Using a cheap ATmega328P MCU instead of an expensive wireless MCU like ESP32 or ESP8266. This would make it harder to get the athan times accurately because I cannot fetch them from online APIs, and I cannot get the local time accurately from online, but I solved this by using an RTC module for time tracking and implementing an offline athan time calculation code in C, which was not too difficult for me because of my background in C and programming in general. This decision saved me approximately 350 EGP (Egyptian Pounds): ATmega328P and DS3231 RTC module together cost approx. 250 EGP, ESP32 costs approx. 600 EGP.
* Using the SMD version of ATmega328P instead of THT. I only did this because it looked nicer and it saved space on the PCB to fit inside the mosque, but this decision caused me big problems during the fabrication stage which is explained later in the README.
* Using 3.3V instead of 5V to power the ATmega328P. Most reference designs online power the ATmega328P using 5V but this means that the chip will use 5V logic which is not supported by the flash IC I chose (W25Q), so I power it using 3.3V to make the logic level 3.3V to directly communicate with the flash IC without expensive level shifters.
* Designing two voltage regulation circuits. I wanted to use the LM2596 voltage regulator to step down 12V input from the wall adapter to 3.3V for the circuit but I could not find the 3.3V version of this chip locally in Egypt, but I found the 5V version of this chip and a cheap 3.3V linear converter called AMS1117, so I designed a circuit to step down from 12V to 5V using LM2596 then from 5V to 3.3V using AMS1117. I didn't regulate voltage from 12V to 3.3V directly using AMS1117 because AMS1117 is a linear regulator which dissipates power as heat and has low efficiency, so I thought it's a better idea to first use LM2596 which is a switching regulator to do most of the work then use AMS1117 to do the rest of the work to finally output 3.3V. I'm not sure if the heat dissipated by the AMS1117 with my input voltage and required current would have been a problem but I still wanted to do this two stage regulation circuit to gain experience.
* Using TDA2003 audio amplifier. I chose this amplifier because it can operate at 12V which I already have from the wall adapter. And because this amplifier is made for cars it can output a very high volume to the speakers.
* Using 16MHz crystal oscillator for the MCU. The ATmega328P spec does not officially support this clock speed at 3.3V but I found many people online who say it works even if it's not officially supported so I will take the risk.
* Using PWM + an RC filter instead of a DAC to output audio from the MCU to the amplifier. DACs are too expensive and I think the audio quality from PWM would be good enough, I don't know if this will make the audio quality bad but I will find out when I finish soldering the project and writing the firmware.
* Limiting the PCB to 2 layers only because I will fabricate the PCB by myself using a CNC machine.

## PCB Fabrication
This was the most difficult part of the project and it had the most trial and error. I didn't want to outsource the PCB fabrication because
* I wanted to do as much as possible from the project by myself. And I already had some experience in PCB fabrication.
* I wanted to get more experience in PCB fabrication.
* I didn't want to spend time I spent a little bit more than a week trying to fabricate the PCB and I kept failing each time until it worked, and in the end I still had to use a bodge wire because one of the traces on the PCB was discontinuous because it's too thin.
You can see all the failed attempts below,

