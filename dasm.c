#ifndef _DASMLIB_C
#define _DASMLIB_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "dasm.h"
#include "defs.h"

/* Parse assembly instruction from file stream at current position and return a pointer to the string */
char *parse_instr(FILE *fp, long int curaddr) {
	BYTE b = fgetc(fp);
	BYTE mod;
	BYTE ext;
	char *ret = (char *) malloc(64 * sizeof(char));
	char *opa1 = (char *) malloc(64 * sizeof(char));
	char *opa2 = (char *) malloc(64 * sizeof(char));
	char *tmp;
	DWORD val32;
	int i;

	///TEST show first byte of instruction
	printf("%.2X\t", b);

	switch (b) {

	case 0x50:	/* 50+rd  PUSH r32 */
	case 0x51:	/* 50+rd  PUSH r32 */
	case 0x52:	/* 50+rd  PUSH r32 */
	case 0x53:	/* 50+rd  PUSH r32 */
	case 0x54:	/* 50+rd  PUSH r32 */
	case 0x55:	/* 50+rd  PUSH r32 */
	case 0x56:	/* 50+rd  PUSH r32 */
	case 0x57:	/* 50+rd  PUSH r32 */
		sprintf(ret, "PUSH %s", reg_table(b, 'd'));
		break;

	case 0x74:	/* 74 cb  JE rel8 */
		// Get 1-byte operand for offset
		b = fgetc(fp);
		sprintf(ret, "JE SHORT %.8X", (b + 2 + curaddr));
		break;

	case 0x83:	/* 83 /d ib */
		// Get ModR/M and opcode extension
		b = fgetc(fp);
		mod = (b & 0xC0) >> 6;
		ext = (b & 0x38) >> 3;
		switch (ext) {
		case 5:	/* 83 /5 ib  SUB r/m32,imm8 */
			if (mod == 0) {
				//TODO
			} else if (mod == 3) {
				opa1 = reg_table((b & 7), 'd');
				// Get constant argument
				b = fgetc(fp);
				*opa2 = b;
			} else {
				opa1 = "OPA1ERR";
				opa2 = "OPA2ERR";
			}
			sprintf(ret, "SUB %s,%X", opa1, *opa2);
			break;
		default:
			ret = "OPCERR";
			break;
		}
		break;

	case 0x85:	/* 85 /r  TEST r/m32,r32 */
		b = fgetc(fp);
		mod = (b & 0xC0) >> 6;
		if (mod == 0) {
			//TODO
		} else if (mod == 3) {
			opa1 = reg_table(((b >> 3) & 7), 'd');
			opa2 = reg_table((b & 7), 'd');
		} else {
			opa1 = "OPA1ERR";
			opa2 = "OPA2ERR";
		}
		sprintf(ret, "TEST %s,%s", opa1, opa2);
		break;

	case 0x89:	/* 89 /r  MOV r/m32,r32 */
		b = fgetc(fp);
		mod = (b & 0xC0) >> 6;
		if (mod == 0) {
			//TODO
		} else if (mod == 3) {
			opa1 = reg_table(((b >> 3) & 7), 'd');
			opa2 = reg_table((b & 7), 'd');
		} else {
			opa1 = "OPA1ERR";
			opa2 = "OPA2ERR";
		}
		sprintf(ret, "MOV %s,%s", opa1, opa2);
		break;

	case 0xA1:	/* A1  MOV EAX,moffs32* */
		val32 = 0;
		for (i = 0; i < 4; i++) {
			b = fgetc(fp);
			val32 |= (b << (i * 8));
		}
		sprintf(ret, "MOV EAX,DWORD PTR DS:[%X]", val32);
		break;

	case 0xC7:	/* C7 /0  MOV r/m32,imm32 */
		b = fgetc(fp);		// Get ModR/M and opcode extension
		mod = (b & 0xC0) >> 6;
		ext = (b & 0x38) >> 3;
		if (ext == 0) {
			if (mod == 0) {
				b = fgetc(fp);		// Get SIB
				tmp = sib_to_str(b);
				sprintf(ret, "MOV DWORD PTR SS:[%s],", tmp);
				free(tmp);
			} else if (mod == 1) {
				b = fgetc(fp);		// Get SIB
				tmp = sib_to_str(b);
				b = fgetc(fp);		// Get displacement byte (signed)
				if (b & 0x80) {
					sprintf(ret, "MOV DWORD PTR SS:[%s-%X],", tmp, (BYTE) ((~b) + 1));
				} else {
					sprintf(ret, "MOV DWORD PTR SS:[%s+%X],", tmp, b);
				}
				free(tmp);
			} else {
				//TODO
			}
			/* Get second operand */
			val32 = 0;
			for (i = 0; i < 4; i++) {
				b = fgetc(fp);
				val32 |= (b << (i * 8));
			}
			sprintf(ret + strlen(ret), "%X", val32);	// Append to string
		} else {
			ret = "OPCERR";
		}
		break;

	case 0xFF:	/* FF */
		b = fgetc(fp);		// Get ModR/M and opcode extension
		mod = (b & 0xC0) >> 6;
		ext = (b & 0x38) >> 3;
		switch (ext) {
			//TODO
		case 2:	/* FF /2  CALL r/m32 */
			opa1 = reg_table((b & 7), 'd');
			sprintf(ret, "CALL %s", opa1);
			break;
		default:
			ret = "OPCERR";
			break;
		}
		break;

	default:
		ret = "OPCERR";
		break;
	}

	free(opa1);
	free(opa2);

	return ret;
}

