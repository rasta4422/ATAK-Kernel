#!/bin/bash

export PREFIX="$HOME/opt/cross"
export TARGET=x86_64-elf
export PATH="$PREFIX/bin:$PATH"

echo "Building gcc..."


which -- $TARGET-as || exit 1

version=$(ls ../ | grep -Po '(?<=gcc-)([0-9\.]+)[^\.a-z]')


echo "Found GCC version -- ${version}"

if find ../gcc-${version} -maxdepth 0; then
    echo "Tar already extracted..."
else
    tar -xvf ../gcc-${version}.tar.xz --directory ../
fi

if find ../gcc-${version} -maxdepth 0; then
    echo "Tar successful..."
else
    echo "Tar unsuccessful..."
    exit 1
fi

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

