include ../Makefile.conf

FIRMWAREOBJ := ../out/firmware.o

all: ../out/program.mem

../out/program.mem: ../out/firmware.bin
	xxd -e -g4 -c4 $< | cut -d' ' -f2 | awk '{ print toupper($$0) }' > $@

../out/firmware.bin: ../out/firmware.elf
	$(OBJCOPY) -O binary $< $@

../out/firmware.elf: $(FIRMWAREOBJ)
	$(LD) -o $@ $^ -T link.ld

../out/%.o: %.S
	$(CC) -o $@ $< $(CCFLAGS)

../out/%.o: %.c
	$(CC) -o $@ $< $(CCFLAGS)

write: ../out/firmware.mem
	cat $< > mem/program.mem

clean:
	rm -rf out/

.PHONY: all write clean
