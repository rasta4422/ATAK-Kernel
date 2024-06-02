#!/bin/bash

./mkbootimg atakconfig.json initrd.bin

rm -rf initrd

mkdir iso iso/bootboot iso/boot iso/boot/grub 2>/dev/null || true

cp bootx86.bin iso/bootboot/loader || true

cp config iso/bootboot/config || true

cp initrd.bin iso/bootboot/initrd || true

cat > iso/boot/grub/grub.cfg << EOF 
menuentry "ATAK" {
    set root=(hd0)
    chainloader +1
    boot
}
EOF
grub-mkrescue -o grub.iso iso

rm -r iso 2>/dev/null || true
