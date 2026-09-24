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
* Module: dsp.c
* Everything related to DSP communication
* Processing volume and source selection
* Reading and processing VU and spectrum informations
*
*/

#include "twi_master_driver.h"
#include "dsp.h"
#include "display.h"
#include "draw.h"

//Look-Up Table for Volume Control
const uint32_t lut_volume[115] =
{	0x07ECA9CD,		//0 
	0x07100C4D,
	0x064B6CAD,
	0x059C2F01,
	0x05000000,
	0x0474CD1B,
	0x04000000,
	0x038A2BAC,
	0x0327A01A,
	0x02CFCC01,
	0x02818508,
	0x023BC147,
	0x02000000,
	0x01C62940,
	0x0194C583,
	0x0168C0C5,
	0x0141857E,
	0x011E8E6A,
	0x00F11B69,
	0x00D6E30C,
	0x00CADDC7,
	0x00B4CE07,
	0x00A12477,
	0x008F9E4C,
	0x00800000,		//24 = 0dB
	0x00721482,
	0x0065AC8C,
	0x005A9DF7,
	0x0050C335,
	0x0047FACC,
	0x00400000,
	0x00392CED,
	0x0032F52C,
	0x002D6A86,
	0x00287A26,
	0x00241346,
	0x00200000,
	0x001CA7D7,
	0x00198A13,
	0x0016C310,
	0x00144960,
	0x0012149A,
	0x00100000,
	0x000E5CA1,
	0x000CCCCC,
	0x000B6873,
	0x000A2ADA,
	0x00090FCB,
	0x00080000,
	0x000732AE,
	0x00066A4A,
	0x0005B7B1,
	0x00051884,
	0x00048AA7,
	0x00040000,
	0x00039B87,
	0x00033718,
	0x0002DD95,
	0x00028DCE,
	0x000246B4,
	0x00020000,
	0x0001CEDC,
	0x00019C86,
	0x00016FA9,
	0x000147AE,
	0x0001240B,
	0x00010000,
	0x0000E7FA,
	0x0000CEC0,
	0x0000B844,
	0x0000A43A,
	0x0000925E,
	0x00008000,
	0x00007443,
	0x0000679F,
	0x00005C5A,
	0x0000524F,
	0x0000495B,
	0x00004000,
	0x00003A45,
	0x000033EF,
	0x00002E49,
	0x00002940,
	0x000024C4,
	0x00002000,
	0x00001D34,
	0x00001A07,
	0x00001732,
	0x000014AC,
	0x0000126D,
	0x00001000,
	0x00000EA3,
	0x00000D0B,
	0x00000BA0,
	0x00000A5C,
	0x00000800,
	0x00000689,
	0x00000531,
	0x00000400,
	0x00000346,
	0x0000029A,
	0x00000200,
	0x000001A4,
	0x0000014D,
	0x00000109,
	0x000000D2,
	0x000000A7,
	0x00000084,
	0x00000069,
	0x00000053,
	0x00000042,
	0x00000034,
	0x0000002A,
	0x00000021,
	0x0000001A,
};




