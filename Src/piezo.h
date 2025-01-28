/**
 * Name: Crystal Keck
 * Course: CPE2610
 * Section: 121
 * Assignment: Lab 8 - 
 * File: piezo.h
 * Dependencies: tim.h
 */

#ifndef PIEZO_H_
#define PIEZO_H_

#include <stdint.h>
#include <stdbool.h>
#include "tim.h"

#define PIEZO_TIM TIM3

void PiezoInit();

/**
 * @brief Sets the period for the piezo buzzer.
 *
 * This function sets the period for the piezo buzzer. The period is specified
 * in terms of the number of timer ticks. The actual time period will depend on
 * the clock frequency of the timer.
 *
 * @param period The period for the piezo buzzer, in timer ticks.
 */
void PiezoSetPeriod(uint32_t period);

/**
 * @brief Turns on the piezo buzzer.
 *
 * This function turns on the piezo buzzer.
 * with the period that was previously set using the PiezoSetPeriod function.
 */
void PiezoOn();

/**
 * @brief Turns off the piezo buzzer.
 *
 * This function turns off the piezo buzzer.
 */
void PiezoOff();

/**
 * @brief Checks if the piezo buzzer is on.
 *
 * This function checks if the piezo buzzer is on.
 *
 * @return true if the piezo buzzer is on, false otherwise.
 */
bool IsPiezoOn();

#endif /* PIEZO_H_ */