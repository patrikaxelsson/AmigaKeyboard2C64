#ifndef __UART_H__
#define __UART_H__

void uart_init(void);
int uart_putchar(char c, FILE *stream);

#endif
