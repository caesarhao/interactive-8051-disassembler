/************************************************************************
 * Copyright (C) 2020-2021 Matthias Rosenthal
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 ***********************************************************************/
 
#include "disassembler.hpp"
#include <stdio.h>
#include <string.h>
#include <iostream>

#include "../utils/utils.hpp"

namespace disas8051 {

//all 256 op codes
// table created by Collin Kidder
Instruct8051 OpCodes[] =
{
//   name,  bytes  arg1,  arg2,  jmp,   cond-jmp
    {"NOP", 1, NONE, NONE, false, false},           //  0x00
    {"AJMP ", 2, ADDR11, NONE, true, false},        //  0x01
    {"LJMP ", 3, ADDR16, NONE, true, false},        //  0x02
    {"RR A", 1, NONE, NONE, false, false},          //  0x03
    {"INC A", 1, NONE, NONE, false, false},         //  0x04
    {"INC ", 2, DIRECT, NONE, false, false},        //  0x05
    {"INC @R0", 1, NONE, NONE, false, false},       //  0x06
    {"INC @R1", 1, NONE, NONE, false, false},       //  0x07
    {"INC R0", 1, NONE, NONE, false, false},        //  0x08
    {"INC R1", 1, NONE, NONE, false, false},        //  0x09
    {"INC R2", 1, NONE, NONE, false, false},        //  0x0A
    {"INC R3", 1, NONE, NONE, false, false},        //  0x0B
    {"INC R4", 1, NONE, NONE, false, false},        //  0x0C
    {"INC R5", 1, NONE, NONE, false, false},        //  0x0D
    {"INC R6", 1, NONE, NONE, false, false},        //  0x0E
    {"INC R7", 1, NONE, NONE, false, false},        //  0x0F
    {"JBC ", 3, BIT, OFFSET, false, true},          //  0x10
    {"ACALL ", 2, ADDR11, NONE, false, true},       //  0x11
    {"LCALL ", 3, ADDR16, NONE, false, true},       //  0x12
    {"RRC A", 1, NONE, NONE, false, false},         //  0x13
    {"DEC A", 1, NONE, NONE, false, false},         //  0x14
    {"DEC ", 2, DIRECT, NONE, false, false},        //  0x15
    {"DEC @R0", 1, NONE, NONE, false, false},       //  0x16
    {"DEC @R1", 1, NONE, NONE, false, false},       //  0x17
    {"DEC R0", 1, NONE, NONE, false, false},        //  0x18
    {"DEC R1", 1, NONE, NONE, false, false},        //  0x19
    {"DEC R2", 1, NONE, NONE, false, false},        //  0x1A
    {"DEC R3", 1, NONE, NONE, false, false},        //  0x1B
    {"DEC R4", 1, NONE, NONE, false, false},        //  0x1C
    {"DEC R5", 1, NONE, NONE, false, false},        //  0x1D
    {"DEC R6", 1, NONE, NONE, false, false},        //  0x1E
    {"DEC R7", 1, NONE, NONE, false, false},        //  0x1F
    {"JB ", 3, BIT, OFFSET, false, true},           //  0x20
    {"AJMP ", 2, ADDR11, NONE, true, false},        //  0x21
    {"RET", 1, NONE, NONE, false, false},           //  0x22
    {"RL A", 1, NONE, NONE, false, false},          //  0x23
    {"ADD A, ", 2, IMMED, NONE, false, false},      //  0x24
    {"ADD A, ", 2, DIRECT, NONE, false, false},     //  0x25
    {"ADD A, @R0", 1, NONE, NONE, false, false},    //  0x26
    {"ADD A, @R1", 1, NONE, NONE, false, false},    //  0x27
    {"ADD A, R0", 1, NONE, NONE, false, false},     //  0x28
    {"ADD A, R1", 1, NONE, NONE, false, false},     //  0x29
    {"ADD A, R2", 1, NONE, NONE, false, false},     //  0x2A
    {"ADD A, R3", 1, NONE, NONE, false, false},     //  0x2B
    {"ADD A, R4", 1, NONE, NONE, false, false},     //  0x2C
    {"ADD A, R5", 1, NONE, NONE, false, false},     //  0x2D
    {"ADD A, R6", 1, NONE, NONE, false, false},     //  0x2E
    {"ADD A, R7", 1, NONE, NONE, false, false},     //  0x2F
    {"JNB ", 3, BIT, OFFSET, false, true},          //  0x30
    {"ACALL ", 2, ADDR11, NONE, false, true},       //  0x31
    {"RETI", 1, NONE, NONE, false, false},          //  0x32
    {"RLC A", 1, NONE, NONE, false, false},         //  0x33
    {"ADDC A, ", 2, IMMED, NONE, false, false},     //  0x34
    {"ADDC A, ", 2, DIRECT, NONE, false, false},    //  0x35
    {"ADDC A, @R0", 1, NONE, NONE, false, false},   //  0x36
    {"ADDC A, @R1", 1, NONE, NONE, false, false},   //  0x37
    {"ADDC A, R0", 1, NONE, NONE, false, false},    //  0x38
    {"ADDC A, R1", 1, NONE, NONE, false, false},    //  0x39
    {"ADDC A, R2", 1, NONE, NONE, false, false},    //  0x3A
    {"ADDC A, R3", 1, NONE, NONE, false, false},    //  0x3B
    {"ADDC A, R4", 1, NONE, NONE, false, false},    //  0x3C
    {"ADDC A, R5", 1, NONE, NONE, false, false},    //  0x3D
    {"ADDC A, R6", 1, NONE, NONE, false, false},    //  0x3E
    {"ADDC A, R7", 1, NONE, NONE, false, false},    //  0x3F
    {"JC ", 2, OFFSET, NONE, false, true},          //  0x40
    {"AJMP ", 2, ADDR11, NONE, true, false},        //  0x41
    {"ORL ", 2, DIRECT, A, false, false},           //  0x42
    {"ORL ", 3, DIRECT, IMMED, false, false},       //  0x43
    {"ORL A, ", 2, IMMED, NONE, false, false},      //  0x44
    {"ORL A, ", 2, DIRECT, NONE, false, false},     //  0x45
    {"ORL A, @R0", 1, NONE, NONE, false, false},    //  0x46
    {"ORL A, @R1", 1, NONE, NONE, false, false},    //  0x47
    {"ORL A, R0", 1, NONE, NONE, false, false},     //  0x48
    {"ORL A, R1", 1, NONE, NONE, false, false},     //  0x49
    {"ORL A, R2", 1, NONE, NONE, false, false},     //  0x4A
    {"ORL A, R3", 1, NONE, NONE, false, false},     //  0x4B
    {"ORL A, R4", 1, NONE, NONE, false, false},     //  0x4C
    {"ORL A, R5", 1, NONE, NONE, false, false},     //  0x4D
    {"ORL A, R6", 1, NONE, NONE, false, false},     //  0x4E
    {"ORL A, R7", 1, NONE, NONE, false, false},     //  0x4F
    {"JNC ", 2, OFFSET, NONE, false, true},         //  0x50    
    {"ACALL ", 2, ADDR11, NONE, false, true},       //  0x51
    {"ANL ", 2, DIRECT, A, false, false},           //  0x52
    {"ANL ", 3, DIRECT, IMMED, false, false},       //  0x53
    {"ANL A, ", 2, IMMED, NONE, false, false},      //  0x54
    {"ANL A, ", 2, DIRECT, NONE, false, false},     //  0x55
    {"ANL A, @R0", 1, NONE, NONE, false, false},    //  0x56
    {"ANL A, @R1", 1, NONE, NONE, false, false},    //  0x57
    {"ANL A, R0", 1, NONE, NONE, false, false},     //  0x58
    {"ANL A, R1", 1, NONE, NONE, false, false},     //  0x59
    {"ANL A, R2", 1, NONE, NONE, false, false},     //  0x5A
    {"ANL A, R3", 1, NONE, NONE, false, false},     //  0x5B
    {"ANL A, R4", 1, NONE, NONE, false, false},     //  0x5C
    {"ANL A, R5", 1, NONE, NONE, false, false},     //  0x5D
    {"ANL A, R6", 1, NONE, NONE, false, false},     //  0x5E
    {"ANL A, R7", 1, NONE, NONE, false, false},     //  0x5F
    {"JZ ", 2, OFFSET, NONE, false, true},          //  0x60
    {"AJMP ", 2, ADDR11, NONE, true, false},        //  0x61
    {"XRL ", 2, DIRECT, A, false, false},           //  0x62
    {"XRL ", 3, DIRECT, IMMED, false, false},       //  0x63
    {"XRL A, ", 2, IMMED, NONE, false, false},      //  0x64
    {"XRL A, ", 2, DIRECT, NONE, false, false},     //  0x65
    {"XRL A, @R0", 1, NONE, NONE, false, false},    //  0x66
    {"XRL A, @R1", 1, NONE, NONE, false, false},    //  0x67
    {"XRL A, R0", 1, NONE, NONE, false, false},     //  0x68
    {"XRL A, R1", 1, NONE, NONE, false, false},     //  0x69
    {"XRL A, R2", 1, NONE, NONE, false, false},     //  0x6A
    {"XRL A, R3", 1, NONE, NONE, false, false},     //  0x6B
    {"XRL A, R4", 1, NONE, NONE, false, false},     //  0x6C
    {"XRL A, R5", 1, NONE, NONE, false, false},     //  0x6D
    {"XRL A, R6", 1, NONE, NONE, false, false},     //  0x6E
    {"XRL A, R7", 1, NONE, NONE, false, false},     //  0x6F
    {"JNZ ", 2, OFFSET, NONE, false, true},         //  0x70
    {"ACALL ", 2, ADDR11, NONE, false, true},       //  0x71
    {"ORL C, ", 2, BIT, NONE, false, false},        //  0x72
    {"JMP @A+DPTR", 1, NONE, NONE, false, false},   //  0x73 //yes, this really is a jump instr. but this program can't decode DPTR so don't follow it.
    {"MOV A, ", 2, IMMED, NONE, false, false},      //  0x74
    {"MOV ", 3, DIRECT, IMMED, false, false},       //  0x75
    {"MOV @R0, ", 2, IMMED, NONE, false, false},    //  0x76
    {"MOV @R1, ", 2, IMMED, NONE, false, false},    //  0x77
    {"MOV R0, ", 2, IMMED, NONE, false, false},     //  0x78
    {"MOV R1, ", 2, IMMED, NONE, false, false},     //  0x79
    {"MOV R2, ", 2, IMMED, NONE, false, false},     //  0x7A
    {"MOV R3, ", 2, IMMED, NONE, false, false},     //  0x7B
    {"MOV R4, ", 2, IMMED, NONE, false, false},     //  0x7C
    {"MOV R5, ", 2, IMMED, NONE, false, false},     //  0x7D
    {"MOV R6, ", 2, IMMED, NONE, false, false},     //  0x7E
    {"MOV R7, ", 2, IMMED, NONE, false, false},     //  0x7F
    {"SJMP ", 2, OFFSET, NONE, true, false},        //  0x80
    {"AJMP ", 2, ADDR11, NONE, true, false},        //  0x81
    {"ANL C, ", 2, BIT, NONE, false, false},        //  0x82
    {"MOVC A, @A+PC", 1, NONE, NONE, false, false}, //  0x83
    {"DIV AB", 1, NONE, NONE, false, false},        //  0x84
    {"MOV ", 3, DIRECT, DIRECT, false, false},      //  0x85
    {"MOV ", 2, DIRECT, IR0, false, false},         //  0x86
    {"MOV ", 2, DIRECT, IR1, false, false},         //  0x87
    {"MOV ", 2, DIRECT, R0, false, false},          //  0x88
    {"MOV ", 2, DIRECT, R1, false, false},          //  0x89
    {"MOV ", 2, DIRECT, R2, false, false},          //  0x8A
    {"MOV ", 2, DIRECT, R3, false, false},          //  0x8B
    {"MOV ", 2, DIRECT, R4, false, false},          //  0x8C
    {"MOV ", 2, DIRECT, R5, false, false},          //  0x8D
    {"MOV ", 2, DIRECT, R6, false, false},          //  0x8E
    {"MOV ", 2, DIRECT, R7, false, false},          //  0x8F
    {"MOV DPTR, ", 3, IMMED16, NONE, false, false}, //  0x90
    {"ACALL ", 2, ADDR11, NONE, false, true},       //  0x91
    {"MOV ", 2, BIT, C, false, false},              //  0x92
    {"MOVC A, @A+DPTR", 1, NONE, NONE, false, false},  //  0x93
    {"SUBB A, ", 2, IMMED, NONE, false, false},     //  0x94
    {"SUBB A, ", 2, DIRECT, NONE, false, false},    //  0x95
    {"SUBB A, @R0", 1, NONE, NONE, false, false},   //  0x96
    {"SUBB A, @R1", 1, NONE, NONE, false, false},   //  0x97
    {"SUBB A, R0", 1, NONE, NONE, false, false},    //  0x98
    {"SUBB A, R1", 1, NONE, NONE, false, false},    //  0x99
    {"SUBB A, R2", 1, NONE, NONE, false, false},    //  0x9A
    {"SUBB A, R3", 1, NONE, NONE, false, false},    //  0x9B
    {"SUBB A, R4", 1, NONE, NONE, false, false},    //  0x9C
    {"SUBB A, R5", 1, NONE, NONE, false, false},    //  0x9D
    {"SUBB A, R6", 1, NONE, NONE, false, false},    //  0x9E
    {"SUBB A, R7", 1, NONE, NONE, false, false},    //  0x9F
    {"ORL C, ", 2, BIT, NONE, false, false},        //  0xA0
    {"AJMP ", 2, ADDR11, NONE, true, false},        //  0xA1
    {"MOV C, ", 2, BIT, NONE, false, false},        //  0xA2
    {"INC DPTR", 1, NONE, NONE, false, false},      //  0xA3
    {"MUL AB", 1, NONE, NONE, false, false},        //  0xA4
    {"RESERVED", 1, NONE, NONE, false, false},      //  0xA5
    {"MOV @R0, ", 2, DIRECT, NONE, false, false},   //  0xA6
    {"MOV @R1, ", 2, DIRECT, NONE, false, false},   //  0xA7
    {"MOV R0, ", 2, DIRECT, NONE, false, false},    //  0xA8
    {"MOV R1, ", 2, DIRECT, NONE, false, false},    //  0xA9
    {"MOV R2, ", 2, DIRECT, NONE, false, false},    //  0xAA
    {"MOV R3, ", 2, DIRECT, NONE, false, false},    //  0xAB
    {"MOV R4, ", 2, DIRECT, NONE, false, false},    //  0xAC
    {"MOV R5, ", 2, DIRECT, NONE, false, false},    //  0xAD
    {"MOV R6, ", 2, DIRECT, NONE, false, false},    //  0xAE
    {"MOV R7, ", 2, DIRECT, NONE, false, false},    //  0xAF
    {"ANL C, ", 2, BIT, NONE, false, false},        //  0xB0
    {"ACALL ", 2, ADDR11, NONE, false, true},       //  0xB1
    {"CPL ", 2, BIT, NONE, false, false},           //  0xB2
    {"CPL C", 1, NONE, NONE, false, false},         //  0xB3
    {"CJNE A, ", 3, IMMED, OFFSET, false, true},    //  0xB4
    {"CJNE A, ", 3, DIRECT, OFFSET, false, true},   //  0xB5
    {"CJNE @R0, ", 3, IMMED, OFFSET, false, true},  //  0xB6
    {"CJNE @R1, ", 3, IMMED, OFFSET, false, true},  //  0xB7
    {"CJNE R0, ", 3, IMMED, OFFSET, false, true},   //  0xB8
    {"CJNE R1, ", 3, IMMED, OFFSET, false, true},   //  0xB9
    {"CJNE R2, ", 3, IMMED, OFFSET, false, true},   //  0xBA
    {"CJNE R3, ", 3, IMMED, OFFSET, false, true},   //  0xBB
    {"CJNE R4, ", 3, IMMED, OFFSET, false, true},   //  0xBC
    {"CJNE R5, ", 3, IMMED, OFFSET, false, true},   //  0xBD
    {"CJNE R6, ", 3, IMMED, OFFSET, false, true},   //  0xBE
    {"CJNE R7, ", 3, IMMED, OFFSET, false, true},   //  0xBF
    {"PUSH ", 2, DIRECT, NONE, false, false},       //  0xC0
    {"AJMP ", 2, ADDR11, NONE, true, false},        //  0xC1
    {"CLR ", 2, BIT, NONE, false, false},           //  0xC2
    {"CLR C", 1, NONE, NONE, false, false},         //  0xC3
    {"SWAP A", 1, NONE, NONE, false, false},        //  0xC4
    {"XCH A, ", 2, DIRECT, NONE, false, false},     //  0xC5
    {"XCH A, @R0", 1, NONE, NONE, false, false},    //  0xC6
    {"XCH A, @R1", 1, NONE, NONE, false, false},    //  0xC7
    {"XCH A, R0", 1, NONE, NONE, false, false},     //  0xC8
    {"XCH A, R1", 1, NONE, NONE, false, false},     //  0xC9
    {"XCH A, R2", 1, NONE, NONE, false, false},     //  0xCA
    {"XCH A, R3", 1, NONE, NONE, false, false},     //  0xCB
    {"XCH A, R4", 1, NONE, NONE, false, false},     //  0xCC
    {"XCH A, R5", 1, NONE, NONE, false, false},     //  0xCD
    {"XCH A, R6", 1, NONE, NONE, false, false},     //  0xCE
    {"XCH A, R7", 1, NONE, NONE, false, false},     //  0xCF
    {"POP ", 2, DIRECT, NONE, false, false},        //  0xD0
    {"ACALL ", 2, ADDR11, NONE, false, true},       //  0xD1
    {"SETB ", 2, BIT, NONE, false, false},          //  0xD2
    {"SETB C", 1, NONE, NONE, false, false},        //  0xD3
    {"DA A", 1, NONE, NONE, false, false},          //  0xD4
    {"DJNZ ", 3, DIRECT, OFFSET, false, true},      //  0xD5
    {"XCHD A, @R0", 1, NONE, NONE, false, false},   //  0xD6
    {"XCHD A, @R1", 1, NONE, NONE, false, false},   //  0xD7
    {"DJNZ R0, ", 2, OFFSET, NONE, false, true},    //  0xD8
    {"DJNZ R1, ", 2, OFFSET, NONE, false, true},    //  0xD9
    {"DJNZ R2, ", 2, OFFSET, NONE, false, true},    //  0xDA
    {"DJNZ R3, ", 2, OFFSET, NONE, false, true},    //  0xDB
    {"DJNZ R4, ", 2, OFFSET, NONE, false, true},    //  0xDC
    {"DJNZ R5, ", 2, OFFSET, NONE, false, true},    //  0xDD
    {"DJNZ R6, ", 2, OFFSET, NONE, false, true},    //  0xDE
    {"DJNZ R7, ", 2, OFFSET, NONE, false, true},    //  0xDF
    {"MOVX A, @DPTR", 1, NONE, NONE, false, false}, //  0xE0
    {"AJMP ", 2, ADDR11, NONE, true, false},        //  0xE1
    {"MOVX, A, @R0", 1, NONE, NONE, false, false},  //  0xE2
    {"MOVX, A, @R1", 1, NONE, NONE, false, false},  //  0xE3
    {"CLR A", 1, NONE, NONE, false, false},         //  0xE4
    {"MOV A, ", 2, DIRECT, NONE, false, false},     //  0xE5
    {"MOV A, @R0", 1, NONE, NONE, false, false},    //  0xE6
    {"MOV A, @R1", 1, NONE, NONE, false, false},    //  0xE7
    {"MOV A, R0", 1, NONE, NONE, false, false},     //  0xE8
    {"MOV A, R1", 1, NONE, NONE, false, false},     //  0xE9
    {"MOV A, R2", 1, NONE, NONE, false, false},     //  0xEA
    {"MOV A, R3", 1, NONE, NONE, false, false},     //  0xEB
    {"MOV A, R4", 1, NONE, NONE, false, false},     //  0xEC
    {"MOV A, R5", 1, NONE, NONE, false, false},     //  0xED
    {"MOV A, R6", 1, NONE, NONE, false, false},     //  0xEE
    {"MOV A, R7", 1, NONE, NONE, false, false},     //  0xEF
    {"MOVX @DPTR, A", 1, NONE, NONE, false, false}, //  0xF0
    {"ACALL ", 2, ADDR11, NONE, false, true},       //  0xF1
    {"MOVX @R0, A", 1, NONE, NONE, false, false},   //  0xF2
    {"MOVX @R1, A", 1, NONE, NONE, false, false},   //  0xF3
    {"CPL A", 1, NONE, NONE, false, false},         //  0xF4
    {"MOV ", 2, DIRECT, A, false, false},           //  0xF5
    {"MOV @R0, A", 1, NONE, NONE, false, false},    //  0xF6
    {"MOV @R1, A", 1, NONE, NONE, false, false},    //  0xF7
    {"MOV R0, A", 1, NONE, NONE, false, false},     //  0xF8
    {"MOV R1, A", 1, NONE, NONE, false, false},     //  0xF9
    {"MOV R2, A", 1, NONE, NONE, false, false},     //  0xFA
    {"MOV R3, A", 1, NONE, NONE, false, false},     //  0xFB
    {"MOV R4, A", 1, NONE, NONE, false, false},     //  0xFC
    {"MOV R5, A", 1, NONE, NONE, false, false},     //  0xFD
    {"MOV R6, A", 1, NONE, NONE, false, false},     //  0xFE
    {"MOV R7, A", 1, NONE, NONE, false, false}      //  0xFF
};

//Starting at 0x80 and going through 0xFF these are the SFR's for a ENE KB9012
const char *SFR[] = 
{
"P0",   "SP",   "DPL",  "DPH",  "x84",  "x85",  "PCON2","PCON", //80-87
"TCON", "TMOD", "TL0",  "TL1",  "TH0",  "TH1",  "x8e",  "x8f",  //88-8F
"P1",   "x91",  "x92",  "x93",  "x94",  "x95",  "x96",  "x97",  //90-87
"SCON", "SBUF", "SCON2","SCON3","SCON4","x9d",  "x9e",  "x9f",  //98-8F
"P2",   "xa1",  "xa2",  "xa3",  "xa4",  "xa5",  "xa6",  "xa7",  //a0-a7
"IE",   "xa9",  "xaa",  "xab",  "xac",  "xad",  "xae",  "xaf",  //a8-af
"P3",   "xb1",  "xb2",  "xb3",  "xb4",  "xb5",  "xb6",  "xb7",  //b0-b7
"IP",   "xb9",  "xba",  "xbb",  "xbc",  "xbd",  "xbe",  "xbf",  //b8-bf
"xc0",  "xc1",  "xc2",  "xc3",  "xc4",  "xc5",  "xc6",  "xc7",  //c0-c7
"T2CON","xc9",  "xca",  "xcb",  "xcc",  "TL2",  "TH2",  "xcf",  //c8-cf
"PSW",  "xd1",  "xd2",  "xd3",  "xd4",  "xd5",  "xd6",  "xd7",  //d0-d7
"P0IF", "xd9",  "xda",  "xdb",  "xdc",  "xdd",  "xde",  "xdf",  //d8-df
"ACC",  "xe1",  "xe2",  "xe3",  "xe4",  "xe5",  "xe6",  "xe7",  //e0-e7
"P1IF", "xe9",  "xea",  "xeb",  "xec",  "xed",  "xee",  "xef",  //e8-ef
"B",    "xf1",  "xf2",  "xf3",  "xf4",  "xf5",  "xf6",  "xf7",  //f0-f7
"P3IF", "xf9",  "xfa",  "xfb",  "xfc",  "xfd",  "xfe",  "xff"   //f8-ff
};

Disassembler::Disassembler(logger::logger_base *logger)
{
	this->logger = logger;
	
    // SFR name check
    for(int i = 0; i < 128; i++) {
        if(SFR[i][0] == 'x') {
            std::string correctHex = fn_FormatToHex(ustring(1, (uint8_t)(i+128)), true);
            if(correctHex != SFR[i] + 1) {
                logger->LogWarning(std::string("SFR for ") + correctHex + std::string(" does not match its name ") + SFR[i]);
            }
        }
    }
}

int32_t Disassembler::resolveAddress(int32_t rawAddress, uint32_t currentAddress)
{
	if(currentAddress >= 0x1C000) {
		if(rawAddress < 0x4000) {
			return rawAddress + 0x1C000;
		}
	}
	else if(currentAddress >= 0x18000) {
		if(rawAddress < 0x4000) {
			return rawAddress + 0x18000;
		}
		// else: e. g. library call on function in regular area
	}
	else if(currentAddress >= 0x14000) {
		if(rawAddress < 0x4000) {
			return rawAddress + 0x14000;
		}
		// else: e. g. library call on function in regular area
	}
	else if(currentAddress >= 0x10000) {
		if(rawAddress < 0x4000) {
			return rawAddress + 0x10000;
		}
		// else: e. g. library call on function in regular area
	}
	return rawAddress;
}

int Disassembler::interpretArgument(int32_t *outInt, const uint8_t *inBuffer, int opType, uint32_t address, int op)
{
    int32_t tempInt = 0;
    int offset = 0;
    int opSize = OpCodes[op].length;
    switch(opType)
    {
    case NONE:
        offset = 0;
        tempInt = 0;
        break;
    case ADDR11: //take top 5 bits of PC (of following instruction), fill in bottom with these 11 bits.
        tempInt = (((uint16_t)(op & 0xE0)) << 3) + inBuffer[0] + ((address + opSize) & 0x0FF800);
        offset = 1;
        break;
    case ADDR16: //direct 16 bit address with no offsets
        tempInt = resolveAddress(((uint16_t)inBuffer[0] << 8) + inBuffer[1], address);
        offset = 2;
        break;
	case DIRECT: //direct 8 bit RAM or SFR
        tempInt = inBuffer[0];
        offset = 1;
        break;
    case IMMED: //an 8 bit literal
        tempInt = inBuffer[0];
        offset = 1;
        break;
    case IMMED16: //a 16 bit literal
        tempInt = ((uint16_t)inBuffer[0] << 8) + inBuffer[1];
        offset = 2;
        break;
    case BIT: //a bit from one of the bitfield locations
        tempInt = inBuffer[0];
        offset = 1;        
        break;
    case OFFSET: //an offset from the end of this current instruction
        tempInt = address + opSize + (signed char)(inBuffer[0]);
        offset = 1;
        break;
    default:
        tempInt = 0;
        offset = 0;
    }
    *outInt = tempInt;

    return offset;
}

Instruction Disassembler::disassembleInstruction(const ustring &buf, uint64_t address)
{
    int offset = 0;
    Instruction op;

    op.opNum = buf[address];
    op.length = OpCodes[op.opNum].length;
    op.arg1Type = OpCodes[op.opNum].arg1;
    op.arg2Type = OpCodes[op.opNum].arg2;
    op.isJump = OpCodes[op.opNum].isJump;
    op.isCondJump = OpCodes[op.opNum].isCondJump;

    if(address + op.length > buf.length()) {
        logger->LogWarning("disassembled nothing on address " + utils::Int_To_String_Hex(address) + " because no more data");
        op.opNum = NONE;
        return op;
    }

    offset = interpretArgument(&op.arg1, buf.data() + address + 1, op.arg1Type, address, op.opNum);
    offset = interpretArgument(&op.arg2, buf.data() + address + 1 + offset, op.arg2Type, address, op.opNum);

    if (op.arg1Type == ADDR11 || op.arg1Type == ADDR16 || op.arg1Type == OFFSET) {
        op.address = op.arg1;
    }
    if (op.arg2Type == ADDR11 || op.arg2Type == ADDR16 || op.arg2Type == OFFSET) {
        op.address = op.arg2;
    }

    return op;
}

void Disassembler::disassembleSwitchStatement(const ustring &buf, uint64_t callAddress, uint64_t startAddress, bool isI16Switch, Disassembly *disassembly)
{
	SwitchStatement switchStatement;
	switchStatement.address = callAddress;
	switchStatement.isI16 = isI16Switch;
	
	logger->LogInformation("Disassembling switch on address " + utils::Int_To_String_Hex(callAddress));
	uint64_t caseWidth = isI16Switch ? 4 : 3;
	while(startAddress + caseWidth <= buf.length()) {
		if((buf[startAddress] == 0) && (buf[startAddress+1] == 0)) {
			// default case
			uint64_t address = resolveAddress(((uint16_t)buf[startAddress+2] << 8) + buf[startAddress+3], startAddress);
			//std::cout << "default goto " << utils::Int_To_String_Hex(address) << std::endl;
			switchStatement.cases.push_back(SwitchStatement::Case{SwitchStatement::DEFAULT_CASE_VALUE, address});
			followCodePath(buf, address, disassembly);
			break;
		}
		else {
			uint64_t address = resolveAddress(((uint16_t)buf[startAddress] << 8) + buf[startAddress+1], startAddress);
			int32_t value;
			if(isI16Switch) {
				value = ((uint16_t)buf[startAddress+2] << 8) + buf[startAddress+3];
			}
			else {
				value = buf[startAddress+2];
			}
			//std::cout << "case " << value << " goto " << utils::Int_To_String_Hex(address) << std::endl;
			switchStatement.cases.push_back(SwitchStatement::Case{value, address});
			followCodePath(buf, address, disassembly);
		}
		startAddress += caseWidth;
	}
	
	int index = disassembly->switchStatements.size();
	for(const SwitchStatement::Case &caseBlock : switchStatement.cases) {
		if(disassembly->caseBlocks.find(caseBlock.address) != disassembly->caseBlocks.end()) {
			if(disassembly->caseBlocks[caseBlock.address] == index) {
				continue; // This is a case block for multiple case values
			}
			logger->LogWarning("Case block for multiple switch statements on address " + utils::Int_To_String_Hex(caseBlock.address));
		}
		disassembly->caseBlocks[caseBlock.address] = index;
	}
	disassembly->switchStatements.push_back(switchStatement);
}

void Disassembler::followCodePath(const ustring &buf, uint64_t startAddress, Disassembly *disassembly)
{
    Instruction currentOp;

#ifdef DEBUG
    printf("Start of code path: %x\n", startAddress);
#endif

    while (true)
    {
        if(startAddress > buf.length()) {
            logger->LogWarning("Got to invalid address " + utils::Int_To_String_Hex(startAddress) + " outside data");
            return;
        }
        
        if(disassembly->ignoreBlocks.find(startAddress) != disassembly->ignoreBlocks.end()) {
			std::cout << "ignoring from " << utils::Int_To_String_Hex(startAddress) << " on " << std::endl;
			return;
		}

		auto itOtherInstruction = disassembly->instructions.find(startAddress);
        if(itOtherInstruction != disassembly->instructions.end()) {
			// do not parse this block again
			return;
        }

        //decode the next instruction
        currentOp = disassembleInstruction(buf, startAddress);
        
        // I first tried auto itOp = instructions.begin(); while(itOp->first<startAddress) itOp++; 
        // but this was suprisingly much slower than the following (see comment at file bottom for old code)
        for(int iByte = 0; iByte < currentOp.length; iByte++) {
			if(disassembly->instructions.find(startAddress + iByte) != disassembly->instructions.end()) {
				logger->LogWarning("Refusing to disassemble instruction on " + utils::Int_To_String_Hex(startAddress) + 
						" because it overlaps with instruction on " + utils::Int_To_String_Hex(startAddress + iByte));
				return;
			}
		}
		disassembly->instructions[startAddress] = currentOp;

        //if this instruction could possibly jump then take that jump
        if (currentOp.isCondJump || currentOp.isJump) {
            auto it = disassembly->jumpTargets.find(currentOp.address);
            if(it == disassembly->jumpTargets.end()) {
                disassembly->jumpTargets[currentOp.address] = std::vector<uint64_t>{startAddress};
            }
            else {
                it->second.push_back(startAddress);
            }
            followCodePath(buf, currentOp.address, disassembly);
            
            auto itFunction = disassembly->functions.find(currentOp.address);
            if(itFunction != disassembly->functions.end()) {
				if(itFunction->second.name.find("CCASE") != std::string::npos) {
					disassembleSwitchStatement(buf, startAddress, startAddress + currentOp.length, false, disassembly);
					break; // the CCASE/ICASE function drops the return address and jumps to the case 
					// statement -> the code does not necesarrily continue after the switch statement
				}
				else if(itFunction->second.name.find("ICASE") != std::string::npos) {
					disassembleSwitchStatement(buf, startAddress, startAddress + currentOp.length, true, disassembly);
					break;
				}
			}
        }

        //there are a variety of reasons we might quit going forward here.
        //perhaps this is a non-conditional jump. In that case we can't go anymore
        //or, perhaps we hit a RET or RETI instruction. So, check for all of the above
        if (currentOp.isJump || (currentOp.getName() == "RET") || (currentOp.getName() == "RETI") || (currentOp.getName() == "JMP @A+DPTR")) {
#ifdef DEBUG
            printf("End of code path at: %x\n", startAddress);
#endif
            return;
        }
              
        startAddress += currentOp.length;
    }
}

} // namespace disas8051

/* old code to check for overlapping
// this is somehow much slower than disassembly->instructions.find(startAddress).
auto itOtherInstruction = disassembly->instructions.begin();
while((itOtherInstruction != disassembly->instructions.end()) && (itOtherInstruction->first < startAddress)) {
	itOtherInstruction++;
}
if(itOtherInstruction != disassembly->instructions.end()) {
	if(itOtherInstruction->first == startAddress) {
		// do not parse this block again
		return;
	}
}

//decode the next instruction
currentOp = disassembleInstruction(buf, startAddress);

if(itOtherInstruction != disassembly->instructions.end()) {
	if(itOtherInstruction->first < startAddress + currentOp.length) {
		logger->LogWarning("Refusing to disassemble instruction on " + utils::Int_To_String_Hex(startAddress) + 
				" because it overlaps with instruction on " + utils::Int_To_String_Hex(itOtherInstruction->first));
		return;
	}
}
disassembly->instructions[startAddress] = currentOp;
*/
