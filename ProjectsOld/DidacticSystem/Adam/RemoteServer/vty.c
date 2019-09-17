#include "main.h"
#include "softwareConfig.h"
#include "vty.h"
#include "hardwareConfig.h"
#include "configuration.h"
#include "net.h"
#include "ip.h"
#include "arp.h"


#if LANG_EN
#include "vty_en.h"
#endif

#if LANG_PL
#include "vty_pl.h"
#endif

#ifndef LANG_VTY
#error "Vty Language not defined"
#endif


static cliExRes_t helpFunction           (cmdState_t *state);
static cliExRes_t resetSlaves            (cmdState_t *state);
static cliExRes_t statusFunction         (cmdState_t *state);
static cliExRes_t statusEncFunction      (cmdState_t *state);

static cliExRes_t debugFunction          (cmdState_t *state);

static cliExRes_t enableFunction         (cmdState_t *state);
static cliExRes_t disableFunction        (cmdState_t *state);
static cliExRes_t configureModeFunction  (cmdState_t *state);

static cliExRes_t setIpFunction          (cmdState_t *state);
static cliExRes_t setIpMaskFunction      (cmdState_t *state);
static cliExRes_t setIpGwFunction        (cmdState_t *state);
static cliExRes_t setUdpFunction         (cmdState_t *state);

static cliExRes_t setMacAddrFunction     (cmdState_t *state);
static cliExRes_t setNoOfSerialPorts     (cmdState_t *state);

static cliExRes_t saveConfigFunction     (cmdState_t *state);


static cliExRes_t sentToUdpTestFunction  (cmdState_t *state);
static cliExRes_t sentToSpiRsTestFunction(cmdState_t *state);


const char okStr[]                        PROGMEM = "OK\r\n";
const char nlStr[]                        PROGMEM = "\r\n";
const char BladBuforaPozostaloBajtowStr[] PROGMEM = "!!! W budorze Rs485 pozostalo %d bajtow\r\n";


const  char * const errorStrings[] PROGMEM = {
  errorOK,
};

const command_t cmdListNormal[] PROGMEM =
{
  {cmd_help,      cmd_help_help,      helpFunction},
  {cmd_status,    cmd_help_status,    statusFunction},
  {cmd_enable,    cmd_help_enable,    enableFunction},
  {cmd_net_test,  cmd_help_net_test,  sentToUdpTestFunction},
  {cmd_rs_test,   cmd_help_rs_test,   sentToSpiRsTestFunction},
  {cmd_res_sl,    cmd_help_res_sl,    resetSlaves},
  {NULL, NULL, NULL}
};

const command_t cmdListEnable[] PROGMEM =
{
  {cmd_help,      cmd_help_help,      helpFunction},
  {cmd_status,    cmd_help_status,    statusFunction},
  {cmd_enc_stat,  cmd_help_enc_stat,  statusEncFunction},
  {cmd_net_dbg,   cmd_help_net_dbg,   debugFunction},
  {cmd_net_test,  cmd_help_net_test,  sentToUdpTestFunction},
  {cmd_rs_test,   cmd_help_rs_test,   sentToSpiRsTestFunction},
  {cmd_res_sl,    cmd_help_res_sl,    resetSlaves},
  {cmd_disable,   cmd_help_disable,   disableFunction},
  {cmd_configure, cmd_help_configure, configureModeFunction},
  {NULL, NULL, NULL}
};

const command_t cmdListConfigure[] PROGMEM =
{
  {cmd_help,         cmd_help_help,         helpFunction},
  {cmd_status,       cmd_help_status,       statusFunction},
  {cmd_conf_ip,      cmd_help_conf_ip,      setIpFunction},
  {cmd_conf_ip_mask, cmd_conf_ip_mask_help, setIpMaskFunction},
  {cmd_conf_NoOfRs,  cmd_help_conf_NoOfRs,  setNoOfSerialPorts},
  {cmd_conf_ip_gw,   cmd_conf_ip_gw_help,   setIpGwFunction},
  {cmd_conf_udp,     cmd_help_conf_udp,     setUdpFunction},
  {cmd_conf_mac,     cmd_help_conf_mac,     setMacAddrFunction},
  {cmd_conf_save,    cmd_help_conf_save,    saveConfigFunction},
  {cmd_enable,       cmd_help_enable,       enableFunction},
  {cmd_disable,      cmd_help_disable,      disableFunction},
  {NULL, NULL, NULL}
};

void VtyInit(cmdState_t* state, FILE *stream)
{
  cmdStateConfigure(state, (char *)(CLI_1_BUF_ADDR), CLI_BUF_TOT_LEN, stream, &cmdListNormal[0], NR_NORMAL);
}

void printErrorInfo(cmdState_t *state)
{
  if (state->errno != 0)
  {
    fprintf_P(state->myStdInOut, (const char*)(pgm_read_word(errorStrings + state->errno)), state->err1, state->err2);
  }
  state->errno = 0;
  state->err1 = 0;
  state->err2 = 0;
}

