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
* Module: twi_master_driver.h
* Based on library by Atmel for ATxMega TWI Communication
* Added functions to communicate with eDIP320-8 display via I2C
* Defines I2C addresses of Display and DSP
*
*/

#ifndef TWI_MASTER_DRIVER_H
#define TWI_MASTER_DRIVER_H

#include "avr_compiler.h"

/*! Define adress of TWI slave */
#define TWI_SLAVE_LCD_W 0xDE
#define TWI_SLAVE_LCD_R 0xDF
#define TWI_SLAVE_DSP_W 0x68
#define TWI_SLAVE_DSP_R 0x69

/*! Baud register setting calculation. Formula described in datasheet. */
#define CPU_SPEED 32000000UL
#define BAUDRATE 100000
#define TWI_BAUD(F_SYS, F_TWI) ((F_SYS / (2 * F_TWI)) - 5)
#define TWI_BAUDSETTING TWI_BAUD(CPU_SPEED, BAUDRATE)

/*! General defines. */
#define FALSE 0
#define TRUE 1
#define RETRY 3

/*! Transaction status defines. */
#define TWIM_STATUS_READY              0
#define TWIM_STATUS_BUSY               1

/*! Global Variables. */
unsigned char failurecounter; //counts corrupt packages


/*! Transaction result enumeration. */
typedef enum TWIM_RESULT_enum {
	TWIM_RESULT_UNKNOWN          = (0x00<<0),
	TWIM_RESULT_OK               = (0x01<<0),
	TWIM_RESULT_BUFFER_OVERFLOW  = (0x02<<0),
	TWIM_RESULT_ARBITRATION_LOST = (0x03<<0),
	TWIM_RESULT_BUS_ERROR        = (0x04<<0),
	TWIM_RESULT_NACK_RECEIVED    = (0x05<<0),
	TWIM_RESULT_FAIL             = (0x06<<0),
} TWIM_RESULT_t;

/*! Buffer size defines */
#define TWIM_WRITE_BUFFER_SIZE         32
#define TWIM_READ_BUFFER_SIZE          32


/*! \brief TWI master driver struct
 *
 *  TWI master struct. Holds pointer to TWI module,
 *  buffers and necessary varibles.
 */
typedef struct TWI_Master {
	TWI_t *interface;								/*!< Pointer to what interface to use */
	register8_t address;                            /*!< Slave address */
	register8_t writeData[TWIM_WRITE_BUFFER_SIZE];  /*!< Data to write */
	register8_t readData[TWIM_READ_BUFFER_SIZE];    /*!< Read data */
	register8_t bytesToWrite;                       /*!< Number of bytes to write */
	register8_t bytesToRead;                        /*!< Number of bytes to read */
	register8_t bytesWritten;                       /*!< Number of bytes written */
	register8_t bytesRead;                          /*!< Number of bytes read */
	register8_t status;                             /*!< Status of transaction */
	register8_t result;                             /*!< Result of transaction */
}TWI_Master_t;

/* Global variables */
TWI_Master_t twiLCD;
TWI_Master_t twiDSP;


void TWI_MasterInit(TWI_Master_t *twi, TWI_t *module, TWI_MASTER_INTLVL_t intLevel, uint8_t baudRateRegisterSetting);
TWI_MASTER_BUSSTATE_t TWI_MasterState(TWI_Master_t *twi);
bool TWI_MasterReady(TWI_Master_t *twi);
bool TWI_MasterWrite(TWI_Master_t *twi, uint8_t address, uint8_t * writeData, uint8_t bytesToWrite);
bool TWI_MasterRead(TWI_Master_t *twi, uint8_t address, uint8_t bytesToRead);
bool TWI_MasterWriteRead(TWI_Master_t *twi, uint8_t address, uint8_t *writeData, uint8_t bytesToWrite, uint8_t bytesToRead);
void TWI_MasterInterruptHandler(TWI_Master_t *twi);
void TWI_MasterArbitrationLostBusErrorHandler(TWI_Master_t *twi);
void TWI_MasterWriteHandler(TWI_Master_t *twi);
void TWI_MasterReadHandler(TWI_Master_t *twi);
void TWI_MasterTransactionFinished(TWI_Master_t *twi, uint8_t result);

_Bool send_Command (unsigned char * data, unsigned char length);
void send_array_smallprot(unsigned char * data, unsigned char length, unsigned char protocoll, unsigned char readlength);
unsigned char checkbuf (void);
unsigned char request_Buffer(unsigned char protocoll);
_Bool receive_decode(unsigned char lenght);


/* Functions */
#define CheckComm(var1)		if(!var1){PORTD.OUTSET = PIN5_bm;failurecounter=0;}else{PORTD.OUTCLR = PIN5_bm;}


#endif /* TWI_MASTER_DRIVER_H */
