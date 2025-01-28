/**
 * Name: Crystal Keck
 * Course: CPE2610
 * Section: 121
 * Assignment: Lab 2 - MatchMe!
 * File: led_bar.c
 * Dependencies: led_bar.h
 */

#include "led_bar.h"
#include "gpio.h"
#include "rcc.h"

static const uint8_t led_positions[10] = {5, 6, 7, 8, 9, 10, 12, 13, 14, 15};

LedBar LedBarInit() {
	// Enable GPIO B
	volatile Gpio *gpio_b = GPIO_B;
	RccEnable(gpio_b);
	// Create Gpio struct
	LedBar led_bar = {
		.gpio = gpio_b,
		.led_count = 10,
		.led_positions = led_positions,
	};

	// Configure pins
	uint8_t count = led_bar.led_count;
	const uint8_t *leds = led_bar.led_positions;
	uint32_t clear_mask = 0;
	uint32_t set_mask = 0;
	for (uint8_t i = 0; i < count; i++) {
		uint8_t moder_location = leds[i] * 2;
		clear_mask |= 3 << moder_location;
		set_mask |= MODER_OUTPUT << moder_location;
	}
	uint32_t mode = gpio_b->moder;
	mode &= ~clear_mask;
	mode |= set_mask;
	gpio_b->moder = mode;
	return led_bar;
}

void LedBarOn(LedBar *led_bar, uint8_t index) {
	const uint8_t *leds = led_bar->led_positions;
	uint8_t offset = leds[index];
	led_bar->gpio->bsrr = 1 << offset;
}

void LedBarOff(LedBar *led_bar, uint8_t index) {
	const uint8_t *leds = led_bar->led_positions;
	uint8_t offset = leds[index] + 16;
	led_bar->gpio->bsrr = 1 << offset;
}

void LedBarToggle(LedBar *led_bar, uint8_t index) {
	const uint8_t *leds = led_bar->led_positions;
	uint8_t offset = leds[index];
	led_bar->gpio->odr ^= 1 << offset;
}

void LedBarWrite(LedBar *led_bar, uint16_t pattern) {
	uint32_t set = 0;
	const uint8_t *leds = led_bar->led_positions;
	for (uint8_t i = 0; i < led_bar->led_count; i++) {
		uint16_t isolate = 1 << i;
		uint8_t led = leds[i];
		if ((pattern & isolate) > 0) {
			set |= 1 << led;
		} else {
			set |= 1 << (led + 16);
		}
	}
	led_bar->gpio->bsrr = set;
}

uint16_t LedBarRead(LedBar *led_bar) {
	uint16_t raw = led_bar->gpio->odr;
	const uint8_t *leds = led_bar->led_positions;
	uint16_t pattern = 0;
	for (uint8_t i = 0; i < led_bar->led_count; i++) {
		uint8_t led = leds[i];
		uint16_t isolate = 1 << led;
		if ((raw & isolate) > 0) {
			pattern |= 1 << i;
		}
	}
	return pattern;
}

void LedBarRollRight(LedBar *led_bar) {
	uint8_t count = led_bar->led_count;
	const uint8_t *leds = led_bar->led_positions;
	uint16_t raw = led_bar->gpio->odr;
	uint32_t set = 0;
	uint8_t last_led = leds[count - 1];
	for (uint8_t i = 0; i < count; i++) {
		uint8_t led = leds[i];
		uint16_t isolate = 1 << led;
		if ((raw & isolate) > 0) {
			// Set
			set |= 1 << last_led;
		} else {
			// Reset
			set |= 1 << (last_led + 16);
		}
		last_led = led;
	}
	led_bar->gpio->bsrr = set;
}

void LedBarRollLeft(LedBar *led_bar) {
	uint8_t count = led_bar->led_count;
	const uint8_t *leds = led_bar->led_positions;
	uint16_t raw = led_bar->gpio->odr;
	uint32_t set = 0;
	uint8_t last_led = leds[0];
	for (int8_t i = count - 1; i >= 0; i--) {
		uint8_t led = leds[i];
		uint16_t isolate = 1 << led;
		if ((raw & isolate) > 0) {
			// Set
			set |= 1 << last_led;
		} else {
			// Reset
			set |= 1 << (last_led + 16);
		}
		last_led = led;
	}
	led_bar->gpio->bsrr = set;
}


