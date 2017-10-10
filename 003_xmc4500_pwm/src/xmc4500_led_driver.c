/**!
 * @file     xmc4500_led_driver.c
 * @version  V0.2
 * @date     JUL 17
 * @author   R. Beneder
 *
 *  \brief This module supports functions to configure the LEDs and to toggle
 *  the LEDs.
*/

#include <xmc4500_led_driver.h>
#include <xmc4500_timer_lib.h>

/*
 * \brief configure_LED() is a driver function to configure the ports and pins 
 * of the XMC4500 to drive the LEDs on the RelaxKit
 *
 * \param none
 * \return none
 */
void configure_LED (void)
{
	P1_0_set_mode (OUTPUT_PP_GP);
	P1_0_set_driver_strength (STRONG);
	P1_0_reset();

	P1_1_set_mode (OUTPUT_PP_GP);
	P1_1_set_driver_strength (STRONG);
	P1_1_reset();
	return;
}

/*
 * \brief toggleLed() is a driver function to toggle the port and pins where the
 * LEDs are connected to.
 *
 * \param uint8_t led P1_0 or P1_1 determines which LED should be toggled.
 * \return true if successful, false if the the parameter is not correct.
 */
_Bool toggleLed (uint8_t led)
{
	if (led == P1_0) {
		P1_0_toggle();
		return true;
	}
	if (led == P1_1) {
		P1_1_toggle();
		return true;
	}
	return false;
}
/*
 * \brief offLed() is a function to change the mode from LEDs to INPUT to turn
 * of the LEDs.
 *
 * \param uint8_t led P1_0 or P1_1 determines which LED should be turned off.
 * \retrun true if successful, false if the the parameter is not correct.
 */
_Bool offLed(uint8_t led)
{
	if (led == P1_0) {
		P1_0_set_mode(INPUT);
		return true;
	}
	
	if (led == P1_1) {
		P1_1_set_mode(INPUT);
		return true;
	}
	return false;
}
/*
 * \brief pwm_ceate() is a function create the PWM Signal for the LEDs.
 *
 * \param uint8_t value to change the PWM
 */
void pwm_create(uint8_t value)
{
	toggleLed(P1_0);
	toggleLed(P1_1);
	
	_delayus(value);
	
	toggleLed(P1_0);
	toggleLed(P1_1);
	
	_delayus(99-value);
}
