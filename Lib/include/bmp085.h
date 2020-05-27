/*
 * 2020 Adam Kaliszan adam.kaliszan@gmail.com
 * 
 * base on: 
 ************************************************** 
  This is a library for the Adafruit BMP085/BMP180 Barometric Pressure + Temp sensor
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
 ****************************************************/

#ifndef BMP085_H
#define BMP085_H

#include <stdint.h>
#include <float.h>
#include <stdio.h>

#include "../../drv/include/twi.h"
#include "cmdline.h"

#define BMP085_DEBUG 0

#define BMP085_I2CADDR 0x77

#define BMP085_ULTRALOWPOWER 0
#define BMP085_STANDARD      1
#define BMP085_HIGHRES       2
#define BMP085_ULTRAHIGHRES  3
#define BMP085_CAL_AC1           0xAA  // R   Calibration data (16 bits)
#define BMP085_CAL_AC2           0xAC  // R   Calibration data (16 bits)
#define BMP085_CAL_AC3           0xAE  // R   Calibration data (16 bits)    
#define BMP085_CAL_AC4           0xB0  // R   Calibration data (16 bits)
#define BMP085_CAL_AC5           0xB2  // R   Calibration data (16 bits)
#define BMP085_CAL_AC6           0xB4  // R   Calibration data (16 bits)
#define BMP085_CAL_B1            0xB6  // R   Calibration data (16 bits)
#define BMP085_CAL_B2            0xB8  // R   Calibration data (16 bits)
#define BMP085_CAL_MB            0xBA  // R   Calibration data (16 bits)
#define BMP085_CAL_MC            0xBC  // R   Calibration data (16 bits)
#define BMP085_CAL_MD            0xBE  // R   Calibration data (16 bits)

#define BMP085_CONTROL           0xF4 
#define BMP085_TEMPDATA          0xF6
#define BMP085_PRESSUREDATA      0xF6
#define BMP085_READTEMPCMD          0x2E
#define BMP085_READPRESSURECMD            0x34


typedef struct Bmp085
{
    TWI_Master_t *twi;
    uint8_t oversampling;
    int16_t ac1, ac2, ac3, b1, b2, mb, mc, md;
    uint16_t ac4, ac5, ac6;
} Bmp085_t;

uint8_t  bmp085_init(Bmp085_t *bmp, TWI_Master_t *twi, FILE *io, uint8_t mode);// = BMP085_ULTRAHIGHRES);  // by default go highres
float    bmp085_readTemperature(Bmp085_t *bmp);
int32_t  bmp085_readPressure(Bmp085_t *bmp);
int32_t  bmp085_readSealevelPressure(Bmp085_t *bmp, float altitude_meters);
float    bmp085_readAltitude(Bmp085_t *bmp, float sealevelPressure);// = 101325); // std atmosphere
uint16_t bmp085_readRawTemperature(Bmp085_t *bmp);
uint32_t bmp085_readRawPressure(Bmp085_t *bmp);
  



#endif //  BMP085_H