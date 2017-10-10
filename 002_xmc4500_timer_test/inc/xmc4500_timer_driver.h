/*
 * xmc4500_timer_driver.h
 *
 *  Created on: 21 Jul 2017
 *      Author: rbeneder
 */

#ifndef _xmc4500_timer_driver_
#define _xmc4500_timer_driver_

#include <stdint.h>
#include <stdbool.h>
#include <XMC4500.h>
#include <stdlib.h>
#include "GPIO.h"


extern _Bool interrupt_enable;
extern void(*function_adress)(void);

/******************************************************************** DEFINES */

/******************************************************** FUNCTION PROTOTYPES */
_Bool configure_timer(void);
_Bool configure_timer_timeout(void);

void SCU_configuration(void);

uint8_t _delayus_configuration(uint8_t us);
uint8_t _delay_configuration ( uint8_t min, uint8_t sec, uint8_t ms );
uint8_t _timeout_configuration ( uint8_t min, uint8_t sec, uint8_t ms, void (* func )( void ) );

void reset_timer(void);
void reset_timer_timeout(void);

#endif
