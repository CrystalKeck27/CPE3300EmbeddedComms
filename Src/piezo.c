/**
 * Name: Crystal Keck
 * Course: CPE2610
 * Section: 121
 * Assignment: Lab 8 - Synth
 * File: piezo.c
 * Dependencies: piezo.h, tim.h, rcc.h, gpio.h
 */

#include <stdint.h>
#include <stdbool.h>
#include "tim.h"
#include "piezo.h"
#include "rcc.h"
#include "gpio.h"

#define PIEZO_TIM TIM3

void PiezoInit() {
    RccEnable(PIEZO_TIM);
    RccEnable(GPIO_B);
    // Set up GPIO B pin 4 as alternate function 2
    volatile Gpio *gpio = GPIO_B;
    uint32_t mode = gpio->moder;
    mode &= 0xFFFFFFCF;
    mode |= 0x00000020;
    gpio->moder = mode;
    uint32_t afr = gpio->afrl;
    afr &= 0xFFF0FFFF;
    afr |= 0x00020000;
    gpio->afrl = afr;
    uint32_t pupd = gpio->pupdr;
    pupd &= 0xFFFFFFCF;
    gpio->pupdr = pupd;
}

void PiezoSetPeriod(uint32_t period) {
    volatile Tim *tim = PIEZO_TIM;
    tim->psc = 3;
    tim->arr = period / 4;
    tim->ccr1 = period / 8; // 50% duty cycle, hopefully
    tim->ccmr1 = 0x0068;
    // By omitting the following line, the piezo on/off state will not be changed
    // It is unknown whether this is the desired behavior or not
    // tim->cr1 = 0x0081; // Enable the timer
    tim->ccer = 0x0001;
    tim->egr = 0x0001;
}

void PiezoOn() {
    volatile Tim *tim = PIEZO_TIM;
    tim->cr1 = 0x0081;
}

void PiezoOff() {
    volatile Tim *tim = PIEZO_TIM;
    tim->cr1 = 0x0000;
}

bool IsPiezoOn() {
    volatile Tim *tim = PIEZO_TIM;
    return (tim->cr1 & 0x0001) == 0x0001;
}