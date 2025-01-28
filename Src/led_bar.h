/**
 * Name: Crystal Keck
 * Course: CPE2610
 * Section: 121
 * Assignment: Lab 2 - MatchMe!
 * File: led_bar.h
 * Dependencies: None
 */

#ifndef LED_BAR_H_
#define LED_BAR_H_

#include <stdint.h>
#include "gpio.h"

typedef struct LedBar {
	volatile Gpio *gpio;
	uint8_t led_count;
	uint8_t const *led_positions;
} LedBar;

/**
 * LedBarInit()
 * Description:
 * 		Configures PB5-PB10,PB12-PB15 as outputs
 * Inputs: Gpio that the bar uses
 * Outputs: LedBar struct
 */
LedBar LedBarInit();

/**
 * LedBarOn()
 * Description:
 * 		Turns on the LED at the index
 * Inputs:
 * 		index - 0-9 the index of light to be activated
 * 			Note: out of range args should have no effect.
 * Outputs: none
 *
 */
void LedBarOn(LedBar *led_bar, uint8_t index);

/**
 * LedBarOff()
 * Description:
 * 		Turns off the LED at the index
 * Inputs:
 * 		index - 0-9 the index of light to be activated
 * 			Note: out of range args should have no effect.
 * Outputs: none
 *
 */
void LedBarOff(LedBar *led_bar, uint8_t index);

/**
 * LedBarToggle()
 * Description:
 * 		Reverses the state the LED at the index
 * Inputs:
 * 		index - 0-9 the index of light to be activated
 * 			Note: out of range args should have no effect.
 * Outputs: none
 *
 */
void LedBarToggle(LedBar *led_bar, uint8_t index);

/**
 * LedBarWrite()
 * Description:
 * 		Write a pattern to the Led Bar
 * 	Inputs:
 * 		pattern - the 10 least significant bit are
 * 						written to the lights
 * 	Outputs: None
 */
void LedBarWrite(LedBar *led_bar, uint16_t pattern);

/**
 * LedBarRead()
 * Description:
 * 		Read the current from Led Bar
 * 	Inputs: None
 * 	Return: pattern - the 10 least significant bits are
 * 						current light patterns
 */
uint16_t LedBarRead(LedBar *led_bar);

/**
 * LedBarRollRight()
 * Description:
 * 		Shift the pattern one bit the right.
 * 		LSBit becomes MSBit.
 * 	Inputs: None
 * 	Outputs: None
 */
void LedBarRollRight(LedBar *led_bar);

/**
 * LedBarRollRight()
 * Description:
 * 		Shift the pattern one bit the left.
 * 		MSBit becomes LSBit.
 * 	Inputs: None
 * 	Outputs: None
 */
void LedBarRollLeft(LedBar *led_bar);




#endif /* LED_BAR_H_ */
