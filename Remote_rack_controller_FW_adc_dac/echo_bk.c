#include <string.h>
#include <stdbool.h>
#include "lwip/tcp.h"
#include <driver_examples.h>
#include <echo.h>
#include "driver_init.h"
#include "table.h"

extern global_table_t data;
extern AMP_control_table_t ctr;

bool dosync=false, retflag=false;
unsigned char channum=0, curmodule=0, curchan=0, curcmd=0;
int32_t dval, nch;

/* This is the data for the actual web page. */
static char recvdata[1560];
static char senddata[1560];
static uint8_t cmd1[6] = {0xAA, 0x00, 0x01, 0x01, 0xAB};
static uint8_t cmd2[6] = {0xAA, 0x00, 0x02, 0x02, 0xAB};

enum enum_mode { MAIN_COMMAND, GET_VALUE };
enum enum_cmd { SELECT_MODULE, SELECT_CHANEL, GET_DAC_VALUE };
	enum enum_mode ethmode;
	enum enum_cmd ethcmd;

void send_test(void)
{
	struct io_descriptor *io;
	int len, t, time_count;

	//usart_async_get_io_descriptor(&USART_0, &io);
	usart_os_get_io(&USART_0, &io);
	recvdata[0]=0x55;
	recvdata[1]=1;
	recvdata[2]=1;
								printf("send: ");
								for (t=2;t<2+recvdata[1];t++) printf("0x%02x ", recvdata[t]);
								printf("\n\r");
								len=com_preparebuf(senddata, sizeof(senddata), &recvdata[2], recvdata[1]);
								printf("com_preparebuf: len=%d\n\r", len);
								io_write(io, senddata, len);
								time_count=0;
								while((com_read_ready() == 0)&&(time_count<250)) { time_count++; vTaskDelay(1); }
								printf("Time_count=%d\n\r", time_count);
								if (time_count==250) len=-10; else len=com_read(&recvdata[1], 256);
								printf("com_read: len=%d\n\r", len);
								if (len>0)
								{
									recvdata[0]=(unsigned char)len;
									printf("recv: ");
									for (t=0;t<len+1;t++) printf("0x%02x ", recvdata[t]);
									printf("\n\r");
									//memcpy(&data.meas, &recvdata[1], len);
								}
								else
								{
									memset(recvdata, 0, 2);
									recvdata[1]=len;
									printf("Error=%d\r\n", len);
									len=1;
								}
								printf("\r\n");
	
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
		switch(ethmode)
		{
		 case MAIN_COMMAND: 	 
	     if (recvdata[0]!=0x55) 
		 {
			 tcp_write(pcb, recvdata, p->len, 0); //echo
		 }
		 curcmd=recvdata[0];
         //switch(recvdata[0])
         //{
	      //case 13:  //sprintf(senddata, "\n\r");
          //          //tcp_write(pcb, senddata, strlen(senddata), 0);
		            switch(curcmd)
					{
	                  case '*': send_test();
								tcp_write(pcb, recvdata, len+1, 0);
                                break;
				      case 'm': sprintf(senddata, "\n\rEnter module number[0-5]: ");
				                tcp_write(pcb, senddata, strlen(senddata), 0);
								ethmode=GET_VALUE;
								ethcmd=SELECT_MODULE;
				                break;
				      case 'c': sprintf(senddata, "\n\rEnter chanel[0-3]: ");
				                tcp_write(pcb, senddata, strlen(senddata), 0);
				                ethmode=GET_VALUE;
				                ethcmd=SELECT_CHANEL;
				                break;
				      case 'd': sprintf(senddata, "\n\rEnter DAC value(0-2500 mV): ");
                                tcp_write(pcb, senddata, strlen(senddata), 0);
				                ethmode=GET_VALUE;
				                ethcmd=GET_DAC_VALUE;
								nch=0;
//					            sprintf(senddata, "Write to modele %d DAC%d value %d: ", 0, 0, 1000);
//	                            tcp_write(pcb, senddata, len, 0);
								break;
					  case '0': channum=0;
                                sprintf(senddata, "\n\rSelect ADC chan 0\n\r> ");
                                tcp_write(pcb, senddata, strlen(senddata), 0);
                                break;
                      case '1': channum=1;
                                sprintf(senddata, "\n\rSelect ADC chan 1\n\r> ");
                                tcp_write(pcb, senddata, strlen(senddata), 0);
                                break;
                      case '2': if (dosync==true) dosync=false; else dosync=true;
                                break;
                      case '3': printf("Read ADC0 register: 0x%02X\r\n", adc_read(0, channum, 1));
                                break;
                      case '4': sprintf(senddata, "init ADC0 ramp...");
                                tcp_write(pcb, senddata, strlen(senddata), 0);
								adc_init_ramp(0, channum);
								sprintf(senddata, "done\n\r> ");
								tcp_write(pcb, senddata, strlen(senddata), 0);
								break;
					  case '5': printf("init ADC0 external signal...\r\n");
					            adc_init_ext_signal(0, channum);
						        break;
					  case '6': printf("Read all reg ADC0...\r\n");
                                printf("0x%02X = 0x%02X\n\r", 0x00, adc_read(0, channum, 0x00));
                                printf("0x%02X = 0x%02X\n\r", 0x5F, adc_read(0, channum, 0x5F));
                                printf("0x%02X = 0x%02X\n\r", 0x0B, adc_read(0, channum, 0x0B));
			                    printf("0x%02X = 0x%02X\n\r", 0x5E, adc_read(0, channum, 0x5E));
                                printf("0x%02X = 0x%02X\n\r", 0xEE, adc_read(0, channum, 0xEE));
                                printf("0x%02X = 0x%02X\n\r", 0xEF, adc_read(0, channum, 0xEF));
                                printf("0x%02X = 0x%02X\n\r", 0x21, adc_read(0, channum, 0x21));
                                printf("0x%02X = 0x%02X\n\r", 0x14, adc_read(0, channum, 0x14));
                                printf("0x%02X = 0x%02X\n\r", 0x6E, adc_read(0, channum, 0x6E));
                                printf("0x%02X = 0x%02X\n\r", 0x73, adc_read(0, channum, 0x73));
                                printf("0x%02X = 0x%02X\n\r", 0x0D, adc_read(0, channum, 0x0D));
                                printf("0x%02X = 0x%02X\n\r", 0xFF, adc_read(0, channum, 0xFF));
                                printf("0x%02X = 0x%02X\n\r", 0x5F, adc_read(0, channum, 0x5F));
                                printf("0x%02X = 0x%02X\n\r", 0xF3, adc_read(0, channum, 0xF3));
			                    printf("0x%02X = 0x%02X\n\r", 0xEE, adc_read(0, channum, 0xEE));
                                printf("0x%02X = 0x%02X\n\r", 0xEF, adc_read(0, channum, 0xEF));
                                printf("0x%02X = 0x%02X\n\r", 0xEE, adc_read(0, channum, 0xEE));
                                printf("0x%02X = 0x%02X\n\r", 0xEF, adc_read(0, channum, 0xEF));
                                printf("0x%02X = 0x%02X\n\r", 0xEE, adc_read(0, channum, 0xEE));
                                printf("0x%02X = 0x%02X\n\r", 0xEF, adc_read(0, channum, 0xEF));
                                printf("0x%02X = 0x%02X\n\r", 0xEE, adc_read(0, channum, 0xEE));
                                printf("0x%02X = 0x%02X\n\r", 0xEF, adc_read(0, channum, 0xEF));
                                printf("0x%02X = 0x%02X\n\r", 0xEE, adc_read(0, channum, 0xEE));
                                printf("0x%02X = 0x%02X\n\r", 0xEF, adc_read(0, channum, 0xEF));
                                printf("0x%02X = 0x%02X\n\r", 0xEE, adc_read(0, channum, 0xEE));
			                    printf("0x%02X = 0x%02X\n\r", 0xEF, adc_read(0, channum, 0xEF));
                                printf("0x%02X = 0x%02X\n\r", 0xEE, adc_read(0, channum, 0xEE));
                                printf("0x%02X = 0x%02X\n\r", 0xEF, adc_read(0, channum, 0xEF));
            					break;
                      case '7': printf("test delay 5000 ms...");
                                delay_ms(5000);
            					printf("done\r\n");
            					break;
					  case 0x55:
								printf("send: ");
								for (t=2;t<2+recvdata[1];t++) printf("0x%02x ", recvdata[t]);
								printf("\n\r");
								len=com_preparebuf(senddata, sizeof(senddata), &recvdata[2], recvdata[1]);
								printf("com_preparebuf: len=%d\n\r", len);
								io_write(io, senddata, len);
								time_count=0;
								while((com_read_ready() == 0)&&(time_count<250)) { time_count++; vTaskDelay(1); }
								printf("Time_count=%d\n\r", time_count);
								if (time_count==250) len=-10; else len=com_read(&recvdata[1], 256);
								printf("com_read: len=%d\n\r", len);
								if (len>0)
								{
								 recvdata[0]=(unsigned char)len;
								printf("recv: ");
								for (t=0;t<len+1;t++) printf("0x%02x ", recvdata[t]);
								printf("\n\r");
								 //memcpy(&data.meas, &recvdata[1], len);
								}
								else
								{
								 memset(recvdata, 0, 2);
								 recvdata[1]=len;
								 printf("Error=%d\r\n", len);
								 len=1;
								}
								tcp_write(pcb, recvdata, len+1, 0);
								printf("\r\n");

            					break;
						default: retmain(pcb);
					}
//					break;
        //}
		break;
		case GET_VALUE: //get VALUE
                    switch(ethcmd)
					{
					 case SELECT_MODULE: switch(recvdata[0])
					                     {
										   case '0':
										   case '1':
										   case '2':
										   case '3':
										   case '4':
										   case '5':
										   tcp_write(pcb, recvdata, p->len, 0); //echo
										   curmodule=atoi(recvdata);
										   sprintf(senddata, "\n\rSelected module %d\n\r> ", curmodule);
								           tcp_write(pcb, senddata, strlen(senddata), 0);
                          				   ethmode=MAIN_COMMAND;
										 }
										 break;
					 case SELECT_CHANEL: switch(recvdata[0])
                                         {
						                   case '0':
						                   case '1':
						                   case '2':
						                   case '3':
										   tcp_write(pcb, recvdata, p->len, 0); //echo
						                   curchan=atoi(recvdata);
						                   sprintf(senddata, "\n\rSelected chanel %d\n\r> ", curchan);
								           tcp_write(pcb, senddata, strlen(senddata), 0);
					                       ethmode=MAIN_COMMAND;
										 }
								         break;
					 case GET_DAC_VALUE: switch(recvdata[0])
					                     {
										   case '0':
										   case '1':
										   case '2':
										   case '3':
										   case '4':
										   case '5':
										   case '6':
										   case '7':
										   case '8':
										   case '9': tcp_write(pcb, recvdata, p->len, 0);
										             if (nch<sizeof(senddata)) senddata[nch++]=recvdata[0];
										             break;
										   case 13:  dval=atoi(senddata);
					                                 senddata[0]=0x03;
													 senddata[1]=curmodule;
													 senddata[2]=curchan;
					                                 senddata[3]=dval&0xFF;
													 senddata[4]=(dval>>8)&0xFF;
													 len=com_preparebuf(recvdata, sizeof(recvdata), senddata, 5);
					                                 io_write(io, recvdata, len);
					                                 sprintf(senddata, "\n\rWrite %d mV in module %d DAC%d\n\r> ", dval, curmodule, curchan);
													 tcp_write(pcb, senddata, strlen(senddata), 0);
					                                 ethmode=MAIN_COMMAND;
										             break;
										   case 0x7F: //backspace
										             tcp_write(pcb, "\b \b", 3, 0);
													 senddata[--nch]=0;
										             break;
									     }
									     break;
										 
					}
		break;
	   }
      }
        /* Free the pbuf. */
        pbuf_free(p);
    }
    /* Close the connection. */
//    tcp_close(pcb);

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

