/*
 * interrupt-simulate-posix.c
 *
 *  Created on: Aug 14, 2016
 *      Author: Dan
 */

#include <semaphore.h>
#include <pthread.h>
#include "Log.h"
#include "interrupt-simulate-posix.h"
#include <stdatomic.h>
#include <errno.h>
#include "board.h"

#define MAX_INTERRUPT_FUNCTIONS 6

struct func_ptr_int_entry
{
	func_ptr_void function_ptr;
};
enum interrupt_request
{
	INT_REQUEST_NONE,
	INT_REQUEST_ENABLE,
	INT_REQUEST_DISABLE
};
struct func_ptr_int_simulate
{
	/**
	 * An array of function pointers used for interrupt assignment
	 */
	struct func_ptr_int_entry func_ptr_int_entry[MAX_INTERRUPT_FUNCTIONS];
	/**
	 * The next available entry in func_ptr_int_entry for assignment to an interrupt function
	 */
	int int_entry_offset;
	/**
	 * Semaphore used to kick the interrupt thread for processing of interrupt flags or requests
	 */
	sem_t int_waiting_sem;
	/**
	 * Mutex used to protect int_flags and request
	 */
	pthread_mutex_t int_flags_mutex;
	/**
	 * Flags indicating which interrupts are waiting to be serviced
	 */
	uint32_t int_flags;
	/**
	 * The request for handling interrupts goign forward. Enable or disable requests will be set here
	 */
	enum interrupt_request request;

	pthread_t interrupt_thread_id;
	/**
	 * True if interrupts are currently enabled
	 */
	bool ints_enabled;

};

struct func_ptr_int_simulate g_func_ptr_int_simulate;
/**
 * Global initializer, ensures we initialize g_func_ptr_int_simulate exactly once
 */
bool int_simulate_initialized=false;


static void *int_simulate_thread(void *arg)
{
	do
	{
		if( sem_wait(&g_func_ptr_int_simulate.int_waiting_sem) == 0 )
		{

			unsigned int i=0;
			uint32_t int_flags;
			enum interrupt_request request;
			struct func_ptr_int_simulate *int_sim=&g_func_ptr_int_simulate;
			pthread_mutex_lock(&g_func_ptr_int_simulate.int_flags_mutex);
			int_flags = int_sim->int_flags;
			int_sim->int_flags=0;
			request=int_sim->request;
			int_sim->request=INT_REQUEST_NONE;
			pthread_mutex_unlock(&g_func_ptr_int_simulate.int_flags_mutex);

			if( int_sim->ints_enabled )
			{
				for( i=0; i< MAX_INTERRUPT_FUNCTIONS; i++ )
				{
					if( (0x01 << i) & int_flags )
					{
						(int_sim->func_ptr_int_entry[i].function_ptr)();
					}
				}
			}
			if( request == INT_REQUEST_ENABLE )
			{
				int_sim->ints_enabled=true;
			}
			else if( request == INT_REQUEST_DISABLE )
			{
				int_sim->ints_enabled=false;
			}
		}
	}while(1);
	return arg;
}

void __enable_irq()
{
	struct func_ptr_int_simulate *int_sim=&g_func_ptr_int_simulate;
	pthread_mutex_lock(&int_sim->int_flags_mutex);
	int_sim->request=INT_REQUEST_ENABLE;
	pthread_mutex_unlock(&int_sim->int_flags_mutex);
	int rc=sem_post(&int_sim->int_waiting_sem);
	if( rc != 0 )
	{
		LOG(Error,"sem_post for int_enable_semfailed in __enable_irq");
	}
}

/**
 * @return true if we are currently in the interrupt thread context
 */
static bool in_interrupt_thread()
{
	return pthread_equal(g_func_ptr_int_simulate.interrupt_thread_id,pthread_self()) != 0;
}

