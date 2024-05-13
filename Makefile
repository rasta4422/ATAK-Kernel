CC :=gcc
CFLAGS :=-Wall

INCDIR := include
LOGDIR := log
BINDIR := bin
SRCDIR := src
ISODIR := iso
OFILES := obj
IMGDIR := img

SHELLDIR := $(SRCDIR)/shell
KERNELDIR := $(SRCDIR)/kernel
THREADDIR := $(SRCDIR)/threading
LINKERDIR := $(SRCDIR)/linkers
BOOTDIR := $(SRCDIR)/boot
CFG := $(BOOTDIR)/grub/grub.cfg

BOOTISODIR := $(ISODIR)/boot
GRUBDIR := $(BOOTISODIR)/grub

CP :=cp
RM :=rm -rf
MKDIR :=mkdir -p

$(OFILES)/bootloader.o: $(BOOTDIR)/bootloader.asm
	nasm -f elf64 $^ -o $@

$(OFILES)/atak_kernel.o: $(KERNELDIR)/atak_kernel.c $(SHELLDIR)/ataksh.c $(SHELLDIR)/logger.c $(THREADDIR)/tstructs.c $(THREADDIR)/thread.c
	$(CC) $(CFLAGS) -r -m64 -I$(INCDIR) $^ -o $@

#$(BINDIR)/atak_kernel: $(OFILES)/atak_kernel.o $(OFILES)/bootloader.o
#	$(CC) $(CFLAGS) -m64 $^ -o $@ -v

all: $(OFILES)/atak_kernel.o $(BINDIR)/atak_kernel

$(BINDIR)/atak_kernel: $(OFILES)/atak_kernel.o $(OFILES)/bootloader.o
	ld -dynamic-linker /lib/ld-linux.so.2 \
		/usr/lib/crt1.o /usr/lib/crti.o $^ \
		-L/usr/lib -lc /usr/lib/crtn.o \
		-m elf_x86_64 -T $(LINKERDIR)/linker.ld -o $@

atak_iso:
	$(MKDIR) $(GRUBDIR)
	@echo "MADE GRUBDIR"
	$(CP) $(BINDIR)/atak_kernel $(BOOTISODIR)
	$(CP) $(CFG) $(GRUBDIR)
	grub-mkrescue -o $(ISODIR)/atak_kernel.iso $(ISODIR)

qemu: atak_iso
	qemu-img create -f raw $(IMGDIR)/atak_img 4G
	qemu-system-x86_64 -cdrom $(ISODIR)/atak_kernel.iso \
		-boot menu=on -drive file=$(IMGDIR)/atak_img,format=raw

shell: $(SHELLDIR)/ataksh.c $(SHELLDIR)/logger.c
	$(CC) $(CFLAGS) -I$(INCDIR) $^ -o $(OFILES)/ataksh

ATAK: shell
	$(OFILES)/ataksh

.PHONY: clean
clean:
	rm -rf $(OFILES)/* $(LOGDIR)/ataksh.log $(BINDIR)/* $(ISODIR)/*
