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
* Module: UART.c
* Configurations of ATxMega UART Modules
*/

#include <stdlib.h>
#include <avr/io.h>


void UsartC0_init(void){
	
	//configure USARTC0 for DMX output
	// Pin from USARTC0 TxD C3 set to output
	PORTC.DIRSET = PIN3_bm;
	// Pin from USARTC0 TxD C3 to high
	PORTC.OUTSET = PIN3_bm;
	// Asynchronous Modus
	USARTC0.CTRLC = (USARTC0.CTRLC & ~USART_CMODE_gm ) | USART_CMODE_ASYNCHRONOUS_gc;
	// No parity
	USARTC0.CTRLC = (USARTC0.CTRLC & ~USART_PMODE_gm ) | USART_PMODE_DISABLED_gc;
	// 8 data bits
	USARTC0.CTRLC = (USARTC0.CTRLC & ~USART_CHSIZE_gm ) | USART_CHSIZE_8BIT_gc;
	// 2 stop bits
	USARTC0.CTRLC = (USARTC0.CTRLC & ~USART_SBMODE_bm) | (0x01<<3);
	// 100 kBaud
	USARTC0.BAUDCTRLA = 19;
	// 100 kBaud
	USARTC0.BAUDCTRLB =(0 << USART_BSCALE0_bp)|(19 >> 8);
	// activate USARTC1
	USARTC0.CTRLB = USART_TXEN_bm;
}

void UsartC1_init(void){
	
	//configure USART C1 for DMX reception from external sources
	// Pin from USARTC1 RxD C6 set to input
	PORTC.DIRCLR = PIN6_bm;
	// Asynchronous Modus
	USARTC1.CTRLC = (USARTC0.CTRLC & ~USART_CMODE_gm ) | USART_CMODE_ASYNCHRONOUS_gc;
	// No parity
	USARTC1.CTRLC = (USARTC0.CTRLC & ~USART_PMODE_gm ) | USART_PMODE_DISABLED_gc;
	// 8 data bits
	USARTC1.CTRLC = (USARTC0.CTRLC & ~USART_CHSIZE_gm ) | USART_CHSIZE_8BIT_gc;
	// 2 stop bits
	USARTC1.CTRLC = (USARTC0.CTRLC & ~USART_SBMODE_bm) | (0x01<<3);
	// 250 kBaud
	USARTC1.BAUDCTRLA = 07;
	// 250 kBaud
	USARTC1.BAUDCTRLB =(0 << USART_BSCALE0_bp)|(07 >> 8);
	// Set interrupt level of RX Int to low
	USARTC1.CTRLA = (USARTC1.CTRLA & ~USART_RXCINTLVL_gm) | USART_RXCINTLVL_MED_gc;
	// activate USARTC1
	USARTC1.CTRLB = USART_RXEN_bm;
	
}