void switchdspinput(uint8_t int_channel, uint8_t ext_channel)
{
	uint8_t mux[5] = {muxreg,0x00,0x00,0x00,int_channel};					//Mux Switch mapped by PPS to address 0x23 and 4 bytes long with channel information
	uint8_t volume[5] = {volreg,0x00,0x00,0x00,0x00};					//PPS mapped address for master volume control
	static uint32_t startvolume = 0;						//The current volume set in the DSP
	static uint8_t lut_start = 0;							//Position in Lut_Volume that matches current volume set in DSP
	static uint8_t currentvolume = 0;						//Lut_Volume position we are currently using during fading actions
	uint8_t stepsize = 4;							//How fast are we going through the look up table? 
	uint32_t temp_volume = 0;
	uint8_t i=0;
	uint8_t j=0;

	//Read current master volume level from DSP
	TWI_MasterWriteRead(&twiDSP, TWI_SLAVE_DSP_W, &volume[0], 1,4);
	
	//Here we fade out the volume
	//Need to construct a 32 bit integer for volume level calculation from 4 single byte read via I2C from DSP
	startvolume = twiDSP.readData[0];			//Copy first byte into 32 bit variable startvolume
	for (i=1;i<4;i++)							//Repeat next steps 3 times starting at i=1 because we need already second bit from DSP
	{
	startvolume = startvolume << 8;				//Bit shift by 8 to move already stored information a whole byte to the left
	startvolume |= twiDSP.readData[i];			//Add via or next byte of DSP volume info to 32 bit variable startvolume
	}
	
	for (lut_start=0;lut_start<115;lut_start++)
	{
		if(startvolume == lut_volume[lut_start])
		{
		currentvolume = lut_start;
		break;
		}
		
	}
	
	for (i=1;i<31;i++)							//Do the fade outs within a maximum of 30 steps
	{
	currentvolume = currentvolume + stepsize;		//Reduce volume via stepping through look up table with step size (4*30 = 120)
	
		if (currentvolume >= 114)				//Ensure that step size won't result in negative value
		{
			uint8_t volume[5] = {volreg,0x00,0x00,0x00,0x1A};	//Cap lowest master volume to -110 dB
		
			//Set lowest master volume level to DSP
			TWI_MasterWriteRead(&twiDSP, TWI_SLAVE_DSP_W, &volume[0], 5,0);

			break;
		} 
		else
		{
			temp_volume = lut_volume[currentvolume];
			for (j=5;j>0;j--)							//Construct last 4 bytes of array volume[] to be send via I2C to DSP
			{
				volume[j-1] = temp_volume;				//Copy 8 Bits of 32 bit value currentvolume into 1 byte of I2C transmission array
				temp_volume = temp_volume >> 8;			//Shift by 8 to move next byte onto correct place for byte copy 
			}
			volume[0] = volreg;							//Add volreg register to first byte of volume array
			
			//Send new master volume level to DSP
			TWI_MasterWriteRead(&twiDSP, TWI_SLAVE_DSP_W, &volume[0], 5,0);
				
	
		}
	
	
	}
	
	
	switch(ext_channel)			//Here we switch the external Mux switches
	{
		case 1:
		PORTD.OUTCLR = PIN0_bm;
		PORTD.OUTCLR = PIN1_bm;
		break;
		
		case 2:
		PORTD.OUTSET = PIN0_bm;
		PORTD.OUTCLR = PIN1_bm;
		break;
		
		case 3: //USB
		PORTD.OUTCLR = PIN0_bm;
		PORTD.OUTSET = PIN1_bm;
		break;
		
		case 4:
		PORTD.OUTSET = PIN0_bm;
		PORTD.OUTSET = PIN1_bm;
		break;
		
		default:
		PORTD.OUTCLR = PIN0_bm;
		PORTD.OUTCLR = PIN1_bm;
		break;
	}
	
	
	//Switch to selected input channel via dsp internal mux switch
	TWI_MasterWriteRead(&twiDSP, TWI_SLAVE_DSP_W, &mux[0], 5,0);

	
		for (i=1;i<31;i++)										//Do the fade ins
		{
			currentvolume = currentvolume - stepsize;			//Increase master volume by stepvolume over time
			
			if (currentvolume <= lut_start)						//Cap master volume to level it had before fade out was applied
			{
				temp_volume = lut_volume[lut_start];
				for (j=5;j>0;j--)								//Construct last 4 bytes of array volume[] to be send via I2C
 				{
					volume[j-1] = temp_volume;					//Copy startvolume into one byte of array
					temp_volume = temp_volume >> 8;				//Shift start volume values around
				}
				volume[0] = volreg;								//Add volume register information to first byte of volume array 
				//Send original master volume level to DSP
				TWI_MasterWriteRead(&twiDSP, TWI_SLAVE_DSP_W, &volume[0], 5,0);
				
				break;
			}
			else
			{
				temp_volume = lut_volume[currentvolume];
				for (j=5;j>0;j--)							//Construct last 4 bytes of array volume[] to be send via I2C
				{
					volume[j-1] = temp_volume;				//Copy current volume into one byte of array
					temp_volume = temp_volume >> 8;			//Shift current volume values around
				}
				volume[0] = volreg;							//Add volume register information to first byte of volume array
				//Send new master volume level to DSP
				TWI_MasterWriteRead(&twiDSP, TWI_SLAVE_DSP_W, &volume[0], 5,0);

			}

		}

}


