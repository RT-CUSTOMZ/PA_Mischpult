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
* Module: draw.c
* Used for defining and displaying content on screen
* Uses drawing functions from display.h 
*
*/

#include "draw.h"
#include "display.h"
#include "dsp.h"
#include "dmx.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

unsigned char  pushed_audio = 191;			//Remember which Audio Source was selected
unsigned char  pushed_dmx = 210;			//Remember which DMX Source was selected
unsigned char  selected_menu = 0;			//Also need to remember which Menu we are displaying
unsigned char  mute_selected = 0;			//Especially remember if audio was muted

unsigned char selected_red;					//Store if Red color button is pressed
unsigned char selected_green;				//Store if Green color button is pressed
unsigned char selected_blue;				//Store if Blue color button is pressed
unsigned char selected_solid_color;			//Store if Solid color button is pressed
unsigned char selected_work_light;			//Store if Work Light button is pressed
unsigned char selected_linked;				//Store if Linked function button is pressed


const char *lut_pegel[115] =				//A LUT for the display of the dB Level of our Audio Signal
{	" 24",
	" 23",
	" 22",
	" 21",
	" 20",
	" 19",
	" 18",
	" 17",
	" 16",
	" 15",
	" 14",
	" 13",
	" 12",
	" 11",
	" 10",
	"  9",
	"  8",
	"  7",
	"  6",
	"  5",
	"  4",
	"  3",
	"  2",
	"  1",			//+ dB Values
	"  0",			//0dB
	"  1",			//- dB Values
	"  2",
	"  3",
	"  4",
	"  5",
	"  6",
	"  7",
	"  8",
	"  9",
	" 10",
	" 11",
	" 12",
	" 13",
	" 14",
	" 15",
	" 16",
	" 17",
	" 18",
	" 19",
	" 20",
	" 21",
	" 22",
	" 23",
	" 24",
	" 25",
	" 26",
	" 27",
	" 28",
	" 29",
	" 30",
	" 31",
	" 32",
	" 33",
	" 34",
	" 35",
	" 36",
	" 37",
	" 38",
	" 39",
	" 40",
	" 41",
	" 42",
	" 43",
	" 44",
	" 45",
	" 46",
	" 47",
	" 48",
	" 49",
	" 50",
	" 51",
	" 52",
	" 53",
	" 54",
	" 55",
	" 56",
	" 57",
	" 58",
	" 59",
	" 60",
	" 61",
	" 62",
	" 63",
	" 64",
	" 65",
	" 66",
	" 67",
	" 68",
	" 69",
	" 70",
	" 72",
	" 74",
	" 76",
	" 78",
	" 80",
	" 82",
	" 84",
	" 86",
	" 88",
	" 90",
	" 92",
	" 94",
	" 96",
	" 98",
	"100",
	"102",
	"104",
	"106",
	"108",
	"110"
};


_Bool draw_basic_frame(uint8_t menu)
{
	eDIP_TA();
	eDIP_DL();
	eDIP_RR(0,0,279,240,15);
	eDIP_AL(0,1);
	eDIP_AR(1);
	eDIP_AE(15,0);
	eDIP_AJ(278,10,14,200,203,"",0);
	eDIP_AJ(278,85,15,201,204,"",0);
	eDIP_AJ(278,160,16,202,205,"",0);
	eDIP_AR(0);
	eDIP_AP(menu,1);
	
	
	return 1;
}



