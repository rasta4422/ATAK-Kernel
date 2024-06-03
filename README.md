# ATAK Kernel and Shell
A very simple 64-bit kernel and command-line shell written in C. This project is currently still in development, with the eventual goal of making it a not so simple shell.

# Status
The kernel is currently able to boot from grub with the help of [BOOTBOOT](https://gitlab.com/bztsrc/bootboot). Upon booting, the kernel will greet you with a message. I am in the processes of creating a standard libary similar to libc, which will allow me to properly integrate the shell.

See the [BOOTBOOT protocol](https://gitlab.com/bztsrc/bootboot/-/blob/master/bootboot_spec_1st_ed.pdf?ref_type=heads) for more info about the boot process.

# Dependancies
  - GRUB (For running grub-mkrescue)
  - Xorriso (.iso creation engine)
  - qemu (with x86 support)
  - GNU make

# Install
## Cross Compiler
First you need to build and install the cross compiler for x86
### Binutils
```
cd cross/build-binutils
./build-binutils.sh
```

### GCC
```
cd cross/build-gcc
./build-gcc.sh
```

## Booting
You install and boot by running this from the root directory:
```
make
```

You can also install and boot one by one, like this:
```
make install
make qemu
```

# Cleaning
From the root directory run
```
make clean
```

