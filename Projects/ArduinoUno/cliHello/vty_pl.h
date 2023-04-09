#ifndef LANG_VTY
#define LANG_VTY PL

// *************************** Error Strings *******************************************************

prog_char errorOK[]                                   = "Wszystko poprawnie\r\n";
prog_char errorNoFile[]                               = "Brak pliku\r\n";
prog_char errorxModemFrameStartTimeout[]              = "\r\n";
prog_char errorxModemByteSendTimeout[]                = "\r\n";
prog_char errorxModemWrongFrameNo[]                   = "\r\n";
prog_char errorxModemFrameFrameNoCorrectionNotMatch[] = "\r\n";
prog_char errorxModemFrameCrc[]                       = "xModem CRC error\r\n";
prog_char errorxModemRemoteSideCan[]                  = "Strona zdalna przerwala transmisje na ramce nr %d\r\n";
prog_char errorxModemUnknownResponse[]                = "xModem nieznana odpowiedx 0x%x\r\n";
prog_char errorNoRemoteDevice[]                       = "Urządzenie %d nie odpowiada (%d)\r\n";
prog_char errorBootloaderNotResponding[]              = "Bootloader nie odpowiada\r\n";
prog_char errorOpenFile[]                             = "Nie mozna otworzyc pliku %s\r\n";

// *************************** Message Strings *****************************************************

prog_char systemStateStr[]            = "Stan systemu:\r\n";
prog_char statusNumberOfTasksStr[]    = "  Liczba zadan        : %d\r\n";
prog_char statusStaticHeapStateStr[]  = "  Sterta dla FreeRtos : %d wolnych z %d bajtow\r\n";
prog_char statusDynamicHeapStateStr[] = "  Sterta dla malloc   : %d wolnych z %d bajtow\r\n";
prog_char systemRamConfigStr[]        = "Ustawienia systemu:\r\n";

prog_char cmd_help[]        = "pomoc";            prog_char cmd_help_help[]        = "Wypisuje wszystkie komendy";
prog_char cmd_status[]      = "status";           prog_char cmd_help_status[]      = "{nazwa pliku} Wypisuje status urzadzenia na ekranie lub zapisuje do pliku";
prog_char cmd_enable[]      = "admin";            prog_char cmd_help_enable[]      = "Wejscie w tryb uprzywilejowany";
prog_char cmd_disable[]     = "normalny";         prog_char cmd_help_disable[]     = "Wyjscie z trybu uprzywilejowanego";
prog_char cmd_configure[]   = "konfig";           prog_char cmd_help_configure[]   = "Wejscie w tryb konfiguracji";
prog_char cmd_conf_save[]   = "zapisz";           prog_char cmd_help_conf_save[]   = "Zapisz konfiguracje";

#endif
