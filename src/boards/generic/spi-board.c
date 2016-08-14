/*
 * spi-board.c
 *
 *  Created on: Jun 26, 2016
 *      Author: Dan
 */


#include "board.h"
#include "spi.h"
#include "spi-board.h"

#if BOARD_GPIO_RASPBERRY_PI
#include "pi/spi-board-pi.c"
#elif BOARD_GPIO_UNDEFINED
#warning "Using UNDEFINED board GPIO, link step will fail"
#else
#error "Please define BOARD_GPIO_ or add handler in spi-board.c"
#endif

void SpiInit( Spi_t *obj, PinNames mosi, PinNames miso, PinNames sclk, PinNames nss )
{
    if( mosi != NC )
    {
    	GpioInit( &obj->Mosi, mosi, PIN_ALTERNATE_FCT, PIN_PUSH_PULL, PIN_PULL_DOWN, 0 );
    }
    if( miso != NC )
    {
    	GpioInit( &obj->Miso, miso, PIN_ALTERNATE_FCT, PIN_PUSH_PULL, PIN_PULL_DOWN, 0 );
    }
    if( sclk != NC )
    {
    	GpioInit( &obj->Sclk, sclk, PIN_ALTERNATE_FCT, PIN_PUSH_PULL, PIN_PULL_DOWN, 0 );
    }

    if( nss != NC )
    {
        GpioInit( &obj->Nss, nss, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
    }

    if( nss == NC )
    {
        SpiFormat( obj, SPI_DATASIZE_8BIT, SPI_POLARITY_LOW, SPI_PHASE_1EDGE, 0 );
    }
    else
    {
        SpiFormat( obj, SPI_DATASIZE_8BIT, SPI_POLARITY_LOW, SPI_PHASE_1EDGE, 1 );
    }
    SpiFrequency( obj, 10000 );
}
