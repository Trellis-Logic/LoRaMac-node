/*
 * rtc-board.h
 *
 *  Created on: Jun 26, 2016
 *      Author: Dan
 */

#ifndef SRC_BOARDS_GENERIC_RTC_BOARD_H_
#define SRC_BOARDS_GENERIC_RTC_BOARD_H_

void RtcInit(void);

void DelayMs(uint32_t ms);

TimerTime_t RtcGetElapsedAlarmTime( void );
TimerTime_t RtcGetTimerValue( void );
TimerTime_t RtcComputeElapsedTime( TimerTime_t eventInTime );
TimerTime_t RtcComputeFutureEventTime( TimerTime_t futureEventInTime );
TimerTime_t RtcGetAdjustedTimeoutValue( uint32_t timeout );
void RtcSetTimeout( uint32_t timeout );
void RtcEnterLowPowerStopMode( void );

#endif /* SRC_BOARDS_GENERIC_RTC_BOARD_H_ */
