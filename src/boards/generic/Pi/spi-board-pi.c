/*
 * spi-board-pi.c
 *
 *  Created on: Jun 26, 2016
 *      Author: Dan
 */

#if 0
#include "board.h"

void SpiFormat( Spi_t *obj, int8_t bits, int8_t cpol, int8_t cpha, int8_t slave )
{
	obj->Spi=0;
	if( bits != SPI_DATASIZE_8BIT )
	{
		LOG(Error,"Unsupported spi bit length %d",bits);
	}
	if( cpol != SPI_POLARITY_LOW )
	{
		LOG(Error,"Unsupported polarity %d",cpol);
	}
	if( cpha != SPI_PHASE_1EDGE )
	{
		LOG(Error,"Unsupported phase %d",cpha)
	}
}
void SpiFrequency( Spi_t *obj, uint32_t hz )
{
	/**
	 * Always use mode 0
	 */
	if( wiringPiSPISetupMode(obj->Spi,hz,0) < 0 )
	{
		LOG(Error,"Unable to configure SPI port with wiringPiSPISetup");
	}
}

uint8_t SpiInOut( Spi_t *obj, uint8_t outData )
{
	uint8_t buffer[1];
	buffer[0]=outData;
	int rc= wiringPiSPIDataRW(obj->Spi,buffer,1);
	if( rc != 0 )
	{
		LOG(Error,"wiringPiSPIDataRW failed with error %d",rc);
	}
	return buffer[0];
}
#endif
