#ifndef LANG_VTY
#define LANG_VTY EN

#include <avr/pgmspace.h>

// *************************** Error Strings *******************************************************

const char errorOK[]                                   PROGMEM = "All OK\r\n";
const char errorNoFile[]                               PROGMEM = "No File\r\n";
const char errorxModemFrameStartTimeout[]              PROGMEM = "\r\n";
const char errorxModemByteSendTimeout[]                PROGMEM = "\r\n";
const char errorxModemWrongFrameNo[]                   PROGMEM = "\r\n";
const char errorxModemFrameFrameNoCorrectionNotMatch[] PROGMEM = "\r\n";
const char errorxModemFrameCrc[]                       PROGMEM = "xModem CRC error\r\n";
const char errorxModemRemoteSideCan[]                  PROGMEM = "Remote side cancelled at frame no %d\r\n";
const char errorxModemUnknownResponse[]                PROGMEM = "xModem unknown response 0x%x\r\n";
const char errorNoRemoteDevice[]                       PROGMEM = "Device %d is not responding (%d)\r\n";
const char errorBootloaderNotResponding[]              PROGMEM = "Bootloader is not responding\r\n";
const char errorOpenFile[]                             PROGMEM = "Can't open file %s\r\n";

// *************************** Message Strings *****************************************************

const char systemStateStr[]            PROGMEM = "System state:\r\n";
const char statusNumberOfTasksStr[]    PROGMEM = "  Number of tasks : %d\r\n";
const char statusStaticHeapStateStr[]  PROGMEM = "  FreeRtos heap   : %d free of %d bytes\r\n";
const char statusDynamicHeapStateStr[] PROGMEM = "  Malloc heap     : %d free of %d bytes\r\n";
const char statusRamDiskStateStr[]     PROGMEM = "  Ram disc space  : %d free of %d clusters\r\n";
const char statusTemperatureStr[]      PROGMEM = "  Temperature     : %d C\r\n";
const char statusVoltageStr[]          PROGMEM = "  Voltage         : %d V\r\n";
const char systemRamConfigStr[]        PROGMEM = "System settings:\r\n";



// *************************** Command Strings *****************************************************

const char cmd_help[]         PROGMEM = "help";     const char cmd_help_help[]          PROGMEM = "Print help string";
const char cmd_status[]       PROGMEM = "status";   const char cmd_help_status[]        PROGMEM = "{filename} Print device status on VTY or write to file";

const char cmd_enable[]       PROGMEM = "enable";   const char cmd_help_enable[]        PROGMEM = "Admin mode";
const char cmd_disable[]      PROGMEM = "disable";  const char cmd_help_disable[]       PROGMEM = "Normal mode";
const char cmd_configure[]    PROGMEM = "config";   const char cmd_help_configure[]     PROGMEM = "Configuration mode";

const char cmd_HC12mode[]     PROGMEM = "hc12mode"; const char cmd_help_HC12mode[]      PROGMEM = "[1-3] Set HC12 operation mode";
const char cmd_HC12channel[]  PROGMEM = "hc12chan"; const char cmd_help_HC12channel[]   PROGMEM = "[1-100] Set transmission channel for HC12";
const char cmd_HC12baudrate[] PROGMEM = "hc12baud"; const char cmd_help_HC12baudrate[]  PROGMEM = "[] Set HC12 bandwidth";
const char cmd_HC12power[]    PROGMEM = "hc12pwr";  const char cmd_help_HC12power[]     PROGMEM = "[1-8] Set HC12 power";
const char cmd_HC12status[]   PROGMEM = "hc12stat"; const char cmd_help_HC12status[]    PROGMEM = "Get HC-12 status";

const char cmd_pwm_freq[]     PROGMEM = "pwm";      const char cmd_help_pwm_freq[]      PROGMEM = "[4-7] Set PWM prescaler 4: 40 kHz, 7: 320 Hz";

const char cmd_forward[]      PROGMEM = "fw";       const char cmd_help_forward[]       PROGMEM = "{left PW} {right PW} move forward";
const char cmd_backward[]     PROGMEM = "bw";       const char cmd_help_backward[]      PROGMEM = "{left PW} {right PW} move backward";
const char cmd_rotateLeft[]   PROGMEM = "rl";       const char cmd_help_rotateLeft[]    PROGMEM = "{left PW} {right PW} rotate left";
const char cmd_rotateRight[]  PROGMEM = "rr";       const char cmd_help_rotateRight[]   PROGMEM = "{left PW} {right PW} rotate right";
const char cmd_stop[]         PROGMEM = "stop";     const char cmd_help_stop[]          PROGMEM = "Disable H-bridges";

const char cmd_pwr[]          PROGMEM = "pwr";      const char cmd_help_pwr[]           PROGMEM = "Power [src no] [0, 1]";


const char cmd_hc12forward[]      PROGMEM = "hfw";       const char cmd_help_hc12forward[]       PROGMEM = "{left PW} {right PW} move forward";
const char cmd_hc12backward[]     PROGMEM = "hbw";       const char cmd_help_hc12backward[]      PROGMEM = "{left PW} {right PW} move backward";
const char cmd_hc12rotateLeft[]   PROGMEM = "hrl";       const char cmd_help_hc12rotateLeft[]    PROGMEM = "{left PW} {right PW} rotate left";
const char cmd_hc12rotateRight[]  PROGMEM = "hrr";       const char cmd_help_hc12rotateRight[]   PROGMEM = "{left PW} {right PW} rotate right";
const char cmd_hc12stop[]         PROGMEM = "hstop";     const char cmd_help_hc12stop[]          PROGMEM = "Disable H-bridges";

const char cmd_sim900on[]         PROGMEM = "son";       const char cmd_help_sim900on[]          PROGMEM = "Sim 900 power on";
const char cmd_sim900off[]        PROGMEM = "soff";      const char cmd_help_sim900off[]         PROGMEM = "Sim 900 power off";
const char cmd_sim900at[]         PROGMEM = "sat";       const char cmd_help_sim900at[]          PROGMEM = "Foreword Sim 900 port";

const char cmd_conf_save[]    PROGMEM = "save";     const char cmd_help_conf_save[]     PROGMEM = "Save configuration";

#endif
