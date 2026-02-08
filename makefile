# makefile for STM32F103C8Tx

MCU = cortex-m3
ARCH = arm-none-eabi

CC = $(ARCH)-gcc
AS = $(ARCH)-as
LD = $(ARCH)-ld
OBJCOPY = $(ARCH)-objcopy
OBJDUMP = $(ARCH)-objdump

CFLAGS = -mcpu=$(MCU) -mthumb -nostdlib -Wall -Wextra -g
LDFLAGS = -T STM32F103C8TX_FLASH.ld -nostdlib -Wl, -Map=firmware.map

C_SRC = main.c
S_SRC = Startup/startup_stm32f103c8tx.s

OBJ = $(C_SRC:.c=.o) $(S_SRC:.s=.o)

all: firmware.bin

%.o: %.s
	$(AS) $(CFLAGS) -o $@ $<


%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

firmware.elf: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

firmware.bin: firmware.elf
	$(OBJCOPY) -O binary $< $@


firmware.hex: firmware.elf
	$(OBJCOPY) -O ihex $< $@

clean:
	rm -f *.o *.elf *.bin *.hex firmware.map

.PHONY: all clean
