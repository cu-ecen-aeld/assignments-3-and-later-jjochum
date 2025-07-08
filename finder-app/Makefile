SYSROOT ?= /home/jonas/arm-gnu-toolchain-13.3.rel1-x86_64-aarch64-none-linux-gnu
CROSS_COMPILE ?=  
CC := $(CROSS_COMPILE)gcc


objects := writer.o
exe := writer

ifdef CROSS_COMPILE
	CC := $(SYSROOT)/bin/$(CROSS_COMPILE)gcc
else
	CC := gcc
endif

all : clean writer

$(exe) : $(objects)
	$(CC) -o writer writer.o

writer.o : writer.c 
	$(CC) -c writer.c

clean : 
	rm $(exe) || true
	rm $(objects) || true
