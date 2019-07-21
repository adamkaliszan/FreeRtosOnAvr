#!/bin/bash
sudo ftdi_eeprom --erase-eeprom ftdi_eep.conf
sudo ftdi_eeprom --read-eeprom ftdi_eep.conf
sudo ftdi_eeprom --flash-eeprom ftdi_eep.conf

