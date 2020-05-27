/*************************************************** 
 * 
 * 2020 Adam Kaliszan adam.kaliszan@gmail.com
 * 
 * base on:
 *   This is a library for the Adafruit BMP085/BMP180 Barometric Pressure + Temp sensor
  Designed specifically to work with the Adafruit BMP085 or BMP180 Breakout 
  ----> http://www.adafruit.com/products/391
  ----> http://www.adafruit.com/products/1603
  These displays use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ***************************************************
 
 */
#include <float.h>
#include <math.h>

#include "bmp085.h"
#include "task.h"

inline static int32_t  bmp085_computeB5(Bmp085_t *bmp, int32_t UT);
inline static uint8_t  bmp085_read8(Bmp085_t *bmp, uint8_t addr);
inline static uint16_t bmp085_read16(Bmp085_t *bmp, uint8_t addr);
inline static void     bmp085_write8(Bmp085_t *bmp, uint8_t addr, uint8_t data);


uint8_t bmp085_init(Bmp085_t *bmp, TWI_Master_t *twi, FILE *io, uint8_t mode) 
{
    bmp->twi = twi;
    if (mode > BMP085_ULTRAHIGHRES) 
        mode = BMP085_ULTRAHIGHRES;
    bmp->oversampling = mode;

    if (io != NULL)
        IO_msg("Initializing bmp085 module ");
    if (bmp085_read8(bmp, 0xD0) != 0x55)
    {
        if (io != NULL)
            IO_msg("Error, connection failed\r\n");
        
        return 0;
    }

  /* read calibration data */
    bmp->ac1 = bmp085_read16(bmp, BMP085_CAL_AC1);
    bmp->ac2 = bmp085_read16(bmp, BMP085_CAL_AC2);
    bmp->ac3 = bmp085_read16(bmp, BMP085_CAL_AC3);
    bmp->ac4 = bmp085_read16(bmp, BMP085_CAL_AC4);
    bmp->ac5 = bmp085_read16(bmp, BMP085_CAL_AC5);
    bmp->ac6 = bmp085_read16(bmp, BMP085_CAL_AC6);

    bmp->b1 = bmp085_read16(bmp, BMP085_CAL_B1);
    bmp->b2 = bmp085_read16(bmp, BMP085_CAL_B2);

    bmp->mb = bmp085_read16(bmp, BMP085_CAL_MB);
    bmp->mc = bmp085_read16(bmp, BMP085_CAL_MC);
    bmp->md = bmp085_read16(bmp, BMP085_CAL_MD);

    IO_printf("\tac1 = %d\r\n", bmp->ac1);
    IO_printf("\tac2 = %d\r\n", bmp->ac2);
    IO_printf("\tac3 = %d\r\n", bmp->ac3);
    IO_printf("\tac4 = %d\r\n", bmp->ac4);
    IO_printf("\tac5 = %d\r\n", bmp->ac5);
    IO_printf("\tac6 = %d\r\n", bmp->ac6);

    IO_printf("\tb1 = %d\r\n", bmp->b1);
    IO_printf("\tb2 = %d\r\n", bmp->b2);

    IO_printf("\tmb = %d\r\n", bmp->mb);
    IO_printf("\tmc = %d\r\n", bmp->mc);
    IO_printf("\tmd = %d\r\n", bmp->md);

    if (io != NULL)
        IO_msg("OK\r\n");

    return 1;
}

static int32_t bmp085_computeB5(Bmp085_t *bmp, int32_t UT)
{
  int32_t X1 = (UT - (int32_t) bmp->ac6) * ((int32_t) bmp->ac5) >> 15;
  int32_t X2 = ((int32_t) bmp->mc << 11) / (X1+(int32_t) bmp->md);
  return X1 + X2;
}

uint16_t bmp085_readRawTemperature(Bmp085_t *bmp)
{
    bmp085_write8(bmp, BMP085_CONTROL, BMP085_READTEMPCMD);
    vTaskDelay(5);
    return bmp085_read16(bmp, BMP085_TEMPDATA);
}

uint32_t bmp085_readRawPressure(Bmp085_t *bmp)
{
    uint32_t raw = 0;

    bmp085_write8(bmp, BMP085_CONTROL, BMP085_READPRESSURECMD + (bmp->oversampling << 6));

    if (bmp->oversampling == BMP085_ULTRALOWPOWER) 
        vTaskDelay(5);
    else if (bmp->oversampling == BMP085_STANDARD) 
        vTaskDelay(10);
    else if (bmp->oversampling == BMP085_HIGHRES) 
        vTaskDelay(5);
    else 
        vTaskDelay(10);

    raw = bmp085_read16(bmp, BMP085_PRESSUREDATA);
    raw<<=8;
    raw |= bmp085_read8(bmp, BMP085_PRESSUREDATA+2);
    raw = raw >> (8 - bmp->oversampling);

    return raw;
}


