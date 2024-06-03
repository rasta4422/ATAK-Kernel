OVMF		:=  /usr/share/qemu/bios-TianoCoreEFI.bin

ARCHDIR		:= arch/x86
BOOTDIR		:= $(ARCHDIR)/boot

export PATH 	:= $(HOME)/opt/cross/bin:$(PATH)

all: qemu

install:
	@cd $(BOOTDIR) && $(MAKE)

qemu: install
	qemu-system-x86_64 -drive file=$(BOOTDIR)/disk-x86.img,format=raw -cdrom $(BOOTDIR)/grub.iso -boot order=d -serial stdio

clean:
	@cd $(BOOTDIR) && $(MAKE) clean

