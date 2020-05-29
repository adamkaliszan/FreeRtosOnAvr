/*
 * 2020 Adam Kaliszan adam.kaliszan@gmail com
 * 
 * base on:
L3G4200D.cpp - Class file for the L3G4200D Triple Axis Gyroscope Arduino Library.
Version: 1.3.3
(c) 2014 Korneliusz Jarzebski
www.jarzebski.pl
This program is free software: you can redistribute it and/or modify
it under the terms of the version 3 GNU General Public License as
published by the Free Software Foundation.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <string.h>
#include <math.h>

#include "FreeRTOS.h"
#include "semphr.h"

#include "include/l3g4200d.h"
#include "../drv/include/twi.h"
#include "task.h"
#include "cmdline.h"

static void l3g_writeRegister8(L3G4200D_t *l3g, uint8_t reg, uint8_t value);
static uint8_t l3g_fastRegister8(L3G4200D_t *l3g, uint8_t reg);
static uint8_t l3g_readRegister8(L3G4200D_t *l3g, uint8_t reg);



uint8_t l3g_init(L3G4200D_t *l3g, TWI_Master_t *twi, l3g4200d_dps_t scale, l3g4200d_odrbw_t odrbw, FILE *io)//L3G4200D_SCALE_2000DPS L3G4200D_DATARATE_100HZ_12_5
{
    memset(l3g, 0, sizeof(L3G4200D_t));
    l3g->twi = twi;
    l3g->mutex = xSemaphoreCreateMutex();

    if (io != NULL)
    {
        IO_msg("Initializing L3g4200D gyroscope module ");
    }
    if (l3g_fastRegister8(l3g, L3G4200D_REG_WHO_AM_I) != 0xD3)
    {
    	return 0;
        if (io != NULL)
        {
            IO_msg("Error");
        }
    }

    // Enable all axis and setup normal mode + Output Data Range & Bandwidth
    uint8_t reg1 = 0x0F; // Enable all axis and setup normal mode
    reg1|= (odrbw << 4); // Set output data rate & bandwidh
    
    l3g_writeRegister8(l3g, L3G4200D_REG_CTRL_REG1, reg1);

    // Disable high pass filter
    l3g_writeRegister8(l3g, L3G4200D_REG_CTRL_REG2, 0x00);

    // Generata data ready interrupt on INT2
    l3g_writeRegister8(l3g, L3G4200D_REG_CTRL_REG3, 0x08);

    // Set full scale selection in continous mode
    l3g_writeRegister8(l3g, L3G4200D_REG_CTRL_REG4, scale << 4);

    switch(scale)
    {
	case L3G4200D_SCALE_250DPS:
	    l3g->dpsPerDigit = .00875f;
	    break;
	case L3G4200D_SCALE_500DPS:
	    l3g->dpsPerDigit = .0175f;
	    break;
	case L3G4200D_SCALE_2000DPS:
	    l3g->dpsPerDigit = .07f;
	    break;
	default:
	    break;
    }

    // Boot in normal mode, disable FIFO, HPF disabled
    l3g_writeRegister8(l3g, L3G4200D_REG_CTRL_REG5, 0x00);


    if (io != NULL)
    {
        IO_msg("OK");
    }

    return 1;
}

// Get current scale
l3g4200d_dps_t l3g_getScale(L3G4200D_t *l3g)
{
    return (l3g4200d_dps_t)((l3g_readRegister8(l3g, L3G4200D_REG_CTRL_REG4) >> 4) & 0x03);
}


// Get current output data range and bandwidth
l3g4200d_odrbw_t l3g_getOdrBw(L3G4200D_t *l3g)
{
    return (l3g4200d_odrbw_t)((l3g_readRegister8(l3g, L3G4200D_REG_CTRL_REG1) >> 4) & 0x0F);
}

// Calibrate algorithm
void l3g_calibrate(L3G4200D_t *l3g, uint8_t samples)
{
    // Set calibrate
    l3g->useCalibrate = 1;

    // Reset values
    float sumX = 0;
    float sumY = 0;
    float sumZ = 0;
    float sigmaX = 0;
    float sigmaY = 0;
    float sigmaZ = 0;

    // Read n-samples
    for (uint8_t i = 0; i < samples; ++i)
    {
        l3g_readRaw(l3g);
    	sumX += l3g->r.XAxis;
        sumY += l3g->r.YAxis;
        sumZ += l3g->r.ZAxis;

    	sigmaX += l3g->r.XAxis * l3g->r.XAxis;
        sigmaY += l3g->r.YAxis * l3g->r.YAxis;
        sigmaZ += l3g->r.ZAxis * l3g->r.ZAxis;
	
        vTaskDelay(1);
    }

    // Calculate delta vectors
    l3g->d.XAxis = sumX / samples;
    l3g->d.YAxis = sumY / samples;
    l3g->d.ZAxis = sumZ / samples;

    // Calculate threshold vectors
    l3g->thresholdX = sqrt((sigmaX / samples) - (l3g->d.XAxis * l3g->d.XAxis));
    l3g->thresholdY = sqrt((sigmaY / samples) - (l3g->d.YAxis * l3g->d.YAxis));
    l3g->thresholdZ = sqrt((sigmaZ / samples) - (l3g->d.ZAxis * l3g->d.ZAxis));

    // If already set threshold, recalculate threshold vectors
    if (l3g->actualThreshold > 0)
    {
        l3g_setThreshold(l3g, l3g->actualThreshold);
    }
}

// Get current threshold value
uint8_t l3g_getThreshold(L3G4200D_t *l3g)
{
    return l3g->actualThreshold;
}

// Set treshold value
void l3g_setThreshold(L3G4200D_t *l3g, uint8_t multiple)
{
    if (multiple > 0)
    {
	// If not calibrated, need calibrate
    	if (!l3g->useCalibrate)
        {
            l3g_calibrate(l3g, multiple);
        }	
	// Calculate threshold vectors
    	l3g->t.XAxis = l3g->thresholdX * multiple;
        l3g->t.YAxis = l3g->thresholdY * multiple;
        l3g->t.ZAxis = l3g->thresholdZ * multiple;
    } 
    else
    {
        memset(&l3g->t, 0, 3*sizeof(float));
    }

    // Remember old threshold value
    l3g->actualThreshold = multiple;
}

// Write 8-bit to register
static void l3g_writeRegister8(L3G4200D_t *l3g, uint8_t reg, uint8_t value)
{
    uint8_t data[2] = { reg, value};
    xSemaphoreTake(l3g->mutex, portMAX_DELAY);
    TwiMaster_Write(l3g->twi, L3G4200D_ADDRESS, data, 2);
    xSemaphoreGive(l3g->mutex);
}

// Fast read 8-bit from register
static uint8_t l3g_fastRegister8(L3G4200D_t *l3g, uint8_t reg)
{
    uint8_t value;
    xSemaphoreTake(l3g->mutex, portMAX_DELAY);
    TwiMaster_ReadAndWrite(l3g->twi, L3G4200D_ADDRESS, 1, &reg, 1, &value);
    xSemaphoreGive(l3g->mutex);
    return value;
}

// Read 8-bit from register
static uint8_t l3g_readRegister8(L3G4200D_t *l3g, uint8_t reg)
{
    uint8_t value;
    xSemaphoreTake(l3g->mutex, portMAX_DELAY);
    TwiMaster_Write(l3g->twi, L3G4200D_ADDRESS, &reg, 1);
    TwiMaster_Read(l3g->twi, L3G4200D_ADDRESS, 1, &value);
    xSemaphoreGive(l3g->mutex);
    return value;
}

// L3G4200D Temperature sensor output change vs temperature: -1digit/degrCelsius (data representation: 2's complement).
// Value represents difference respect to a reference not specified value.
// So temperature sensor can be used to measure temperature variations: temperarture sensor isn't suitable to return absolute temperatures measures.
// If you run two sequential measures and differentiate them you can get temperature variation.
// This also means that two devices in the same temp conditions can return different outputs.
// Finally, you can use this info to compensate drifts due to temperature changes.
uint8_t l3g_readTemperature(L3G4200D_t *l3g)
{
    uint8_t tmp = L3G4200D_REG_OUT_TEMP;
    xSemaphoreTake(l3g->mutex, portMAX_DELAY);
    TwiMaster_Write(l3g->twi, L3G4200D_ADDRESS, &tmp, 1);
    TwiMaster_Read(l3g->twi, L3G4200D_ADDRESS, 1, &tmp);
    xSemaphoreGive(l3g->mutex);
    return tmp;
}

// Read raw values
void l3g_readRaw(L3G4200D_t *l3g)
{
    uint8_t data[6];
    uint8_t dtaOut = L3G4200D_REG_OUT_X_L | (1 << 7);

    xSemaphoreTake(l3g->mutex, portMAX_DELAY);    
    TwiMaster_Write(l3g->twi,L3G4200D_ADDRESS, &dtaOut, 1);
    TwiMaster_Read(l3g->twi, L3G4200D_ADDRESS, 6, data);
    xSemaphoreGive(l3g->mutex);    

    l3g->r.XAxis = data[0] | data[1]<<8;
    l3g->r.YAxis = data[2] | data[3]<<8;
    l3g->r.ZAxis = data[4] | data[5]<<8;
}

// Read normalized values
void l3g_readNormalize(L3G4200D_t *l3g)
{
    l3g_readRaw(l3g);

    if (l3g->useCalibrate)
    {
    	l3g->n.XAxis = (l3g->r.XAxis - l3g->d.XAxis) * l3g->dpsPerDigit;
        l3g->n.YAxis = (l3g->r.YAxis - l3g->d.YAxis) * l3g->dpsPerDigit;
        l3g->n.ZAxis = (l3g->r.ZAxis - l3g->d.ZAxis) * l3g->dpsPerDigit;
    } 
    else
    {
        l3g->n.XAxis = l3g->r.XAxis * l3g->dpsPerDigit;
        l3g->n.YAxis = l3g->r.YAxis * l3g->dpsPerDigit;
        l3g->n.ZAxis = l3g->r.ZAxis * l3g->dpsPerDigit;
    }

    if (l3g->actualThreshold > 0)
    {
    	if (abs(l3g->n.XAxis) < l3g->t.XAxis) l3g->n.XAxis = 0;
        if (abs(l3g->n.YAxis) < l3g->t.YAxis) l3g->n.YAxis = 0;
        if (abs(l3g->n.ZAxis) < l3g->t.ZAxis) l3g->n.ZAxis = 0;
    }
}