#include <atmel_start.h>
#include "lwip/sys.h"
#include "hpl_gmac_config.h"
#include "lwip_macif_config.h"
#include "arch/sys_arch.h"
#include "tcpip.h"
#include "api.h"
#include "echo.h"
#include "table.h"
#include "sio.h"
#include <stdio.h>
#include <string.h>
//#define UART_DEBUG 1

global_table_t data;
AMP_control_table_t ctr;

unsigned char update_control=0;

static char recvdata[1560];
static char senddata[1560];

static char recvusartdata[256];
static char sendusartdata[256];

#define TASK_ETH_STACK_SIZE 4000
#define TASK_ETH_STACK_PRIORITY (tskIDLE_PRIORITY + 3)

#define TASK_LWIP_STACK_SIZE 4000
#define TASK_LWIP_STACK_PRIORITY (tskIDLE_PRIORITY + 2)

#define TASK_UART_STACK_SIZE 256
#define TASK_UART_STACK_PRIORITY (tskIDLE_PRIORITY + 1)

#define TASK_ADC_STACK_SIZE (1024 / sizeof(portSTACK_TYPE))
#define TASK_ADC_STACK_PRIORITY (tskIDLE_PRIORITY + 2)

#define TASK_DAC_STACK_SIZE (1024 / sizeof(portSTACK_TYPE))
#define TASK_DAC_STACK_PRIORITY (tskIDLE_PRIORITY + 1)

static SemaphoreHandle_t uart_mutex;
static TaskHandle_t      xCreatedADCTask;
static TaskHandle_t      xCreatedDACTask;

static TaskHandle_t xCreatedADC_initTask;
static TaskHandle_t xCreatedDAC_initTask;
static TaskHandle_t xCreatedUART_scanTask;
static TaskHandle_t xCreatedExampleTask;
static TaskHandle_t xCreatedEthTask;

static void uart_mutex_create(void)
{
	uart_mutex = xSemaphoreCreateMutex();

	if (uart_mutex == NULL) {
		while (1) {
			;
		}
	}
}

static bool uart_mutex_take(void)
{
	return xSemaphoreTake(uart_mutex, ~0);
}

static void uart_mutex_give(void)
{
	xSemaphoreGive(uart_mutex);
}


typedef struct tag_gmac_device {
	/** Reference to lwIP netif structure. */
	struct netif *netif;
	
	#if NO_SYS == 0
	/** RX task notification semaphore. */
	sys_sem_t rx_sem;
	#endif
}gmac_device;
gmac_device gs_gmac_dev;

static bool           link_up   = false;

u32_t sys_now(void)
{
	return xTaskGetTickCount();
}

static void print_ipaddress(void)
{
	static char tmp_buff[16];
	printf("IP_ADDR    : %s\r\n", ipaddr_ntoa_r((const ip_addr_t *)&(TCPIP_STACK_INTERFACE_0_desc.ip_addr), tmp_buff, 16));
	printf("NET_MASK   : %s\r\n", ipaddr_ntoa_r((const ip_addr_t *)&(TCPIP_STACK_INTERFACE_0_desc.netmask), tmp_buff, 16));
	printf("GATEWAY_IP : %s\r\n", ipaddr_ntoa_r((const ip_addr_t *)&(TCPIP_STACK_INTERFACE_0_desc.gw), tmp_buff, 16));
}

void uart_scan_task(void)
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
			//			portDISABLE_INTERRUPTS();
			len=sizeof(ctr);
			memcpy(recvusartdata, &ctr, len);
			update_control=0;
			//			portENABLE_INTERRUPTS();
		}
		#ifdef UART_DEBUG
		printf("send: ");
		for (t=0;t<len;t++) printf("0x%02x ", recvusartdata[t]);
		printf("\n\r");
		#endif
		len=com_preparebuf(sendusartdata, sizeof(sendusartdata), recvusartdata, len);

		#ifdef UART_DEBUG
		printf("com_preparebuf: len=%d\n\r", len);
		#endif
		
		io_write(io, sendusartdata, len);
		time_count=0;
		while((com_read_ready() == 0)&&(time_count<250)) { time_count++; vTaskDelay(1); }

		#ifdef UART_DEBUG
		printf("Time_count=%d\n\r", time_count);
		#endif
		
		if (time_count==250) len=-10; else len=com_read(recvusartdata, 256);

		#ifdef UART_DEBUG
		printf("com_read: len=%d\n\r", len);
		#endif

		if ((len>0)&&(len==sizeof(data.meas)))
		{
			#ifdef UART_DEBUG
			printf("recv: ");
			for (t=0;t<len;t++) printf("0x%02x ", recvusartdata[t]);
			printf("\n\r");
			#endif
		}
		else
		{
			memset(recvusartdata, 0, sizeof(data.meas));
                        recvusartdata[0]=sizeof(data.meas);
			recvusartdata[1]=recvusartdata[1]|(1<<0);//data.meas.responce_error=data.meas.responce_error=1<<0;
                        recvusartdata[2]=len;//data.meas.size_or_errorcode=len;
			printf("Error=%d\r\n", len);
		}
		//		portDISABLE_INTERRUPTS();
		memcpy(&data.meas, recvusartdata, sizeof(data.meas));
		//		portENABLE_INTERRUPTS();
		vTaskDelay(1000);
	}
}

