/**
 * Name: Crystal Keck
 * Course: CPE2610
 * Section: 121
 * Assignment: Lab 8 - Synth
 * File: interrupt.h
 * Dependencies: None
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "gpio.h"

#include <stdint.h>
#include <stdbool.h>

#define EXTI (volatile Exti *)0x40013C00
#define SYSCFG (volatile Syscfg *)0x40013800

#define NVIC_ISER (volatile uint32_t *)0xE000E100
#define NVIC_ICER (volatile uint32_t *)0xE000E180
#define NVIC_ISPR (volatile uint32_t *)0xE000E200
#define NVIC_ICPR (volatile uint32_t *)0xE000E280
#define NVIC_IPR (volatile uint32_t *)0xE000E400
#define NVIC_STIR (volatile uint32_t *)0xE000EF00

typedef struct Exti
{
    uint32_t imr;
    uint32_t emr;
    uint32_t rtsr;
    uint32_t ftsr;
    uint32_t swier;
    uint32_t pr;
} Exti;

typedef struct Syscfg
{
    uint32_t memrmp;
    uint32_t pmc;
    uint32_t exticr[4];
    uint32_t cmpcr;
    uint32_t reserved[2];
    uint32_t cfgr;
} Syscfg;

/**
 * Enable the interrupt for the given IRQ
 */
void EnableInterrupt(uint32_t irq);

/**
 * Disable the interrupt for the given IRQ
 */
void DisableInterrupt(uint32_t irq);

/**
 * Configure the interrupt for the given GPIO pin
 * Does not configure the pin itself
 * @param gpio GPIO port
 * @param pin GPIO pin
 * @param rising Rising edge trigger
 * @param falling Falling edge trigger
 */
void ConfigureExternalInterrupt(volatile Gpio *gpio, uint32_t pin, bool rising, bool falling);

/**
 * Configure which gpio port controlls the EXTI for line <pin>
 * @param gpio GPIO port
 * @param pin GPIO pin
 */
void ConfigureSyscfg(volatile Gpio *gpio, uint32_t pin);

/**
 * Configure the edge detection for the given pin
 * @param pin GPIO pin
 * @param rising Rising edge trigger
 * @param falling Falling edge trigger
 */
void ConfigureEdges(uint32_t pin, bool rising, bool falling);

/**
 * Enable EXTI imr for the given pin
 * @param pin GPIO pin
 */
void EnableExti(uint32_t pin);

/**
 * Disable EXTI imr for the given pin
 * @param pin GPIO pin
 */
void DisableExti(uint32_t pin);

#endif /* INTERRUPT_H_ */