int32_t bmp085_readPressure(Bmp085_t *bmp) 
{
  int32_t UT, UP, B3, B5, B6, X1, X2, X3, p;
  uint32_t B4, B7;

  UT = bmp085_readRawTemperature(bmp);
  UP = bmp085_readRawPressure(bmp);

  
#if BMP085_DEBUG == 1
  // use datasheet numbers!
  UT = 27898;
  UP = 23843;
  bmp->ac6 = 23153;
  bmp->ac5 = 32757;
  bmp->mc = -8711;
  bmp->md = 2868;
  bmp->b1 = 6190;
  bmp->b2 = 4;
  bmp->ac3 = -14383;
  bmp->ac2 = -72;
  bmp->ac1 = 408;
  bmp->ac4 = 32741;
  bmp->oversampling = 0;
#endif

  B5 = bmp085_computeB5(bmp, UT);

#if BMP085_DEBUG == 1
//  IO_printf("X1 = %d\r\n", X1);
//  IO_printf("X2 = %d\r\n", X2);
//  IO_printf("B5 = %d\r\n", B5);
#endif

  // do pressure calcs
  B6 = B5 - 4000;
  X1 = ((int32_t) bmp->b2 * ( (B6 * B6)>>12 )) >> 11;
  X2 = ((int32_t) bmp->ac2 * B6) >> 11;
  X3 = X1 + X2;
  B3 = ((((int32_t) bmp->ac1*4 + X3) << bmp->oversampling) + 2) / 4;

#if BMP085_DEBUG == 1
//  IO_printf("B6 = %d\r\n", B6);
//  IO_printf("X1 = %d\r\n", X1);
//  IO_printf("X2 = %d\r\n", X2);
//  IO_printf("B3 = %d\r\n", B3);
#endif

  X1 = ((int32_t)bmp->ac3 * B6) >> 13;
  X2 = ((int32_t)bmp->b1 * ((B6 * B6) >> 12)) >> 16;
  X3 = ((X1 + X2) + 2) >> 2;
  B4 = ((uint32_t)bmp->ac4 * (uint32_t)(X3 + 32768)) >> 15;
  B7 = ((uint32_t)UP - B3) * (uint32_t)( 50000UL >> bmp->oversampling );

#if BMP085_DEBUG == 1
//  IO_printf("X1 = %d\r\n", X1);
//  IO_printf("X2 = %d\r\n", X2);
//  IO_printf("B4 = %d\r\n", B4);
//  IO_printf("B7 = %d\r\n", B7);
#endif

  if (B7 < 0x80000000) {
    p = (B7 * 2) / B4;
  } else {
    p = (B7 / B4) * 2;
  }
  X1 = (p >> 8) * (p >> 8);
  X1 = (X1 * 3038) >> 16;
  X2 = (-7357 * p) >> 16;

#if BMP085_DEBUG == 1
//  IO_printf("p = %d\r\n", p);
//  IO_printf("X1 = %d\r\n", X1);
//  IO_printf("X2 = %d\r\n", X2);
#endif

  p = p + ((X1 + X2 + (int32_t)3791)>>4);
#if BMP085_DEBUG == 1
//  IO_printf("p = ", p);
#endif
  return p;
}

int32_t bmp085_readSealevelPressure(Bmp085_t *bmp, float altitude_meters) 
{
  float pressure = bmp085_readPressure(bmp);
  return (int32_t)(pressure / pow(1.0-altitude_meters/44330, 5.255));
}

float bmp085_readTemperature(Bmp085_t *bmp)
{
  int32_t UT, B5;     // following ds convention
  float temp;

  UT = bmp085_readRawTemperature(bmp);

#if BMP085_DEBUG == 1
  // use datasheet numbers!
  UT = 27898;
  bmp->ac6 = 23153;
  bmp->ac5 = 32757;
  bmp->mc = -8711;
  bmp->md = 2868;
#endif

  B5 = bmp085_computeB5(bmp, UT);
  temp = (B5+8) >> 4;
  temp /= 10;
  
  return temp;
}

float bmp085_readAltitude(Bmp085_t *bmp, float sealevelPressure)
{
  float altitude;

  float pressure = bmp085_readPressure(bmp);

  altitude = 44330 * (1.0 - pow(pressure /sealevelPressure,0.1903));

  return altitude;
}


/*********************************************************************/

static uint8_t bmp085_read8(Bmp085_t *bmp, uint8_t a)
{
  uint8_t ret;
  TwiMaster_ReadAndWrite(bmp->twi, BMP085_I2CADDR, 1, &a, 1, &ret);
  return ret;
}

static uint16_t bmp085_read16(Bmp085_t *bmp, uint8_t a)
{
  uint8_t dta[2];
  TwiMaster_ReadAndWrite(bmp->twi, BMP085_I2CADDR, 1, &a, 2, dta);
  
  uint16_t result = (dta[0]<<8) | dta[1];
  return result;
}

static void bmp085_write8(Bmp085_t *bmp, uint8_t a, uint8_t d)
{
    uint8_t data[2]; // = {a, d};
    data[0] = a;
    data[1] = d;
    TwiMaster_Write(bmp->twi, BMP085_I2CADDR, data, 2);
}
