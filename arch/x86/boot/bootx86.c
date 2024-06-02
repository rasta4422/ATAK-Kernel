/* we don't assume stdint.h exists */
#include <tty.h>
typedef short int           int16_t;
typedef unsigned char       uint8_t;
typedef unsigned short int  uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned long int   uint64_t;

#include <bootboot.h>

extern BOOTBOOT bootboot;               /* BOOTBOOT */
extern unsigned char environment[4096]; /* configuration from atakconfig.json */
extern uint8_t fb;                      /* BOOTBOOT framebuffer */

void kernel_main();

/* entry point for kernel main call */
void _start()
{
    int x, y, s=bootboot.fb_scanline, w=bootboot.fb_width, h=bootboot.fb_height;

    if(s) {
        kernel_main();
    }
    
    while(1);
}

