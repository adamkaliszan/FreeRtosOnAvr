#ifndef PROTOCOL1_H
#define PROTOCOL1_H 1

// Addressing devices on RS 485 bus
#define FIRST_ROLLER_DRIVER_ADDR 1
#define LAST_ROLLER_DRIVER_ADDR  31

#define FIRST_LIGHT_DRIVER_ADDR  32
#define LAST_LIGHT_DRIVER_ADDR   63

#define FIRST_SENSOR_ADDR        64
#define LAST_SENSOR_ADDR         95

// *********************************** General message format *****************************************************
#define HDR_LEN                  4
#define SYNC                     0x5A

//  + header (4 bytes)                   + data                + CRC           +
//  +------+----------+---------+--------+-------+     +-------+-------+-------+
//  | SYNC | op. code | address | length | data1 | ... | dataN | CrcHi | CrcLo |
//  | 0x5A |          |         |        |       |     |       |       |       |
//  +------+----------+---------+--------+-------+     +-------+-------+-------+
//
// CRC is calculated according to all bytes without CRC Hi and Lo on the frame end (including sync and excluding CrcHi and CrcLo)

// *********************************** Ping message  **************************************************************
#define rPING             0x80
// ping: master->slave
//  +------+------+---------+--------+-------+     +-------+-------+-------+
//  | SYNC | type | address | length | data1 | ... | dataX | CrcHi | CrcLo |
//  | 0x5A | 0x80 |         |    X   |       |     |       |       |       |
//  +------+------+---------+--------+-------+     +-------+-------+-------+


// ping response: slave->master
//  +------+------+---------+--------+-------+     +-------+-------+-------+
//  | SYNC | type | address | length | data1 | ... | dataX | CrcHi | CrcLo |
//  | 0x5A | 0x80 |    0    |    X   |       |     |       |       |       |
//  +------+------+---------+--------+-------+     +-------+-------+-------+

// *********************************** Hello message **************************************************************
#define rHELLO            0x82

// hello: master->slave
//  +------+------+---------+--------+-------+-------+
//  | SYNC | type | address | length | CrcHi | CrcLo |
//  | 0x5A | 0x82 |         |    0   |       |       |
//  +------+------+---------+--------+-------+-------+


// hello response: slave->master
//  +------+------+---------+--------+--------+--------+-------+-------+-------+-------+-------+-------+-------+-------+
//  | SYNC | type | address | length | data1  | data2  | data3 | data4 | data5 | data6 | data7 | data8 | CrcHi | CrcLo |
//  | 0x5A | 0x82 |    0    |    7   | state1 | state2 |setting|version|version|version|version|Version|       |       |
//  |      |      |         |        |        |        |       |'v'/'b'|       |  '.'  |       |       |       |       |
//  +------+------+---------+--------+--------+--------+-------+-------+-------+-------+-------+-------+-------+-------+

//offsets of hello response data field 
#define HELLO_RESP_STATE1  0
#define HELLO_RESP_STATE2  1
#define HELLO_RESP_VER     2
#define HELLO_RESP_VER_LEN 5
#define HELLO_RESP_LEN     3+HELLO_RESP_VER_LEN



// *********************************** Flash firmware message  *****************************************************
#define rFLASH            0x81


// *********************************** Roller blind 1 down *********************************************************
#define rOpuscRolete1     0x10

// *********************************** Roller blind 1 down *********************************************************
#define rOpuscRolete2     0x11

// *********************************** Roller blind 1 up ***********************************************************
#define rPodniesRolete1   0x20

// *********************************** Roller blind 2 up ***********************************************************
#define rPodniesRolete2   0x21

// *********************************** Roller 1 stop     ***********************************************************
#define rZatrzymajRolete1 0x30

// *********************************** Roller 2 stop     ***********************************************************
#define rZatrzymajRolete2 0x31

// *********************************** Configure module ************************************************************
#define rUstaw 0x40

// *********************************** Write module Settings********************************************************
#define rZapiszUstawienia 0x41


// *********************************** Unknown message  ************************************************************
#define rUNKNOWN          0xFF

#endif /* PROTOCOL1_H */
