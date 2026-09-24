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
* Module: twi_master_driver.c
* Based on library by Atmel for ATxMega TWI Communication
* Added functions to communicate with eDIP320-8 display via I2C
*
*/

#include "twi_master_driver.h"


/*! \brief Initialize the TWI module.
 *
 *  TWI module initialization function.
 *  Enables master read and write interrupts.
 *  Remember to enable interrupts globally from the main application.
 *
 *  \param twi                      The TWI_Master_t struct instance.
 *  \param module                   The TWI module to use.
 *  \param intLevel                 Master interrupt level.
 *  \param baudRateRegisterSetting  The baud rate register value.
 */
void TWI_MasterInit(TWI_Master_t *twi, TWI_t *module, TWI_MASTER_INTLVL_t intLevel, uint8_t baudRateRegisterSetting)
{
	twi->interface = module;
	twi->interface->MASTER.CTRLA = intLevel | TWI_MASTER_RIEN_bm | TWI_MASTER_WIEN_bm | TWI_MASTER_ENABLE_bm;
	twi->interface->MASTER.BAUD = baudRateRegisterSetting;
	twi->interface->MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
}


/*! \brief Returns the TWI bus state.
 *
 *  Returns the TWI bus state (type defined in device headerfile),
 *  unknown, idle, owner or busy.
 *
 *  \param twi The TWI_Master_t struct instance.
 *
 *  \retval TWI_MASTER_BUSSTATE_UNKNOWN_gc Bus state is unknown.
 *  \retval TWI_MASTER_BUSSTATE_IDLE_gc    Bus state is idle.
 *  \retval TWI_MASTER_BUSSTATE_OWNER_gc   Bus state is owned by the master.
 *  \retval TWI_MASTER_BUSSTATE_BUSY_gc    Bus state is busy.
 */
TWI_MASTER_BUSSTATE_t TWI_MasterState(TWI_Master_t *twi)
{
	TWI_MASTER_BUSSTATE_t twi_status;
	twi_status = (TWI_MASTER_BUSSTATE_t) (twi->interface->MASTER.STATUS & TWI_MASTER_BUSSTATE_gm);
	return twi_status;
}


/*! \brief Returns true if transaction is ready.
 *
 *  This function returns a boolean whether the TWI Master is ready
 *  for a new transaction.
 *
 *  \param twi The TWI_Master_t struct instance.
 *
 *  \retval true  If transaction could be started.
 *  \retval false If transaction could not be started.
 */
bool TWI_MasterReady(TWI_Master_t *twi)
{
	bool twi_status = (twi->status & TWIM_STATUS_READY);
	return twi_status;
}


/*! \brief TWI write transaction.
 *
 *  This function is TWI Master wrapper for a write-only transaction.
 *
 *  \param twi          The TWI_Master_t struct instance.
 *  \param address      Slave address.
 *  \param writeData    Pointer to data to write.
 *  \param bytesToWrite Number of data bytes to write.
 *
 *  \retval true  If transaction could be started.
 *  \retval false If transaction could not be started.
 */
bool TWI_MasterWrite(TWI_Master_t *twi, uint8_t address, uint8_t *writeData, uint8_t bytesToWrite)
{
	bool twi_status = TWI_MasterWriteRead(twi, address, writeData, bytesToWrite, 0);
	return twi_status;
}


/*! \brief TWI read transaction.
 *
 *  This function is a TWI Maste wrapper for read-only transaction.
 *
 *  \param twi            The TWI_Master_t struct instance.
 *  \param address        The slave address.
 *  \param bytesToRead    The number of bytes to read.
 *
 *  \retval true  If transaction could be started.
 *  \retval false If transaction could not be started.
 */
bool TWI_MasterRead(TWI_Master_t *twi, uint8_t address, uint8_t bytesToRead)
{
	bool twi_status = TWI_MasterWriteRead(twi, address, 0, 0, bytesToRead);
	return twi_status;
}