_Bool draw_audio_menu(void)
{
	uint8_t i=0;
	uint16_t x1=120;
	uint8_t y1=5;
	uint8_t h=90;
	uint8_t w=8;
	uint8_t s=3;
	uint8_t mastervolume = 0;
	
	draw_basic_frame(200);
	eDIP_UI(0,170,17);							//Draw Select Source Bar Graphic
	eDIP_UI(115,100,18);						//Draw Spectrum Bar Graphic
	eDIP_UI(0,130,21);							//Draw Volume Bar Graphic
	eDIP_AR(2);									//Button Group 2
	eDIP_AE(9,0);								//Touch Button Border
	eDIP_AJ(5,185,8,190,195,"",0);				//Digital Input Button Graphic
	eDIP_AJ(60,185,9,191,196,"",0);				//USB Sound Button Graphic
	eDIP_AJ(115,185,10,192,197,"",0);			//XLR Audio Button Graphic
	eDIP_AJ(170,185,11,193,198,"",0);			//Analog Input Button Graphic
	eDIP_AJ(225,185,12,194,199,"",0);			//Blue tooth Button Graphic
	eDIP_AR(0);									//End Button Group 2

	eDIP_AP(pushed_audio,1);					//Invert selected audio source
	
	eDIP_UI(5,5,19);							//Draw Master Volume Bar Graphic 
	eDIP_BO(20,17,5,47,124,0,115,1,1);			//Define bar graph "master volume" range 0 to 115 to match 115 values of master volume look up table
	eDIP_AB(20);								//Draw bar graph for master volume as touch
	
	if (mute_selected != 0)
	{
		clear_area(2,145,90,165);				//Clear all level information before writing new values
		show_mute();							//write "Muted" as level information while mute is on
	}
	else
	{
		mastervolume = readmastervolume();
		eDIP_BA(20,mastervolume);				//Update bar graph with current master volume
		showlevel(mastervolume);	
	}
	_delay_ms(200);								//Wait for Display to clear internal buffe
	
	for (i=1;i<15;i++)
	{
		eDIP_BO(i,x1,y1,x1+w,y1+h,0,200,0,1);	//Define bar graphs for visualizer
		x1=x1+w+s;
		eDIP_BA(i,0);							//Draw bar graphs for visualizer
		_delay_ms(5);
	}
	
	
	eDIP_UI(54,5,20);							//Draw VU Meter Bar Graphic
	eDIP_BO(21,66,5,88,124,0,190,1,1);			//VU Meter Bar Left Channel
	eDIP_BO(22,91,5,113,124,0,190,1,1);			//VU Meter Bar Right Channel
	eDIP_BA(21,100);
	eDIP_BA(22,100);
	
	eDIP_AJ(225, 115, 13, 180, 185, "", 0);		//Mute Button
		if (mute_selected != 0)					//Invert the selected source when redraw of this menu happens
		{
			eDIP_AP(mute_selected,1);
		}
	
	
	eDIP_AU(170,115,22,181,186,"",0);			//Volume UP Button
	eDIP_AU(115,115,23,182,187,"",0);			//Volume Down Button
	
	selected_menu = 1;							//Store selected Menu information for Audio Tab
	
	return 1;
}


