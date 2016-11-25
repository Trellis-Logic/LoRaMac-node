/*
 * gpio-board.c
 *
 *  Created on: Jun 26, 2016
 *      Author: Dan
 */

#if BOARD_GPIO_RASPBERRY_PI
#include "Pi/gpio-board-pi.c"
#elif BOARD_GPIO_UNDEFINED
#warning "Using undefined BOARD_GPIO, link step will fail"
#else
#error "Please define a BOARD_GPIO macro or update the supported list in gpio-board.c"
#endif

