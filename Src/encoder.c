/**
 * Name: Crystal Keck
 * Course: CPE2610
 * Section: 121
 * Assignment: Lab 12 - Choice
 * File: encoder.h
 * Dependencies: tim.h
 */

#include <stdint.h>
#include <stdbool.h>
#include "rcc.h"
#include "gpio.h"
#include "tim.h"
#include "encoder.h"

static volatile Gpio *gpio_a = GPIO_A;
static volatile Tim *tim = ENCODER_TIM;

void EncoderInit() {
    RccEnable(gpio_a);
    RccEnable(tim);
    // GPIO A
    uint32_t pupd = gpio_a->pupdr;
    pupd &= ~(0b11);
    pupd |= PUPDR_PULL_UP | (PUPDR_PULL_UP << 2);
    gpio_a->pupdr = pupd;

    uint32_t mode = gpio_a->moder;
    mode &= ~(0b1111);
    mode |= MODER_ALTERNATE | (MODER_ALTERNATE << 2);
    gpio_a->moder = mode;

    uint32_t af = gpio_a->afrl;
    af &= ~(0xFF);
    af |= 0b0001 | (0b0001 << 4);
    gpio_a->afrl = af;

    // TIM2
    tim->ccmr1 |= 0b01 << 8 | 0b01;
    tim->smcr |= 0b011;
    tim->ccer |= 0b1 << 0 | 0b1 << 4;
    tim->cr1 |= 1 << CEN;
}

uint16_t EncoderRead() {
    return tim->cnt;
}