/*! \brief TWI write and/or read transaction.
 *
 *  This function is a TWI Master write and/or read transaction. The function
 *  can be used to both write and/or read bytes to/from the TWI Slave in one
 *  transaction.
 *
 *  \param twi            The TWI_Master_t struct instance.
 *  \param address        The slave address.
 *  \param writeData      Pointer to data to write.
 *  \param bytesToWrite   Number of bytes to write.
 *  \param bytesToRead    Number of bytes to read.
 *
 *  \retval true  If transaction could be started.
 *  \retval false If transaction could not be started.
 */
bool TWI_MasterWriteRead(TWI_Master_t *twi, uint8_t address, uint8_t *writeData, uint8_t bytesToWrite, uint8_t bytesToRead)
{
	/*Parameter sanity check. */
	if (bytesToWrite > TWIM_WRITE_BUFFER_SIZE) {
		return false;
	}
	if (bytesToRead > TWIM_READ_BUFFER_SIZE) {
		return false;
	}

	/*Initiate transaction if bus is ready. */
	if (twi->status == TWIM_STATUS_READY) {

		twi->status = TWIM_STATUS_BUSY;
		twi->result = TWIM_RESULT_UNKNOWN;

		twi->address = address;//<<1;

		/* Fill write data buffer. */
		for (uint8_t bufferIndex=0; bufferIndex < bytesToWrite; bufferIndex++) {
			twi->writeData[bufferIndex] = writeData[bufferIndex];
		}

		twi->bytesToWrite = bytesToWrite;
		twi->bytesToRead = bytesToRead;
		twi->bytesWritten = 0;
		twi->bytesRead = 0;

		/* If write command, send the START condition + Address +
		 * 'R/_W = 0'
		 */
		if (twi->bytesToWrite > 0) {
			uint8_t writeAddress = twi->address & ~0x01;
			twi->interface->MASTER.ADDR = writeAddress;
		}

		/* If read command, send the START condition + Address +
		 * 'R/_W = 1'
		 */
		else if (twi->bytesToRead > 0) {
			uint8_t readAddress = twi->address | 0x01;
			twi->interface->MASTER.ADDR = readAddress;
		}
		
		
		while (twi->status != TWIM_STATUS_READY)
		{
			/* Wait until transaction is complete. */
		}
		return true;
	} else {
		return false;
	}
}


/*! \brief Common TWI master interrupt service routine.
 *
 *  Check current status and calls the appropriate handler.
 *
 *  \param twi  The TWI_Master_t struct instance.
 */
void TWI_MasterInterruptHandler(TWI_Master_t *twi)
{
	uint8_t currentStatus = twi->interface->MASTER.STATUS;

	/* If arbitration lost or bus error. */
	if ((currentStatus & TWI_MASTER_ARBLOST_bm) ||
	    (currentStatus & TWI_MASTER_BUSERR_bm)) {

		TWI_MasterArbitrationLostBusErrorHandler(twi);
	}

	/* If master write interrupt. */
	else if (currentStatus & TWI_MASTER_WIF_bm) {
		TWI_MasterWriteHandler(twi);
	}

	/* If master read interrupt. */
	else if (currentStatus & TWI_MASTER_RIF_bm) {
		TWI_MasterReadHandler(twi);
	}

	/* If unexpected state. */
	else {
		TWI_MasterTransactionFinished(twi, TWIM_RESULT_FAIL);
	}
}


/*! \brief TWI master arbitration lost and bus error interrupt handler.
 *
 *  Handles TWI responses to lost arbitration and bus error.
 *
 *  \param twi  The TWI_Master_t struct instance.
 */
void TWI_MasterArbitrationLostBusErrorHandler(TWI_Master_t *twi)
{
	uint8_t currentStatus = twi->interface->MASTER.STATUS;

	/* If bus error. */
	if (currentStatus & TWI_MASTER_BUSERR_bm) {
		twi->result = TWIM_RESULT_BUS_ERROR;
	}
	/* If arbitration lost. */
	else {
		twi->result = TWIM_RESULT_ARBITRATION_LOST;
	}

	/* Clear interrupt flag. */
	twi->interface->MASTER.STATUS = currentStatus | TWI_MASTER_ARBLOST_bm;

	twi->status = TWIM_STATUS_READY;
}


