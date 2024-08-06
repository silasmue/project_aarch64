# Program and learn 64-bit ARM Assembly (AARCH64)
This is a short summary of [Low Level Learning's](https://www.youtube.com/watch?v=8NdrdxkBP3U) video.

Hello World assembly:
```
# AARCH64 

.global _start
.section .text

_start:
  # Write to console
  mov x8, #64
  mov x0, #1
  ldr x1, =message
  mov x2, #13
  svc 0

  # exit savely
  mov x8, #0x5d
  mov x0, #0x41
  svc 0


.section .data
  message:
    .ascii "Hello World\n"
```
We can compile and execute this example with:
```
$ aarch64-linux-gnu-as -c example.S -o example.o
$ aarch64-linux-gnu-gcc example.o -o example.elf -nostdlib -static
$ qemu-aarch64 ./example.elf
```
We can also use our self compiled cross compiler for that task if we want.

## Systemcalls in AARCH64
To invoke a syscall we search in the table for the systemcall and load the given values into the specific registers. In the example above to to exit savely we need to `mov x8, #0x5d`, this moves `#0x5d` to `x8`. According to the table `#0x5d` is the number of the system call to exit a programm. The value we load to `x0` is the argument of the systemcall in this case it is the error code (we can load any value here).
The other example is a bit more complex but basically the same, we just need to load (`ldr x1, message`) the message to the specified register and move the length of the string (13bytes) to `x2`.

### AARCH64 syscall table
https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md#arm64-64_bit
# Source
https://www.youtube.com/watch?v=8NdrdxkBP3U
