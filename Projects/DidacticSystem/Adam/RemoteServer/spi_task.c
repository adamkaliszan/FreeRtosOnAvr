#include "spi_task.h"
#include "softwareConfig.h"


uint8_t * const spiTxbuffer0 = (uint8_t *) SPI_TX_BUFFER;
uint8_t * const spiTxbuffer1 = (uint8_t *) (SPI_TX_BUFFER + 512);
uint8_t * const spiRxbuffer0 = (uint8_t *) SPI_RX_BUFFER;
uint8_t * const spiRxbuffer1 = (uint8_t *) (SPI_RX_BUFFER + 512);


#if NET_DEBUG
void setNetDebug(FILE *stream, uint8_t level)
{
  netDebug = stream;
  netDebugLevel = level;
  if (level == 0)
    netDebug = NULL;
}
#endif /* NET_DEBUG */

uint16_t powerOff = 0;
uint16_t needToBeUpdated = 0;

void XpNetReceiverTask ( void *pvParameters )
{
  (void) pvParameters;
  enum REC_STATE recState = ST_SYNC;
  uint8_t bufNo  = 0;
  uint8_t dtaLen = 0;
  uint8_t crcLo  = 0;
  uint8_t crcHi  = 0;
  uint16_t crc   = 0;

  for( ; ; )
  {
    uint8_t dta;

    if (!xQueueReceive(udpSocket->Rx, &dta, 100))
      continue;

    switch (recState)
    {
    case ST_SYNC:
      if (dta == 0xA5)
        recState = ST_CMD;
      crc = 0;
      break;

    case ST_CMD:
      crc = _crc16_update(crc, dta);
      recState = ST_LEN;
      bufNo = dta;
      break;

    case ST_LEN:
      crc = _crc16_update(crc, dta);
      recState = ST_DATA;
      dtaLen=dta;
      break;

    case ST_DATA:
      crc = _crc16_update(crc, dta);
      if (bufNo < NO_OF_SPI2SERIAL_RX_BUF)
      {
        xQueueSend(xSpi2SerialTx[bufNo], &dta, 0);
      } //dopisać obsługę resetowania

      if (bufNo == POWER_OFF)
      {
          if (dta < NO_OF_SPI2SERIAL_RX_BUF)
          {
            powerOff        |= (1<<dta);
            needToBeUpdated |= (1<<dta);
          }
      }
      if (bufNo == POWER_ON)
      {
          if (dta < NO_OF_SPI2SERIAL_RX_BUF)
          {
            powerOff        &= (~(1<<dta));
            needToBeUpdated |= (1<<dta);
          }
      }

      dtaLen--;
      if (dtaLen == 0)
      {
        recState = ST_CRC_HI;
      }
      break;
    case ST_CRC_HI:
      crcHi = dta;
      recState = ST_CRC_LO;
      break;

    case ST_CRC_LO:
      crcLo = dta;
      recState = ST_SYNC;
#if NET_DEBUG
      if ((uint8_t)(crcLo + (crcHi<<8)) != crc)
        if (netDebug != NULL)
          fprintf_P(netDebug, PSTR("CRC error"));
#endif // NET_DEBUG
      break;
    }
  }
}

