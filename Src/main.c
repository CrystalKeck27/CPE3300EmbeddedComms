/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stdio.h>
#include "led_bar.h"
#include "gpio.h"
#include "lcd.h"
#include "delay.h"
#include "rcc.h"
#include "tim.h"
#include "interrupt.h"
#include "uart_driver.h"

volatile uint16_t count = 0;
volatile bool trigger = false;
volatile bool bad = false;
LedBar led_bar;

// Control pin = PB0
// Timer 3 Channel 3 = AF2
int main(void)
{
	init_usart2(57600, 16000000);
	LcdSetup();
	LcdInit();
	LcdClear();
	led_bar = LedBarInit();
	LedBarOn(&led_bar, 0);
	volatile Gpio *gpio_b = GPIO_B;
	volatile Tim *tim3 = TIM3;
	RccEnable(tim3);
	// Enable pull-up resistor
	gpio_b->pupdr &= ~(3 << 0);
	gpio_b->pupdr |= (1 << 0);
	// Set PB0 to alternate function mode
	gpio_b->moder &= ~(3 << 0);
	gpio_b->moder |= (2 << 0);
	// Set PB0 to AF2
	gpio_b->afrl &= ~(0xF << 0);
	gpio_b->afrl |= (2 << 0);

	// Set up timer 3
	tim3->psc = 803;
	tim3->arr = 0xFFFFFFFF;
	tim3->dier |= 0x00000008;
	tim3->ccmr2 = 0x0000000000000001;
	tim3->ccer = 0x1B00;
	tim3->cr1 |= 0x00000081;
	EnableInterrupt(29);
	tim3->egr |= 0x0001;
	printf("Count: %d\n", count);

	while (1)
	{
		// count = tim3->cnt;
		char buffer[16];
		sprintf(buffer, "Count: %d", count);
		LcdClear();
		LcdHome();
		LcdWriteStr(buffer);
		while (!trigger)
			;
		trigger = false;
		printf("X: %d\n", count);
		if (bad)
		{
			LedBarOff(&led_bar, 0);
			LedBarOn(&led_bar, 1);
			bad = false;
		}
	}
}

void TIM3_IRQHandler(void)
{
	volatile Tim *tim3 = TIM3;

	uint32_t sr = tim3->sr;
	if (sr & 0x08)
	{
		uint16_t value = tim3->ccr3;
		value += 11100;
		tim3->ccr4 = value;
		tim3->dier |= 0x00000010;
		LedBarToggle(&led_bar, 5);
	}
	else if (sr & 0x10)
	{
		bad = true;
		LedBarToggle(&led_bar, 7);
		tim3->dier &= ~(1 << 4);
		tim3->sr &= ~(1 << 4);
	}
	trigger = true;
}