_Bool draw_dmx_menu(void)
{
	draw_basic_frame(201);
		eDIP_UI(0,170,17);						//Draw Select Source Bar Graphic
		//eDIP_UI(115,0,31);					//Draw Strobes Bar Graphic
		eDIP_UI(0,0,30);						//Draw Master Bar Graphic
		eDIP_AR(3);								//Button Group 2
		eDIP_AE(9,0);							//Touch Button Border
		eDIP_AJ(5,185,24,210,215,"",0);			//Internal USB Button Graphic
		eDIP_AJ(60,185,25,211,216,"",0);		//External DMX Button Graphic
		eDIP_AJ(115,185,26,212,217,"",0);		//Sound 2 Light Button Graphic
		eDIP_AJ(225,185,27,214,219,"",0);		//Work Light Button Graphic
		eDIP_AR(0);								//End Button Group 2
		eDIP_AJ(170,185,39,232,233,"",0);		//Analog Input Button Graphic
		
		eDIP_AP(pushed_dmx,1);					//Invert active DMX source

		if(selected_linked){
			 eDIP_AP(232,1);		//Invert linked function if selected
		}
			
		eDIP_UI(5,20,28);						//Draw Brightness Bar Graphic
		eDIP_BO(30,19,20,49,165,0,127,1,1);		//Define bar graph "brightness" range 0 to 255 to match DMX range
		eDIP_AB(30);							//Draw bar graph for brightness as touch
		eDIP_BA(30,global_brightness);			//Update bar graph with current red level
		
		eDIP_UI(60,20,29);						//Draw Frequency Bar Graphic
		eDIP_BO(31,75,20,105,165,0,127,1,1);	//Define bar graph "frequency" range 0 to 255 to match DMX range
		eDIP_AB(31);							//Draw bar graph for frequency as touch
		eDIP_BA(31,global_frequency);			//Update bar graph with current red level
		
		eDIP_AU(225,115,33,215,220,"",0);		//Flash Button
		eDIP_AU(170,115,34,216,221,"",0);		//Puke Button
		eDIP_AU(115,115,32,217,222,"",0);		//Strobe Button
				
		_delay_ms(200);							//Wait for Display to clear internal buffer 
		
		
		eDIP_AJ(115,20,36,223,227,"",0);		//R Button
			if (selected_red != 0)				//Invert the red color button when redraw of this menu happens
			{
				eDIP_AP(selected_red,1);
			}
		eDIP_AJ(115,50,37,224,228,"",0);		//G Button
			if (selected_green != 0)			//Invert the green color button when redraw of this menu happens
			{
				eDIP_AP(selected_green,1);
			}
		eDIP_AJ(115,80,38,225,229,"",0);		//B Button
			if (selected_blue != 0)				//Invert the blue color button when redraw of this menu happens
			{
				eDIP_AP(selected_blue,1);
			}
		eDIP_AJ(236,20,35,226,230,"",0);		//Solid Color Button
			if (selected_solid_color != 0)		//Invert the solid color button when redraw of this menu happens
			{
				eDIP_AP(selected_solid_color,1);
			}
		
		eDIP_BR(40,145,20,230,44,0,127,1,1);	//Define bar graph Red range 0 to 255 to match DMX range
		eDIP_AB(40);							//Draw bar graph for red as touch
		eDIP_BA(40,global_red);					//Update bar graph with current red level
				
		eDIP_BR(41,145,50,230,74,0,127,1,1);	//Define bar graph Green range 0 to 255 to match DMX range
		eDIP_AB(41);							//Draw bar graph for Green as touch
		eDIP_BA(41,global_green);				//Update bar graph with current green level
						
		eDIP_BR(42,145,80,230,104,0,127,1,1);	//Define bar graph Blue range 0 to 255 to match DMX range
		eDIP_AB(42);							//Draw bar graph for Blue as touch
		eDIP_BA(42,global_blue);				//Update bar graph with current blue level
	
	selected_menu = 2;
	
	return 1;
}


_Bool draw_setup_menu(void)
{
	draw_basic_frame(202);
	eDIP_AR(4);
	eDIP_AE(9,0);
	eDIP_AK(10,180,60,230,210,215,"Audio",5);
	eDIP_AK(70,180,120,230,211,216,"DMX",3);
	eDIP_AK(130,180,180,230,212,217,"System",6);
	eDIP_AK(190,180,240,230,213,218,"Info",4);
	eDIP_AR(0);
	
	selected_menu = 3;
	
	return 1;
}


//Show level value in dB underneath Master Volume
_Bool showlevel(uint8_t level)
{
	if (level > 91)									//All values over 91 are positive dB values
	{
		eDIP_ZL(10,150,"+",1);						//Write a "+" to the screen
		eDIP_ZL(20,150,lut_pegel[115-level],3);		//Look up dB value based on volume level index
		eDIP_ZL(45,150,"dB",2);						//Write dB after the numeric value for OCD reasons
	} 
	else if (level < 91)							//All values over 91 are negative dB values
	{
		eDIP_ZL(10,150,"-",1);
		eDIP_ZL(20,150,lut_pegel[115-level],3);
		eDIP_ZL(45,150,"dB",2);
	}
	else											//Position 90 of the LUT equals 0dB which does not need a sign
	{
		eDIP_ZL(10,150," ",1);
		eDIP_ZL(20,150,lut_pegel[115-level],3);
		eDIP_ZL(45,150,"dB",2);
	}
	return 1;
}

_Bool update_levels(void)
{
	uint8_t mastervolume = 0;
	
	mastervolume = readmastervolume();
	eDIP_BA(20,mastervolume);						//Update bar graph with current master volume
	clear_area(2,145,90,165);						//Clear all level information before writing new values
	showlevel(mastervolume);
	
	return 1;
}


//Displays the word "Muted" as long as audio source is muted
_Bool show_mute(void)
{
	eDIP_ZL(30,150,"Muted",5);
	
	return 1;
}

//Function to clear a specific area of the DSP 
_Bool clear_area(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
	eDIP_RL(x1,y1,x2,y2);
	
	return 1;
}