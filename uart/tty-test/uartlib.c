#include "uartlib.h"
/********************************************************************
* �������ƣ� ComOpen
* ���������� ���豸
* ��������� port:���豸���豸���
* ��������� ��
* �� �� ֵ�� �ɹ��򷵻ش��豸���ļ�������
********************************************************************/
int ComOpen(unsigned char *port)
{
   int fd;
   fd = open(port, O_RDWR|O_NOCTTY);
   if(fd==-1)
   {
      perror("ArmCom: Can't Open Com Port");
      return(-1);
   }
   return fd;
}
/*****************************************************************************
* �������ƣ� ComSetBaud
* ���������� ���ò�����
* ��������� fd:���豸���ļ�������
   baud:������
* ��������� ��
* �� �� ֵ�� �ɹ��򷵻�0
******************************************************************************/
int baud_attr[]={B1000000,B921600,B500000,B230400,B115200, B57600,
          B38400, B19200, B9600, B4800, B2400, B1200, B300};
int name_attr[]={1000000,921600,500000,230400,115200, 57600,
          38400, 19200, 9600, 4800, 2400, 1200, 300};
int ComSetBaud(int fd, int baud)
{
   int i, j;
   int status;
   struct termios Opt;
   tcgetattr(fd, &Opt);
 for(j=0; j<2; j++)
 {
  for(i=0; i<sizeof(baud_attr)/sizeof(int); i++)
  {
   if(baud==name_attr[i])
   {
    tcflush(fd, TCIOFLUSH);
    cfsetispeed(&Opt, baud_attr[i]);
    cfsetospeed(&Opt, baud_attr[i]);
    status = tcsetattr(fd, TCSANOW, &Opt);
    if(status != 0)
    {
     perror("ArmCom: tcsetattr fail");
     return(-1);
    }
    tcflush(fd, TCIOFLUSH);
   }
  }
 }
 return (0);
}
/*****************************************************************************
* �������ƣ� ComSetParity
* ���������� ��������λ��ֹͣλ��У��λ
* ��������� fd:���豸���ļ�������
   databits:����λ
   stopbits:ֹͣλ
   parity:У��λ
* ��������� ��
* �� �� ֵ�� �ɹ��򷵻�0
******************************************************************************/
int ComSetParity(int fd, int databits, int stopbits, char parity)
{
 struct termios options;
 if(tcgetattr(fd, &options) != 0)
 {
  perror("ArmCom: tcgetattr fail");
  return(-1);
 }
 options.c_cflag &= ~CSIZE;
 switch(databits)
 {
  case 5:
   options.c_cflag |= CS5; //����λ��5
   break;
  case 6:
   options.c_cflag |= CS6;
   break;
  case 7:
   options.c_cflag |= CS7;
   break;
  case 8:
   options.c_cflag |= CS8; //����λ��8
   break;
  default:
   fprintf(stderr, "ArmCom: Unsupported data size");
  return(-1);
    }
    switch(parity)
    {
  case 'n':
  case 'N':
    options.c_cflag &= ~PARENB;
    options.c_iflag &= ~INPCK; //У�飺none
    break;
  case 'o':
  case 'O':
    options.c_cflag |= (PARODD | PARENB); //У�飺odd
    options.c_iflag |= INPCK;
    break;
  case 'e':
  case 'E':
    options.c_cflag |= PARENB;	 //У�飺even
    options.c_cflag &= ~PARODD;
    options.c_iflag |= INPCK;
    break;
  case 'm': //У�飺mark
  case 'M':
   options.c_cflag |= PARENB | CMSPAR | PARODD;
   break;
  case 's':
  case 'S': //У�飺space
   options.c_cflag &= ~PARENB;
   options.c_cflag &= ~CSTOPB;
   //options.c_cflag |= PARENB | CMSPAR;
   break;
  default:
    fprintf(stderr, "ArmCom: Unsupported parity");
    return(-1);
   }
   switch(stopbits)
   {
       case 1:
    options.c_cflag &= ~CSTOPB;
    break;
       case 2:
    options.c_cflag |= CSTOPB;
       break;
       default:
    fprintf(stderr, "ArmCom: Unsupported stop bits");
    return(-1);
   }
   
/*
    if(parity=='n' || parity=='N')
 options.c_iflag =0;
 options.c_lflag &= ~(ICANON | ECHO | ECHOE |ISIG);
 options.c_oflag = 0;
 tcflush(fd, TCIFLUSH);
 options.c_cc[VTIME] = 0;
 options.c_cc[VMIN] = 0;
*/

 if (parity != 'n') 			   
 	options.c_iflag |= INPCK;		
 options.c_cc[VTIME] = 30; // 3 seconds		  
 options.c_cc[VMIN] = 0;
 		   
 options.c_lflag	&= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/				  
 options.c_oflag  &= ~OPOST;	/*Output*/				  
		  
 if(tcsetattr(fd, TCSANOW, &options) != 0)
 {
   perror("ArmCom: Setup Serial Port");
   return (-1);
 }
 return(0);
}
/*****************************************************************************
* �������ƣ� uart_init
* ���������� �˿ڳ�ʼ��
* ��������� baudrate:������
   databits:����λ
   stopbits:ֹͣλ
   parity:У��λ
   com:�˿ں�
* ��������� ��
* �� �� ֵ�� �ɹ��򷵻ش򿪶˿ڵ��ļ�������
******************************************************************************/
int uart_init(char *dev, int baudrate, int databits, int stopbits, char parity)
{
   int fd, i;
   int fdd = 0;
   
   /*
   char *dev[PORT_NUM] = {"/dev/ttyS5", "/dev/ttyS4", "/dev/ttyS3", "/dev/ttyS2",
                     "/dev/ttyS9", "/dev/ttyS8", "/dev/ttyS7", "/dev/ttyS6"};
   */
   
   fd = ComOpen(dev);
   if(fd == -1)
   {
      printf("ArmCom: Fail to Open COM(%s)\n", dev);
      return(-1);
   }
   else
   {
      ComSetBaud(fd, baudrate);
   if(ComSetParity(fd, databits, stopbits, parity)==(-1))
   {
    printf("ArmCom: Set COM(%s) Parity Error\n", dev);
    return (-1);
   }
   //printf("Armcom: Open COM(%s), Baudrate=%d\n", dev, baudrate);
   return fd;
 }
}
