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
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


/* ###--------------------------------------------------------------### */
/* file		: mips_defs.h						*/
/* date		: Dec  6 1999						*/
/* version	: v0.3							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* description	: MIPS assembler - defines (operation codes)		*/
/* ###--------------------------------------------------------------### */

#define MPS_PLSDFN	0		/* add				*/
#define MPS_MNSDFN	1		/* subtract			*/
#define MPS_SHLDFN	2		/* shift left			*/
#define MPS_SHRDFN	3		/* shift right			*/
#define MPS_ORDFN 	4		/* logical or			*/
#define MPS_ANDDFN	5		/* logical and			*/
#define MPS_XORDFN	6		/* logical exclusive or		*/
#define MPS_MULDFN	7		/* multiply			*/
#define MPS_DIVDFN	8		/* divide			*/

	/* ### ---------------------------------------------------- ### */
	/* instruction set table:					*/
	/*    Opcods in   lower case         are MIPS instructions	*/
	/*    Opcods in   upper case         are application specific	*/
	/*								*/
	/*								*/
	/* primary opcod (31 downto 26):				*/
	/*      |  0     1     2     3     4     5     6     7		*/
	/*    --+-----+-----+-----+-----+-----+-----+-----+-----+	*/
	/*    0 |speci|bcond|  j  | jal | beq | bne |blez |bgtz |	*/
	/*    1 |addi |addui|slti |sltui|andi | ori |xori | lui |	*/
	/*    2 |cop0 |  +  |  +  |  +  |     |  +  |     |     |	*/
	/*    3 |     |     |     |     |     |     |     |     |	*/
	/*    4 | lb  | lh  |  +  | lw  | lbu | lhu |  +  |SWAP |	*/
	/*    5 | sb  | sh  |  +  | sw  |     |     |  +  |     |	*/
	/*    6 |  +  |  +  |  +  |  +  |Seqi |Snei |Slei |Sgti |	*/
	/*    7 |  +  |  +  |  +  |  +  |Sgei |Sgeui|Sleui|Sgtui|	*/
	/*								*/
	/*								*/
	/* special opcod extension (5 downto 0):			*/
	/*      |  0     1     2     3     4     5     6     7		*/
	/*    --+-----+-----+-----+-----+-----+-----+-----+-----+	*/
	/*    0 | sll |     | srl | sra |sllv |     |srlv |srav |	*/
	/*    1 | jr  |jalr |     |     |sysca|break|     |SLEEP|	*/
	/*    2 |mfhi |mthi |mflo |mtlo |     |     |     |     |	*/
	/*    3 |  +  |  +  |  +  |  +  |     |     |     |     |	*/
	/*    4 | add |addu | sub |subu | and | or  | xor | nor |	*/
	/*    5 |     |     | slt |sltu |     |     |     |     |	*/
	/*    6 |     |     |     |     |  +  |  +  |  +  |  +  |	*/
	/*    7 |     |     |     |     |  +  |  +  |  +  |  +  |	*/
	/*								*/
	/*								*/
	/* bcond opcod extension (20 downto 16):			*/
	/*      |  0     1     2     3     4     5     6     7		*/
	/*    --+-----+-----+-----+-----+-----+-----+-----+-----+	*/
	/*    0 |bltz |bgez |     |     |     |     |     |     |	*/
	/*    1 |     |     |     |     |     |     |     |     |	*/
	/*    2 |bltza|bgeza|     |     |     |     |     |     |	*/
	/*    3 |     |     |     |     |     |     |     |     |	*/
	/*								*/
	/*								*/
	/* cop0 opcod extension (22, 21, 16, 25, 24, 23):		*/
	/*      |  0     1     2     3     4     5     6     7		*/
	/*    --+-----+-----+-----+-----+-----+-----+-----+-----+	*/
	/*    0 | mf  | mt  |  +  |  +  | c0  | c0  | c0  | c0  |	*/
	/*    1 | mf  | mt  |  +  |  +  | c0  | c0  | c0  | c0  |	*/
	/*    2 |     |     |     |     | c0  | c0  | c0  | c0  |	*/
	/*    3 |     |     |     |     | c0  | c0  | c0  | c0  |	*/
	/*    4 |  +  |  +  |     |     | c0  | c0  | c0  | c0  |	*/
	/*    5 |  +  |  +  |     |     | c0  | c0  | c0  | c0  |	*/
	/*    6 |     |     |     |     | c0  | c0  | c0  | c0  |	*/
	/*    7 |     |     |     |     | c0  | c0  | c0  | c0  |	*/
	/*								*/
	/*								*/
	/* c0 cop0 extension extension (4 downto 0):			*/
	/*      |  0     1     2     3     4     5     6     7		*/
	/*    --+-----+-----+-----+-----+-----+-----+-----+-----+	*/
	/*    0 |  +  |  +  |  +  |     |     |     |  +  |     |	*/
	/*    1 |  +  |     |     |     |     |     |     |     |	*/
	/*    2 |rstfe|     |     |     |     |     |     |     |	*/
	/*    3 |  +  |     |     |     |     |     |     |     |	*/
	/*								*/
	/* ### ---------------------------------------------------- ### */

