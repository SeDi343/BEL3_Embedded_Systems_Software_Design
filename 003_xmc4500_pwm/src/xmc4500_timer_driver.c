/**!
 * @file     xmc4500_timer_driver.c
 * @version  V0.2
 * @date     JUL 17
 * @author   R. Beneder
 *
 *  \brief This module supports functions to configure the CCU4 unit.
*/

#include <xmc4500_timer_driver.h>

/*
 * \brief SCU_configuration() is a driver function to configure the SCU function 
 * registers for the CCU4 capture and compare unit.
 *
 * \param none
 * \return none
 */

void SCU_configuration (void)
{
	/****************************************************** TIMER CONFIGURATION */
	/*******	SCU_RESET REGISTER	*******/
	//Applies reset of the CCU4
	SCU_RESET->PRSET0 |= 0x01UL << SCU_RESET_PRSET0_CCU40RS_Pos;
	SCU_RESET->PRSET0 |= 0x01UL << SCU_RESET_PRSET0_CCU41RS_Pos;
	SCU_RESET->PRSET0 |= 0x01UL << SCU_RESET_PRSET0_CCU42RS_Pos;
	//SCU_RESET->PRSET1 |= 0x01UL << SCU_RESET_PRSET1_CCU43RS_Pos;
	//Releases reset of the CCU4
	SCU_RESET->PRCLR0 |= 0x01UL << SCU_RESET_PRCLR0_CCU40RS_Pos;
	SCU_RESET->PRCLR0 |= 0x01UL << SCU_RESET_PRCLR0_CCU41RS_Pos;
	SCU_RESET->PRCLR0 |= 0x01UL << SCU_RESET_PRCLR0_CCU42RS_Pos;
	//SCU_RESET->PRCLR1 |= 0x01UL << SCU_RESET_PRCLR1_CCU43RS_Pos;
	//Enables the CCU4 clock via the specific SCU register
	SCU_CLK->CLKSET   |=  0x01UL << SCU_CLK_CLKSET_CCUCEN_Pos;
	//CCU Clock Control in Sleep Mode (ENABLE)
	SCU_CLK ->SLEEPCR |= 0x01UL << SCU_CLK_SLEEPCR_CCUCR_Pos;
	return;
}

/*
 * \brief configure_timer() is a driver function to configure the CCU4 capture 
 * and compare unit for timer mode.
 *
 * \param none
 * \return true after successful configuration
 */