uint8_t readmastervolume(void)
{
	uint8_t volume[1] = {volreg};				//PPS mapped address for master volume control
	uint32_t mastervolume = 0;
	uint8_t i;

	//Read current master volume level from DSP
	TWI_MasterWriteRead(&twiDSP, TWI_SLAVE_DSP_W, &volume[0], 1,4);

	
	//Lets calculate the current volume
	//Need to construct a 32 bit integer for volume level calculation from 4 single byte read via I2C from DSP
	mastervolume |= twiDSP.readData[0];				//Copy first byte into 32 bit variable startvolume
	for (i=1;i<4;i++)							//Repeat next steps 3 times starting at i=1 because we need already second bit from DSP
	{
		mastervolume = mastervolume << 8;					//Bit shift by 8 to move already stored information a whole byte to the left
		mastervolume |= twiDSP.readData[i];			//Add via or next byte of DSP volume info to 32 bit variable startvolume
	}


	//now map master volume to a value between 0 and 100% for touch bar graph 
	
	for (i=0;i<115;i++)
	{
		if(mastervolume == lut_volume[i])
		break;
	}
	return 115-i;	
}


void setmastervolume(uint8_t level)
{
	uint8_t volume[5] = {volreg,0x00,0x00,0x00,0x00};	
	uint8_t i = 0;
	uint32_t temp_volume = 0;
	
	
	temp_volume = lut_volume[115-level];
	for (i=4;i>0;i--)							//Construct last 4 bytes of array volume[] to be send via I2C
	{
		volume[i] = temp_volume;				//Copy current volume into one byte of array
		temp_volume = temp_volume >> 8;			//Shift current volume values around
	}
					
	//Send new master volume level to DSP
	TWI_MasterWriteRead(&twiDSP, TWI_SLAVE_DSP_W, &volume[0], 5,0);
					
}


void vumeter(unsigned char update_on)
{
	uint8_t vuenable[5]={specenbl,0x00,0x80,0x00,0x00};				//Command to enable the vu meter
	uint8_t vuread[1]={specenbl};									//Address to read vu meter data from
	uint8_t vudisable[5]={specenbl,0x00,0x00,0x00,0x00};			//Command to disable the vu meter


		TWI_MasterWriteRead(&twiDSP, TWI_SLAVE_DSP_W, &vuenable[0], 5,0);
		
		_delay_ms(1);								//Give the DSP some time to sample the values internaly

		TWI_MasterWriteRead(&twiDSP, TWI_SLAVE_DSP_W, &vuread[0], 1,4);

		if (update_on)								//If the bar graph should be updated
		{
			eDIP_BA(21,twiDSP.readData[0]);			//Set bar graph left channel to vu meter read value
			eDIP_BA(22,twiDSP.readData[1]);			//Set bar graph right channel to vu meter read value
		}
		
		TWI_MasterWriteRead(&twiDSP, TWI_SLAVE_DSP_W, &vudisable[0], 5,0);
					
}


void spectrum(unsigned char update_on)
{
	uint8_t specenable[5]={vuenbl,0x00,0x80,0x00,0x00};				//Command to enable the spectrum analysis
	uint8_t speread[1]={vuenbl};									//Address to read spectrum data from
	uint8_t specdisable[5]={vuenbl,0x00,0x00,0x00,0x00};			//Command to disable the spectrum analysis	
		
	
	TWI_MasterWriteRead(&twiDSP, TWI_SLAVE_DSP_W, &specenable[0], 5,0);
	
	_delay_ms(1);								//Give the DSP some time to sample the values internaly

	TWI_MasterWriteRead(&twiDSP, TWI_SLAVE_DSP_W, &speread[0], 1,16);

	if (update_on)								//if the spectrum analysis graph should be updated
	{
		eDIP_BA(1,twiDSP.readData[0]);			//Update all 14 values from spectrum analysis
		eDIP_BA(2,twiDSP.readData[1]);			//First 7 bytes left channel spectrum
		eDIP_BA(3,twiDSP.readData[2]);			//Last 7 bytes right channel spectrum
		eDIP_BA(4,twiDSP.readData[3]);
		eDIP_BA(5,twiDSP.readData[4]);
		eDIP_BA(6,twiDSP.readData[5]);
		eDIP_BA(7,twiDSP.readData[6]);
		eDIP_BA(8,twiDSP.readData[7]);
		eDIP_BA(9,twiDSP.readData[8]);
		eDIP_BA(10,twiDSP.readData[9]);
		eDIP_BA(11,twiDSP.readData[10]);
		eDIP_BA(12,twiDSP.readData[11]);
		eDIP_BA(13,twiDSP.readData[12]);
		eDIP_BA(14,twiDSP.readData[13]);
	}
	
	TWI_MasterWriteRead(&twiDSP, TWI_SLAVE_DSP_W, &specdisable[0], 5,0);
}