/*! \brief TWI master write interrupt handler.
 *
 *  Handles TWI transactions (master write) and responses to (N)ACK.
 *
 *  \param twi The TWI_Master_t struct instance.
 */
void TWI_MasterWriteHandler(TWI_Master_t *twi)
{
	/* Local variables used in if tests to avoid compiler warning. */
	uint8_t bytesToWrite  = twi->bytesToWrite;
	uint8_t bytesToRead   = twi->bytesToRead;

	/* If NOT acknowledged (NACK) by slave cancel the transaction. */
	if (twi->interface->MASTER.STATUS & TWI_MASTER_RXACK_bm) {
		twi->interface->MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc;
		twi->result = TWIM_RESULT_NACK_RECEIVED;
		twi->status = TWIM_STATUS_READY;
	}

	/* If more bytes to write, send data. */
	else if (twi->bytesWritten < bytesToWrite) {
		uint8_t data = twi->writeData[twi->bytesWritten];
		twi->interface->MASTER.DATA = data;
		++twi->bytesWritten;
	}

	/* If bytes to read, send repeated START condition + Address +
	 * 'R/_W = 1'
	 */
	else if (twi->bytesRead < bytesToRead) {
		uint8_t readAddress = twi->address | 0x01;
		twi->interface->MASTER.ADDR = readAddress;
	}

	/* If transaction finished, send STOP condition and set RESULT OK. */
	else {
		twi->interface->MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc;
		TWI_MasterTransactionFinished(twi, TWIM_RESULT_OK);
	}
}


/*! \brief TWI master read interrupt handler.
 *
 *  This is the master read interrupt handler that takes care of
 *  reading bytes from the TWI slave.
 *
 *  \param twi The TWI_Master_t struct instance.
 */
void TWI_MasterReadHandler(TWI_Master_t *twi)
{
	/* Fetch data if bytes to be read. */
	if (twi->bytesRead < TWIM_READ_BUFFER_SIZE) {
		uint8_t data = twi->interface->MASTER.DATA;
		twi->readData[twi->bytesRead] = data;
		twi->bytesRead++;
	}

	/* If buffer overflow, issue STOP and BUFFER_OVERFLOW condition. */
	else {
		twi->interface->MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc;
		TWI_MasterTransactionFinished(twi, TWIM_RESULT_BUFFER_OVERFLOW);
	}

	/* Local variable used in if test to avoid compiler warning. */
	uint8_t bytesToRead = twi->bytesToRead;

	/* If more bytes to read, issue ACK and start a byte read. */
	if (twi->bytesRead < bytesToRead) {
		twi->interface->MASTER.CTRLC = TWI_MASTER_CMD_RECVTRANS_gc;
	}

	/* If transaction finished, issue NACK and STOP condition. */
	else {
		twi->interface->MASTER.CTRLC = TWI_MASTER_ACKACT_bm |
		                               TWI_MASTER_CMD_STOP_gc;
		TWI_MasterTransactionFinished(twi, TWIM_RESULT_OK);
	}
}


/*! \brief TWI transaction finished handler.
 *
 *  Prepares module for new transaction.
 *
 *  \param twi     The TWI_Master_t struct instance.
 *  \param result  The result of the operation.
 */
void TWI_MasterTransactionFinished(TWI_Master_t *twi, uint8_t result)
{
	twi->result = result;
	twi->status = TWIM_STATUS_READY;
}



/*============================================================================
Function: 	send_Command
Input:		uchar * data, uchar length
Output:		Bool (true if ACK received, false if transfer failed
Discription:send commands and data to eDIP with SmallProtocoll
============================================================================*/
_Bool send_Command (unsigned char * data, unsigned char length)
{
	unsigned char failure=0;
	unsigned char i=0;
	
	for (i=0;i<=RETRY;i++)
	{
		send_array_smallprot(data, length, FALSE, 1); //Send data
		failure=twiLCD.readData[0];  //check for Ack-Byte
		if (failure == 0x06)
			{
				return TRUE;
				break;
			}
	}
	return FALSE;
	
}



