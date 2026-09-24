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
* Module: Mathe.h
* Typedefs for floating averaging calculation ring buffer and functions
*
*/


#ifndef MATHE_H_
#define MATHE_H_

#include <stdio.h>

// For how many values the floating mean value will be calculated?
// Choose between 1.....255
#define SIZE_OF_AVG  5

// Data type for floating average calculation
typedef uint8_t tFloatAvgType;
// typedef float tFloatAvgType;

// Only needed for mean value calculation
// Must be able to store values that SIZE_OF_AVG times bigger than tFloatAvgType are
typedef uint16_t tTempSumType;
// typedef float tTempSumType;

// The ring buffer structure
typedef struct
{
	tFloatAvgType aData[SIZE_OF_AVG];
	uint8_t IndexNextValue;
} tFloatAvgFilter;


// Initialize buffer with start values
void InitFloatAvg(tFloatAvgFilter * io_pFloatAvgFilter, tFloatAvgType i_DefaultValue);

// Add new value to buffer
void AddToFloatAvg(tFloatAvgFilter * io_pFloatAvgFilter, tFloatAvgType i_ui8NewValue);

// Calculates the floating average value over the ring buffer content
tFloatAvgType GetOutputValue(tFloatAvgFilter * io_pFloatAvgFilter);

#endif /* MATHE_H_ */