/*********************************************
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher 
 * Copyright: GPL V2
 * http://www.gnu.org/licenses/gpl.html
 *
 * Based on the enc28j60.c file from the AVRlib library by Pascal Stang.
 * For AVRlib See http://www.procyonengineering.com/
 * Used with explicit permission of Pascal Stang.
 *
 * Title: Microchip ENC28J60 Ethernet Interface Driver
 * Chip type           : ATMEGA88 with ENC28J60
 *********************************************/
#include <avr/io.h>
#include <util/delay.h>
#include "ds1305.h"
#include "spiXmega.h"

void spiEnableDS1305(void)  {};
void spiDisableDS1305(void) {};


void readTimeBCD(timeBCD_t *time)
{
  spiTake();
  spiEnableDS1305();
  
  uint8_t *ptr = (uint8_t *)(time);
  uint8_t i;
  
  spiSend(0x00);

  for (i=0; i<sizeof(timeBCD_t); i++)
  {
    *ptr = spiSend(i);
    ptr++;
  }  
  spiDisableDS1305();  
  spiGive();
}

#if USE_DECODED_TIME_STRUCT
void readTimeDecoded(timeDecoded_t *time)
{
  readTimeBCD((timeBCD_t *)(time));
}
void readTime (time_t *time)
{
  readTimeBCD((timeBCD_t *)(time));
}
#endif /* USE_DECODED_TIME_STRUCT */


void setTimeBCD(timeBCD_t *time)
{
  spiTake();
  spiEnableDS1305();
  
  uint8_t *ptr = (uint8_t *)(time);
  uint8_t i;
  spiSend(0x80);
  for (i=0; i<sizeof(timeBCD_t); i++)
  {
    spiSend(*ptr);
    ptr++;
  }  
  
  spiDisableDS1305();  
  spiGive();
}

#if USE_DECODED_TIME_STRUCT
void setTimeDecoded(timeDecoded_t *time)
{
  setTimeBCD((timeBCD_t *)(time));
}
void setTime(time_t *time)
{
  setTimeBCD((timeBCD_t *)(time));
}
#endif /* USE_DECODED_TIME_STRUCT */

void ds1305start(void)
{  
  spiTake();
  spiEnableDS1305();

  spiSend(0x8F);
  spiSend(0x00);
  
  spiDisableDS1305();  
  spiGive();

  return;
}


uint8_t ds1305writeMem      (uint8_t addr, uint8_t length, uint8_t *data)
{
  if (addr > 95)
    return 1;
  if (addr + length > 95)
    return 2;

  addr += 0xA0;
  
  spiTake();
  spiEnableDS1305();

  spiSend(addr);
  while (length > 0)
  {
    spiSend(*data);
    data++;
    length--;
  }
  
  spiDisableDS1305();  
  spiGive();
  return 0;
}
uint8_t ds1305readMem       (uint8_t addr, uint8_t length, uint8_t *data)
{
  if (addr >95)
    return 1;
  if (addr + length > 95)
    return 2;
  
  addr += 0x20;
  
  spiTake();
  spiEnableDS1305();

  spiSend(addr);
  while (length > 0)
  {
    *data = spiSend(0);
    data++;
    length--;
  }
  
  spiDisableDS1305();  
  spiGive();
  return 0;
}
