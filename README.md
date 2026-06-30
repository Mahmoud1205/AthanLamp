# Athan Lamp

Hardware, software, and devlog of the Athan Lamp project.

![Athan Lamp Built](/Media/AthanLampBuilt.jpeg)
![Athan Lamp LED](/Media/AthanLampLED.jpeg)

Video of the athan alarm working at the correct time: https://youtu.be/aE38t-XnRy0
Video of the LED strip being controlled by the board: https://youtube.com/shorts/xvR4h-AePn8

## ‼️ Note: The current design of the athan lamp is that it is placed inside the wall or hidden somewhere, with the LED strip coming out of it, not in an enclosure. This is why there is no enclosure. The videos of the athan lamp have the PCB outside of a wall because I couldn't drill a hole in the wall. ‼️

I recently moved to a home which is far away from a mosque, meaning I cannot hear the athan (Islamic call to prayer) anymore. I could've solve this by installing an athan application on my phone, but I can do better.

So I got the idea of making a device that's placed in the wall of my house and can play an alarm during the athan time according to my location, this device also acts as a lamp, or more accurately a house lighting because it controls a LED strip.

The electronic schematic, PCB design, and firmware, are all made by me.

I would like to keep this main README short, but check out the Hardware and Software folders for more info and implementation details. Both folders have READMEs.

![3D render of the PCB](/Media/pcb3d.png)
![Schematic of the PCB](/Media/schematic.png)
![PCB layout](/Media/pcb.png)