_Bool configure_timer (void)
{
	/*******	Timer Concatenation Enable	*******/
	//*****CC41*****
	CCU40_CC41 -> CMC |= 0x01UL << CCU4_CC4_CMC_TCE_Pos;
	//*****CC42*****
	CCU40_CC42 -> CMC |= 0x01UL << CCU4_CC4_CMC_TCE_Pos;
	//*****CC43*****
	//CCU40_CC43 -> CMC |= 0x01UL << CCU4_CC4_CMC_TCE_Pos;
	/*******	CCU4x SPECIFIC REGISTER	*******/
	//****** 	Prescale run bit set - Enables the prescaler Block
	CCU40->GIDLC |= 0x01UL << CCU4_GIDLC_SPRB_Pos;

	//Shadow Transfer on Clear - Enables a shadow transfer when
	// a timer clearing action is performed
	//****CC40****
	CCU40_CC40->TC |= 0x01UL << CCU4_CC4_TC_CLST_Pos;
	//****CC41****
	CCU40_CC41->TC |= 0x01UL << CCU4_CC4_TC_CLST_Pos;
	//****CC42****
	CCU40_CC42->TC |= 0x01UL << CCU4_CC4_TC_CLST_Pos;

	//Prescaler Initial Value (Is set on 0x00UL for max. accuracy)
	//****CC40****
	CCU40_CC40->PSC = 0x00UL << CCU4_CC4_PSC_PSIV_Pos;
	//****CC41****
	CCU40_CC41->PSC = 0x00UL << CCU4_CC4_PSC_PSIV_Pos;
	//****CC42****
	CCU40_CC42->PSC = 0x00UL << CCU4_CC4_PSC_PSIV_Pos;
	//****CC43****
	//CCU40_CC43->PSC = 0x00UL << CCU4_CC4_PSC_PSIV_Pos;

	//Timer Shadow Period Value
	//****CC40****
	CCU40_CC40->PRS |= 0xFFFFUL;
	//****CC41****
	CCU40_CC41->PRS |= 0xFFFFUL;
	//****CC42****
	CCU40_CC42->PRS |= 0xFFFFUL;
	//****CC43****
	//CCU40_CC43->PRS |= 0xFFFFUL;

	//Slice 0 shadow transfer set enable
	//****CC40****
	CCU40->GCSS |= 0x01UL << CCU4_GCSS_S0SE_Pos;
	//****CC41****
	CCU40->GCSS |= 0x01UL << CCU4_GCSS_S1SE_Pos;
	//****CC42****
	CCU40->GCSS |= 0x01UL << CCU4_GCSS_S2SE_Pos;
	//****CC43****
	//CCU40->GCSS |= 0x01UL << CCU4_GCSS_S3SE_Pos;

	//Slice 0 Prescaler shadow transfer set enable
	//****CC40****
	CCU40->GCSS |= 0x01UL << CCU4_GCSS_S0PSE_Pos;
	//****CC41****
	CCU40->GCSS |= 0x01UL << CCU4_GCSS_S1PSE_Pos;
	//****CC42****
	CCU40->GCSS |= 0x01UL << CCU4_GCSS_S2PSE_Pos;
	//****CC43****
	//CCU40->GCSS |= 0x01UL << CCU4_GCSS_S3PSE_Pos;


	/*******	INTERRUPT	*******/
	//Period match while counting up enable
	//****CC42****
	CCU40_CC42->INTE |= 0x01UL << CCU4_CC4_INTE_PME_Pos;
	//Enables node 1 for interrupt request generation
	NVIC_EnableIRQ (CCU40_0_IRQn);
	//CC40 IDLE mode clear. Removes the CC40 from IDLE mode.
	CCU40->GIDLC |= 0x01UL << CCU4_GIDLC_CS0I_Pos;
	CCU40->GIDLC |= 0x01UL << CCU4_GIDLC_CS1I_Pos;
	CCU40->GIDLC |= 0x01UL << CCU4_GIDLC_CS2I_Pos;
	//Global Start Control CCU40 - ENABELED
	SCU_GENERAL->CCUCON |= 0x01UL << SCU_GENERAL_CCUCON_GSC40_Pos;
	SCU_GENERAL->CCUCON |= 0x01UL << SCU_GENERAL_CCUCON_GSC41_Pos;
	SCU_GENERAL->CCUCON |= 0x01UL << SCU_GENERAL_CCUCON_GSC42_Pos;
	return true;
}

/*
 * \brief configure_timer_timeout() is a driver function to configure the CCU4 
 * capture and compare unit for timeout mode.
 *
 * \param none
 * \return true after successful configuration
 */

