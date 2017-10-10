/**!
 * @file     xmc4500_led_lib.c
 * @version  V0.2
 * @date     JUL 17
 * @author   R. Beneder
 *
 *  \brief This module supports functions to setup the LEDs and to toggle
 *  the LEDs.
*/

#include <xmc4500_led_lib.h>
#include <xmc4500_led_driver.h>

/*
 * \brief setup_LED() is a library function to configure and setup the LEDs of 
 * the XMC4500 RelaxKit.
 *
 * \param none
 * \return true.
 */
_Bool setup_LED (void)
{
	configure_LED();
	return true;
}

/*
 * \brief toggle() is a library function to toggle either LED1 with the 
 * parameter 1 or LED2 with the given parameter 2 or to turn off the LED1 with
 * the parameter 10 or LED2 with the given parameter 20.
 *
 * \param uint8_t LED can either be 1 for LED1 or 2 for LED2
 * \return true if successful, false if the the parameter is not correct.
 */
_Bool  toggle (uint8_t LED)
{
	if (LED == 1) {
		toggleLed (P1_0);
		return true;
	}
	if (LED == 2) {
		toggleLed (P1_1);
		return true;
	}
	if (LED == 10) {
		offLed(P1_0);
		return true;
	}
	if (LED == 20) {
		offLed(P1_1);
		return true;
	}
	return false;
}
/*
 * \brief pwm() is a function create the PWM Signal for the LEDs opening
 * function pwm_create().
 *
 * \param uint8_t value to change the PWM
 */
void pwm(uint8_t value)
{
	pwm_create(value);
}
