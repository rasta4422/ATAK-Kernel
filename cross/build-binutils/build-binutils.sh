#!/bin/sh

export PREFIX="$HOME/opt/cross"
export TARGET=x86_64-elf
export PATH="$PREFIX/bin:$PATH"

echo "Building binutils..."

version=$(ls ../ | grep -Po '(?<=binutils-)([0-9\.]+)[^\.a-z]')

echo "Found binutils version -- ${version}"

if find ../binutils-${version} -maxdepth 0; then
    echo "Tar already extracted..."
else
    tar -xvf ../binutils-${version}.tar.xz --directory ../
fi

if find ../binutils-${version} -maxdepth 0; then
    echo "Tar successful..."
else
    echo "Tar unsuccessful..."
    exit 1
fi


echo "Building binutils..."
../binutils-${version}/configure \
    --target="$TARGET" \
    --prefix="$PRFIX" \
    --with-sysroot \
    --disable-nls \
    --disable-werror

make
make install

echo "Finished building binutils..."