## Bill of Materials
| #  | References                               | Value       | Footprint                                      | Qty | Price   | Link |
|---:|------------------------------------------|-------------|------------------------------------------------|----:|---------|------|
| 1  | C2, C6, C7, C9, C10, C11, C16, C19       | 100nF       | C_0805_2012Metric                              | 8   | $0.15   | https://makerselectronics.com/product/multilayer-ceramic-capacitors-mlcc-smd-smt-100nf-%C2%B110-50v-x7r-0805/ |
| 2  | C1, C12                                  | 100uF_25V   | CP_Radial_D5.0mm_P2.50mm                       | 2   | $0.038  | https://makerselectronics.com/product/capacitor-100uf-25v-11x6-3mm/ |
| 3  | C13, C14                                 | 47nF        | C_0805_2012Metric                              | 2   | $0.038  | https://makerselectronics.com/product/multilayer-ceramic-capacitors-mlcc-smd-smt-47nf-%C2%B110-50v-0805/ |
| 4  | C15, C17                                 | 22pF        | C_0805_2012Metric                              | 2   | $0.038  | https://makerselectronics.com/product/multilayer-ceramic-capacitors-mlcc-smd-smt-22pf-%C2%B11-50v-0805/ |
| 5  | C3                                       | 10uF        | CP_Radial_D4.0mm_P2.00mm                       | 1   | $0.019  | https://makerselectronics.com/product/capacitor-10uf-25v-4x7mm/ |
| 6  | C4                                       | 22uF        | CP_Radial_D6.3mm_P2.50mm                       | 1   | $0.019  | https://makerselectronics.com/product/nichicon-aluminum-electrolytic-capacitor-22uf-16v-5x11-5mm-made-in-japan/ |
| 7  | C5                                       | 220uF_16V   | CP_Radial_D8.0mm_P3.80mm                       | 1   | $0.019  | https://makerselectronics.com/product/capacitor-220uf-16v/ |
| 8  | C8                                       | 100uF       | CP_Radial_D5.0mm_P2.50mm                       | 1   | $0.019  | https://makerselectronics.com/product/capacitor-100uf-16v/ |
| 9  | C18                                      | 330uF_25V   | CP_Radial_D8.0mm_P3.80mm                       | 1   | $0.019  | https://makerselectronics.com/product/capacitor-330uf-25v/ |
| 10 | R1, R8, R9, R13                          | 1R          | R_0805_2012Metric                              | 4   | $0.0152 | https://makerselectronics.com/product/chip-resistor-smd-1%CF%89-%C2%B11-0-125w-%C2%B1200ppm-%E2%84%83-0805/ |
| 11 | R3, R5, R12                              | 10K         | R_0805_2012Metric                              | 3   | $0.0114 | https://makerselectronics.com/product/chip-resistor-smd-10k%CF%89-%C2%B15-0-125w-%C2%B1100ppm-%E2%84%83-0805/ |
| 12 | R7, R14, R17                             | 220R        | R_0805_2012Metric                              | 3   | $0.0114 | https://makerselectronics.com/product/chip-resistor-smd-220%CF%89-%C2%B11-0-125w-%C2%B1100ppm-%E2%84%83-0805/ |
| 13 | R4, R15                                  | 1K          | R_0805_2012Metric                              | 2   | $0.0076 | https://makerselectronics.com/product/chip-resistor-smd-1k%CF%89-%C2%B15-0-125w-%C2%B1100ppm-%E2%84%83-0805/ |
| 14 | R10, R11                                 | 4.7K        | R_0805_2012Metric                              | 2   | $0.0076 | https://lampatronics.com/product/smd-resistor-4-7kohm-472-0805-10pcs |
| 15 | R2                                       | 100R        | R_0603_1608Metric                              | 1   | $0.0038 | https://makerselectronics.com/product/chip-resistor-smd-100%CF%89-%C2%B11-100mw-%C2%B1100ppm-%E2%84%83-0603/ |
| 16 | R6                                       | 39R         | R_1206_3216Metric                              | 1   | $0.0038 | https://makerselectronics.com/product/chip-resistor-smd-39%cf%89-%c2%b11-250mw-1206/ |
| 17 | R16                                      | 330R        | R_0805_2012Metric                              | 1   | $0.0038 | https://makerselectronics.com/product/chip-resistor-smd-330%CF%89-%C2%B15-0-125w-%C2%B1100ppm-%E2%84%83-0805/ |
| 18 | L1                                       | 47uH        | L_Taiyo-Yuden_MD-3030                          | 1   | $0.13   | https://free-electronic.com/product/inductor-coil-47uh-smd/ |
| 19 | D1, D2                                   | SS34        | D_SMA                                          | 2   | $0.19   | https://makerselectronics.com/product/ss34-sma-schottky-barrier-diodes-sbd-40v-550mv3a-3a/ |
| 20 | D3                                       | FW0_LED     | LED_0805_2012Metric_Pad1.15x1.40mm_HandSolder  | 1   | $0.015  | https://makerselectronics.com/product/smd-led-0805/ |
| 21 | D4                                       | 12V_STAT    | LED_0805_2012Metric_Pad1.15x1.40mm_HandSolder  | 1   | $0.015  | https://makerselectronics.com/product/smd-led-0805/ |
| 22 | D5                                       | 5V_STAT     | LED_0805_2012Metric_Pad1.15x1.40mm_HandSolder  | 1   | $0.015  | https://makerselectronics.com/product/smd-led-0805/ |
| 23 | D6                                       | 3V3_STAT    | LED_0805_2012Metric_Pad1.15x1.40mm_HandSolder  | 1   | $0.015  | https://makerselectronics.com/product/smd-led-0805/ |
| 24 | U1                                       | AMS1117-3.3 | SOT-223-3_TabPin2                              | 1   | $0.038  | https://lampatronics.com/product/ams1117-33v-sot-223-33v-linear-voltage-regulator |
| 25 | U2                                       | LM2596T-5   | TO-220-5_P3.4x3.7mm_StaggerOdd_Lead3.8mm_Vertical | 1 | $0.57   | https://makerselectronics.com/product/lm2596t-5-0-buck-fixed-40v-5v-3a-to-220-5l-dc-dc-converters-dip-ic/ |
| 26 | U3                                       | W25Q32JVSS  | SOIC-8_5.3x5.3mm_P1.27mm                       | 1   | $0.95   | https://makerselectronics.com/product/25q32-flash-memory-32mbit-4mb-soic-8-smd/ |
| 27 | U4                                       | TDA2003     | TO-220-5_P3.4x3.7mm_StaggerEven_Lead3.8mm_Vertical | 1 | $0.23   | https://makerselectronics.com/product/tda2003-car-radio-audio-amplifier-to220-5/ |
| 28 | U5                                       | ATmega328P-P| Package_DIP:DIP-28_W7.62mm                     | 1   | $2.55   | https://makerselectronics.com/product/atmega328-pu-microcontroller-for-ard/ |
| 29 | Y1                                       | 16MHz       | Crystal_HC49-4H_Vertical                       | 1   | $0.057  | https://lampatronics.com/product/crystal-oscillator-16mhz |
| 30 | SW1                                      | RST_BTN     | SW_PUSH_6mm_H4.3mm                             | 1   | $0.014  | https://lampatronics.com/product/push-button-4pin-6x6x5mm |
| 31 | Q1                                       | IRLZ44N     | TO-220-3_Vertical                              | 1   | $0.85   | https://free-electronic.com/product/irlz44n/ |
| 32 | J2                                       | LIGHT_SWITCH_CONNx2 | SolderWire-0.1sqmm_1x02_P3.6mm_D0.4mm_OD1mm | 1 | $0.047 | https://free-electronic.com/product/boat-rocker-switch-on-off-6a250vac-kcd1-101/ |
| 33 | J3                                       | ISP_CONNx6  | PinHeader_1x06_P2.54mm_Vertical                | 1   | $0.019  | https://free-electronic.com/product/pin-header-male-2-54mm-2x40-straight-2/ |
| 34 | J4                                       | LED_STRIP_12V_CONNx2 | SolderWire-0.1sqmm_1x02_P3.6mm_D0.4mm_OD1mm | 1 | $1.89 | https://lampatronics.com/product/neon-flexible-led-strip-light-1m-warm-12vdc |
| 35 | J5                                       | FTDI_CONNx6 | PinHeader_1x06_P2.54mm_Vertical                | 1   | $0.019  | https://free-electronic.com/product/pin-header-male-2-54mm-2x40-straight-2/ |
| 36 | J6                                       | SPEAKER_CONNx2 | SolderWire-0.1sqmm_1x02_P3.6mm_D0.4mm_OD1mm | 1 | $1.13 | https://lampatronics.com/product/speaker-8w-4-ohm-64x64x32mm-1pcs-t1rek |
| 37 | J7                                       | DS3231_CONNx4 | SolderWire-0.1sqmm_1x04_P3.6mm_D0.4mm_OD1mm | 1 | $1.70 | https://lampatronics.com/product/ds3231-high-precision-real-time-clock-rtc-module-without-battery |
| 38 | N/A                                      | PAM8403     | MODULE                                         | 1   | $0.61   | https://free-electronic.com/product/pam8403-5v-power-audio-amplifier-board/ |
