/**
 * Name: Crystal Keck
 * Course: CPE2610
 * Section: 121
 * Assignment: Lab 6 - Timers
 * File: tim.h
 * Dependencies: None
 */

#ifndef TIM_H_
#define TIM_H_

#include <stdint.h>

#define TIM1 (volatile Tim *) 0x40012C00
#define TIM2 (volatile Tim *) 0x40000000
#define TIM3 (volatile Tim *) 0x40000400
#define TIM4 (volatile Tim *) 0x40000800
#define TIM5 (volatile Tim *) 0x40000C00
#define TIM9 (volatile Tim *) 0x40014000
#define TIM10 (volatile Tim *) 0x40014400
#define TIM11 (volatile Tim *) 0x40014800

#define TIM1_FREQ_HZ 16000000
#define TIM2_FREQ_HZ 16000000
#define TIM3_FREQ_HZ 16000000
#define TIM4_FREQ_HZ 16000000
#define TIM5_FREQ_HZ 16000000
#define TIM9_FREQ_HZ 16000000
#define TIM10_FREQ_HZ 16000000
#define TIM11_FREQ_HZ 16000000

#define CEN 0

typedef struct Tim {
    uint32_t cr1;
    uint32_t cr2;
    uint32_t smcr;
    uint32_t dier;
    uint32_t sr;
    uint32_t egr;
    uint32_t ccmr1;
    uint32_t ccmr2;
    uint32_t ccer;
    uint32_t cnt;
    uint32_t psc;
    uint32_t arr;
    uint32_t rcr;
    uint32_t ccr1;
    uint32_t ccr2;
    uint32_t ccr3;
    uint32_t ccr4;
    uint32_t bdtr;
    uint32_t dcr;
    uint32_t dmar;
} Tim;

#endif /* TIM_H_ */