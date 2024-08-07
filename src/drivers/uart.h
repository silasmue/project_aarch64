#ifndef UART_H_
#define UART_H_

void init_uart(void);
void uart_putc (const char c);

extern int (*__read_char__)(void);
extern void (*__write_char__)(char);

#endif