void XpNetTransmitterTask ( void *pvParameters )
{
  (void) pvParameters;
  uint8_t bufIdx;

  for(bufIdx=0; ; bufIdx++)
  {
    if (bufIdx == NO_OF_SPI2SERIAL_RX_BUF)
    {
      bufIdx = 0;
      vTaskDelay ( 0 );
    }

    uint16_t crc = 0;
    uint8_t dta = 0xA5;;
    uint8_t dtaLen = uxQueueMessagesWaiting(xSpi2SerialRx[bufIdx]);

    if (dtaLen == 0)
      continue;

#if NET_DEBUG
    if (netDebug != NULL)
      fprintf_P(netDebug, PSTR("%d of bytes in waiting on qeue %d\r\n"), dtaLen, bufIdx);
#endif // NET_DEBUG

    crc = _crc16_update(crc, dtaLen);
    xQueueSend(udpSocket->Tx, &dta, 0);     //SYNC - 1bajt ma zawsze wartość 0xA5
    crc = _crc16_update(crc, dta);
    xQueueSend(udpSocket->Tx, &bufIdx, 0);  //TYPE - 1 bajt, który  określa numer portu lub polecenie. Polecenia: 254 wyłącz zasilanie, 255 włącz zasilanie
    crc = _crc16_update(crc, dta);
    xQueueSend(udpSocket->Tx, &dtaLen, 0);  //LENGTH - 1 bajt  który określa długość wiadomości (wiadomość nie może przekrczać 200 bajtów bo mam mało pamięci w CPU)
    crc = _crc16_update(crc, dta);
    while (dtaLen >0)                       //WIADOMOSC - no dane, które odebrano lub mają być wysłane pod określony port szeregowy
    {
        xQueueReceive(xSpi2SerialRx[bufIdx], &dta, 0);
        xQueueSend(udpSocket->Tx, &dta, 0);
        crc = _crc16_update(crc, dta);
        dtaLen--;
    }
    dta = crc>>8;
    xQueueSend(udpSocket->Tx, &dta, 0);

    dta = crc;
    xQueueSend(udpSocket->Tx, &dta, 0);
  }
}

void spiTask ( void *pvParameters )
{
  (void) pvParameters;


  nicInit();
  arpInit();
  icmpInit();


  uint16_t plen;


  //TODO    init_ip_arp_udp_tcp (mymac, ipGetConfig()->ip, MYWWWPORT);

  uint8_t spiDevIdx = 0;
  for ( ; ; )
  {
    vTaskDelay ( 0 );         //Zastąpić oczekiwaniem na zwolnienie semafora. Semafor zostaje zwolniony po odebraniu przerwania od ENC

    // get the next new packet:
    plen = nicPoll();
    /*plen will ne unequal to zero if there is a valid
    * packet (without crc error) */
    if ( plen==0 )
    {
      handleSpiDev(spiDevIdx);

      spiDevIdx++;
      if (spiDevIdx == NoOfSpiSlaves)
        spiDevIdx = 0; //spiDevIdx &= 0x07;

      flushUdpQueues();
      continue;
    }

    if(nicState.layer2.ethHeader->type == htons(ETHTYPE_IP))             // process an IP packet
    {
      arpIpIn();
      netstackIPv4Process();
    }
    else if(nicState.layer2.ethHeader->type == htons(ETHTYPE_ARP))       // process an ARP packet
    {
      arpArpIn();
    }
  }
}

