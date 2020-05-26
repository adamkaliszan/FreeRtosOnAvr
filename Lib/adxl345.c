/*
ADXL345.h - Header file for the ADXL345 Triple Axis Accelerometer.
Version: 0.9.0
(c) 2020 Adam Kaliszan adam.kaliszan@gmail.com
(c) 2014 Korneliusz Jarzebski www.jarzebski.pl
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


#include <stdint.h>
#include <stdio.h>
#include <float.h>

#include "adxl345.h"
#include "../drv/include/twi.h"
#include "cmdline.h"

static void adxl345_writeRegister8(TWI_Master_t *twi, uint8_t reg, uint8_t value);
static uint8_t adxl345_readRegister8(TWI_Master_t *twi, uint8_t reg);
static uint8_t adxl345_fastRegister8(TWI_Master_t *twi, uint8_t reg);
static int16_t adxl345_readRegister16(TWI_Master_t *twi, uint8_t reg);
static void adxl345_writeRegisterBit(TWI_Master_t *twi, uint8_t reg, uint8_t pos, uint8_t state);
static uint8_t adxl345_readRegisterBit(TWI_Master_t *twi, uint8_t reg, uint8_t pos);
static float constrain (float x, float min, float max);
void setDoubleTapLatency(ADXL345_t *adxl, float latency);

void adxl345_init(ADXL345_t *adxl, TWI_Master_t *twi, adxl345_range_t range, FILE *io)
{
    adxl->twi = twi;
    if (io != NULL)
        IO_msg("Initializing Adxl345 accelerometer: ");    
    
    if (adxl345_fastRegister8(adxl->twi, ADXL345_REG_DEVID) != 0xE5)
    {
        if (io != NULL)
            IO_msg("Connection Error\r\n");
        return;
    }

    // Enable measurement mode (0b00001000)
    adxl345_writeRegister8(adxl->twi, ADXL345_REG_POWER_CTL, 0x08);

    adxl345_clearSettings(adxl);

    
    adxl345_setRange(adxl, range);
    if (io != NULL)
    {
        IO_msg("Range: ");
        switch(adxl345_getRange(adxl))
        {
            case ADXL345_RANGE_16G: IO_msg("+/- 16 g"); break;
            case ADXL345_RANGE_8G:  IO_msg("+/- 8 g");  break;
            case ADXL345_RANGE_4G:  IO_msg("+/- 4 g");  break;
            case ADXL345_RANGE_2G:  IO_msg("+/- 2 g");  break;
            default: IO_msg("Wrong range"); break;
        }
    }
    
    if (io != NULL)
    {
        IO_msg(", sample rate: ");
        switch(adxl345_getSampleRate(adxl))
        {
            case ADXL345_DATARATE_3200HZ: IO_msg("3200 Hz"); break;
            case ADXL345_DATARATE_1600HZ: IO_msg("1600 Hz"); break;
            case ADXL345_DATARATE_800HZ:  IO_msg("800 Hz");  break;
            case ADXL345_DATARATE_400HZ:  IO_msg("400 Hz");  break;
            case ADXL345_DATARATE_200HZ:  IO_msg("200 Hz");  break;
            case ADXL345_DATARATE_100HZ:  IO_msg("100 Hz");  break;
            case ADXL345_DATARATE_50HZ:   IO_msg("50 Hz");   break;
            case ADXL345_DATARATE_25HZ:   IO_msg("25 Hz");   break;
            case ADXL345_DATARATE_12_5HZ: IO_msg("12.5 Hz"); break;
            case ADXL345_DATARATE_6_25HZ: IO_msg("6.25 Hz"); break;
            case ADXL345_DATARATE_3_13HZ: IO_msg("3.13 Hz"); break;
            case ADXL345_DATARATE_1_56HZ: IO_msg("1.56 Hz"); break;
            case ADXL345_DATARATE_0_78HZ: IO_msg("0.78 Hz"); break;
            case ADXL345_DATARATE_0_39HZ: IO_msg("0.39 Hz"); break;
            case ADXL345_DATARATE_0_20HZ: IO_msg("0.20 Hz"); break;
            case ADXL345_DATARATE_0_10HZ: IO_msg("0.10 Hz"); break;
            default: IO_msg("???"); break;
        }
    }
    if (io != NULL)
        IO_msg("\r\n");        
}

static float constrain (float x, float min, float max)
{
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

// Set Range
void adxl345_setRange(ADXL345_t *adxl, adxl345_range_t range)
{
  // Get actual value register
  uint8_t value = adxl345_readRegister8(adxl->twi, ADXL345_REG_DATA_FORMAT);

  // Update the data rate
  // (&) 0b11110000 (0xF0 - Leave HSB)
  // (|) 0b0000xx?? (range - Set range)
  // (|) 0b00001000 (0x08 - Set Full Res)
  value &= 0xF0;
  value |= range;
  value |= 0x08;

  adxl345_writeRegister8(adxl->twi, ADXL345_REG_DATA_FORMAT, value);
}

// Get Range
adxl345_range_t adxl345_getRange(ADXL345_t *adxl)
{
    return (adxl345_range_t)(adxl345_readRegister8(adxl->twi, ADXL345_REG_DATA_FORMAT) & 0x03);
}

// Set Data Rate
void adxl345_setSampleRate(ADXL345_t *adxl, adxl345_dataRate_t dataRate)
{
    adxl345_writeRegister8(adxl->twi, ADXL345_REG_BW_RATE, dataRate);
}

// Get Data Rate
adxl345_dataRate_t adxl345_getSampleRate(ADXL345_t *adxl)
{
    return (adxl345_dataRate_t)(adxl345_readRegister8(adxl->twi, ADXL345_REG_BW_RATE) & 0x0F);
}

// Low Pass Filter
void adxl345_lowPassFilter(ADXL345_t *adxl, const struct VectorFloat *vector, float alpha)
{
    if (alpha == 0)
        alpha = 0.5;
    adxl->data.XAxis = vector->XAxis * alpha + (adxl->data.XAxis * (1.0 - alpha));
    adxl->data.YAxis = vector->YAxis * alpha + (adxl->data.YAxis * (1.0 - alpha));
    adxl->data.ZAxis = vector->ZAxis * alpha + (adxl->data.ZAxis * (1.0 - alpha));
}

// Read raw values
void adxl345_readRaw(ADXL345_t *adxl, struct VectorUint16 *result)
{
    result->XAxis = adxl345_readRegister16(adxl->twi, ADXL345_REG_DATAX0);
    result->YAxis = adxl345_readRegister16(adxl->twi, ADXL345_REG_DATAY0);
    result->ZAxis = adxl345_readRegister16(adxl->twi, ADXL345_REG_DATAZ0);
}

// Read normalized values
void adxl345_readNormalize(ADXL345_t *adxl, struct VectorFloat *result, float gravityFactor)
{
    struct VectorUint16 *tmp = (struct VectorUint16*) result;
    adxl345_readRaw(adxl, tmp);

    // (4 mg/LSB scale factor in Full Res) * gravity factor
    result->XAxis = tmp->XAxis * 0.004 * gravityFactor;
    result->YAxis = tmp->YAxis * 0.004 * gravityFactor;
    result->ZAxis = tmp->ZAxis * 0.004 * gravityFactor;
}

// Read scaled values
void adxl345_readScaled(ADXL345_t *adxl, struct VectorFloat *result)
{
    struct VectorUint16 *tmp = (struct VectorUint16*) result;
    adxl345_readRaw(adxl, tmp);

    // (4 mg/LSB scale factor in Full Res)
    result->XAxis = tmp->XAxis * 0.004;
    result->YAxis = tmp->YAxis * 0.004;
    result->ZAxis = tmp->ZAxis * 0.004;
}

void adxl345_clearSettings(ADXL345_t *adxl)
{
    uint8_t value;

    adxl345_setRange(adxl, ADXL345_RANGE_2G);
    adxl345_setSampleRate(adxl, ADXL345_DATARATE_100HZ);

    adxl345_writeRegister8(adxl->twi, ADXL345_REG_THRESH_TAP, 0x00);
    adxl345_writeRegister8(adxl->twi, ADXL345_REG_DUR, 0x00);
    adxl345_writeRegister8(adxl->twi, ADXL345_REG_LATENT, 0x00);
    adxl345_writeRegister8(adxl->twi, ADXL345_REG_WINDOW, 0x00);
    adxl345_writeRegister8(adxl->twi, ADXL345_REG_THRESH_ACT, 0x00);
    adxl345_writeRegister8(adxl->twi, ADXL345_REG_THRESH_INACT, 0x00);
    adxl345_writeRegister8(adxl->twi, ADXL345_REG_TIME_INACT, 0x00);
    adxl345_writeRegister8(adxl->twi, ADXL345_REG_THRESH_FF, 0x00);
    adxl345_writeRegister8(adxl->twi, ADXL345_REG_TIME_FF, 0x00);


    value = adxl345_readRegister8(adxl->twi, ADXL345_REG_ACT_INACT_CTL);
    value &= 0b10001000;
    adxl345_writeRegister8(adxl->twi, ADXL345_REG_ACT_INACT_CTL, value);

    value = adxl345_readRegister8(adxl->twi, ADXL345_REG_TAP_AXES);
    value &= 0b11111000;
    adxl345_writeRegister8(adxl->twi, ADXL345_REG_TAP_AXES, value);
}

// Set Tap Threshold (62.5mg / LSB)
void adxl345_setTapThreshold(ADXL345_t *adxl, float threshold)
{
    uint8_t scaled = constrain(threshold / 0.0625f, 0, 255);
    adxl345_writeRegister8(adxl->twi, ADXL345_REG_THRESH_TAP, scaled);
}

// Get Tap Threshold (62.5mg / LSB)
float adxl345_getTapThreshold(ADXL345_t *adxl)
{
    return adxl345_readRegister8(adxl->twi, ADXL345_REG_THRESH_TAP) * 0.0625f;
}

// Set Tap Duration (625us / LSB)
void adxl345_setTapDuration(ADXL345_t *adxl, float duration)
{
    uint8_t scaled = constrain(duration / 0.000625f, 0, 255);
    adxl345_writeRegister8(adxl->twi, ADXL345_REG_DUR, scaled);
}

// Get Tap Duration (625us / LSB)
float adxl345_getTapDuration(ADXL345_t *adxl)
{
    return adxl345_readRegister8(adxl->twi, ADXL345_REG_DUR) * 0.000625f;
}

// Set Double Tap Latency (1.25ms / LSB)
void setDoubleTapLatency(ADXL345_t *adxl, float latency)
{
    uint8_t scaled = constrain(latency / 0.00125f, 0, 255);
    adxl345_writeRegister8(adxl->twi, ADXL345_REG_LATENT, scaled);
}

// Get Double Tap Latency (1.25ms / LSB)
float adxl345_getDoubleTapLatency(ADXL345_t *adxl)
{
    return adxl345_readRegister8(adxl->twi, ADXL345_REG_LATENT) * 0.00125f;
}

// Set Double Tap Window (1.25ms / LSB)
void adxl345_setDoubleTapWindow(ADXL345_t *adxl, float window)
{
    uint8_t scaled = constrain(window / 0.00125f, 0, 255);
    adxl345_writeRegister8(adxl->twi, ADXL345_REG_WINDOW, scaled);
}

// Get Double Tap Window (1.25ms / LSB)
float adxl345_getDoubleTapWindow(ADXL345_t *adxl)
{
    return adxl345_readRegister8(adxl->twi, ADXL345_REG_WINDOW) * 0.00125f;
}

// Set Activity Threshold (62.5mg / LSB)
void adxl345_setActivityThreshold(ADXL345_t *adxl, float threshold)
{
    uint8_t scaled = constrain(threshold / 0.0625f, 0, 255);
    adxl345_writeRegister8(adxl->twi, ADXL345_REG_THRESH_ACT, scaled);
}

// Get Activity Threshold (65.5mg / LSB)
float adxl345_getActivityThreshold(ADXL345_t *adxl)
{
    return adxl345_readRegister8(adxl->twi, ADXL345_REG_THRESH_ACT) * 0.0625f;
}

// Set Inactivity Threshold (65.5mg / LSB)
void adxl345_setInactivityThreshold(ADXL345_t *adxl, float threshold)
{
    uint8_t scaled = constrain(threshold / 0.0625f, 0, 255);
    adxl345_writeRegister8(adxl->twi, ADXL345_REG_THRESH_INACT, scaled);
}

// Get Incactivity Threshold (65.5mg / LSB)
float adxl345_getInactivityThreshold(ADXL345_t *adxl)
{
    return adxl345_readRegister8(adxl->twi, ADXL345_REG_THRESH_INACT) * 0.0625f;
}

// Set Inactivity Time (s / LSB)
void adxl345_setTimeInactivity(ADXL345_t *adxl, uint8_t time)
{
    adxl345_writeRegister8(adxl->twi, ADXL345_REG_TIME_INACT, time);
}

// Get Inactivity Time (s / LSB)
uint8_t adxl345_getTimeInactivity(ADXL345_t *adxl)
{
    return adxl345_readRegister8(adxl->twi, ADXL345_REG_TIME_INACT);
}

// Set Free Fall Threshold (65.5mg / LSB)
void adxl345_setFreeFallThreshold(ADXL345_t *adxl, float threshold)
{
    uint8_t scaled = constrain(threshold / 0.0625f, 0, 255);
    adxl345_writeRegister8(adxl->twi, ADXL345_REG_THRESH_FF, scaled);
}

// Get Free Fall Threshold (65.5mg / LSB)
float adxl345_getFreeFallThreshold(ADXL345_t *adxl)
{
    return adxl345_readRegister8(adxl->twi, ADXL345_REG_THRESH_FF) * 0.0625f;
}

// Set Free Fall Duratiom (5ms / LSB)
void adxl345_setFreeFallDuration(ADXL345_t *adxl, float duration)
{
    uint8_t scaled = constrain(duration / 0.005f, 0, 255);
    adxl345_writeRegister8(adxl->twi, ADXL345_REG_TIME_FF, scaled);
}

// Get Free Fall Duratiom
float adxl345_getFreeFallDuration(ADXL345_t *adxl)
{
    return adxl345_readRegister8(adxl->twi, ADXL345_REG_TIME_FF) * 0.005f;
}

void adxl345_setActivityX(ADXL345_t *adxl, uint8_t state)
{
    adxl345_writeRegisterBit(adxl->twi, ADXL345_REG_ACT_INACT_CTL, 6, state);
}

uint8_t adxl345_getActivityX(ADXL345_t *adxl)
{
    return adxl345_readRegisterBit(adxl->twi, ADXL345_REG_ACT_INACT_CTL, 6);
}

void adxl345_setActivityY(ADXL345_t *adxl, uint8_t state)
{
    adxl345_writeRegisterBit(adxl->twi, ADXL345_REG_ACT_INACT_CTL, 5, state);
}

uint8_t adxl345_getActivityY(ADXL345_t *adxl)
{
    return adxl345_readRegisterBit(adxl->twi, ADXL345_REG_ACT_INACT_CTL, 5);
}

void adxl345_setActivityZ(ADXL345_t *adxl, uint8_t state)
{
    adxl345_writeRegisterBit(adxl->twi, ADXL345_REG_ACT_INACT_CTL, 4, state);
}

uint8_t adxl345_getActivityZ(ADXL345_t *adxl)
{
    return adxl345_readRegisterBit(adxl->twi, ADXL345_REG_ACT_INACT_CTL, 4);
}

void adxl345_setActivityXYZ(ADXL345_t *adxl, uint8_t state)
{
    uint8_t value;

    value = adxl345_readRegister8(adxl->twi, ADXL345_REG_ACT_INACT_CTL);

    if (state)
    	value |= 0b00111000;
    else
        value &= 0b11000111;

    adxl345_writeRegister8(adxl->twi, ADXL345_REG_ACT_INACT_CTL, value);
}


void adxl345_setInactivityX(ADXL345_t *adxl, uint8_t state) 
{
    adxl345_writeRegisterBit(adxl->twi, ADXL345_REG_ACT_INACT_CTL, 2, state);
}

uint8_t adxl345_getInactivityX(ADXL345_t *adxl)
{
    return adxl345_readRegisterBit(adxl->twi, ADXL345_REG_ACT_INACT_CTL, 2);
}

void adxl345_setInactivityY(ADXL345_t *adxl, uint8_t state)
{
    adxl345_writeRegisterBit(adxl->twi, ADXL345_REG_ACT_INACT_CTL, 1, state);
}

uint8_t adxl345_getInactivityY(ADXL345_t *adxl)
{
    return adxl345_readRegisterBit(adxl->twi, ADXL345_REG_ACT_INACT_CTL, 1);
}

void adxl345_setInactivityZ(ADXL345_t *adxl, uint8_t state)
{
    adxl345_writeRegisterBit(adxl->twi, ADXL345_REG_ACT_INACT_CTL, 0, state);
}

uint8_t adxl345_getInactivityZ(ADXL345_t *adxl)
{
    return adxl345_readRegisterBit(adxl->twi, ADXL345_REG_ACT_INACT_CTL, 0);
}

void adxl345_setInactivityXYZ(ADXL345_t *adxl, uint8_t state)
{
    uint8_t value;

    value = adxl345_readRegister8(adxl->twi, ADXL345_REG_ACT_INACT_CTL);

    if (state)
    	value |= 0b00000111;
    else
    	value &= 0b11111000;

    adxl345_writeRegister8(adxl->twi, ADXL345_REG_ACT_INACT_CTL, value);
}

void adxl345_setTapDetectionX(ADXL345_t *adxl, uint8_t state)
{
    adxl345_writeRegisterBit(adxl->twi, ADXL345_REG_TAP_AXES, 2, state);
}

uint8_t adxl345_getTapDetectionX(ADXL345_t *adxl)
{
    return adxl345_readRegisterBit(adxl->twi, ADXL345_REG_TAP_AXES, 2);
}

void adxl345_setTapDetectionY(ADXL345_t *adxl, uint8_t state)
{
    adxl345_writeRegisterBit(adxl->twi, ADXL345_REG_TAP_AXES, 1, state);
}

uint8_t adxl345_getTapDetectionY(ADXL345_t *adxl)
{
    return adxl345_readRegisterBit(adxl->twi, ADXL345_REG_TAP_AXES, 1);
}

void adxl345_setTapDetectionZ(ADXL345_t *adxl, uint8_t state)
{
    adxl345_writeRegisterBit(adxl->twi, ADXL345_REG_TAP_AXES, 0, state);
}

uint8_t adxl345_getTapDetectionZ(ADXL345_t *adxl)
{
    return adxl345_readRegisterBit(adxl->twi, ADXL345_REG_TAP_AXES, 0);
}

void adxl345_setTapDetectionXYZ(ADXL345_t *adxl, uint8_t state)
{
    uint8_t value;

    value = adxl345_readRegister8(adxl->twi, ADXL345_REG_TAP_AXES);

    if (state)
    	value |= 0b00000111;
    else
    	value &= 0b11111000;

    adxl345_writeRegister8(adxl->twi, ADXL345_REG_TAP_AXES, value);
}


void adxl345_useInterrupt(ADXL345_t *adxl, adxl345_int_t interrupt)
{
    if (interrupt == 0)
    {
    	adxl345_writeRegister8(adxl->twi, ADXL345_REG_INT_MAP, 0x00);
    } 
    else
    {
    	adxl345_writeRegister8(adxl->twi, ADXL345_REG_INT_MAP, 0xFF);
    }
    adxl345_writeRegister8(adxl->twi, ADXL345_REG_INT_ENABLE, 0xFF);
}

void adxl345_readActivites(ADXL345_t *adxl, struct Activites *result)
{
    result->data1 = adxl345_readRegister8(adxl->twi, ADXL345_REG_INT_SOURCE);
    //adxl345_a.isOverrun = ((data >> ADXL345_OVERRUN) & 1);
    //adxl345_a.isWatermark = ((data >> ADXL345_WATERMARK) & 1);
    //adxl345_a.isFreeFall = ((data >> ADXL345_FREE_FALL) & 1);
    //adxl345_a.isInactivity = ((data >> ADXL345_INACTIVITY) & 1);
    //adxl345_a.isActivity = ((data >> ADXL345_ACTIVITY) & 1);
    //adxl345_a.isDoubleTap = ((data >> ADXL345_DOUBLE_TAP) & 1);
    //adxl345_a.isTap = ((data >> ADXL345_SINGLE_TAP) & 1);
    //adxl345_a.isDataReady = ((data >> ADXL345_DATA_READY) & 1);

    result->data2 = adxl345_readRegister8(adxl->twi, ADXL345_REG_ACT_TAP_STATUS);
    //adxl345_a.isActivityOnX = ((data >> 6) & 1);
    //adxl345_a.isActivityOnY = ((data >> 5) & 1);
    //adxl345_a.isActivityOnZ = ((data >> 4) & 1);
    //adxl345_a.isTapOnX = ((data >> 2) & 1);
    //adxl345_a.isTapOnY = ((data >> 1) & 1);
    //adxl345_a.isTapOnZ = ((data >> 0) & 1);
}

// Write byte to register
void adxl345_writeRegister8(TWI_Master_t *twi, uint8_t reg, uint8_t value)
{
    uint8_t data[2] = {reg, value};
    TwiMaster_Write(twi, ADXL345_ADDRESS, data, 2);
}

// Read byte to register
uint8_t adxl345_fastRegister8(TWI_Master_t *twi, uint8_t reg)
{
    uint8_t value;
    TwiMaster_ReadAndWrite(twi, ADXL345_ADDRESS, 1, &reg, 1, &value);
    return value;
}

// Read byte from register
uint8_t adxl345_readRegister8(TWI_Master_t *twi, uint8_t reg)
{
    uint8_t value;
    TwiMaster_Write(twi, ADXL345_ADDRESS, &reg, 1);
    TwiMaster_Read(twi, ADXL345_ADDRESS, 1, &value);
    return value;
}

// Read word from register
int16_t adxl345_readRegister16(TWI_Master_t *twi, uint8_t reg)
{
    int16_t value;
    TwiMaster_ReadAndWrite(twi, ADXL345_ADDRESS, 1, &reg, 2, (uint8_t *)&value);
    return value;
}

static void adxl345_writeRegisterBit(TWI_Master_t *twi, uint8_t reg, uint8_t pos, uint8_t state)
{
    uint8_t value;
    value = adxl345_readRegister8(twi, reg);

    if (state)
    {
	value |= (1 << pos);
    } else 
    {
	value &= ~(1 << pos);
    }

    adxl345_writeRegister8(twi, reg, value);
}

uint8_t adxl345_readRegisterBit(TWI_Master_t *twi, uint8_t reg, uint8_t pos)
{
    uint8_t value;
    value = adxl345_readRegister8(twi, reg);
    return ((value >> pos) & 1);
}
