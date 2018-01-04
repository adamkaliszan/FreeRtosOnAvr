#include "udpconsole.h"
#include "vt100.h"

int main(int argc, char **argv)
{
  if(argc!=4)
  {
    fprintf(stderr, "Usage: %s <ip> <local port number> <remote port number>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  memset(srv_addr, 0, 16);
  memcpy(srv_addr, argv[1], strlen(argv[1]));
  
  local_port=atoi(argv[2]);
  remote_port=atoi(argv[3]);

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
        if (sendto(fd, buf2, odczytano, 0, (struct sockaddr *)&si_remote, sizeof(si_remote))==-1)
        {
          perror("send");
          exit(EXIT_FAILURE);
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

