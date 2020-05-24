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

#include <stdio.h>
#include "twi.h"
#include "cVector.h"

#ifndef ADXL345_h
#define ADXL345_h

#define ADXL345_ADDRESS              (0x53)
#define ADXL345_REG_DEVID            (0x00)
#define ADXL345_REG_THRESH_TAP       (0x1D) // 1
#define ADXL345_REG_OFSX             (0x1E)
#define ADXL345_REG_OFSY             (0x1F)
#define ADXL345_REG_OFSZ             (0x20)
#define ADXL345_REG_DUR              (0x21) // 2
#define ADXL345_REG_LATENT           (0x22) // 3
#define ADXL345_REG_WINDOW           (0x23) // 4
#define ADXL345_REG_THRESH_ACT       (0x24) // 5
#define ADXL345_REG_THRESH_INACT     (0x25) // 6
#define ADXL345_REG_TIME_INACT       (0x26) // 7
#define ADXL345_REG_ACT_INACT_CTL    (0x27)
#define ADXL345_REG_THRESH_FF        (0x28) // 8
#define ADXL345_REG_TIME_FF          (0x29) // 9
#define ADXL345_REG_TAP_AXES         (0x2A)
#define ADXL345_REG_ACT_TAP_STATUS   (0x2B)
#define ADXL345_REG_BW_RATE          (0x2C)
#define ADXL345_REG_POWER_CTL        (0x2D)
#define ADXL345_REG_INT_ENABLE       (0x2E)
#define ADXL345_REG_INT_MAP          (0x2F)
#define ADXL345_REG_INT_SOURCE       (0x30) // A
#define ADXL345_REG_DATA_FORMAT      (0x31)
#define ADXL345_REG_DATAX0           (0x32)
#define ADXL345_REG_DATAX1           (0x33)
#define ADXL345_REG_DATAY0           (0x34)
#define ADXL345_REG_DATAY1           (0x35)
#define ADXL345_REG_DATAZ0           (0x36)
#define ADXL345_REG_DATAZ1           (0x37)
#define ADXL345_REG_FIFO_CTL         (0x38)
#define ADXL345_REG_FIFO_STATUS      (0x39)

#define ADXL345_GRAVITY_SUN          273.95f
#define ADXL345_GRAVITY_EARTH        9.80665f
#define ADXL345_GRAVITY_MOON         1.622f
#define ADXL345_GRAVITY_MARS         3.69f
#define ADXL345_GRAVITY_NONE         1.00f

typedef enum
{
    ADXL345_DATARATE_3200HZ    = 0b1111,
    ADXL345_DATARATE_1600HZ    = 0b1110,
    ADXL345_DATARATE_800HZ     = 0b1101,
    ADXL345_DATARATE_400HZ     = 0b1100,
    ADXL345_DATARATE_200HZ     = 0b1011,
    ADXL345_DATARATE_100HZ     = 0b1010,
    ADXL345_DATARATE_50HZ      = 0b1001,
    ADXL345_DATARATE_25HZ      = 0b1000,
    ADXL345_DATARATE_12_5HZ    = 0b0111,
    ADXL345_DATARATE_6_25HZ    = 0b0110,
    ADXL345_DATARATE_3_13HZ    = 0b0101,
    ADXL345_DATARATE_1_56HZ    = 0b0100,
    ADXL345_DATARATE_0_78HZ    = 0b0011,
    ADXL345_DATARATE_0_39HZ    = 0b0010,
    ADXL345_DATARATE_0_20HZ    = 0b0001,
    ADXL345_DATARATE_0_10HZ    = 0b0000
} adxl345_dataRate_t;

typedef enum
{
    ADXL345_INT2 = 0b01,
    ADXL345_INT1 = 0b00
} adxl345_int_t;

typedef enum
{
    ADXL345_DATA_READY         = 0x07,
    ADXL345_SINGLE_TAP         = 0x06,
    ADXL345_DOUBLE_TAP         = 0x05,
    ADXL345_ACTIVITY           = 0x04,
    ADXL345_INACTIVITY         = 0x03,
    ADXL345_FREE_FALL          = 0x02,
    ADXL345_WATERMARK          = 0x01,
    ADXL345_OVERRUN            = 0x00
} adxl345_activity_t;

typedef enum
{
    ADXL345_RANGE_16G          = 0b11,
    ADXL345_RANGE_8G           = 0b10,
    ADXL345_RANGE_4G           = 0b01,
    ADXL345_RANGE_2G           = 0b00
} adxl345_range_t;



