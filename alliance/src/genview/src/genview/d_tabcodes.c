/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
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

/* Associative table that indicates machine language value for memomnique.
   Needed at assembly time. */

# include "d_asm.h"
# include "d_codes.h"

int NbTabCodes = NB_TAB_CODES;

SYMB TabCodes[NB_TAB_CODES] =
{
    "add.df", ADDDF, 0,
    "add.hi", ADDHI, 0,
    "add.qi", ADDQI, 0,
    "add.sf", ADDSF, 0,
    "add.si", ADDSI, 0,
    "and.hi", ANDHI, 0,
    "and.qi", ANDQI, 0,
    "and.si", ANDSI, 0,
    "asl.hi", ASLHI, 0,
    "asl.qi", ASLQI, 0,
    "asl.si", ASLSI, 0,
    "asr.hi", ASRHI, 0,
    "asr.qi", ASRQI, 0,
    "asr.si", ASRSI, 0,
    "beq", BEQ, 0,
    "bge", BGE, 0,
    "bgeu", BGEU, 0,
    "bgt", BGT, 0,
    "bgtu", BGTU, 0,
    "ble", BLE, 0,
    "bleu", BLEU, 0,
    "blt", BLT, 0,
    "bltu", BLTU, 0,
    "bne", BNE, 0,
    "bra", BRA, 0,
    "call", CALL, 0,
    "cmp.df", CMPDF, 0,
    "cmp.hi", CMPHI, 0,
    "cmp.qi", CMPQI, 0,
    "cmp.sf", CMPSF, 0,
    "cmp.si", CMPSI, 0,
    "div.df", DIVDF, 0,
    "div.hi", DIVHI, 0,
    "div.qi", DIVQI, 0,
    "div.sf", DIVSF, 0,
    "div.si", DIVSI, 0,
    "ext.hi.si", EXTHISI, 0,
    "ext.qi.hi", EXTQIHI, 0,
    "ext.qi.si", EXTQISI, 0,
    "ext.sf.df", EXTSFDF, 0,
    "ext0.hi.si", EXT0HISI, 0,
    "ext0.qi.hi", EXT0QIHI, 0,
    "ext0.qi.si", EXT0QISI, 0,
    "fix_trunc.df.hi", FIXTRUNCDFHI, 0,
    "fix_trunc.df.qi", FIXTRUNCDFQI, 0,
    "fix_trunc.df.si", FIXTRUNCDFSI, 0,
    "fix_trunc.sf.hi", FIXTRUNCSFHI, 0,
    "fix_trunc.sf.qi", FIXTRUNCSFQI, 0,
    "fix_trunc.sf.si", FIXTRUNCSFSI, 0,
    "fixuns.df.hi", FIXUNSDFHI, 0,
    "fixuns.df.qi", FIXUNSDFQI, 0,
    "fixuns.df.si", FIXUNSDFSI, 0,
    "fixuns.sf.hi", FIXUNSSFHI, 0,
    "fixuns.sf.qi", FIXUNSSFQI, 0,
    "fixuns.sf.si", FIXUNSSFSI, 0,
    "float.hi.df", FLOATHIDF, 0,
    "float.hi.sf", FLOATHISF, 0,
    "float.qi.df", FLOATQIDF, 0,
    "float.qi.sf", FLOATQISF, 0,
    "float.si.df", FLOATSIDF, 0,
    "float.si.sf", FLOATSISF, 0,
    "lsl.hi", LSLHI, 0,
    "lsl.qi", LSLQI, 0,
    "lsl.si", LSLSI, 0,
    "lsr.hi", LSRHI, 0,
    "lsr.qi", LSRQI, 0,
    "lsr.si", LSRSI, 0,
    "mod.df", MODDF, 0,
    "mod.hi", MODHI, 0,
    "mod.qi", MODQI, 0,
    "mod.sf", MODSI, 0,
    "mov.df", MOVDF, 0,
    "mov.hi", MOVHI, 0,
    "mov.qi", MOVQI, 0,
    "mov.sf", MOVSF, 0,
    "mov.si", MOVSI, 0,
    "mul.df", MULDF, 0,
    "mul.hi", MULHI, 0,
    "mul.qi", MULQI, 0,
    "mul.sf", MULSF, 0,
    "mul.si", MULSI, 0,
    "neg.hi", NEGHI, 0,
    "neg.qi", NEGQI, 0,
    "neg.si", NEGSI, 0,
    "nop", NOP, 0,
    "not.hi", NOTHI, 0,
    "not.qi", NOTQI, 0,
    "not.si", NOTSI, 0,
    "or.hi", ORHI, 0,
    "or.qi", ORQI, 0,
    "or.si", ORSI, 0,
    "ret", RET, 0,
    "sub.df", SUBDF, 0,
    "sub.hi", SUBHI, 0,
    "sub.qi", SUBQI, 0,
    "sub.sf", SUBSF, 0,
    "sub.si", SUBSI, 0,
    "trunc.df.sf", TRUNCDFSF, 0,
    "trunc.hi.qi", TRUNCHIQI, 0,
    "trunc.si.hi", TRUNCSIHI, 0,
    "trunc.si.qi", TRUNCSIQI, 0,
    "xor.hi", XORHI, 0,
    "xor.qi", XORQI, 0,
    "xor.si", XORSI, 0
};
