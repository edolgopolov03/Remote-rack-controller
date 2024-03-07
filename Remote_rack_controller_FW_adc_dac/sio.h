#ifndef SIO_H
#define SIO_H

#ifdef __cplusplus
extern "C" {
	#endif

#define TBUF_SIZE	16	// Must be one of these powers of 2 (2,4,8,16,32,64,128)
#define RBUF_SIZE	512	// Must be one of these powers of 2 (2,4,8,16,32,64,128)

#define V_START		0xAA
#define V_STOP		0xAB
#define V_SHIFT		0xAC

void InitVolkov(void);
void RX_int(unsigned char temp);
void TX_int(void);
int ifchar(void);
unsigned char com_getchar (void);
unsigned long com_rbuflen (void);
unsigned long com_tbuflen (void);
unsigned char com_read_ready (void);
void com_send (unsigned char *dat, unsigned long size);
int com_preparebuf(unsigned char *buf_for_send, unsigned long maxsize, unsigned char *curbuf, unsigned long cursize);
int com_read (unsigned char *dat, unsigned long size);
void init(struct io_descriptor *const io_descr);
void put_char(unsigned char dat);
void wait_end_of_send(void);
void  BeginSend(void);
void FinishSend(void);
unsigned long get_t_in(void);
int com_putchar(unsigned char c);
int ifchar(void);
unsigned char com_getchar(void);

#ifdef __cplusplus
}
#endif

#endif /* SIO_H */
