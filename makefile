# makefile for STM32F103C8Tx

MCU = cortex-m3
ARCH = arm-none-eabi

CC = $(ARCH)-gcc
OBJCOPY = $(ARCH)-objcopy
SIZE = $(ARCH)-size

BUILD_DIR = build
OBJ_DIR = obj

OPT = -O0
DEBUG_FLAGS = -g

INC_DIRS = $(addprefix -I, . sys_init drivers)

# Флаги компиляции: добавляем -ffunction-sections для оптимизации
CFLAGS = -mcpu=$(MCU) -mthumb -Wall $(DEBUG_FLAGS) $(OPT) -ffunction-sections -fdata-sections -MMD -MP
CFLAGS +=$(INC_DIRS)
ASFLAGS = -mcpu=$(MCU) -mthumb $(DEBUG_FLAGS) -c

#Флаги прошивки для OpenOCD
INTERFACE = interface/stlink.cfg
TARGET_CFG = target/stm32f1x.cfg
BINARY = $(BUILD_DIR)/firmware.bin

# Флаги линковки: 
# 1. --specs=nosys.specs убирает ошибки _write, _read, _sbrk
# 2. -Wl,--gc-sections удаляет неиспользуемый код
LDFLAGS = -mcpu=$(MCU) -mthumb -T STM32F103C8TX_FLASH.ld --specs=nosys.specs -Wl,-Map=$(BUILD_DIR)/firmware.map -Wl,--gc-sections

C_SRC := $(wildcard *.c ) $(wildcard sys_init/*.c ) $(wildcard drivers/*.c )
S_SRC := Startup/startup_stm32f103c8tx.s
OBJ = $(addprefix $(OBJ_DIR)/, $(notdir $(C_SRC:.c=.o)))
OBJ += $(addprefix $(OBJ_DIR)/, $(notdir $(S_SRC:.s=.o)))

VPATH = $(dir $(C_SRC) $(S_SRC))

all: debug

debug: clean $(BUILD_DIR)/firmware.bin
	@$(SIZE) $(BUILD_DIR)/firmware.elf
	@echo "Build completed!(DEBUG MODE)!"

release: OPT = -Os
release: DEBUG_FLAGS =
release: clean $(BUILD_DIR)/firmware.bin
	@$(SIZE) $(BUILD_DIR)/firmware.elf
	@echo "Build completed!(RELEASE MODE)"

# Собираем ASM через GCC! Это важно для Thumb
$(OBJ_DIR)/%.o: %.s | $(OBJ_DIR) # - | Важно чтобы make не проверял время каталога
	$(CC) $(ASFLAGS) -o $@ $<

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/firmware.elf: $(OBJ) | $(BUILD_DIR)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf
	$(OBJCOPY) -O binary $< $@

$(OBJ_DIR) $(BUILD_DIR):
	mkdir -p $@

flash: release
	openocd -f $(INTERFACE) -f $(TARGET_CFG) \
	-c "program $(BINARY) verify reset exit 0x08000000"

clean:
	@rm -rf $(OBJ_DIR) $(BUILD_DIR)
	@echo "Delete successful"

.PHONY: all clean release debug

-include $(wildcard $(OBJ_DIR)/*.d)