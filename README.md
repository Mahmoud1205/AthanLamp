# Athan Lamp
Hardware, software, and devlog of the Athan Lamp project.

I recently moved to a home which is far away from a mosque, meaning I cannot hear the athan (Islamic call to prayer) anymore. I could've solve this by installing an athan application on my phone, but I can do better.

So I got the idea of making a device that's placed in my house and can play the athan during the athan time according to my location, this device also acts as a decoration in the shape of a mosque and a lamp. Below is a render I made in Blender to demonstrate the idea.
https://github.com/user-attachments/assets/cd089860-c122-4150-9e4c-b0a5cfae7b2e
*The timer in Arabic says: "time until athan"*

The electronic schematic, PCB design, firmware, and soon the physical design of the device, are all made by me.

I would like to keep this main README short, but check out the Hardware and Software folders for more info and implementation details. Both folders have READMEs.

![3D render of the PCB](/Media/pcb3d.png)
![Schematic of the PCB](/Media/schematic.png)
![PCB layout](/Media/pcb.png)

## Bill of Materials
|   # | References                         | Value                | Footprint                                          |   Quantity |
|----:|:-----------------------------------|:---------------------|:---------------------------------------------------|-----------:|
|   1 | C2, C6, C7, C9, C10, C11, C16, C19 | 100nF                | C_0805_2012Metric                                  |          8 |
|   2 | C1, C12                            | 100uF_25V            | CP_Radial_D5.0mm_P2.50mm                           |          2 |
|   3 | C13, C14                           | 47nF                 | C_0805_2012Metric                                  |          2 |
|   4 | C15, C17                           | 22pF                 | C_0805_2012Metric                                  |          2 |
|   5 | C3                                 | 10uF                 | CP_Radial_D4.0mm_P2.00mm                           |          1 |
|   6 | C4                                 | 22uF                 | CP_Radial_D6.3mm_P2.50mm                           |          1 |
|   7 | C5                                 | 220uF_16V            | CP_Radial_D8.0mm_P3.80mm                           |          1 |
|   8 | C8                                 | 100uF                | CP_Radial_D5.0mm_P2.50mm                           |          1 |
|   9 | C18                                | 330uF_25V            | CP_Radial_D8.0mm_P3.80mm                           |          1 |
|  10 | R1, R8, R9, R13                    | 1R                   | R_0805_2012Metric                                  |          4 |
|  11 | R3, R5, R12                        | 10K                  | R_0805_2012Metric                                  |          3 |
|  12 | R7, R14, R17                       | 220R                 | R_0805_2012Metric                                  |          3 |
|  13 | R4, R15                            | 1K                   | R_0805_2012Metric                                  |          2 |
|  14 | R10, R11                           | 4.7K                 | R_0805_2012Metric                                  |          2 |
|  15 | R2                                 | 100R                 | R_0603_1608Metric                                  |          1 |
|  16 | R6                                 | 39R                  | R_1206_3216Metric                                  |          1 |
|  17 | R16                                | 330R                 | R_0805_2012Metric                                  |          1 |
|  18 | L1                                 | 47uH                 | L_Taiyo-Yuden_MD-3030                              |          1 |
|  19 | D1, D2                             | SS34                 | D_SMA                                              |          2 |
|  20 | D3                                 | FW0_LED              | LED_0805_2012Metric_Pad1.15x1.40mm_HandSolder      |          1 |
|  21 | D4                                 | 12V_STAT             | LED_0805_2012Metric_Pad1.15x1.40mm_HandSolder      |          1 |
|  22 | D5                                 | 5V_STAT              | LED_0805_2012Metric_Pad1.15x1.40mm_HandSolder      |          1 |
|  23 | D6                                 | 3V3_STAT             | LED_0805_2012Metric_Pad1.15x1.40mm_HandSolder      |          1 |
|  24 | U1                                 | AMS1117-3.3          | SOT-223-3_TabPin2                                  |          1 |
|  25 | U2                                 | LM2596T-5            | TO-220-5_P3.4x3.7mm_StaggerOdd_Lead3.8mm_Vertical  |          1 |
|  26 | U3                                 | W25Q32JVSS           | SOIC-8_5.3x5.3mm_P1.27mm                           |          1 |
|  27 | U4                                 | TDA2003              | TO-220-5_P3.4x3.7mm_StaggerEven_Lead3.8mm_Vertical |          1 |
|  28 | U5                                 | ATmega328P-A         | TQFP-32_7x7mm_P0.8mm                               |          1 |
|  29 | Y1                                 | 16MHz                | Crystal_HC49-4H_Vertical                           |          1 |
|  30 | SW1                                | RST_BTN              | SW_PUSH_6mm_H4.3mm                                 |          1 |
|  31 | Q1                                 | IRLZ44N              | TO-220-3_Vertical                                  |          1 |
|  32 | J1                                 | 12V_CONNx2           | SolderWire-0.1sqmm_1x02_P3.6mm_D0.4mm_OD1mm        |          1 |
|  33 | J2                                 | LIGHT_SWITCH_CONNx2  | SolderWire-0.1sqmm_1x02_P3.6mm_D0.4mm_OD1mm        |          1 |
|  34 | J3                                 | ISP_CONNx6           | PinHeader_1x06_P2.54mm_Vertical                    |          1 |
|  35 | J4                                 | LED_STRIP_12V_CONNx2 | SolderWire-0.1sqmm_1x02_P3.6mm_D0.4mm_OD1mm        |          1 |
|  36 | J5                                 | FTDI_CONNx6          | PinHeader_1x06_P2.54mm_Vertical                    |          1 |
|  37 | J6                                 | SPEAKER_CONNx2       | SolderWire-0.1sqmm_1x02_P3.6mm_D0.4mm_OD1mm        |          1 |
|  38 | J7                                 | DS3231_CONNx4        | SolderWire-0.1sqmm_1x04_P3.6mm_D0.4mm_OD1mm        |          1 |
