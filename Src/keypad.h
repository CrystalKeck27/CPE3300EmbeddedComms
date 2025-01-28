/**
 * Name: Crystal Keck
 * Course: CPE2610
 * Section: 121
 * Assignment: Lab 5 - Keypad
 * File: keypad.h
 * Dependencies: None
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <stdint.h>

void KeypadInit();

uint8_t KeypadStatus();

// uint8_t KeypadGetKey();

void KeypadInit();

void KeypadWatch();

void KeypadIgnore();

#endif /* KEYPAD_H_ */
