#ifndef _SPECTRE_DISPLAY_SLEEP_H
#define _SPECTRE_DISPLAY_SLEEP_H

#include <linux/jiffies.h>

#define sleep( ms ) { \
	unsigned long start = jiffies; \
	while ( jiffies < ( start + ( ( ( unsigned long ) ( ms ) * HZ ) / 1000 ) ) ) {} \
}

#endif // _SPECTRE_DISPLAY_SLEEP_H

