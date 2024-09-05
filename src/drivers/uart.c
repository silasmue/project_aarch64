#include "uart.h"
#include "gpio.h"

#define w32(addr, value) *((volatile unsigned int *) addr) = value
#define r32(addr) *((volatile unsigned int *) addr)

unsigned char(*__read_char__)(void);
void(*__write_char__)(unsigned char);

void set_read_char(unsigned char(*func)(void)) { __read_char__ = func; }
void set_write_char(void (*func)(unsigned char)) { __write_char__ = func; }

//void init_uart(void) {
//    w32(0xFE215004, 1);
//    w32(0xFE215044, 0);
//    w32(0xFE215060, 0);
//    w32(0xFE21504c, 3);
//    w32(0xFE215050, 0);
//    w32(0xFE215044, 0);
//    w32(0xFE215048, 0xC6);
//    w32(0xFE215068, ((200000000/(115200*8))-1));
//    w32(0xFE2000:04, ((2 << 12) | (2 << 15)));
//    w32(0xFE215060, 3);

    // Register UART Tx function
//    set_write_char(uart_putc);
//    set_read_char(uart_getc);
//} //void uart_putc (const char c) { while (!(r32(0xFE215054) & 0x20));
//    w32(0xFE215040, (unsigned int) c);
//}

// A Mailbox message with set clock rate of PL011 to 3MHz tag
volatile unsigned int  __attribute__((aligned(16))) mbox[9] = {
    9*4, 0, 0x38002, 12, 8, 2, 3000000, 0 ,0
};

void init_uart(void) {
  // Disable UART0.
	mmio_write(UART0_CR, 0x00000000);
	// Setup the GPIO pin 14 && 15.

	// Disable pull up/down for all GPIO pins & delay for 150 cycles.
	mmio_write(GPPUD, 0x00000000);
	delay(150);

	// Disable pull up/down for pin 14,15 & delay for 150 cycles.
	mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
	delay(150);

	// Write 0 to GPPUDCLK0 to make it take effect.
	mmio_write(GPPUDCLK0, 0x00000000);

	// Clear pending interrupts.
	mmio_write(UART0_ICR, 0x7FF);

  // UART_CLOCK = 30000000;
	unsigned int r = (((unsigned int)(&mbox) & ~0xF) | 8);
	// wait until we can talk to the VC
	while ( mmio_read(MBOX_STATUS) & 0x80000000 ) { }
	// send our message to property channel and wait for the response
	mmio_write(MBOX_WRITE, r);
	while ( (mmio_read(MBOX_STATUS) & 0x40000000) || mmio_read(MBOX_READ) != r ) { }
  
  // Divider = 3000000 / (16 * 115200) = 1.627 = ~1.
	mmio_write(UART0_IBRD, 1);
	// Fractional part register = (.627 * 64) + 0.5 = 40.6 = ~40.
	mmio_write(UART0_FBRD, 40);

	// Enable FIFO & 8 bit data transmission (1 stop bit, no parity).
	mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

	// Mask all interrupts.
	mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
	                       (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));

	// Enable UART0, receive & transfer part of UART.
	mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
  // Register UART Tx
  set_write_char(uart_putc);

  // Register UART Rx
  set_read_char(uart_getc);
}


void uart_putc(unsigned char c) {
	// Wait for UART to become ready to transmit.
	while ( mmio_read(UART0_FR) & (1 << 5) ) { }
	mmio_write(UART0_DR, c);
}

unsigned char uart_getc() {
    // Wait for UART to have received something.
    while ( mmio_read(UART0_FR) & (1 << 4) ) { }
    return mmio_read(UART0_DR);
}

void uart_puts(const char* str) {
	for (size_t i = 0; str[i] != '\0'; i ++)
		uart_putc((unsigned char)str[i]);
}
    

