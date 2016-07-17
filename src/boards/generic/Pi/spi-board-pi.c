/*
 * spi-board-pi.c
 *
 *  Created on: Jun 26, 2016
 *      Author: Dan
 */

#include "board.h"
#include "Log.h"
#include <wiringPiSPI.h>
#include <errno.h>

void SpiFormat( Spi_t *obj, int8_t bits, int8_t cpol, int8_t cpha, int8_t slave )
{
	bool error=false;
	obj->Spi=PI_BOARD_SPI_PORT;
	if( bits != SPI_DATASIZE_8BIT )
	{
		LOG(Error,"Unsupported spi bit length %d",bits);
		error=true;
	}
	if( cpol != SPI_POLARITY_LOW )
	{
		LOG(Error,"Unsupported polarity %d",cpol);
		error=true;
	}
	if( cpha != SPI_PHASE_1EDGE )
	{
		LOG(Error,"Unsupported phase %d",cpha)
		error=true;
	}
	if( !error )
	{
		LOG(Info,"SpiFormat complete with success");
	}
}
void SpiFrequency( Spi_t *obj, uint32_t hz )
{
	/**
	 * Always use mode 0
	 */
	int result=wiringPiSPISetupMode(obj->Spi,hz,0);
	if( result < 0 )
	{
		LOG(Error,"Unable to configure SPI port with wiringPiSPISetup, returned %d",result);
	}
	else
	{
		LOG(Info,"SpiFrequency complete with success on SPI port %d",obj->Spi);
	}
}

uint16_t SpiInOut( Spi_t *obj, uint16_t outData )
{
	uint8_t buffer[1];
	/**
	 * Note: for some reason the paramter is a uint16_t but only a single byte is transferred
	 */
	buffer[0]=(uint8_t)outData;
	int rc= wiringPiSPIDataRW(obj->Spi,buffer,1);
	if( rc < 0 )
	{
		LOG(Error,"wiringPiSPIDataRW failed with rc %d errno %d sending byte %d on SPI port %d",rc,errno,buffer[0],obj->Spi);
	}
	return buffer[0];
}
