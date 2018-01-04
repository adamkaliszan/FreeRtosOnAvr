#ifndef HARDWARE_CONFIG_H
#define HARDWARE_CONFIG_H

// --------------------- Configuration I/O module --------------------------------

//Locker 1 (723 / 687) broken
#define LOCK_SENS_1_LIGHT 0x40
#define LOCK_SENS_1_AC_IN 4
#define LOCK_SENS_1_THR   700
#define LOCK_SENS_1_ENA   0

//Locker 2 (603 / 993) OK
#define LOCK_SENS_2_LIGHT 0x20
#define LOCK_SENS_2_AC_IN 5
#define LOCK_SENS_2_THR   750
#define LOCK_SENS_2_ENA   1

//Locker 3 (934/937) broken
#define LOCK_SENS_3_LIGHT 0x10
#define LOCK_SENS_3_AC_IN 6
#define LOCK_SENS_3_THR   700
#define LOCK_SENS_3_ENA   0

//Locker 4 (831 / 980) OK
#define LOCK_SENS_4_LIGHT 0x08
#define LOCK_SENS_4_AC_IN 7
#define LOCK_SENS_4_THR   900
#define LOCK_SENS_4_ENA   1


// --------------------- Konfiguracja pamięci ------------------------------------
//

#define HEAP_BEGIN                   0x1100
#define HEAP_END                     CLI_1_BUF_ADDR - 1
#define HEAP_SIZE                    HEAP_END - HEAP_BEGIN + 1

#define CLI_BUF_TOT_LEN              0x0100
#define CLI_1_BUF_ADDR               0x2800
#define CLI_2_BUF_ADDR               0x2900
#define CLI_3_BUF_ADDR               0x2A00
#define CLI_4_BUF_ADDR               0x2B00

#define RTOS_TCP_BUF_BASE_ADDR       0x2C00

#define SPI_TX_BUFFER                0x7000
#define SPI_RX_BUFFER                0x7400

#define RTOS_UDP_TX_BUF_ADDR         0x7800
#define RTOS_UDP_RX_BUF_ADDR         0x7900

#define SPI2SERIAl_TX_BUFFERS        0x8000
#define NO_OF_SPI2SERIAL_TX_BUF      16
#define OF_SPI2SERIAL_TX_BUF_SIZE    256
#define SPI2SERIAl_RX_BUFFERS        0x9000
#define NO_OF_SPI2SERIAL_RX_BUF      16
#define OF_SPI2SERIAL_RX_BUF_SIZE    256

#define NETWORK_STACK_BUF_SIZE       0x0600 // 1532 bytes
#define NETWORK_STACK_BUF_ADDR       0x7A00 // 30 1/4 - 32 kB
#define ENC28J60BUF_ADDR_END         ENC28J60BUF_ADDR + ENC28J60BUF_SIZE - 1

/* Memory Map
 0x0000 +-----------------------------+
  256   | Controll registers          |
 0x0100 +-----------------------------+
  4k    | Internal memory             |
 0x1100 +-----------------------------+  4 K
  5k768 | Heap                        |
 0x2800 +-----------------------------+  11 k 768
    256 * CLI 1 buffer                +
 0x2900 +-----------------------------+
    256 * CLI 2 buffer                +
 0x2A00 +-----------------------------+


 0x7000 +-----------------------------+  28 k
   1k   | SPI Tx Buffer               |
 0x7400 +-----------------------------+  29 k
   1k   | SPI Rx Buffer               |
 0x7800 +-----------------------------+  30 k
  256   | RTOS UDP Tx buffer          |
 0x7900 +-----------------------------+
  256   | RTOS UDP Rx buffer          |
 0x7A00 +-----------------------------+
  1k512 | Enc28j60Buffer              |
 0x8000 +-----------------------------+  32 k
   4k   | Spi2SerialTx                |
 0x9000 +-----------------------------+  36 k
   4k   | Spi2SerialRx                |
 0xA000 +-----------------------------+  40 k

*/


//Konfiguracja biblioteki ds1305.h
#define USE_DECODED_TIME_STRUCT 1

//Konfiguracja Sterownika ethenretowego Enc28j60
//CS jest na PORT E.3
#define ENC_SPI_CS_PORT              PORTE
#define ENC_SPI_CS_EN_MASK_OR        0x00
#define ENC_SPI_CS_EN_MASK_AND       0xF7

//Konfiguracja Karty SD
//CS jest na PORT G.3
#define SD_SPI_CS_PORT               PORTG
#define SD_SPI_CS_EN_MASK_OR         0x00
#define SD_SPI_CS_EN_MASK_AND        0xF7

//Konfiguracja portu równoległego MPC23S17
//CS jest na PORT B.7
#define MPC23S17_SPI_CS_PORT         PORTB
#define MPC23S17_SPI_CS_EN_MASK_OR   0x00
#define MPC23S17_SPI_CS_EN_MASK_AND  0x7F

//Konfiguracja Układu analogowo cyfrowego MPC3008
//CS jest na PORT B.6
#define MCP3008_SPI_CS_PORT          PORTB
#define MCP3008_SPI_CS_EN_MASK_OR    0x00
#define MCP3008_SPI_CS_EN_MASK_AND   0xBF

//Konfiguracja rezystora cyfrowego MCP4150
//CS jest na PORT B.6
#define MCP4150_SPI_CS_PORT          PORTB
#define MCP4150_SPI_CS_EN_MASK_OR    0x00
#define MCP4150_SPI_CS_EN_MASK_AND   0xBF


//Konfiguracja Zegara czasu rzeczywistego DS1305
//CE jest na PORT B.5
#define DS1305_SPI_CS_PORT           PORTB
#define DS1305_SPI_CS_EN_MASK_OR     0x20
#define DS1305_SPI_CS_EN_MASK_AND    0xFF



//konfiguracja wyłączania wszystkich urządzeń SPI

//PORT A: Zewnętrzna pamięć
#define disableSpiPORTA_OR           0x00
#define disableSpiPORTA_AND          0xFF

//PORT B:
// PB4 - Sl 3, CPU 1
// PB5 - SL 3, CPU 2
// PB6 - SL 4, CPU 1
// PB7 - SL 4, CPU 2
#define disableSpiPORTB_OR           0xF0
#define disableSpiPORTB_AND          0xFF

//PORT C: Zewnętrzna pamięć
#define disableSpiPORTC_OR           0x00
#define disableSpiPORTC_AND          0xFF

//PORD D:
// PD4 - Sl 1, CPU 1
// PD5 - SL 1, CPU 2
// PD6 - SL 2, CPU 1
// PD7 - SL 2, CPU 2
#define disableSpiPORTD_OR           0xF0
#define disableSpiPORTD_AND          0xFF

//PORT E
// PE3 - ENC28j60  0 - on,  1 - off
#define disableSpiPORTE_OR           0x08
#define disableSpiPORTE_AND          0xFF

//PORT F - brak SPI SS
#define disableSpiPORTF_OR           0x00
#define disableSpiPORTF_AND          0xFF

//PORT G
// PG3 - SD
#define disableSpiPORTG_OR           0x08
#define disableSpiPORTG_AND          0xFF

#endif
