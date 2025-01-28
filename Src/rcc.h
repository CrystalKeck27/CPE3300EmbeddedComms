/**
 * Name: Crystal Keck
 * Course: CPE2610
 * Section: 121
 * Assignment: Lab 5 - Keypad
 * File: rcc.h
 * Dependencies: None
 */

#ifndef RCC_H_
#define RCC_H_

#define RCC_RSTR 0x40023810
#define RCC_ENR 0x40023830
#define RCC_LPENR 0x40023850

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Resets the peripheral
 * 
 * Convenience function for ModifyRcc.
 * Equivalent to:
 * ```
 * ModifyRcc(peripheral, RCC_RSTR, true)
 * ```
 */
void RccReset(volatile void* peripheral);

/**
 * @brief Unresets the peripheral
 * 
 * Convenience function for ModifyRcc.
 * Equivalent to:
 * ```
 * ModifyRcc(peripheral, RCC_RSTR, false)
 * ```
 */
void RccUnreset(volatile void* peripheral);

/**
 * @brief Enables the peripheral
 * 
 * Convenience function for ModifyRcc.
 * Equivalent to:
 * ```
 * ModifyRcc(peripheral, RCC_ENR, true)
 * ```
 */
void RccEnable(volatile void* peripheral);

/**
 * @brief Disables the peripheral
 * 
 * Convenience function for ModifyRcc.
 * Equivalent to:
 * ```
 * ModifyRcc(peripheral, RCC_ENR, false)
 * ```
 */
void RccDisable(volatile void* peripheral);

/**
 * @brief Enables the peripheral in low power mode
 * 
 * Convenience function for ModifyRcc.
 * Equivalent to:
 * ```
 * ModifyRcc(peripheral, RCC_LPENR, true)
 * ```
 */
void RccLowPowerEnable(volatile void* peripheral);

/**
 * @brief Disables the peripheral in low power mode
 * 
 * Convenience function for ModifyRcc.
 * Equivalent to:
 * ```
 * ModifyRcc(peripheral, RCC_LPENR, false)
 * ```
 */
void RccLowPowerDisable(volatile void* peripheral);

/**
 * @brief Modifies the RCC register
 * 
 * @param ptr The peripheral's base address
 * @param action The action to perform. From `(RCC_RSTR, RCC_ENR, RCC_LPENR)`
 * @param state The state to set
 */
void ModifyRcc(volatile void* peripheral, uint32_t action, bool state);

#endif /* RCC_H_ */