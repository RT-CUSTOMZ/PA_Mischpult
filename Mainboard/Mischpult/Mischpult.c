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
* Module: Mischpult.c
* Main function code of PA Mischpult project
* Initializes µController functions
* Main Loop with touch screen polling, VU / Spectrum update and DMX output
* Defines Interrupt Service Routines for all HW modules
*
*/


#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "avr_compiler.h"
#include "twi_master_driver.h"
#include "display.h"
#include "draw.h"
#include "dsp.h"
#include "dmx.h"
#include "adc.h"
#include "UART.h"
#include "Mathe.h"

_Bool update_equilizer =0;
_Bool update_spectrum = 0;

/*
 * Clock_init
 * Select external 16 MHz crystal for clock source
 */ 
void Clock_init(void)
{
	//Xosc Bit 7&6 to 11 | Bit 3 to 0 as 1011
	OSC.XOSCCTRL = OSC_FRQRANGE_12TO16_gc | OSC_XOSCSEL_XTAL_16KCLK_gc;
	//Bit 3 to 1 to enable external oscillator
	OSC.CTRL |= OSC_XOSCEN_bm;
	//wait for stable clock
	while(!(OSC.STATUS & OSC_XOSCRDY_bm));
	//Disable register protection for 4 clocks
	CCP = CCP_IOREG_gc;
	//Set Bit 2:0 to 011 for external oscillator
	CLK.CTRL = CLK_SCLKSEL_XOSC_gc;
}


/*
* PLL_init
* Double clock rate to 32 MHz via internal PLL
*/  
void PLL_init(void)
{
	//Bit 7:0 to 11 for external oscillator as PLL source | 2 as multiplication value
	OSC.PLLCTRL = OSC_PLLSRC_XOSC_gc | 0x02;
	//enable PLL
	OSC.CTRL |= OSC_PLLEN_bm;
	//wait for stable oscillation
	while (!(OSC.STATUS & OSC_PLLRDY_bm));
	//Disable register protection for 4 clocks
	CCP = CCP_IOREG_gc;
	//Set Bit 2:0 for PLL as clock source
	CLK.CTRL = CLK_SCLKSEL_PLL_gc;
}


/*
* Port_init
* I/O Configuration of ports
*/
void Port_init(void)
{
		//Set Port Directions
		PORTA.DIR &= ~PIN1_bm;									//Pin A0 as Input for ADC
		PORTA.DIR |= PIN6_bm;									//Pin A6 as Output to enable Line-Out IC
		PORTC.DIR |= PIN5_bm;									//Pin C5 as Output to reset DSP
		PORTD.DIR |= PIN0_bm;									//Pin D0 as Output for audio switch
		PORTD.DIR |= PIN1_bm;									//Pin D1 as Output for audio switch
		PORTD.DIR |= PIN5_bm;									//PIN D5 as Output for Error Display Comm LED
		PORTD.DIR &= ~PIN6_bm;									//Pin D6 as Input for Display SBUF Interrupt
		PORTD.DIR |= PIN7_bm;									//Pin D7 as Output
		PORTE.DIR |= PIN6_bm;									//Pin E6 as Output
		PORTE.DIR |= PIN7_bm;									//Pin E7 as Output
		//Set Output Levels
		PORTC.OUTCLR = PIN5_bm;									//Pin C5 to Low = Force Reset DSP
		PORTD.OUTCLR = PIN7_bm;									//Pin D7 to Low = Force Reset Display
		PORTE.OUTSET |= PIN6_bm;								//Port E to Output and High for routing Uart to SP3T Output
}

/*
* Timer_init
* Configure Timer C0 to interrupt all 100ms
*/
void Timer_init(void){
	TCC0.CTRLA = TC_CLKSEL_DIV1024_gc;							//Set prescaler to 1024 (1(32MHz / 1024) = 32µs per tick)
	TCC0.CTRLB = TC_WGMODE_NORMAL_gc;							//Normal mode count from 0 to Per Register value
	TCC0.INTCTRLA = TC_OVFINTLVL_LO_gc;							//Lo Interrupt level
	TCC0.PER = 3125;											//100 ms / 32µs = 3125 ticks
}


