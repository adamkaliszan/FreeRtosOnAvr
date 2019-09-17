#ifndef LANG_VTY
#define LANG_VTY EN

#include<avr/pgmspace.h>
// *************************** Error Strings *******************************************************

const char errorOK[]                                   PROGMEM = "All OK\r\n";

// *************************** Message Strings *****************************************************

const char systemStateStr[]            PROGMEM = "System state:\r\n";
const char statusNumberOfTasksStr[]    PROGMEM = "  Number of tasks : %d\r\n";
const char statusStaticHeapStateStr[]  PROGMEM = "  FreeRtos heap   : %d free of %d bytes\r\n";
const char statusDynamicHeapStateStr[] PROGMEM = "  Malloc heap     : %d free of %d bytes\r\n";
const char statusRamDiskStateStr[]     PROGMEM = "  Ram disc space  : %d free of %d clusters\r\n";
const char statusTemperatureStr[]      PROGMEM = "  Temperature     : %d C\r\n";
const char statusVoltageStr[]          PROGMEM = "  Voltage         : %d V\r\n";
const char systemRamConfigStr[]        PROGMEM = "System settings:\r\n";
const char statusMacStr[]              PROGMEM = "  Mac address     : ";
const char statusIpStr[]               PROGMEM = "  IP address      : ";
const char statusIpMaskStr[]           PROGMEM = "  mask            : ";
const char statusIpGwStr[]             PROGMEM = "  gateway         : ";

const char debugEnabledInfoStr[]       PROGMEM = "Enabled %s debug\r\n";
const char debugDisabledInfoStr[]      PROGMEM = "Disabled %s debug\r\n";

// *************************** Command Strings *****************************************************

const char cmd_help[]         PROGMEM = "help";             const char cmd_help_help[]         PROGMEM = "Print help string";
const char cmd_status[]       PROGMEM = "status";           const char cmd_help_status[]       PROGMEM = "{filename} Print device status on VTY or write to file";
const char cmd_enc_stat[]     PROGMEM = "encstat";          const char cmd_help_enc_stat[]     PROGMEM = "Print Enc 28j60 registers";
const char cmd_time[]         PROGMEM = "time";             const char cmd_help_time[]         PROGMEM = "Print time";
const char cmd_net_dbg[]      PROGMEM = "debug";            const char cmd_help_net_dbg[]      PROGMEM = "[net|arp|icmp|ip|tcp|udp] [level] write debug info. Level 0 disable debugging";

const char cmd_net_test[]     PROGMEM = "ntest";            const char cmd_help_net_test[]     PROGMEM = "[serialNo] {msg} forward fake message from serial to net";
const char cmd_rs_test[]      PROGMEM = "rtest";            const char cmd_help_rs_test[]      PROGMEM = "[serialNo] {msg} forward fake message from net to serial";


const char cmd_res_sl[]       PROGMEM = "reset";            const char cmd_help_res_sl[]       PROGMEM = "Restart slave devices";

const char cmd_ping[]         PROGMEM = "ping";             const char cmd_help_ping[]         PROGMEM = "[A1] [A2] [A3] [A4] Sends ping throught ethernet";
const char cmd_xRec[]         PROGMEM = "xrec";             const char cmd_help_xRec[]         PROGMEM = "[file name] receive file using xModem";
const char cmd_xSend[]        PROGMEM = "xsend";            const char cmd_help_xSend[]        PROGMEM = "[file name] send file using xModem";
const char cmd_xflash[]       PROGMEM = "xflash";           const char cmd_help_xflash[]       PROGMEM = "[device no] [file name] flash device connected to Rs485";
const char cmd_dir_rf[]       PROGMEM = "dirrf";            const char cmd_help_dir_rf[]       PROGMEM = "Print ramdisk files";
const char cmd_create_rf[]    PROGMEM = "crf";              const char cmd_help_create_rf[]    PROGMEM = "[file name] create ram file";
const char cmd_erase_rf[]     PROGMEM = "eraserf";          const char cmd_help_erase_rf[]     PROGMEM = "[file name] erase file from ram disk";
const char cmd_edit_rf[]      PROGMEM = "editrf";           const char cmd_help_edit_rf[]      PROGMEM = "[file name] edit file located on ram disk";
const char cmd_read_rf[]      PROGMEM = "readrf";           const char cmd_help_read_rf[]      PROGMEM = "[file name] read file located on ram disk";

const char cmd_up[]           PROGMEM = "up";               const char cmd_help_up[]           PROGMEM = "[driver no] [channel] {value} move up";
const char cmd_down[]         PROGMEM = "down";             const char cmd_help_down[]         PROGMEM = "[driver no] [channel] {value} move down";
const char cmd_spa[]          PROGMEM = "spa";              const char cmd_help_spa[]          PROGMEM = "[value] set port A";
const char cmd_spb[]          PROGMEM = "spb";              const char cmd_help_spb[]          PROGMEM = "[value] set port B";

const char cmd_settime[]      PROGMEM = "settime";          const char cmd_help_settime[]      PROGMEM = "[h] [m] [s] set time (24h format)";
const char cmd_ac[]           PROGMEM = "ac";               const char cmd_help_ac[]           PROGMEM = "[channel 0-7] read analog value";
const char cmd_enable[]       PROGMEM = "enable";           const char cmd_help_enable[]       PROGMEM = "Enable mode";
const char cmd_disable[]      PROGMEM = "disable";          const char cmd_help_disable[]      PROGMEM = "View mode";
const char cmd_configure[]    PROGMEM = "config";           const char cmd_help_configure[]    PROGMEM = "Configure mode";
const char cmd_conf_ip[]      PROGMEM = "ip";               const char cmd_help_conf_ip[]      PROGMEM = "[A1] [A2] [A3] [A4] set IP address";
const char cmd_conf_udp[]     PROGMEM = "udp";              const char cmd_help_conf_udp[]     PROGMEM = "[A1] [A2] [A3] [A4] [src port] {dst port} set udp client IP address and ports";
const char cmd_conf_ip_mask[] PROGMEM = "mask";             const char cmd_conf_ip_mask_help[] PROGMEM = "[mask] set mask";
const char cmd_conf_ip_gw[]   PROGMEM = "gw";               const char cmd_conf_ip_gw_help[]   PROGMEM = "[A1] [A2] [A3] [A4] set default gateway";
const char cmd_conf_mac[]     PROGMEM = "mac";              const char cmd_help_conf_mac[]     PROGMEM = "[A1] [A2] [A3] [A4] [A5] [A6] set MAC address";
const char cmd_conf_NoOfRs[]  PROGMEM = "maxser";           const char cmd_help_conf_NoOfRs[]  PROGMEM = "Maximum number of serial port [0-15]";
const char cmd_conf_save[]    PROGMEM = "save";             const char cmd_help_conf_save[]    PROGMEM = "Save configuration";
const char cmd_ustawR[]       PROGMEM = "setr";             const char cmd_help_ustawR[]       PROGMEM = "[value] set resistance value";

const char cmd_ustawMW[]      PROGMEM = "rset";             const char cmd_help_ustawMW[]      PROGMEM = "[A] [C] set execution module";
const char cmd_zapiszMW[]     PROGMEM = "rsave";            const char cmd_help_zapiszMW[]     PROGMEM = "[A] save execution module settings";

#endif
