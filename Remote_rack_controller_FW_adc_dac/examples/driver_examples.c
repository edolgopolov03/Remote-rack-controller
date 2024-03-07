/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include <stdio.h>
#include <sio.h>
#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"
#include "hal_spi_m_sync.h"

#define ADC0_SEN 5
#define ADC1_SEN 6
#define ADC_RST 7

#define ADC_READ  0x8000
#define ADC_WRITE 0x0000

static uint8_t example_SPI_0[12] = "Hello World!";

/**
 * Example of using ADC_0 to generate waveform.
 */
void ADC_0_example(void)
{
	uint8_t buffer[2];

	adc_sync_enable_channel(&ADC_0, 0);

	while (1) {
		adc_sync_read_channel(&ADC_0, 0, buffer, 2);
	}
}

/**
 * Example of using DAC_0 to generate waveform.
 */
void DAC_0_example(void)
{
	uint16_t i = 0;

	dac_sync_enable_channel(&DAC_0, 0);

	for (;;) {
		dac_sync_write(&DAC_0, 0, &i, 1);
		i = (i + 1) % 1024;
	}
}

void adc_reset(unsigned char nADC, unsigned short addr, unsigned char data)
{ 
  gpio_set_pin_level(SSA0, true); gpio_set_pin_level(SSA1, true);
	switch(nADC)
	{
		case 0: gpio_set_pin_level(SS0, false);	break;
		case 1: gpio_set_pin_level(SS1, false);	break;	
		case 2: gpio_set_pin_level(SS2, false);	break;	
		case 3: gpio_set_pin_level(SS3, false);	break;	
		case 4: gpio_set_pin_level(SS4, false);	break;	
		case 5: gpio_set_pin_level(SS5, false);	break;	
               default:;	
	}	
    __asm("nop\n");
	__asm("nop\n");
	__asm("nop\n");
	__asm("nop\n");
	__asm("nop\n");
	__asm("nop\n");
	switch(nADC)
	{
		case 0: gpio_set_pin_level(SS0, true);	break;
		case 1: gpio_set_pin_level(SS1, true);	break;	
		case 2: gpio_set_pin_level(SS2, true);	break;	
		case 3: gpio_set_pin_level(SS3, true);	break;	
		case 4: gpio_set_pin_level(SS4, true);	break;	
		case 5: gpio_set_pin_level(SS5, true);	break;	
	}	
  gpio_set_pin_level(SSA0, false);gpio_set_pin_level(SSA1, false);
}

void adc_write(unsigned char nADC, unsigned char chan, unsigned short addr, unsigned char data)
{
	struct io_descriptor *io;
	unsigned short tmp=ADC_WRITE|(addr&0x1FFF);
	unsigned char ctmp;
	spi_m_sync_get_io_descriptor(&SPI_0, &io);
	spi_m_sync_enable(&SPI_0);
	switch(chan&3)
	{
		case 0: gpio_set_pin_level(SSA0, false);gpio_set_pin_level(SSA1, false); break;
		case 1: gpio_set_pin_level(SSA0, true);	gpio_set_pin_level(SSA1, false); break;
		//case 2: gpio_set_pin_level(SSA0, fasle);	gpio_set_pin_level(SSA1, true); break; //reset ADC
	       default: ;
	}
	switch(nADC)
	{
		case 0: gpio_set_pin_level(SS0, false);	break;
		case 1: gpio_set_pin_level(SS1, false);	break;	
		case 2: gpio_set_pin_level(SS2, false);	break;	
		case 3: gpio_set_pin_level(SS3, false);	break;	
		case 4: gpio_set_pin_level(SS4, false);	break;	
		case 5: gpio_set_pin_level(SS5, false);	break;	
               default:;	
	}	
    ctmp=(tmp>> 8)&0xFF;
    io_write(io, &ctmp, 1);
    ctmp=(tmp>> 0)&0xFF;
    io_write(io, &ctmp, 1);
	io_write(io, &data, 1);
    __asm("nop\n");
    __asm("nop\n");
    __asm("nop\n");
    __asm("nop\n");
    __asm("nop\n");
    __asm("nop\n");
	switch(nADC)
	{
		case 0: gpio_set_pin_level(SS0, true);	break;
		case 1: gpio_set_pin_level(SS1, true);	break;	
		case 2: gpio_set_pin_level(SS2, true);	break;	
		case 3: gpio_set_pin_level(SS3, true);	break;	
		case 4: gpio_set_pin_level(SS4, true);	break;	
		case 5: gpio_set_pin_level(SS5, true);	break;	
               default:;	
	}	
  gpio_set_pin_level(SSA0, false);gpio_set_pin_level(SSA1, false);
}

