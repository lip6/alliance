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

/*



	prototypes des fonctions de traitement dees instructions

*/


int adddf() ;
int addhi() ;
int addqi() ;
int addsf() ;
int addsi() ;
int andhi() ;
int andqi() ;
int andsi() ;
int aslhi() ;
int aslqi() ;
int aslsi() ;
int asrhi() ;
int asrqi() ;
int asrsi() ;
int beq() ;
int bge() ;
int bgeu() ;
int bgt() ;
int bgtu() ;
int ble() ;
int bleu() ;
int blt() ;
int bltu() ;
int bne() ;
int bra() ;
int call() ;
int cmpdf() ;
int cmphi() ;
int cmpqi() ;
int cmpsf() ;
int cmpsi() ;
int divdf() ;
int divhi() ;
int divqi() ;
int divsf() ; 
int divsi() ;
int exthisi() ;
int extqihi() ;
int extqisi() ;
int extsfdf() ;
int ext0hisi() ;
int ext0qihi() ;
int ext0qisi() ;
int fix_truncdfhi() ;
int fix_truncdfqi() ;
int fix_truncdfsi() ;
int fix_truncsfhi() ;
int fix_truncsfqi() ;
int fix_truncsfsi() ;
int fix_unsdfhi() ;
int fix_unsdfqi() ;
int fix_unsdfsi() ;
int fix_unssfhi() ;
int fix_unssfqi() ;
int fix_unssfsi() ;
int floathidf() ; 
int floathisf() ; 
int floatqidf() ; 
int floatqisf() ; 
int floatsidf() ; 
int floatsisf() ; 
int lslhi() ;
int lslqi() ;
int lslsi() ;
int lsrhi() ;
int lsrqi() ;
int lsrsi() ;
int moddf() ;
int modhi() ;
int modsi() ;
int modqi() ;
int modsf() ;
int movdf() ;
int movhi() ;
int movqi() ;
int movsf() ;
int movsi() ;
int muldf() ;
int mulsf() ;
int mulhi() ;
int mulqi() ;
int mulsi() ;
int neghi() ;
int negqi() ;
int negsi() ;
int nop() ;
int nothi() ;
int notqi() ;
int notsi() ;
int orhi() ;
int orqi() ;
int orsi() ;
int ret() ;
int subdf() ;
int subhi() ;
int subqi() ;
int subsf() ;
int subsi() ;
int truncdfsf() ;
int trunchiqi() ;
int trunchisi() ;
int truncsihi() ;
int truncsiqi() ;
int xorhi() ;
int xorqi() ;
int xorsi() ;
/* ############### interp_oper.c ############### */

int dest();
double oper();
void save();

/* ############## interp_br.c ################## */

int ligne ();
int interrupt() ;
