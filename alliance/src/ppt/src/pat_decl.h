/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
 * 
 * This library is free software; you  can redistribute it and/or modify it
 * under the terms  of the GNU Library General Public  License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
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
/* file		: pat_decl.h						*/
/* date		: Mar  9 2000						*/
/* version	: v109							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* content	: parser-driver level library				*/
/* ###--------------------------------------------------------------### */

static struct paseq    *PAT_SEQPNT  ;
static struct pagrp    *PAT_GRPPNT  ;
static struct paiol    *PAT_IOLPNT  ;
static struct pacom    *PAT_COMPNT  ;
static struct pains    *PAT_INSPNT  ;

static unsigned int     STT_NBR     ;
static unsigned short   DCL_NBR     ;
static char             ERR_FLG     ;

static short            TAB_IDX     ;
static struct array     TAB    [256];
static short            NAM_IDX     ;
static char            *NAMTAB [ 30];
static struct entry    *ENTRY_HEAD  = NULL;
static struct entry   **HSHTAB      ;
