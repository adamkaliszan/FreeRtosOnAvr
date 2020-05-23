#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "FreeRTOS.h"
#include "semphr.h"

#include "hardwareBsp.h"



#ifndef TWI_MASTER_DRIVER_H
#define TWI_MASTER_DRIVER_H

#include <stdint.h>
#include "hardwareConfig.h"

/*! Baud register setting calculation. Formula described in datasheet. */
#define TWI_BAUD(F_SYS, F_TWI) ((F_SYS / (2 * F_TWI)) - 5)


/*! Transaction status defines. */
#define TWIM_STATUS_READY              0
#define TWIM_STATUS_BUSY               1


/*! Transaction result enumeration. */
typedef enum TWI_REZ_enum {
	TWI_REZ_UNKNOWN                 = 0,
	TWI_REZ_OK                      = 0x01,
    
	TWI_REZ_OVERFLOW                = 0x02,
	TWI_REZ_ARBITRATION_LOST        = 0x04,
	TWI_REZ_BUS_ERROR               = 0x08,
	TWI_REX_NACK_RECEIVED           = 0x10,
	TWI_REZ_FAIL                    = 0x20,
	TWI_REZ_MUTEX_TIMEOUT_STAGE1    = 0x40,
	TWI_REZ_MUTEX_TIMEOUT_STAGE2    = 0x80
} TWI_REZ_t;

/*! Buffer size defines */
#define TWIM_WRITE_BUFFER_SIZE         8
#define TWIM_READ_BUFFER_SIZE          8


/*! \brief TWI master driver struct
 *
 *  TWI master struct. Holds pointer to TWI module,
 *  buffers and necessary varibles.
 */
typedef struct TWI_Master {
	TWI_t *interface;                           /*!< Pointer to what interface to use */
    xSemaphoreHandle busy;                      /*!< Mutex, async blocking operations */
    signed portBASE_TYPE hptw;                  /*!< Higher Priority Task Woken*/
    
	uint8_t address;                            /*!< Slave address */
	uint8_t writeData[TWIM_WRITE_BUFFER_SIZE];  /*!< Data to write */
	uint8_t readData[TWIM_READ_BUFFER_SIZE];    /*!< Read data */
	uint8_t bytesToWrite;                       /*!< Number of bytes to write */
	uint8_t bytesToRead;                        /*!< Number of bytes to read */
	uint8_t bytesWritten;                       /*!< Number of bytes written */
	uint8_t bytesRead;                          /*!< Number of bytes read */
	volatile uint8_t result;                    /*!< Result of transaction */
}TWI_Master_t;



void TwiMaster_Init(TWI_Master_t *twi, TWI_t *module, TWI_MASTER_INTLVL_t intLevel, uint8_t baudRateRegisterSetting);
void TwiMaster_Restart(TWI_Master_t *twi);

TWI_MASTER_BUSSTATE_t TwiMaster_State(TWI_Master_t *twi);
uint8_t TwiMaster_IsReady(TWI_Master_t *twi);
uint8_t TwiMaster_Write(TWI_Master_t *twi, uint8_t address, uint8_t * writeData, uint8_t bytesToWrite);
uint8_t TwiMaster_Read(TWI_Master_t *twi, uint8_t address, uint8_t bytesToRead, uint8_t *rdDta);
uint8_t TwiMaster_ReadAndWrite(TWI_Master_t *twi, uint8_t address, uint8_t bytesToWrite, const uint8_t *writeData,  uint8_t bytesToRead, uint8_t *rdData);

void TwiMaster_Irq(TWI_Master_t *twi);


/*! TWI master interrupt service routine.
 *
 *  Interrupt service routine for the TWI master. Copy the needed vectors
 *  into your code.
 *

 *
 */

#endif /* TWI_MASTER_DRIVER_H */