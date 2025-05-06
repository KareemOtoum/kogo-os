
SRC_DIR := src
BUILD_DIR := build
BOOT_DIR := $(BUILD_DIR)/boot
ISO_DIR := $(BUILD_DIR)/iso

AS := i686-elf-as
CXX := i686-elf-g++
LD := i686-elf-g++
GRUB_MKRESCUE := grub-mkrescue

# Find the path to the libstdc++ static library
LIBSTDCXX_PATH := $(shell $(CXX) -print-file-name=libstdc++.a)
LIBGCC_PATH := $(shell $(CXX) -print-file-name=libgcc.a)

ASFLAGS :=
								# -02 was acting up
CXXFLAGS := -Iinclude -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti

LDFLAGS := -T $(SRC_DIR)/linker.ld -ffreestanding -O2 -nostdlib
LIBS := $(LIBSTDCXX_PATH) $(LIBGCC_PATH)

ASM_SRC := $(shell find $(SRC_DIR) -name '*.asm')
CPP_SRC := $(shell find $(SRC_DIR) -name '*.cpp')

ASM_OBJS := $(patsubst $(SRC_DIR)/%.asm, $(BUILD_DIR)/%.o, $(ASM_SRC))
CPP_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CPP_SRC))

KERNEL_BIN := $(BUILD_DIR)/kernel.bin
ISO := $(BUILD_DIR)/os.iso

.PHONY: all clean run

all: $(ISO)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(KERNEL_BIN): $(ASM_OBJS) $(CPP_OBJS)
	@echo "Linking with libstdc++ at: $(LIBSTDCXX_PATH)"
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

$(ISO): $(KERNEL_BIN)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL_BIN) $(ISO_DIR)/boot/kernel.bin
	cp $(SRC_DIR)/grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	$(GRUB_MKRESCUE) -o $@ $(ISO_DIR)

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO) -d int

clean:
	rm -rf $(BUILD_DIR)
