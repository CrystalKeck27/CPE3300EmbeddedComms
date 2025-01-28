/**
 * Name: Crystal Keck
 * Course: CPE2610
 * Section: 121
 * Assignment: Lab 5 - Keypad
 * File: rcc.c
 * Dependencies: None
 */

#include <stdint.h>
#include <stdbool.h>
#include "rcc.h"

static uint32_t calculateOffset(uint32_t location) {
    switch (location >> 16) {
        // APB1
        case 0x4000:
            return 0x10;
        // APB2
        case 0x4001:
            return 0x14;
        // AHB1
        case 0x4002:
            return 0x0;
        // AHB2
        case 0x5000:
            return 0x4;
    }
    return 0;
}

/**
 * @brief Resets the peripheral
 * 
 * Convenience function for ModifyRcc.
 * Equivalent to:
 * ```
 * ModifyRcc(peripheral, RCC_RSTR, true)
 * ```
*/
void RccReset(volatile void* peripheral) {
    ModifyRcc(peripheral, RCC_RSTR, true);
}

/**
 * @brief Unresets the peripheral
 * 
 * Convenience function for ModifyRcc.
 * Equivalent to:
 * ```
 * ModifyRcc(peripheral, RCC_RSTR, false)
 * ```
*/
void RccUnreset(volatile void* peripheral) {
    ModifyRcc(peripheral, RCC_RSTR, false);
}

/**
 * @brief Enables the peripheral
 * 
 * Convenience function for ModifyRcc.
 * Equivalent to:
 * ```
 * ModifyRcc(peripheral, RCC_ENR, true)
 * ```
*/
void RccEnable(volatile void* peripheral) {
    ModifyRcc(peripheral, RCC_ENR, true);
}

/**
 * @brief Disables the peripheral
 * 
 * Convenience function for ModifyRcc.
 * Equivalent to:
 * ```
 * ModifyRcc(peripheral, RCC_ENR, false)
 * ```
*/
void RccDisable(volatile void* peripheral) {
    ModifyRcc(peripheral, RCC_ENR, false);
}

/**
 * @brief Enables the peripheral in low power mode
 * 
 * Convenience function for ModifyRcc.
 * Equivalent to:
 * ```
 * ModifyRcc(peripheral, RCC_LPENR, true)
 * ```
*/
void RccLowPowerEnable(volatile void* peripheral) {
    ModifyRcc(peripheral, RCC_LPENR, true);
}

/**
 * @brief Disables the peripheral in low power mode
 * 
 * Convenience function for ModifyRcc.
 * Equivalent to:
 * ```
 * ModifyRcc(peripheral, RCC_LPENR, false)
 * ```
*/
void RccLowPowerDisable(volatile void* peripheral) {
    ModifyRcc(peripheral, RCC_LPENR, false);
}

/**
 * @brief Modifies the RCC register
 * 
 * @param ptr The peripheral's base address
 * @param action The action to perform. From `(RCC_RSTR, RCC_ENR, RCC_LPENR)`
 * @param state The state to set
 */
void ModifyRcc(volatile void* peripheral, uint32_t action, bool state) {
    uint32_t peripheral_as_int = (uint32_t) peripheral;
    uint32_t offset = calculateOffset(peripheral_as_int);
    uint32_t bit = (peripheral_as_int & 0xffff) >> 10;
    volatile uint32_t* rccr = (volatile uint32_t*)(action + offset);
    
    uint32_t rcc = *rccr;
    if (state) {
        rcc |= (1 << bit);
    } else {
        rcc &= ~(1 << bit);
    }
    *rccr = rcc;
}