#include "printf.h"

#define w32(addr, value) *((volatile unsigned int *) addr) = value
#define r32(addr) *((volatile unsigned int *) addr)

int (*__read_char__)(void);
void (*__write_char__)(char c);

void uart_putc (const char c);

void set_read_char(int (*func)(void)) { __read_char__ = func; }
void set_write_char(void (*func)(char)) { __write_char__ = func; }

void uart_init (void) {
    w32(0xFE215004, 1);
    w32(0xFE215044, 0);
    w32(0xFE215060, 0);
    w32(0xFE21504c, 3);
    w32(0xFE215050, 0);
    w32(0xFE215044, 0);
    w32(0xFE215048, 0xC6);
    w32(0xFE215068, ((200000000/(115200*8))-1));
    w32(0xFE200004, ((2 << 12) | (2 << 15)));
    w32(0xFE215060, 3);

    // Register UART Tx function
    set_write_char(uart_putc);

}

void uart_putc (const char c) {
    while (!(r32(0xFE215054) & 0x20));
    w32(0xFE215040, (unsigned int) c);
}

void main()
{
    uart_init();
    printf("Kernel booted.\r\n");
    printf("Version: 0.0.1\r\n");
    printf("Author: Silas Müller\r\n");
    while(1);
}
