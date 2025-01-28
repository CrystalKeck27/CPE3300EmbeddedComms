/**
 * Name: Crystal Keck
 * Course: CPE2610
 * Section: 121
 * Assignment: Lab 5 - Keypad
 * File: delay.c
 * Dependencies: delay.h
 */

#include "delay.h"
#include <stdint.h>
#include "systick.h"

void BusyMicro(uint32_t N){
	for(int i = 0; i < N; i++);
}

void BusyMilli(uint32_t N){
	N *= ONE_MILLI;
	for(int i = 0; i < N; i++);
}

void BusySec(uint32_t N){
	N *= ONE_SEC;
	for(int i = 0; i < N; i++);
}

void DelaySysMicroSec(uint32_t N) {
	volatile SysTick *stk = STK;
	// Clock is 100MHz
	N *= 50;
	// 24-bit counter
	N &= 0x00FFFFFF;
	stk->ctrl = STK_ENABLE | STK_PROCESSOR_CLK;
	stk->load = N;
	stk->val = 0;
	while((stk->ctrl & STK_COUNTFLAG) == 0);
	stk->ctrl = 0;
}

void DelaySysMilliSec(uint32_t N) {
	volatile SysTick *stk = STK;
	// Clock is 100MHz
	N *= 3125;
	// 24-bit counter
	N &= 0x00FFFFFF;
	stk->ctrl = STK_ENABLE;
	stk->load = N;
	stk->val = 0;
	while((stk->ctrl & STK_COUNTFLAG) == 0);
	stk->ctrl = 0;
}

// Somehow, AHB/8 is not timed right
void DelaySysSec(uint32_t N) {
	volatile SysTick *stk = STK;
	// One second
	uint32_t one_sec = 1250000;
	stk->ctrl = STK_ENABLE;
	stk->load = one_sec;
	stk->val = 0;
	for(int i = 0; i < N; i++) {
		while((stk->ctrl & STK_COUNTFLAG) == 0);
		stk->val = 0;
	}
	stk->ctrl = 0;
}

static void (*callback_static)(void);

void DelayCallSysMicroSec(uint32_t N, void (*callback)(void)) {
	callback_static = callback;
	volatile SysTick *stk = STK;
	// Clock is 100MHz
	N *= 50;
	// 24-bit counter
	N &= 0x00FFFFFF;
	stk->ctrl = STK_ENABLE | STK_PROCESSOR_CLK | STK_TICKINT;
	stk->load = N;
	stk->val = 0;
}

void DelayCallSysMilliSec(uint32_t N, void (*callback)(void)) {
	callback_static = callback;
	volatile SysTick *stk = STK;
	// Clock is 100MHz
	N *= 3125;
	// 24-bit counter
	N &= 0x00FFFFFF;
	stk->ctrl = STK_ENABLE | STK_TICKINT;
	stk->load = N;
	stk->val = 0;
}

void SysTick_Handler(void) {
	volatile SysTick *stk = STK;
	stk->ctrl = 0;
	callback_static();
}
