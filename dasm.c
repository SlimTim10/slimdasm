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

	/* case 0x00:	/\* 00 /r	ADD r/m8,r8 *\/ */
	/* 	/\* ADD Eb,Gb *\/ */
	/* 	// Get ModR/M byte */
	/* 	b = fgetc(fp); */
	/* 	opa1 = parse_modrm(b); */
	/* 	sprintf(ret, "ADD", opa1); */
	/* 	mod = get_mod(b); */
	/* 	break; */

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

	case 0x66:	/* Operand-size override */
		//TODO
		break;

	case 0x74:	/* 74 cb  JE rel8 */
		// Get 1-byte operand for offset
		b = fgetc(fp);
		sprintf(ret, "JE SHORT %.8X", (b + 2 + curaddr));
		break;

	case 0x83:	/* 83 /d ib */
		// Get ModR/M and opcode extension
		b = fgetc(fp);
		mod = get_mod(b);
		ext = get_regop(b);
		switch (ext) {
		case 5:	/* 83 /5 ib  SUB r/m32,imm8 */
			if (mod == 0) {
				//TODO
			} else if (mod == 3) {
				opa1 = reg_table(get_rm(b), 'd');
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
		mod = get_mod(b);
		if (mod == 0) {
			//TODO
		} else if (mod == 3) {
			opa1 = reg_table(get_regop(b), 'd');
			opa2 = reg_table(get_rm(b), 'd');
		} else {
			opa1 = "OPA1ERR";
			opa2 = "OPA2ERR";
		}
		sprintf(ret, "TEST %s,%s", opa1, opa2);
		break;

	case 0x89:	/* 89 /r  MOV r/m32,r32 */
		b = fgetc(fp);
		mod = get_mod(b);
		if (mod == 0) {
			//TODO
		} else if (mod == 3) {
			opa1 = reg_table(get_regop(b), 'd');
			opa2 = reg_table(get_rm(b), 'd');
		} else {
			opa1 = "OPA1ERR";
			opa2 = "OPA2ERR";
		}
		sprintf(ret, "MOV %s,%s", opa1, opa2);
		break;

	case 0x8B:	// MOV Gv,Ev
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'G', 'd');
		opa2 = parse_modrm(fp, b, 'E', 'd');
		sprintf(ret, "MOV %s,%s", opa1, opa2);
		break;

	case 0xA1:	/* A1  MOV EAX,moffs32* */
		val32 = 0;
		for (i = 0; i < 4; i++) {
			b = fgetc(fp);
			val32 |= (b << (i * 8));
		}
		sprintf(ret, "MOV EAX,DWORD PTR [%X]", val32);
		break;

	case 0xC7:	/* C7 /0  MOV r/m32,imm32 */
		b = fgetc(fp);		// Get ModR/M and opcode extension
		mod = get_mod(b);
		ext = get_regop(b);
		if (ext == 0) {
			if (mod == 0) {
				b = fgetc(fp);		// Get SIB
				tmp = sib_to_str(b);
				sprintf(ret, "MOV DWORD PTR [%s],", tmp);
				free(tmp);
			} else if (mod == 1) {
				b = fgetc(fp);		// Get SIB
				tmp = sib_to_str(b);
				b = fgetc(fp);		// Get displacement byte (signed)
				if (b & 0x80) {
					sprintf(ret, "MOV DWORD PTR [%s-%X],", tmp, (BYTE) ((~b) + 1));
				} else {
					sprintf(ret, "MOV DWORD PTR [%s+%X],", tmp, b);
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
		mod = get_mod(b);
		ext = get_regop(b);
		switch (ext) {
			//TODO
		case 2:	/* FF /2  CALL r/m32 */
			switch(get_rm(b)) {
			case 0:
				//TODO
				break;
			case 5:
				val32 = fgetc(fp)
					+ (fgetc(fp) << 8)
					+ (fgetc(fp) << 16)
					+ (fgetc(fp) << 24);
				sprintf(ret, "CALL DWORD PTR [%X]", val32);
				break;
			case 6:
				//TODO
				break;
			}
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

/* Parse ModR/M byte and return a string of the operand, given the addressing code (A.1.1.) and operand size, according to Table 2-2 */
char *parse_modrm(FILE *fp, BYTE b, char addr_code, char bwd) {
	char *ret = (char *) malloc(64 * sizeof(char));
	BYTE mod = get_mod(b);
	BYTE regop = get_regop(b);
	BYTE rm = get_rm(b);
	DWORD val32;
	char *tmp;

	switch (addr_code) {
	case 'E':	// Register or memory address
		switch (mod) {
		case 0x00:
			switch (rm) {
			case 0:	// Mod = 00, R/M = 000
			case 1:	// Mod = 00, R/M = 001
			case 2:	// Mod = 00, R/M = 010
			case 3:	// Mod = 00, R/M = 011
			case 6:	// Mod = 00, R/M = 110
			case 7:	// Mod = 00, R/M = 111
				sprintf(ret, "DWORD PTR [%s]", reg_table(rm, bwd));
				break;
			case 4:	// Mod = 00, R/M = 100
				// SIB byte follows
				b = fgetc(fp);
				sprintf(ret, "DWORD PTR [%s]", sib_to_str(b));
				break;
			case 5:	// Mod = 00, R/M = 101
				// 32-bit displacement follows
				val32 = fgetc(fp)
					+ (fgetc(fp) << 8)
					+ (fgetc(fp) << 16)
					+ (fgetc(fp) << 24);
				sprintf(ret, "CALL DWORD PTR [%X]", val32);
				break;
			}
			break;
		case 0x01:
			switch (rm) {
			case 0:	// Mod = 01, R/M = 000
			case 1:	// Mod = 01, R/M = 001
			case 2:	// Mod = 01, R/M = 010
			case 3:	// Mod = 01, R/M = 011
			case 5:	// Mod = 01, R/M = 101
			case 6:	// Mod = 01, R/M = 110
			case 7:	// Mod = 01, R/M = 111
				// sign-extended 8-bit displacement follows
				b = fgetc(fp);
				if (b & 0x80) {
					sprintf(ret, "DWORD PTR [%s-%X]", reg_table(rm, bwd), ((~b)+1));
				} else {
					sprintf(ret, "DWORD PTR [%s+%X]", reg_table(rm, bwd), b);
				}
				break;
			case 4:	// Mod = 01, R/M = 100
				// SIB byte follows
				b = fgetc(fp);
				tmp = sib_to_str(b);
				// sign-extended 8-bit displacement follows
				b = fgetc(fp);
				if (b & 0x80) {
					sprintf(ret, "DWORD PTR [%s-%X]", tmp, ((~b)+1));
				} else {
					sprintf(ret, "DWORD PTR [%s+%X]", tmp, b);
				}
				break;
			}
			break;
		case 0x02:
			switch (rm) {
			case 0:	// Mod = 10, R/M = 000
			case 1:	// Mod = 10, R/M = 001
			case 2:	// Mod = 10, R/M = 010
			case 3:	// Mod = 10, R/M = 011
			case 5:	// Mod = 10, R/M = 101
			case 6:	// Mod = 10, R/M = 110
			case 7:	// Mod = 10, R/M = 111
				// 32-bit displacement follows
				val32 = fgetc(fp)
					+ (fgetc(fp) << 8)
					+ (fgetc(fp) << 16)
					+ (fgetc(fp) << 24);
				sprintf(ret, "DWORD PTR [%s+%X]", reg_table(rm, bwd), val32);
				break;
			case 4:	// Mod = 10, R/M = 100
				// SIB byte follows
				b = fgetc(fp);
				tmp = sib_to_str(b);
				// 32-bit displacement follows
				val32 = fgetc(fp)
					+ (fgetc(fp) << 8)
					+ (fgetc(fp) << 16)
					+ (fgetc(fp) << 24);
				sprintf(ret, "DWORD PTR [%s+%X]", tmp, val32);
				break;
			}
			break;
		case 0x03:
			ret = reg_table(rm, bwd);
			break;
		}
		break;
	case 'G':	// Register
		ret = reg_table(regop, bwd);
		break;
	}

	return ret;
}

/* Return Mod from given ModR/M byte, according to Figure 2-1 */
BYTE get_mod(BYTE b) {
	return (b & 0xC0) >> 6;
}

/* Return Reg/Opcode from given ModR/M byte, according to Figure 2-1 */
BYTE get_regop(BYTE b) {
	return (b & 0x38) >> 3;
}

/* Return R/M from given ModR/M byte, according to Figure 2-1 */
BYTE get_rm(BYTE b) {
	return (b & 0x07);
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
