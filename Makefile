OBJS:= start.o main.o device/device.o lib/lib.o

CFLAGS:= -fno-builtin -I$(shell pwd)/include	
export CFLAGS

gboot.bin:gboot.elf
	arm-linux-objcopy -O binary gboot.elf gboot.bin	
	
gboot.elf:$(OBJS)
	arm-linux-ld -Tgboot.lds -o gboot.elf $^
	
	arm-linux-readelf -a gboot.elf >dump

%.o:%.S
	arm-linux-gcc -g -c $^
%.o:%.c
	arm-linux-gcc $(CFLAGS) -c $^
	
lib/lib.o:
	make -C lib all
	
device/device.o:
	make -C device all

.PHONY:clean
clean:
	rm *.o *.elf *.bin dump
	make -C lib clean
	make -C device clean




