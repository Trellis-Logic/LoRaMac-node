/*
 * rtc-board.c
 *
 * Description: RTC callouts for generic hardware, required by timer.c
 * License: Revised BSD License, see LICENSE.TXT file include in the project
 *  Created on: Jun 25, 2016
 *      Author: Dan Walkes <danwalkes@trellis-logic.com>
 *
*/


#include "board.h"
#include "timer.h"
#include <string.h>


#if BOARD_RTC_USE_POSIX_TIMER
#include "posix/rtc-board-posix.c"
#else
#error "Please define BOARD_RTC_ macro based on contents in rtc-board.c or define a new one"
#endif

struct RtcAlarm
{
	bool	isSet;
	TimerTime_t	value;
};

struct RtcAlarm RtcAlarm;

void RtcInit(void)
{
	RtcResetStartupTimeReference();
	memset(&RtcAlarm,0,sizeof(RtcAlarm));
}


TimerTime_t RtcGetTimerValue( void )
{
    return RtcGetTimeInMillisecondsSinceStartup();
}


/**
 * @param Delay time in milliseconds to wait inline
 */
void HAL_Delay(uint32_t Delay)
{
	TimerTime_t startTime=RtcGetTimerValue();
	bool exit=false;
	do
	{
		TimerTime_t currentTime=RtcGetTimerValue();
		if( currentTime - startTime < Delay )
		{
			BoardDelay();
		}
		else
		{
			exit=true;
		}
	}while( !exit );
}

void DelayMs(uint32_t Delay)
{
	HAL_Delay(Delay);
}

TimerTime_t RtcGetElapsedAlarmTime( void )
{
	TimerTime_t elapsedTime=0;
	if( RtcAlarm.isSet )
	{
		elapsedTime=RtcGetTimerValue()-RtcAlarm.value;
	}
	else
	{
		LOG(Error,"RtcGetElapsedAlarmTime called before RtcSetTimeout()");
	}
	return elapsedTime;
}

TimerTime_t RtcComputeFutureEventTime( TimerTime_t futureEventInTime )
{
    return( RtcGetTimerValue( ) + futureEventInTime );
}

TimerTime_t RtcComputeElapsedTime( TimerTime_t eventInTime )
{
	return RtcGetTimerValue()-eventInTime;
}


void RTC_Alarm_IRQHandler( int unused )
{
    TimerIrqHandler( );
}

void RtcSetTimeout( uint32_t timeout )
{
	RtcAlarm.isSet=true;
	RtcAlarm.value=RtcComputeFutureEventTime(timeout);
	RtcStartWakeUpAlarm(timeout,RTC_Alarm_IRQHandler);
}

void RtcEnterLowPowerStopMode( void )
{

}

TimerTime_t RtcGetAdjustedTimeoutValue( uint32_t timeout )
{
    return  (TimerTime_t) timeout;
}




