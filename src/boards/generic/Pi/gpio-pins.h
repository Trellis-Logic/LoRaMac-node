/*
 * gpio-pins.h
 *
 *  Created on: Jul 8, 2016
 *      Author: Dan
 */

#ifndef SRC_BOARDS_GENERIC_PI_GPIO_PINS_H_
#define SRC_BOARDS_GENERIC_PI_GPIO_PINS_H_


/**
 * See pin name definitions in pinName-wiringPi.h
 * and wiring diagram at http://www.digikey.com/schemeit/project/pi-to-inair9b-connection-Q6JIJC8202QG/
 */
#define RADIO_RESET                         PI_GPIO_2
#define RADIO_DIO_0                         PI_GPIO_1
#define RADIO_DIO_1                         PI_GPIO_4
#define RADIO_DIO_2                         PI_GPIO_3
#define RADIO_DIO_3                         PI_GPIO_0
#define RADIO_DIO_4                         NC	// not connected on inAir9B
#define RADIO_DIO_5                         NC	// not connected on inAir9B
#define RADIO_MOSI							NC	// IO initialized automatically for SPI pins on Raspberry Pi
#define RADIO_MISO							NC	// IO initialized automatically for SPI pins on Raspberry Pi
#define RADIO_SCLK							NC	// IO initialized automatically for SPI pins on Raspberry Pi
#define RADIO_NSS							PI_GPIO_7

/**
 * Use SPI port 0 by default
 */
#define PI_BOARD_SPI_PORT					0
#endif /* SRC_BOARDS_GENERIC_PI_GPIO_PINS_H_ */