void ethernet_task(void)
{
 unsigned long len;
 uint32_t total_pages, page_size;
 static uint8_t src_data[512];
	total_pages = _flash_get_total_pages(&FLASH_0);//(&flash->dev);
	page_size = flash_get_page_size(&FLASH_0);
	printf("Pages of flash=%d page_size=%d\n\r", total_pages, page_size);
//	memset(src_data, 0, 512);

 while(1)
 {
//   portENTER_CRITICAL();
   if (ether_read_ready()!=0)
   {
      len=ether_read(recvdata, sizeof(recvdata));
      if (len>0)
      {
		switch(recvdata[0])
		{
		case 1: printf("Reading from flash...");
				ether_send(senddata, 128);
		default:;
		}
	  }
   }
   else
   {
//    portEXIT_CRITICAL();
   }
  vTaskDelay(1);
 }

}

/*void init_adc_and_sync_task()
{
	

}
*/

static void read_macaddress(u8_t *mac)
{
	#if CONF_AT24MAC_ADDRESS != 0
	uint8_t addr = 0x9A;

	i2c_m_sync_enable(&I2C_AT24MAC);
	i2c_m_sync_set_slaveaddr(&I2C_AT24MAC, CONF_AT24MAC_ADDRESS, I2C_M_SEVEN);
	io_write(&(I2C_AT24MAC.io), &addr, 1);
	io_read(&(I2C_AT24MAC.io), mac, 6);
	#else
	/* set mac to 0x11 if no EEPROM mounted */
	//	memset(mac, 0x11, 6);
	mac[0]=0xFC;
	mac[1]=0xC2;
	mac[2]=0x3D;
	mac[3]=0x0D;
	mac[4]=0x96;
	mac[5]=0x06;//05
	#endif
}

void gmac_handler_cb(void)
{
	portBASE_TYPE xGMACTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(gs_gmac_dev.rx_sem, &xGMACTaskWoken);
	NVIC_ClearPendingIRQ(GMAC_IRQn);
	portEND_SWITCHING_ISR(xGMACTaskWoken);
}

void gmac_task(void *pvParameters)
{
	//int32_t ret;
	gmac_device *ps_gmac_dev = pvParameters;

	netif_set_up(&TCPIP_STACK_INTERFACE_0_desc);

	netif_set_default(&TCPIP_STACK_INTERFACE_0_desc);

	mac_async_enable(&ETHERNET_MAC_0);

	/* Print IP address info */
	if (link_up && TCPIP_STACK_INTERFACE_0_desc.ip_addr.addr) {
		link_up = false;
		print_ipaddress();
	}

	while (1) {
		/* Wait for the counting RX notification semaphore. */
		xSemaphoreTake( ps_gmac_dev->rx_sem, portMAX_DELAY);
		/* Process the incoming packet. */
		ethernetif_mac_input(ps_gmac_dev->netif);
	}
}

//OS task for ADC
 
static void task_adc(void *p)
{
	
	uint8_t buffer[2];
	adc_sync_enable_channel(&ADC_0, 0);
	printf("dac channel enabled\n\r");
	
	//struct io_descriptor *io;
	//usart_sync_get_io_descriptor(&USART_1, &io);
	//usart_sync_enable(&USART_1);
	//printf("rs485 enabled\n\r");
	
	for (;;) {
		adc_sync_read_channel(&ADC_0, 0, buffer, 2);
		if (uart_mutex_take()) {
			printf("ADC is working\n\r");
			printf("%d\n\r", *buffer);
			uart_mutex_give();
			os_sleep(500);
		}
	}
}

//OS task for DAC

