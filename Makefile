# Makefile for boot

# Programs, flags, etc.
ASM			:= nasm
CC			:= gcc
LD 			:= ld

ASMFLAGS	:= -I ./boot/include/
CFLAGS		:= -I include/ -m32 -c -fno-builtin

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

$(BOOT) : boot/boot.asm boot/include/const.inc boot/include/lib16.inc
	@echo "\033[49;34mBuild $(BOOT)\033[0m"
	@$(ASM) $(ASMFLAGS) -o $@ $< 

$(LOADER) : boot/loader.asm boot/include/const.inc boot/include/lib16.inc boot/include/pm.inc boot/include/lib32.inc boot/include/loader.inc
	@echo "\033[49;33mBuild $(LOADER)\033[0m"
	@$(ASM) $(ASMFLAGS) -o $@ $<

# $ nasm -f elf foo.asm -o foo.o
# $ gcc -m32 -c bar.c -o bar.o
# $ ld -s -m elf_i386 foo.o bar.o -o foobar
# build/kernel.o : ./kernel/kernel.asm ./include/const.h ./include/protect.h ./include/type.h
build/kernel.o : kernel/main.c include/const.h include/type.h 
	@echo "\033[49;36mBuild kernel\033[0m"
	@$(CC) $(CFLAGS) -o $@ $<

build/klib.o : lib/klib.asm 
	@echo "\033[49;36mBuild klib\033[0m"
	@$(ASM) -f elf -o $@ $<

$(KERNEL): build/kernel.o build/klib.o
	@echo "\033[49;35mLink kernel\033[0m"
	@$(LD) -s -Ttext 0x100a0 -e kernel_main -m elf_i386 -o $@ $^

clean:
	@rm -rf $(BUILD) $(IMG)

debug:$(IMG)
	bochs -f bochsrc -q 	>/dev/null 2>/dev/null

rebuild:clean all