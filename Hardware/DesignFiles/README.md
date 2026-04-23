# Design Files

The hardware design files currently available are the KiCAD files and the BOM. The enclosure has not been designed yet.

## Notes and Instructions
* It is recommended to read the `/Hardware/README.md` file to learn from my mistakes when milling the PCB.
* In KiCAD the PCB is in the top left of the screen outside the work area. I did this so the origin point of the CNC machine can be on the bottom left side of the bed because that is the flattest area on the CNC machine I am using, you should change the PCB location according to your CNC bed's flatness.
* There are 4 holes on each PCB corner for 1.5mm nails, they are also outside the PCB location by few millimeters. Please change the hole diameter and distance from PCB corners to match your requirements.
* Dont forget to add two alignment nails to help in flipping the PCB. I made space on the bottom side of the PCB for nails. Make sure to keep the nails at equal distance from the PCB corners so both PCB layers are aligned perfectly.
* Use a 0.3mm or smaller endmill.
* Make sure the PCB is perfectly flat on the bed.
* Remember to only use 3 of the nails which are outside the PCB area, because you must remove the nail which is at the CNC origin point so the drill bit doesn't cut the nail and be damaged.
* Remember to remove the alignment nails when milling so the drill bit doesn't cut the nail and be damaged.
