#include "vt100.h"

// defines
#define ASCII_BEL           0x07
#define ASCII_BS            0x08
#define ASCII_CR            0x0D
#define ASCII_LF            0x0A
#define ASCII_ESC           0x1B
#define ASCII_DEL           0x7F

#define VT100_ARROWUP       'A'
#define VT100_ARROWDOWN     'B'
#define VT100_ARROWRIGHT    'C'
#define VT100_ARROWLEFT     'D'
#define VT100_ATTR_OFF      0
#define VT100_BOLD          1
#define VT100_USCORE        4
#define VT100_BLINK         5
#define VT100_REVERSE       7
#define VT100_BOLD_OFF      21
#define VT100_USCORE_OFF    24
#define VT100_BLINK_OFF     25
#define VT100_REVERSE_OFF   27


int strToVt100(const char *srcBuf, int srcBufLen, char *dstBuf, int maxDstBufLen)
{
  int result = 0;
  int i;
  for (i=0; i < srcBufLen; i++)
  {
    switch (*srcBuf)
    {
      case ASCII_LF:
      case ASCII_CR:
//        *dstBuf++ = ASCII_ESC;
//        *dstBuf++ = '[';
        *dstBuf++ = ASCII_CR;
//        result +=3;
        result ++;
        break;
      case ASCII_DEL:
//        *dstBuf++ = ASCII_ESC;
//        *dstBuf++ = '[';
        *dstBuf++ = ASCII_BS;
//        result +=3;
        result ++;
        break;
      default:
        *dstBuf++ = *srcBuf;
        result++;
        break;
    }
    srcBuf++;
  }
  return result;  
}

int vt100Tostr(const char *scrBuf, int srcBufLen, char *dstBuf, int maxDstBufLen)
{
  strncpy(dstBuf, scrBuf, srcBufLen);
  return srcBufLen;  
}

