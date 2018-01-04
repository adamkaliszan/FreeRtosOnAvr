/**
 * Author Adam Kaliszan, Jakub Muszynski
 * 
 * Creation: xx.2010
 * Modified: 30,08,2011
 * 
 * Version: 0.5
 * 
 * Simple TCP/UDP client for sending/receiving data on socket layer3: IPv4 or IPv6
 * 
 * Notice that it is very simple
 * Reading stdin (0) and send it to remote ip
 * 
 * Todo: Add some argv input validation; change strcmp(argv[1], "6t") to enum variable
 */

#include "netconsole.h"
#include "vt100.h"

int main(int argc, char **argv)
{
  local_port=2222; //to tylko po to, aby (jesli argc <5) if sprawdzajacy czy porty <1024 nie wysypal sie
  if(argc<4 || argc>5)
  { 
    fprintf(stderr, "Usage: %s [type] [options] \n", argv[0]);
    fprintf(stderr, "4t - TCP_IPv4; 6t - TCP_IPv6; 4u - UDP_IPv4\n");
    fprintf(stderr, "Options UDP_IPv4: <ip> <remote port number> [<local port number> - optional]\n", argv[0]);
    fprintf(stderr, "Options TCP_IPv4: <ip> <remote port number>\n", argv[0]);
    fprintf(stderr, "Options TCP_IPv6: <ipv6> <remote port number>\n", argv[0]);
    fprintf(stderr, "Example: ./netConsole 4u 192.168.4.4 2345 1234\n");
    fprintf(stderr, "Example: ./udpConsole 6t 2000::1111 2345\n");
    exit(EXIT_FAILURE);
  }
  //I found out, that local port is not required
  if(argc > 4)
    local_port=atoi(argv[4]);
  remote_port=atoi(argv[3]);
  if (argc > 1 && strcmp(argv[1], "6t")==0)
    strcpy(server, argv[2]);
     
  memset(srv_addr, 0, 128);
  memcpy(srv_addr, argv[2], strlen(argv[2]));

  if((local_port < 1024) || (remote_port < 1024))
  {
    fprintf(stderr, "Usage: %s <ip> <local port number> <remote port number>\n", argv[0]);
    fprintf(stderr, "\twhere <port number> shall be > 1023\n");
    exit(EXIT_FAILURE);
  }

  struct termios  attr;
  struct termios *p=&attr;
  int fd_term_in=fileno(stdin);
  int fd_term_out=fileno(stdout);

  if(!flush_term(fd_term_in, p) )
    term_error();
 
  char str[128];
  unsigned char buf3[sizeof(struct in6_addr)];

  /**
   * Here, depending on protocol type, create and open socket
   */
    if(strcmp(argv[1], "4t")==0)
    {
	if ((fd=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
	  perror("socket");
	  exit(EXIT_FAILURE);
	}
	memset((char *) &si_remote, 0, sizeof(si_remote));
	si_remote.sin_family       =  AF_INET;
	si_remote.sin_port         =  htons(remote_port);
	if (inet_aton(srv_addr, &si_remote.sin_addr)==0)
	{
	  fprintf(stderr, "inet_pton() failed\n");
	}
	if (connect(fd, (const struct sockaddr *)&si_remote, sizeof(si_remote))==-1)
	{
	  printf("Failed to connect ip %s\r\n", inet_ntoa(si_remote.sin_addr));
	  perror("connect");
	  exit(EXIT_FAILURE);
	}
    }
    else if(strcmp(argv[1], "6t")==0)
    {
      if ((fd=socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP)) < 0)
      {
	perror("socket");
	exit(EXIT_FAILURE);
      }
      memset((char *) &si_remote6, 0, sizeof(si_remote6));
      si_remote6.sin6_family       =  AF_INET6;
      si_remote6.sin6_port         =  htons(remote_port);
      rc = inet_pton(AF_INET6, server, buf3);
      if (rc != 1) /* valid IPv6 text address? */
      {
	fprintf(stderr, "inet_pton() failed\n");
      }
      if (inet_ntop(AF_INET6, buf3, str, INET6_ADDRSTRLEN) == NULL) {
	    perror("inet_ntop");
	    exit(EXIT_FAILURE);
      }
      memcpy(&si_remote6.sin6_addr, buf3, sizeof(struct in6_addr));  
      if (inet_ntop(AF_INET6, &si_remote6.sin6_addr, str, INET6_ADDRSTRLEN) == NULL) 
      {
	    perror("inet_ntop");
	    exit(EXIT_FAILURE);
      }      
      if (connect(fd,  (const struct sockaddr *)&si_remote6, sizeof(si_remote6))==-1)
      {
	printf("Failed to connect ip \r\n");
	perror("connect");
	exit(EXIT_FAILURE);
      }      
    }	
    else if(strcmp(argv[1], "4u")==0)
    {
      if ((fd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
      {
	perror("socket");
	exit(EXIT_FAILURE);
      }
      memset((char *) &si_local, 0, sizeof(si_local));
      si_local.sin_family       =  AF_INET;
      si_local.sin_port         =  htons(local_port);
      si_local.sin_addr.s_addr  =  htonl(INADDR_ANY);

      memset((char *) &si_remote, 0, sizeof(si_local));
      si_remote.sin_family       =  AF_INET;
      si_remote.sin_port         =  htons(remote_port);
      if (inet_aton(srv_addr, &si_remote.sin_addr)==0)
      {
	fprintf(stderr, "inet_aton() failed\n");
	exit(EXIT_FAILURE);
      }
      if (bind(fd, (const struct sockaddr *)&si_local, sizeof(si_local))==-1)
      {
	printf("Failed to bind ip %s\r\n", inet_ntoa(si_local.sin_addr));
	perror("bind");
	exit(EXIT_FAILURE);
      }
    }  
    else if(strcmp(argv[1], "4u")==0)
    {
      	printf("Failed to prepare socket, check argv[1] - type\r\n");
	exit(EXIT_FAILURE);
    }

  fd_set rfds;
  struct timeval tv;
  int retval;

/* Obserwacja stdin (fd 0) i sprawdzanie kiedy ma wejście. */
  FD_ZERO(&rfds);
  FD_SET(0, &rfds);
  FD_SET(fd, &rfds);

  
  while(1)
  {
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    FD_SET(fd, &rfds);
    /* Czekanie nie dłużej niż sekund. */
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    retval = select(fd+1, &rfds, NULL, NULL, &tv);
    /* Nie należy już polegać na wartości tv! */

    if (retval)
    {
      if (FD_ISSET(0, &rfds))
      {
        int odczytano = read(0, buf, BUFLEN);
        if (odczytano == -1)
        {
          perror("read");
          exit(EXIT_FAILURE);	  
        }
        odczytano = strToVt100(buf, odczytano, buf2, BUFLEN);
	if(strcmp(argv[1], "4u")==0)
	{
	  if (sendto(fd, buf2, odczytano, 0, (struct sockaddr *)&si_remote, sizeof(si_remote))==-1)
	  {
	    perror("send");
	    exit(EXIT_FAILURE);
	  }
	}
	else
	{
	  if (send(fd, buf2, odczytano, 0)==-1)
	  {
	    perror("send");
	    exit(EXIT_FAILURE);
	  }
	}
      }
      if (FD_ISSET(fd, &rfds))
      {
        memset(buf, 0, sizeof(char)*BUFLEN);
        int odczytano = recv(fd, buf, BUFLEN, 0);
        if (odczytano == -1)
        {
          perror("recvfrom()");
          exit(EXIT_FAILURE);
        }
        if (write(1, buf, odczytano) == -1)
        {
          perror("write to console");  
        }
      }
    }
  }
  shutdown(fd, SHUT_RDWR);
  close(fd);
  return 0;
}

int checktty(struct termios *p, int term_fd)
{
  struct termios ck;
return (tcgetattr(term_fd, &ck) == 0 && (p->c_lflag == ck.c_lflag) && (p->c_cc[VMIN] == ck.c_cc[VMIN]) && (p->c_cc[VTIME] == ck.c_cc[VMIN]) );
}

int flush_term(int term_fd, struct termios *p)
{
  struct termios newterm;
  errno=0;
  tcgetattr(term_fd, p);  /* get current stty settings*/

  newterm = *p; 
//  newterm.c_lflag &= ~(ECHO); 
  newterm.c_lflag &= ~(ECHO | ICANON); 
  newterm.c_cc[VMIN] = 0; 
  newterm.c_cc[VTIME] = 0; 

  return(tcgetattr(term_fd, p) == 0 && tcsetattr(term_fd, TCSAFLUSH, &newterm) == 0 && checktty(&newterm, term_fd) != 0);
}

void term_error(void)
{
  fprintf(stderr, "unable to set terminal characteristics\n");
  perror("");                                                
  exit(1);                                                   
}

#if 0
http://publib.boulder.ibm.com/iseries/v5r2/ic2928/index.htm?info/rzab6/rzab6xip6client.htm

/**************************************************************************/
/* This is an IPv4 or IPv6 client.                                        */
/**************************************************************************/
 
/**************************************************************************/
/* Header files needed for this sample program                            */
/**************************************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
 
/**************************************************************************/
/* Constants used by this program                                         */
/**************************************************************************/
#define BUFFER_LENGTH    250
#define FALSE              0
#define SERVER_NAME     "ServerHostName"
 
/* Pass in 1 parameter which is either the */
/* address or host name of the server, or  */
/* set the server name in the #define      */
/* SERVER_NAME.                            */
void main(int argc, char *argv[])
{
   /***********************************************************************/
   /* Variable and structure definitions.                                 */
   /***********************************************************************/
   int    sd=-1, rc, bytesReceived=0;
   char   buffer[BUFFER_LENGTH];
   char   server[NETDB_MAX_HOST_NAME_LENGTH];
   char   servport[] = "3005";
   struct in6_addr serveraddr; 
   struct addrinfo hints, *res=NULL;
 
   /***********************************************************************/
   /* A do/while(FALSE) loop is used to make error cleanup easier.  The   */
   /* close() of the socket descriptor is only done once at the very end  */
   /* of the program along with the free of the list of addresses.        */
 /***********************************************************************/
   do
   {
      /********************************************************************/
      /* If an argument was passed in, use this as the server, otherwise  */
      /* use the #define that is located at the top of this program.      */
      /********************************************************************/
      if (argc > 1)
         strcpy(server, argv[1]);
      else
         strcpy(server, SERVER_NAME);
 
      memset(&hints, 0x00, sizeof(hints));
      hints.ai_flags    = AI_NUMERICSERV;
      hints.ai_family   = AF_UNSPEC;
      hints.ai_socktype = SOCK_STREAM;
      /********************************************************************/
      /* Check if we were provided the address of the server using        */
      /* inet_pton() to convert the text form of the address to binary    */
      /* form. If it is numeric then we want to prevent getaddrinfo()     */
      /* from doing any name resolution.                                  */
      /********************************************************************/
      rc = inet_pton(AF_INET, server, &serveraddr);
      if (rc == 1)    /* valid IPv4 text address? */
      {
         hints.ai_family = AF_INET;
         hints.ai_flags |= AI_NUMERICHOST;
      }
      else
      {
         rc = inet_pton(AF_INET6, server, &serveraddr);
         if (rc == 1) /* valid IPv6 text address? */
         {
 
            hints.ai_family = AF_INET6;
            hints.ai_flags |= AI_NUMERICHOST;
         }
      }
      /********************************************************************/
      /* Get the address information for the server using getaddrinfo().  */
      /********************************************************************/
      rc = getaddrinfo(server, servport, &hints, &res);
      if (rc != 0)
      {
         printf("Host not found --> %s\n", gai_strerror(rc));
         if (rc == EAI_SYSTEM)
            perror("getaddrinfo() failed"); 
         break;
      }
 
      /********************************************************************/
      /* The socket() function returns a socket descriptor representing   */
      /* an endpoint.  The statement also identifies the address family,  */
      /* socket type, and protocol using the information returned from    */
      /* getaddrinfo().                                                   */
      /********************************************************************/
      sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
      if (sd < 0) 
      {
         perror("socket() failed");
         break;
      }
      /********************************************************************/
      /* Use the connect() function to establish a connection to the      */
      /* server.                                                          */
      /********************************************************************/
      rc = connect(sd, res->ai_addr, res->ai_addrlen); 
      if (rc < 0)
      {
         /*****************************************************************/
         /* Note: the res is a linked list of addresses found for server. */
         /* If the connect() fails to the first one, subsequent addresses */
         /* (if any) in the list could be tried if desired.               */
         /*****************************************************************/
         perror("connect() failed");
         break;
      }
 
      /********************************************************************/
      /* Send 250 bytes of a's to the server                              */
      /********************************************************************/
      memset(buffer, 'a', sizeof(buffer));
      rc = send(sd, buffer, sizeof(buffer), 0);
      if (rc < 0)
      {
         perror("send() failed");
         break;
      }
 
      /********************************************************************/
      /* In this example we know that the server is going to respond with */
      /* the same 250 bytes that we just sent.  Since we know that 250    */
      /* bytes are going to be sent back to us, we could use the          */
      /* SO_RCVLOWAT socket option and then issue a single recv() and     */
      /* retrieve all of the data.                                        */
      /*                                                                  */
      /* The use of SO_RCVLOWAT is already illustrated in the server      */
      /* side of this example, so we will do something different here.    */
      /* The 250 bytes of the data may arrive in separate packets,        */
      /* therefore we will issue recv() over and over again until all     */
      /* 250 bytes have arrived.                                          */
      /********************************************************************/
      while (bytesReceived < BUFFER_LENGTH)
      {
         rc = recv(sd, & buffer[bytesReceived],
                   BUFFER_LENGTH - bytesReceived, 0);
         if (rc < 0)
         {
            perror("recv() failed");
            break;
         }
         else if (rc == 0)
         {
            printf("The server closed the connection\n");
            break;
         }
 
         /*****************************************************************/
         /* Increment the number of bytes that have been received so far  */
         /*****************************************************************/
         bytesReceived += rc;
      }
 
   } while (FALSE); 
 
   /***********************************************************************/
   /* Close down any open socket descriptors                              */
   /***********************************************************************/
   if (sd != -1)
      close(sd);
   /***********************************************************************/
   /* Free any results returned from getaddrinfo                          */
   /***********************************************************************/
   if (res != NULL)
      freeaddrinfo(res); 
}




#endif
