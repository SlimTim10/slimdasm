#ifndef _SCAN_CODES_H
#define _SCAN_CODES_H

enum single_scans {
	KEY_ESC = 0x1B,
};

enum special_scans {
	SPECIAL_KEY = 0xE0,	// Special key prefix for following scan codes
	KEY_PGDN = 0x51,
	KEY_DOWN = 0x50,
	KEY_PGUP = 0x49,
	KEY_UP = 0x48,
	KEY_HOME = 0x47,
};

#endif
