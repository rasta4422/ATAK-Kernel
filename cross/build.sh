#!/bin/sh

export PREFIX="$HOME/opt/cross"
export TARGET=x86_64-elf
export PATH="$PREFIX/bin:$PATH"

echo "Building binutils..."

function build_dir() {
    cd build-binutils
}

version=$(ls | grep -Po '(?<=binutils-)([0-9\.]+)$')

build_dir

echo "${version}"

../binutils-${version}/configure \
    --target="$TARGET" \
    --prefix="$PRFIX" \
    --with-sysroot \
    --disable-nls \
    --disable-werror

make
make install

echo "Finished building binutils..."

echo "Building gcc..."
cd ".."
which -- $TARGET-as || exit 1

mkdir build-gcc
cd "build-gcc"

version=$(ls | grep -Po '(?<=gcc-)([0-9\.]+)$')

../gcc-${version}/configure \
    --target=$TARGET \
    --prefix="$PREFIX" \
    --disable-nls \
    --enable-languages=c,c++ \
    --without-headers

make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc

echo "Finished building gcc..."

