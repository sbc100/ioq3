#!/bin/sh

if [ -z "$NACL_SDK_ROOT" ]; then
   echo "Set \$NACL_SDK_ROOT to build for NaCl"
   exit 1
fi

export CC=x86_64-nacl-gcc
export CXX=x86_64-nacl-g++
export LD=x86_64-nacl-g++
export PLATFORM=nacl
export ARCH=x86_64
export PKG_CONFIG_PATH=$NACL_SDK_ROOT/toolchain/linux_x86_glibc/x86_64-nacl/usr/lib/pkgconfig
export PATH=$NACL_SDK_ROOT/toolchain/linux_x86_glibc/bin:$PATH

exec make $*
