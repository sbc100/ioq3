#!/bin/sh
# Script for building ioquake for NaCl
#
# Before running this script you need to have installed some
# NaCl libraries from naclports.
#  export NACL_GLIBC=1
#  export NACL_PACKAGES_BITSIZE=64
#  export NACL_SDK_ROOT=/path/to/pepper_sdk
#  make sdl
#  make nacl-mounts
#  make regal

if [ -z "$NACL_SDK_ROOT" ]; then
   echo "Set \$NACL_SDK_ROOT to build for NaCl"
   exit 1
fi

export CC=x86_64-nacl-gcc
export CXX=x86_64-nacl-g++
export LD=x86_64-nacl-g++
export PLATFORM=nacl
export ARCH=x86_64
export CFLAGS=-DNO_SDL_GLEXT
export PKG_CONFIG_PATH=$NACL_SDK_ROOT/toolchain/linux_x86_glibc/x86_64-nacl/usr/lib/pkgconfig
export PATH=$NACL_SDK_ROOT/toolchain/linux_x86_glibc/bin:$PATH

exec make $*
