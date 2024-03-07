#include <string.h>
#include <stdbool.h>
#include "lwip/tcp.h"
#include <driver_examples.h>
#include <echo.h>
#include "driver_init.h"
#include "table.h"

global_table_t data;
AMP_control_table_t ctr;

bool dosync=false, retflag=false;
unsigned char channum=0, curmodule=0, curchan=0, curcmd=0, update_control=0;
int32_t dval, nch;

/* This is the data for the actual web page. */
static char recvdata[1560];
static char senddata[1560];

static char recvusartdata[256];
static char sendusartdata[256];

//static uint8_t cmd1[6] = {0xAA, 0x00, 0x01, 0x01, 0xAB};
//static uint8_t cmd2[6] = {0xAA, 0x00, 0x02, 0x02, 0xAB};

enum enum_mode { MAIN_COMMAND, GET_VALUE };
enum enum_cmd { SELECT_MODULE, SELECT_CHANEL, GET_DAC_VALUE };
	enum enum_mode ethmode;
	enum enum_cmd ethcmd;

void send_test(void)
{
	struct io_descriptor *io;
	int len, t, nt, time_count, once_get=0;
//	TickType_t xLastTime, xTimeNow = xTaskGetTickCount();
	
	//usart_async_get_io_descriptor(&USART_0, &io);
	usart_os_get_io(&USART_0, &io);

	while (1)
	{
		if (update_control==0)
		{
		    recvusartdata[0]=1;
			len=1;
		}
		else
		{
			portDISABLE_INTERRUPTS();
				len=sizeof(ctr);
				memcpy(recvusartdata, &ctr, len);
				update_control=0;
			portENABLE_INTERRUPTS();
		}
/*
		printf("send: ");
		for (t=0;t<len;t++) printf("0x%02x ", recvusartdata[t]);
		printf("\n\r");
*/
		len=com_preparebuf(sendusartdata, sizeof(sendusartdata), recvusartdata, len);
//		printf("com_preparebuf: len=%d\n\r", len);
		io_write(io, sendusartdata, len);
		time_count=0;
		while((com_read_ready() == 0)&&(time_count<250)) { time_count++; vTaskDelay(1); }
//		printf("Time_count=%d\n\r", time_count);
		if (time_count==250) len=-10; else len=com_read(recvusartdata, 256);
//		printf("com_read: len=%d\n\r", len);
		if ((len>0)&&(len==(sizeof(data.meas)+1)))
		{
/*
			printf("recv: ");
			for (t=0;t<len;t++) printf("0x%02x ", recvusartdata[t]);
			printf("\n\r");
*/
		}
		else
		{
			memset(recvusartdata, 0, sizeof(data));
//			data.response_err=len;
//			printf("Error=%d\r\n", data.response_err);
		}
		portDISABLE_INTERRUPTS();
		memcpy(&data.meas, &recvusartdata[1], sizeof(data.meas));
		portENABLE_INTERRUPTS();
//		printf("\r\n");
	    vTaskDelay(1000);
	}
}

void retmain(struct tcp_pcb *pcb)
{
	 sprintf(senddata, "\n\r> ");
	 tcp_write(pcb, senddata, strlen(senddata), 0);
}

/* This is the callback function that is called
when a TCP segment has arrived in the connection. */
static err_t echo_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
	struct io_descriptor *io;
    int len, t, time_count;

	//usart_async_get_io_descriptor(&USART_0, &io);
	usart_os_get_io(&USART_0, &io);


    //printf("CB recv %x, %x, %x, %x\n\r", arg, pcb, p, err);
    if (err != ERR_OK) {
        return err;
    }

    /* If we got a NULL pbuf in p, the remote end has closed
    the connection. */
    if(p != NULL) {

        /* The payload pointer in the pbuf contains the data
        in the TCP segment. */
      if (p->len>0)
      {
        memcpy(recvdata, p->payload, p->len);
//        sprintf(senddata, "0x%02X 0x%02X\n\r", recvdata[0], recvdata[1]);
//        tcp_write(pcb, senddata, strlen(senddata), 0);
		if (p->len>=1)
		{
		 curcmd=recvdata[0];
		            switch(curcmd)
					{
					  case 0x55:
								if (p->len>=2)
								{
									switch(recvdata[2])
									{
										case 0x02:	if ((sizeof(ctr)+1)==recvdata[1])
													{
														portDISABLE_INTERRUPTS();
														memcpy(&ctr, &recvdata[2], recvdata[1]);
														update_control=1;
														portENABLE_INTERRUPTS();
					            						printf("Control command\r\n");
													}
										case 0x01:		if (p->len>=3) 
														{
															senddata[0]=sizeof(data.meas);
															portDISABLE_INTERRUPTS();
															memcpy(&senddata[1], &data.meas, senddata[0]);
															portENABLE_INTERRUPTS();
//															tcp_ack_now(pcb);
															tcp_write(pcb, senddata, senddata[0]+1, 0);
														}
														else
														printf("len=%d\n", p->len);
													break;
									}
								}
						default:;
					}
     	if (recvdata[2]==0x02) printf("*"); else printf(".");
		}
      }
        /* Free the pbuf. */
        pbuf_free(p);
    }

    return ERR_OK;
}


/* This is the callback function that is called when
a connection has been accepted. */
static err_t
echo_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{
    if (err != ERR_OK) {
        return err;
    }

    /* Set up the function echo_recv() to be called when data
    arrives. */
    tcp_recv(pcb, echo_recv);
//pcb->remote_ip

    printf("Connection established\n\r");// with client %d.%d.%d.%d on port %d.\r\n", "%d.%d.%d.%d", ip4_addr1_16(ip_current_dest_addr()->addr), ip4_addr2_16(ip_current_dest_addr()->addr),
//    printf("Connection established with client %d.%d.%d.%d on port %d.\r\n", "%d.%d.%d.%d", ip4_addr1_16(ip_current_dest_addr()->addr), ip4_addr2_16(ip_current_dest_addr()->addr),
//    ip4_addr3_16(ip_current_dest_addr()->addr), ip4_addr4_16(ip_current_dest_addr()->addr), (int)(pcb->local_port));
	
    return ERR_OK;
}

/* The initialization function. */
err_t echo_init(void)
{
    struct tcp_pcb *pcb;
    err_t  err;
    /* Create a new TCP PCB. */
    pcb = tcp_new();
    if (pcb == NULL) {
        printf("F: Fail to create PCB\n\r");
        return ERR_BUF;
    }

//Peter
//	tcp_nagle_disable(pcb);
//	tcp_nagle_enable(pcb);

    /* Bind the PCB to TCP port 2000. */
    err = tcp_bind(pcb, NULL, 2000);
    if (err != ERR_OK) {
        printf("E: tcp_bind %x\n\r", err);
        return err;
    }
    /* Change TCP state to LISTEN. */
    pcb = tcp_listen(pcb);
    if (pcb == NULL) {
        printf("E: tcp_listen\n\r");
        return ERR_BUF;
    }
    /* Set up echo_accet() function to be called
    when a new connection arrives. */
    tcp_accept(pcb, echo_accept);

    return ERR_OK;
}

