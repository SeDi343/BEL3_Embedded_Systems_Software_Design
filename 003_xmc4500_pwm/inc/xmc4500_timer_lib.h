/*
 * xmc4500_timer_lib.h
 *
 *  Created on: 21 Jul 2017
 *      Author: rbeneder
 */

#ifndef INC_XMC4500_TIMER_LIB_H_
#define INC_XMC4500_TIMER_LIB_H_

#include <stdint.h>

/******************************************************** FUNCTION PROTOTYPES */
_Bool setup_timer(void);
_Bool setup_timer_timeout(void);


uint8_t _delayus ( uint8_t us );
uint8_t _delay   ( uint8_t min, uint8_t sec, uint8_t ms );
uint8_t _timeout ( uint8_t min, uint8_t sec, uint8_t ms, void (* func )( void ) );

#endif /* INC_XMC4500_TIMER_LIB_H_ */
