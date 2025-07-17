include Makefile.conf

all: firmware

firmware:
	mkdir -p out
	make -C firmware

clean:
	rm -rf out/

.PHONY: all firmware clean
