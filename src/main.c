#include <linux/init.h>
#include <linux/module.h>

#include <spi.h>

#include "ssd1351.h"

#define LEFT_DISPLAY_DIN	5
#define LEFT_DISPLAY_RST	4

#define RIGHT_DISPLAY_DIN	6
#define RIGHT_DISPLAY_RST	12

static struct ssd1351* left_display;
static struct ssd1351* right_display;

static int __init spectre_display_init( void ) {
	left_display = ssd1351_init( SPI_CHIP0, LEFT_DISPLAY_DIN, LEFT_DISPLAY_RST );
	if ( !left_display ) {
		return -1;
	}
	right_display = ssd1351_init( SPI_CHIP1, RIGHT_DISPLAY_DIN, RIGHT_DISPLAY_RST );
	if ( !right_display ) {
		return -1;
	}

	return 0;
}

static void __exit spectre_display_exit( void ) {
	if ( left_display ) {
		ssd1351_exit( left_display );
	}
	if ( right_display ) {
		ssd1351_exit( right_display );
	}
}

MODULE_LICENSE( "GPL" );

module_init( spectre_display_init );
module_exit( spectre_display_exit );

