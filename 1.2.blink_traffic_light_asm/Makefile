# Tools
CC = riscv64-elf-gcc
OBJCOPY = riscv64-elf-objcopy
FLASH = wlink

# Flags
ARCH = rv32emc
ABI = ilp32e
CFLAGS = -march=$(ARCH) -mabi=$(ABI) -nostdlib
CFLAGS += -Iinclude -Os -Wall -Wextra
LDFLAGS = -T lib/ch32v003f4p6.ld

# Files and directories
TARGET = firmware
SRC_DIR = src
BUILD_DIR = build

# Output files
ELF = $(BUILD_DIR)/$(TARGET).elf
BIN = $(BUILD_DIR)/$(TARGET).bin

# Default target
all: $(BIN)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build firmware from assembly file (изменено с .c на .S)
$(BIN): $(SRC_DIR)/main.S | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(ELF) $<
	$(OBJCOPY) -O binary $(ELF) $@

# Flash to device
flash: $(BIN)
	$(FLASH) flash $< --address 0x08000000

# Clean build artifacts and remove build directory
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all flash clean