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
* Module: Mathe.c
* Functions for calculation of floating averaging values of a ring buffer
*
*/

#include "Mathe.h"
#include <stdlib.h>
#include <avr/io.h>

void InitFloatAvg(tFloatAvgFilter * io_pFloatAvgFilter, tFloatAvgType i_DefaultValue)
{
	// Fill the buffer with initial values:
	for (uint8_t i = 0; i < SIZE_OF_AVG; ++i)
	{
		io_pFloatAvgFilter->aData[i] = i_DefaultValue;
	}
	// The next value shall be written at the start of the buffer:
	io_pFloatAvgFilter->IndexNextValue = 0;
}


void AddToFloatAvg(tFloatAvgFilter * io_pFloatAvgFilter, tFloatAvgType i_NewValue)
{
	// Put new value to specific position in buffer
	io_pFloatAvgFilter->aData[io_pFloatAvgFilter->IndexNextValue] =
	i_NewValue;
	// Increase specific position for next value 
	io_pFloatAvgFilter->IndexNextValue++;
	// When specific position is at end of buffer restart ring buffer
	io_pFloatAvgFilter->IndexNextValue %= SIZE_OF_AVG;
}


tFloatAvgType GetOutputValue(tFloatAvgFilter * io_pFloatAvgFilter)
{
	tTempSumType TempSum = 0;
	// calculate floating mean value
	for (uint8_t i = 0; i < SIZE_OF_AVG; ++i)
	{
		TempSum += io_pFloatAvgFilter->aData[i];
	}
	// Cast is ok if tFloatAvgType and tTempSumType where choosen correctly	
	tFloatAvgType o_Result = (tFloatAvgType) (TempSum / SIZE_OF_AVG);
	return o_Result;
}