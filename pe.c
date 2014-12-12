#include <stdio.h>
#include <stdlib.h>

#include "pe.h"

/* Parse PE header and populate PESTRUCT variable */
void parse_pe_header(FILE *fp, PESTRUCT *p) {
	BYTE *buf = (BYTE *) malloc(16 * sizeof(BYTE));

	DWORD fpos = ftell(fp);	// Store the current file position

	// Get PE offset value from DOS stub
	fseek(fp, 0x3C, SEEK_SET);
	fgets(buf, 4, fp);
	p->offset = lendian(buf, 4);

	// Get number of sections
	fseek(fp, (p->offset + 6), SEEK_SET);
	fgets(buf, 2, fp);
	p->numsects = lendian(buf, 2);

	// Get size of PE header
	fseek(fp, (p->offset + 20), SEEK_SET);
	fgets(buf, 2, fp);
	p->peheadersize = lendian(buf, 2) + 24;	// 24 bytes for COFF header and PE signature

	// Get RVA of entry point
	fseek(fp, (p->offset + 40), SEEK_SET);
	fgets(buf, 4, fp);
	p->rvaep = lendian(buf, 4);

	// Get RVA of code section
	fseek(fp, (p->offset + 44), SEEK_SET);
	fgets(buf, 4, fp);
	p->rvacode = lendian(buf, 4);

	// Get RVA of data section
	fseek(fp, (p->offset + 48), SEEK_SET);
	fgets(buf, 4, fp);
	p->rvadata = lendian(buf, 4);

	// Get address of image base
	fseek(fp, (p->offset + 52), SEEK_SET);
	fgets(buf, 4, fp);
	p->imagebase = lendian(buf, 4);

	// Get size of code section
	fseek(fp, (p->offset + 28), SEEK_SET);
	fgets(buf, 4, fp);
	p->codesize = lendian(buf, 4);

	// Each section header size is 40 bytes
	p->sectheadersize = 40;

	fseek(fp, fpos, SEEK_SET);	// Restore the file position
	free(buf);
}

/* Parse given section number and populate SECTSTRUCT variable */
void parse_section(FILE *fp, PESTRUCT *p, SECTSTRUCT *sect, int n) {
	BYTE *buf = (BYTE *) malloc(4 * sizeof(BYTE));
	DWORD fpos = ftell(fp);	// Store the current file position

	// Go to beginning of section information
	DWORD sect_offset = (p->offset + p->peheadersize + (n * p->sectheadersize));
	fseek(fp, sect_offset, SEEK_SET);

	// Get section name
	fgets(sect->name, 8, fp);

	// Get virtual address
	fseek(fp, sect_offset + 12, SEEK_SET);
	fgets(buf, 4, fp);
	sect->va = lendian(buf, 4);

	// Get size of raw data
	fseek(fp, sect_offset + 16, SEEK_SET);
	fgets(buf, 4, fp);
	sect->size = lendian(buf, 4);

	// Get pointer to raw data
	fseek(fp, sect_offset + 20, SEEK_SET);
	fgets(buf, 4, fp);
	sect->offset = lendian(buf, 4);

	fseek(fp, fpos, SEEK_SET);	// Restore the file position
	free(buf);
}
