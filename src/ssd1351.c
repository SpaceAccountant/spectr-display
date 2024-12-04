#include "ssd1351.h"

#include <gpio.h>
#include <spi.h>

#include "sleep.h"

#define NR_DISPLAYS 2

#define SSD1351_SPI_CLK_DIV		32
#define SSD1351_FRONT_CLK_DIV_AND_FREQ	0xF1

#define SSD1351_DISPLAY_BYTES_PER_PIXEL ( SSD1351_DISPLAY_BPP >> 3 )

#define SSD1351_DISPLAY_BUFFER_SIZE ( SSD1351_DISPLAY_XRES * SSD1351_DISPLAY_YRES \
				  * SSD1351_DISPLAY_BYTES_PER_PIXEL )

/**
 * SSD1351 commands.
 *
 */
#define SSD1351_SET_COLUMN_ADDR			0x15
#define SSD1351_SET_ROW_ADDR			0x75
#define SSD1351_WRITE_RAM			0x5C
#define SSD1351_READ_RAM			0x5D
#define SSD1351_SET_REMAP_AND_COLOR_DEPTH	0xA0
#define SSD1351_SET_DISPLAY_START_LINE		0xA1
#define SSD1351_SET_DISPLAY_OFFSET		0xA2
#define SSD1351_SET_DISPLAY_MODE_ALL_OFF	0xA4
#define SSD1351_SET_DISPLAY_MODE_ALL_ON		0xA5
#define SSD1351_SET_DISPLAY_MODE_NORMAL		0xA6
#define SSD1351_SET_DISPLAY_MODE_INVERSE	0xA7
#define SSD1351_FUNCTION_SELECT			0xAB
#define SSD1351_SET_SLEEP_MODE_ON		0xAE
#define SSD1351_SET_SLEEP_MODE_OFF		0xAF
#define SSD1351_SET_RESET_AND_PRE_CHARGE	0xB1
#define SSD1351_SET_DISPLAY_ENHANCEMENT		0xB2
#define SSD1351_SET_FRONT_CLK_DIV_AND_FREQ	0xB3
#define SSD1351_SET_SEGMENT_LOW_VOLTAGE		0xB4
#define SSD1351_SET_GPIO			0xB5
#define SSD1351_SET_SECOND_PRE_CHARGE_PERIOD	0xB6
#define SSD1351_SET_GRAY_SCALE_LOOKUP_TABLE	0xB8
#define SSD1351_USE_BUILT_IN_LINEAR_BUT		0xB9
#define SSD1351_SET_PRE_CHARGE_VOLTAGE		0xBB
#define SSD1351_SET_VCOMH_VOLTAGE		0xBE
#define SSD1351_SET_COLOR_CONTRAST_CURRENT	0xC1
#define SSD1351_SET_MASTER_CONTRAST_CURRENT	0xC7
#define SSD1351_SET_MUX_RATIO			0xCA
#define SSD1351_SET_COMMAND_LOCK		0xFD
#define SSD1351_SET_HORIZONTAL_SCROLL		0x96
#define SSD1351_STOP_MOVING			0x9E
#define SSD1351_START_MOVING			0x9F

struct ssd1351 {
	int cs;
	int din;
	int rst;
	unsigned char back_buffer[SSD1351_DISPLAY_BUFFER_SIZE];
};

static struct ssd1351 displays[NR_DISPLAYS];

static const unsigned char init_list[] = {
	SSD1351_SET_COMMAND_LOCK,
	1,
	0x12,

	SSD1351_SET_COMMAND_LOCK,
	1,
	0xB1,

	SSD1351_SET_FRONT_CLK_DIV_AND_FREQ,
	1,
	SSD1351_FRONT_CLK_DIV_AND_FREQ,

	SSD1351_SET_GPIO,
	1,
	0x00,

	SSD1351_SET_DISPLAY_OFFSET,
	1,
	0x00,

	SSD1351_SET_COLOR_CONTRAST_CURRENT,
	3,
	0xC8,
	0x80,
	0xC8,

	SSD1351_SET_MASTER_CONTRAST_CURRENT,
	1,
	0x0F,

	SSD1351_SET_SEGMENT_LOW_VOLTAGE,
	3,
	0xA0,
	0xB5,
	0x55,

	SSD1351_SET_SLEEP_MODE_OFF,
	0
};

static const unsigned char exit_list[] = {
	SSD1351_SET_DISPLAY_MODE_ALL_OFF,
	0,

	SSD1351_SET_SLEEP_MODE_ON,
	0
};

static void ssd1351_send_command( struct ssd1351* display, unsigned char command,
		const unsigned char* data, int len ) {
	// Configure the SPI bus
	spi_set_clk_div( SSD1351_SPI_CLK_DIV );
	spi_select_chip( display->cs );
	spi_set_mode( SPI_MODE0 );
	spi_disable_reads();

	// Start an SPI transfer
	spi_begin_transfer();

	// Send the command
	gpio_set_pin_low( display->din );
	spi_write_byte( command );

	// Send any arguments/data
	if ( len > 0 ) {
		gpio_set_pin_high( display->din );
		spi_write( len, data );
	}

	// Synchronize with the SPI bus and end the SPI transfer
	spi_await_transfer();
	spi_end_transfer();
}

static void ssd1351_reset( struct ssd1351* display ) {
	// Run the reset proceedure for the display
	gpio_set_pin_high( display->rst );
	sleep( 100 );
	gpio_set_pin_low( display->rst );
	sleep( 100 );
	gpio_set_pin_high( display->rst );
	sleep( 100 );
}

static void ssd1351_run_list( struct ssd1351* display, const unsigned char* list, int len ) {
	// Walk through a sequential array of commands where each command is formated as:
	// +---------+--------------+--------------+
	// | Command | Nr Arguments | Arguments... |
	// +---------+--------------+--------------+
	for ( int i = 0; i < len; i += 2 + list[i + 1] ) {
		ssd1351_send_command( display, list[i], &( list[i + 2] ), list[i + 1] );
	}
}

struct ssd1351* __init ssd1351_init( unsigned int cs, unsigned int din, unsigned int rst ) {
	if ( cs >= NR_DISPLAYS ) {
		return ( struct ssd1351* ) 0;
	}

	gpio_set_pin_mode( din, GPIO_PIN_MODE_OUTPUT );
	gpio_set_pin_mode( rst, GPIO_PIN_MODE_OUTPUT );

	struct ssd1351* display = &( displays[cs] );
	display->cs	= cs;
	display->din	= din;
	display->rst	= rst;

	ssd1351_reset( display );
	ssd1351_run_list( display, init_list, sizeof( init_list ) );

	return display;
}

void __exit ssd1351_exit( struct ssd1351* display ) {
	ssd1351_run_list( display, exit_list, sizeof( exit_list ) );

	gpio_set_pin_mode( display->din, GPIO_PIN_MODE_INPUT );
	gpio_set_pin_mode( display->rst, GPIO_PIN_MODE_INPUT );
}

unsigned char* ssd1351_get_back_buffer( struct ssd1351* display ) {
	return display->back_buffer;
}

void ssd1351_flush_back_buffer( struct ssd1351* display ) {
	ssd1351_send_command( display, SSD1351_WRITE_RAM, display->back_buffer,
		sizeof( display->back_buffer ) );
}

