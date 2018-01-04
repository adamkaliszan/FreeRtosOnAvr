#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <strings.h>

/* Change to the baud rate of the port B2400, B9600, B19200, etc */
#define SPEED B115200
//#define SPEED B9600

/* Change to the serial port you want to use /dev/ttyUSB0, /dev/ttyS0, etc. */
#define PORT "/dev/avrMultiTool"

int main( )
{
  int fd = open( PORT, O_RDONLY | O_NOCTTY );
  if (fd <0)
  {
    perror(PORT); exit(-1); 
  }

  struct termios options;

  bzero(&options, sizeof(options));
  options.c_cflag = SPEED | CS8 | CLOCAL | CREAD | CRTSCTS;
  options.c_iflag = IGNPAR;
//  options.c_lflag &= ~(ECHO | ICANON); 
  options.c_cc[VMIN] = 1; 
  options.c_cc[VTIME] = 0; 

  tcflush(fd, TCIFLUSH);
  tcsetattr(fd, TCSANOW, &options);



  int r;
  unsigned char buf[255];
  int i;
  while( 1 )
  {
    r = read( fd, buf, 255 );
    if (r == 0)
      continue;
    printf("długość = %d (0x%02x)", r, r);
    for(i=0; i<r; i++)
    {
      printf(" %02x", buf[i]);
    }
    printf("\n");
  }
}

