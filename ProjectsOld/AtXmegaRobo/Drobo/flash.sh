#!/usr/bin/expect -f

system "hex2bin firmware.hex"
system "echo To_jest_wypelnienie_poniewaz_bootloader_ma_jeszcze_buga_i_nie_wgrywa_ostatniego_bloku_256_bajtow._Zatem_tutaj_jest_jakis_zbedny_tekst_ktory_bedzie_widzany_w_obrazie_firmware_procka._Chyba_juz_starcz_tego_tekstu >> firmware.bin"


set dev "/dev/atmega128eval"
set file "./firmware.bin"

system "stty 115200 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke < $dev"

spawn -open [ open $dev r+ ]
send_user "Calling bootloader\n"

send "flash"
expect "C"
send_user "\nStarting xmodem $dev\n"
close
system "sx -vv -o -b -X  $file > $dev < $dev"
send_user "\nOK ready\n"
