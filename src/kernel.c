#include "printf.h"
#include "drivers/uart.h"

void main()
{
  init_uart();
  printf("Kernel booted.\r\n");
  printf("Version: 0.0.1\r\n");
  printf("Author: Silas Müller\r\n");
  int b[50] = {0};
  while(1) {
    uart_putc(uart_getc());     
  };
}
