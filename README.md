# QiuOS

## 编译方法
需要工具`nasm`汇编器、`gcc`编译器
```bash
# 在Makefile的同级目录下
make
```
make成功后会得到一个QiuOS.img的镜像文件，我们的操作系统就在镜像文件当中
## 运行方法
使用`qemu`或者`bochs`运行
```bash
# bochs
bochs -f bochsrc
# qemu
qemu-system-x86_64 -fda QiuOS.img
```
