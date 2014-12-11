#include <stdio.h>

#include "pe.h"

/* Parse PE header and populate PESTRUCT variable */
void parse_pe_header(PESTRUCT *p, FILE *fp, BYTE *buf) {
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
	p->peheadersize = lendian(buf, 2) + 40;	// 40 bytes for COFF header

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
}

/* Parse given section number and populate SECTSTRUCT variable */
void parse_section(SECTSTRUCT *sect, FILE *fp, BYTE *buf, int n) {
	;
}
