/*
 * main.c
 *
 * Description: Main entry point for a basic The Things Network (TTN) node application running ttnmapper.
 * 	See LoRaMac API documentation at http://stackforce.github.io/LoRaMac-doc/
 * 	and ttnmapper arduino project at https://github.com/jpmeijers/ttnmapperarduino/blob/master/TXfastViaTTN/TXfastViaTTN.ino
 * License: Revised BSD License, see LICENSE.TXT file include in the project
 * Created on: Jun 25, 2016
 * Author: Dan Walkes <danwalkes@trellis-logic.com>
 *
*/

#include "board.h"
#include "LoRaMac.h"
#include "Log.h"
#include "LoRaMac-api-v3.h"

#define TTN_MAPPER_NWK_SKEY { 0xAE, 0x17, 0xE5, 0x67, 0xAE, 0xCC, 0x87, 0x87, 0xF7, 0x49, 0xA6, 0x2F, 0x55, 0x41, 0xD5, 0x22 }
#define TTN_MAPPER_APP_SKEY { 0x8D, 0x7F, 0xFE, 0xF9, 0x38, 0x58, 0x9D, 0x95, 0xAA, 0xD9, 0x28, 0xC2, 0xE2, 0xE7, 0xE4, 0x8F }
/**
 * Make this value something unique and constant for your device
 */
#define TTN_MAPPER_ADDR 0x01FFFFFF



static void McpsConfirm( McpsConfirm_t *McpsConfirm )
{
  // Implementation of the MCPS-Confirm primitive
}
static void McpsIndication( McpsIndication_t *McpsIndication )
{
  // Implementation of the MCPS-Indication primitive
}
static void MlmeConfirm( MlmeConfirm_t *MlmeConfirm )
{
  // Implementation of the MLME-Confirm primitive
}

LoRaMacStatus_t InitializeMac( LoRaMacPrimitives_t *Primitives, LoRaMacCallback_t *Callbacks )
{
	Primitives->MacMcpsConfirm = McpsConfirm;
	Primitives->MacMcpsIndication = McpsIndication;
	Primitives->MacMlmeConfirm = MlmeConfirm;
	Callbacks->GetBatteryLevel=BoardGetBatteryLevel;
	return LoRaMacInitialization( Primitives, Callbacks );
}

LoRaMacStatus_t ActivateByPersonalization( uint8_t *NwkSKey, uint8_t *AppSKey, uint32_t DevAddr )
{
	LoRaMacStatus_t status;
	MibRequestConfirm_t mibReq;
	mibReq.Type = MIB_NET_ID;
	mibReq.Param.NetID = 0x000000;
	status= LoRaMacMibSetRequestConfirm( &mibReq );
	if( status == LORAMAC_STATUS_OK )
	{
		mibReq.Type = MIB_DEV_ADDR;
		mibReq.Param.DevAddr = DevAddr;
		status == LoRaMacMibSetRequestConfirm( &mibReq );
		if( status == LORAMAC_STATUS_OK )
		{
			mibReq.Type = MIB_NWK_SKEY;
			mibReq.Param.NwkSKey = NwkSKey;
			status = LoRaMacMibSetRequestConfirm( &mibReq );
			if( status == LORAMAC_STATUS_OK )
			{
				mibReq.Type = MIB_APP_SKEY;
				mibReq.Param.AppSKey = AppSKey;
				status =LoRaMacMibSetRequestConfirm( &mibReq );
				if( status == LORAMAC_STATUS_OK )
				{
					mibReq.Type = MIB_NETWORK_JOINED;
					mibReq.Param.IsNetworkJoined = true;
					status=LoRaMacMibSetRequestConfirm( &mibReq );
				}
			}
		}
	}
	if( status != LORAMAC_STATUS_OK )
	{
		LOG(Warning,"ActivateByPersonalization failed with status %d",status);
	}
	return status;
}



int main( void )
{
	uint8_t NwkSKey[] = TTN_MAPPER_NWK_SKEY;
	uint8_t AppSKey[] = TTN_MAPPER_APP_SKEY;
	uint32_t DevAddr = TTN_MAPPER_ADDR;
	LOG(Info,"Initializing MCU");
	BoardInitMcu( );
	LOG(Info,"Initializing Peripherals");
    BoardInitPeriph( );

	LoRaMacPrimitives_t LoRaMacPrimitives;
	LoRaMacCallback_t LoRaMacCallbacks;
	LOG(Info,"Initializing MAC");
	LoRaMacStatus_t status = InitializeMac(&LoRaMacPrimitives,&LoRaMacCallbacks);
	if(  status == LORAMAC_STATUS_OK )
	{
		const int LORAWAN_NETWORK_ID=0;
		//status = ActivateByPersonalization(NwkSKey,AppSKey,DevAddr);
		LoRaMacInitNwkIds( LORAWAN_NETWORK_ID, DevAddr, NwkSKey, AppSKey );

		LOG(Info,"Joined network, starting transmit");
		// LoRaWAN node has joined the network
	    {
			static uint8_t AppPort = 2;
			#define LORAWAN_APP_DATA_MAX_SIZE 64
			/*!
			 * User application data
			 */
			static uint8_t AppData[LORAWAN_APP_DATA_MAX_SIZE];
			/**
			 * See see https://github.com/jpmeijers/ttnmapperarduino/blob/master/TXfastViaTTN/TXfastViaTTN.ino
			 * Send a single byte "!" each millisecond
			 */
			AppData[0]=(uint8_t)'!';
			bool exit=false;
			do
			{
				uint8_t sendFrameStatus=0;
				sendFrameStatus = LoRaMacSendFrame( AppPort, AppData, 1 );
				if( sendFrameStatus != 0 )
				{
					if( sendFrameStatus == 1 )
					{
						LOG(Info,"LoRaMacSendFrame returned status busy, retrying");
					}
					else
					{
						LOG(Error,"LoRaMacSendFrame failed with error %d",sendFrameStatus);
						exit=true;
					}
				}
				DelayMs(1000);
			}while (!exit);
		}
	}
	else
	{
		LOG(Error,"InitializeMac failed with status %d",status);
	}
}

