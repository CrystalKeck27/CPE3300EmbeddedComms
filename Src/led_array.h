/**
 * Name: Crystal Keck
 * Course: CPE2610
 * Section: 121
 * Assignment: Lab 10 - SPI
 * File: led_array.h
 * Dependencies: None
 */

#ifndef LED_ARRAY_H_
#define LED_ARRAY_H_

#include <stdint.h>

void LedArrayInit();

void LedArraySend(uint8_t* rows);

#endif /* LED_ARRAY_H_ */