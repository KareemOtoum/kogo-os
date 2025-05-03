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

KOut& KOut::operator<<(char c)
{
	terminal_putchar(c);

	return kout;
}

KOut& KOut::operator<<(int num)
{
	if(num == 0)
	{
		kout << '0';
	}
	if(num < 0)
	{
		kout << "-";
	}

	char buffer[10];
	int i {};
	while(num > 0)
	{
		buffer[i++] = '0' + num % 10;
		num /= 10;
	}

	while(i--)
	{
		kout << buffer[i];
	}
	
	return kout;
}

KOut& KOut::operator<<(uint32_t hex)
{
    kout << "0x";  // print "0x" at the start

    for (int i = 7; i >= 0; i--) 
    {
        uint8_t nibble = (hex >> (i * 4)) & 0xF;
        if (nibble < 10) 
        {
            kout << static_cast<char>('0' + nibble);  // Prints '0'-'9'
        }
        else 
        {
            kout << static_cast<char>('A' + (nibble - 10));  // Prints 'A'-'F'
        }
    }

    return kout;
}

KOut& KOut::operator<<(void* ptr)
{
	kout << reinterpret_cast<uintptr_t>(ptr);

	return kout;
}

void KOut::set_cursor_state(bool enable)
{
    outb(0x3D4, 0x0A);
    uint8_t cursor_start = inb(0x3D5);

    if (enable)
	{
		cursor_start &= ~(1 << 5); // clear bit 5 to enable
	}
    else
	{
		cursor_start |= (1 << 5);  // set bit 5 to disable
	}

    outb(0x3D4, 0x0A);
    outb(0x3D5, cursor_start);
}

void set_cursor_position(uint16_t row, uint16_t col) 
{
    uint16_t position = row * 80 + col;

    outb(0x3D4, 0x0F);
    outb(0x3D5, position & 0xFF);

    outb(0x3D4, 0x0E);
    outb(0x3D5, (position >> 8) & 0xFF);
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
	terminal_color = terminal::vga_entry_color(
        terminal::VGA_COLOR_LIGHT_GREY, terminal::VGA_COLOR_BLACK);
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

void terminal_scrolltwo()
{
	for(int y{}; y < terminal::k_VGAHeight - 2; ++y)
	{
		for(int x{}; x < terminal::k_VGAWidth; ++x)
		{
			terminal_buffer[y * terminal::k_VGAWidth + x] = 
				terminal_buffer[(y + 2) * terminal::k_VGAWidth + x];
		}
	}

	// clear last two lines and set cursor there
	for(int y{ terminal::k_VGAHeight - 2 }; y < terminal::k_VGAHeight; ++y)
	{
		for(int x{}; x < terminal::k_VGAWidth; ++x)
		{
			terminal_putentryat(' ', terminal_color, x, y);
		}
	}

	terminal_column = 0;
	terminal_row = terminal::k_VGAHeight - 2;
}

void terminal_putchar(char c) 
{
    if(c == '\n')
    {
        terminal_column = 0;
        ++terminal_row; 
        if(terminal_row == terminal::k_VGAHeight)
        {
			terminal_scrolltwo();
        }
        return;
    }

	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);

	if (++terminal_column == terminal::k_VGAWidth) {
		terminal_column = 0;
		if (++terminal_row == terminal::k_VGAHeight)
        {
            terminal_scrolltwo();        
		}
	}

	set_cursor_position(terminal_row, terminal_column);
}

