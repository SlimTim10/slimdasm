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
	char *ret = (char *) malloc(64 * sizeof(char));	// Return string
	char *op = (char *) malloc(64 * sizeof(char));	// Opcode
	char *opa1 = (char *) malloc(64 * sizeof(char));	// Operand
	char *opa2 = (char *) malloc(64 * sizeof(char));	// Operand
	char *tmp;
	WORD w;
	DWORD d;
	int i;

	///TEST show first byte of instruction
	printf("%.2X\t", b);

	switch (b) {

	case 0x00:	// ADD Eb,Gb
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'b');
		opa2 = parse_modrm(fp, b, 'G', 'b');
		sprintf(ret, "ADD %s,%s", opa1, opa2);
		break;

	case 0x01:	// ADD Ev,Gv
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'd');
		opa2 = parse_modrm(fp, b, 'G', 'd');
		sprintf(ret, "ADD %s,%s", opa1, opa2);
		break;

	case 0x02:	// ADD Gb,Eb
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'b');
		opa2 = parse_modrm(fp, b, 'G', 'b');
		sprintf(ret, "ADD %s,%s", opa1, opa2);
		break;

	case 0x03:	// ADD Gv,Ev
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'G', 'd');
		opa2 = parse_modrm(fp, b, 'E', 'd');
		sprintf(ret, "ADD %s,%s", opa1, opa2);
		break;

	case 0x04:	// ADD AL,Ib
		b = fgetc(fp);
		sprintf(ret, "ADD AL,%X", b);
		break;

	case 0x05:	// ADD EAX,Iv
		d = get_dword(fp);
		sprintf(ret, "ADD EAX,%X", d);

	case 0x06:	// PUSH ES
		sprintf(ret, "PUSH ES");
		break;

	case 0x07:	// POP ES
		sprintf(ret, "POP ES");
		break;

	case 0x08:	// OR Eb,Gb
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'b');
		opa2 = parse_modrm(fp, b, 'G', 'b');
		sprintf(ret, "OR %s,%s", opa1, opa2);
		break;

	case 0x09:	// OR Ev,Gv
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'd');
		opa2 = parse_modrm(fp, b, 'G', 'd');
		sprintf(ret, "OR %s,%s", opa1, opa2);
		break;

	case 0x0A:	// OR Gb,Eb
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'b');
		opa2 = parse_modrm(fp, b, 'G', 'b');
		sprintf(ret, "OR %s,%s", opa1, opa2);
		break;

	case 0x0B:	// OR Gv,Ev
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'G', 'd');
		opa2 = parse_modrm(fp, b, 'E', 'd');
		sprintf(ret, "OR %s,%s", opa1, opa2);
		break;

	case 0x0C:	// OR AL,Ib
		b = fgetc(fp);
		sprintf(ret, "OR AL,%X", b);
		break;

	case 0x0D:	// OR EAX,Iv
		d = get_dword(fp);
		sprintf(ret, "OR EAX,%X", d);

	case 0x0E:	// PUSH CS
		sprintf(ret, "PUSH CS");
		break;

	case 0x0F:	// 2-byte escape
		sprintf(ret, "???");
		break;

	case 0x10:	// ADC Eb,Gb
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'b');
		opa2 = parse_modrm(fp, b, 'G', 'b');
		sprintf(ret, "ADC %s,%s", opa1, opa2);
		break;

	case 0x11:	// ADC Ev,Gv
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'd');
		opa2 = parse_modrm(fp, b, 'G', 'd');
		sprintf(ret, "ADC %s,%s", opa1, opa2);
		break;

	case 0x12:	// ADC Gb,Eb
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'b');
		opa2 = parse_modrm(fp, b, 'G', 'b');
		sprintf(ret, "ADC %s,%s", opa1, opa2);
		break;

	case 0x13:	// ADC Gv,Ev
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'G', 'd');
		opa2 = parse_modrm(fp, b, 'E', 'd');
		sprintf(ret, "ADC %s,%s", opa1, opa2);
		break;

	case 0x14:	// ADC AL,Ib
		b = fgetc(fp);
		sprintf(ret, "ADC AL,%X", b);
		break;

	case 0x15:	// ADC EAX,Iv
		d = get_dword(fp);
		sprintf(ret, "ADC EAX,%X", d);

	case 0x16:	// PUSH SS
		sprintf(ret, "PUSH SS");
		break;

	case 0x17:	// POP SS
		sprintf(ret, "POP SS");
		break;

	case 0x18:	// SBB Eb,Gb
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'b');
		opa2 = parse_modrm(fp, b, 'G', 'b');
		sprintf(ret, "SBB %s,%s", opa1, opa2);
		break;

	case 0x19:	// SBB Ev,Gv
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'd');
		opa2 = parse_modrm(fp, b, 'G', 'd');
		sprintf(ret, "SBB %s,%s", opa1, opa2);
		break;

	case 0x1A:	// SBB Gb,Eb
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'b');
		opa2 = parse_modrm(fp, b, 'G', 'b');
		sprintf(ret, "SBB %s,%s", opa1, opa2);
		break;

	case 0x1B:	// SBB Gv,Ev
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'G', 'd');
		opa2 = parse_modrm(fp, b, 'E', 'd');
		sprintf(ret, "SBB %s,%s", opa1, opa2);
		break;

	case 0x1C:	// SBB AL,Ib
		b = fgetc(fp);
		sprintf(ret, "SBB AL,%X", b);
		break;

	case 0x1D:	// SBB EAX,Iv
		d = get_dword(fp);
		sprintf(ret, "SBB EAX,%X", d);

	case 0x1E:	// PUSH DS
		sprintf(ret, "PUSH DS");
		break;

	case 0x1F:	// POP DS
		sprintf(ret, "POP DS");
		break;

	case 0x20:	// AND Eb,Gb
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'b');
		opa2 = parse_modrm(fp, b, 'G', 'b');
		sprintf(ret, "AND %s,%s", opa1, opa2);
		break;

	case 0x21:	// AND Ev,Gv
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'd');
		opa2 = parse_modrm(fp, b, 'G', 'd');
		sprintf(ret, "AND %s,%s", opa1, opa2);
		break;

	case 0x22:	// AND Gb,Eb
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'b');
		opa2 = parse_modrm(fp, b, 'G', 'b');
		sprintf(ret, "AND %s,%s", opa1, opa2);
		break;

	case 0x23:	// AND Gv,Ev
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'G', 'd');
		opa2 = parse_modrm(fp, b, 'E', 'd');
		sprintf(ret, "AND %s,%s", opa1, opa2);
		break;

	case 0x24:	// AND AL,Ib
		b = fgetc(fp);
		sprintf(ret, "AND AL,%X", b);
		break;

	case 0x25:	// AND EAX,Iv
		d = get_dword(fp);
		sprintf(ret, "AND EAX,%X", d);

	case 0x26:	// SEG=ES
		sprintf(ret, "SEG=ES");
		break;

	case 0x27:	// DAA
		sprintf(ret, "DAA");
		break;

	case 0x28:	// SUB Eb,Gb
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'b');
		opa2 = parse_modrm(fp, b, 'G', 'b');
		sprintf(ret, "SUB %s,%s", opa1, opa2);
		break;

	case 0x29:	// SUB Ev,Gv
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'd');
		opa2 = parse_modrm(fp, b, 'G', 'd');
		sprintf(ret, "SUB %s,%s", opa1, opa2);
		break;

	case 0x2A:	// SUB Gb,Eb
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'b');
		opa2 = parse_modrm(fp, b, 'G', 'b');
		sprintf(ret, "SUB %s,%s", opa1, opa2);
		break;

	case 0x2B:	// SUB Gv,Ev
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'G', 'd');
		opa2 = parse_modrm(fp, b, 'E', 'd');
		sprintf(ret, "SUB %s,%s", opa1, opa2);
		break;

	case 0x2C:	// SUB AL,Ib
		b = fgetc(fp);
		sprintf(ret, "SUB AL,%X", b);
		break;

	case 0x2D:	// SUB EAX,Iv
		d = get_dword(fp);
		sprintf(ret, "SUB EAX,%X", d);

	case 0x2E:	// SEG=CS
		sprintf(ret, "SEG=CS");
		break;

	case 0x2F:	// DAS
		sprintf(ret, "DAS");
		break;

	case 0x30:	// XOR Eb,Gb
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'b');
		opa2 = parse_modrm(fp, b, 'G', 'b');
		sprintf(ret, "XOR %s,%s", opa1, opa2);
		break;

	case 0x31:	// XOR Ev,Gv
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'd');
		opa2 = parse_modrm(fp, b, 'G', 'd');
		sprintf(ret, "XOR %s,%s", opa1, opa2);
		break;

	case 0x32:	// XOR Gb,Eb
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'b');
		opa2 = parse_modrm(fp, b, 'G', 'b');
		sprintf(ret, "XOR %s,%s", opa1, opa2);
		break;

	case 0x33:	// XOR Gv,Ev
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'G', 'd');
		opa2 = parse_modrm(fp, b, 'E', 'd');
		sprintf(ret, "XOR %s,%s", opa1, opa2);
		break;

	case 0x34:	// XOR AL,Ib
		b = fgetc(fp);
		sprintf(ret, "XOR AL,%X", b);
		break;

	case 0x35:	// XOR EAX,Iv
		d = get_dword(fp);
		sprintf(ret, "XOR EAX,%X", d);

	case 0x36:	// SEG=SS
		sprintf(ret, "SEG=SS");
		break;

	case 0x37:	// AAA
		sprintf(ret, "AAA");
		break;

	case 0x38:	// CMP Eb,Gb
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'b');
		opa2 = parse_modrm(fp, b, 'G', 'b');
		sprintf(ret, "CMP %s,%s", opa1, opa2);
		break;

	case 0x39:	// CMP Ev,Gv
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'd');
		opa2 = parse_modrm(fp, b, 'G', 'd');
		sprintf(ret, "CMP %s,%s", opa1, opa2);
		break;

	case 0x3A:	// CMP Gb,Eb
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'b');
		opa2 = parse_modrm(fp, b, 'G', 'b');
		sprintf(ret, "CMP %s,%s", opa1, opa2);
		break;

	case 0x3B:	// CMP Gv,Ev
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'G', 'd');
		opa2 = parse_modrm(fp, b, 'E', 'd');
		sprintf(ret, "CMP %s,%s", opa1, opa2);
		break;

	case 0x3C:	// CMP AL,Ib
		b = fgetc(fp);
		sprintf(ret, "CMP AL,%X", b);
		break;

	case 0x3D:	// CMP EAX,Iv
		d = get_dword(fp);
		sprintf(ret, "CMP EAX,%X", d);

	case 0x3E:	// SEG=DS
		sprintf(ret, "SEG=DS");
		break;

	case 0x3F:	// AAS
		sprintf(ret, "AAS");
		break;

	case 0x40:	// INC EAX
	case 0x41:	// INC ECX
	case 0x42:	// INC EDX
	case 0x43:	// INC EBX
	case 0x44:	// INC ESP
	case 0x45:	// INC EBP
	case 0x46:	// INC ESI
	case 0x47:	// INC EDI
	 	sprintf(ret, "INC %s", reg_table(b, 'd'));	// Same order as reg table
		break;

	case 0x48:	// DEC EAX
	case 0x49:	// DEC ECX
	case 0x4A:	// DEC EDX
	case 0x4B:	// DEC EBX
	case 0x4C:	// DEC ESP
	case 0x4D:	// DEC EBP
	case 0x4E:	// DEC ESI
	case 0x4F:	// DEC EDI
	 	sprintf(ret, "DEC %s", reg_table(b-7, 'd'));
		break;

	case 0x50:	// PUSH EAX
	case 0x51:	// PUSH ECX
	case 0x52:	// PUSH EDX
	case 0x53:	// PUSH EBX
	case 0x54:	// PUSH ESP
	case 0x55:	// PUSH EBP
	case 0x56:	// PUSH ESI
	case 0x57:	// PUSH EDI
	 	sprintf(ret, "PUSH %s", reg_table(b, 'd'));
		break;

	case 0x58:	// POP EAX
	case 0x59:	// POP ECX
	case 0x5A:	// POP EDX
	case 0x5B:	// POP EBX
	case 0x5C:	// POP ESP
	case 0x5D:	// POP EBP
	case 0x5E:	// POP ESI
	case 0x5F:	// POP EDI
	 	sprintf(ret, "PUSH %s", reg_table(b-7, 'd'));
		break;

	case 0x60:	// PUSHAD
		sprintf(ret, "PUSHAD");
		break;

	case 0x61:	// POPAD
		sprintf(ret, "POPAD");
		break;

	case 0x62:	// BOUND Gv,Ma
		//TODO
		break;

	case 0x63:	// ARPL Ew,Gw
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'w');
		opa2 = parse_modrm(fp, b, 'G', 'w');
		sprintf(ret, "ARPL %s,%s", opa1, opa2);
		break;

	case 0x64:	// SEG=FS
		sprintf(ret, "SEG=FS");
		break;

	case 0x65:	// SEG=GS
		sprintf(ret, "SEG=GS");
		break;

	case 0x66:	// Operand size override
		sprintf(ret, "Opd Size 16");
		break;

	case 0x67:	// Address size override
		sprintf(ret, "Addr Size 16");
		break;

	case 0x68:	// PUSH Iv
		d = get_dword(fp);
		sprintf(ret, "PUSH %X", d);
		break;

	case 0x69:	// IMUL Gv,Ev,Iv
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'G', 'd');
		opa2 = parse_modrm(fp, b, 'E', 'd');
		d = get_dword(fp);
		sprintf(ret, "IMUL %s,%s,%X", opa1, opa2, d);
		break;

	case 0x6A:	// PUSH Ib
		b = fgetc(fp);
		// Signed byte
		if (b & 0x80) {
			sprintf(ret, "PUSH -%X", ((~b)+1));
		} else {
			sprintf(ret, "PUSH %X", b);
		}
		break;

	case 0x6B:	// IMUL Gv,Ev,Ib
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'G', 'd');
		opa2 = parse_modrm(fp, b, 'E', 'd');
		b = fgetc(fp);
		sprintf(ret, "IMUL %s,%s,%X", opa1, opa2, b);
		break;

	case 0x6C:	// INS Yb,DX
		sprintf(ret, "INS BYTE PTR ES:[EDI],DX");
		break;

	case 0x6D:	// INS Yv,DX
		sprintf(ret, "INS DWORD PTR ES:[EDI],DX");
		break;

	case 0x6E:	// OUTS DX,Xb
		sprintf(ret, "OUTS DX,BYTE PTR ES:[EDI]");
		break;

	case 0x6F:	// OUTS DX,Xv
		sprintf(ret, "OUTS DX,DWORD PTR ES:[EDI]");
		break;

	case 0x70:	// JO SHORT
		b = fgetc(fp);
		sprintf(ret, "JO SHORT %X", curaddr + 2 + (signed char) b);
		break;

	case 0x71:	// JNO SHORT
		b = fgetc(fp);
		sprintf(ret, "JNO SHORT %X", curaddr + 2 + (signed char) b);
		break;

	case 0x72:	// JB SHORT
		b = fgetc(fp);
		sprintf(ret, "JB SHORT %X", curaddr + 2 + (signed char) b);
		break;

	case 0x73:	// JAE SHORT
		b = fgetc(fp);
		sprintf(ret, "JAE SHORT %X", curaddr + 2 + (signed char) b);
		break;

	case 0x74:	// JE SHORT
		b = fgetc(fp);
		sprintf(ret, "JE SHORT %X", curaddr + 2 + (signed char) b);
		break;

	case 0x75:	// JNE SHORT
		b = fgetc(fp);
		sprintf(ret, "JNE SHORT %X", curaddr + 2 + (signed char) b);
		break;

	case 0x76:	// JBE SHORT
		b = fgetc(fp);
		sprintf(ret, "JBE SHORT %X", curaddr + 2 + (signed char) b);
		break;

	case 0x77:	// JA SHORT
		b = fgetc(fp);
		sprintf(ret, "JA SHORT %X", curaddr + 2 + (signed char) b);
		break;

	case 0x78:	// JS SHORT
		b = fgetc(fp);
		sprintf(ret, "JS SHORT %X", curaddr + 2 + (signed char) b);
		break;

	case 0x79:	// JNS SHORT
		b = fgetc(fp);
		sprintf(ret, "JNS SHORT %X", curaddr + 2 + (signed char) b);
		break;

	case 0x7A:	// JP SHORT
		b = fgetc(fp);
		sprintf(ret, "JP SHORT %X", curaddr + 2 + (signed char) b);
		break;

	case 0x7B:	// JNP SHORT
		b = fgetc(fp);
		sprintf(ret, "JNP SHORT %X", curaddr + 2 + (signed char) b);
		break;

	case 0x7C:	// JL SHORT
		b = fgetc(fp);
		sprintf(ret, "JL SHORT %X", curaddr + 2 + (signed char) b);
		break;

	case 0x7D:	// JGE SHORT
		b = fgetc(fp);
		sprintf(ret, "JGE SHORT %X", curaddr + 2 + (signed char) b);
		break;

	case 0x7E:	// JLE SHORT
		b = fgetc(fp);
		sprintf(ret, "JLE SHORT %X", curaddr + 2 + (signed char) b);
		break;

	case 0x7F:	// JG SHORT
		b = fgetc(fp);
		sprintf(ret, "JG SHORT %X", curaddr + 2 + (signed char) b);
		break;

	// Immediate Group 1
	case 0x80:	// Eb,Ib
		b = fgetc(fp);
		op = group1_op(get_regop(b));
		opa1 = parse_modrm(fp, b, 'E', 'b');
		b = fgetc(fp);
		sprintf(ret, "%s %s,%X", op, opa1, b);
		break;

	// Immediate Group 1
	case 0x81:	// Ev,Iv
		b = fgetc(fp);
		op = group1_op(get_regop(b));
		opa1 = parse_modrm(fp, b, 'E', 'd');
		d = get_dword(fp);
		sprintf(ret, "%s %s,%X", op, opa1, d);
		break;

	// Immediate Group 1
	case 0x82:	// Ev,Ib
		b = fgetc(fp);
		op = group1_op(get_regop(b));
		opa1 = parse_modrm(fp, b, 'E', 'd');
		b = fgetc(fp);
		sprintf(ret, "%s %s,%X", op, opa1, b);
		break;

	// Immediate Group 1
	case 0x83:	// Ev,Ib
		b = fgetc(fp);
		op = group1_op(get_regop(b));
		opa1 = parse_modrm(fp, b, 'E', 'd');
		b = fgetc(fp);
		sprintf(ret, "%s %s,%X", op, opa1, b);
		break;

	case 0x84:	// TEST Eb,Gb
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'b');
		opa2 = parse_modrm(fp, b, 'G', 'b');
		sprintf(ret, "TEST %s,%s", opa1, opa2);
		break;

	case 0x85:	// TEST Ev,Gv
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'd');
		opa2 = parse_modrm(fp, b, 'G', 'd');
		sprintf(ret, "TEST %s,%s", opa1, opa2);
		break;

	case 0x86:	// XCHG Eb,Gb
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'b');
		opa2 = parse_modrm(fp, b, 'G', 'b');
		sprintf(ret, "XCHG %s,%s", opa1, opa2);
		break;

	case 0x87:	// XCHG Ev,Gv
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'd');
		opa2 = parse_modrm(fp, b, 'G', 'd');
		sprintf(ret, "XCHG %s,%s", opa1, opa2);
		break;

	case 0x88:	// MOV Eb,Gb
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'b');
		opa2 = parse_modrm(fp, b, 'G', 'b');
		sprintf(ret, "MOV %s,%s", opa1, opa2);
		break;

	case 0x89:	// MOV Ev,Gv
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'd');
		opa2 = parse_modrm(fp, b, 'G', 'd');
		sprintf(ret, "MOV %s,%s", opa1, opa2);
		break;

	case 0x8A:	// MOV Gb,Eb
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'G', 'b');
		opa2 = parse_modrm(fp, b, 'E', 'b');
		sprintf(ret, "MOV %s,%s", opa1, opa2);
		break;

	case 0x8B:	// MOV Gv,Ev
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'G', 'd');
		opa2 = parse_modrm(fp, b, 'E', 'd');
		sprintf(ret, "MOV %s,%s", opa1, opa2);
		break;

	case 0x8C:	// MOV Ew,Sw
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'w');
		opa2 = segreg_table(get_regop(b));
		sprintf(ret, "MOV %s,%s", opa1, opa2);
		break;

	case 0x8D:	// LEA Gv,M
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'G', 'd');
		opa2 = parse_modrm(fp, b, 'E', 'd');
		sprintf(ret, "LEA %s,%s", opa1, opa2);
		break;

	case 0x8E:	// MOV Sw,Ew
		b = fgetc(fp);
		opa1 = segreg_table(get_regop(b));
		opa2 = parse_modrm(fp, b, 'E', 'w');
		sprintf(ret, "MOV %s,%s", opa1, opa2);
		break;

	case 0x8F:	// POP Ev
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'd');
		sprintf(ret, "POP %s", opa1);
		break;

	case 0x90:	// NOP
		sprintf(ret, "NOP");
		break;

	// XCHG register with EAX
	case 0x91:
	case 0x92:
	case 0x93:
	case 0x94:
	case 0x95:
	case 0x96:
	case 0x97:
		sprintf(ret, "XCHG EAX,%s", reg_table(b, 'd'));
		break;

	case 0x98:	// CWDE
		sprintf(ret, "CWDE");
		break;

	case 0x99:	// CDQ
		sprintf(ret, "CDQ");
		break;

	case 0x9A:	// CALLF Ap
		d = get_dword(fp);
		w = get_word(fp);
		sprintf(ret, "CALL FAR %X:%X", w, d);
		break;

	case 0x9B:	// WAIT
		sprintf(ret, "WAIT");
		break;

	case 0x9C:	// PUSHFD
		sprintf(ret, "PUSHFD");
		break;

	case 0x9D:	// POPFD
		sprintf(ret, "POPFD");
		break;

	case 0x9E:	// SAHF
		sprintf(ret, "SAHF");
		break;

	case 0x9F:	// LAHF
		sprintf(ret, "LAHF");
		break;

	case 0xA0:	// MOV AL,Ob
		d = get_dword(fp);
		sprintf(ret, "MOV AL,BYTE PTR [%X]", d);
		break;

	case 0xA1:	// MOV EAX,Ov
		d = get_dword(fp);
		sprintf(ret, "MOV EAX,DWORD PTR [%X]", d);
		break;

	case 0xA2:	// MOV Ob,AL
		d = get_dword(fp);
		sprintf(ret, "MOV BYTE PTR [%X],AL", d);
		break;

	case 0xA3:	// MOV Ov,EAX
		d = get_dword(fp);
		sprintf(ret, "MOV DWORD PTR [%X],EAX", d);
		break;

	case 0xA4:	// MOVS Xb,Yb
		sprintf(ret, "MOVS BYTE PTR [ESI],BYTE PTR [EDI]");
		break;

	case 0xA5:	// MOVS Xv,Yv
		sprintf(ret, "MOVS DWORD PTR [ESI],DWORD PTR [EDI]");
		break;

	case 0xA6:	// CMPS Xb,Yb
		sprintf(ret, "CMPS BYTE PTR [ESI],BYTE PTR [EDI]");
		break;

	case 0xA7:	// CMPS Xv,Yv
		sprintf(ret, "CMPS DWORD PTR [ESI],DWORD PTR [EDI]");
		break;

	case 0xA8:	// TEST AL,Ib
		b = fgetc(fp);
		sprintf(ret, "TEST AL,%X", b);
		break;

	case 0xA9:	// TEST EAX,Iv
		d = get_dword(fp);
		sprintf(ret, "TEST EAX,%X", d);
		break;

	//TODO

	case 0xC3:	// RETN
		sprintf(ret, "RETN");
		break;

	//TODO

	case 0xCC:	// INT 3
		sprintf(ret, "INT 3");
		break;