unsigned char adc_read(unsigned char nADC, unsigned char chan, unsigned short addr)
{
	struct io_descriptor *io;
	unsigned short result, tmp=ADC_READ|(addr&0x1FFF);
	unsigned char data=0xFF;
	unsigned char ctmp;
	spi_m_sync_get_io_descriptor(&SPI_0, &io);
	spi_m_sync_enable(&SPI_0);
	switch(chan&3)
	{
		case 0: gpio_set_pin_level(SSA0, false);gpio_set_pin_level(SSA1, false); break;
		case 1: gpio_set_pin_level(SSA0, true);	gpio_set_pin_level(SSA1, false); break;
	       default: ;
	}
	switch(nADC)
	{
		case 0: gpio_set_pin_level(SS0, false);	break;
		case 1: gpio_set_pin_level(SS1, false);	break;	
		case 2: gpio_set_pin_level(SS2, false);	break;	
		case 3: gpio_set_pin_level(SS3, false);	break;	
		case 4: gpio_set_pin_level(SS4, false);	break;	
		case 5: gpio_set_pin_level(SS5, false);	break;	
               default:;	
	}	
    ctmp=(tmp>> 8)&0xFF;
	io_write(io, &ctmp, 1);
    ctmp=(tmp>> 0)&0xFF;
	io_write(io, &ctmp, 1);
	io_read(io, &data, 1);
	result=data;
    __asm("nop\n");
    __asm("nop\n");
    __asm("nop\n");
    __asm("nop\n");
    __asm("nop\n");
    __asm("nop\n");
	switch(nADC)
	{
		case 0: gpio_set_pin_level(SS0, true);	break;
		case 1: gpio_set_pin_level(SS1, true);	break;	
		case 2: gpio_set_pin_level(SS2, true);	break;	
		case 3: gpio_set_pin_level(SS3, true);	break;	
		case 4: gpio_set_pin_level(SS4, true);	break;	
		case 5: gpio_set_pin_level(SS5, true);	break;	
               default:;	
	}	
  gpio_set_pin_level(SSA0, false);gpio_set_pin_level(SSA1, false);
	return result;
}
void adc_init_ramp(unsigned char nADC, unsigned char chan)
{
                                adc_write(nADC, chan, 0x00, 0x3C);
                                //W600
                                delay_us(600);
                                adc_write(nADC, chan, 0x5F, 0x55);//0x15
                                adc_write(nADC, chan, 0x0B, 0x00);
                                adc_write(nADC, chan, 0x5E, 0x22);
                                adc_write(nADC, chan, 0xEE, 0x80);
                                adc_write(nADC, chan, 0xEF, 0x80);
                                adc_write(nADC, chan, 0x21, 0x00);
                                adc_write(nADC, chan, 0x14, 0x00);
                                adc_write(nADC, chan, 0x6E, 0x01);
                                adc_write(nADC, chan, 0x70, 0x0F);//K-1, K=16
                                adc_write(nADC, chan, 0x73, 0x0F);
                                adc_write(nADC, chan, 0x0D, 0x0F);
                                adc_write(nADC, chan, 0xFF, 0x01);
                                adc_write(nADC, chan, 0x5F, 0x54);//0x14
                                adc_write(nADC, chan, 0xF3, 0xFF);
                                //W3
                                delay_us(3);
                                adc_write(nADC, chan, 0xEE, 0x81);
                                adc_write(nADC, chan, 0xEF, 0x81);
                                adc_write(nADC, chan, 0xEE, 0x82);
                                adc_write(nADC, chan, 0xEF, 0x82);
                                adc_write(nADC, chan, 0xEE, 0x83);
                                adc_write(nADC, chan, 0xEF, 0x83);
                                adc_write(nADC, chan, 0xEE, 0x84);
                                adc_write(nADC, chan, 0xEF, 0x84);
                                adc_write(nADC, chan, 0xEE, 0x85);
                                adc_write(nADC, chan, 0xEF, 0x85);
                                adc_write(nADC, chan, 0xEE, 0x86);
                                adc_write(nADC, chan, 0xEF, 0x86);
                                adc_write(nADC, chan, 0xEE, 0x87);
                                adc_write(nADC, chan, 0xEF, 0x87);
                                //W3
                                delay_us(3);
}