void mute(uint8_t mute_on_off)
{
	uint8_t volume[5] = {volreg,0x00,0x00,0x00,0x00};			//PPS mapped address for master volume control
	static uint32_t startvolume = 0;							//The current volume set in the DSP
	static uint8_t lut_start = 0;								//Position in Lut_Volume that matches current volume set in DSP
	static uint8_t currentvolume = 0;							//Lut_Volume position we are currently using during fading actions
	uint8_t stepsize = 4;										//How fast are we going through the look up table?
	uint32_t temp_volume = 0;
	uint8_t i=0;
	uint8_t j=0;
	
	if (!mute_on_off)		//Lets unmute audio
	{
		for (i=1;i<31;i++)										//Do the fade ins
		{
			currentvolume = currentvolume - stepsize;			//Increase master volume by stepvolume over time
			
			if (currentvolume <= lut_start)						//Cap master volume to level it had before fade out was applied
			{
				temp_volume = lut_volume[lut_start];
				for (j=4;j>0;j--)								//Construct last 4 bytes of array volume[] to be send via I2C
				{
					volume[j] = temp_volume;					//Copy startvolume into one byte of array
					temp_volume = temp_volume >> 8;				//Shift start volume values around
				}
				//Send original master volume level to DSP
				TWI_MasterWriteRead(&twiDSP, TWI_SLAVE_DSP_W, &volume[0], 5,0);
				break;
			}
			else
			{
				temp_volume = lut_volume[currentvolume];
				for (j=4;j>0;j--)								//Construct last 4 bytes of array volume[] to be send via I2C
				{
					volume[j] = temp_volume;					//Copy current volume into one byte of array
					temp_volume = temp_volume >> 8;				//Shift current volume values around
				}
				//Send new master volume level to DSP
				TWI_MasterWriteRead(&twiDSP, TWI_SLAVE_DSP_W, &volume[0], 5,0);
			}
		}
	update_levels();		//And also restore the volume information and slider	
	} 
	
	
	else
	{
		TWI_MasterWriteRead(&twiDSP, TWI_SLAVE_DSP_W, &volume[0], 1,4);

		//Here we fade out the volume
		//Need to construct a 32 bit integer for volume level calculation from 4 single byte read via I2C from DSP
		startvolume = twiDSP.readData[0];						//Copy first byte into 32 bit variable startvolume
		for (i=1;i<4;i++)										//Repeat next steps 3 times starting at i=1 because we need already second bit from DSP
			{
				startvolume = startvolume << 8;						//Bit shift by 8 to move already stored information a whole byte to the left
				startvolume |= twiDSP.readData[i];					//Add via or next byte of DSP volume info to 32 bit variable startvolume
			}
			
		for (lut_start=0;lut_start<115;lut_start++)
		{
			if(startvolume == lut_volume[lut_start])
			{
				currentvolume = lut_start;
				break;
			}
		}
				
		for (i=1;i<31;i++)										//Do the fade outs within a maximum of 30 steps
		{
			currentvolume = currentvolume + stepsize;			//Reduce volume via stepping through look up table with step size (4*30 = 120)
				
			if (currentvolume >= 114 - stepsize)				//Ensure that step size won't result in negative value
			{
				uint8_t volume[5] = {volreg,0x00,0x00,0x00,0x1A};	//Cap lowest master volume to -110 dB
				
				//Set lowest master volume level to DSP
				TWI_MasterWriteRead(&twiDSP, TWI_SLAVE_DSP_W, &volume[0], 5,0);
					
				break;
			}
			else
			{
				temp_volume = lut_volume[currentvolume];
				for (j=4;j>0;j--)								//Construct last 4 bytes of array volume[] to be send via I2C to DSP
				{
					volume[j] = temp_volume;					//Copy 8 Bits of 32 bit value currentvolume into 1 byte of I2C transmission array
					temp_volume = temp_volume >> 8;				//Shift by 8 to move next byte onto correct place for byte copy
				}
					
				//Send new master volume level to DSP
				TWI_MasterWriteRead(&twiDSP, TWI_SLAVE_DSP_W, &volume[0], 5,0);
			}
		}
	clear_area(2,145,90,165);									//Clear all level information before writing new values
	show_mute();												//write "Muted" as level information while mute is on
	}
}

