/* ! I/O Task XMC4500
 *
 * \description A simple I/O program for the XMC4500
 *
 * \author Sebastian Dichler <el16b032@technikum-wien.at>
 *
 * \version Rev.: 01, 27.09.2017 - Created the c and h file for the project
 */

#ifndef _01_task_io_h
#define _01_task_io_h

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <errno.h>
#include <XMC4500.h>

#include "xmc4500.h"
#include "GPIO.h"
#include <debug_lib.h>
#include <xmc_uart.h>
#include <xmc_gpio.h>

#define LED1 P1_1
#define LED2 P1_0
#define DEBUG 0

#define TICKS_WAIT 500
#define TICKS_PER_SECOND 1000
#define RX_BUFFER_SIZE 64

void wait(unsigned long);

void _initUART0_CH0(void);
uint8_t _uart_send_char(char c);
uint8_t _uart_printf(char *fmt, ...);
uint8_t _uart_send_string(char *str);
uint8_t _uart_get_string (char *str);

void initRetargetSwo(void);
int fgetc(FILE *f);
int fputc(int ch, FILE *f);
#endif /* _01_task_io_h */