void adc_init_ext_signal(unsigned char nADC, unsigned char chan, unsigned char inversion_flag)
{
								adc_write(nADC, chan, 0x00, 0x3C);
								//W600
								delay_us(600);
								adc_write(nADC, chan, 0x5F, 0x55);//0x15
								adc_write(nADC, chan, 0x0B, 0x00);
								adc_write(nADC, chan, 0x5E, 0x22);
								adc_write(nADC, chan, 0xEE, 0x80);
								adc_write(nADC, chan, 0xEF, 0x80);
								adc_write(nADC, chan, 0x21, 0x00);
if (inversion_flag!=0)			adc_write(nADC, chan, 0x14, 0x04);//inversion
else							adc_write(nADC, chan, 0x14, 0x00);
								adc_write(nADC, chan, 0x6E, 0x01);
								adc_write(nADC, chan, 0x70, 0x0F);//K-1, K=16
								adc_write(nADC, chan, 0x73, 0x0F);
								adc_write(nADC, chan, 0x0D, 0x00);
								adc_write(nADC, chan, 0xFF, 0x01);
								adc_write(nADC, chan, 0x5F, 0x54);//0x14
								adc_write(nADC, chan, 0xF3, 0xFF);
								//W3
								delay_us(3);
								adc_write(nADC, chan, 0xEE, 0x81);
								adc_write(nADC, chan, 0xEF, 0x81);
								adc_write(nADC, chan, 0xEE, 0x82);
								adc_write(nADC, chan, 0xEF, 0x82);
								adc_write(nADC, chan, 0xEE, 0x83);
								adc_write(nADC, chan, 0xEF, 0x83);
								adc_write(nADC, chan, 0xEE, 0x84);
								adc_write(nADC, chan, 0xEF, 0x84);
								adc_write(nADC, chan, 0xEE, 0x85);
								adc_write(nADC, chan, 0xEF, 0x85);
								adc_write(nADC, chan, 0xEE, 0x86);
								adc_write(nADC, chan, 0xEF, 0x86);
								adc_write(nADC, chan, 0xEE, 0x87);
								adc_write(nADC, chan, 0xEF, 0x87);
								//W3
								delay_us(3);
}

/**
 * Example of using USART_0 to write "Hello World" using the IO abstraction.
 *
 * Since the driver is asynchronous we need to use statically allocated memory for string
 * because driver initiates transfer and then returns before the transmission is completed.
 *
 * Once transfer has been completed the tx_cb function will be called.
 */

//static uint8_t example_USART_0[12] = "Hello World!";
//static uint8_t example_USART_0[6] = {0xAA, 0x00, 0x85, 0x85, 0xAB};
//static uint8_t example_USART_0[6] = {0xAA, 0x00, 0x01, 0x01, 0xAB};

//#define MAX_BUF_SIZE 256
//static unsigned char send_buf[MAX_BUF_SIZE]; //static
//static unsigned char recv_buf[MAX_BUF_SIZE]; //static

void USART_0_example_task(void *p)
{
	struct io_descriptor *io;
	uint16_t              data;

	(void)p;

	usart_os_get_io(&USART_0, &io);

	for (;;) {
		if (io->read(io, (uint8_t *)&data, 1) == 1) {
			io->write(io, (uint8_t *)&data, 1);
		}
	}
}

/**
 * Example of using USART_1 to write "Hello World" using the IO abstraction.
 */
void USART_1_example(void)
{
	struct io_descriptor *io;
	usart_sync_get_io_descriptor(&USART_1, &io);
	usart_sync_enable(&USART_1);

	io_write(io, (uint8_t *)"Hello World!", 12);
}