void volumeup()
{
	uint8_t volume[5] = {volreg,0x00,0x00,0x00,0x00};			//PPS mapped address for master volume control
	static uint32_t startvolume = 0;							//The current volume set in the DSP
	static uint8_t lut_start = 0;								//Position in Lut_Volume that matches current volume set in DSP
	static uint8_t currentvolume = 0;							//Lut_Volume position we are currently using during fading actions
	uint32_t temp_volume = 0;
	uint8_t i=0;
	uint8_t j=0;
	
	
	TWI_MasterWriteRead(&twiDSP, TWI_SLAVE_DSP_W, &volume[0], 1,4);
			
			
	//Get to know where from which volume we are starting
	//Need to construct a 32 bit integer for volume level calculation from 4 single byte read via I2C from DSP
	startvolume = twiDSP.readData[0];						//Copy first byte into 32 bit variable startvolume
	for (i=1;i<4;i++)										//Repeat next steps 3 times starting at i=1 because we need already second bit from DSP
	{
		startvolume = startvolume << 8;						//Bit shift by 8 to move already stored information a whole byte to the left
		startvolume |= twiDSP.readData[i];					//Add via or next byte of DSP volume info to 32 bit variable startvolume
	}
			
	for (lut_start=0;lut_start<115;lut_start++)
	{
		if(startvolume == lut_volume[lut_start])
		{
		currentvolume = lut_start;
		break;	
		}
	}
		
		
	while(currentvolume > 0)
	{
		currentvolume = currentvolume - 1;
		
		temp_volume = lut_volume[currentvolume];
		for (j=5;j>0;j--)								//Construct last 4 bytes of array volume[] to be send via I2C to DSP
		{
			volume[j-1] = temp_volume;					//Copy 8 Bits of 32 bit value currentvolume into 1 byte of I2C transmission array
			temp_volume = temp_volume >> 8;				//Shift by 8 to move next byte onto correct place for byte copy
		}
		volume[0] = volreg;
					
		//Send new master volume level to DSP
		TWI_MasterWriteRead(&twiDSP, TWI_SLAVE_DSP_W, &volume[0], 5,0);
					
		
		update_levels();
		_delay_ms(500);
		
		if (!(PORTD.IN & (1<<6)))										//wait for SBUF signal from display to get low
		{
			break;
		}
		
	}
}


void volumedown()
{
	
	uint8_t volume[5] = {volreg,0x00,0x00,0x00,0x00};			//PPS mapped address for master volume control
	static uint32_t startvolume = 0;							//The current volume set in the DSP
	static uint8_t lut_start = 0;								//Position in Lut_Volume that matches current volume set in DSP
	static uint8_t currentvolume = 0;							//Lut_Volume position we are currently using during fading actions
	uint32_t temp_volume = 0;
	uint8_t i=0;
	uint8_t j=0;
	
	
	TWI_MasterWriteRead(&twiDSP, TWI_SLAVE_DSP_W, &volume[0], 1,4);

	
	//Get to know where from which volume we are starting
	//Need to construct a 32 bit integer for volume level calculation from 4 single byte read via I2C from DSP
	startvolume = twiDSP.readData[0];						//Copy first byte into 32 bit variable startvolume
	for (i=1;i<4;i++)										//Repeat next steps 3 times starting at i=1 because we need already second bit from DSP
	{
		startvolume = startvolume << 8;						//Bit shift by 8 to move already stored information a whole byte to the left
		startvolume |= twiDSP.readData[i];					//Add via or next byte of DSP volume info to 32 bit variable startvolume
	}
	
	for (lut_start=0;lut_start<115;lut_start++)
	{
		if(startvolume == lut_volume[lut_start])
			{
				currentvolume = lut_start;
				break;
			}

	}
	
	
	
    while(currentvolume < 114)
    {
	    currentvolume = currentvolume + 1;
	    
	    temp_volume = lut_volume[currentvolume];
	    for (j=5;j>0;j--)								//Construct last 4 bytes of array volume[] to be send via I2C to DSP
	    {
		    volume[j-1] = temp_volume;					//Copy 8 Bits of 32 bit value currentvolume into 1 byte of I2C transmission array
		    temp_volume = temp_volume >> 8;				//Shift by 8 to move next byte onto correct place for byte copy
	    }
	    volume[0] = volreg;
		
	    //Send new master volume level to DSP
	    TWI_MasterWriteRead(&twiDSP, TWI_SLAVE_DSP_W, &volume[0], 5,0);
		
		update_levels();
		_delay_ms(500);
		
		if (!(PORTD.IN & (1<<6)))										//wait for SBUF signal from display to get low
		{
			break;
		}
    }
}