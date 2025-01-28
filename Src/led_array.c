/**
 * Name: Crystal Keck
 * Course: CPE2610
 * Section: 121
 * Assignment: Lab 10 - SPI
 * File: led_array.c
 * Dependencies: led_array.h, spi.h
 */

#include <stdint.h>
#include "spi.h"
#include "gpio.h"
#include "rcc.h"

static volatile Gpio *gpio_b = GPIO_B;
static volatile Gpio *gpio_c = GPIO_C;
static volatile Spi *spi = SPI3;

void spiSend(uint16_t data)
{
    // CS low
    gpio_c->bsrr = 1 << 16;
    // SPE high
    spi->cr1 |= 1 << SPE;
    // DR <- DATA
    spi->dr = data;
    // wait TXE = 1
    uint32_t sr = spi->sr;
    while(true){
        if(sr & (1 << TXE)){
            break;
        }
        sr = spi->sr;
    }
    while (!(spi->sr & (1 << TXE)))
        ;
    // read DR
    SpiRead(spi);
    // Wait Busy = 0
    while (spi->sr & (1 << BSY))
        ;
    // SPE low
    spi->cr1 &= ~(1 << SPE);
    // CS high
    gpio_c->bsrr = 1 << 0;
}

void LedArrayInit()
{
    // CS - PC0
    // CLK - PB3 - AF06 - Move to PC13??
    // MOSI - PC12 - AF06
    RccEnable(gpio_b);
    RccEnable(gpio_c);
    SpiInit(spi);
    // SSM = 1, DFF = 1
    spi->cr1 |= 1 << SSM | 1 << SSI | 1 << DFF | 1 << MSTR;
    // Gpio c
    uint32_t mode = gpio_c->moder;
    mode &= ~(0b11 | (0b11 << 24));
    mode |= MODER_OUTPUT | (MODER_ALTERNATE << 24);
    gpio_c->moder = mode;
    uint32_t af = gpio_c->afrh;
    af &= ~(0b1111 << 16);
    af |= 0b0110 << 16;
    gpio_c->afrh = af;

    // Gpio b
    mode = gpio_b->moder;
    mode &= ~(0b11 << 6);
    mode |= MODER_ALTERNATE << 6;
    gpio_b->moder = mode;
    af = gpio_b->afrl;
    af &= ~(0b1111 << 12);
    af |= 0b0110 << 12;
    gpio_b->afrl = af;

    spiSend(0x0C01);
    spiSend(0x0900);
    spiSend(0x0B07);
}

void LedArraySend(uint8_t *rows)
{
    for (int i = 0; i < 8; i++)
    {
        uint16_t data = 0;
        data |= ((i + 1) << 8) | rows[i];
        spiSend(data);
    }
}