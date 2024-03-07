#ifndef BASIC_WEB_SERVER_H
#define BASIC_WEB_SERVER_H

/* Initialisation required by lwIP. */
extern void init_ether( void );

extern unsigned char ETH_IP[4], ETH_HOST[4];
extern unsigned short ETH_PORT_OUT;
extern void Set_ETH_IP(unsigned char *vmas);
extern void Set_ETH_HOST(unsigned char *vmas);
extern void Set_ETH_PORT_OUT(unsigned short value);
extern void Get_ETH_IP(unsigned char *vmas);
extern void Get_ETH_HOST(unsigned char *vmas);
extern unsigned short Get_ETH_PORT_OUT(void);

extern int ether_connect(void);
extern int ether_send_to(char *databuf, unsigned long datalength);
extern int ether_disconnect(void);
extern unsigned char ether_getconnection(void);
extern unsigned char ether_getstart(void);
extern unsigned char ether_read_ready(void);
extern int ether_read(char *databuf, unsigned long datalength);
extern void ether_send(char *databuf, unsigned long datalength);
extern void ether_flush(void);

#endif

