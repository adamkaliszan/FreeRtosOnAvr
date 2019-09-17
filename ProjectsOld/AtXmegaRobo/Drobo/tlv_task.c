#include "tlv_task.h"

static void sendHc12CmdVal(const char str[], uint16_t val, FILE *errStr)
{
    vTaskDelay(25);
    fprintf_P(errStr, str, val);
    fprintf_P(&hc12Stream, str, val);
    while (xQueueReceive(xHC12Rec, &val, 10) == pdTRUE)
    {
        fputc(val, errStr);
    }
}


void vTaskTLV(void *tlvIntPtr)
{
  tlvInterpreter_t *state = (tlvInterpreter_t *)(tlvIntPtr);


  if (xSemaphoreTake(Hc12semaphore, 10) == pdTRUE)
  {
    vTaskDelay(2);
    HC12setAtMode();


    sendHc12CmdVal(PSTR("AT+FU%d\r\n"), confHC12mode,     state->errStr);

    if (confHC12channel < 10)
        sendHc12CmdVal(PSTR("AT+C00%d\r\n"),  confHC12channel,  state->errStr);
    else if (confHC12channel < 100)
        sendHc12CmdVal(PSTR("AT+C0%d\r\n"),  confHC12channel,  state->errStr);
    else
        sendHc12CmdVal(PSTR("AT+C00%d\r\n"),  confHC12channel,  state->errStr);

    sendHc12CmdVal(PSTR("AT+B%d\r\n"),  confHC12baud,     state->errStr);
    sendHc12CmdVal(PSTR("AT+P%d\r\n"),  confHC12power,    state->errStr);

    HC12setTransparentMode();
    xSemaphoreGive(Hc12semaphore );
  }

  uint8_t znak;
  for( ;; )
  {
    vTaskDelay(0);
    if (xSemaphoreTake(Hc12semaphore, 5) == pdFALSE)
      continue;

    if( xQueueReceive(xHC12Rec, &znak, 0) == pdFALSE)
    {
      xSemaphoreGive(Hc12semaphore);
      continue;
    }

    HC12setTransparentMode();
    do
    {
      fprintf_P(state->errStr, PSTR("0x%02x "), znak);
      tlvProcessDta(state, znak);
    }
    while( xQueueReceive(xHC12Rec, &znak, 1) == pdTRUE);

    xSemaphoreGive(Hc12semaphore);
  }
}

