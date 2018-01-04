#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "vt100.h"

#define BUFLEN 1024

struct sockaddr_in   si_local;
struct sockaddr_in   si_remote;
int                  fd;
int                  local_port;
int                  remote_port;
char                 buf[BUFLEN];
char                 buf2[BUFLEN];
char                 srv_addr[16];

/*void wait_and_exit(void);*/
void term_error(void);
int flush_term(int term_fd, struct termios *p);
/*int keypress(int term_fd);*/
int checktty(struct termios *p, int term_fd);
