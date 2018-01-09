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
} commandType_t;



//MSG type = 1

typedef struct
{
  uint8_t          duration;
  uint8_t          pwmLeft;
  uint8_t          pwmRight;
}  tlvMsgMoveDta_t;

typedef struct
{
  tlvMsg_t         header;
  tlvMsgMoveDta_t  data;
} tlvMsgMove_t;



#endif // TLV
