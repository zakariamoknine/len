include Makefile.conf

OUTDIR := out
ELF    := $(OUTDIR)/kernel.elf
KERNEL := $(OUTDIR)/kernel.img

SRC := $(shell find kernel -type f \( -name "*.c" -o -name "*.S" \))

OBJ := $(patsubst %.c, $(OUTDIR)/%.o, $(SRC:.S=.c))
OBJ := $(patsubst %.c.o, %.o, $(OBJ))

all: $(KERNEL)

$(KERNEL): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(ELF): $(OBJ)
	$(LD) -T kernel.ld -o $@ $^

$(OUTDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -o $@ $<

$(OUTDIR)/%.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -o $@ $<

qemu:
	$(QEMU) -machine virt -bios none -kernel $(KERNEL) -smp $(NHART) -m 128M -nographic

clean:
	rm -rf $(OUTDIR)

.PHONY: all clean
