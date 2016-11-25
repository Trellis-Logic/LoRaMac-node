/*
 * pinName-board.h
 *
 *  Created on: Jun 26, 2016
 *      Author: Dan
 */

#ifndef SRC_BOARDS_GENERIC_PINNAME_BOARD_H_
#define SRC_BOARDS_GENERIC_PINNAME_BOARD_H_

#if BOARD_GPIO_RASPBERRY_PI
#include "Pi/pinName-wiringPi.h"
#elif BOARD_GPIO_UNDEFINED
#warning "Using UNDEFINED board GPIO, link step will fail"
#define MCU_PINS \
	UNDEFINED_GPIO_0=0,\
	UNDEFINED_GPIO_1=1,\
	UNDEFINED_GPIO_2=2,\
	UNDEFINED_GPIO_3=3,\
	UNDEFINED_GPIO_4=4,\
	UNDEFINED_GPIO_5=5,\
	UNDEFINED_GPIO_6=6,\
	UNDEFINED_GPIO_7=7
#else
#error "Please define a board GPIO handler in pinName-board.h"
#endif


#endif /* SRC_BOARDS_GENERIC_PINNAME_BOARD_H_ */
