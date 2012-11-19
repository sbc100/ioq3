#!/bin/sh

export CC=x86_64-nacl-gcc
export LD=x86_64-nacl-g++
export PLATFORM=nacl
export ARCH=x86_64
export PKG_CONFIG_PATH=$NACL_SDK_ROOT/toolchain/linux_x86_glibc/x86_64-nacl/usr/lib/pkgconfig
export PATH=$NACL_SDK_ROOT/toolchain/linux_x86_glibc/x86_64-nacl/usr/bin:$PATH

exec make $*
