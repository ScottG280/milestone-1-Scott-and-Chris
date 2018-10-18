# Milestone 1: Scott and Chris
- Created by Scott Gordon and Chris Amling
- Created: October 10, 2018
- Last Updated: October 18, 2018 (code documentation and correct functionality implemented)
- Using MSP430F5529 Microprocessor
## Purpose
The purpose of this program is to allow the user to control an RGB LED by sending bytes to control the brightness of the Red, Green, and Blue LEDs. This functionality is possible with the use of the MSP430F5529 microprocessor.
## Use
To be able to control the RGB LED, the microprocessor has to receive values from UART. The first bit is the number of bytes being sent to the processor, while the following bytes are used to control the Red, Green, and Blue values. When new bytes are sent in, the old bytes get passed on to the next device.
## Implementation
The brightness of each LED can be controlled using PWM. Since the values being taken in to set RGB can be varied, the values change the brightness of the LED. The byte ranges from 0x00 to 0xFF with 0x00 being off and 0xFF setting the LEDs to their maximum brightness.
### Board Setup
The RGB LED is connected to Pins 1.2, 1.3, and 1.4 respectively through 1K resistors. The anode of the LED is connected to the 3.3V rail on the microprocessor.
