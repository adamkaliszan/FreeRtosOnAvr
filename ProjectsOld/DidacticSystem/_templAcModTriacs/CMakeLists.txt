PROJECT(ModWykonawczy)

ADD_EXECUTABLE(modWykonawczy main.c automat.c hardware.c serial.c)
add_library(headers main.h automat.h FreeRTOSConfig.h hardware.h ../protocol1.h serial.h)
