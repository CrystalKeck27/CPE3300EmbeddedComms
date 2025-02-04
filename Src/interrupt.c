/**
 * Name: Crystal Keck
 * Course: CPE2610
 * Section: 121
 * Assignment: Lab 8 - Synth
 * File: interrupt.c
 * Dependencies: interrupt.h
 */

#include <stdint.h>
#include <stdbool.h>
#include "interrupt.h"
#include "gpio.h"

static volatile Exti *exti = EXTI;
static volatile Syscfg *syscfg = SYSCFG;
static volatile uint32_t *nvic_iser = (uint32_t*)0xE000E100;

/**
 * Enable the interrupt for the given IRQ
 */
void EnableInterrupt(uint32_t irq) {
    nvic_iser[irq / 32] |= 1 << (irq % 32);
}

/**
 * Disable the interrupt for the given IRQ
 */
void DisableInterrupt(uint32_t irq) {
    nvic_iser[irq / 32] &= ~(1 << (irq % 32));
}

/**
 * Configure the interrupt for the given GPIO pin
 * Does not configure the pin itself
 * @param gpio GPIO port
 * @param pin GPIO pin
 * @param rising Rising edge trigger
 * @param falling Falling edge trigger
 */
void ConfigureExternalInterrupt(volatile Gpio *gpio, uint32_t pin, bool rising, bool falling)
{
    // Set the interrupt mask
    exti->imr |= 1 << pin;

    // Set the interrupt trigger
    if (rising)
    {
        exti->rtsr |= 1 << pin;
    }
    else
    {
        exti->rtsr &= ~(1 << pin);
    }
    if (falling)
    {
        exti->ftsr |= 1 << pin;
    }
    else
    {
        exti->ftsr &= ~(1 << pin);
    }

    // Map gpio registeer to a number 0-7
    uint32_t gpio_number = (uint32_t)gpio;
    gpio_number -= 0x40020000;
    gpio_number /= 0x400;

    // Set the SYSCFG_EXTICR register
    syscfg->exticr[pin / 4] &= ~(0xF << (4 * (pin % 4)));
    syscfg->exticr[pin / 4] |= gpio_number << (4 * (pin % 4));
}

void ConfigureSyscfg(volatile Gpio *gpio, uint32_t pin) {
    // Map gpio registeer to a number 0-7
    uint32_t gpio_number = (uint32_t)gpio;
    gpio_number -= 0x40020000;
    gpio_number /= 0x400;

    // Set the SYSCFG_EXTICR register
    syscfg->exticr[pin / 4] &= ~(0xF << (4 * (pin % 4)));
    syscfg->exticr[pin / 4] |= gpio_number << (4 * (pin % 4));
}

void ConfigureEdges(uint32_t pin, bool rising, bool falling) {
    // Set the interrupt trigger
    if (rising)
    {
        exti->rtsr |= 1 << pin;
    }
    else
    {
        exti->rtsr &= ~(1 << pin);
    }
    if (falling)
    {
        exti->ftsr |= 1 << pin;
    }
    else
    {
        exti->ftsr &= ~(1 << pin);
    }
}

void EnableExti(uint32_t pin) {
    exti->imr |= 1 << pin;
}

void DisableExti(uint32_t pin) {
    exti->imr &= ~(1 << pin);
}