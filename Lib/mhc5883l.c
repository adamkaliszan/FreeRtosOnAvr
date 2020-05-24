
/*
 * 2020 Adam Kaliszan adam.kaliszan@gmail.com
 * BAse on
HMC5883L.cpp - Class file for the HMC5883L Triple Axis Digital Compass Arduino Library.
Version: 1.1.0
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

#include "mhc5883l.h"
#include "cmdline.h"

static void mhc5833_writeRegister8(MHC5883L_t *mhc, uint8_t reg, uint8_t value);
static uint8_t mhc5833_readRegister8(MHC5883L_t *mhc, uint8_t reg);
static uint8_t mhc5833_fastRegister8(MHC5883L_t *mhc, uint8_t reg);
static int16_t mhc5833_readRegister16(MHC5883L_t *mhc, uint8_t reg);


uint8_t mhc5883l_init(MHC5883L_t *mhc, TWI_Master_t *twi, FILE *io)
{
    if (io != NULL)
        IO_msg("Initializing Mhc5833l ");
    mhc->mutex = xSemaphoreCreateMutex();

    mhc->twi = twi;
    
    mhc->v.XAxis = 0;
    mhc->v.YAxis = 0;
    mhc->v.ZAxis = 0;
    
    
    if ((mhc5833_fastRegister8(mhc, HMC5883L_REG_IDENT_A) != 0x48)
    || (mhc5833_fastRegister8(mhc, HMC5883L_REG_IDENT_B) != 0x34)
    || (mhc5833_fastRegister8(mhc, HMC5883L_REG_IDENT_C) != 0x33))
    {
        if (io != NULL)
            IO_msg(" Conncetion lost\r\n");
    	return 0;
    }
    
    mhc5883_setRange(mhc, HMC5883L_RANGE_1_3GA);
    mhc5883l_setMeasurementMode(mhc, HMC5883L_CONTINOUS);
    mhc5883l_setDataRate(mhc, HMC5883L_DATARATE_15HZ);
    mhc5883l_setSamples(mhc, HMC5883L_SAMPLES_1);
  
    mhc->mgPerDigit = 0.92f;

    if (io != NULL)
        IO_msg("OK\r\n");
    return 1;
}

void mhc5883l_readRaw(MHC5883L_t *mhc)
{
    mhc->v.XAxis = mhc5833_readRegister16(mhc, HMC5883L_REG_OUT_X_M) - mhc->xOffset;
    mhc->v.YAxis = mhc5833_readRegister16(mhc, HMC5883L_REG_OUT_Y_M) - mhc->yOffset;
    mhc->v.ZAxis = mhc5833_readRegister16(mhc, HMC5883L_REG_OUT_Z_M);
}

void mhc5883l_readNormalize(MHC5883L_t *mhc, VectorFloat_t *result)
{
    mhc5883l_readRaw(mhc);
    result->XAxis = (mhc->v.XAxis -mhc->xOffset) * mhc->mgPerDigit;
    result->YAxis = (mhc->v.YAxis -mhc->yOffset) * mhc->mgPerDigit;
    result->ZAxis = (mhc->v.ZAxis) * mhc->mgPerDigit;
}

void mhc5883l_setOffset(MHC5883L_t *mhc, uint16_t xo, uint16_t yo)
{
    mhc->xOffset = xo;
    mhc->yOffset = yo;
}

void mhc5883_setRange(MHC5883L_t *mhc, hmc5883l_range_t range)
{
    switch(range)
    {
	case HMC5883L_RANGE_0_88GA:
	    mhc->mgPerDigit = 0.073f;
	    break;

	case HMC5883L_RANGE_1_3GA:
	    mhc->mgPerDigit = 0.92f;
	    break;

	case HMC5883L_RANGE_1_9GA:
	    mhc->mgPerDigit = 1.22f;
	    break;

	case HMC5883L_RANGE_2_5GA:
	    mhc->mgPerDigit = 1.52f;
	    break;

	case HMC5883L_RANGE_4GA:
	    mhc->mgPerDigit = 2.27f;
	    break;

	case HMC5883L_RANGE_4_7GA:
	    mhc->mgPerDigit = 2.56f;
	    break;

	case HMC5883L_RANGE_5_6GA:
	    mhc->mgPerDigit = 3.03f;
	    break;

	case HMC5883L_RANGE_8_1GA:
	    mhc->mgPerDigit = 4.35f;
	    break;

	default:
	    break;
    }

    mhc5833_writeRegister8(mhc, HMC5883L_REG_CONFIG_B, range << 5);
}

hmc5883l_range_t mhc5833l_getRange(MHC5883L_t *mhc)
{
    return (hmc5883l_range_t)((mhc5833_readRegister8(mhc, HMC5883L_REG_CONFIG_B) >> 5));
}

void mhc5883l_setMeasurementMode(MHC5883L_t *mhc, hmc5883l_mode_t mode)
{
    uint8_t value;

    value = mhc5833_readRegister8(mhc, HMC5883L_REG_MODE);
    value &= 0b11111100;
    value |= mode;

    mhc5833_writeRegister8(mhc, HMC5883L_REG_MODE, value);
}

hmc5883l_mode_t mhc5883l_getMeasurementMode(MHC5883L_t *mhc)
{
    uint8_t value;

    value = mhc5833_readRegister8(mhc, HMC5883L_REG_MODE);
    value &= 0b00000011;

    return (hmc5883l_mode_t)value;
}

void mhc5883l_setDataRate(MHC5883L_t *mhc, hmc5883l_dataRate_t dataRate)
{
    uint8_t value;

    value = mhc5833_readRegister8(mhc, HMC5883L_REG_CONFIG_A);
    value &= 0b11100011;
    value |= (dataRate << 2);

    mhc5833_writeRegister8(mhc, HMC5883L_REG_CONFIG_A, value);
}

hmc5883l_dataRate_t mhc5883l_getDataRate(MHC5883L_t *mhc)
{
    uint8_t value;

    value = mhc5833_readRegister8(mhc, HMC5883L_REG_CONFIG_A);
    value &= 0b00011100;
    value >>= 2;

    return (hmc5883l_dataRate_t)value;
}

void mhc5883l_setSamples(MHC5883L_t *mhc, hmc5883l_samples_t samples)
{
    uint8_t value;

    value = mhc5833_readRegister8(mhc, HMC5883L_REG_CONFIG_A);
    value &= 0b10011111;
    value |= (samples << 5);

    mhc5833_writeRegister8(mhc, HMC5883L_REG_CONFIG_A, value);
}

hmc5883l_samples_t mhc5883l_getSamples(MHC5883L_t *mhc)
{
    uint8_t value;

    value = mhc5833_readRegister8(mhc, HMC5883L_REG_CONFIG_A);
    value &= 0b01100000;
    value >>= 5;

    return (hmc5883l_samples_t)value;
}

// Write byte to register
static void mhc5833_writeRegister8(MHC5883L_t *mhc, uint8_t reg, uint8_t value)
{
    uint8_t data [2] = {reg, value};
    xSemaphoreTake(mhc->mutex, 1000);
    TwiMaster_Write(mhc->twi, HMC5883L_ADDRESS, data, 2);
    xSemaphoreGive(mhc->mutex);
}

// Read byte to register
static uint8_t mhc5833_fastRegister8(MHC5883L_t *mhc, uint8_t reg)
{
    uint8_t value;
    
    //Lock
    xSemaphoreTake(mhc->mutex, 1000);
    TwiMaster_ReadAndWrite(mhc->twi, HMC5883L_ADDRESS, 1, &reg, 1, &value);
    xSemaphoreGive(mhc->mutex);
    return value;
}

// Read byte from register
static uint8_t mhc5833_readRegister8(MHC5883L_t *mhc, uint8_t reg)
{
    uint8_t value;
    xSemaphoreTake(mhc->mutex, 1000);
    TwiMaster_Write(mhc->twi, HMC5883L_ADDRESS, &reg, 1);
    TwiMaster_Read(mhc->twi, HMC5883L_ADDRESS , 1, &value);
    xSemaphoreGive(mhc->mutex);

    return value;
}

// Read word from register
static int16_t mhc5833_readRegister16(MHC5883L_t *mhc, uint8_t reg)
{
    int16_t value;
    uint8_t data[2];
    
    xSemaphoreTake(mhc->mutex, 1000);
    TwiMaster_Write(mhc->twi, HMC5883L_ADDRESS, &reg, 1);
    TwiMaster_Read(mhc->twi, HMC5883L_ADDRESS, 2, data);
    xSemaphoreGive(mhc->mutex);
    
    value = data[0] << 8 | data[1];
    return value;
}