void handleSpiDev(uint8_t spiDevNo)
{
  uint8_t *bufTxPtr;

  uint8_t bufTx0_len=0;
  uint8_t bufTx1_len=0;

  uint8_t bufRx0_len=0;
  uint8_t bufRx1_len=0;

  uint8_t tmpDta;
  uint8_t tmpDtaLo;
  uint8_t tmpDtaHi;

  uint8_t qeueNo;

  uint8_t i;
  uint8_t isDataOnSlave = 1;

  ///Przygotownanie bufora nadawczego do SPI.
  qeueNo = 2*spiDevNo;
  bufTxPtr = spiTxbuffer0;

//if ((needToBeUpdated & (1<<qeueNo)) != 0)
//{
//  if ((powerOff & (1<<qeueNo)) != 0)
//    i = 0xC0;
//  else
//    i = 0xE0;
//  *(bufTxPtr++) = i;
//  *(bufTxPtr++) = i;
//  bufTx0_len += 2;
//  needToBeUpdated &= (~(1<<qeueNo));
//}

  while (xQueueReceive(xSpi2SerialTx[qeueNo], &tmpDta, 0))
  {
    tmpDtaLo = (tmpDta & 0x0F);
    tmpDtaHi = ((tmpDta>>4) & 0x0F) | 0x10;

    *(bufTxPtr++) = tmpDtaLo;
    *(bufTxPtr++) = tmpDtaHi;
    bufTx0_len += 2;
  }
  qeueNo++;
  bufTxPtr = spiTxbuffer1;

//if ((needToBeUpdated & (1<<qeueNo)) != 0)
//{
//  if ((powerOff & (1<<qeueNo)) != 0)
//    i = 0xD0;
//  else
//    i = 0xF0;

//  *(bufTxPtr++) = i;
//  *(bufTxPtr++) = i;
//  bufTx1_len += 2;
//  needToBeUpdated &= (~(1<<qeueNo));
//}

  while (xQueueReceive(xSpi2SerialTx[qeueNo], &tmpDta, 0))
  {
    tmpDtaLo = (tmpDta & 0x0F) | 0x20;
    tmpDtaHi = ((tmpDta>>4) & 0x0F) | 0x30;

    *(bufTxPtr++) = tmpDtaLo;
    *(bufTxPtr++) = tmpDtaHi;
    bufTx1_len +=  2;
  }


  ///Wysłanie zawartości bufora TxSpi oraz odczyt od modułów slave
  spiEnableDev(spiDevNo);

#define SPI_TRANSMISSION(BUFNO)              \
  {                                          \
    tmpDta = spiSend(spiTxbuffer##BUFNO[i]); \
    if ((tmpDta & 0xC0) == 0xC0)             \
    {                                        \
        isDataOnSlave = 0;                   \
        continue;                            \
    }                                        \
    if ((tmpDta & 0x20) == 0)                \
      spiRxbuffer0[bufRx0_len++] = tmpDta;   \
    else                                     \
      spiRxbuffer1[bufRx1_len++] = tmpDta;   \
                                             \
    if (bufRx0_len==254) break;              \
    if (bufRx1_len==254) break;              \
  }

  for (i=0; i<bufTx0_len; i++)
    SPI_TRANSMISSION(0)

  for (i=0; i<bufTx1_len; i++)
    SPI_TRANSMISSION(1)

  spiTxbuffer0[i] = 0x8F;
  while(isDataOnSlave)
    SPI_TRANSMISSION(0)
#undef SPI_TRANSMISSION
  spiDisableDev(spiDevNo);


  ///Przetwarzanie danych odebranych z SPI i umieszczanie ich w odpowiednim buforze
  qeueNo = 2*spiDevNo;
  for (i=0; i<bufRx0_len; i+=2)
  {
      uint8_t tmpValue = ((spiRxbuffer0[i] & 0x0F) | ((spiRxbuffer0[i+1]<<4) & 0xF0));
      xQueueSend(xSpi2SerialRx[qeueNo], &tmpValue, 0);
  }
  qeueNo++;
  for (i=0; i<bufRx1_len; i+=2)
  {
      uint8_t tmpValue = ((spiRxbuffer1[i] & 0x0F) | ((spiRxbuffer1[i+1]<<4) & 0xF0));
      xQueueSend(xSpi2SerialRx[qeueNo], &tmpValue, 0);
  }
}

void initQeuesSpi2Serial()
{
  portENTER_CRITICAL();
  {
      uint8_t i;
      for (i=0; i<NO_OF_SPI2SERIAL_TX_BUF; i++)
      {
        xSpi2SerialTx[i]  =   xQueueCreateExternal(255, 1, (void *)(SPI2SERIAl_TX_BUFFERS + i*OF_SPI2SERIAL_TX_BUF_SIZE));
      }
      for (i=0; i<NO_OF_SPI2SERIAL_RX_BUF; i++)
      {
        xSpi2SerialRx[i]  =   xQueueCreateExternal(255, 1, (void *)(SPI2SERIAl_RX_BUFFERS + i*OF_SPI2SERIAL_RX_BUF_SIZE));
      }
  }
  portEXIT_CRITICAL();
}
