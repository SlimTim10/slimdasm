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

	// DEBUGGING show first byte of instruction
	//	printf("%.2X\t", b);

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
	 	sprintf(ret, "DEC %s", reg_table(b, 'd'));
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
	 	sprintf(ret, "POP %s", reg_table(b, 'd'));
		break;

	case 0x60:	// PUSHAD
		sprintf(ret, "PUSHAD");
		break;

	case 0x61:	// POPAD
		sprintf(ret, "POPAD");
		break;

	case 0x62:	// BOUND Gv,Ma
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'G', 'd');
		opa2 = parse_modrm(fp, b, 'E', 'd');
		sprintf(ret, "BOUND %s,%s", opa1, opa2);
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
		sprintf(ret, "PUSH %s", sign8x(b));
		break;

	case 0x6B:	// IMUL Gv,Ev,Ib
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'G', 'd');
		opa2 = parse_modrm(fp, b, 'E', 'd');
		b = fgetc(fp);
		sprintf(ret, "IMUL %s,%s,%s", opa1, opa2, sign8x(b));
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
		sprintf(ret, "JO SHORT %X", curaddr + 2 + (int8_t) b);
		break;

	case 0x71:	// JNO SHORT
		b = fgetc(fp);
		sprintf(ret, "JNO SHORT %X", curaddr + 2 + (int8_t) b);
		break;

	case 0x72:	// JB SHORT
		b = fgetc(fp);
		sprintf(ret, "JB SHORT %X", curaddr + 2 + (int8_t) b);
		break;

	case 0x73:	// JAE SHORT
		b = fgetc(fp);
		sprintf(ret, "JAE SHORT %X", curaddr + 2 + (int8_t) b);
		break;

	case 0x74:	// JE SHORT
		b = fgetc(fp);
		sprintf(ret, "JE SHORT %X", curaddr + 2 + (int8_t) b);
		break;

	case 0x75:	// JNE SHORT
		b = fgetc(fp);
		sprintf(ret, "JNE SHORT %X", curaddr + 2 + (int8_t) b);
		break;

	case 0x76:	// JBE SHORT
		b = fgetc(fp);
		sprintf(ret, "JBE SHORT %X", curaddr + 2 + (int8_t) b);
		break;

	case 0x77:	// JA SHORT
		b = fgetc(fp);
		sprintf(ret, "JA SHORT %X", curaddr + 2 + (int8_t) b);
		break;

	case 0x78:	// JS SHORT
		b = fgetc(fp);
		sprintf(ret, "JS SHORT %X", curaddr + 2 + (int8_t) b);
		break;

	case 0x79:	// JNS SHORT
		b = fgetc(fp);
		sprintf(ret, "JNS SHORT %X", curaddr + 2 + (int8_t) b);
		break;

	case 0x7A:	// JP SHORT
		b = fgetc(fp);
		sprintf(ret, "JP SHORT %X", curaddr + 2 + (int8_t) b);
		break;

	case 0x7B:	// JNP SHORT
		b = fgetc(fp);
		sprintf(ret, "JNP SHORT %X", curaddr + 2 + (int8_t) b);
		break;

	case 0x7C:	// JL SHORT
		b = fgetc(fp);
		sprintf(ret, "JL SHORT %X", curaddr + 2 + (int8_t) b);
		break;

	case 0x7D:	// JGE SHORT
		b = fgetc(fp);
		sprintf(ret, "JGE SHORT %X", curaddr + 2 + (int8_t) b);
		break;

	case 0x7E:	// JLE SHORT
		b = fgetc(fp);
		sprintf(ret, "JLE SHORT %X", curaddr + 2 + (int8_t) b);
		break;

	case 0x7F:	// JG SHORT
		b = fgetc(fp);
		sprintf(ret, "JG SHORT %X", curaddr + 2 + (int8_t) b);
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
		sprintf(ret, "%s %s,%s", op, opa1, sign8x(b));
		break;

	// Immediate Group 1
	case 0x83:	// Ev,Ib
		b = fgetc(fp);
		op = group1_op(get_regop(b));
		opa1 = parse_modrm(fp, b, 'E', 'd');
		b = fgetc(fp);
		sprintf(ret, "%s %s,%s", op, opa1, sign8x(b));
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

	case 0xAA:	// STOS Yb,AL
		sprintf(ret, "STOS BYTE PTR [EDI],AL");
		break;

	case 0xAB:	// STOS Yv,EAX
		sprintf(ret, "STOS BYTE PTR [EDI],EAX");
		break;

	case 0xAC:	// LODS AL,Xb
		sprintf(ret, "LODS AL,BYTE PTR [ESI]");
		break;

	case 0xAD:	// LODS EAX,Xv
		sprintf(ret, "LODS EAX,DWORD PTR [ESI]");
		break;

	case 0xAE:	// SCAS AL,Yb
		sprintf(ret, "SCAS AL,BYTE PTR [EDI]");
		break;

	case 0xAF:	// SCAS EAX,Xv
		sprintf(ret, "LODS EAX,DWORD PTR [ESI]");
		break;

	case 0xB0:	// MOV AL,Ib
	case 0xB1:	// MOV CL,Ib
	case 0xB2:	// MOV DL,Ib
	case 0xB3:	// MOV BL,Ib
	case 0xB4:	// MOV AH,Ib
	case 0xB5:	// MOV CH,Ib
	case 0xB6:	// MOV DH,Ib
	case 0xB7:	// MOV BH,Ib
		opa1 = reg_table(b, 'b');
		b = fgetc(fp);
		sprintf(ret, "MOV %s,%X", opa1, b);
		break;

	case 0xB8:	// MOV EAX,Iv
	case 0xB9:	// MOV ECX,Iv
	case 0xBA:	// MOV EDX,Iv
	case 0xBB:	// MOV EBX,Iv
	case 0xBC:	// MOV ESP,Iv
	case 0xBD:	// MOV EBP,Iv
	case 0xBE:	// MOV ESI,Iv
	case 0xBF:	// MOV EDI,Iv
		opa1 = reg_table(b, 'd');
		d = get_dword(fp);
		sprintf(ret, "MOV %s,%X", opa1, d);
		break;

	// Shift Group 2
	case 0xC0:	// Eb,Ib
		b = fgetc(fp);
		op = group2_op(get_regop(b));
		opa1 = parse_modrm(fp, b, 'E', 'b');
		b = fgetc(fp);
		sprintf(ret, "%s %s,%X", op, opa1, b);
		break;

	// Shift Group 2
	case 0xC1:	// Ev,Ib
		b = fgetc(fp);
		op = group2_op(get_regop(b));
		opa1 = parse_modrm(fp, b, 'E', 'v');
		b = fgetc(fp);
		sprintf(ret, "%s %s,%X", op, opa1, b);
		break;

	case 0xC2:	// RETN Iw
		w = get_word(fp);
		sprintf(ret, "RETN %X", w);
		break;

	case 0xC3:	// RETN
		sprintf(ret, "RETN");
		break;

	case 0xC4:	// LES Gv,Mp
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'G', 'd');
		opa2 = parse_modrm(fp, b, 'E', 'd');
		sprintf(ret, "LES %s,%s", opa1, opa2);
		break;

	case 0xC5:	// LDS Gv,Mp
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'G', 'd');
		opa2 = parse_modrm(fp, b, 'E', 'd');
		sprintf(ret, "LDS %s,%s", opa1, opa2);
		break;

	// Group 11
	case 0xC6:	// MOV Eb,Ib
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'b');
		b = fgetc(fp);
		sprintf(ret, "MOV %s,%X", opa1, b);
		break;

	// Group 11
	case 0xC7:	// MOV Ev,Iv
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'd');
		d = get_dword(fp);
		sprintf(ret, "MOV %s,%X", opa1, d);
		break;

	case 0xC8:	// ENTER Iw,Ib
		w = get_word(fp);
		b = fgetc(fp);
		sprintf(ret, "ENTER %X,%X", w, b);
		break;

	case 0xC9:	// LEAVE
		sprintf(ret, "LEAVE");
		break;

	case 0xCA:	// RETF Iw
		w = get_word(fp);
		sprintf(ret, "RETF %X", w);
		break;

	case 0xCB:	// RETF
		sprintf(ret, "RETF");
		break;

	case 0xCC:	// INT 3
		sprintf(ret, "INT 3");
		break;

	case 0xCD:	// INT Ib
		b = fgetc(fp);
		sprintf(ret, "INT %X", b);
		break;

	case 0xCE:	// INTO
		sprintf(ret, "INTO");
		break;

	case 0xCF:	// IRET
		sprintf(ret, "IRET");
		break;

	// Shift Group 2
	case 0xD0:	// Eb,1
		b = fgetc(fp);
		op = group2_op(get_regop(b));
		opa1 = parse_modrm(fp, b, 'E', 'b');
		sprintf(ret, "%s %s,1", op, opa1);
		break;

	// Shift Group 2
	case 0xD1:	// Ev,1
		b = fgetc(fp);
		op = group2_op(get_regop(b));
		opa1 = parse_modrm(fp, b, 'E', 'd');
		sprintf(ret, "%s %s,1", op, opa1);
		break;

	// Shift Group 2
	case 0xD2:	// Eb,CL
		b = fgetc(fp);
		op = group2_op(get_regop(b));
		opa1 = parse_modrm(fp, b, 'E', 'b');
		sprintf(ret, "%s %s,CL", op, opa1);
		break;

	// Shift Group 2
	case 0xD3:	// Ev,CL
		b = fgetc(fp);
		op = group2_op(get_regop(b));
		opa1 = parse_modrm(fp, b, 'E', 'd');
		sprintf(ret, "%s %s,CL", op, opa1);
		break;

	case 0xD4:	// AAM Ib
		b = fgetc(fp);
		sprintf(ret, "AAM %X", b);
		break;

	case 0xD5:	// AAD Ib
		b = fgetc(fp);
		sprintf(ret, "AAD %X", b);
		break;

	case 0xD6:	// None
		sprintf(ret, "OPCERR");
		break;

	case 0xD7:	// XLAT
		sprintf(ret, "XLAT");
		break;

	case 0xD8:	// Escape code (A.2.6.3)
		b = fgetc(fp);
		if (b <= 0xBF) {
			ext = get_regop(b);
			switch (ext) {
			case 0:
				op = "FADD"; break;
			case 1:
				op = "FMUL"; break;
			case 2:
				op = "FCOM"; break;
			case 3:
				op = "FCOMP"; break;
			case 4:
				op = "FSUB"; break;
			case 5:
				op = "FSUBR"; break;
			case 6:
				op = "FDIV"; break;
			case 7:
				op = "FDIVR"; break;
			}
			opa1 = parse_modrm(fp, b, 'E', 'd');
			sprintf(ret, "%s %s", op, opa1);
		} else {
			if (b >= 0xC0 && b <= 0xC7) sprintf(ret, "FADD ST(0),ST(%d)", (b & 7));
			else if (b >= 0xC8 && b <= 0xCF) sprintf(ret, "FMUL ST(0),ST(%d)", (b & 7));
			else if (b >= 0xD0 && b <= 0xD7) sprintf(ret, "FCOM ST(0),ST(%d)", (b & 7));
			else if (b >= 0xD8 && b <= 0xDF) sprintf(ret, "FMUL ST(0),ST(%d)", (b & 7));
			else if (b >= 0xE0 && b <= 0xE7) sprintf(ret, "FSUB ST(0),ST(%d)", (b & 7));
			else if (b >= 0xE8 && b <= 0xEF) sprintf(ret, "FSUBR ST(0),ST(%d)", (b & 7));
			else if (b >= 0xF0 && b <= 0xF7) sprintf(ret, "FDIV ST(0),ST(%d)", (b & 7));
			else if (b >= 0xF8 && b <= 0xFF) sprintf(ret, "FDIVR ST(0),ST(%d)", (b & 7));
		}
		break;

	case 0xD9:	// Escape code (A.2.6.4)
		b = fgetc(fp);
		if (b <= 0xBF) {
			ext = get_regop(b);
			switch (ext) {
			case 0:
				op = "FLD"; break;
			case 1:
				op = "OPCERR"; break;
			case 2:
				op = "FST"; break;
			case 3:
				op = "FSTP"; break;
			case 4:
				op = "FLDENV"; break;
			case 5:
				op = "FLDCW"; break;
			case 6:
				op = "FSTENV"; break;
			case 7:
				op = "FSTCW"; break;
			}
			opa1 = parse_modrm(fp, b, 'E', 'd');
			sprintf(ret, "%s %s", op, opa1);
		} else {
			if (b >= 0xC0 && b <= 0xC7) sprintf(ret, "FLD ST(0),ST(%d)", (b & 7));
			else if (b == 0xD0) sprintf(ret, "FNOP");
			else if (b == 0xE0) sprintf(ret, "FCHS");
			else if (b == 0xE1) sprintf(ret, "FABS");
			else if (b == 0xE4) sprintf(ret, "FTST");
			else if (b == 0xE5) sprintf(ret, "FXAM");
			else if (b == 0xE8) sprintf(ret, "FLD1");
			else if (b == 0xE9) sprintf(ret, "FLDL2T");
			else if (b == 0xEA) sprintf(ret, "FLDL2E");
			else if (b == 0xEB) sprintf(ret, "FLDPI");
			else if (b == 0xEC) sprintf(ret, "FLDLG2");
			else if (b == 0xED) sprintf(ret, "FLDLN2");
			else if (b == 0xEE) sprintf(ret, "FLDZ");
			else if (b == 0xF0) sprintf(ret, "F2XM1");
			else if (b == 0xF1) sprintf(ret, "FYL2X");
			else if (b == 0xF2) sprintf(ret, "FPTAN");
			else if (b == 0xF3) sprintf(ret, "FPATAN");
			else if (b == 0xF4) sprintf(ret, "FXTRACT");
			else if (b == 0xF5) sprintf(ret, "FPREM1");
			else if (b == 0xF6) sprintf(ret, "FDECSTP");
			else if (b == 0xF7) sprintf(ret, "FINCSTP");
			else if (b == 0xF8) sprintf(ret, "FPREM");
			else if (b == 0xF9) sprintf(ret, "FYL2XP1");
			else if (b == 0xFA) sprintf(ret, "FSQRT");
			else if (b == 0xFB) sprintf(ret, "FSINCOS");
			else if (b == 0xFC) sprintf(ret, "FRNDINT");
			else if (b == 0xFD) sprintf(ret, "FSCALE");
			else if (b == 0xFE) sprintf(ret, "FSIN");
			else if (b == 0xFF) sprintf(ret, "FCOS");
			else sprintf(ret, "OPCERR");
		}
		break;

	case 0xDA:	// Escape code (A.2.6.5)
		b = fgetc(fp);
		if (b <= 0xBF) {
			ext = get_regop(b);
			switch (ext) {
			case 0:
				op = "FIADD"; break;
			case 1:
				op = "FIMUL"; break;
			case 2:
				op = "FICOM"; break;
			case 3:
				op = "FICOMP"; break;
			case 4:
				op = "FISUB"; break;
			case 5:
				op = "FISUBR"; break;
			case 6:
				op = "FIDIV"; break;
			case 7:
				op = "FIDIVR"; break;
			}
			opa1 = parse_modrm(fp, b, 'E', 'd');
			sprintf(ret, "%s %s", op, opa1);
		} else {
			if (b >= 0xC0 && b <= 0xC7) sprintf(ret, "FCMOVB ST(0),ST(%d)", (b & 7));
			else if (b >= 0xC8 && b <= 0xCF) sprintf(ret, "FCMOVE ST(0),ST(%d)", (b & 7));
			else if (b >= 0xD0 && b <= 0xD7) sprintf(ret, "FCMOVBE ST(0),ST(%d)", (b & 7));
			else if (b >= 0xD8 && b <= 0xDF) sprintf(ret, "FCMOVU ST(0),ST(%d)", (b & 7));
			else if (b == 0xE9) sprintf(ret, "FUCOMPP");
			else sprintf(ret, "OPCERR");
		}
		break;

	case 0xDB:	// Escape code (A.2.6.6)
		b = fgetc(fp);
		if (b <= 0xBF) {
			ext = get_regop(b);
			switch (ext) {
			case 0:
				op = "FILD"; break;
			case 1:
				op = "OPCERR"; break;
			case 2:
				op = "FIST"; break;
			case 3:
				op = "FISTP"; break;
			case 4:
				op = "OPCERR"; break;
			case 5:
				op = "FLD"; break;
			case 6:
				op = "OPCERR"; break;
			case 7:
				op = "FSTP"; break;
			}
			opa1 = parse_modrm(fp, b, 'E', 'd');
			sprintf(ret, "%s %s", op, opa1);
		} else {
			if (b >= 0xC0 && b <= 0xC7) sprintf(ret, "FCMOVNB ST(0),ST(%d)", (b & 7));
			else if (b >= 0xC8 && b <= 0xCF) sprintf(ret, "FCMOVNE ST(0),ST(%d)", (b & 7));
			else if (b >= 0xD0 && b <= 0xD7) sprintf(ret, "FCMOVNBE ST(0),ST(%d)", (b & 7));
			else if (b >= 0xD8 && b <= 0xDF) sprintf(ret, "FCMOVNU ST(0),ST(%d)", (b & 7));
			else if (b == 0xE2) sprintf(ret, "FCLEX");
			else if (b == 0xE3) sprintf(ret, "FINIT");
			else if (b >= 0xE8 && b <= 0xEF) sprintf(ret, "FUCOMI ST(0),ST(%d)", (b & 7));
			else if (b >= 0xF0 && b <= 0xF7) sprintf(ret, "FCOMI ST(0),ST(%d)", (b & 7));
			else sprintf(ret, "OPCERR");
		}
		break;

	case 0xDC:	// Escape code (A.2.6.7)
		b = fgetc(fp);
		if (b <= 0xBF) {
			ext = get_regop(b);
			switch (ext) {
			case 0:
				op = "FADD"; break;
			case 1:
				op = "FMUL"; break;
			case 2:
				op = "FCOM"; break;
			case 3:
				op = "FCOMP"; break;
			case 4:
				op = "FSUB"; break;
			case 5:
				op = "FSUBR"; break;
			case 6:
				op = "FDIV"; break;
			case 7:
				op = "FDIVR"; break;
			}
			opa1 = parse_modrm(fp, b, 'E', 'd');
			sprintf(ret, "%s %s", op, opa1);
		} else {
			if (b >= 0xC0 && b <= 0xC7) sprintf(ret, "FADD ST(%d),ST(0)", (b & 7));
			else if (b >= 0xC8 && b <= 0xCF) sprintf(ret, "FMUL ST(%d),ST(0)", (b & 7));
			else if (b >= 0xE0 && b <= 0xE7) sprintf(ret, "FSUBR ST(%d),ST(0)", (b & 7));
			else if (b >= 0xE8 && b <= 0xEF) sprintf(ret, "FSUB ST(%d),ST(0)", (b & 7));
			else if (b >= 0xF0 && b <= 0xF7) sprintf(ret, "FDIVR ST(%d),ST(0)", (b & 7));
			else if (b >= 0xF8 && b <= 0xFF) sprintf(ret, "FDIV ST(%d),ST(0)", (b & 7));
			else sprintf(ret, "OPCERR");
		}
		break;

	case 0xDD:	// Escape code (A.2.6.8)
		b = fgetc(fp);
		if (b <= 0xBF) {
			ext = get_regop(b);
			switch (ext) {
			case 0:
				op = "FILD"; break;
			case 1:
				op = "OPCERR"; break;
			case 2:
				op = "FST"; break;
			case 3:
				op = "FSTP"; break;
			case 4:
				op = "FRSTOR"; break;
			case 5:
				op = "OPCERR"; break;
			case 6:
				op = "FSAVE"; break;
			case 7:
				op = "FSTSW"; break;
			}
			opa1 = parse_modrm(fp, b, 'E', 'd');
			sprintf(ret, "%s %s", op, opa1);
		} else {
			if (b >= 0xC0 && b <= 0xC7) sprintf(ret, "FFREE ST(%d)", (b & 7));
			else if (b >= 0xD0 && b <= 0xD7) sprintf(ret, "FST ST(%d)", (b & 7));
			else if (b >= 0xD8 && b <= 0xDF) sprintf(ret, "FSTP ST(%d)", (b & 7));
			else if (b >= 0xE0 && b <= 0xE7) sprintf(ret, "FUCOM ST(%d),ST(0)", (b & 7));
			else if (b >= 0xE8 && b <= 0xEF) sprintf(ret, "FUCOMP ST(%d)", (b & 7));
			else sprintf(ret, "OPCERR");
		}
		break;

	case 0xDE:	// Escape code (A.2.6.9)
		b = fgetc(fp);
		if (b <= 0xBF) {
			ext = get_regop(b);
			switch (ext) {
			case 0:
				op = "FIADD"; break;
			case 1:
				op = "FIMUL"; break;
			case 2:
				op = "FICOM"; break;
			case 3:
				op = "FICOMP"; break;
			case 4:
				op = "FISUB"; break;
			case 5:
				op = "FISUBR"; break;
			case 6:
				op = "FIDIV"; break;
			case 7:
				op = "FIDIVR"; break;
			}
			opa1 = parse_modrm(fp, b, 'E', 'd');
			sprintf(ret, "%s %s", op, opa1);
		} else {
			if (b >= 0xC0 && b <= 0xC7) sprintf(ret, "FADDP ST(%d),ST(0)", (b & 7));
			else if (b >= 0xC8 && b <= 0xCF) sprintf(ret, "FMULP ST(%d),ST(0)", (b & 7));
			else if (b == 0xD9) sprintf(ret, "FCOMPP");
			else if (b >= 0xE0 && b <= 0xE7) sprintf(ret, "FSUBRP ST(%d),ST(0)", (b & 7));
			else if (b >= 0xE8 && b <= 0xEF) sprintf(ret, "FSUBP ST(%d),ST(0)", (b & 7));
			else if (b >= 0xF0 && b <= 0xF7) sprintf(ret, "FDIVRP ST(%d),ST(0)", (b & 7));
			else if (b >= 0xF8 && b <= 0xFF) sprintf(ret, "FDIVP ST(%d),ST(0)", (b & 7));
			else sprintf(ret, "OPCERR");
		}
		break;

	case 0xDF:	// Escape code (A.2.6.10)
		b = fgetc(fp);
		if (b <= 0xBF) {
			ext = get_regop(b);
			switch (ext) {
			case 0:
				op = "FILD"; break;
			case 1:
				op = "OPCERR"; break;
			case 2:
				op = "FIST"; break;
			case 3:
				op = "FISTP"; break;
			case 4:
				op = "FBLD"; break;
			case 5:
				op = "FILD"; break;
			case 6:
				op = "FBSTP"; break;
			case 7:
				op = "FISTP"; break;
			}
			opa1 = parse_modrm(fp, b, 'E', 'd');
			sprintf(ret, "%s %s", op, opa1);
		} else {
			if (b == 0xE0) sprintf(ret, "FSTSW AX");
			else if (b >= 0xE8 && b <= 0xEF) sprintf(ret, "FUCOMIP ST(0),ST(%d)", (b & 7));
			else if (b >= 0xF0 && b <= 0xF7) sprintf(ret, "FCOMIP ST(0),ST(%d)", (b & 7));
			else sprintf(ret, "OPCERR");
		}
		break;

	case 0xE0:	// LOOPNE Jb
		b = fgetc(fp);
		sprintf(ret, "LOOPNE SHORT %X", curaddr + 2 + (int8_t) b);
		break;

	case 0xE1:	// LOOPE Jb
		b = fgetc(fp);
		sprintf(ret, "LOOPE SHORT %X", curaddr + 2 + (int8_t) b);
		break;

	case 0xE2:	// LOOP Jb
		b = fgetc(fp);
		sprintf(ret, "LOOP SHORT %X", curaddr + 2 + (int8_t) b);
		break;

	case 0xE3:	// JECXZ Jb
		b = fgetc(fp);
		sprintf(ret, "JECXZ SHORT %X", curaddr + 2 + (int8_t) b);
		break;

	case 0xE4:	// IN AL,Ib
		b = fgetc(fp);
		sprintf(ret, "IN AL,%X", b);
		break;

	case 0xE5:	// IN EAX,Ib
		b = fgetc(fp);
		sprintf(ret, "IN EAX,%X", b);
		break;

	case 0xE6:	// OUT Ib,AL
		b = fgetc(fp);
		sprintf(ret, "OUT %X,AL", b);
		break;

	case 0xE7:	// OUT Ib,EAX
		b = fgetc(fp);
		sprintf(ret, "OUT %X,EAX", b);
		break;

	case 0xE8:	// CALL Jv
		d = get_dword(fp);
		sprintf(ret, "CALL %X", curaddr + 5 + (int32_t) d);
		break;

	case 0xE9:	// JMP NEAR Jv
		d = get_dword(fp);
		sprintf(ret, "JMP NEAR %X", curaddr + 5 + (int32_t) d);
		break;

	case 0xEA:	// JMP FAR Ap
		d = get_dword(fp);
		w = get_word(fp);
		sprintf(ret, "JMP FAR %X:%X", w, d);
		break;

	case 0xEB:	// JMP SHORT Jb
		b = fgetc(fp);
		sprintf(ret, "JMP SHORT %X", curaddr + 2 + (int8_t) b);
		break;

	case 0xEC:	// INT AL,DX
		sprintf(ret, "INT AL,DX");
		break;

	case 0xED:	// INT EAX,DX
		sprintf(ret, "INT EAX,DX");
		break;

	case 0xEE:	// OUT DX,AL
		sprintf(ret, "OUT DX,AL");
		break;

	case 0xEF:	// OUT DX,EAX
		sprintf(ret, "OUT DX,EAX");
		break;

	case 0xF0:	// LOCK
		sprintf(ret, "LOCK");
		break;

	case 0xF1:	// None
		sprintf(ret, "OPCERR");
		break;

	case 0xF2:	// REPNE
		sprintf(ret, "REPNE");
		break;

	case 0xF3:	// REP
		sprintf(ret, "REP");
		break;

	case 0xF4:	// HLT
		sprintf(ret, "HLT");
		break;

	case 0xF5:	// CMC
		sprintf(ret, "CMC");
		break;

	// Unary Group 3
	case 0xF6:	// Eb
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'b');
		switch (get_regop(b)) {
		case 0:
			op = "TEST";
			b = fgetc(fp);
			sprintf(ret, "TEST %s,%X", opa1, b);
			break;
		case 2:
			sprintf(ret, "NOT %s", opa1);
			break;
		case 3:
			sprintf(ret, "NEG %s", opa1);
			break;
		case 4:
			sprintf(ret, "MUL %s", opa1);
			break;
		case 5:
			sprintf(ret, "IMUL %s", opa1);
			break;
		case 6:
			sprintf(ret, "DIV %s", opa1);
			break;
		case 7:
			sprintf(ret, "IDIV %s", opa1);
			break;
		default:
			op = "OPCERR";
			break;
		}
		break;

	// Unary Group 3
	case 0xF7:	// Ev
		b = fgetc(fp);
		opa1 = parse_modrm(fp, b, 'E', 'd');
		switch (get_regop(b)) {
		case 0:
			op = "TEST";
			d = get_dword(fp);
			sprintf(ret, "TEST %s,%X", opa1, d);
			break;
		case 2:
			sprintf(ret, "NOT %s", opa1);
			break;
		case 3:
			sprintf(ret, "NEG %s", opa1);
			break;
		case 4:
			sprintf(ret, "MUL %s", opa1);
			break;
		case 5:
			sprintf(ret, "IMUL %s", opa1);
			break;
		case 6:
			sprintf(ret, "DIV %s", opa1);
			break;
		case 7:
			sprintf(ret, "IDIV %s", opa1);
			break;
		default:
			op = "OPCERR";
			break;
		}
		break;

	case 0xF8:	// CLC
		sprintf(ret, "CLC");
		break;

	case 0xF9:	// STC
		sprintf(ret, "STC");
		break;

	case 0xFA:	// CLI
		sprintf(ret, "CLI");
		break;

	case 0xFB:	// STI
		sprintf(ret, "STI");
		break;

	case 0xFC:	// CLD
		sprintf(ret, "CLD");
		break;

	case 0xFD:	// STD
		sprintf(ret, "STD");
		break;

	case 0xFE:	// INC/DEC Group 4
		b = fgetc(fp);
		switch (get_regop(b)) {
		case 0:
			op = "INC";
			break;
		case 1:
			op = "DEC";
			break;
		default:
			op = "OPCERR";
			break;
		}
		opa1 = parse_modrm(fp, b, 'E', 'b');
		sprintf(ret, "%s %s", op, opa1);
		break;

	case 0xFF:	// INC/DEC Group 5
		b = fgetc(fp);
		switch (get_regop(b)) {
		case 0:
			op = "INC";
			break;
		case 1:
			op = "DEC";
			break;
		case 2:
			op = "CALL";
			break;
		case 3:
			op = "CALL FAR";
			break;
		case 4:
			op = "JMP";
			break;
		case 5:
			op = "JMP FAR";
			break;
		case 6:
			op = "PUSH";
			break;
		default:
			op = "OPCERR";
			break;
		}
		opa1 = parse_modrm(fp, b, 'E', 'd');
		sprintf(ret, "%s %s", op, opa1);
		break;

	default:	// Unrecognized instruction
		sprintf(ret, "OPCERR");
		break;

	}

	free(op);
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
					sprintf(ret, "%s PTR [%s-%X]", size, reg_table(rm, 'd'), (BYTE) ((~b)+1));
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
					sprintf(ret, "%s PTR [%s-%X]", size, tmp, (BYTE) ((~b)+1));
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

/* Return string of corresponding opcode for Group 2, according to Table A-6 */
char *group2_op(BYTE op) {
	switch (op) {
	case 0:
		return "ROL";
		break;
	case 1:
		return "ROR";
		break;
	case 2:
		return "RCL";
		break;
	case 3:
		return "RCR";
		break;
	case 4:
		return "SHL";
		break;
	case 5:
		return "SHR";
		break;
	case 7:
		return "SAR";
		break;
	default:
		return "REGERR";
		break;
	}
}
