#!/bin/bash

# kernelのビルド
cd $HOME/github/osilis/kernel
clang++ -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone \
    -fno-exceptions -fno-rtti -std=c++17 -c main.cpp
ld.lld --entry KernelMain -z norelro -z separate-code \
    --image-base 0x100000 --static -o kernel.elf main.o

cd $HOME/edk2
# 初回のみ
# ln -s $HOME/github/osilis/OsilisLoaderPkg ./
source edksetup.sh
build
$HOME/osbook/devenv/run_qemu.sh \
    Build/OsilisLoaderX64/DEBUG_CLANG38/X64/Loader.efi \
    $HOME/github/osilis/kernel/kernel.elf
