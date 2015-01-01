#include <stdio.h>
#include <stdlib.h>

#include "pe.h"
#include "global.h"

/* Parse PE header and populate PESTRUCT variable */
void parse_pe_header(void) {
	BYTE *buf = (BYTE *) malloc(16 * sizeof(BYTE));

	DWORD fpos = ftell(fin);	// Store the current file position

	// Get PE offset value from DOS stub
	fseek(fin, 0x3C, SEEK_SET);
	fgets(buf, 4, fin);
	pe->offset = lendian(buf, 4);

	// Get number of sections
	fseek(fin, (pe->offset + 6), SEEK_SET);
	fgets(buf, 2, fin);
	pe->numsects = lendian(buf, 2);

	// Get size of PE header
	fseek(fin, (pe->offset + 20), SEEK_SET);
	fgets(buf, 2, fin);
	pe->peheadersize = lendian(buf, 2) + 24;	// 24 bytes for COFF header and PE signature

	// Get RVA of entry point
	fseek(fin, (pe->offset + 40), SEEK_SET);
	fgets(buf, 4, fin);
	pe->rvaep = lendian(buf, 4);

	// Get RVA of code section
	fseek(fin, (pe->offset + 44), SEEK_SET);
	fgets(buf, 4, fin);
	pe->rvacode = lendian(buf, 4);

	// Get RVA of data section
	fseek(fin, (pe->offset + 48), SEEK_SET);
	fgets(buf, 4, fin);
	pe->rvadata = lendian(buf, 4);

	// Get address of image base
	fseek(fin, (pe->offset + 52), SEEK_SET);
	fgets(buf, 4, fin);
	pe->imagebase = lendian(buf, 4);

	// Get size of code section
	fseek(fin, (pe->offset + 28), SEEK_SET);
	fgets(buf, 4, fin);
	pe->codesize = lendian(buf, 4);

	// Each section header size is 40 bytes
	pe->sectheadersize = 40;

	fseek(fin, fpos, SEEK_SET);	// Restore the file position
	free(buf);
}

/* Parse given section number and populate SECTSTRUCT variable */
void parse_section(SECTSTRUCT *sect, int n) {
	BYTE *buf = (BYTE *) malloc(4 * sizeof(BYTE));
	DWORD fpos = ftell(fin);	// Store the current file position

	// Go to beginning of section information
	DWORD sect_offset = (pe->offset + pe->peheadersize + (n * pe->sectheadersize));
	fseek(fin, sect_offset, SEEK_SET);

	// Get section name
	fgets(sect->name, 8, fin);

	// Get virtual address
	fseek(fin, sect_offset + 12, SEEK_SET);
	fgets(buf, 4, fin);
	sect->va = lendian(buf, 4);

	// Get size of raw data
	fseek(fin, sect_offset + 16, SEEK_SET);
	fgets(buf, 4, fin);
	sect->size = lendian(buf, 4);

	// Get pointer to raw data
	fseek(fin, sect_offset + 20, SEEK_SET);
	fgets(buf, 4, fin);
	sect->offset = lendian(buf, 4);

	fseek(fin, fpos, SEEK_SET);	// Restore the file position
	free(buf);
}
