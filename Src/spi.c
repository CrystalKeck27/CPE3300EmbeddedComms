/**
 * Name: Crystal Keck
 * Course: CPE2610
 * Section: 121
 * Assignment: Lab 10 - SPI
 * File: spi.c
 * Dependencies: spi.h, rcc.h
 */

#include <stdint.h>
#include "spi.h"
#include "rcc.h"

void SpiInit(volatile Spi *spi) {
    RccEnable(spi);
}

void SpiWrite(volatile Spi *spi, uint16_t data) {
    spi->dr = data;
}

uint16_t SpiRead(volatile Spi *spi) {
    return spi->dr;
}