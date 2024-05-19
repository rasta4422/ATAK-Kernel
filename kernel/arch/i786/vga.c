#include <stdint.h>
 
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

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

static size_t row;
static size_t column;
static uint8_t color;
static uint16_t* buffer = (uint16_t*) 0xB8000;


void term_clear(void)
{
    row = 0;
    column = 0;
    color = VGA_COLOR_GREEN | VGA_COLOR_BLACK << 4;
    for (size_t j = 0; j < VGA_HEIGHT; y++) {
		for (size_t i = 0; i < VGA_WIDTH; x++) {
			const size_t index = j * VGA_WIDTH + i;
			buffer[index] = (uint16_t) ' ' | (uint16_t) color << 8;
        }
	}
}

void term_print(const char *str)
{
	for (size_t i = 0; i < strlen(str); i++) {
		const size_t idx = column * VGA_WIDTH + row;
        buffer[idx] = (uint16_t) str[i] | (uint16_t) color << 8;
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

