/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>
#include "hal_spi_m_sync.h"
#include "stdio_start.h"
#include "tc_lite.h"

#include <hpl_adc_base.h>
#include <stdio.h>

/* The priority of the peripheral should be between the low and high interrupt priority set by chosen RTOS,
 * Otherwise, some of the RTOS APIs may fail to work inside interrupts
 * In case of FreeRTOS,the Lowest Interrupt priority is set by configLIBRARY_LOWEST_INTERRUPT_PRIORITY and
 * Maximum interrupt priority by configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY, So Interrupt Priority of the peripheral
 * should be between configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY and configLIBRARY_LOWEST_INTERRUPT_PRIORITY
 */
#define PERIPHERAL_INTERRUPT_PRIORITY (configLIBRARY_LOWEST_INTERRUPT_PRIORITY - 1)

struct flash_descriptor FLASH_0;

struct spi_m_os_descriptor SPI_0;

struct adc_sync_descriptor ADC_0;

struct dac_sync_descriptor DAC_0;

struct usart_os_descriptor USART_0;
uint8_t                    USART_0_buffer[USART_0_BUFFER_SIZE];

struct usart_sync_descriptor USART_1;

struct mac_async_descriptor ETHERNET_MAC_0;

void FLASH_0_CLOCK_init(void)
{

	hri_mclk_set_AHBMASK_NVMCTRL_bit(MCLK);
}

void FLASH_0_init(void)
{
	FLASH_0_CLOCK_init();
	flash_init(&FLASH_0, NVMCTRL);
}

void SPI_0_PORT_init(void)
{

	gpio_set_pin_level(PA04,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PA04, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PA04, PINMUX_PA04D_SERCOM0_PAD0);

	gpio_set_pin_level(PA05,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PA05, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PA05, PINMUX_PA05D_SERCOM0_PAD1);

	// Set pin direction to input
	gpio_set_pin_direction(PA06, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(PA06,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA06, PINMUX_PA06D_SERCOM0_PAD2);
}

void SPI_0_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM0_GCLK_ID_CORE, CONF_GCLK_SERCOM0_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM0_GCLK_ID_SLOW, CONF_GCLK_SERCOM0_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBAMASK_SERCOM0_bit(MCLK);
}

void SPI_0_init(void)
{
	SPI_0_CLOCK_init();
	spi_m_sync_init(&SPI_0, SERCOM0);
	SPI_0_PORT_init();
}

void ADC_0_PORT_init(void)
{

	// Disable digital pin circuitry
	gpio_set_pin_direction(PB04, GPIO_DIRECTION_OFF);

	gpio_set_pin_function(PB04, PINMUX_PB04B_ADC1_AIN6);
}

void ADC_0_CLOCK_init(void)
{
	hri_mclk_set_APBDMASK_ADC1_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, ADC1_GCLK_ID, CONF_GCLK_ADC1_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
}

void ADC_0_init(void)
{
	ADC_0_CLOCK_init();
	ADC_0_PORT_init();
	adc_sync_init(&ADC_0, ADC1, (void *)NULL);
}

void DAC_0_PORT_init(void)
{

	// Disable digital pin circuitry
	gpio_set_pin_direction(PA02, GPIO_DIRECTION_OFF);

	gpio_set_pin_function(PA02, PINMUX_PA02B_DAC_VOUT0);

	// Disable digital pin circuitry
	gpio_set_pin_direction(PA05, GPIO_DIRECTION_OFF);

	gpio_set_pin_function(PA05, PINMUX_PA05B_DAC_VOUT1);
}

void DAC_0_CLOCK_init(void)
{

	hri_mclk_set_APBDMASK_DAC_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, DAC_GCLK_ID, CONF_GCLK_DAC_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
}

void DAC_0_init(void)
{
	DAC_0_CLOCK_init();
	dac_sync_init(&DAC_0, DAC);
	DAC_0_PORT_init();
}

