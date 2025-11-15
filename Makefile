include Makefile.conf

BUILD_DIR := build
ELF := $(BUILD_DIR)/kernel.elf
KERNEL := $(BUILD_DIR)/image

SRCDIRS := kernel drivers mm fs lib fdt

SRC := $(shell find $(SRCDIRS) -type f \( -name "*.c" -o -name "*.S" \))
OBJ := $(patsubst %.c, $(BUILD_DIR)/%.o, $(filter %.c,$(SRC))) $(patsubst %.S, $(BUILD_DIR)/%.o, $(filter %.S,$(SRC)))

all: $(KERNEL)

$(KERNEL): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(ELF): $(OBJ)
	$(LD) -T linker.ld -o $@ $^

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -o $@ $<

-include $(OBJ:.o=.d)

qemu:
	$(QEMU) -M virt -bios default -kernel $(KERNEL) -smp $(HART_NUM) -m 128M -nographic

qemugdb:
	$(QEMU) -M virt -bios default -kernel $(KERNEL) -smp $(HART_NUM) -m 128M -nographic -s -S

CSCOPEDIR := $(SRCDIRS) include

tags: cscope 
	$(CTAGS) -L cscope.files

cscope:
	find $(CSCOPEDIR) -type f -name *.[chSs] > cscope.files
	$(CSCOPE) -q -R -k -b -i cscope.files

clean-tags:
	rm -rf tags cscope.*

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all qemu qemugdb tags cscope clean