_Bool configure_timer_timeout (void)
{
	/****************************************************** TIMER CONFIGURATION */
	/*******	Timer Concatenation Enable	*******/
	//*****CC41*****
	CCU41_CC41 -> CMC |= 0x01UL << CCU4_CC4_CMC_TCE_Pos;
	//*****CC42*****
	CCU41_CC42 -> CMC |= 0x01UL << CCU4_CC4_CMC_TCE_Pos;
	//*****CC43*****
	//CCU40_CC43 -> CMC |= 0x01UL << CCU4_CC4_CMC_TCE_Pos;
	/*******	CCU4x SPECIFIC REGISTER	*******/
	//****** 	Prescale run bit set - Enables the prescaler Block
	CCU41->GIDLC |= 0x01UL << CCU4_GIDLC_SPRB_Pos;

	//Shadow Transfer on Clear - Enables a shadow transfer when
	// a timer clearing action is performed
	//****CC40****
	CCU41_CC40->TC |= 0x01UL << CCU4_CC4_TC_CLST_Pos;
	//****CC41****
	CCU41_CC41->TC |= 0x01UL << CCU4_CC4_TC_CLST_Pos;
	//****CC42****
	CCU41_CC42->TC |= 0x01UL << CCU4_CC4_TC_CLST_Pos;

	//Prescaler Initial Value (Is set on 0x00UL for max. accuracy)
	//****CC40****
	CCU41_CC40->PSC = 0x00UL << CCU4_CC4_PSC_PSIV_Pos;
	//****CC41****
	CCU41_CC41->PSC = 0x00UL << CCU4_CC4_PSC_PSIV_Pos;
	//****CC42****
	CCU41_CC42->PSC = 0x00UL << CCU4_CC4_PSC_PSIV_Pos;
	//****CC43****
	//CCU40_CC43->PSC = 0x00UL << CCU4_CC4_PSC_PSIV_Pos;

	//Timer Shadow Period Value
	//****CC40****
	CCU41_CC40->PRS |= 0xFFFFUL;
	//****CC41****
	CCU41_CC41->PRS |= 0xFFFFUL;
	//****CC42****
	CCU41_CC42->PRS |= 0xFFFFUL;
	//****CC43****
	//CCU40_CC43->PRS |= 0xFFFFUL;

	//Slice 1 shadow transfer set enable
	//****CC40****
	CCU41->GCSS |= 0x01UL << CCU4_GCSS_S0SE_Pos;
	//****CC41****
	CCU41->GCSS |= 0x01UL << CCU4_GCSS_S1SE_Pos;
	//****CC42****
	CCU41->GCSS |= 0x01UL << CCU4_GCSS_S2SE_Pos;
	//****CC43****
	//CCU40->GCSS |= 0x01UL << CCU4_GCSS_S3SE_Pos;

	//Slice 1 Prescaler shadow transfer set enable
	//****CC40****
	CCU41->GCSS |= 0x01UL << CCU4_GCSS_S0PSE_Pos;
	//****CC41****
	CCU41->GCSS |= 0x01UL << CCU4_GCSS_S1PSE_Pos;
	//****CC42****
	CCU41->GCSS |= 0x01UL << CCU4_GCSS_S2PSE_Pos;
	//****CC43****
	//CCU40->GCSS |= 0x01UL << CCU4_GCSS_S3PSE_Pos;
	/*******	INTERRUPT	*******/
	//Period match while counting up enable
	//****CC42****
	CCU41_CC42->INTE |= 0x01UL << CCU4_CC4_INTE_PME_Pos;
	//Enables node 1 for interrupt request generation
	NVIC_EnableIRQ (CCU41_0_IRQn);
	//CC40 IDLE mode clear. Removes the CC40 from IDLE mode.
	CCU41->GIDLC |= 0x01UL << CCU4_GIDLC_CS0I_Pos;
	CCU41->GIDLC |= 0x01UL << CCU4_GIDLC_CS1I_Pos;
	CCU41->GIDLC |= 0x01UL << CCU4_GIDLC_CS2I_Pos;
	//Global Start Control CCU40 - ENABELED
	SCU_GENERAL->CCUCON |= 0x01UL << SCU_GENERAL_CCUCON_GSC40_Pos;
	SCU_GENERAL->CCUCON |= 0x01UL << SCU_GENERAL_CCUCON_GSC41_Pos;
	SCU_GENERAL->CCUCON |= 0x01UL << SCU_GENERAL_CCUCON_GSC42_Pos;
	return true;
}

/*
 * \brief _delayus_configuration() is a driver function to configure the CCU4 
 * capture and compare unit for a microseconds time delay.
 *
 * \param uint8_t us determines how many microseconds the delay should last
 * \return true after successful configuration
 */

