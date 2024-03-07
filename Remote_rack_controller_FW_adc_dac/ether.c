/* Standard includes. */
#include <stdio.h>
#include <string.h>

#define DEBUG_ETHERNET 1

/* lwIP includes. */
#include "lwip/api.h" 
#include "lwip/tcpip.h"
#include "lwip/memp.h" 
#include "lwip/stats.h"
//#include "netif/loopif.h"
#include "lwip/sockets.h"

unsigned char ETH_IP[4], ETH_HOST[4];
unsigned short ETH_PORT_OUT;

unsigned char flagsend=0;

fd_set rfds;
struct timeval tv;

int ns_in;
static char tmp_buf_recv_data[1500], buf_recv_data[1500];
unsigned long size_of_recv_data;
unsigned char flag_recv=0, flag_connection=0, flag_start=0;

int s_out;
struct sockaddr_in sname_out;
int flag_connection_out;

void Set_ETH_IP(unsigned char *vmas)
{
 memcpy(ETH_IP, vmas, 4);
}

void Set_ETH_HOST(unsigned char *vmas)
{
 memcpy(ETH_HOST, vmas, 4);
}

void Set_ETH_PORT_OUT(unsigned short value)
{
 ETH_PORT_OUT=value;
}

void Get_ETH_IP(unsigned char *vmas)
{
 memcpy(vmas, ETH_IP, 4);
}

void Get_ETH_HOST(unsigned char *vmas)
{
 memcpy(vmas, ETH_HOST, 4);
}

unsigned short Get_ETH_PORT_OUT(void)
{
 return(ETH_PORT_OUT);
}

int ether_connect(void)
{
 int res;

  memset(&sname_out, 0, sizeof(sname_out));
  sname_out.sin_family = AF_INET;
  sname_out.sin_addr.s_addr = ETH_HOST[0]|(ETH_HOST[1]<<8)|(ETH_HOST[2]<<16)|(ETH_HOST[3]<<24);
  sname_out.sin_port = htons(ETH_PORT_OUT);

#ifdef DEBUG_ETHERNET
  printf("Подключаюсь...\n\r");
#endif
  if ((s_out = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
  {
   res=-1;
#ifdef DEBUG_ETHERNET
   printf("error: socket()\n\r"); 
#endif
  }
  else
  {
    if ((flag_connection_out=connect(s_out, (struct sockaddr *) &sname_out, sizeof(sname_out))) < 0)
    {
     res=-1;
#ifdef DEBUG_ETHERNET
     printf("error: connect()\n\r");
#endif
    }
    else
    {
     res=0;
#ifdef DEBUG_ETHERNET
     printf("Подключились удачно\n\r");
#endif
    }
  }
 return(res);
}

int ether_send_to(char *databuf, unsigned long datalength)
{
 flag_connection_out=send(s_out, databuf, datalength, MSG_DONTWAIT);
 if ((s_out<0)||(flag_connection_out<0)) return(-1);
 return(datalength);
}

int ether_disconnect(void)
{
 close(s_out);
 return(0);
}

unsigned char ether_getconnection(void)
{
  return(flag_connection);
}

unsigned char ether_getstart(void)
{
  return(flag_start);
}

unsigned char ether_read_ready(void)
{
  return(flag_recv);
}

int ether_read(char *databuf, unsigned long datalength)
{
  flag_recv=0;
  if (datalength<size_of_recv_data) 
  {  
   memcpy(databuf, buf_recv_data, datalength);
   return(datalength);
  }
  else
  {
   memcpy(databuf, buf_recv_data, size_of_recv_data);
   return(size_of_recv_data);
  }  
}

void ether_send(char *databuf, unsigned long datalength)
{
 if (flag_connection==1)
 {
  send(ns_in, databuf, datalength, MSG_DONTWAIT); 
 }
}

void ether_flush(void)
{
  flag_recv=0;
  size_of_recv_data=0;
}

int checksocket(int tstsock)
{
 FD_ZERO(&rfds);           
 FD_SET(tstsock, &rfds);
 return(select(tstsock+1, &rfds, NULL, NULL, &tv));
}

static void tcp_thread(void *arg)
{
  static struct netif EMAC_if;
  extern err_t ethernetif_init( struct netif *netif );
  struct ip_addr xIpAddr, xNetMast, xGateway;

 int i_in,s_in;
 struct sockaddr_in name_in,cname_in;
 int nbyte;

 if ((s_in=socket(AF_INET,SOCK_STREAM,0))<0) 
 {
#ifdef DEBUG_ETHERNET
  printf("error: socket\n\r"); 
#endif
 }
 name_in.sin_family=AF_INET;
 name_in.sin_addr.s_addr=INADDR_ANY;
 name_in.sin_port=htons(2000);
 if (bind(s_in,(struct sockaddr *)&name_in,sizeof(name_in))<0) 
 {
#ifdef DEBUG_ETHERNET
  printf("error: bind\n\r");
#endif
 }
 if (listen(s_in,5)<0) 
 {
#ifdef DEBUG_ETHERNET
  printf("error: listen\n\r");
#endif
 }
 flag_start=1;
#ifdef DEBUG_ETHERNET
 printf("Сервер готов к работе\n\r");
#endif
 while(1)       
 {
  i_in=sizeof(struct sockaddr_in);
  if ((ns_in=accept(s_in,(struct sockaddr *)&cname_in,&i_in))<0) 
  {
#ifdef DEBUG_ETHERNET
   printf("error: accept\n\r");
#endif
  }
  else
  {
   flag_connection=1;
#ifdef DEBUG_ETHERNET
  printf("Клиент %s подключился\n\r",inet_ntoa(cname_in.sin_addr));
#endif
  }
  if (flag_connection!=0)
  do
  {
    nbyte = read(ns_in, &tmp_buf_recv_data, sizeof(buf_recv_data));
     if ((nbyte>0)&&(flag_recv==0))
     {
      flag_recv=1;
      memcpy(buf_recv_data, tmp_buf_recv_data, nbyte);
      size_of_recv_data=nbyte;
#ifdef DEBUG_ETHERNET
//      printf("вычитали %lu байт\n\r", nbyte);
#endif
     }
     else
     {
#ifdef DEBUG_ETHERNET
      printf("error: recv\n\r");
#endif
     }
  } while(nbyte>0);
  
  close(ns_in);
  flag_connection=0;
#ifdef DEBUG_ETHERNET
  printf("Клиент отключился...\n\r");
  printf("Слушаю...\n\r");
#endif
 }
}

/*------------------------------------------------------------*/
void init_ether( void )
{
    ETH_HOST[0]=10; ETH_HOST[1]=0; ETH_HOST[2]=0; ETH_HOST[3]=100;
    ETH_PORT_OUT=2000;
    sys_init();
    mem_init();
    memp_init();
    pbuf_init();
    netif_init();
    tcpip_init( NULL, NULL );
    //lwip_init();
    sys_thread_new("ether", tcp_thread, NULL, 1000, tskIDLE_PRIORITY);  
}


