/*
 * generic-board.c
 *
 * Description: List of customizations and setup for a generic project
 * License: Revised BSD License, see LICENSE.TXT file include in the project
 *  Created on: Jun 25, 2016
 *      Author: Dan Walkes <danwalkes@trellis-logic.com>
 *
*/
#ifndef __GENERIC_BOARD_H__
#define __GENERIC_BOARD_H__

#include <stdint.h>		// uint_xx typedefs
#include <stdio.h>		// printf

#include "utilities.h"


#ifndef bool
#define bool uint8_t
#endif
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

extern void __disable_irq(void);

/**
 * Use printf for logging
 */
#define BOARD_LOGGER(level,format,...)	printf(#level ":" format "\n" , ##__VA_ARGS__);

/**
 * TODO: move this to a shared definitions file
 */
enum BoardPowerSource
{
    USB_POWER = 0,
    BATTERY_POWER
};

void BoardInitMcu(void);
void BoardInitPeriph(void);

void GpioMcuInitialize(void);

/**
 * Called in tight loops, by default do nothing
 */
static inline void BoardDelay(void) { }

uint8_t BoardGetBatteryLevel( void );

void DelayMs(uint32_t ms);



#include "gpio.h"	// required for radio/sx1276.c/.h
#include "timer.h"  // required for radio/sx1276.c

/**
 * The list of macros below may be configured in a makefile or other global include file
 */
#if BOARD_RADIO_SX1276
#include "sx1276-board.h"
#else
#error "Please define a BOARD_RADIO_xxxx type to one of the supported radios in board.h or add a new one"
#endif

#if BOARD_GPIO_RASPBERRY_PI
#include "Pi/spi-board-pi.h"
#include "Pi/gpio-pins.h"
#elif BOARD_GPIO_UNDEFINED
#warning "Using undefined board GPIO, link will fail"
#define RADIO_RESET                         UNDEFINED_GPIO_0
#define RADIO_DIO_0                         UNDEFINED_GPIO_1
#define RADIO_DIO_1                         UNDEFINED_GPIO_2
#define RADIO_DIO_2                         UNDEFINED_GPIO_3
#define RADIO_DIO_3                         UNDEFINED_GPIO_4
#define RADIO_DIO_4                         UNDEFINED_GPIO_5
#define RADIO_DIO_5                         UNDEFINED_GPIO_6
#else
#error "Please define a BOARD_GPIO_xxxx type to one of the supported types in board.h or add a new one"
#endif

#include "utilities.h"

uint8_t GetBoardPowerSource( void );
void DumpRadioRegs( void );

//TODO: Define assertions for your platform
#ifndef assert_param
#define assert_param(unused)
#endif

#define __enable_irq()

#endif /* __GENERIC_BOARD_H__ */

