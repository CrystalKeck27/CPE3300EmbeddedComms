/**
 * Name: Crystal Keck
 * Course: CPE2610
 * Section: 121
 * Assignment: Lab 3 - Buttons
 * File: push_button.h
 * Dependencies: gpio.h
 */

#ifndef PUSH_BUTTON_H_
#define PUSH_BUTTON_H_

#include <stdbool.h>
#include "gpio.h"

typedef struct {
	volatile Gpio *gpio;
	uint8_t position;
	bool is_pull_up;
} PushButton;

/**
 * PushButtonInit()
 * Description:
 * 		Configures the button as an input with the proper pull resistor
 * Inputs: Gpio that the bar uses, position in the gpio port, is pull up or pull down, and if the pull resistor is implemented in the hardware.
 * Outputs: PushButton struct
 */
PushButton PushButtonInit(volatile Gpio *gpio, uint8_t position, bool is_pull_up, bool hardware_pull);

/**
 * IsPressed(uint16_t pattern)
 * Description:
 * 		Returns the state of the button
 * Inputs: the button to check
 * Outputs: True if button is pressed, false otherwise
 */
bool IsPressed(PushButton *button);

#endif /* PUSH_BUTTON_H_ */
