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

/* ###--------------------------------------------------------------### */
/* file		: mips_yacc.h						*/
/* date		: Dec  9 1999						*/
/* version	: v0.3							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* description	: MIPS assembler - external and static variables and	*/
/*		  functions						*/
/* ###--------------------------------------------------------------### */

extern int                MPS_LINNUM       ; 

static int                MPS_PASNBR       ;
static int                MPS_ERRFLG       ;

static FILE              *MPS_TXTFIL = NULL;
static unsigned long      MPS_TXTADR = 0   ;
static int                MPS_TXTSIZ = 0   ;

static FILE              *MPS_DATFIL = NULL;
static unsigned long      MPS_DATADR = 0   ;
static int                MPS_DATSIZ = 0   ;

static char              *MPS_LABEL  = NULL;

static FILE              *MPS_WRTFIL = NULL;
static FILE              *MPS_SYMFIL = NULL;

static unsigned long      MPS_ADDRES       ;
static int                MPS_ADRSIZ = 0   ;
static int                MPS_WRDSIZ = 0   ;
static char               MPS_ADRSPC = 'T' ;
static char               MPS_SYMBOL = 'n' ;
static char               MPS_DUMP   = 'n' ;

static unsigned char      MPS_BYTTAB [1024];

static struct beden     **MPS_HSHTAB = NULL;

static void               mips_error    ();
static void               mips_head     ();
static void               mips_foot     ();
static void               mips_print    ();
static void               mips_symbol   ();
static int                mips_strtobin ();
