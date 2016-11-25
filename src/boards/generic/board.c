/*
 * board.c
 *
 * Description: Generic board definition file, to customize, see the board.h file in this directory
 * License: Revised BSD License, see LICENSE.TXT file include in the project
 *  Created on: Jun 25, 2016
 *      Author: Dan Walkes <danwalkes@trellis-logic.com>
 *
*/
#include "board.h"
#include "radio.h"
#include "rtc-board.h"

const struct Radio_s Radio = RADIO_CALLBACKS;

void __disable_irq(void)
{

}

void BoardInitMcu(void)
{
	RtcInit();
	SX1276IoInit( );
}

void BoardInitPeriph(void)
{
	GpioMcuInitialize();
}

/**
 * Required for timer.c
 */
uint8_t GetBoardPowerSource( void )
{
    return USB_POWER;
}
/**
 * Required for Mac initialization
 */
uint8_t BoardGetBatteryLevel( void )
{
	return 254;
}


