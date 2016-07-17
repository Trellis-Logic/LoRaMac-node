/*
 * gpio-b	oard.c
 *
 *  Created on: Jun 26, 2016
 *      Author: Dan
 */


#include "board.h"
#include <wiringPi.h>
#include "gpio.h"
#include "Log.h"

void GpioMcuInitialize(void)
{
	wiringPiSetup();
}
void GpioMcuInit( Gpio_t *obj, PinNames pin, PinModes mode, PinConfigs config, PinTypes type, uint32_t value )
{
	int piMode=INPUT;
    obj->pin = pin;
	if( mode == PIN_INPUT || mode == PIN_OUTPUT )
	{
		piMode= PIN_INPUT ? INPUT : OUTPUT;
		pinMode(pin,piMode);
		if( type ==  PIN_PULL_DOWN )
		{
			pullUpDnControl(pin,PUD_DOWN);
		}
		if( type == PIN_PULL_UP )
		{
			pullUpDnControl(pin,PUD_UP);
		}
		else
		{
			pullUpDnControl (pin, PUD_OFF) ;
		}
		digitalWrite(pin,value);
	}
	else if( mode != PIN_ALTERNATE_FCT )
	{
		LOG(Warn,"Unhandled pin mode %d for pin %d, ignored",mode,pin);
	}

}

void GpioMcuSetInterrupt( Gpio_t *obj, IrqModes irqMode, IrqPriorities irqPriority, GpioIrqHandler *irqHandler )
{
	switch( irqMode )
	{
		case IRQ_RISING_EDGE:
			irqMode=INT_EDGE_RISING;
			break;
		case IRQ_FALLING_EDGE:
			irqMode=INT_EDGE_FALLING;
			break;
		case IRQ_RISING_FALLING_EDGE:
			irqMode=INT_EDGE_BOTH;
			break;
		default:
			LOG(Warn,"Unhandled irqMode %d for pin %d, ignoring",irqMode,obj->pin);
			break;
	}
	if( irqMode != INT_EDGE_SETUP )
	{
		wiringPiISR (obj->pin, irqMode, irqHandler);
	}
}


void GpioMcuWrite( Gpio_t *obj, uint32_t value )
{
	digitalWrite(obj->pin,value);
}

uint32_t GpioMcuRead( Gpio_t *obj )
{
	return digitalRead(obj->pin);
}

void GpioMcuToggle( Gpio_t *obj )
{
	GpioMcuWrite(obj,GpioMcuRead(obj)==0);
}

