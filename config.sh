#!/bin/sh
BUILDENV="libc kernel"

export MAKE=${MAKE:-make}
export HOST=i786-elf

export AR=${HOST}-ar
export AS=${HOST}-as
export CC=${HOST}-gcc

export PREFIX=/usr
export EXEC_PREFIX=$PREFIX
export BOOTDIR=/boot
export LIBDIR=$EXEC_PREFIX/lib
export INCLUDEDIR=$PREFIX/include

export CFLAGS='-O2 -g'
export CPPFLAGS=''

# Configure the cross-compiler to use the desired system root.
export ROOT="$(pwd)/atakroot"
export CC="$CC --sysroot=$ROOT"

mkdir -p "$ROOT"

for PROJECT in $BUILDENV; do
  (cd $PROJECT && ROOT="$ROOT" $MAKE install)
done

for PROJECT in $BUILDENV; do
  (cd $PROJECT && ROOT="$ROOT" $MAKE install)
done


