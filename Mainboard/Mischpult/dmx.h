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
* Module: DMX.h
* Declarations for DMX processing
*
*/

#ifndef DMX_H_
#define DMX_H_

#include "Mathe.h"

#define dmxchannels 512
#define flash_blue_ch 2//99
#define flash_red_ch 300
#define flash_green_ch 301

void dma_set(unsigned char *dmxdata, unsigned int length);
void start_dmx(unsigned char *dmxdata, unsigned int length);
void dma_bytecopy(unsigned char *dmxsource, unsigned char *dmxtarget, unsigned int length);

void dmx_internal(void);
void dmx_external(void);
void dmx_s2l(void);
void dmx_worklight(void);

void flash_white(void);
void flash_white_linked(void);
void flash_white_overall(void);

void flash_puke(void);
void flash_puke_linked(void);
void flash_puke_overall(void);

void flash_color(void);
void flash_color_linked(void);
void flash_color_overall(void);

void solid_color(void);
void solid_color_linked(void);
void solid_color_overall(void);

void s2l_output(void);

void level_brightness(unsigned char slider);
void level_red(unsigned char slider);
void level_blue(unsigned char slider);
void level_green(unsigned char slider);
void frequency(unsigned char slider);

void switchdmxinput(unsigned char source);

//Enumeration for DMX transmission frame status
enum{BREAK, STARTBYTE, DMXDATA, FIRSTBREAK};
unsigned char DMXStatus;

//Enumeration for DMX signal routing via SP3T switches
enum{PA_INTERNAL,PA_EXTERNAL,PA_USB,PA_S2L,STRB_INTERNAL,STRB_EXTERNAL,STRB_USB};
extern unsigned char global_pa_source;
extern unsigned char global_strb_source;

extern unsigned char global_brightness;
extern unsigned char global_red;
extern unsigned char global_blue;
extern unsigned char global_green;
extern unsigned char global_frequency;	

extern _Bool toogle_red;
extern _Bool toogle_green;
extern _Bool toogle_blue;
extern _Bool toogle_solid;
extern _Bool toogle_worklight;
extern _Bool toogle_s2l;

extern volatile _Bool DMX_data_valid;
extern volatile _Bool DMA_complete;

extern volatile unsigned char dmx_buffer[512];
extern unsigned char dmx_output[512];

extern tFloatAvgFilter FilterS2LHighValues;
extern tFloatAvgFilter FilterS2LMidValues;
extern tFloatAvgFilter FilterS2LLowValues;

#endif /* DMX_H_ */