static void task_dac(void *p)
{
	
	uint16_t i = 0;
	
	dac_sync_enable_channel(&DAC_0, 0);
	printf("dac channel enabled\n\r");
	
	//struct io_descriptor *io;
	//usart_sync_get_io_descriptor(&USART_1, &io);
	//usart_sync_enable(&USART_1);
	//printf("rs485 enabled\n\r");
	
	for (;;) {
		dac_sync_write(&DAC_0, 0, &i, 1);
		i = (i + 1) % 1024;
		
		if (uart_mutex_take()) {
			printf("DAC is working\n\r");
			uart_mutex_give();
			os_sleep(500);
		}
	}
}


//static void task_adc_pause(bool pause);

//static void task_dac_pause(bool pause);

/*int main(void)
{
	int32_t ret;
	u8_t    mac[6];

	 Initializes MCU, drivers and middleware */
	//atmel_start_init();

	/* Read MacAddress from EEPROM */
	//read_macaddress(mac);
	//mac_async_register_callback(&ETHERNET_MAC_0, MAC_ASYNC_RECEIVE_CB, (FUNC_PTR)gmac_handler_cb);
	//hri_gmac_set_IMR_RCOMP_bit(ETHERNET_MAC_0.dev.hw);//???
	/* Enable NVIC GMAC interrupt. */
	/* Interrupt priorities. (lowest value = highest priority) */
	/* ISRs using FreeRTOS *FromISR APIs must have priorities below or equal to */
	/* configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY. */
	//NVIC_SetPriority(GMAC_IRQn,5);
	//NVIC_EnableIRQ(GMAC_IRQn);
	
//	eth_ipstack_init();
    //init_ether();
	
	/*do {
		ret = ethernet_phy_get_link_status(&ETHERNET_PHY_0_desc, &link_up);
		if (ret == ERR_NONE && link_up) {
			break;
		}
	} while (true);

    TCPIP_STACK_INTERFACE_0_init(mac);
    gs_gmac_dev.netif = &TCPIP_STACK_INTERFACE_0_desc;*/
    /* Incoming packet notification semaphore. */
    //gs_gmac_dev.rx_sem = xSemaphoreCreateCounting( CONF_GMAC_RXDESCR_NUM,0 );

    /*if( gs_gmac_dev.rx_sem == NULL )
    {
	    printf("error\n\r");
	    //return ERR_MEM;
    }*/

//	if (xTaskCreate((TaskFunction_t)init_adc_and_sync_task, "adc_init", TASK_ADC_INIT_STACK_SIZE, NULL, TASK_ADC_INIT_UART_STACK_PRIORITY, xCreatedADC_initTask)!= pdPASS) { while (1); }
//    if (xTaskCreate((TaskFunction_t)uart_scan_task, "uartscan", TASK_UART_STACK_SIZE, NULL, TASK_UART_STACK_PRIORITY, xCreatedUART_scanTask)!= pdPASS) { while (1); }
	//if (xTaskCreate((TaskFunction_t)ethernet_task, "Lwip", TASK_LWIP_STACK_SIZE, NULL, TASK_LWIP_STACK_PRIORITY, xCreatedExampleTask)!= pdPASS) { while (1); }
	/* Create Application task */
	//if (xTaskCreate((TaskFunction_t)gmac_task, "Phyether", TASK_ETH_STACK_SIZE, &gs_gmac_dev, TASK_ETH_STACK_PRIORITY, xCreatedEthTask)!= pdPASS) { while (1); }

	/* Start the RTOS Scheduler */
	//vTaskStartScheduler();

	//while (1) {
		
	//}
//}

int main(void)
{
	atmel_start_init();
	//#define TEST_TEMPLATE_EXAMPLE 0
	//#if TEST_TEMPLATE_EXAMPLE

	//if (xTaskCreate(USART_0_example_task, "Example", TASK_CLI_STACK_SIZE, NULL, TASK_CLI_PRIORITY, &xCreatedConsoleTask)
	//!= pdPASS) {
		//while (1) {
			//;
		//}
	//}

	//#else
	uart_mutex_create();
	if (xTaskCreate(task_dac, "DAC", TASK_DAC_STACK_SIZE, NULL, TASK_DAC_STACK_PRIORITY, &xCreatedDACTask) != pdPASS) {while (1);}
	if (xTaskCreate(task_adc, "ADC", TASK_ADC_STACK_SIZE, NULL, TASK_ADC_STACK_PRIORITY, &xCreatedADCTask) != pdPASS) {while (1);}
	//#endif
	vTaskStartScheduler();

	return 0;
}