static cliExRes_t enableFunction(cmdState_t *state)
{
  if (state->cliMode != RESTRICTED_NORMAL)
  {
    state->cmdList = cmdListEnable;
    state->cliMode = NR_ENABLE;
    return OK_SILENT;
  }
  return ERROR_OPERATION_NOT_ALLOWED;
}
static cliExRes_t disableFunction(cmdState_t *state)
{
  state->cmdList = cmdListNormal;
  if (state->cliMode != RESTRICTED_NORMAL)
  {
    state->cliMode = NR_NORMAL;
  }
  return OK_SILENT;
}
static cliExRes_t configureModeFunction(cmdState_t *state)
{
  if (state->cliMode == NR_ENABLE)
  {
    state->cmdList = cmdListConfigure;
    state->cliMode = NR_CONFIGURE;
    return OK_SILENT;
  }
  return ERROR_OPERATION_NOT_ALLOWED;
}

// ************************** VTY API ***************************************************************************************
void printStatus(FILE *stream)
{
  fprintf_P(stream, PSTR(SYSTEM_NAME" ver "S_VERSION" build: "__DATE__", "__TIME__"\r\n"));
  //Print system state
  fprintf_P(stream, systemStateStr);
  fprintf_P(stream, statusNumberOfTasksStr,    uxTaskGetNumberOfTasks());
  fprintf_P(stream, statusStaticHeapStateStr,  xPortGetFreeHeapSize(), configTOTAL_HEAP_SIZE);
  fprintf_P(stream, statusDynamicHeapStateStr, xmallocAvailable(),   HEAP_SIZE);

//  printErrorInfo(state); //TODO fix and uncomment

  //Print system configuration
  fprintf_P(stream, systemRamConfigStr);

  fprintf_P(stream, statusMacStr);
  netPrintEthAddr(stream, &nicState.mac);
  fprintf_P(stream, PSTR("\r\n"));

  fprintf_P(stream, statusIpStr);
  netPrintIPAddr(stream, ipGetConfig()->ip);
  fprintf_P(stream, PSTR("\r\n"));

  fprintf_P(stream, statusIpMaskStr);
  netPrintIPAddr(stream, ipGetConfig()->netmask);
  fprintf_P(stream, PSTR("\r\n"));

  fprintf_P(stream, statusIpGwStr);
  netPrintIPAddr(stream, ipGetConfig()->gateway);
  fprintf_P(stream, PSTR("\r\n"));

  udpPrintStatus(stream);

  fprintf_P(stream, PSTR("\r\nNumber of serial ports %d\r\n"), NoOfSerialPorts);
//  arpPrintTable(stream);
}


// ************************** CLI Functions *********************************************************************************

static cliExRes_t statusFunction(cmdState_t *state)
{
  printStatus(state->myStdInOut);
  return OK_SILENT;
}

static cliExRes_t statusEncFunction(cmdState_t *state)
{
  nicRegDump(state->myStdInOut);
  return OK_SILENT;
}

static cliExRes_t debugFunction          (cmdState_t *state)
{
  if (state->argc < 2)
    return SYNTAX_ERROR;

  uint8_t level = cmdlineGetArgInt(2, state);
  const char *str = (const char*)cmdlineGetArgStr(1, state);


  if (strncmp_P(str, PSTR("net"), 3) == 0)
  {
    setNetDebug(state->myStdInOut, level);
    fprintf_P(state->myStdInOut, debugEnabledInfoStr, str);
    return OK_SILENT;
  }

  if (strncmp_P(str, PSTR("arp"), 3) == 0)
  {
    setArpDebug(state->myStdInOut, level);
    fprintf_P(state->myStdInOut, debugEnabledInfoStr, str);
    return OK_SILENT;
  }

  if (strncmp_P(str, PSTR("ip"), 2) == 0)
  {
    setIpDebug(state->myStdInOut, level);
    fprintf_P(state->myStdInOut, debugEnabledInfoStr, str);
    return OK_SILENT;
  }

  if (strncmp_P(str, PSTR("icmp"), 2) == 0)
  {
    setIcmpDebug(state->myStdInOut, level);
    fprintf_P(state->myStdInOut, debugEnabledInfoStr, str);
    return OK_SILENT;
  }

#ifdef TCP_H
  if (strncmp_P(str, PSTR("tcp"), 2) == 0)
  {
    setTcpDebug(state->myStdInOut, level);
    fprintf_P(state->myStdInOut, debugEnabledInfoStr, str);
    return OK_SILENT;
  }
#endif

  if (strncmp_P(str, PSTR("udp"), 2) == 0)
  {
    setUdpDebug(state->myStdInOut, level);
    fprintf_P(state->myStdInOut, debugEnabledInfoStr, str);
    return OK_SILENT;
  }
  return SYNTAX_ERROR;
}

static cliExRes_t setIpFunction(cmdState_t *state)
{
  if (state->argc < 4)
    return SYNTAX_ERROR;

  uint32_t ip = cmdlineGetArgInt(1, state) +
                (((uint32_t)(cmdlineGetArgInt(2, state)))<< 8) +
                (((uint32_t)(cmdlineGetArgInt(3, state)))<<16) +
                (((uint32_t)(cmdlineGetArgInt(4, state)))<<24);

  ipSetConfigIp(ip);
  return OK_SILENT;
}

