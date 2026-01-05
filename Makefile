# Makefile for kacchiOS
# Changed to use i686 cross-compiler tools for 32-bit compilation on ARM64 system
CC = i686-linux-gnu-gcc
LD = i686-linux-gnu-ld
AS = i686-linux-gnu-as
# Original: CC = gcc
# Original: LD = ld
# Original: AS = as

# Removed -m32 flag as it's not supported; using cross-compiler instead
CFLAGS = -ffreestanding -O2 -Wall -Wextra -nostdinc \
         -fno-builtin -fno-stack-protector -I.
# Original: CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -nostdinc \
#          -fno-builtin -fno-stack-protector -I.
ASFLAGS = 
# Original: ASFLAGS = --32
LDFLAGS = -m elf_i386

OBJS = boot.o kernel.o serial.o string.o memory.o process.o scheduler.o ctxsw.o

all: kernel.elf

kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) -T link.ld -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(AS) $(ASFLAGS) $< -o $@

run: kernel.elf
	qemu-system-i386 -kernel kernel.elf -m 64M -serial stdio -display none

run-vga: kernel.elf
	qemu-system-i386 -kernel kernel.elf -m 64M -serial mon:stdio

debug: kernel.elf
	qemu-system-i386 -kernel kernel.elf -m 64M -serial stdio -display none -s -S &
	@echo "Waiting for GDB connection on port 1234..."
	@echo "In another terminal run: gdb -ex 'target remote localhost:1234' -ex 'symbol-file kernel.elf'"

clean:
	rm -f *.o kernel.elf

.PHONY: all run run-vga debug clean