/**
 * Name: Crystal Keck
 * Course: CPE2610
 * Section: 121
 * Assignment: Lab 3 - Buttons
 * File: push_button.c
 * Dependencies: gpio.h, push_button.h
 */

#include <stdbool.h>
#include "gpio.h"
#include "rcc.h"
#include "push_button.h"

PushButton PushButtonInit(volatile Gpio *gpio, uint8_t position, bool is_pull_up, bool hardware_pull) {
	RccEnable(gpio);
	uint32_t mode = gpio->moder;
	uint32_t clear_mask = 3 << (position * 2);
	uint32_t set_mask = MODER_INPUT << (position * 2);
	mode &= ~clear_mask;
	mode |= set_mask;
	gpio->moder = mode;

	uint32_t pupd = gpio->pupdr;
	clear_mask = 3 << (position * 2);
	if (hardware_pull) {
		set_mask = PUPDR_NONE;
	} else if (is_pull_up) {
		set_mask = PUPDR_PULL_UP;
	} else {
		set_mask = PUPDR_PULL_DOWN;
	}
	set_mask <<= (position * 2);
	pupd &= ~clear_mask;
	pupd |= set_mask;
	gpio->pupdr = pupd;

	PushButton button = {
		.gpio = gpio,
		.position = position,
		.is_pull_up = is_pull_up,
	};
	return button;
}

bool IsPressed(PushButton *button) {
	uint16_t input = button->gpio->idr;
	uint16_t state = input & (1 << button->position);
	return button->is_pull_up ^ (bool) state;
}
