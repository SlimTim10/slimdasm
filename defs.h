#ifndef _DEFS_H
#define _DEFS_H

#include <stdint.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;

enum constants {
	DUMP_MAX = 512,
	STRLEN_MAX = 256,
	FILE_BUFSIZE = 2048,
};

#endif
