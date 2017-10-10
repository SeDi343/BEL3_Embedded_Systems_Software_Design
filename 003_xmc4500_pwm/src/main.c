/* ! PWM Task XMC4500
 *
 * \description A Software PWM Creator / Pulse LED
 *
 * \author Sebastian Dichler <el16b032@technikum-wien.at>
 *
 * \version Rev.: 01, 10.10.2017 - Changing the main.c file from the Timer Task
 *                                 for the PWM Task
 */

/**!
 * @file     main.c
 * @version  V1
 * @date     10.10.2017
 * @author   Sebastian Dichler
 *
 * @mainpage	Software PWM Creator
 *
 *  \brief Function _delayus() implements a blocking delay for a given
 *         number of microseconds
 *  \param us the number of microseconds to delay in the range 1 to 99
 *  \returns 0 upon success, >0 upon error
 *
 * ***uint8_t _delayus ( uint8_t us );
 *
 *  \brief Function _delay() implements a blocking delay for a given
 *         number of minutes, seconds, and milliseconds
 *  \param min the number of minutes to delay in the range 0 to 59
 *  \param sec the number of seconds to delay in the range 0 to 59
 *  \param ms the number of milliseconds to delay in the range 1 to 99
 *  \returns 0 upon success, >0 upon error
 *
 *	 ***uint8_t _delay ( uint8_t min, uint8_t sec, uint8_t ms );
 *  \brief Function _timeout() implements a non-blocking delay for a given
 *         number of minutes, seconds, and milliseconds. After the given
 *         timeout the callback function func() shall be invoked.
 *  \param min the number of minutes to delay in the range 0 to 59
 *  \param sec the number of seconds to delay in the range 0 to 59
 *  \param ms the number of milliseconds to delay in the range 1 to 99
 *  \returns 0 upon success, >0 upon error
 *
 *  ***uint8_t _timeout ( uint8_t min, uint8_t sec, uint8_t ms, 
 *                        void (* func )( void ) );
*/

#include <stdint.h>
#include <stdbool.h>
#include <GPIO.h>

#include <xmc4500_timer_lib.h>
#include <xmc4500_led_lib.h>

void _timeoutfunction (void);
void SysTick_Handler(void);

#define TICKS_PER_SECOND 10
#define TIMEWAIT 30

uint8_t value;			//Variable for the PWM value in percent
uint8_t togglemode;		//Variable to toggle the PWM from inc to dec
uint8_t breakup;		//Variable for the 30sec timer

/*
 * \brief main() is the MAIN function.
 */
int main (void)
{
	value = 0;
	togglemode = 0;
	breakup = 0;
	
	setup_LED();
	setup_timer();
	setup_timer_timeout();
	
	SysTick_Config(SystemCoreClock / TICKS_PER_SECOND);

	while (1) {
		_timeout (0, 0, 9, _timeoutfunction);	//INC or DEC the value
		pwm(value);								//Create the PWM Signal
	}
}
/*
 * \brief _timeoutfunction() is a function to increase or decrease the value for
 * the PWM signal.
 */
void _timeoutfunction()
{
	if (togglemode == 0)
	{
		value=value+1;
		if (value >= 99)
		{
			togglemode = 1;
		}
	}
	
	if (togglemode == 1)
	{
		value=value-1;
		if (value <= 0)
		{
			togglemode = 0;
		}
	}
}
/*
 * \brief SysTick_Handler() is a function to halt the program after 30secs.
 */
void SysTick_Handler(void)
{
	static uint32_t ticks = 0;
	ticks++;
	
	if (ticks >= TIMEWAIT * TICKS_PER_SECOND)
	{
		toggle(10);
		toggle(20);
		__WFI();
	}
}
