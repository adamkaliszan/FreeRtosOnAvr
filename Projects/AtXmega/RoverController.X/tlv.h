#ifndef TLV
#define TLV


#include "../../../Lib/include/tlvProt.h"
#include "hardware.h"

typedef enum
{
  FORWARD      = 1,
  BACKWORD     = 2,
  ROTATE_LEFT  = 3,
  ROTATE_RIGHT = 4,
  STOP         = 6,
  PING         = 7,
} CommandType_t;



//MSG type = 1

typedef struct
{
  uint8_t          duration;
  uint8_t          pwmLeft;
  uint8_t          pwmRight;
}  TlvMsgMoveDta_t;

typedef struct
{
  TlvMsg_t         header;
  TlvMsgMoveDta_t  data;
} TlvMsgMove_t;



#endif // TLV
