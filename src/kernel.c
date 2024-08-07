#include "printf.h"
#include "drivers/uart.h"

void main()
{
    init_uart();
    printf("Kernel booted.\r\n");
    printf("Version: 0.0.1\r\n");
    printf("Author: Silas Müller\r\n");
    while(1);
}