struct Activites
{
    union
    {
        uint8_t data1;
        struct
        {
            uint8_t isOverrun:1;
            uint8_t isWatermark:1;
            uint8_t isFreeFall:1;
            uint8_t isInactivity:1;
            uint8_t isActivity:1;
            uint8_t isDoubleTap:1;
            uint8_t isTap:1;
            uint8_t isDataReady:1;   
        };
    };
    union
    {
        uint8_t data2;
        struct
        {
            uint8_t isTapOnZ:1;
            uint8_t isTapOnY:1;
            uint8_t isTapOnX:1;
            uint8_t reserved:1;
            uint8_t isActivityOnZ:1;
            uint8_t isActivityOnY:1;
            uint8_t isActivityOnX:1;
        };
    };
};

typedef struct ADXL345
{
    TWI_Master_t *twi;
    struct VectorFloat data;
} ADXL345_t;

void adxl345_init(ADXL345_t *adxl, TWI_Master_t *twi, adxl345_range_t range, FILE *io);

void adxl345_clearSettings(ADXL345_t *adxl);

void adxl345_readRaw(ADXL345_t *adxl, struct VectorUint16 *result);
void adxl345_readNormalize(ADXL345_t *adxl, struct VectorFloat *result, float gravityFactor);
void adxl345_readScaled(ADXL345_t *adxl, struct VectorFloat *result);

void adxl345_readActivites(ADXL345_t *adxl, struct Activites *result);

void adxl345_lowPassFilter(ADXL345_t *adxl, const struct VectorFloat *vector, float alpha); // = 0.5

void adxl345_setRange(ADXL345_t *adxl, adxl345_range_t range);
adxl345_range_t adxl345_getRange(ADXL345_t *adxl);

void  adxl345_setSampleRate(ADXL345_t *adxl, adxl345_dataRate_t dataRate);
adxl345_dataRate_t adxl345_getSampleRate(ADXL345_t *adxl);

void adxl345_setTapThreshold(ADXL345_t *adxl, float threshold);
float adxl345_getTapThreshold(ADXL345_t *adxl);

void adxl345_setTapDuration(ADXL345_t *adxl, float duration);
float adxl345_getTapDuration(ADXL345_t *adxl);

void adxl345_setDoubleTapLatency(ADXL345_t *adxl, float latency);
float adxl345_getDoubleTapLatency(ADXL345_t *adxl);

void adxl345_setDoubleTapWindow(ADXL345_t *adxl, float window);
float adxl345_getDoubleTapWindow(ADXL345_t *adxl);

void adxl345_setActivityThreshold(ADXL345_t *adxl, float threshold);
float adxl345_getActivityThreshold(ADXL345_t *adxl);

void adxl345_setInactivityThreshold(ADXL345_t *adxl, float threshold);
float adxl345_getInactivityThreshold(ADXL345_t *adxl);

void adxl345_setTimeInactivity(ADXL345_t *adxl, uint8_t time);
uint8_t adxl345_getTimeInactivity(ADXL345_t *adxl);

void adxl345_setFreeFallThreshold(ADXL345_t *adxl, float threshold);
float adxl345_getFreeFallThreshold(ADXL345_t *adxl);

void adxl345_setFreeFallDuration(ADXL345_t *adxl, float duration);
float adxl345_getFreeFallDuration(ADXL345_t *adxl);

void adxl345_setActivityX(ADXL345_t *adxl, uint8_t state);
uint8_t adxl345_getActivityX(ADXL345_t *adxl);
void adxl345_setActivityY(ADXL345_t *adxl, uint8_t state);
uint8_t adxl345_getActivityY(ADXL345_t *adxl);
void adxl345_setActivityZ(ADXL345_t *adxl, uint8_t state);
uint8_t adxl345_getActivityZ(ADXL345_t *adxl);
void adxl345_setActivityXYZ(ADXL345_t *adxl, uint8_t state);

void adxl345_setInactivityX(ADXL345_t *adxl, uint8_t state);
uint8_t adxl345_getInactivityX(ADXL345_t *adxl);
void adxl345_setInactivityY(ADXL345_t *adxl, uint8_t state);
uint8_t adxl345_getInactivityY(ADXL345_t *adxl);
void adxl345_setInactivityZ(ADXL345_t *adxl, uint8_t state);
uint8_t adxl345_getInactivityZ(ADXL345_t *adxl);
void adxl345_setInactivityXYZ(ADXL345_t *adxl, uint8_t state);

void adxl345_setTapDetectionX(ADXL345_t *adxl, uint8_t state);
uint8_t adxl345_getTapDetectionX(ADXL345_t *adxl);
void adxl345_setTapDetectionY(ADXL345_t *adxl, uint8_t state);
uint8_t adxl345_getTapDetectionY(ADXL345_t *adxl);
void adxl345_setTapDetectionZ(ADXL345_t *adxl, uint8_t state);
uint8_t adxl345_getTapDetectionZ(ADXL345_t *adxl);
void adxl345_setTapDetectionXYZ(ADXL345_t *adxl, uint8_t state);

void adxl345_useInterrupt(ADXL345_t *adxl, adxl345_int_t interrupt);

#endif