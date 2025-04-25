SRC_DIR := src
BUILD_DIR := build
BOOT_DIR := $(BUILD_DIR)/boot
ISO_DIR := $(BUILD_DIR)/iso

# Toolchain
AS := i686-elf-as
CXX := i686-elf-g++
LD := i686-elf-g++
GRUB_MKRESCUE := grub-mkrescue

# Flags
ASFLAGS :=
CXXFLAGS := -Iinclude -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
LDFLAGS := -T $(SRC_DIR)/linker.ld -ffreestanding -O2 -nostdlib -lgcc

# Files
ASM_SRC := $(SRC_DIR)/boot.asm
CPP_SRC := $(wildcard $(SRC_DIR)/*.cpp)
ASM_OBJ := $(BUILD_DIR)/boot.o
CPP_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CPP_SRC))

KERNEL_BIN := $(BUILD_DIR)/kernel.bin
ISO := $(BUILD_DIR)/os.iso

.PHONY: all clean run

all: $(ISO)

# Compile assembly source
$(ASM_OBJ): $(ASM_SRC)
	@mkdir -p $(BUILD_DIR)
	$(AS) $(ASFLAGS) -o $@ $<

# Compile C++ sources
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Link kernel binary
$(KERNEL_BIN): $(ASM_OBJ) $(CPP_OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

# Create ISO with GRUB
$(ISO): $(KERNEL_BIN)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL_BIN) $(ISO_DIR)/boot/kernel.bin
	cp $(SRC_DIR)/grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	$(GRUB_MKRESCUE) -o $@ $(ISO_DIR)

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

clean:
	rm -rf $(BUILD_DIR)
