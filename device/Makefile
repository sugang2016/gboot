objs := led.o mmu.o button.o beep.o interrupt.o nand.o uart.o dma.o lcd.o touch.o dm9000.o arp.o

all : $(objs)
	arm-linux-ld -r -o device.o $^
	
%.o : %.c
	arm-linux-gcc ${CFLAGS} -c $^
	
%.o : %.S
	arm-linux-gcc ${CFLAGS} -c $^

clean:
	rm -f *.o		
	
