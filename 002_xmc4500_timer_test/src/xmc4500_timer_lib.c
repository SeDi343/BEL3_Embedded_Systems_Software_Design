/**!
 * @file     xmc4500_led_lib.c
 * @version  V0.2
 * @date     JUL 17
 * @author   R. Beneder
 *
 *  \brief This module supports functions to setup the timer and to setup
 *  the timeout timer.
*/

#include <xmc4500_timer_driver.h>
#include <xmc4500_timer_lib.h>

/******************************************************************** GLOBALS */
_Bool interrupt_enable;
/********************************************************************/

void (*function_adress) (void) = NULL;

/*
 * \brief CCU40_0_IRQHandler() CCU40 interrupt handler which is called if the 
 * CCU4 capture and compare unit is configured into timer mode. Within the 
 * interrupt service routine a variable is set which is required to signal the 
 * completion of the configured delay.
 *
 * \param none
 * \return none
 */

void CCU40_0_IRQHandler (void)
{
	interrupt_enable = 1;
}

/*
 * \brief CCU41_0_IRQHandler() CCU41 interrupt handler which is called if the 
 * CCU4 capture and compare unit is configured into timeout mode. Within the 
 * interrupt service routine the CCU41 unit is reseted from timeout mode and if 
 * a valid function pointer is transferred the function pointer is called.
 *
 * \param none
 * \return none
 */

void CCU41_0_IRQHandler (void)
{
	reset_timer_timeout();
	if (function_adress)
		function_adress();
}

/*
 * \brief setup_timer() function is called by the main-routine. Within this 
 * function the SCU configuration setup is called and the timer setup for the 
 * CCU4 unit is called. If the timer configuration returns false the 
 * setup_timer() function will also returns false.
 *
 * \param none
 * \return true if the timer configuration was successful, or false if the timer 
 * configuration failed.
 */

_Bool setup_timer (void)
{
	SCU_configuration();
	if (configure_timer() == false) {
		return false;
	}
	return true;
}

/*
 * \brief setup_timer_timeout() function is called by the main-routine. Within 
 * this function the setup function for the timeout mode is called.
 *
 * \param none
 * \return true if the timeout configuration was successful, or false if the 
 * timer configuration failed.
 */

_Bool setup_timer_timeout (void)
{
	if (configure_timer_timeout() == false) {
		return false;
	}
	return true;
}

/*
 * \brief _delayus() function is called by the main-routine. Within this 
 * function the the driver function _delayus_configuration() is called which 
 * configures the CCU4 unit for a microseconds based delay. Afterwards the 
 * function waits for the variable to be set by the interrupt service routine 
 * CCU40_0_IRQHandler() in order to continue and return.
 *
 * \param uint8_t us, delay in mircoseconds
 * \return 0 if the _delayus() was successful, or 1 if not.
 */

uint8_t _delayus (uint8_t us)
{
	if ( (us >= 1) && (us <= 99)) {
		if (_delayus_configuration (us) == 0) {
			while (!interrupt_enable) {
			}
		}
		interrupt_enable = 0;
		reset_timer();
		return 0;
	}
	return 1;
}

/*
 * \brief _delay() function is called by the main-routine. Within this function
 * the the driver function _delay_configuration() is called which configures the
 * CCU4 unit for a much longer timebasis to gain minutes, seconds and 
 * milliseconds granularity. Afterwards the function waits for the variable to 
 * be set by the interrupt service routine CCU40_0_IRQHandler() in order to 
 * continue and return.
 *
 * \param uint8_t min Delay in minutes
 * \param uint8_t sec Delay in seconds
 * \param uint8_t ms  Delay in milliseconds
 * \return 0 if the _delay() was successful, or 1 if not.
 */

uint8_t _delay (uint8_t min, uint8_t sec, uint8_t ms)
{
	if ( (min >= 0) && (min <= 59) && (sec >= 0) && (sec <= 59) && 
	     (ms >= 1) && (ms <= 99)) {
		if (_delay_configuration (min, sec, ms) == 0) {
			while (!interrupt_enable) {
			}
		}
		interrupt_enable = 0;
		reset_timer();
		return 0;
	}
	return 1;
}

/*
 * \brief _timeout() function is called by the main-routine. Within this 
 * function the the driver function _timeout_configuration() is called which 
 * configures the CCU4 unit for a much longer timebasis to gain minutes, seconds 
 * and milliseconds granularity. Afterwards the function pointer address is 
 * stored in the global variable function_adress which is called within the 
 * CCU41_0_IRQHandler interrupt service routine.
 *
 * \param uint8_t min Delay in minutes
 * \param uint8_t sec Delay in seconds
 * \param uint8_t ms  Delay in milliseconds
 * \param void (*func)(void) function pointer address
 * \return 0 if the _timeout() was successful, or 1 if not.
 */

uint8_t _timeout (uint8_t min, uint8_t sec, uint8_t ms, void (* func) (void))
{
	if ( (min >= 0) && (min <= 59) && (sec >= 0) && (sec <= 59) && 
	     (ms >= 1) && (ms <= 99)) {
		if (_timeout_configuration (min, sec, ms, *func) == 0) {
			function_adress = func;
		}
		return 0;
	}
	return 1;
}

/* EOF */
