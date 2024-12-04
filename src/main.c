#include <linux/init.h>
#include <linux/module.h>

#include <spi.h>

#include "ssd1351.h"

static struct ssd1351* left_display;

static int __init spectre_display_init( void ) {
	left_display = ssd1351_init( SPI_CHIP0, 5, 4 );
	if ( !left_display ) {
		return -1;
	}

	return 0;
}

static void __exit spectre_display_exit( void ) {
	ssd1351_exit( left_display );
}

MODULE_LICENSE( "GPL" );

module_init( spectre_display_init );
module_exit( spectre_display_exit );