/* Return register corresponding to given byte and size, according to Table 3-1 */
/* bwd: 'b' (byte), 'w' (word), 'd' (dword)
   /* Table 3-1. Register Encodings Associated with the +rb, +rw, and +rd Nomenclature
   /*   rb      rw       rd
   /* AL = 0  AX = 0  EAX = 0
   /* CL = 1  CX = 1  ECX = 1
   /* DL = 2  DX = 2  EDX = 2
   /* BL = 3  BX = 3  EBX = 3
   /* AH = 4  SP = 4  ESP = 4
   /* CH = 5  BP = 5  EBP = 5
   /* DH = 6  SI = 6  ESI = 6
   /* BH = 7  DI = 7  EDI = 7
*/
char *reg_table(BYTE b, char bwd) {
	// Only use 3 bits
	b &= 7;

	char *reg;

	switch (bwd) {
	case 'b':
		switch (b) {
		case 0:
			reg = "AL";
			break;
		case 1:
			reg = "CL";
			break;
		case 2:
			reg = "DL";
			break;
		case 3:
			reg = "BL";
			break;
		case 4:
			reg = "AH";
			break;
		case 5:
			reg = "CH";
			break;
		case 6:
			reg = "DH";
			break;
		case 7:
			reg = "BH";
			break;
		default:
			break;
		}
		break;
	case 'w':
		switch (b) {
		case 0:
			reg = "AX";
			break;
		case 1:
			reg = "CX";
			break;
		case 2:
			reg = "DX";
			break;
		case 3:
			reg = "BX";
			break;
		case 4:
			reg = "SP";
			break;
		case 5:
			reg = "BP";
			break;
		case 6:
			reg = "SI";
			break;
		case 7:
			reg = "DI";
			break;
		default:
			break;
		}
		break;
	case 'd':
		switch (b) {
		case 0:
			reg = "EAX";
			break;
		case 1:
			reg = "ECX";
			break;
		case 2:
			reg = "EDX";
			break;
		case 3:
			reg = "EBX";
			break;
		case 4:
			reg = "ESP";
			break;
		case 5:
			reg = "EBP";
			break;
		case 6:
			reg = "ESI";
			break;
		case 7:
			reg = "EDI";
			break;
		default:
			break;
		}
		break;
	default:
		reg = "REGERR";
		break;
	}

	return reg;
}

/* Return string from given SIB (Scale-Index-Base) byte */
/* 7 6    5 4 3  2 1 0 */
/* Scale  Index  Base */
/* string format: Index*2^Scale+Base */
char *sib_to_str(BYTE sib) {
	BYTE scale;
	BYTE index;
	BYTE base;
	char *str = (char *) malloc(32 * sizeof(char));

	scale = (sib & 0xC0) >> 6;
	index = (sib & 0x38) >> 3;
	base = (sib & 0x07);

	if (index == base && base == 4) {	// Special case where Index and Base both refer to ESP
		sprintf(str, "%s", reg_table(base, 'd'));
	} else if (scale != 0) {
		sprintf(str, "%s*%d+%s", reg_table(index, 'd'), (BYTE) pow(2, scale), reg_table(base, 'd'));
	} else {
		sprintf(str, "%s+%s", reg_table(index, 'd'), reg_table(base, 'd'));
	}

	return str;
}

#endif
