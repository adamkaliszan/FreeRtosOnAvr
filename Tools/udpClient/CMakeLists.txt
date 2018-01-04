PROJECT(udpConsole)

cmake_minimum_required(VERSION 2.8)

ADD_EXECUTABLE(udpConsole udpconsole.c vt100.c)
INSTALL(TARGETS udpConsole DESTINATION /usr/local/bin/)
