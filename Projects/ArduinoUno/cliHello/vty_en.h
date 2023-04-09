#ifndef LANG_VTY
#define LANG_VTY EN

#include<avr/pgmspace.h>
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
const char systemRamConfigStr[]        PROGMEM = "System settings:\r\n";

const char debugEnabledInfoStr[]       PROGMEM = "Enabled %s debug\r\n";
const char debugDisabledInfoStr[]      PROGMEM = "Disabled %s debug\r\n";

// *************************** Command Strings *****************************************************

const char cmd_help[]         PROGMEM = "help";             const char cmd_help_help[]         PROGMEM = "Print help string";
const char cmd_status[]       PROGMEM = "status";           const char cmd_help_status[]       PROGMEM = "{filename} Print device status on VTY or write to file";
const char cmd_enable[]       PROGMEM = "enable";           const char cmd_help_enable[]       PROGMEM = "Enable mode";
const char cmd_disable[]      PROGMEM = "disable";          const char cmd_help_disable[]      PROGMEM = "View mode";
const char cmd_configure[]    PROGMEM = "config";           const char cmd_help_configure[]    PROGMEM = "Configure mode";
const char cmd_conf_save[]    PROGMEM = "save";             const char cmd_help_conf_save[]    PROGMEM = "Save configuration";
#endif

