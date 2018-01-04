#include "queueStream.h"

static int streamQueueOutputFun(char c, FILE *ostream);
static int streamQueueInputFun(FILE *istream);

void initQueueStream(FILE *stream, streamBuffers_t *buffer, xQueueHandle Rx, xQueueHandle Tx)
{
  fdev_setup_stream(stream, streamQueueOutputFun, streamQueueInputFun, _FDEV_SETUP_RW);
  fdev_set_udata(stream, (void *)buffer);
  buffer->Rx = Rx;
  buffer->Tx = Tx;
  return;
}

static int streamQueueOutputFun(char c, FILE *ostream)
{
  streamBuffers_t *strBuf = (streamBuffers_t *) ostream->udata;
/*  if (xQueueSend( strBuf->Tx, &c, strBuf->tx_timeout))
  {
    return EOF;
  }
  else*/
  xQueueSend( strBuf->Tx, &c, portMAX_DELAY);
  return 0;
}

static int streamQueueInputFun(FILE *istream)
{
  char c;
  streamBuffers_t *strBuf = (streamBuffers_t *) istream->udata;
  xQueueReceive(strBuf->Rx, &c, portMAX_DELAY);
  return c;
}