/*============================================================================
Function: 	send_array_smallprot
Input:		uchar * data, uchar length, uchar ptotocoll
Output:		---
Discription:Sends data to display, adding protocoll:
			DC1/DC2 | length | data..... | bcc
			<DC1> = 17(dec.) = 0x11 <DC2> = 18(dec.) = 0x12 
			length = count of user data (without <DC1/DC2>, without checksum bcc)
			bcc = 1 byte = sum of all bytes incl. <DC1/DC2> and len, modulo 256
			sends array, using uart_send(), length = data amount to be send
			protocoll = False (Command/Data to the display)
			protocoll = True (protocoll commands to the display)
============================================================================*/
void send_array_smallprot(unsigned char * data, unsigned char length, unsigned char protocoll, unsigned char readlength)
{
	unsigned char i,bcc=0;	//internal index variable and checksum
	
	uint8_t sendbuf[32];

	
	if(protocoll)				//protocoll commands to display
	{
		sendbuf[0]=0x12;		//send DC2
		bcc = 0x12;				
	}
	else						//Command/Data to the display
	{
		sendbuf[0]=0x11;		//send DC1
		bcc = 0x11;
	}
	
	bcc = bcc + length;			//Add length to checksum
	sendbuf[1] = length;		//Add length to send buffer
	
	for( i=0; i<length; i++)	
	{
		bcc = bcc + (*data);	//add to checksum 
		sendbuf[i+2] = (*data++); //add to send buffer and increment readpointer for next step
	}
	
	sendbuf[length+2] = bcc;				//add checksum to last byte of sendbuffer
	
	TWI_MasterWriteRead(&twiLCD, TWI_SLAVE_LCD_W, &sendbuf[0], length+3,readlength);
	
}


/*============================================================================
Function: 	checkbuf
Input:		---
Output:		true if data available, 2 if no data is present, FAlSE if communication error
Description:checks if data is available in sendbuffer and calls it
============================================================================*/
unsigned char checkbuf (void)
{
	if(request_Buffer(FALSE)) //False to request buffer fill level
	{
		if(twiLCD.readData[3]>0) //Check Buffer fill level byte to be valide
		{
			return request_Buffer(TRUE); //True to request buffer content itself
		}
		else
			return 2;
	}
	else
		return FALSE;
}


/*============================================================================
Function: 	request_Buffer
Input:		---
Output:		
Description:Request buffer information or buffer content
============================================================================*/
unsigned char request_Buffer(unsigned char protocoll)
{
	if(protocoll)
	{
		
		//Clear and read the buffer according to fill level information gathered before + 4 stuff bits for ack, dc, lenght, bcc 
		send_array_smallprot("S",1,TRUE,twiLCD.readData[3]+4);
		//Decode bcc according to new fill level information, also using 4 stuff bits
		return receive_decode(twiLCD.readData[2]+4);
	}
	else
	{	
		//Here we read the buffer fill level information back, always 6 byte including ack and bcc
		send_array_smallprot("I",1,TRUE,6);
		//lets check the bcc value
		return receive_decode(6);
	}
	
}



/*============================================================================
Function: 	receive_decode
Input:		---
Output:		---
Discription:decode received data, check if display answered correctly via bcc
============================================================================*/
_Bool receive_decode(unsigned char length)
{
	unsigned char bcc=0,i=0;
	//start at [1] to ignore ack signal, also stop 1 before end of array where bcc itself is stored
	for(i=1; i<length-1; i++)
	{
		bcc+=twiLCD.readData[i];

	}
	//compare calculated bcc with last value in array
	if(bcc==twiLCD.readData[i])
	return TRUE;
	else
	return FALSE;
}