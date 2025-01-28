/**
 * Name: Crystal Keck
 * Course: CPE2610
 * Section: 121
 * Assignment: Lab 5 - Keypad
 * File: systick.h
 * Dependencies: None
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include <stdint.h>

#define STK (volatile SysTick*)0xE000E010

#define STK_ENABLE 0b001
#define STK_DISABLE ~(STK_ENABLE)
#define STK_TICKINT 0b010

#define STK_PROCESSOR_CLK 0b100
#define STK_PROCESSOR_CLK_DIV_8 ~(STK_PROCESSOR_CLK)
#define STK_COUNTFLAG 0x10000

typedef struct SysTick {
	uint32_t ctrl;
    uint32_t load;
    uint32_t val;
    uint32_t calib;
} SysTick;

#endif /* SYSTICK_H_ */
