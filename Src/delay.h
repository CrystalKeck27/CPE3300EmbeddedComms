/**
 * Name: Crystal Keck
 * Course: CPE2610
 * Section: 121
 * Assignment: Lab 5 - Keypad
 * File: delay.h
 * Dependencies: None
 */

#ifndef DELAY_H_
#define DELAY_H_

#include <stdint.h>
#include "systick.h"

#define ONE_MILLI 1450
#define ONE_SEC 1450000

void BusyMicro(uint32_t N);

void BusyMilli(uint32_t N);

void BusySec(uint32_t N);

void DelaySysMicroSec(uint32_t N);

void DelaySysMilliSec(uint32_t N);

void DelaySysSec(uint32_t N);

void DelayCallSysMicroSec(uint32_t N, void (*callback)(void));

void DelayCallSysMilliSec(uint32_t N, void (*callback)(void));

#endif /* DELAY_H_ */
