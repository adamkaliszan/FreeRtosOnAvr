#!/bin/bash

if [ "$#" -ne 1 ]; 
then
 echo "usage: flash.sh SERIAL_PORT, e.g. flash.sh /dev/ttyUSB0"
else
  echo "avrude -p atmega16 -P $1 -c stk500v2 -U lfuse:w:lfuse:w:0xCE:m -U hfuse:w:0xc98:m -U flash:w:boot.hex"
  avrdude -p atmega16 -P $1 -c stk500v2 -U lfuse:w:0xCE:m -U hfuse:w:0xc98:m -U flash:w:boot.hex
fi

