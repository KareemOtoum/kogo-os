#include "kout.h"

size_t strlen(const char* str);
void terminal_initialize(void);
void terminal_putchar(char c);

KOut kout{};

KOut::KOut()
{
    terminal_initialize();
}

KOut& KOut::operator<<(const char* str)
{
    size_t size { strlen(str) };
	for (size_t i = 0; i < size; i++)
		terminal_putchar(str[i]);

    return kout;
}

size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer = (uint16_t*)terminal::k_VGAMemory;

void clear_screen()
{
	terminal_row = 0;
	terminal_column = 0;

	for (size_t y = 0; y < terminal::k_VGAHeight; y++) 
    {
		for (size_t x = 0; x < terminal::k_VGAWidth; x++) 
        {
			const size_t index = y * terminal::k_VGAWidth + x;
			terminal_buffer[index] = terminal::vga_entry(' ', terminal_color);
		}
	}
}

void terminal_initialize(void) 
{
	terminal_color = terminal::vga_entry_color(terminal::VGA_COLOR_LIGHT_GREY, terminal::VGA_COLOR_BLACK);
	clear_screen();
}

void terminal_setcolor(uint8_t color) 
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * terminal::k_VGAWidth + x;
	terminal_buffer[index] = terminal::vga_entry(c, color);
}

void terminal_putchar(char c) 
{
    if(c == '\n')
    {
        terminal_column = 0;
        ++terminal_row; 
        if(terminal_row == terminal::k_VGAHeight)
        {
            clear_screen();
        }
        return;
    }

	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == terminal::k_VGAWidth) {
		terminal_column = 0;
		if (++terminal_row == terminal::k_VGAHeight)
        {
            clear_screen();
        }
	}
}

