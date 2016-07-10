/*
 * rtc-board-posix.c
 *
 * Description: Simulates RTC related required functions on posix
 * License: Revised BSD License, see LICENSE.TXT file include in the project
 *  Created on: Jun 25, 2016
 *      Author: Dan Walkes <danwalkes@trellis-logic.com>
 *
*/


#include <time.h>	//posix time functions
#include <math.h>	// round
#include <sys/time.h>	//posix time functions
#include <signal.h>	// posix signals
#include "system/log.h"

struct timespec StartTime;

void RtcResetStartupTimeReference()
{
	int error=clock_gettime(CLOCK_MONOTONIC,&StartTime);
	if( error != 0 )
	{
		LOG(Error,"Could not set time, error was %d",error);
	}
}

static void timespec_diff(struct timespec *result, struct timespec *start, struct timespec *stop)
{
    if ((stop->tv_nsec - start->tv_nsec) < 0) {
        result->tv_sec = stop->tv_sec - start->tv_sec - 1;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
    } else {
        result->tv_sec = stop->tv_sec - start->tv_sec;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec;
    }
    return;
}

/**
 * @return time in relative milliseconds since RtcResetStartupTimeReference() was called
 * Supports up to 136 years of runtime operation before rollover
 */
uint32_t RtcGetTimeInMillisecondsSinceStartup()
{
	struct timespec currentTime;
	struct timespec difference;
	uint64_t ms;
	int error=clock_gettime(CLOCK_MONOTONIC,&currentTime);
	if( error != 0 )
	{
		LOG(Error,"Could get current time, error was %d",error);
	}
	timespec_diff(&difference,&StartTime,&currentTime);
	ms=difference.tv_sec*1000+round(difference.tv_nsec / 1.0e6);
	if( ms > 0xFFFFFFFF )
	{
		LOG(Error,"Overflow calculating millisecond since startup in RtcGetTimeInMillisecondsSinceStartup");
	}
	return (uint32_t)ms;
}

/**
 * Implement the wakeup alarm using posix interval timers
 */
void RtcStartWakeUpAlarm(uint32_t ms_timeout,void(*callback)(int))
{
	struct sigaction action;
	memset(&action,0,sizeof(struct sigaction));
	action.sa_handler=callback;
	sigemptyset (&action.sa_mask);
	action.sa_flags = 0;

	if (sigaction(SIGALRM, &action, NULL) < 0 ) {
		LOG(Error,"Unable to catch SIGALRM");
	}
	struct itimerval it_val;  /* for setting itimer */
	it_val.it_value.tv_sec =     ms_timeout/1000;
	it_val.it_value.tv_usec =    (ms_timeout*1000) % 1000000;
	/**
	 * Make this a one shot timer
	 */
	it_val.it_interval.tv_sec =0;
	it_val.it_interval.tv_usec =0;
	if (setitimer(ITIMER_REAL, &it_val, NULL) == -1) {
		LOG(Error,"error calling setitimer()");
	}
}



