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

# include <stdio.h>
# include MUT_H
# include MPH_H
# include MLO_H
# include RDS_H
# include RWI_H
# include RUT_H
# include RPR_H
# include RFM_H
# include "mbkgen.h"
# include "G_global.h"
# include "d_fonc.h"
# include "d_codes.h"
# include "d_asm.h"


/* interp.c : partie de l' interpreteur contenant les appels aux instructions */

int (*fonc[256])();

#define NB_VAR_EXT  20
int NbVarExt = NB_VAR_EXT;
VAR TabVarExt[NB_VAR_EXT + 1];

int RempFonc ()
{
  fonc[ADDQI] = addqi;
  fonc[ADDSI] = addsi;
  fonc[ADDHI] = addhi;
  fonc[ADDSF] = addsf;
  fonc[ADDDF] = adddf;

  fonc[SUBQI] = subqi;
  fonc[SUBSI] = subsi;
  fonc[SUBHI] = subhi;
  fonc[SUBSF] = subsf;
  fonc[SUBDF] = subdf;

  fonc[MULQI] = mulqi;
  fonc[MULSI] = mulsi;
  fonc[MULHI] = mulhi;
  fonc[MULSF] = mulsf;
  fonc[MULDF] = muldf;

  fonc[MODQI] = modqi;
  fonc[MODSI] = modsi;
  fonc[MODHI] = modhi;

  fonc[DIVQI] = divqi;
  fonc[DIVSI] = divsi;
  fonc[DIVHI] = divhi;
  fonc[DIVSF] = divsf;
  fonc[DIVDF] = divdf;

  fonc[NEGQI] = negqi;
  fonc[NEGSI] = negsi;
  fonc[NEGHI] = neghi;

  fonc[ASLQI] = aslqi;
  fonc[ASLSI] = aslsi;
  fonc[ASLHI] = aslhi;

  fonc[ASRQI] = asrqi;
  fonc[ASRSI] = asrsi;
  fonc[ASRHI] = asrhi;

  fonc[ANDQI] = andqi;
  fonc[ANDSI] = andsi;
  fonc[ANDHI] = andhi;

  fonc[ORQI] = orqi;
  fonc[ORSI] = orsi;
  fonc[ORHI] = orhi;

  fonc[XORQI] = xorqi;
  fonc[XORSI] = xorsi;
  fonc[XORHI] = xorhi;

  fonc[NOTQI] = notqi;
  fonc[NOTSI] = notsi;
  fonc[NOTHI] = nothi;

  fonc[LSLQI] = lslqi;
  fonc[LSLSI] = lslsi;
  fonc[LSLHI] = lslhi;

  fonc[LSRQI] = lsrqi;
  fonc[LSRSI] = lsrsi;
  fonc[LSRHI] = lsrhi;

  fonc[MOVSI] = movsi;
  fonc[MOVQI] = movqi;
  fonc[MOVHI] = movhi;
  fonc[MOVSF] = movsf;
  fonc[MOVDF] = movdf;

  fonc[NOP] = nop;
  fonc[RET] = ret;
  fonc[CALL] = call;
  fonc[LIGNE] = ligne;
  fonc[INT] = interrupt;
  fonc[BRA] = bra;
  fonc[BNE] = bne;
  fonc[BGT] = bgt;
  fonc[BGTU] = bgtu;
  fonc[BEQ] = beq;
  fonc[BLT] = blt;
  fonc[BLTU] = bltu;
  fonc[BGE] = bge;
  fonc[BGEU] = bgeu;
  fonc[BLE] = ble;
  fonc[BLEU] = bleu;

  fonc[CMPSI] = cmpsi;
  fonc[CMPQI] = cmpqi;
  fonc[CMPHI] = cmphi;
  fonc[CMPSF] = cmpsf;
  fonc[CMPDF] = cmpdf;

  fonc[EXTQISI] = extqisi;
  fonc[EXTQIHI] = extqihi;
  fonc[EXTHISI] = exthisi;
  fonc[EXT0QISI] = ext0qisi;
  fonc[EXT0QIHI] = ext0qihi;
  fonc[EXT0HISI] = ext0hisi;
  fonc[EXTSFDF] = extsfdf;

  fonc[TRUNCSIQI] = truncsiqi;
  fonc[TRUNCSIHI] = truncsihi;
  fonc[TRUNCHIQI] = trunchiqi;
  fonc[TRUNCDFSF] = truncdfsf;

  fonc[FLOATQISF] = floatqisf;
  fonc[FLOATSISF] = floatsisf;
  fonc[FLOATHISF] = floathisf;
  fonc[FLOATQIDF] = floatqidf;
  fonc[FLOATSIDF] = floatsidf;
  fonc[FLOATHIDF] = floathidf;

  fonc[FIXTRUNCSFQI] = fix_truncsfqi;
  fonc[FIXTRUNCSFHI] = fix_truncsfhi;
  fonc[FIXTRUNCSFSI] = fix_truncsfsi;
  fonc[FIXTRUNCDFQI] = fix_truncdfqi;
  fonc[FIXTRUNCDFHI] = fix_truncdfhi;
  fonc[FIXTRUNCDFSI] = fix_truncdfsi;

  fonc[FIXUNSSFQI] = fix_unssfqi;
  fonc[FIXUNSSFHI] = fix_unssfhi;
  fonc[FIXUNSSFSI] = fix_unssfsi;
  fonc[FIXUNSDFQI] = fix_unsdfqi;
  fonc[FIXUNSDFHI] = fix_unsdfhi;
  fonc[FIXUNSDFSI] = fix_unsdfsi;

  TabVarExt[0].var = "_CATA_LIB";
  TabVarExt[0].adr.var = (char *) CATA_LIB;
  TabVarExt[0].type = VAR_DAT;

  TabVarExt[1].var = "_DEBUG_MODE";
  TabVarExt[1].adr.var = (char *) &DEBUG_MODE;
  TabVarExt[1].type = VAR_DAT;

  TabVarExt[2].var = "_FAST_MODE";
  TabVarExt[2].adr.var = (char *) &FAST_MODE;
  TabVarExt[2].type = VAR_DAT;

  TabVarExt[3].var = "_HEAD_CHAIN";
  TabVarExt[3].adr.var = (char *) &HEAD_CHAIN;
  TabVarExt[3].type = VAR_DAT;

  TabVarExt[4].var = "_HEAD_LOFIG";
  TabVarExt[4].adr.ptr = (char **) &HEAD_LOFIG;
  TabVarExt[4].type = VAR_PTR;

  TabVarExt[5].var = "_HEAD_NUM ";
  TabVarExt[5].adr.ptr = (char **) &HEAD_NUM;
  TabVarExt[5].type = VAR_PTR;

  TabVarExt[6].var = "_HEAD_PHFIG";
  TabVarExt[6].adr.ptr = (char **) &HEAD_PHFIG;
  TabVarExt[6].type = VAR_PTR;

  TabVarExt[7].var = "_HEAD_PTYPE";
  TabVarExt[7].adr.ptr = (char **) &HEAD_PTYPE;
  TabVarExt[7].type = VAR_PTR;

  TabVarExt[8].var = "_IN_PH";
  TabVarExt[8].adr.var = (char *) IN_PH;
  TabVarExt[8].type = VAR_DAT;

  TabVarExt[9].var = "_IN_LO";
  TabVarExt[9].adr.var = (char *) IN_LO;
  TabVarExt[9].type = VAR_DAT;

  TabVarExt[10].var = "_OUT_PH";
  TabVarExt[10].adr.var = (char *) OUT_PH;
  TabVarExt[10].type = VAR_DAT;

  TabVarExt[11].var = "_OUT_LO";
  TabVarExt[11].adr.var = (char *) OUT_LO;
  TabVarExt[11].type = VAR_DAT;

  TabVarExt[12].var = "_SCALE_X";
  TabVarExt[12].adr.var = (char *) &SCALE_X;
  TabVarExt[12].type = VAR_DAT;

  TabVarExt[13].var = "_TRACE_MODE";
  TabVarExt[13].adr.var = (char *) &TRACE_MODE;
  TabVarExt[13].type = VAR_DAT;

  TabVarExt[14].var = "_WORK_LIB";
  TabVarExt[14].adr.var = (char *) WORK_LIB;
  TabVarExt[14].type = VAR_DAT;

  TabVarExt[15].var = "_WORK_LOFIG";
  TabVarExt[15].adr.ptr = (char **) &WORK_LOFIG;
  TabVarExt[15].type = VAR_PTR;

  TabVarExt[16].var = "_WORK_PHFIG";
  TabVarExt[16].adr.ptr = (char **) &WORK_PHFIG;
  TabVarExt[16].type = VAR_PTR;

  TabVarExt[17].var = "_WORK_PHINS";
  TabVarExt[17].adr.ptr = (char **) &WORK_PHINS;
  TabVarExt[17].type = VAR_PTR;
#ifndef zantafio_la_plus_bestiale_des_brutes
  {
  char *pwet;
  TabVarExt[18].var = "_pwet";
  TabVarExt[18].adr.var = (char *) pwet;
  TabVarExt[18].type = VAR_DAT;
  }
#else
  TabVarExt[18].var = "__iob";
  TabVarExt[18].adr.var = (char *) _iob;
  TabVarExt[18].type = VAR_DAT;
#endif

  TabVarExt[19].var = "~";
  TabVarExt[19].adr.ptr = NULL;
  TabVarExt[19].type = VAR_PTR;

  TabVarExt[20].var = "_CATAL";
  TabVarExt[20].adr.var = (char *) CATAL;
  TabVarExt[20].type = VAR_DAT;
}
