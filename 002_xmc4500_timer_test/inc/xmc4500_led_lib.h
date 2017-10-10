/*
 * xmc4500_led_lib.h
 *
 *  Created on: 21 Jul 2017
 *      Author: rbeneder
 */

#ifndef INC_XMC4500_LED_LIB_H_
#define INC_XMC4500_LED_LIB_H_

#include <stdint.h>

_Bool setup_LED (void);
_Bool  toggle(uint8_t LED);

#endif /* INC_XMC4500_LED_LIB_H_ */
