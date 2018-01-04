#!/bin/bash

if [ "$#" -ne 1 ]; 
then
 echo "usage: flash.sh SERIAL_PORT, e.g. flash.sh /dev/ttyUSB0"
else
  echo "avrude -p atmega8 -P $1 -c stk500v2 -U lfuse:w:0xde:m -U hfuse:w:0xc9:m -U flash:w:avrusb500_pre.hex"
  avrdude -p atmega8 -P $1 -c stk500v2 -U lfuse:w:0xde:m -U hfuse:w:0xc9:m -U flash:w:avrusb500_pre.hex
fi

