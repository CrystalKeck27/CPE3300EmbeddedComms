/**
 * Name: Crystal Keck
 * Course: CPE2610
 * Section: 121
 * Assignment: Lab 12 - Choice
 * File: encoder.h
 * Dependencies: tim.h
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include <stdint.h>
#include <stdbool.h>
#include "tim.h"

#define ENCODER_TIM TIM2

void EncoderInit();

uint16_t EncoderRead();

#endif /* ENCODER_H_ */