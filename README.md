# slimdasm

## Description

slimdasm is a minimal x86 disassembler.
Currently works only with executables of the PE file format.
Disassembly instruction output is in Intel format.

## Compiling

Compile with GCC:

```
gcc -o output/slimdasm.exe slimdasm.c utils.c pe.c dasm.c output.c
```

## Usage

```
usage: slimdasm target
```

Interactive commands:

```
q-Quit
n-Next instruction      space-Next 32 instructions
o-Go to OEP
g-Go to address
f-Follow instruction
d-Dump
s-Search for string
e-Edit binary
h-Show help
```

## Example

```
>slimdasm.exe test.exe
EP RVA: 00001280
Code section RVA: 00001000
Data section RVA: 0000A000
Image base: 00400000
Size of code section: 00008C00
Code section offset: 00000400
OEP address: 00401000

:h

q-Quit
n-Next instruction      space-Next 32 instructions
o-Go to OEP
g-Go to address
f-Follow instruction
d-Dump
s-Search for string
e-Edit binary
h-Show help

:n
00401000        PUSH EBX
:n
00401001        SUB ESP,38
:n
00401004        MOV EAX,DWORD PTR [40C588]
:o

EP RVA: 00001280
Code section RVA: 00001000
Data section RVA: 0000A000
Image base: 00400000
Size of code section: 00008C00
Code section offset: 00000400
OEP address: 00401000

:n
00401000        PUSH EBX
:g
Go to address: 40100b
0040100B        JE SHORT 401029
:f
Address of instruction to follow: 40100b
0040100B        JE SHORT 401029
                v
00401029        MOV DWORD PTR [ESP],401110
:s
Search for string: help
0040C534: 68 2D 53 68 6F 77 20 68 65 6C 70 0A    h-Show help.

:d
Address to dump: 40c534
Number of bytes to dump (default 16): 32
0040C534: 68 2D 53 68 6F 77 20 68 65 6C 70 0A 00 41 64 64    h-Show help..Add
0040C544: 72 65 73 73 20 6F 75 74 20 6F 66 20 62 6F 75 6E    ress out of boun
:e
Starting address for editing: 40c534
Input hex bytes: 74
Edited file output\test_edit.exe saved

:q
>
```