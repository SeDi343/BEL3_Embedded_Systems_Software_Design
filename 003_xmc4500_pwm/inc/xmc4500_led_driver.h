/*
 * xmc4500_led_driver.h
 *
 *  Created on: 21 Jul 2017
 *      Author: rbeneder
 */

#ifndef INC_XMC4500_LED_DRIVER_H_
#define INC_XMC4500_LED_DRIVER_H_

#include <GPIO.h>

#include <stdint.h>
#include <stdbool.h>

/******************************************************************** DEFINES */
enum pins {P1_0=0, P1_1, P1_2, P1_3, P1_4, P1_5, P1_6, P1_7, \
           P1_8, P1_9, P1_10, P1_11, P1_12, P1_13, P1_14, P1_15};

#define LED1 P1_1
#define LED2 P1_0

void configure_LED(void);
_Bool toggleLed(uint8_t led);
_Bool offLed(uint8_t led);
void pwm_create(uint8_t value);

#endif /* INC_XMC4500_LED_DRIVER_H_ */
