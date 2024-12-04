#ifndef _SPECTRE_DISPLAY_SSD1351_H
#define _SPECTRE_DISPLAY_SSD1351_H

#include <linux/init.h>

#define SSD1351_ERR_INVALID_CS -1 // The chip select provided for a command is not valid.

#define SSD1351_DISPLAY_XRES		128
#define SSD1351_DISPLAY_YRES		128
#define SSD1351_DISPLAY_BPP		16

#define SSD1351_DISPLAY_RED_BPP		5
#define SSD1351_DISPLAY_RED_OFFSET	11

#define SSD1351_DISPLAY_GREEN_BPP	6
#define SSD1351_DISPLAY_GREEN_OFFSET	5

#define SSD1351_DISPLAY_BLUE_BPP	5
#define SSD1351_DISPLAY_BLUE_OFFSET	0

struct ssd1351;

/**
 * Initializes an SSD1351 display.
 * 
 * @param cs The SPI chip select of the display.
 * @param din The GPIO pin for the D/C pin of the display.
 * @param rst The GPIO pin for the RES pin of the display.
 *
 * @returns The handle to the display.
 * 
 */
struct ssd1351* __init ssd1351_init( unsigned int cs, unsigned int din, unsigned int rst );

/**
 * Destroys an SSD1351 display.
 * 
 * @param display The handle to the display.
 * 
 */
void __exit ssd1351_exit( struct ssd1351* display );

/**
 * Gets the back buffer of an SSD1351 dispaly.
 * 
 * @param display The handle to the display.
 * 
 * @returns The address of the back buffer.
 * 
 */
unsigned char* ssd1351_get_back_buffer( struct ssd1351* display );

/**
 * Flushes the back buffer of an SSD1351 display to the display hardware.
 * 
 * @param display The handle to the display.
 * 
 */
void ssd1351_flush_back_buffer( struct ssd1351* display );

#endif // _SPECTRE_DISPLAY_SSD1351_H