///OLD

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
			d = 0;
			for (i = 0; i < 4; i++) {
				b = fgetc(fp);
				d |= (b << (i * 8));
			}
			sprintf(ret + strlen(ret), "%X", d);	// Append to string
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
				d = get_dword(fp);
				sprintf(ret, "CALL DWORD PTR [%X]", d);
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
	DWORD d;
	char *tmp;
	char *size = (char *) malloc(8 * sizeof(char));	// BYTE/WORD/DWORD

	switch (bwd) {
	case 'b':
		size = "BYTE";
		break;
	case 'w':
		size = "WORD";
		break;
	case 'd':
		size = "DWORD";
		break;
	}

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
				sprintf(ret, "%s PTR [%s]", size, reg_table(rm, 'd'));
				break;
			case 4:	// Mod = 00, R/M = 100
				// SIB byte follows
				b = fgetc(fp);
				sprintf(ret, "%s PTR [%s]", size, sib_to_str(b));
				break;
			case 5:	// Mod = 00, R/M = 101
				// 32-bit displacement follows
				d = get_dword(fp);
				sprintf(ret, "%s PTR [%X]", size, d);
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
					sprintf(ret, "%s PTR [%s-%X]", size, reg_table(rm, 'd'), ((~b)+1));
				} else {
					sprintf(ret, "%s PTR [%s+%X]", size, reg_table(rm, 'd'), b);
				}
				break;
			case 4:	// Mod = 01, R/M = 100
				// SIB byte follows
				b = fgetc(fp);
				tmp = sib_to_str(b);
				// sign-extended 8-bit displacement follows
				b = fgetc(fp);
				if (b & 0x80) {
					sprintf(ret, "%s PTR [%s-%X]", size, tmp, ((~b)+1));
				} else {
					sprintf(ret, "%s PTR [%s+%X]", size, tmp, b);
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
				d = get_dword(fp);
				sprintf(ret, "%s PTR [%s+%X]", size, reg_table(rm, 'd'), d);
				break;
			case 4:	// Mod = 10, R/M = 100
				// SIB byte follows
				b = fgetc(fp);
				tmp = sib_to_str(b);
				// 32-bit displacement follows
				d = get_dword(fp);
				sprintf(ret, "%s PTR [%s+%X]", size, tmp, d);
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

/* Return next 4 bytes in file as WORD */
WORD get_word(FILE *fp) {
	WORD r = fgetc(fp)
		| (fgetc(fp) << 8);

	return r;
}

/* Return next 4 bytes in file as DWORD */
DWORD get_dword(FILE *fp) {
	DWORD r = fgetc(fp)
		| (fgetc(fp) << 8)
		| (fgetc(fp) << 16)
		| (fgetc(fp) << 24);

	return r;
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
			return "AL";
			break;
		case 1:
			return "CL";
			break;
		case 2:
			return "DL";
			break;
		case 3:
			return "BL";
			break;
		case 4:
			return "AH";
			break;
		case 5:
			return "CH";
			break;
		case 6:
			return "DH";
			break;
		case 7:
			return "BH";
			break;
		}
		break;
	case 'w':
		switch (b) {
		case 0:
			return "AX";
			break;
		case 1:
			return "CX";
			break;
		case 2:
			return "DX";
			break;
		case 3:
			return "BX";
			break;
		case 4:
			return "SP";
			break;
		case 5:
			return "BP";
			break;
		case 6:
			return "SI";
			break;
		case 7:
			return "DI";
			break;
		}
		break;
	case 'd':
		switch (b) {
		case 0:
			return "EAX";
			break;
		case 1:
			return "ECX";
			break;
		case 2:
			return "EDX";
			break;
		case 3:
			return "EBX";
			break;
		case 4:
			return "ESP";
			break;
		case 5:
			return "EBP";
			break;
		case 6:
			return "ESI";
			break;
		case 7:
			return "EDI";
			break;
		}
		break;
	default:
		return "REGERR";
		break;
	}

	return reg;
}

/* Return segment register corresponding to given byte, according to Table B-6 */
char *segreg_table(BYTE reg) {
	switch (reg) {
	case 0:
		return "ES";
		break;
	case 1:
		return "CS";
		break;
	case 2:
		return "SS";
		break;
	case 3:
		return "DS";
		break;
	case 4:
		return "FS";
		break;
	case 5:
		return "GS";
		break;
	default:
		return "REGERR";
		break;
	}
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

/* Return string of corresponding opcode for Group 1, according to Table A-6 */
char *group1_op(BYTE op) {
	switch (op) {
	case 0:
		return "ADD";
		break;
	case 1:
		return "OR";
		break;
	case 2:
		return "ADC";
		break;
	case 3:
		return "SBB";
		break;
	case 4:
		return "AND";
		break;
	case 5:
		return "SUB";
		break;
	case 6:
		return "XOR";
		break;
	case 7:
		return "CMP";
		break;
	default:
		return "REGERR";
		break;
	}
}

#endif
