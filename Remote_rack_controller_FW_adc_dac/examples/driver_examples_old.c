/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"

static uint8_t src_data[512];
static uint8_t chk_data[512];
/**
 * Example of using FLASH_0 to read and write Flash main array.
 */
void FLASH_0_example(void)
{
	uint32_t page_size;
	uint16_t i;

	/* Init source data */
	page_size = flash_get_page_size(&FLASH_0);
	
	for (i = 0; i < page_size; i++) {
		src_data[i] = i;
	}

	/* Write data to flash */
	flash_write(&FLASH_0, 0x3200, src_data, page_size);

	/* Read data from flash */
	flash_read(&FLASH_0, 0x3200, chk_data, page_size);
}

/**
 * Example task of using SPI_0 to echo using the IO abstraction.
 */
void SPI_0_example_task(void *p)
{
	struct io_descriptor *io;
	uint16_t              data;

	(void)p;

	spi_m_os_get_io_descriptor(&SPI_0, &io);

	for (;;) {
		if (io->read(io, (uint8_t *)&data, 2) == 2) {
			/* read OK, handle data. */;
		} else {
			/* error. */;
		}
	}
}

/**
 * Example task of using SPI_1 to echo using the IO abstraction.
 */
void SPI_1_example_task(void *p)
{
	struct io_descriptor *io;
	uint16_t              data;

	(void)p;

	spi_m_os_get_io_descriptor(&SPI_1, &io);

	for (;;) {
		if (io->read(io, (uint8_t *)&data, 2) == 2) {
			/* read OK, handle data. */;
		} else {
			/* error. */;
		}
	}
}

/**
 * Example task of using SPI_2 to echo using the IO abstraction.
 */
void SPI_2_example_task(void *p)
{
	struct io_descriptor *io;
	uint16_t              data;

	(void)p;

	spi_m_os_get_io_descriptor(&SPI_2, &io);

	for (;;) {
		if (io->read(io, (uint8_t *)&data, 2) == 2) {
			/* read OK, handle data. */;
		} else {
			/* error. */;
		}
	}
}

/**
 * Example task of using SPI_3 to echo using the IO abstraction.
 */
void SPI_3_example_task(void *p)
{
	struct io_descriptor *io;
	uint16_t              data;

	(void)p;

	spi_m_os_get_io_descriptor(&SPI_3, &io);

	for (;;) {
		if (io->read(io, (uint8_t *)&data, 2) == 2) {
			/* read OK, handle data. */;
		} else {
			/* error. */;
		}
	}
}

/**
 * Example task of using SPI_4 to echo using the IO abstraction.
 */
void SPI_4_example_task(void *p)
{
	struct io_descriptor *io;
	uint16_t              data;

	(void)p;

	spi_m_os_get_io_descriptor(&SPI_4, &io);

	for (;;) {
		if (io->read(io, (uint8_t *)&data, 2) == 2) {
			/* read OK, handle data. */;
		} else {
			/* error. */;
		}
	}
}

/**
 * Example task of using SPI_5 to echo using the IO abstraction.
 */
void SPI_5_example_task(void *p)
{
	struct io_descriptor *io;
	uint16_t              data;

	(void)p;

	spi_m_os_get_io_descriptor(&SPI_5, &io);

	for (;;) {
		if (io->read(io, (uint8_t *)&data, 2) == 2) {
			/* read OK, handle data. */;
		} else {
			/* error. */;
		}
	}
}

/**
 * Example task of using USART_0 to echo using the IO abstraction.
 */
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
