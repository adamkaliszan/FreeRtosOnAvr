#ifndef SIM900_H
#define SIM900_H

#include <avr/eeprom.h>
#include <avr/pgmspace.h>

struct tcpConDesc
{
    char srcAddress[30];
    uint16_t dstPortNo;
};

typedef struct tcpConDesc tcpConDesc_t;

/**
 * Włączenie modułu sim900
 */
void sim900pwrOn(void);

/**
 * Odłączenie zasilania od modułu sim900
 */
void sim900pwrOffHw(void);

/**
 * Programowe wyłączenie modułu sim900
 */
void sim900pwrOffSoft(void);

/**
 * Rsetart modułu
 */
void sim900reset(void);


/**
 * Otwiera połączenie TCP
 * @param bufNo - numer bufora (1-4)
 * @param defConNo - numer połączenia, które zostało zdefiniowane wcześniej
 */
void openTcpCon(uint8_t bufNo, uint8_t defConNo);

/**
 *
 */
void closeTcpCon(uint8_t conNo);


#endif // SIM900_H
