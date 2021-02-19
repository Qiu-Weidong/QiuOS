# Makefile for QiuOS

# 编译、链接、调试和运行程序
ASM 		:= nasm
CC  		:= gcc
LD  		:= ld
DEBUG		:= gdb
BOCHS		:= bochs
BXIMG		:= bximage

# 参数
BOOTFLAGS	:= -I ./boot/include/
ASMFLAGS	:= -f elf -g -F DWARF
CFLAGS		:= -I include/ -m32 -c -fno-builtin -fno-stack-protector -g
LDFLAGS		:= -Ttext 0x10400 -e kernel_main -m elf_i386
GDBFLAGS	:= -tui -q  -ex "target remote localhost:1234"

# 目标文件夹
BUILD 		:= build

# 挂载目录
MNTDIR		:= /mnt/floppy

# 配置文件
BOCHSRC 	:= bochsrc

# 源文件搜索目录
vpath %.c kernel lib
vpath %.asm kernel lib

# 源文件
CSOURCE		:= $(notdir $(wildcard kernel/*c lib/*.c))
ASMSOURCE	:= $(notdir $(wildcard kernel/*.asm lib/*.asm))

# 可重定位目标文件(*.o文件)
OBJS		:= $(addprefix $(BUILD)/,$(CSOURCE:.c=.o) $(ASMSOURCE:.asm=.o))

# 目标文件
IMG 		:= QiuOS.img 
BOOT 		:= $(BUILD)/boot.bin
LOADER 		:= $(BUILD)/loader.bin
KERNEL		:= $(BUILD)/kernel.bin

# 颜色


# 伪目标
.PHONY:all clean rebuild run debug

all:  $(IMG) 

$(BUILD):
	mkdir $@

$(IMG):$(BUILD) $(BOOT) $(LOADER) $(KERNEL)
	rm -rf *.img
	$(BXIMG) $@ -q -mode=create -fd=1.44M >/dev/null
	dd if=$(BOOT) of=$@ bs=512 count=1 conv=notrunc >/dev/null 2>/dev/null
	sudo mount $@ $(MNTDIR) -t vfat -o loop
	sudo cp $(LOADER) $(KERNEL) $(MNTDIR) -v >/dev/null
	sudo sync
	sudo umount $(MNTDIR)
	@echo "\033[49;32mBuild Sucess ===> $@\033[0m"

$(BOOT) : boot/boot.asm boot/include/*
	$(ASM) $(BOOTFLAGS) -o $@ $< 

$(LOADER) : boot/loader.asm boot/include/*
	$(ASM) $(BOOTFLAGS) -o $@ $<

$(BUILD)/%.o:%.c include/*
	$(CC) $(CFLAGS) -o $@ $<

$(BUILD)/%.o:%.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

$(KERNEL): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

clean:
	rm -rf $(BUILD) $(IMG)

rebuild:clean all

# 请确保bochsrc文件中最后一行gdbstub:enabled=1 ... 被取消注释
debug:$(IMG) $(KERNEL)
	@$(DEBUG) $(GDBFLAGS) $(KERNEL)

run:$(IMG) $(BOCHSRC)
	@$(BOCHS) -f $(BOCHSRC) -q 

