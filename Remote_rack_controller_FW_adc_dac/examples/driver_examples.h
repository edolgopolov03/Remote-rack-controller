/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef DRIVER_EXAMPLES_H_INCLUDED
#define DRIVER_EXAMPLES_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

void adc_reset(unsigned char nADC, unsigned short addr, unsigned char data);
void adc_write(unsigned char nADC, unsigned char chan, unsigned short addr, unsigned char data);
unsigned char adc_read(unsigned char nADC, unsigned char chan, unsigned short addr);
void adc_init_ramp(unsigned char nADC, unsigned char chan);
void adc_init_ext_signal(unsigned char nADC, unsigned char chan, unsigned char inversion_flag);

void ADC_0_example(void);

void DAC_0_example(void);

void USART_0_example(void);

void USART_1_example(void);

#ifdef __cplusplus
}
#endif
#endif // DRIVER_EXAMPLES_H_INCLUDED