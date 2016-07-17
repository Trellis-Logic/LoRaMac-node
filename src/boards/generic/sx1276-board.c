/*
 * sx1276-board.c
 *
 *  Created on: Jul 8, 2016
 *      Author: Dan
 */

#include "board.h"
#include "system/gpio.h"
#include "sx1276-board.h"
#include "radio/sx1276/sx1276Regs-Fsk.h"

/*!
 * Antenna switch GPIO pins objects
 */
Gpio_t AntSwitchLf;
Gpio_t AntSwitchHf;

/*!
 * Flag used to set the RF switch control pins in low power mode when the radio is not active.
 */
static bool RadioIsActive = false;

void SX1276IoIrqInit( DioIrqHandler **irqHandlers )
{
    GpioSetInterrupt( &SX1276.DIO0, IRQ_RISING_EDGE, IRQ_HIGH_PRIORITY, irqHandlers[0] );
    GpioSetInterrupt( &SX1276.DIO1, IRQ_RISING_EDGE, IRQ_HIGH_PRIORITY, irqHandlers[1] );
    GpioSetInterrupt( &SX1276.DIO2, IRQ_RISING_EDGE, IRQ_HIGH_PRIORITY, irqHandlers[2] );
    GpioSetInterrupt( &SX1276.DIO3, IRQ_RISING_EDGE, IRQ_HIGH_PRIORITY, irqHandlers[3] );
    GpioSetInterrupt( &SX1276.DIO4, IRQ_RISING_EDGE, IRQ_HIGH_PRIORITY, irqHandlers[4] );
    GpioSetInterrupt( &SX1276.DIO5, IRQ_RISING_EDGE, IRQ_HIGH_PRIORITY, irqHandlers[5] );
}

void SX1276IoInit( void )
{
    GpioInit( &SX1276.DIO0, RADIO_DIO_0, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0 );
    GpioInit( &SX1276.DIO1, RADIO_DIO_1, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0 );
    GpioInit( &SX1276.DIO2, RADIO_DIO_2, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0 );
    GpioInit( &SX1276.DIO3, RADIO_DIO_3, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0 );
    GpioInit( &SX1276.DIO4, RADIO_DIO_4, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0 );
    GpioInit( &SX1276.DIO5, RADIO_DIO_5, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0 );
	SpiInit( &SX1276.Spi, RADIO_MOSI, RADIO_MISO, RADIO_SCLK, NC );
}


uint8_t SX1276GetPaSelect( uint32_t channel )
{
    if( channel < RF_MID_BAND_THRESH )
    {
        return RF_PACONFIG_PASELECT_PABOOST;
    }
    else
    {
        return RF_PACONFIG_PASELECT_RFO;
    }
}

#ifdef RADIO_ANT_SWITCH_LF
/**
 * The Nymtech Cherry-LLC module used on the LoRaMote project has pins defined for ANT_LF and ANT_HF.
 * I haven't been able to find any details about this part.
 * The Modtronix inair9b doesn't have antenna select pins, instead connects directly to the tx/rx pin on
 * the SX1276.
 */
void SX1276AntSwInit( void )
{
	// TODO: Understand what this does and if we need it
    GpioInit( &AntSwitchLf, RADIO_ANT_SWITCH_LF, PIN_OUTPUT, PIN_PUSH_PULL, PIN_PULL_UP, 1 );
    GpioInit( &AntSwitchHf, RADIO_ANT_SWITCH_HF, PIN_OUTPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0 );
}

void SX1276AntSwDeInit( void )
{
    GpioInit( &AntSwitchLf, RADIO_ANT_SWITCH_LF, PIN_OUTPUT, PIN_OPEN_DRAIN, PIN_NO_PULL, 0 );
    GpioInit( &AntSwitchHf, RADIO_ANT_SWITCH_HF, PIN_OUTPUT, PIN_OPEN_DRAIN, PIN_NO_PULL, 0 );
}


void SX1276SetAntSwLowPower( bool status )
{
    if( RadioIsActive != status )
    {
        RadioIsActive = status;

        if( status == false )
        {
            SX1276AntSwInit( );
        }
        else
        {
            SX1276AntSwDeInit( );
        }
    }
}


void SX1276SetAntSw( uint8_t rxTx )
{
    if( rxTx != 0 ) // 1: TX, 0: RX
    {
        GpioWrite( &AntSwitchLf, 0 );
        GpioWrite( &AntSwitchHf, 1 );
    }
    else
    {
        GpioWrite( &AntSwitchLf, 1 );
        GpioWrite( &AntSwitchHf, 0 );
    }
}
#else
/**
 * Board configurations like modtronics inair9b which don't use an antenna switch on the processor
 */
void SX1276SetAntSwLowPower( bool status )
{

}
void SX1276SetAntSw( uint8_t rxTx )
{

}
#endif
