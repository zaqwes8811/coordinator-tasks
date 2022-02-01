 #!/bin/sh

# https://code.google.com/p/v8/wiki/CrossCompilingForARM

V=/home/zaqwes/big_device/condesourcery/arm-2009q1
#V=/home/zaqwes/big_device/condesourcery/arm-2007q1
# !!! It revision. Newer failed run or changed api.
#svn checkout http://v8.googlecode.com/svn/trunk/ v8 -r10448

export TOOL_PREFIX=$V/bin/arm-none-linux-gnueabi
export CXX=$TOOL_PREFIX-g++
export AR=$TOOL_PREFIX-ar
export RANLIB=$TOOL_PREFIX-ranlib
export CC=$TOOL_PREFIX-gcc
export LD=$TOOL_PREFIX-g++

export CCFLAGS="-mcpu=arm926ej-s -march=armv5te -mno-thumb-interwork"
#export ARM_TARGET_LIB=$V/arm-none-linux-gnueabi/libc  # Can't find ld-linux.so.3

# http://www.wigwag.com/devblog/cross-compile-v8-target-arm/!!
# https://code.google.com/p/v8/wiki/CrossCompilingForARM
# -j4 - ?
scons armeabi=soft wordsize=32 snapshot=off arch=arm vfp3=off sample=shell component=static_library

