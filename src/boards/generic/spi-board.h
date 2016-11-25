/*
 * spi-board.h
 *
 *  Created on: Jun 26, 2016
 *      Author: Dan
 */

#ifndef SRC_BOARDS_GENERIC_SPI_BOARD_H_
#define SRC_BOARDS_GENERIC_SPI_BOARD_H_

#if BOARD_GPIO_RASPBERRY_PI
#include "pi/spi-board-pi.h"
#elif BOARD_GPIO_UNDEFINED
#warning "Using undefined GPIO, link step will fail"
typedef int SPI_HandleTypeDef;
#else
#error "Please define BOARD_GPIO_ or add handler in spi-board.h"
#endif

#include "gpio.h"

struct Spi_s
{
    SPI_HandleTypeDef Spi;
    Gpio_t Mosi;
    Gpio_t Miso;
    Gpio_t Sclk;
    Gpio_t Nss;
};

#define SPI_DATASIZE_8BIT               0


/**
  * @}
  */

/** @defgroup SPI_Clock_Polarity SPI Clock Polarity
  * @{
  */
#define SPI_POLARITY_LOW                0


/**
  * @}
  */

/** @defgroup SPI_Clock_Phase SPI Clock Phase
  * @{
  */
#define SPI_PHASE_1EDGE                 0




#endif /* SRC_BOARDS_GENERIC_SPI_BOARD_H_ */
