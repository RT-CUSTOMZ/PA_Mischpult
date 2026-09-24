/*        _         _             _       _                  _          _            _            _   _            _
*         /\ \      /\ \         /\ \     /\_\               / /\       /\ \         /\ \         /\_\/\_\ _      /\ \
*        /  \ \     \_\ \       /  \ \   / / /         _    / /  \      \_\ \       /  \ \       / / / / //\_\   /  \ \
*       / /\ \ \    /\__ \     / /\ \ \  \ \ \__      /\_\ / / /\ \__   /\__ \     / /\ \ \     /\ \/ \ \/ / /__/ /\ \ \
*      / / /\ \_\  / /_ \ \   / / /\ \ \  \ \___\    / / // / /\ \___\ / /_ \ \   / / /\ \ \   /  \____\__/ //___/ /\ \ \
*     / / /_/ / / / / /\ \ \ / / /  \ \_\  \__  /   / / / \ \ \ \/___// / /\ \ \ / / /  \ \_\ / /\/________/ \___\/ / / /
*    / / /__\/ / / / /  \/_// / /    \/_/  / / /   / / /   \ \ \     / / /  \/_// / /   / / // / /\/_// / /        / / /
*   / / /_____/ / / /      / / /          / / /   / / /_    \ \ \   / / /      / / /   / / // / /    / / /        / / /    _
*  / / /\ \ \  / / /      / / /________  / / /___/ / //_/\__/ / /  / / /      / / /___/ / // / /    / / /         \ \ \__/\_\
* / / /  \ \ \/_/ /      / / /_________\/ / /____\/ / \ \/___/ /  /_/ /      / / /____\/ / \/_/    / / /           \ \___\/ /
* \/_/    \_\/\_\/       \/____________/\/_________/   \_____\/   \_\/       \/_________/          \/_/             \/___/_/
*
*
* Main Project: PA Mischpult
* Based on ATxMega256A3U µController
*
* Module: adc.c
* Configuration and control of ADC conversion
* Functions for random number generation
*
*/

#include "adc.h"
#include <avr/pgmspace.h>
#include <stddef.h>

/*! Defining number of bytes in buffer. */
#define NUM_BYTES        8

unsigned char max_bits=BYTES;  // max. number

void ADCA_init()
{
	//Set ADC to 8 Bit Resolution
	ADCA.CTRLB = ADC_RESOLUTION_8BIT_gc;
	//Use AREF of PORTA as Reference Voltage
	ADCA.REFCTRL = ADC_REFSEL_INT1V_gc | 0x02;
	//Divide system clock by 256 = 125kHz ADC clock
	ADCA.PRESCALER = ADC_PRESCALER_DIV512_gc;
	//Enable ADCA
	ADCA.CTRLA = ADC_ENABLE_bm;
}

unsigned int ADCA_Conversion(ADC_CH_t *Channel, char Pin)
{
	//Configure channel to single ended mode
	Channel->CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc;
	//Select the wanted input pin
	Channel->MUXCTRL = (Pin << 3);
	//Start a conversion
	Channel->CTRL |= ADC_CH_START_bm;
	//Wait until conversion has finished
	while(!Channel->INTFLAGS);
	//Clear interupt flag by hand
	Channel->INTFLAGS = ADC_CH_CHIF_bm;
	//Return result of conversion
	return Channel->RES;
}


int LeseKalibrationsbyte(int Index)
{
	int result;
	NVM_CMD = NVM_CMD_READ_CALIB_ROW_gc;
	result = pgm_read_byte(Index);
	NVM_CMD = NVM_CMD_NO_OPERATION_gc;
	return(result);
}


void ADCA_Cal(void)
{
	ADCA.CALL =LeseKalibrationsbyte(offsetof(NVM_PROD_SIGNATURES_t,ADCACAL0));
	ADCA.CALH =LeseKalibrationsbyte(offsetof(NVM_PROD_SIGNATURES_t,	ADCACAL1));
}


unsigned int get_seed(unsigned char bit_count)
{
	volatile unsigned int mix_up=0;
	unsigned char index=0;
	while(index<bit_count)
	{
		// LSB of ADC Conversion will be shifted into a word
		mix_up+=(ADCA_Conversion(&(ADCA.CH0), 1)&1)<<index++;
	}
	return mix_up;
}

signed int get_rand(signed int min, signed int max)
{
	return min+(((signed long)(max-min+1)*get_seed(max_bits))>>max_bits);
}