#ifndef SOFTWARE_CONFIG_H
#define SOFTWARE_CONFIG_H 1

#include <inttypes.h>


/* Only one language can be available */
#define LANG_EN 1
#define LANG_PL 0


/* CLI */
#define CLI_BUF_TOT_LEN              0x0080
#define CMD_STATE_HISTORY 4
#define CMD_STATE_HISTORY_MASK 0x03



//#define HEAP_BEGIN                   0x0100
//#define HEAP_END                     CLI_1_BUF_ADDR - 1
//#define HEAP_SIZE                    HEAP_END - HEAP_BEGIN + 1


char cliBuffer[CLI_BUF_TOT_LEN];

//#define configUSE_MALLOC_FAILED_HOOK 1



/* Memory Map
 0x0000 +-----------------------------+
  256   | Controll registers          |
 0x0100 +-----------------------------+
  2k    | Internal memory             |





 0x08FF +-----------------------------+
*/

#endif
