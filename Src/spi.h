/**
 * Name: Crystal Keck
 * Course: CPE2610
 * Section: 121
 * Assignment: Lab 10 - SPI
 * File: spi.h
 * Dependencies: None
 */

#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>

#define SPI2 (volatile Spi *) 0x40003800
#define SPI3 (volatile Spi *) 0x40003C00
#define SPI1 (volatile Spi *) 0x40013000
#define SPI4 (volatile Spi *) 0x40013400

#define CPHA 0
#define CPOL 1
#define MSTR 2
#define BR 3
#define SPE 6
#define LSBFIRST 7
#define SSI 8
#define SSM 9
#define RXONLY 10
#define DFF 11
#define CRCNEXT 12
#define CRCEN 13
#define BIDIOE 14
#define BIDIMODE 15

#define RXDMAEN 0
#define TXDMAEN 1
#define SSOE 2
#define ERRIE 5
#define RXNEIE 6
#define TXEIE 7

#define RXNE 0
#define TXE 1
#define CHSIDE 2
#define UDR 3
#define CRCERR 4
#define MODF 5
#define OVR 6
#define BSY 7
#define FRE 8

typedef struct Spi {
    uint32_t cr1;
    uint32_t cr2;
    uint32_t sr;
    uint32_t dr;
    uint32_t crcpr;
    uint32_t rxcrcr;
    uint32_t txcrcr;
    uint32_t i2scfgr;
    uint32_t i2spr;
} Spi;

void SpiInit(volatile Spi *spi);

void SpiWrite(volatile Spi *spi, uint16_t data);

uint16_t SpiRead(volatile Spi *spi);

#endif /* SPI_H_ */