int main(void)
{
	Clock_init();												//Start-Up external crystal
	PLL_init();													//Use PLL to generate 32 MHz tact from external crystal
	Port_init();												//Configurations of GPIO
	Timer_init();												//Set Timer for DSP data update interval
	UsartC0_init();												//Configure USART C0 used for DMX transmission	
	UsartC1_init();												//Configure USART C1 used for reception of external DMX signals					
	ADCA_init();												//Configure ADC Input used for random number generation
	
	/* Initialize SL2 ring buffers for High, Mid and Low frequencies with 0 */
	InitFloatAvg(&FilterS2LHighValues,0);
	InitFloatAvg(&FilterS2LMidValues,0);
	InitFloatAvg(&FilterS2LLowValues,0);
	
	/* Initialize TWI master. */
	TWI_MasterInit(&twiLCD, &TWIE, TWI_MASTER_INTLVL_LO_gc, TWI_BAUDSETTING);
	TWI_MasterInit(&twiDSP, &TWIC, TWI_MASTER_INTLVL_LO_gc, TWI_BAUDSETTING);
	
	/* Enable LO interrupt level. */
	PMIC.CTRL |= PMIC_LOLVLEN_bm + PMIC_MEDLVLEN_bm;
	sei();


	_delay_ms(100);														//Wait a little bit longer

	
	PORTC.OUTSET = PIN5_bm;												//Pin C5 to High = Release DSP from reset
	
	_delay_ms(100);														//Wait again for voltage supply to stabilize
	
	PORTD.OUTSET = PIN7_bm;												//Pin D7 to High = Release display from reset
	
	_delay_ms(3000);													//Wait 3 seconds to give display time to boot up
	
	PORTA.OUTSET = PIN6_bm;												//Pin A6 to High = Enable Line-Out
	
	switchdmxinput(PA_INTERNAL);										//Switch DMX output to USB/Internal as Default
																		
	draw_audio_menu();													//draw audio menu as default start-up page
	switchdspinput(3,3);												//select USB as default start-up audio source	
		

	while(1)															//now we can read buttons back
	{
		if(selected_menu==1 && update_equilizer)						//Check if the audio menu is displayed
		{	
			update_equilizer = 0;										//Clear update eq. bit until next timer event
			vumeter(1);													//Read and update display VU Meter graphs
			spectrum(1);												//Read and update display spectrum graphs

		}
		if (toogle_worklight)											//If work light has been selected				
		{
			dmx_worklight();											//Constant DMX output to enable work lights
		}
		
		if(toogle_s2l==1 && update_spectrum)							//If s2l is activated execute all 100 ms, but
		{
			//only do the s2l when no other internal dmx function is activated
			if (!toogle_solid && !toogle_red && !toogle_blue && !toogle_green)
			{
			update_spectrum = 0;										//Clear update spectrum bit until next timer event
			spectrum(0);												//Sample a spectrum from the DSP but do not update display
			AddToFloatAvg(&FilterS2LHighValues,twiDSP.readData[5]);		//Add right channels to band specific mean values
			AddToFloatAvg(&FilterS2LHighValues,twiDSP.readData[12]);
			AddToFloatAvg(&FilterS2LMidValues,twiDSP.readData[3]);
			AddToFloatAvg(&FilterS2LMidValues,twiDSP.readData[10]);		//Add left channels to band specific mean values
			AddToFloatAvg(&FilterS2LLowValues,twiDSP.readData[0]);
			AddToFloatAvg(&FilterS2LLowValues,twiDSP.readData[7]);
			s2l_output();												//Output sound 2 light via DMX
			}
		}
		
		//Else check if DMX shall be generated internally
		if(global_pa_source == PA_INTERNAL || toogle_solid || toogle_red || toogle_blue || toogle_green)		
		{
			solid_color();												//Generate DMX output according to display settings
			
		}
		if (!(PORTD.IN & (1<<6)))										//wait for SBUF signal from display to get low to indicate touch interaction
		{
			
			if(checkbuf())												//check display transmission buffer
			{
				if(answerdecode())
				{
					CheckComm(1);										//Display answers right
				}
				else
				CheckComm(0);											//Display not answering
				
			}
		}
	}
}

/*
* TWIE Master Interrupt Vector
* LCD screen I2C Communication
*/
ISR(TWIE_TWIM_vect)
{
	TWI_MasterInterruptHandler(&twiLCD);
}


/*
* TWIE Master Interrupt Vector
* DSP I2C Communication
*/
ISR(TWIC_TWIM_vect)
{
	TWI_MasterInterruptHandler(&twiDSP);
}

//Interrupt Service routing of TC0 executed every 100ms
ISR(TCC0_OVF_vect){
	update_equilizer = 1;								//Allow update of equalizer and visualizer in main loop	
	update_spectrum = 1;								//Allow update of spectrum analysis in main loop
}

//DMA Interrupt used for copying transmitted DMX bytes to UART output buffer
ISR(DMA_CH0_vect)
{
	//clear interrupt flag
	DMA.CH0.CTRLB |= DMA_CH_TRNIF_bm;
	//Deactivate DMA
	DMA.CH0.CTRLA &= ~DMA_CH_ENABLE_bm;
	//Check for finished physical transmission and clear flag
	while (!(USARTC0.STATUS & USART_TXCIF_bm));
	USARTC0.STATUS |= USART_TXCIF_bm;
	//Go back to start of DMX protocol
	DMXStatus = BREAK;
}


//DMA Interrupt used for copying bytes between registers
ISR(DMA_CH1_vect)
{
	//clear interrupt flag
	DMA.CH1.CTRLB |= DMA_CH_TRNIF_bm;
	//Deactivate DMA
	DMA.CH1.CTRLA &= ~DMA_CH_ENABLE_bm;
	//Mark dmx data as invalid to sample new frame from UART
	DMA_complete = 1;
}



//Interrupt Vector for DMX reception from external DMX source
ISR (USARTC1_RXC_vect){
	static unsigned int dmx_channel_rx_count = 0;		//Let's count the DMX channels
	static unsigned char dmx_valid = 0;					//And check if DMX is valid
	unsigned char tmp = 0;								//Tmp is never wrong

	tmp =  USARTC1.DATA;								//Store currently received byte into tmp variable

	if(DMX_data_valid) return;							//If no function uses the collected data quit isr

	if(USARTC1.STATUS & USART_FERR_bm)					//Check for framing error as start of DMX transmission
	{
		//DMX_data_valid = 0;
		dmx_channel_rx_count = 0;						//Start to count at first channel
		if(tmp == 0)									//Check if first value was a 0 which is the start code for normal DMX transmissions
		{
			dmx_valid = 1;								//OK DMX was valid
		}
		else
		{
			dmx_valid = 0;								//First byte was not a DMX start code so wait for the next frame error
		}
		return;
	}

	if(dmx_valid)										//DMX was valid so we can process the other 512 channels
	{
		dmx_buffer[dmx_channel_rx_count] = tmp;			//Copy the received channel byte into the correct position of our DMX reception buffer
		if(dmx_channel_rx_count < 512)					//Repeat until we have gathered all 512 channels
		{
			dmx_channel_rx_count++;
		}
		else
		{
			 DMX_data_valid = 1;							//When the complete DMX frame has been received mark data as valid
			 dmx_valid = 0;								//Restart at next framing error
		}
		return;
	}
}