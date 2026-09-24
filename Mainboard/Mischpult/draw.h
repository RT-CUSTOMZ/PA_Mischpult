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
* Module: draw.h
* Functions for drawing display content
* Defines global variables to store information on selected menu items
*
*/



#ifndef DRAW_H_
#define DRAW_H_

#include <avr/io.h>

_Bool draw_basic_frame(uint8_t menu);
_Bool draw_audio_menu(void);
_Bool draw_dmx_menu(void);
_Bool draw_setup_menu(void);
_Bool showlevel(uint8_t level);
_Bool update_levels(void);
_Bool show_mute(void);
_Bool clear_area(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

extern unsigned char pushed_audio;			//Global variable to remember which button for source selection was pressed
extern unsigned char pushed_dmx;			//Global variable to remember which button for source selection was pressed
extern unsigned char selected_menu;			//Global variable to remember which menu was selected last
extern unsigned char mute_selected;			//Global variable to remember that mute was selected

extern unsigned char selected_red;
extern unsigned char selected_green;
extern unsigned char selected_blue;
extern unsigned char selected_solid_color;
extern unsigned char selected_work_light;
extern unsigned char selected_linked;

#endif /* DRAW_H_ */