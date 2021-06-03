#include <avr/io.h>
#include "avr_compiler.h"
#define USART USARTC0
/*
+------------------------------------------------------------------------------
| Function    : uart_putc
+------------------------------------------------------------------------------
| Description : send the data 
|				
| Parameters  :  DATA to send  (HEX)
| Returns     : 
|
+------------------------------------------------------------------------------
*/
void uart_putc(unsigned char c)
{
    if(c == '\n')
        uart_putc('\r');

    /* wait until transmit buffer is empty */
    while(!(USART.STATUS & USART_DREIF_bm));

    /* send next byte */
    USART.DATA = c;
}

/*
+------------------------------------------------------------------------------
| Function    : uart_putc_hex
+------------------------------------------------------------------------------
| Description : convert the data to ASCII(2 Bytes) and send it 
|				0xf1 -->> 'F'+'1'
| Parameters  :  DATA to send  (8 bits HEX)
| Returns     : 
|
+------------------------------------------------------------------------------
*/
void uart_putc_hex(unsigned char b)
{
    /* upper nibble */
    if((b >> 4) < 0x0a)
        uart_putc((b >> 4) + '0');
    else
        uart_putc((b >> 4) - 0x0a + 'a');

    /* lower nibble */
    if((b & 0x0f) < 0x0a)
        uart_putc((b & 0x0f) + '0');
    else
        uart_putc((b & 0x0f) - 0x0a + 'a');
}

/*
+------------------------------------------------------------------------------
| Function    : uart_putw_hex
+------------------------------------------------------------------------------
| Description : convert the data to ASCII(4 Bytes) and send it 
|				0xf1e0 -->> 'f'+'1'+'e'+'0'
| Parameters  :  DATA to send  (16 bits HEX)
| Returns     : 
|
+------------------------------------------------------------------------------
*/
void uart_putw_hex(unsigned int w)
{
    uart_putc_hex((unsigned char) (w >> 8));
    uart_putc_hex((unsigned char) (w & 0xff));
}

/*
+------------------------------------------------------------------------------
| Function    : uart_putdw_hex
+------------------------------------------------------------------------------
| Description : convert the data to ASCII(2 Bytes) and send it 
|				0xf1e01234 -->> 'f'+'1'+'e'+'0'+'1'+'2'+'3'+'4'
| Parameters  :  DATA to send  (32 bits HEX)
| Returns     : 
|
+------------------------------------------------------------------------------
*/
void uart_putdw_hex(unsigned long dw)
{
    uart_putw_hex((unsigned int) (dw >> 16));
    uart_putw_hex((unsigned int) (dw & 0xffff));
}

/*
+------------------------------------------------------------------------------
| Function    : uart_putw_dec
+------------------------------------------------------------------------------
| Description : convert the data to decimal numbers and send it 
|				0x123 -->> '2'+'9'+'1'
| Parameters  :  DATA to send  (16 bits HEX)
| Returns     : 
|
+------------------------------------------------------------------------------
*/
void uart_putw_dec(unsigned int w)
{
    unsigned int num = 10000;
    unsigned char started = 0;

    while(num > 0)
    {
        unsigned char b = w / num;
        if(b > 0 || started || num == 1)
        {
            uart_putc('0' + b);
            started = 1;
        }
        w -= b * num;

        num /= 10;
    }
}

/*
+------------------------------------------------------------------------------
| Function    : uart_putdw_dec
+------------------------------------------------------------------------------
| Description : convert the data to decimal numbers and send it 
|				0x123 -->> '2'+'9'+'1'
| Parameters  :  DATA to send  (32 bits HEX)
| Returns     : 
|
+------------------------------------------------------------------------------
*/
void uart_putdw_dec(unsigned long dw)
{
    unsigned long num = 1000000000;
    unsigned char started = 0;

    while(num > 0)
    {
        unsigned char b = dw / num;
        if(b > 0 || started || num == 1)
        {
            uart_putc('0' + b);
            started = 1;
        }
        dw -= b * num;

        num /= 10;
    }
}

/*
+------------------------------------------------------------------------------
| Function    : uart_puts
+------------------------------------------------------------------------------
| Description : send string
|				
| Parameters  : string to send
| Returns     : 
|
+------------------------------------------------------------------------------
*/
void uart_puts(const char* str)
{
    while(*str)
        uart_putc(*str++);
}

/*
+------------------------------------------------------------------------------
| Function    : uart_getc
+------------------------------------------------------------------------------
| Description : receive one byte 
|				
| Parameters  : 
| Returns     : data
|
+------------------------------------------------------------------------------
*/
unsigned char uart_getc(void)
{
    /* wait until receive buffer is full */
    while(!(USART.STATUS & USART_RXCIF_bm));

    unsigned char b = USART.DATA;
    if(b == '\r')
        b = '\n';
		
    return b;
}
//+------------------------------------------------------------------------------


