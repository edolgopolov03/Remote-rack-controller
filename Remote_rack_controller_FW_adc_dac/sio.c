#include <hal_io.h>
#include <utils_assert.h>
#include "sio.h"

struct io_descriptor *vio;

unsigned char AddressPLC = 0;
unsigned char echo_flag = 1;
unsigned char ma_sl_flag = 1;

static unsigned char volatile tbuf[TBUF_SIZE];
static unsigned char volatile rbuf[RBUF_SIZE];

unsigned long t_in=0;
unsigned long t_out=0;

unsigned long r_in=0;
unsigned long r_out=0;

unsigned char ti_restart=0;	// NZ if TI=1 is required

unsigned char START_flag=0;
unsigned char STOP_echo_flag=0;
unsigned char STOP_flag=0;

unsigned int count=0;

void InitVolkov(void) { ti_restart=1; }

void RX_int(unsigned char temp)
{
//  unsigned char temp;
//  RecvByte(&temp);
  if ((ma_sl_flag == 0) && (echo_flag == 1) && (ti_restart == 1)) com_putchar(temp);//SendByte(temp);
  if (temp == V_START)
  {
     r_in=0; 
     r_out=0; 
     START_flag=1; 
     STOP_echo_flag=0;
     STOP_flag=0;
   }
   if (START_flag == 1)		// пока старт не придёт, ничего не делать 
   {
     if (((r_in-r_out) & ~(RBUF_SIZE-1)) == 0)
     {
       rbuf[r_in & (RBUF_SIZE-1)]=temp;	//укладываем байты
       r_in++;
     }
     if (temp == V_STOP)		// уложенный байт мог оказаться последним
     {
       if ((ma_sl_flag==1) && (echo_flag==1)) //если контроллер в режиме мастера
       {
         if (STOP_echo_flag == 1) 
         {
           START_flag=0;
           STOP_echo_flag=0;
           STOP_flag=1;
         }
         else
         {
           r_in=0;
           r_out=0;
           STOP_echo_flag=1;
         }
      } 
      else			// если режим работы без эхо */
      {
        START_flag=0;		// просто конец приёма
        STOP_flag=1;
      } 
    }
  } 
}

void real_send(void)
{
//	SendByte(tbuf[t_out & (TBUF_SIZE-1)]);
	t_out++;
}

void TX_int(void)
{
  if (t_in != t_out) real_send();
  else
  { 
    t_in=0;			// empty transmit buffer
    t_out=0;
    ti_restart=1;
  }  
} 

void put_char(unsigned char dat)
{
  if ((dat == V_START) || (dat == V_STOP) || (dat == V_SHIFT))
  {
    while(com_putchar(V_SHIFT)!=0);
    while(com_putchar(dat-V_START)!=0);
  }
  else 
  {
    while(com_putchar(dat)!=0);
  }
}

void  BeginSend(void)
{
  ti_restart=0;
  t_in=0;			// empty transmit buffer
  t_out=0;
  START_flag=0;
  STOP_echo_flag=0;
  STOP_flag=0;
  //Для серверной версии посылка идёт со стартом
  if (ma_sl_flag == 1)
  {
	 com_putchar(V_START);
  }
}

void FinishSend(void)
{
  com_putchar(V_STOP);	
}   

unsigned long get_t_in(void) {return(t_in);}

void com_send(unsigned char *curbuf, unsigned long cursize)
{
  unsigned char chksum;
  unsigned long i;
  BeginSend();
  chksum=AddressPLC;
  put_char(chksum);
  for (i=0; i < cursize; i++)
  {
    chksum^=curbuf[i];
    put_char(curbuf[i]);
  }
  put_char(chksum);
  FinishSend();
}

int com_putchar_pk(unsigned char *buf_for_send, unsigned long maxsize, unsigned char c)
{
 buf_for_send[t_in]=c;	t_in++;
 if (t_in >= maxsize)  return(-1);
 return 0; 
}

