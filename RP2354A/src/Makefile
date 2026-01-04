build: gensin.h
	cmake --build build --target blink

flash:
	picotool load build/blink.elf && picotool reboot

prep: gensin.h
	cmake -DPICO_PLATFORM=rp2350 -S . -B build

gensin.h: gensin
	./gensin > gensin.h

gensin: gensin.c
	$(CC) gensin.c -o gensin -lm

test_sincos: gensin.h test_sincos.c util.h
	 $(CC) test_sincos.c -o test_sincos -lm

test_pow2: test_pow2.c util.h
	 $(CC) test_pow2.c -o test_pow2 -lm

test: test_sincos test_pow2
	./test_sincos
	./test_pow2

.PHONY: build test
