/**
 * Name: Crystal Keck
 * Course: CPE2610
 * Section: 121
 * Assignment: Lab 3 - Buttons
 * File: gpio.h
 * Dependencies: None
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>

#define GPIO_A (volatile Gpio*)0x40020000
#define GPIO_B (volatile Gpio*)0x40020400
#define GPIO_C (volatile Gpio*)0x40020800
#define GPIO_D (volatile Gpio*)0x40020C00
#define GPIO_E (volatile Gpio*)0x40021000
#define GPIO_H (volatile Gpio*)0x40021C00

#define MODER_INPUT 0b00
#define MODER_OUTPUT 0b01
#define MODER_ALTERNATE 0b10
#define MODER_ANALOG 0b11

#define PUPDR_NONE 0b00
#define PUPDR_PULL_UP 0b01
#define PUPDR_PULL_DOWN 0b10

typedef struct Gpio {
	uint32_t moder;
	uint16_t otyper;
	uint16_t reserved_0_do_not_use;
	uint32_t ospeedr;
	uint32_t pupdr;
	uint16_t idr;
	uint16_t reserved_1_do_not_use;
	uint16_t odr;
	uint16_t reserved_2_do_not_use;
	uint32_t bsrr;
	uint32_t lckr;
	uint32_t afrl;
	uint32_t afrh;
} Gpio;

#endif /* GPIO_H_ */
