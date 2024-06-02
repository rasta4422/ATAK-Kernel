#include <tty.h>
//#include <string.h>


//unsigned int strlen(const char*);

// enum for different vga colors
/*
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

// width and height of screen 
static const unsigned int VGA_WIDTH = 80;
static const unsigned int VGA_HEIGHT = 25;

static unsigned int row;
static unsigned int column;
static unsigned char color;

// VGA magic number 
static unsigned short int *buffer = (unsigned short int*) 0xB8000;


void tty_clear(void)
{
    row = 0;
    column = 0;
    color = VGA_COLOR_GREEN | VGA_COLOR_BLACK << 4;
    for (unsigned int j = 0; j < VGA_HEIGHT; j++) {
        for (unsigned int i = 0; i < VGA_WIDTH; i++) {
            const unsigned int index = j * VGA_WIDTH + i;
            buffer[index] = (unsigned short int) ' ' | (unsigned short int) color << 8;
        }
    }
}

void tty_print(const char *str)
{
    for (unsigned int i = 0; i < strlen(str); i++) {
        const unsigned int idx = column * VGA_WIDTH + row;
        buffer[idx] = (unsigned short int) str[i] | (unsigned short int) color << 8;
        if (column == VGA_WIDTH - 1){
            column = 0;
            if (row == VGA_HEIGHT - 1){
                row = 0;
            }
        }
        column++;
        row++;
    }
}

*/

typedef short int           int16_t;
typedef unsigned char       uint8_t;
typedef unsigned short int  uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned long int   uint64_t;

#include <bootboot.h>

extern BOOTBOOT bootboot;               // 
extern unsigned char environment[4096]; // configuration, UTF-8 text key=value pairs
extern uint8_t fb;                      // linear framebuffer mapped

typedef struct {
    uint32_t magic;
    uint32_t version;
    uint32_t headersize;
    uint32_t flags;
    uint32_t numglyph;
    uint32_t bytesperglyph;
    uint32_t height;
    uint32_t width;
    uint8_t glyphs;
} __attribute__((packed)) psf2_t;
extern volatile unsigned char _binary_font_psf_start;

void puts(char *s)
{
    psf2_t *font = (psf2_t*)&_binary_font_psf_start;
    int x,y,kx=0,line,mask,offs;
    int bpl=(font->width+7)/8;
    while(*s) {
        unsigned char *glyph = (unsigned char*)&_binary_font_psf_start + font->headersize +
            (*s>0&&*s<font->numglyph?*s:0)*font->bytesperglyph;
        offs = (kx * (font->width+1) * 4);
        for(y=0;y<font->height;y++) {
            line=offs; mask=1<<(font->width-1);
            for(x=0;x<font->width;x++) {
                *((uint32_t*)((uint64_t)&fb+line))=((int)*glyph) & (mask)?0xFFFFFF:0;
                mask>>=1; line+=4;
            }
            *((uint32_t*)((uint64_t)&fb+line))=0; glyph+=bpl; offs+=bootboot.fb_scanline;
        }
        s++; kx++;
    }
}

