#include <string.h>

#include "include/twi.h"
#include "FreeRTOS.h"
#include "task.h"

static inline void _twiMasterWriteHandler(TWI_Master_t *twi);
static inline void _twiMasterReadHandler(TWI_Master_t *twi);
static inline void _twiMasterTransactionFinished(TWI_Master_t *twi, uint8_t result);


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
void TwiMaster_Init(TWI_Master_t *twi,
                    TWI_t *module,
                    TWI_MASTER_INTLVL_t intLevel,
                    uint8_t baudRateRegisterSetting)
{
	twi->interface = module;
    twi->busy = xSemaphoreCreateMutex();
    
	twi->interface->MASTER.CTRLA = intLevel |
	                               TWI_MASTER_RIEN_bm |
	                               TWI_MASTER_WIEN_bm |
	                               TWI_MASTER_ENABLE_bm;
	twi->interface->MASTER.BAUD = baudRateRegisterSetting;
	twi->interface->MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
}

void TwiMaster_Restart(TWI_Master_t *twi)
{
    xSemaphoreGive(twi->busy);
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
TWI_MASTER_BUSSTATE_t TwiMaster_State(TWI_Master_t *twi)
{
	TWI_MASTER_BUSSTATE_t twi_status;
	twi_status = (TWI_MASTER_BUSSTATE_t) (twi->interface->MASTER.STATUS &
	                                      TWI_MASTER_BUSSTATE_gm);
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
uint8_t TwiMaster_IsReady(TWI_Master_t *twi)
{
	uint8_t twi_status = 0;
    if (xSemaphoreTake(twi->busy, 100) == pdFALSE)
        goto exit;
    twi_status = 1;

    xSemaphoreGive(twi->busy);
	exit:
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
uint8_t TwiMaster_Write(TWI_Master_t *twi, uint8_t address, uint8_t *writeData, uint8_t bytesToWrite)
{
	uint8_t twi_status = TwiMaster_ReadAndWrite(twi, address, bytesToWrite, writeData, 0, NULL);
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
uint8_t TwiMaster_Read(TWI_Master_t *twi, uint8_t address, uint8_t bytesToRead, uint8_t *rdDta)
{
	uint8_t twi_status = TwiMaster_ReadAndWrite(twi, address, 0, NULL, bytesToRead, rdDta);
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
uint8_t TwiMaster_ReadAndWrite(TWI_Master_t *twi, uint8_t address, uint8_t bytesToWrite, const uint8_t *writeData,  uint8_t bytesToRead, uint8_t *rdData)
{
    uint8_t result = 0;
	/*Parameter sanity check. */
	if (bytesToWrite > TWIM_WRITE_BUFFER_SIZE)
    {
        result = TWI_REZ_OVERFLOW;
        goto exit;
    }

	
	if (bytesToRead > TWIM_READ_BUFFER_SIZE)
    {
        result = TWI_REZ_OVERFLOW;
        goto exit;
    }
    
	/*Initiate transaction if bus is ready. */

    if (xSemaphoreTake(twi->busy, 10) == pdFALSE)
    {
        result|= TWI_REZ_MUTEX_TIMEOUT_STAGE1;
        goto exit;
    }
    
	twi->result = TWI_REZ_UNKNOWN;
	twi->address = address<<1;

	/* Fill write data buffer. */
	for (uint8_t bufferIndex=0; bufferIndex < bytesToWrite; bufferIndex++)
		twi->writeData[bufferIndex] = writeData[bufferIndex];

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
	else if (twi->bytesToRead > 0) {
		uint8_t readAddress = twi->address | 0x01;
        twi->interface->MASTER.ADDR = readAddress;
    }

    if (xSemaphoreTake(twi->busy, 200) == pdTRUE) //Possible race condition. Release Semapthore after reading receive data.
    {
        
        if (rdData != NULL)
        {
            memcpy(rdData, twi->readData, bytesToRead);
        }
        result = twi->result;
        xSemaphoreGive(twi->busy);
    }
    else
    {
        result = twi->result | TWI_REZ_MUTEX_TIMEOUT_STAGE2;
    }
    
    exit:
    return result;
}


/*! \brief Common TWI master interrupt service routine.
 *
 *  Check current status and calls the appropriate handler.
 *
 *  \param twi  The TWI_Master_t struct instance.
 */
void TwiMaster_Irq(TWI_Master_t *twi)
{
	uint8_t currentStatus = twi->interface->MASTER.STATUS;

	/* If arbitration lost or bus error. */
	if (currentStatus & TWI_MASTER_ARBLOST_bm)
    {
        twi->result|= TWI_REZ_ARBITRATION_LOST;

    	/* Clear interrupt flag. */
        twi->interface->MASTER.STATUS = twi->interface->MASTER.STATUS | TWI_MASTER_ARBLOST_bm;
        xSemaphoreGiveFromISR(twi->busy, &twi->hptw);
	}
    else if (currentStatus & TWI_MASTER_BUSERR_bm) 
    {
      	twi->result|= TWI_REZ_BUS_ERROR;

    	/* Clear interrupt flag. */
        twi->interface->MASTER.STATUS = twi->interface->MASTER.STATUS | TWI_MASTER_ARBLOST_bm;
        xSemaphoreGiveFromISR(twi->busy, &twi->hptw);
	}
	else if (currentStatus & TWI_MASTER_WIF_bm) /* If master write interrupt. */
    {
		_twiMasterWriteHandler(twi);
	}
	else if (currentStatus & TWI_MASTER_RIF_bm) /* If master read interrupt. */
    {
		_twiMasterReadHandler(twi);
	}
	else 	/* If unexpected state. */
    {
		_twiMasterTransactionFinished(twi, TWI_REZ_FAIL);
	}
}


/*! \brief TWI master write interrupt handler.
 *
 *  Handles TWI transactions (master write) and responses to (N)ACK.
 *
 *  \param twi The TWI_Master_t struct instance.
 */
static inline void _twiMasterWriteHandler(TWI_Master_t *twi)
{
	if (twi->interface->MASTER.STATUS & TWI_MASTER_RXACK_bm) 	/* If NOT acknowledged (NACK) by slave */
    {                                                           /* cancel the transaction. */
		twi->interface->MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc;
		twi->result|= TWI_REX_NACK_RECEIVED;

        xSemaphoreGiveFromISR(twi->busy, &twi->hptw);
	}
	else if (twi->bytesWritten < twi->bytesToWrite) 	        /* If more bytes to write, send data. */
    {
		uint8_t data = twi->writeData[twi->bytesWritten];
		twi->interface->MASTER.DATA = data;
		++twi->bytesWritten;
	}
	else if (twi->bytesRead < twi->bytesToRead)                 /* If bytes to read, send repeated */ 
    {                                                           /* START condition + Address + 'R/_W = 1' */
		uint8_t readAddress = twi->address | 0x01;
		twi->interface->MASTER.ADDR = readAddress;
	}
	else 	/* If transaction finished, send STOP condition and set RESULT OK. */
    {
		twi->interface->MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc;
		_twiMasterTransactionFinished(twi, TWI_REZ_OK);
	}
}


/*! \brief TWI master read interrupt handler.
 *
 *  This is the master read interrupt handler that takes care of
 *  reading bytes from the TWI slave.
 *
 *  \param twi The TWI_Master_t struct instance.
 */
static inline void _twiMasterReadHandler(TWI_Master_t *twi)
{
	if (twi->bytesRead < TWIM_READ_BUFFER_SIZE)  	/* Fetch data if bytes to be read. */
    {
		twi->readData[twi->bytesRead] = twi->interface->MASTER.DATA;
		twi->bytesRead++;
	}
	else 	/* If buffer overflow, issue STOP and BUFFER_OVERFLOW condition. */
    {
		twi->interface->MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc;
		_twiMasterTransactionFinished(twi, TWI_REZ_OVERFLOW);
	}

	if (twi->bytesRead < twi->bytesToRead) 	/* If more bytes to read, issue ACK and start a byte read. */
    {
		twi->interface->MASTER.CTRLC = TWI_MASTER_CMD_RECVTRANS_gc;
	}
	else	/* If transaction finished, issue NACK and STOP condition. */
    {
		twi->interface->MASTER.CTRLC = TWI_MASTER_ACKACT_bm | TWI_MASTER_CMD_STOP_gc;
		_twiMasterTransactionFinished(twi, TWI_REZ_OK);
	}
}


/*! \brief TWI transaction finished handler.
 *
 *  Prepares module for new transaction.
 *
 *  \param twi     The TWI_Master_t struct instance.
 *  \param result  The result of the operation.
 */
static inline void _twiMasterTransactionFinished(TWI_Master_t *twi, uint8_t result)
{
	twi->result|= result;
    xSemaphoreGiveFromISR(twi->busy, &twi->hptw);
}
