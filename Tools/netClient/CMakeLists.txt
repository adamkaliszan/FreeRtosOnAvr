PROJECT(netConsole)

cmake_minimum_required(VERSION 2.8)

ADD_EXECUTABLE(netConsole netconsole.c vt100.c)
INSTALL(TARGETS netConsole DESTINATION /usr/local/bin/)
