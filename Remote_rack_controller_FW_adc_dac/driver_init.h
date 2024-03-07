/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef DRIVER_INIT_INCLUDED
#define DRIVER_INIT_INCLUDED

#include "atmel_start_pins.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <hal_atomic.h>
#include <hal_delay.h>
#include <hal_gpio.h>
#include <hal_init.h>
#include <hal_io.h>
#include <hal_sleep.h>

#include <hal_adc_sync.h>

#include <hal_dac_sync.h>

#include <hal_flash.h>

#include <hal_spi_m_os.h>
#include <hal_spi_m_os.h>
#include <hal_spi_m_os.h>
#include <hal_spi_m_os.h>
#include <hal_spi_m_os.h>
#include <hal_spi_m_os.h>

#include <hal_usart_os.h>

#include <hal_usart_sync.h>

#include "hal_usb_device.h"

#include <hal_mac_async.h>

#define USART_0_BUFFER_SIZE 256

extern struct flash_descriptor       FLASH_0;
extern struct spi_m_os_descriptor SPI_0;

extern struct adc_sync_descriptor ADC_0;

extern struct dac_sync_descriptor DAC_0;

extern struct usart_os_descriptor USART_0;
extern uint8_t                    USART_0_buffer[];

extern struct usart_sync_descriptor USART_1;

extern struct mac_async_descriptor ETHERNET_MAC_0;

void FLASH_0_init(void);
void FLASH_0_CLOCK_init(void);

void SPI_0_PORT_init(void);
void SPI_0_CLOCK_init(void);
void SPI_0_init(void);

void USART_0_PORT_init(void);
void USART_0_CLOCK_init(void);
void USART_0_init(void);

void USART_1_PORT_init(void);
void USART_1_CLOCK_init(void);
void USART_1_init(void);

void USB_0_CLOCK_init(void);
void USB_0_init(void);

void ETHERNET_MAC_0_CLOCK_init(void);
void ETHERNET_MAC_0_init(void);
void ETHERNET_MAC_0_PORT_init(void);
void ETHERNET_MAC_0_example(void);

/**
 * \brief Perform system initialization, initialize pins and clocks for
 * peripherals
 */
void system_init(void);

#ifdef __cplusplus
}
#endif
#endif // DRIVER_INIT_INCLUDED