static cliExRes_t setUdpFunction(cmdState_t *state)
{
  if (state->argc < 5)
    return SYNTAX_ERROR;

  uint32_t ip = cmdlineGetArgInt(1, state) +
                (((uint32_t)(cmdlineGetArgInt(2, state)))<< 8) +
                (((uint32_t)(cmdlineGetArgInt(3, state)))<<16) +
                (((uint32_t)(cmdlineGetArgInt(4, state)))<<24);
  udpSocket->dstIp = ip;

  uint16_t port = cmdlineGetArgInt(5, state);
  udpSocket->srcPort = htons(port);

  if (state->argc > 5)
  {
    port = cmdlineGetArgInt(6, state);
    udpSocket->dstPort = htons(port);
  }
  return OK_SILENT;
}


static cliExRes_t setIpMaskFunction(cmdState_t *state)
{
  if (state->argc < 1)
    return SYNTAX_ERROR;

  uint32_t mask = ((uint32_t)(0xFFFFFFFF))>>(32-cmdlineGetArgInt(1, state));

  ipSetConfigMask(mask);
  return OK_SILENT;
}


static cliExRes_t setIpGwFunction(cmdState_t *state)
{
  if (state->argc < 4)
    return SYNTAX_ERROR;

  uint32_t gw = cmdlineGetArgInt(1, state) +
                (((uint32_t)(cmdlineGetArgInt(2, state)))<< 8) +
                (((uint32_t)(cmdlineGetArgInt(3, state)))<<16)  +
                (((uint32_t)(cmdlineGetArgInt(4, state)))<<24);
  ipSetConfigGw(gw);
  return OK_SILENT;
}

static cliExRes_t setMacAddrFunction(cmdState_t *state)
{
  if (state->argc < 6)
    return SYNTAX_ERROR;

  nicState.mac.addr[0] = cmdlineGetArgHex(1, state);
  nicState.mac.addr[1] = cmdlineGetArgHex(2, state);
  nicState.mac.addr[2] = cmdlineGetArgHex(3, state);
  nicState.mac.addr[3] = cmdlineGetArgHex(4, state);
  nicState.mac.addr[4] = cmdlineGetArgHex(5, state);
  nicState.mac.addr[5] = cmdlineGetArgHex(6, state);
  nicSetMacAddress(nicState.mac.addr);
  return OK_SILENT;
}

static cliExRes_t setNoOfSerialPorts(cmdState_t *state)
{
  if (state->argc < 2)
    return SYNTAX_ERROR;

  NoOfSerialPorts = cmdlineGetArgInt(1, state);
  NoOfSpiSlaves = (NoOfSerialPorts+1)>>1;

  return OK_SILENT;
}

static cliExRes_t helpFunction(cmdState_t *state)
{
  cmdPrintHelp(state);
  return OK_SILENT;
}

static cliExRes_t saveConfigFunction(cmdState_t *state)
{
  (void) state;
  saveConfiguration();
  return OK_SILENT;
}


static cliExRes_t sentToUdpTestFunction(cmdState_t *state)
{
  if (state->argc < 1)
    return SYNTAX_ERROR;

  uint8_t rsNo = (uint8_t)(cmdlineGetArgInt(1, state));
  if (rsNo >= 16)
    return SYNTAX_ERROR;

  uint8_t tmp;
  if (state->argc >= 2)
  {
    char *msg=cmdlineGetArgStr(2, state);
    while(*msg != '\0')
    {
      xQueueSend(xSpi2SerialRx[rsNo], msg, 0);
      msg++;
    }
  }
  tmp = '\r';
  xQueueSend(xSpi2SerialRx[rsNo], &tmp, 0);
  tmp = '\n';
  xQueueSend(xSpi2SerialRx[rsNo], &tmp, 0);

  return OK_SILENT;
}

static cliExRes_t sentToSpiRsTestFunction(cmdState_t *state)
{
  if (state->argc < 1)
    return SYNTAX_ERROR;

  uint8_t rsNo = (uint8_t)(cmdlineGetArgInt(1, state));
  if (rsNo >= 16)
    return SYNTAX_ERROR;

  uint8_t tmp;

  if (state->argc >= 2)
  {
    char *msg=cmdlineGetArgStr(2, state);
    while(*msg != '\0')
    {
      xQueueSend(xSpi2SerialTx[rsNo], msg, 0);
      msg++;
    }
  }
  tmp = '\r';
  xQueueSend(xSpi2SerialTx[rsNo], &tmp, 0);
  tmp = '\n';
  xQueueSend(xSpi2SerialTx[rsNo], &tmp, 0);

  return OK_SILENT;
}

static cliExRes_t resetSlaves(cmdState_t *state)
{
  PORTB &= 0xFE;
  fprintf_P(state->myStdInOut, PSTR("Restarting slave devices\r\n"));
  PORTB |= 0x01;
  return OK_INFORM;
}
