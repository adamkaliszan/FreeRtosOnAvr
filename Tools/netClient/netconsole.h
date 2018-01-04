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
#include <netdb.h>

#include "vt100.h"

#define BUFLEN 1024

struct sockaddr_in   si_local;//I found out, that local port is not required - you can remove it if you like
struct sockaddr_in   si_remote;
struct sockaddr_in6   si_remote6;
int                  fd;
int                  local_port;
int                  remote_port;
char                 buf[BUFLEN];
char                 buf2[BUFLEN];
char                 srv_addr[16];

char   server[40];
int rc;
struct in6_addr serveraddr; 
struct addrinfo hints, *res=NULL;
  

/*void wait_and_exit(void);*/
void term_error(void);
int flush_term(int term_fd, struct termios *p);
/*int keypress(int term_fd);*/
int checktty(struct termios *p, int term_fd);