void put_char_pk(unsigned char *buf_for_send, unsigned long maxsize, unsigned char dat)
{
  if ((dat == V_START) || (dat == V_STOP) || (dat == V_SHIFT))
  {
    while(com_putchar_pk(buf_for_send, maxsize, V_SHIFT)!=0);
    while(com_putchar_pk(buf_for_send, maxsize, dat-V_START)!=0);
  }
  else 
  {
    while(com_putchar_pk(buf_for_send, maxsize, dat)!=0);
  }
}

void  BeginSend_pk(unsigned char *buf_for_send, unsigned long maxsize)
{
  ti_restart=0;
  t_in=0;			// empty transmit buffer
  t_out=0;
  START_flag=0;
  STOP_echo_flag=0;
  STOP_flag=0;
  //Для серверной версии посылка идёт со стартом
  if (ma_sl_flag == 1)
  {
	 com_putchar_pk(buf_for_send, maxsize, V_START);
  }
}

void FinishSend_pk(unsigned char *buf_for_send, unsigned long maxsize)
{
  com_putchar_pk(buf_for_send, maxsize, V_STOP);	
}   

int com_preparebuf(unsigned char *buf_for_send, unsigned long maxsize, unsigned char *curbuf, unsigned long cursize)
{
  unsigned char chksum;
  unsigned long i;
  BeginSend_pk(buf_for_send, maxsize);
  chksum=AddressPLC;
  put_char_pk(buf_for_send, maxsize, chksum);
  for (i=0; i < cursize; i++)
  {
    chksum^=curbuf[i];
    put_char_pk(buf_for_send, maxsize, curbuf[i]);
  }
  put_char_pk(buf_for_send, maxsize, chksum);
  FinishSend_pk(buf_for_send, maxsize);
  return(t_in);
}

int com_read(unsigned char *curbuf, unsigned long cursize)
{
//распаковка принятого массива
  unsigned char chksum=0, m;
  unsigned long  i, j, k;
  if (STOP_flag == 0) return -1;		// read is not complite
  STOP_flag=0;
  j=r_in;
  r_in=0;
  i=0;
  if (ma_sl_flag==0) if (rbuf[i++]!=V_START) return(-3);			// START not found
  if (rbuf[i]!=V_SHIFT) m=rbuf[i]; else m=rbuf[++i]+V_START;
  i++;
  if (m!=AddressPLC) return -4;		// illegal Address
  chksum=m;
  k=0;
  do
  {
    if (rbuf[i]!=V_SHIFT) m=rbuf[i]; else m=rbuf[++i]+V_START;
    i++;
    chksum^=m;
	if (k>cursize) return(-2);					// small output buffer
	if (k<cursize) { curbuf[k]=m; }
    k++;
  } while (i<(j-1));
  if (chksum == 0) k--; else return(-5);	// illegal checksum
  return(k);
}

void init(struct io_descriptor *const io_descr)
{
//	InitUart();
    vio=io_descr;
	t_in = 0;
	t_out = 0;
	r_in = 0;
	r_out = 0;
	START_flag = 0;
	STOP_flag = 0;
	ti_restart = 1;
//	TX_interrupt_enable();
}

unsigned char com_read_ready(void) { return STOP_flag; }
unsigned long com_tbuflen (void) { return (t_in-t_out); }
unsigned long com_rbuflen (void) { return (r_in-r_out); }

int com_putchar(unsigned char c)
{
 tbuf [t_in & (TBUF_SIZE-1)]=c;	t_in++;
 if (t_in - t_out >= TBUF_SIZE)  return(-1);
// if (t_in!=t_out) TX_interrupt_enable();
 return 0; 
}

int ifchar(void)
{
  if (com_rbuflen() == 0) return(-1); else return(0);	// buffer is empty
}	

unsigned char com_getchar(void) { return (rbuf[(r_out++) & (RBUF_SIZE-1)]); }
