/* ! I/O Task XMC4500
 *
 * \description A simple I/O program for the XMC4500
 *
 * \author Sebastian Dichler <el16b032@technikum-wien.at>
 *
 * \version Rev.: 01, 27.09.2017 - Created the c and h file for the project
 *          Rev.: 02, 27.09.2017 - Writing first code from the xmc4500 template
 *          Rev.: 03, 27.09.2017 - Adding 4 different modes without buttons
 *                                 for first try how to talk to the LEDs
 *          Rev.: 04, 28.09.2017 - Added the Buttons to the header file
 *          Rev.: 05, 03.10.2017 - Added Button function, everything is working
 *                                 how it should be but no oversampling of buttons and
 *                                 bitcode with two buttons not just one and one for change
 *          Rev.: 06, 03.10.2017 - Having problems with the button chose button activate
 *                                 algorithm (keeps staying in init)
 *          Rev.: 07, 03.10.2017 - Problem was the Button 2 mechanics and the oversampling
 *
 * \information Using Thomas Jungwirth <el16b028@technikum-wien.at> & Martin Horauers
 *              solution with the button oversampling
 */

/**
 @mainpage XMC I/O Task
 
 @section Usage
 
 This project is intended to be used along with Eclipse or from the CMD-Line
 using a Makefile. The latter provides the following commands:
 
 @verbatim
 make                        # build the project
 make debug                  # build the project and start the debugger
 make flash                  # flash the application
 make doc                    # generate the documentation
 make clean                  # delete intermediate files
 @endverbatim
 
 @section Folders
 
 The code is organized in various folders - checkout the comments below:
 
 @verbatim
 Makefile          # top level Makefile used for the command-line variant
 doxygen           # doxygen configuration file
 inc/              # the place for your header files
 src/              # the place for your source files
 bin/              # output folder - here you will find listings and binaries
 Debug/            # this fiolder is only used by Eclipse
 doc/              # here the generated documentation is found
 system/           # CMSIS library, Infineon header files, linker script, etc.
 xmclib/           # low-level XMC library
 @endverbatim
 
 @file 001_task_io.c
 @date 03 Oct, 2017
 @version 1.0.0
 
 @brief XMC4500 Relax kit GPIO I/O Task
 
 GPIO I/O Task to change bitpattern with one button and activate the led bitpattern with the other button.
 LED1 is connected to P1.1
 LED2 is connected to P1.0
 BUTTON1 is connected to P1.14
 BUTTON2 is connected to P1.15
 
 History <br>
 Rev.: 01, 27.09.2017 - Created the c and h file for the project
 Rev.: 02, 27.09.2017 - Writing first code from the xmc4500 template
 Rev.: 03, 27.09.2017 - Adding 4 different modes without buttons for first try how to talk to the LEDs
 Rev.: 04, 28.09.2017 - Added the Buttons to the header file
 Rev.: 05, 03.10.2017 - Added Button function, everything is working how it should be but no oversampling of buttons and bitcode with two buttons not just one and one for change
 Rev.: 06, 03.10.2017 - Having problems with the button chose button activate algorithm (keeps staying in init)
 Rev.: 07, 03.10.2017 - Problem was the Button 2 mechanics and the oversampling
 <br>
 */

#include "001_task_io.h"

/* ---- gloabl variables ----- */

int toggle = 1;
uint32_t p1_14_pressed;
uint32_t p1_15_pressed;

void SysTick_Handler(void);

int main(void)
{
	XMC_GPIO_CONFIG_t config;
	
	initRetargetSwo();
	
/* ---- set up config for LEDs ---- */
	
	config.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL;
	config.output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH;
	config.output_strength = XMC_GPIO_OUTPUT_STRENGTH_MEDIUM;
	
/* ---- set up config for BUTTONs ---- */
	
	P1_14_set_mode(INPUT);
	P1_15_set_mode(INPUT);
	
	p1_14_pressed = 0;
	p1_15_pressed = 0;
	
/* ---- write config into output LED1 & LED2 ---- */
	
	XMC_GPIO_Init(LED1, &config);
	XMC_GPIO_Init(LED2, &config);
	
	_initUART0_CH0();
	
	SysTick_Config(SystemCoreClock / TICKS_PER_SECOND);
	
	while(1)
	{
		
	}
}

void SysTick_Handler(void)
{
	static uint32_t ticks = 0;
	static uint32_t oversample = 0;
	static uint32_t status = 0;
	static uint32_t activate = 0;
	
/* ---- tickrate and oversampling counter for bouncing buttons ---- */
	
	ticks++;
	oversample++;
	
/* ---- button 1 - changes status ---- */
	
	if ((P1_14_read() == 0) && (p1_14_pressed == 0))
	{
		p1_14_pressed = 1;
		oversample = 0;
	}
	if ((P1_14_read() == 0) && (p1_14_pressed == 1) &&
		(oversample >= TICKS_WAIT/2) && (activate == 0))
	{
		status++;
		if (status > 3)
		{
			status = 0;
		}
		printf("DEBUG: Chosen status: %d\n", (int)status);
		p1_14_pressed = 0;
	}
	
/* ---- button 2 - changes output ---- */
	
	if ((P1_15_read() == 0) && (p1_15_pressed == 0))
	{
		p1_15_pressed = 1;
		oversample = 0;
	}
	if ((P1_15_read() == 0) && (p1_15_pressed == 1) && (oversample >= TICKS_WAIT/2))
	{
		XMC_GPIO_SetOutputLow(LED1);
		XMC_GPIO_SetOutputLow(LED2);
		ticks = 0;
		if (activate == 0)
		{
			activate = 1;
			printf("DEBUG: Button 2 : active\n");
		}
		else
		{
			activate = 0;
			status = 0;
			printf("DEBUG: Button 2 : deactivated, status 0\n");
		}
		p1_15_pressed = 0;
	}
	
/* ---- status cases for LEDs ---- */
	
	if (ticks == TICKS_WAIT && activate == 1)
	{
		switch (status)
		{
			case 0:
				printf("DEBUG: LED1 blinking\n");
				XMC_GPIO_ToggleOutput(LED1);
				XMC_GPIO_SetOutputLow(LED2);
				break;
			case 1:
				printf("DEBUG: LED2 blinking\n");
				XMC_GPIO_SetOutputLow(LED1);
				XMC_GPIO_ToggleOutput(LED2);
				break;
			case 2:
				printf("DEBUG: LED1 and LED2 blinking simutaniously\n");
				if (toggle == 1)
				{
					XMC_GPIO_SetOutputHigh(LED1);
					XMC_GPIO_SetOutputHigh(LED2);
					toggle = 0;
				}
				else
				{
					XMC_GPIO_SetOutputLow(LED1);
					XMC_GPIO_SetOutputLow(LED2);
					toggle = 1;
				}
				break;
			case 3:
				printf("DEBUG: LED1 and LED2 blinking alternating.\n");
				if (toggle == 1)
				{
					XMC_GPIO_SetOutputHigh(LED1);
					XMC_GPIO_SetOutputLow(LED2);
					toggle = 0;
				}
				else
				{
					XMC_GPIO_SetOutputLow(LED1);
					XMC_GPIO_SetOutputHigh(LED2);
					toggle = 1;
				}
				break;
			default:
				printf("ERROR: Both LEDs stop\n");
				XMC_GPIO_SetOutputLow(LED1);
				XMC_GPIO_SetOutputLow(LED2);
				break;
		}
		ticks = 0;
	}
}
