# Makefile
# needs aarch64-elf-gcc, should also work with aarch64-linux-gnu-gcc

PREFIX = aarch64-elf-
FLAGS = -Wall -nostdlib -nostartfiles -ffreestanding -g
INCLUDES = -Ilib
CC = $(PREFIX)gcc $(INCLUDES) $(FLAGS)

LIBSRC = $(wildcard lib/*.c)
LIB_O = target/$(LIBSRC:%.c=%.o)


all: img

target:
	mkdir  -p target

target/lib: target
	mkdir -p target/lib

start.o: src/start.S target
	@echo "Compiling start.S"
	$(CC) -c src/start.S -o target/start.o

kernel.o: src/kernel.c target
	@echo "Compiling kernel.c"
	$(CC) -c src/kernel.c -o target/kernel.o

lib: target/lib
	@echo "Compiling libraries"
	$(CC) -c $(LIBSRC) -o $(LIB_O)

linking: start.o kernel.o lib
	@echo "Linking..."
	$(PREFIX)ld -g -m aarch64elf -nostdlib -T linker.ld target/start.o target/kernel.o $(LIB_O) -o target/kernel8.elf

img: linking
	@echo "Making the image"
	$(PREFIX)objcopy target/kernel8.elf -O binary kernel8.img
	@echo "Generated: kernel8.img"

qemu: img
	@echo "Starting a QEMU process. Be aware that you will need QEMU version 9+"
	@echo "To stop QEMU, use another terminal and search the PID, for example with HTOP and kill the process using kill <pid>"
	qemu-system-aarch64 -machine raspi4b -serial none -serial mon:stdio -display none -kernel kernel8.img

clean:
	rm -rf target
	rm -f kernel8.img