void __disable_irq()
{
	struct func_ptr_int_simulate *int_sim=&g_func_ptr_int_simulate;
	pthread_mutex_lock(&int_sim->int_flags_mutex);
	int_sim->request=INT_REQUEST_DISABLE;
	pthread_mutex_unlock(&int_sim->int_flags_mutex);
	int rc=sem_post(&g_func_ptr_int_simulate.int_waiting_sem);
	if( rc != 0 )
	{
		LOG(Error,"sem_post for int_enable_semfailed in __enable_irq");
	}
}



static void interrupt_simulate_init(void)
{
	if( int_simulate_initialized == false )
	{
		int_simulate_initialized=true;
		pthread_attr_t attr;

		memset(&g_func_ptr_int_simulate,0,sizeof(struct func_ptr_int_simulate));
		sem_init(&g_func_ptr_int_simulate.int_waiting_sem,0,0);
		pthread_mutex_init(&g_func_ptr_int_simulate.int_flags_mutex,0);
		int thread_num=0;
		int result=pthread_attr_init(&attr);
		if( result != 0 )
		{
			LOG(Error,"pthread_attr_init returned %d in interrupt_simulate_init",result);
		}
		result = pthread_create(&g_func_ptr_int_simulate.interrupt_thread_id, &attr,
										  int_simulate_thread, &thread_num);
		if( result != 0 )
		{
			LOG(Error,"pthread_create returned %d in interrupt_simulate_init",result);
		}
	}
}

static void func_ptr_interrupt_callback(unsigned char int_number)
{
	pthread_mutex_lock(&g_func_ptr_int_simulate.int_flags_mutex);
	g_func_ptr_int_simulate.int_flags |= (0x01 << int_number);
	pthread_mutex_unlock(&g_func_ptr_int_simulate.int_flags_mutex);
	sem_post(&g_func_ptr_int_simulate.int_waiting_sem);
}

void FuncPtrInterruptCallback0(void)
{
	func_ptr_interrupt_callback(0);
}

void FuncPtrInterruptCallback1(void)
{
	func_ptr_interrupt_callback(1);
}
void FuncPtrInterruptCallback2(void)
{
	func_ptr_interrupt_callback(2);
}
void FuncPtrInterruptCallback3(void)
{
	func_ptr_interrupt_callback(3);
}
void FuncPtrInterruptCallback4(void)
{
	func_ptr_interrupt_callback(4);
}
void FuncPtrInterruptCallback5(void)
{
	func_ptr_interrupt_callback(5);
}

int interrupt_simulate_alloc_next()
{
	int next=g_func_ptr_int_simulate.int_entry_offset;
	g_func_ptr_int_simulate.int_entry_offset++;
	return next;
}

func_ptr_void interrupt_simulate_set_func_ptr_for_int_number(int offset,func_ptr_void new_ptr)
{
	func_ptr_void rtnval=0;
	switch( offset )
	{
		case 0:
			rtnval=FuncPtrInterruptCallback0;
			break;
		case 1:
			rtnval=FuncPtrInterruptCallback1;
			break;
		case 2:
			rtnval=FuncPtrInterruptCallback2;
			break;
		case 3:
			rtnval=FuncPtrInterruptCallback3;
			break;
		case 4:
			rtnval=FuncPtrInterruptCallback4;
			break;
		case 5:
			rtnval=FuncPtrInterruptCallback5;
			break;
		default:
			LOG(Error,"Too many interrupts defined, only %d supported but %d defined",
					MAX_INTERRUPT_FUNCTIONS,offset+1);
			break;
	}
	if( rtnval != NULL )
	{
		g_func_ptr_int_simulate.func_ptr_int_entry[offset].function_ptr=new_ptr;
	}
	return rtnval;
}


/**
 * Simulate an IRQ callback for @param interrupt_callback.
 * @return a function pointer which can be used to trigger callback @param interrupt_callback
 * only when interrupts are enabled.
 */
func_ptr_void interrupt_simulate_map_next_irq( func_ptr_void interrupt_callback )
{
	interrupt_simulate_init();
	int offset=interrupt_simulate_alloc_next();
	func_ptr_void rtnvalue=interrupt_simulate_set_func_ptr_for_int_number(offset,interrupt_callback);
	return rtnvalue;
}


