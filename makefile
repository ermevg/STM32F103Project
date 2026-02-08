# makefile for STM32F103C8Tx

MCU = cortex-m3
ARCH = arm-none-eabi

CC = $(ARCH)-gcc
OBJCOPY = $(ARCH)-objcopy
SIZE = $(ARCH)-size

# Флаги компиляции: добавляем -ffunction-sections для оптимизации
CFLAGS = -mcpu=$(MCU) -mthumb -Wall -g -O0 -ffunction-sections -fdata-sections
ASFLAGS = -mcpu=$(MCU) -mthumb -g -c

# Флаги линковки: 
# 1. --specs=nosys.specs убирает ошибки _write, _read, _sbrk
# 2. -Wl,--gc-sections удаляет неиспользуемый код
LDFLAGS = -mcpu=$(MCU) -mthumb -T STM32F103C8TX_FLASH.ld --specs=nosys.specs -Wl,-Map=firmware.map -Wl,--gc-sections

C_SRC = main.c
S_SRC = Startup/startup_stm32f103c8tx.s
OBJ = main.o Startup/startup_stm32f103c8tx.o

all: firmware.bin
	@$(SIZE) firmware.elf

# Собираем ASM через GCC! Это важно для Thumb
Startup/startup_stm32f103c8tx.o: Startup/startup_stm32f103c8tx.s
	$(CC) $(ASFLAGS) -o $@ $<

main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

firmware.elf: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

firmware.bin: firmware.elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -f *.o Startup/*.o *.elf *.bin *.map

.PHONY: all clean