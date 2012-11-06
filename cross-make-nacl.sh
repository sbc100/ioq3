#!/bin/sh

export CC=i686-nacl-gcc
export PLATFORM=nacl
export ARCH=x86_64
export PKG_CONFIG_PATH=$NACL_SDK_ROOT/toolchain/linux_x86_glibc/i686-nacl/usr/lib/pkgconfig
export PATH=$NACL_SDK_ROOT/toolchain/linux_x86_glibc/i686-nacl/usr/bin:$PATH

exec make $*
