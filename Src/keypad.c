/**
 * Name: Crystal Keck
 * Course: CPE2610
 * Section: 121
 * Assignment: Lab 5 - Keypad
 * File: keypad.c
 * Dependencies: None
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "gpio.h"
#include "rcc.h"
#include "delay.h"
#include <stdint.h>

/*
Interrupt-driven keypad implementation
Set all cols high
Wait for a row to go high with interrupt
Disable interrupt for 5ms (debounce)
Switch inputs and outputs
Set specific row high
Read cols
Return key

*/

// pins 0-3 are columns, 4-7 are rows

void KeypadInit() {
    volatile Gpio *gpio = GPIO_C;
    RccEnable(gpio);
    uint32_t mode = gpio->moder;
    // clear bits 0-7
    mode &= 0xFFFF0000;
    // set cols as outputs
    mode |= 0x00000055;
    gpio->moder = mode;
    uint32_t pupd = gpio->pupdr;
    // clear bits 0-7
    pupd &= 0xFFFF0000;
    // set rows as pull-down
    pupd |= 0x0000AA00;
    gpio->pupdr = pupd;
}

uint8_t KeypadStatus() {
    volatile Gpio *gpio = GPIO_C;
    for (uint8_t i = 0; i < 4; i++) {
        uint16_t od = gpio->odr;
        // No clobbering
        od &= 0xFFF0;
        od |= 1 << i;
        gpio->odr = od;
        for (uint8_t t = 255; t > 0; t--);
        uint16_t idr = gpio->idr;
        for (uint8_t j = 0; j < 4; j++) {
            if (idr & (1 << (j + 4))) {
                return (j * 4) + i + 1;
            }
        }
    }
    return 0;
}

// uint8_t KeypadGetKey() {
//     uint8_t key = 0;
//     while (key == 0) {
//         key = KeypadStatus();
//     }
//     return key;
// }

#endif /* KEYPAD_H_ */
