#ifndef UART_H_
#define UART_H_

void init_uart(void);
void uart_putc (unsigned char c);
unsigned char uart_getc(void);

extern unsigned char (*__read_char__)(void); // not sure what extern means
extern void (*__write_char__)(unsigned char); // ??

#endif
