/*
 * board-radio.h
 *
 *  Created on: Jun 26, 2016
 *      Author: Dan
 */

#ifndef SRC_BOARDS_GENERIC_SX1276_BOARD_H_
#define SRC_BOARDS_GENERIC_SX1276_BOARD_H_

#include "radio/radio.h"
#include "system/spi.h"
#include "radio/sx1276/sx1276.h"

static inline bool SX1276CheckRfFrequency( uint32_t frequency )
{
    // Implement check. Currently all frequencies are supported
    return true;
}


/**
 * A radio callbacks structure suitable for use with a const struct Radio_s Radio = declaration
 */
#define RADIO_CALLBACKS \
	{ \
    SX1276Init,\
    SX1276GetStatus,\
    SX1276SetModem,\
    SX1276SetChannel,\
    SX1276IsChannelFree,\
    SX1276Random,\
    SX1276SetRxConfig,\
    SX1276SetTxConfig,\
    SX1276CheckRfFrequency,\
    SX1276GetTimeOnAir,\
    SX1276Send,\
    SX1276SetSleep,\
    SX1276SetStby,\
    SX1276SetRx,\
    SX1276StartCad,\
    SX1276ReadRssi,\
    SX1276Write,\
    SX1276Read,\
    SX1276WriteBuffer,\
    SX1276ReadBuffer,\
    SX1276SetMaxPayloadLength\
	}

#define RADIO_INIT_REGISTERS_VALUE                \
{                                                 \
    { MODEM_FSK , REG_LNA                , 0x23 },\
    { MODEM_FSK , REG_RXCONFIG           , 0x1E },\
    { MODEM_FSK , REG_RSSICONFIG         , 0xD2 },\
    { MODEM_FSK , REG_AFCFEI             , 0x01 },\
    { MODEM_FSK , REG_PREAMBLEDETECT     , 0xAA },\
    { MODEM_FSK , REG_OSC                , 0x07 },\
    { MODEM_FSK , REG_SYNCCONFIG         , 0x12 },\
    { MODEM_FSK , REG_SYNCVALUE1         , 0xC1 },\
    { MODEM_FSK , REG_SYNCVALUE2         , 0x94 },\
    { MODEM_FSK , REG_SYNCVALUE3         , 0xC1 },\
    { MODEM_FSK , REG_PACKETCONFIG1      , 0xD8 },\
    { MODEM_FSK , REG_FIFOTHRESH         , 0x8F },\
    { MODEM_FSK , REG_IMAGECAL           , 0x02 },\
    { MODEM_FSK , REG_DIOMAPPING1        , 0x00 },\
    { MODEM_FSK , REG_DIOMAPPING2        , 0x30 },\
    { MODEM_LORA, REG_LR_DETECTOPTIMIZE  , 0x43 },\
    { MODEM_LORA, REG_LR_PAYLOADMAXLENGTH, 0x40 },\
}                                                 \

#define RF_MID_BAND_THRESH                          525000000

/*!
 * Radio hardware and global parameters
 */
extern SX1276_t SX1276;

void SX1276IoIrqInit( DioIrqHandler **irqHandlers );

void SX1276SetAntSw( uint8_t rxTx );
void SX1276SetAntSwLowPower( bool status );
uint8_t SX1276GetPaSelect( uint32_t channel );
void SX1276IoInit( void );

#endif /* SRC_BOARDS_GENERIC_SX1276_BOARD_H_ */