void USART_0_PORT_init(void)
{

	gpio_set_pin_function(PC04, PINMUX_PC04C_SERCOM6_PAD0);

	gpio_set_pin_function(PC05, PINMUX_PC05C_SERCOM6_PAD1);
}

void USART_0_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM6_GCLK_ID_CORE, CONF_GCLK_SERCOM6_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM6_GCLK_ID_SLOW, CONF_GCLK_SERCOM6_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBDMASK_SERCOM6_bit(MCLK);
}

void USART_0_init(void) //для связи с контроллером предусилителей
{

	USART_0_CLOCK_init();
	uint32_t irq = SERCOM6_0_IRQn;
	for (uint32_t i = 0; i < 4; i++) {
		NVIC_SetPriority((IRQn_Type)irq, PERIPHERAL_INTERRUPT_PRIORITY);
		irq++;
	}
	usart_os_init(&USART_0, SERCOM6, USART_0_buffer, USART_0_BUFFER_SIZE, (void *)NULL);
	usart_os_enable(&USART_0);
	USART_0_PORT_init();
}

void USART_1_PORT_init(void)
{

	gpio_set_pin_function(PD08, PINMUX_PD08C_SERCOM7_PAD0);

	gpio_set_pin_function(PD09, PINMUX_PD09C_SERCOM7_PAD1);
}

void USART_1_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM7_GCLK_ID_CORE, CONF_GCLK_SERCOM7_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM7_GCLK_ID_SLOW, CONF_GCLK_SERCOM7_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBDMASK_SERCOM7_bit(MCLK);
}

void USART_1_init(void) // для отладки
{
	USART_1_CLOCK_init();
	usart_sync_init(&USART_1, SERCOM7, (void *)NULL);
	USART_1_PORT_init();
}

