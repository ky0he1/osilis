#!/bin/bash

cd $HOME/edk2
# 初回のみ
# ln -s $HOME/github/osilis/OsilisLoaderPkg ./
source edksetup.sh
build
$HOME/osbook/devenv/run_qemu.sh Build/OsilisLoaderX64/DEBUG_CLANG38/X64/Loader.efi