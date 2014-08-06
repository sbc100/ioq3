#!/bin/sh
# Script for building ioquake for NaCl
#
# Before running this script you need to have installed some
# NaCl libraries from naclports.
#  export TOOLCHAIN=glibc
#  export NACL_SDK_ROOT=/path/to/pepper_sdk
#  make sdl

if [ -z "$NACL_SDK_ROOT" ]; then
   echo "Set \$NACL_SDK_ROOT to build for NaCl"
   exit 1
fi

CONFIG=debug
export CC=x86_64-nacl-gcc
export CXX=x86_64-nacl-g++
export LINK=x86_64-nacl-g++
export PLATFORM=nacl
export ARCH=x86_64
export CFLAGS=-DNO_SDL_GLEXT
export PKG_CONFIG_PATH=$NACL_SDK_ROOT/toolchain/linux_x86_glibc/x86_64-nacl/usr/lib/pkgconfig
export PATH=$NACL_SDK_ROOT/toolchain/linux_x86_glibc/bin:$PATH

# Build
make BUILD_SERVER=0 ${CONFIG} $* || exit $?

# Create nmf
EXE=build/${CONFIG}-nacl-$ARCH/ioquake3.$ARCH.nexe
NMF=build/${CONFIG}-nacl-$ARCH/ioquake3.nmf
SHARED_OBJECTS="build/${CONFIG}-nacl-$ARCH/renderer_opengl1_$ARCH.so build/${CONFIG}-nacl-$ARCH/renderer_opengl2_$ARCH.so"
LIB_PATH="-L$NACL_SDK_ROOT/toolchain/linux_x86_glibc/x86_64-nacl/lib"
LIB_PATH+=" -L$NACL_SDK_ROOT/toolchain/linux_x86_glibc/x86_64-nacl/usr/lib"
LIB_PATH+=" -L$NACL_SDK_ROOT/lib/glibc_$ARCH/Release"
LIB_PATH+=" -L$NACL_SDK_ROOT/ports/lib/glibc_$ARCH/Release"
CREATE_NMF="$NACL_SDK_ROOT/tools/create_nmf.py ${LIB_PATH} -D$NACL_SDK_ROOT/toolchain/linux_x86_glibc/bin/i686-nacl-objdump $EXE -o $NMF $SHARED_OBJECTS -s build/${CONFIG}-nacl-$ARCH"

echo $CREATE_NMF
$CREATE_NMF || echo "creat_nmf failed"

cp misc/index.html build/${CONFIG}-nacl-$ARCH
DATA_DIR=build/${CONFIG}-nacl-$ARCH
${NACL_SDK_ROOT}/tools/genhttpfs.py -r -C ${DATA_DIR} . > ${DATA_DIR}/nacl_manifest.txt

echo "created $NMF"