uint8_t _delayus_configuration (uint8_t us)
{
	//Define 16Bit Variable and multiply 120 (1us) with us
	uint16_t value_us = 0;
	value_us = 120 * us;
	//Define Prescale Value to fout/1 (0000B)
	//****CC40****
	CCU40_CC40->PSC = 0x00UL << CCU4_CC4_PSC_PSIV_Pos;
	//****CC40****
	CCU40_CC41->PSC = 0x00UL << CCU4_CC4_PSC_PSIV_Pos;
	//****CC40****
	CCU40_CC42->PSC = 0x00UL << CCU4_CC4_PSC_PSIV_Pos;
	//Load Period Shadow Register
	CCU40_CC40->PRS = value_us;
	CCU40_CC41->PRS = 0;
	CCU40_CC42->PRS = 0;
	//Slice 0 shadow transfer set enable
	CCU40->GCSS |= 0x01UL << CCU4_GCSS_S0SE_Pos;
	CCU40->GCSS |= 0x01UL << CCU4_GCSS_S1SE_Pos;
	CCU40->GCSS |= 0x01UL << CCU4_GCSS_S2SE_Pos;
	CCU40->GCSS |= 0x01UL << CCU4_GCSS_S3SE_Pos;

	CCU40->GCSS |= 0x01UL << CCU4_GCSS_S0PSE_Pos;
	CCU40->GCSS |= 0x01UL << CCU4_GCSS_S1PSE_Pos;
	CCU40->GCSS |= 0x01UL << CCU4_GCSS_S2PSE_Pos;
	CCU40->GCSS |= 0x01UL << CCU4_GCSS_S3PSE_Pos;
	//Starts the timer
	CCU40_CC40->TCSET = 0x01UL << CCU4_CC4_TCSET_TRBS_Pos;
	CCU40_CC41->TCSET = 0x01UL << CCU4_CC4_TCSET_TRBS_Pos;
	CCU40_CC42->TCSET = 0x01UL << CCU4_CC4_TCSET_TRBS_Pos;
	//CCU40_CC43->TCSET = 0x01UL << CCU4_CC4_TCSET_TRBS_Pos;
	return 0;
}

/*
 * \brief _delay_configuration() is a driver function to configure the CCU4 
 * capture and compare unit for a minutes, seconds, and milliseconds delay.
 *
 * \param uint8_t min Delay in minutes
 * \param uint8_t sec Delay in seconds
 * \param uint8_t ms  Delay in milliseconds
 * \return true after successful configuration
 */

uint8_t _delay_configuration (uint8_t min, uint8_t sec, uint8_t ms)
{
	//Define 64Bit Variable
	uint64_t value_delay = 0;
	uint16_t value_LSB = 0;
	uint16_t value_MSB = 0;

	value_delay = (ms * 120000) + (sec * 120000000) + (min * 7200000000);

	value_LSB = value_delay / 65535;

	if (value_delay <= 4294967296) {
		//Load Period Shadow Register
		CCU40_CC40->PRS = 65535;
		CCU40_CC41->PRS = value_LSB;
		CCU40_CC42->PRS = 0;
	}
	if (value_delay >= 4294967296) {
		value_MSB = value_delay / (65535 * 32768) * 2;
		//Load Period Shadow Register
		CCU40_CC40->PRS = 65535;
		CCU40_CC41->PRS = 32768;
		CCU40_CC42->PRS = value_MSB;
	}
	//Slice 0 shadow transfer set enable
	CCU40->GCSS |= 0x01UL << CCU4_GCSS_S0SE_Pos;
	CCU40->GCSS |= 0x01UL << CCU4_GCSS_S1SE_Pos;
	CCU40->GCSS |= 0x01UL << CCU4_GCSS_S2SE_Pos;
	CCU40->GCSS |= 0x01UL << CCU4_GCSS_S0PSE_Pos;
	CCU40->GCSS |= 0x01UL << CCU4_GCSS_S1PSE_Pos;
	CCU40->GCSS |= 0x01UL << CCU4_GCSS_S2PSE_Pos;
	//Starts the timer
	CCU40_CC40->TCSET = 0x01UL << CCU4_CC4_TCSET_TRBS_Pos;
	CCU40_CC41->TCSET = 0x01UL << CCU4_CC4_TCSET_TRBS_Pos;
	CCU40_CC42->TCSET = 0x01UL << CCU4_CC4_TCSET_TRBS_Pos;
	return 0;
}

/*
 * \brief _timeout_configuration() is a driver function to configure the CCU4 
 * capture and compare unit for a minutes, seconds, and milliseconds delay and 
 * to transfer a function pointer address to be triggered after the delay 
 * expires.
 *
 * \param uint8_t min Delay in minutes
 * \param uint8_t sec Delay in seconds
 * \param uint8_t ms  Delay in milliseconds
 * \param void (*func)(void) function pointer address
 * \return true after successful configuration
 */

