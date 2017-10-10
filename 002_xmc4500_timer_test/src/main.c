/**!
 * @file     main.c
 * @version  V0.2
 * @date     JUL 17
 * @author   R. Beneder
 *
 * @mainpage	Structured Timer example
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

#include <xmc4500_timer_lib.h>
#include <xmc4500_led_lib.h>

/******************************************************** FUNCTION PROTOTYPES */
void _timeoutfunction (void);
/******************************************************************** DEFINES */

/******************************************************************** GLOBALS */

/********************************************************************/
void _timeoutfunction()
{
	toggle (2);
}

int main (void)
{
	setup_LED();
	setup_timer();
	setup_timer_timeout();

	_timeout (0, 5, 1, _timeoutfunction);

	while (1) {
		//_delayus (1);
		//_delay (0,1,1);
		//_timeout (0,5,1,_timeoutfunction);
		_delay (0, 1, 1);
		toggle (1);
	}
}

/* EOF */
