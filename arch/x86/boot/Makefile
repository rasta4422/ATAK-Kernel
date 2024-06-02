ARCH		:=	x86_64

OVMF 		:= 	/usr/share/qemu/bios-TianoCoreEFI.bin

CC			:=	$(ARCH)-elf-gcc
LD			:=	$(ARCH)-elf-ld
STRIP		:=	$(ARCH)-elf-strip
READELF		:=	$(ARCH)-elf-readelf

CFLAGS 		:= 	-Wall -fpic -ffreestanding -fno-stack-protector -nostdinc -nostdlib -I./include/
LDFLAGS 	:=	-nostdlib -n -T link.ld
STRIPFLAGS 	:=	-s -K mmio -K fb -K bootboot -K environment -K initstack

TARGET		:= 	bootx86

OBJS=\
	 strlen.o \
	 tty.o	\
	 atak.o	\
	 $(TARGET).o \

all: grub.iso

.c.o:
	$(CC) $(CFLAGS) -mno-red-zone -c $^ -o $@

$(TARGET).bin: $(OBJS)
	$(CC) $(CFLAGS) -r -mno-red-zone -o $@ $^
	$(LD) -r -b binary -o font.o font.psf
	$(LD) $(LDFLAGS) $@ font.o -o $(TARGET).elf
	$(STRIP) $(STRIPFLAGS) $(TARGET).elf

install: $(TARGET).bin
	@mkdir initrd initrd/atak 2>/dev/null | true
	@cp $(TARGET).elf initrd/atak/kernel

disk: install atakconfig.json
	@./mkbootimg atakconfig.json disk-x86.img
	#@rm -rf initrd

grub.iso: disk
	@. ./grub-mkcfg.sh

readelf:
	$(READELF) -hls $(TARGET).elf >$(TARGET).txt

clean:
	rm -rf *.o *.bin *.iso *.elf *.img initrd || true