uint8_t _timeout_configuration (uint8_t min, uint8_t sec, uint8_t ms, 
                                void (* func) (void))
{
	//Define 64Bit Variable and multiply 120 (1us) with us
	uint64_t value_delay = 0;
	uint16_t value_LSB = 0;
	uint16_t value_MSB = 0;
	value_delay = (ms * 120000) + (sec * 120000000) + (min * 7200000000);

	value_LSB = value_delay / 65535;

	if (value_delay <= 4294967296) {
		//Load Period Shadow Register
		CCU41_CC40->PRS = 65535;
		CCU41_CC41->PRS = value_LSB;
		CCU41_CC42->PRS = 0;
	}
	if (value_delay >= 4294967296) {
		value_MSB = value_delay / (65535 * 32768) * 2;
		//Load Period Shadow Register
		CCU41_CC40->PRS = 65535;
		CCU41_CC41->PRS = 32768;
		CCU41_CC42->PRS = value_MSB;
	}
	//Slice 0 shadow transfer set enable
	CCU41->GCSS |= 0x01UL << CCU4_GCSS_S0SE_Pos;
	CCU41->GCSS |= 0x01UL << CCU4_GCSS_S1SE_Pos;
	CCU41->GCSS |= 0x01UL << CCU4_GCSS_S2SE_Pos;
	CCU41->GCSS |= 0x01UL << CCU4_GCSS_S0PSE_Pos;
	CCU41->GCSS |= 0x01UL << CCU4_GCSS_S1PSE_Pos;
	CCU41->GCSS |= 0x01UL << CCU4_GCSS_S2PSE_Pos;
	//Starts the timer
	CCU41_CC40->TCSET = 0x01UL << CCU4_CC4_TCSET_TRBS_Pos;
	CCU41_CC41->TCSET = 0x01UL << CCU4_CC4_TCSET_TRBS_Pos;
	CCU41_CC42->TCSET = 0x01UL << CCU4_CC4_TCSET_TRBS_Pos;
	return 0;
}

/*
 * \brief reset_timer() resets the CCU4 capture and compare unit from timer
 * mode.
 *
 * \param none
 * \return none
 */

void reset_timer()
{
	CCU40_CC40-> TCCLR = 0x01UL << CCU4_CC4_TCCLR_TRBC_Pos; //Timer run bit clear
	CCU40_CC40 -> TCCLR = 0x01UL << CCU4_CC4_TCCLR_TCC_Pos; //Timer clear
	CCU40_CC41-> TCCLR = 0x01UL << CCU4_CC4_TCCLR_TRBC_Pos; //Timer run bit clear
	CCU40_CC41 -> TCCLR = 0x01UL << CCU4_CC4_TCCLR_TCC_Pos; //Timer clear
	CCU40_CC42-> TCCLR = 0x01UL << CCU4_CC4_TCCLR_TRBC_Pos; //Timer run bit clear
	CCU40_CC42 -> TCCLR = 0x01UL << CCU4_CC4_TCCLR_TCC_Pos; //Timer clear
	return;
}

/*
 * \brief reset_timer_timeout() resets the CCU4 capture and compare unit from
 * timeout mode.
 *
 * \param none
 * \return none
 */

void reset_timer_timeout()
{
	CCU41_CC40-> TCCLR = 0x01UL << CCU4_CC4_TCCLR_TRBC_Pos; //Timer run bit clear
	CCU41_CC40 ->TCCLR = 0x01UL << CCU4_CC4_TCCLR_TCC_Pos;  //Timer clear
	CCU41_CC41-> TCCLR = 0x01UL << CCU4_CC4_TCCLR_TRBC_Pos; //Timer run bit clear
	CCU41_CC41 ->TCCLR = 0x01UL << CCU4_CC4_TCCLR_TCC_Pos;  //Timer clear
	CCU41_CC42-> TCCLR = 0x01UL << CCU4_CC4_TCCLR_TRBC_Pos; //Timer run bit clear
	CCU41_CC42 ->TCCLR = 0x01UL << CCU4_CC4_TCCLR_TCC_Pos;  //Timer clear
	return;
}

/* EOF */
