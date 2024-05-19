RM:=rm -rf
MKDIR:=mkdir -p

.PHONY: clean all qemu

all:
	@set -e
	. ./config.sh
	$(MKDIR) iso/boot/grub
	cp atakroot/boot/atak_kernel.bin iso/boot/atak_kernel.bin
	cat > iso/boot/grub/grub.cfg << EOF
	menuentry "myos" {
		multiboot /boot/atak_kernel.bin
	}
	grub-mkrescue -o atak_kernel.iso iso

qemu:
	qemu-system-i786 -cdrom atak_kernel.iso -boot menu=on

clean:
	$(RM) atakroot
	$(RM) iso
	$(RM) atak_kernel.iso 