#define _ADD		0x00000020	/* Mips architecture		*/
#define _ADDI		0x20000000	/* Mips architecture		*/
#define _ADDU		0x00000021	/* Mips architecture		*/
#define _ADDIU		0x24000000	/* Mips architecture		*/
#define _AND		0x00000024	/* Mips architecture		*/
#define _ANDI		0x30000000	/* Mips architecture		*/
#define _BEQ		0x10000000	/* Mips architecture		*/
#define _BGEZ		0x04010000	/* Mips architecture		*/
#define _BGEZAL		0x04110000	/* Mips architecture		*/
#define _BGTZ		0x1C000000	/* Mips architecture		*/
#define _BLEZ		0x18000000	/* Mips architecture		*/
#define _BLTZ		0x04000000	/* Mips architecture		*/
#define _BLTZAL		0x04100000	/* Mips architecture		*/
#define _BNE		0x14000000	/* Mips architecture		*/
#define _BREAK		0x0000000D	/* Mips architecture		*/
#define _J		0x08000000	/* Mips architecture		*/
#define _JAL		0x0C000000	/* Mips architecture		*/
#define _JALR		0x00000009	/* Mips architecture		*/
#define _JR		0x00000008	/* Mips architecture		*/
#define _LB		0x80000000	/* Mips architecture		*/
#define _LBU		0x90000000	/* Mips architecture		*/
#define _LH		0x84000000	/* Mips architecture		*/
#define _LHU		0x94000000	/* Mips architecture		*/
#define _LUI		0x3C000000	/* Mips architecture		*/
#define _LW		0x8C000000	/* Mips architecture		*/
#define _MFC0		0x40000000	/* Mips architecture		*/
#define _MFHI		0x00000010	/* Mips architecture		*/
#define _MFLO		0x00000012	/* Mips architecture		*/
#define _MTC0		0x40800000	/* Mips architecture		*/
#define _MTHI		0x00000011	/* Mips architecture		*/
#define _MTLO		0x00000013	/* Mips architecture		*/
#define _NOR		0x00000027	/* Mips architecture		*/
#define _OR		0x00000025	/* Mips architecture		*/
#define _ORI		0x34000000	/* Mips architecture		*/
#define _RSTFE		0x42000010	/* Mips architecture		*/
#define _SB		0xA0000000	/* Mips architecture		*/
#define _SH		0xA4000000	/* Mips architecture		*/
#define _SLEEP		0x0000000F	/* Mips architecture		*/
#define _SLL		0x00000000	/* Mips architecture		*/
#define _SLLV		0x00000004	/* Mips architecture		*/
#define _SLT		0x0000002A	/* Mips architecture		*/
#define _SLTI		0x28000000	/* Mips architecture		*/
#define _SLTU		0x0000002B	/* Mips architecture		*/
#define _SLTIU		0x2C000000	/* Mips architecture		*/
#define _SRA		0x00000003	/* Mips architecture		*/
#define _SRAV		0x00000007	/* Mips architecture		*/
#define _SRL		0x00000002	/* Mips architecture		*/
#define _SRLV		0x00000006	/* Mips architecture		*/
#define _SUB		0x00000022	/* Mips architecture		*/
#define _SUBU		0x00000023	/* Mips architecture		*/
#define _SW		0xAC000000	/* Mips architecture		*/
#define _SWAP		0x9C000000	/* Mips architecture		*/
#define _SYSCALL	0x0000000C	/* Mips architecture		*/
#define _XOR		0x00000026	/* Mips architecture		*/
#define _XORI		0x38000000	/* Mips architecture		*/

#define _NOP		0x00000001	/* Mips macro			*/
#define _LOADI		0x00000002	/* Mips macro			*/

#define _BADVADDR	 8
#define _STATUS		12
#define _CAUSE		13
#define _EPC		14
