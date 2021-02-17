# Makefile for boot

# Programs, flags, etc.
ASM			:= nasm
CC			:= gcc
LD 			:= ld

ASMFLAGS	:= -I ./boot/include/
CFLAGS		:= -I include/ -m32 -c -fno-builtin -fno-stack-protector
LDFLAGS		:= -s -Ttext 0x100a0 -e kernel_main -m elf_i386

IMG			:= QiuOS.img 
BOOT 		:= build/boot.bin
LOADER 		:= build/loader.bin
KERNEL		:= build/kernel.bin
BUILD 		:= build


.PHONY:all clean debug rebuild


all: $(BUILD) $(IMG) 

$(BUILD):
	@mkdir $@

$(IMG): $(BOOT) $(LOADER) $(KERNEL)
	@rm -rf *.img
	@bximage $@ -q -mode=create -fd=1.44M 					>/dev/null
	@dd if=$< of=$@ bs=512 count=1 conv=notrunc 			>/dev/null 2>/dev/null

	@sudo mount $@ /mnt/floppy -t vfat -o loop
	@sudo cp $(LOADER) $(KERNEL) /mnt/floppy -v 			>/dev/null
	@sudo sync
	@sudo umount /mnt/floppy
	@echo "\033[49;32mBuild Sucess ===> QiuOS.img\033[0m"

$(BOOT) : boot/boot.asm boot/include/*
	@echo "\033[49;34mBuild $(BOOT)\033[0m"
	@$(ASM) $(ASMFLAGS) -o $@ $< 

$(LOADER) : boot/loader.asm boot/include/*
	@echo "\033[49;33mBuild $(LOADER)\033[0m"
	@$(ASM) $(ASMFLAGS) -o $@ $<

# $ nasm -f elf foo.asm -o foo.o
# $ gcc -m32 -c bar.c -o bar.o
# $ ld -s -m elf_i386 foo.o bar.o -o foobar
build/kernel.o : kernel/kernel.c include/*
	@echo "\033[49;35mBuild kernel\033[0m"
	@$(CC) $(CFLAGS) -o $@ $<

build/interrupt.o : kernel/interrupt.c include/*
	@echo "\033[49;38mBuild interrupt\033[0m"
	@$(CC) $(CFLAGS) -o $@ $<

build/kliba.o : lib/kliba.asm 
	@echo "\033[49;32mBuild kliba\033[0m"
	@$(ASM) -f elf -o $@ $<

build/klibc.o: lib/klibc.c include/*
	@echo "\033[49;37mBuild klibc\033[0m"
	@$(CC) $(CFLAGS) -o $@ $<

$(KERNEL): build/kernel.o build/kliba.o build/interrupt.o build/klibc.o 
	@echo "\033[49;35mLink kernel\033[0m"
	@$(LD) $(LDFLAGS) -o $@ $^

clean:
	@rm -rf $(BUILD) $(IMG)

debug:$(IMG)
	bochs -f bochsrc -q 	>/dev/null 2>/dev/null

rebuild:clean all