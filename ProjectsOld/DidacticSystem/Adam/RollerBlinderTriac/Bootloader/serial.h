#ifndef SERIAL_H
#define SERIAL_H

#define RS485Enable()      (PORTD |=  0x08)
#define RS485Disable()     (PORTD &=  0xF7)


#endif /* SERIAL_H */
