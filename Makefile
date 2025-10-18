include Makefile.conf

OUTDIR := out
ELF := $(OUTDIR)/kernel.elf
KERNEL := $(OUTDIR)/kernel.img

SRCDIRS := kernel drivers mm fs lib fdt

SRC := $(shell find $(SRCDIRS) -type f \( -name "*.c" -o -name "*.S" \))
OBJ := $(patsubst %.c, $(OUTDIR)/%.o, $(filter %.c,$(SRC))) $(patsubst %.S, $(OUTDIR)/%.o, $(filter %.S,$(SRC)))

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

-include $(OBJ:.o=.d)

qemu:
	$(QEMU) -machine virt -bios default -kernel $(ELF) -smp $(HART_NUM) -m 128M -nographic

qemugdb:
	$(QEMU) -machine virt -bios default -kernel $(ELF) -smp $(HART_NUM) -m 128M -nographic -s -S

CSCOPEDIR := $(SRCDIRS) include

tags: cscope
	$(CTAGS) -L cscope.files

cscope:
	find $(CSCOPEDIR) -type f -name *.[chSs] > cscope.files
	$(CSCOPE) -q -R -k -b -i cscope.files

clean:
	rm -rf $(OUTDIR)
	rm -rf tags cscope.files cscope.out cscope.in.out cscope.po.out

.PHONY: all qemu qemugdb tags cscope clean
