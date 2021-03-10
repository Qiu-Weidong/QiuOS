# Qiux

## 编译方法
需要工具`nasm`汇编器、`gcc`编译器，以及`bximage`制作镜像文件
```bash
# 在Makefile的同级目录下
make
```
make成功后会得到一个Qiux.img的镜像文件，我们的操作系统就在镜像文件当中
## 运行方法
使用`qemu`或者`bochs`运行。如果使用`bochs`，需要配置bochsr文件，将第8行romimage配置为你的机器的BIOS-bochs-latest文件所在的目录，并将第9行vgaromimage配置为你的VGABIOS-lgpl-latest所在的目录，如果不进行调试，请将最后一行gdbstub注释掉。
```bash
# bochs,Qiux.img所在目录
bochs -f bochsrc
或
make run
# qemu，在Qiux.img所在目录
qemu-system-x86_64 -fda Qiux.img
```
## 调试方法
使用`bochs`和`gdb`调试，请确保`bochs`使用了`gdbstub`。
首先将bochsrc最后一行的`gdbstub:enabled=1,port=1234,text_base=0,data_base=0,bss_base=0`取消注释，然后在Makefile的同级目录下输入命令`make run`，接下来新开一个终端，同样在Makefile所在目录下，输入命令`make debug`，然后就可以使用gdb进行调试了。
```bash
# 在第一个终端当中，请将gdbstub取消注释
make run
# 新开一个终端
make debug
```
