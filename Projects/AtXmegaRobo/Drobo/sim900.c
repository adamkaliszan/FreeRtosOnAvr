#include "sim900.h"

#include "FreeRTOS.h"
#include "task.h"


tcpConDesc_t definedTcpConnectrions[]  __attribute__((section (".eeprom"))) =
{
    { "makgywer.vipserv.org", 2002},
    { "makgywer.vipserv.org", 2002},
    { "makgywer.vipserv.org", 2002},
    { "makgywer.vipserv.org", 2002}
};

void sim900pwrOn(void)
{
  PORTR.OUTSET = 0x00;
  vTaskDelay(100);
  PORTD.DIRSET = 0x10;
  PORTD.OUTCLR = 0x10;
  vTaskDelay(150);
  PORTD.DIRCLR = 0x10;
}

void sim900pwrOffHw(void)
{
  PORTD.DIRSET = 0x10;
  PORTD.OUTCLR = 0x10;
  vTaskDelay(150);
  PORTD.DIRCLR = 0x10;
vTaskDelay(200);
}


void sim900pwrOffSoft(void)
{
  //fprintf_P(&sim900Str, PSRT("AT+CPOWD=1"));
}

void sim900reset(void)
{
  PORTD.DIRSET = 0x02;
  PORTD.OUTCLR = 0x10;
  vTaskDelay(15);
  PORTD.DIRCLR = 0x10;
}

void openTcpCon(uint8_t bufNo, uint8_t defConNo)
{
#warning "Implement it"
  (void) bufNo;
  (void) defConNo;
    //uint16_t portNo = eeprom_read_word(&definedTcpConnectrions[defConNo].dstPortNo);
    //uint8_t *eepromPtr = (uint8_t *) definedTcpConnectrions[defConNo].srcAddress;
//  eeprom_read_block()
}

void closeTcpCon(uint8_t conNo)
{
#warning "Implement it"
  (void) conNo;

}