void USB_0_PORT_init(void)
{

	gpio_set_pin_direction(PA24,
	                       // <y> Pin direction
	                       // <id> pad_direction
	                       // <GPIO_DIRECTION_OFF"> Off
	                       // <GPIO_DIRECTION_IN"> In
	                       // <GPIO_DIRECTION_OUT"> Out
	                       GPIO_DIRECTION_OUT);

	gpio_set_pin_level(PA24,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_pull_mode(PA24,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA24,
	                      // <y> Pin function
	                      // <id> pad_function
	                      // <i> Auto : use driver pinmux if signal is imported by driver, else turn off function
	                      // <PINMUX_PA24H_USB_DM"> Auto
	                      // <GPIO_PIN_FUNCTION_OFF"> Off
	                      // <GPIO_PIN_FUNCTION_A"> A
	                      // <GPIO_PIN_FUNCTION_B"> B
	                      // <GPIO_PIN_FUNCTION_C"> C
	                      // <GPIO_PIN_FUNCTION_D"> D
	                      // <GPIO_PIN_FUNCTION_E"> E
	                      // <GPIO_PIN_FUNCTION_F"> F
	                      // <GPIO_PIN_FUNCTION_G"> G
	                      // <GPIO_PIN_FUNCTION_H"> H
	                      // <GPIO_PIN_FUNCTION_I"> I
	                      // <GPIO_PIN_FUNCTION_J"> J
	                      // <GPIO_PIN_FUNCTION_K"> K
	                      // <GPIO_PIN_FUNCTION_L"> L
	                      // <GPIO_PIN_FUNCTION_M"> M
	                      // <GPIO_PIN_FUNCTION_N"> N
	                      PINMUX_PA24H_USB_DM);

	gpio_set_pin_direction(PA25,
	                       // <y> Pin direction
	                       // <id> pad_direction
	                       // <GPIO_DIRECTION_OFF"> Off
	                       // <GPIO_DIRECTION_IN"> In
	                       // <GPIO_DIRECTION_OUT"> Out
	                       GPIO_DIRECTION_OUT);

	gpio_set_pin_level(PA25,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_pull_mode(PA25,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA25,
	                      // <y> Pin function
	                      // <id> pad_function
	                      // <i> Auto : use driver pinmux if signal is imported by driver, else turn off function
	                      // <PINMUX_PA25H_USB_DP"> Auto
	                      // <GPIO_PIN_FUNCTION_OFF"> Off
	                      // <GPIO_PIN_FUNCTION_A"> A
	                      // <GPIO_PIN_FUNCTION_B"> B
	                      // <GPIO_PIN_FUNCTION_C"> C
	                      // <GPIO_PIN_FUNCTION_D"> D
	                      // <GPIO_PIN_FUNCTION_E"> E
	                      // <GPIO_PIN_FUNCTION_F"> F
	                      // <GPIO_PIN_FUNCTION_G"> G
	                      // <GPIO_PIN_FUNCTION_H"> H
	                      // <GPIO_PIN_FUNCTION_I"> I
	                      // <GPIO_PIN_FUNCTION_J"> J
	                      // <GPIO_PIN_FUNCTION_K"> K
	                      // <GPIO_PIN_FUNCTION_L"> L
	                      // <GPIO_PIN_FUNCTION_M"> M
	                      // <GPIO_PIN_FUNCTION_N"> N
	                      PINMUX_PA25H_USB_DP);
}

/* The USB module requires a GCLK_USB of 48 MHz ~ 0.25% clock
 * for low speed and full speed operation. */
#if (CONF_GCLK_USB_FREQUENCY > (48000000 + 48000000 / 400)) || (CONF_GCLK_USB_FREQUENCY < (48000000 - 48000000 / 400))
#warning USB clock should be 48MHz ~ 0.25% clock, check your configuration!
#endif

void USB_0_CLOCK_init(void)
{

	hri_gclk_write_PCHCTRL_reg(GCLK, USB_GCLK_ID, CONF_GCLK_USB_SRC | GCLK_PCHCTRL_CHEN);
	hri_mclk_set_AHBMASK_USB_bit(MCLK);
	hri_mclk_set_APBBMASK_USB_bit(MCLK);
}

void TIMER_1_CLOCK_init(void)
{
	hri_mclk_set_APBAMASK_TC0_bit(MCLK);

	hri_mclk_set_APBAMASK_TC1_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, TC0_GCLK_ID, CONF_GCLK_TC0_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
}

void TIMER_0_CLOCK_init(void)
{
	hri_mclk_set_APBBMASK_TC2_bit(MCLK);

	hri_mclk_set_APBBMASK_TC3_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, TC2_GCLK_ID, CONF_GCLK_TC2_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
}

void USB_0_init(void)
{
	USB_0_CLOCK_init();
	usb_d_init();
	USB_0_PORT_init();
}

void ETHERNET_MAC_0_PORT_init(void)
{

	gpio_set_pin_function(PC21, PINMUX_PC21L_GMAC_GCOL);

	gpio_set_pin_function(PA16, PINMUX_PA16L_GMAC_GCRS);

	gpio_set_pin_function(PA20, PINMUX_PA20L_GMAC_GMDC);

	gpio_set_pin_function(PA21, PINMUX_PA21L_GMAC_GMDIO);

	gpio_set_pin_function(PA13, PINMUX_PA13L_GMAC_GRX0);

	gpio_set_pin_function(PA12, PINMUX_PA12L_GMAC_GRX1);

	gpio_set_pin_function(PC15, PINMUX_PC15L_GMAC_GRX2);

	gpio_set_pin_function(PC14, PINMUX_PC14L_GMAC_GRX3);

	gpio_set_pin_function(PC18, PINMUX_PC18L_GMAC_GRXCK);

	gpio_set_pin_function(PC20, PINMUX_PC20L_GMAC_GRXDV);

	gpio_set_pin_function(PA15, PINMUX_PA15L_GMAC_GRXER);

	gpio_set_pin_function(PA18, PINMUX_PA18L_GMAC_GTX0);

	gpio_set_pin_function(PA19, PINMUX_PA19L_GMAC_GTX1);

	gpio_set_pin_function(PC16, PINMUX_PC16L_GMAC_GTX2);

	gpio_set_pin_function(PC17, PINMUX_PC17L_GMAC_GTX3);

	gpio_set_pin_function(PA14, PINMUX_PA14L_GMAC_GTXCK);

	gpio_set_pin_function(PA17, PINMUX_PA17L_GMAC_GTXEN);

	gpio_set_pin_function(PC19, PINMUX_PC19L_GMAC_GTXER);
}

void ETHERNET_MAC_0_CLOCK_init(void)
{
	hri_mclk_set_AHBMASK_GMAC_bit(MCLK);
	hri_mclk_set_APBCMASK_GMAC_bit(MCLK);
}

void ETHERNET_MAC_0_init(void)
{
	ETHERNET_MAC_0_CLOCK_init();
	mac_async_init(&ETHERNET_MAC_0, GMAC);
	ETHERNET_MAC_0_PORT_init();
}

void ETHERNET_MAC_0_example(void)
{
	mac_async_enable(&ETHERNET_MAC_0);
	mac_async_write(&ETHERNET_MAC_0, (uint8_t *)"Hello World!", 12);
}

void system_init(void)
{
	//int n, curmodule=0, channum=0;
	uint32_t total_pages, page_size;
	static uint8_t src_data[512];

	init_mcu();

	// GPIO on PA03

	gpio_set_pin_level(SYNC,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);
	// Set pin direction to output
	gpio_set_pin_direction(SYNC, GPIO_DIRECTION_IN);
	gpio_set_pin_function(SYNC, GPIO_PIN_FUNCTION_OFF);

	gpio_set_pin_level(SS0,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(SS0, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SS0, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA07

	gpio_set_pin_level(SS2,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(SS2, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SS2, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA22

	gpio_set_pin_level(PWDN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(PWDN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PWDN, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA23

	gpio_set_pin_level(RESET,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(RESET, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(RESET, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB04

	gpio_set_pin_level(SS4,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(SS4, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SS4, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB05

	gpio_set_pin_level(SSA1,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(SSA1, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SSA1, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB15

	gpio_set_pin_level(SS5,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(SS5, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SS5, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB23

	gpio_set_pin_level(TXDIS,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(TXDIS, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(TXDIS, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC07

	// Set pin direction to input
	gpio_set_pin_direction(RST, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(RST,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(RST, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC13

	// Set pin direction to input
	gpio_set_pin_direction(SPEED, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(SPEED,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(SPEED, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC24

	gpio_set_pin_level(SS3,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(SS3, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SS3, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC26

	gpio_set_pin_level(SS1,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(SS1, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SS1, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PD00

	gpio_set_pin_level(SSA0,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(SSA0, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SSA0, GPIO_PIN_FUNCTION_OFF);

	FLASH_0_init();

	SPI_0_init();

	ADC_0_init();

	DAC_0_init();

	USART_0_init();

	USART_1_init();

	stdio_redirect_init();

	total_pages = _flash_get_total_pages(&FLASH_0);//(&flash->dev);
	page_size = flash_get_page_size(&FLASH_0);
	printf("Reading from flash...");
	flash_read(&FLASH_0, (total_pages-1)*page_size, src_data, page_size);
	printf("done\n\r");

/* for(n=0;n<6;n++)
 {
	printf("init chan%d ADC0 ext...", n);
	adc_init_ext_signal(n, 0, src_data[0]);
	printf("done\n\r");

	printf("init chan%d ADC1 ext...", n);
	adc_init_ext_signal(n, 1, src_data[0]);
	printf("done\n\r");
 }

	printf("Read all reg ADC0...\r\n");
        printf("0x%02X = 0x%02X\n\r", 0x00, adc_read(curmodule, channum, 0x00));
        printf("0x%02X = 0x%02X\n\r", 0x5F, adc_read(curmodule, channum, 0x5F));
        printf("0x%02X = 0x%02X\n\r", 0x0B, adc_read(curmodule, channum, 0x0B));
        printf("0x%02X = 0x%02X\n\r", 0x5E, adc_read(curmodule, channum, 0x5E));
        printf("0x%02X = 0x%02X\n\r", 0xEE, adc_read(curmodule, channum, 0xEE));
        printf("0x%02X = 0x%02X\n\r", 0xEF, adc_read(curmodule, channum, 0xEF));
        printf("0x%02X = 0x%02X\n\r", 0x21, adc_read(curmodule, channum, 0x21));
        printf("0x%02X = 0x%02X\n\r", 0x14, adc_read(curmodule, channum, 0x14));
        printf("0x%02X = 0x%02X\n\r", 0x6E, adc_read(curmodule, channum, 0x6E));
        printf("0x%02X = 0x%02X\n\r", 0x73, adc_read(curmodule, channum, 0x73));
        printf("0x%02X = 0x%02X\n\r", 0x0D, adc_read(curmodule, channum, 0x0D));
        printf("0x%02X = 0x%02X\n\r", 0xFF, adc_read(curmodule, channum, 0xFF));
        printf("0x%02X = 0x%02X\n\r", 0x5F, adc_read(curmodule, channum, 0x5F));
        printf("0x%02X = 0x%02X\n\r", 0xF3, adc_read(curmodule, channum, 0xF3));
        printf("0x%02X = 0x%02X\n\r", 0xEE, adc_read(curmodule, channum, 0xEE));
        printf("0x%02X = 0x%02X\n\r", 0xEF, adc_read(curmodule, channum, 0xEF));
        printf("0x%02X = 0x%02X\n\r", 0xEE, adc_read(curmodule, channum, 0xEE));
        printf("0x%02X = 0x%02X\n\r", 0xEF, adc_read(curmodule, channum, 0xEF));
        printf("0x%02X = 0x%02X\n\r", 0xEE, adc_read(curmodule, channum, 0xEE));
        printf("0x%02X = 0x%02X\n\r", 0xEF, adc_read(curmodule, channum, 0xEF));
        printf("0x%02X = 0x%02X\n\r", 0xEE, adc_read(curmodule, channum, 0xEE));
        printf("0x%02X = 0x%02X\n\r", 0xEF, adc_read(curmodule, channum, 0xEF));
        printf("0x%02X = 0x%02X\n\r", 0xEE, adc_read(curmodule, channum, 0xEE));
        printf("0x%02X = 0x%02X\n\r", 0xEF, adc_read(curmodule, channum, 0xEF));
        printf("0x%02X = 0x%02X\n\r", 0xEE, adc_read(curmodule, channum, 0xEE));
        printf("0x%02X = 0x%02X\n\r", 0xEF, adc_read(curmodule, channum, 0xEF));
        printf("0x%02X = 0x%02X\n\r", 0xEE, adc_read(curmodule, channum, 0xEE));
        printf("0x%02X = 0x%02X\n\r", 0xEF, adc_read(curmodule, channum, 0xEF));
*/
	TIMER_1_CLOCK_init();

	gpio_set_pin_direction(PA08, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(PA08, false);
	gpio_set_pin_pull_mode(PA08, GPIO_PULL_UP);//GPIO_PULL_OFF);
	gpio_set_pin_function(PA08, GPIO_PIN_FUNCTION_E);
	gpio_set_pin_direction(PA09, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(PA09, false);
	gpio_set_pin_pull_mode(PA09, GPIO_PULL_UP);//GPIO_PULL_OFF);
	gpio_set_pin_function(PA09, GPIO_PIN_FUNCTION_E);

	TIMER_1_init();

	TIMER_0_CLOCK_init();

	TIMER_0_init();

	USB_0_init();

	ETHERNET_MAC_0_init();
}
