/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
 * 
 * This progam is  free software; you can redistribute it  and/or modify it
 * under the  terms of the GNU  General Public License as  published by the
 * Free Software Foundation;  either version 2 of the License,  or (at your
 * option) any later version.
 * 
 * Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* Binary codes :
    They correspond to assembler mnemonics of the machine and are used to
    access the functions table while interpreting. */
#define ADDQI          1
#define ADDSI          2
#define ADDHI          3
#define ADDSF          4
#define ADDDF          5
 
#define SUBQI          6
#define SUBSI          7
#define SUBHI          8
#define SUBSF          9
#define SUBDF          10
 
#define MULQI          11
#define MULSI          12
#define MULHI          13
#define MULSF          14
#define MULDF          15
 
#define DIVQI          16
#define DIVSI          17
#define DIVHI          18
#define DIVSF          19
#define DIVDF          20
 
#define MODQI          21
#define MODSI          22
#define MODHI          23
#define MODSF          24
#define MODDF          25
 
#define ASLQI          26
#define ASLSI          27
#define ASLHI          28
#define ASLSF          29
#define ASLDF          30
 
#define ASRQI          31
#define ASRSI          32
#define ASRHI          33
#define ASRSF          34
#define ASRDF          35
 
#define NEGQI          40
#define NEGSI          41
#define NEGHI          42
 
#define ANDQI          100
#define ANDSI          101
#define ANDHI          102

#define ORQI           103
#define ORSI           104
#define ORHI           105

#define XORQI          106
#define XORSI          107
#define XORHI          108

#define LSLQI          109
#define LSLSI          110
#define LSLHI          111

#define LSRQI          112
#define LSRSI          113
#define LSRHI          114

#define NOTQI          115
#define NOTSI          116
#define NOTHI          117
 
#define CMPQI          120
#define CMPSI          121
#define CMPHI          122
#define CMPSF          123
#define CMPDF          124

#define BRA            129
#define BNE            130
#define BGT            131
#define BGTU           132
#define BEQ            133
#define BLT            134
#define BLTU           135
#define BGE            136
#define BGEU           137
#define BLE            138
#define BLEU           139

#define CALL           140
#define INT            141
#define RET            142

#define NOP            143

#define MOVQI          150
#define MOVSI          151
#define MOVHI          152
#define MOVSF          153
#define MOVDF          154

#define LIGNE          200

#define EXTQISI        160  
#define EXTQIHI        161
#define EXTHISI        162
#define EXT0QISI       163
#define EXT0QIHI       164
#define EXT0HISI       165
#define EXTSFDF        166

#define TRUNCSIQI      167
#define TRUNCSIHI      168
#define TRUNCHIQI      169
#define TRUNCDFSF      170

#define FLOATQISF      171
#define FLOATSISF      172
#define FLOATHISF      173
#define FLOATQIDF      174
#define FLOATSIDF      175
#define FLOATHIDF      176

#define FIXTRUNCSFQI   177
#define FIXTRUNCSFHI   178
#define FIXTRUNCSFSI   179
#define FIXTRUNCDFQI   180
#define FIXTRUNCDFHI   181
#define FIXTRUNCDFSI   182

#define FIXUNSSFQI     183
#define FIXUNSSFHI     184
#define FIXUNSSFSI     185
#define FIXUNSDFQI     186
#define FIXUNSDFHI     187
#define FIXUNSDFSI     188

/* addressing mode :
   They fit on the 3 leftmost bits of a char. */
#define MODE_IMM       0x00
#define MODE_DIRECT    0x20
#define MODE_REG       0xA0
#define MODE_IND_REG   0xC0
#define MODE_IND_REG_D 0xE0

