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
/* file		: pat_addpaseq.c					*/
/* date		: Mar  8 2000						*/
/* version	: v109							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level pat function				*/
/* ###--------------------------------------------------------------### */

#include "mut.h"
#include "pat.h"

/* ###--------------------------------------------------------------### */
/* function	: pat_addpaseq						*/
/* description	: create an empty PASEQ (sequence of patterns)		*/
/*		  structure at the top of a list			*/
/* called func.	: mbkalloc						*/
/* ###--------------------------------------------------------------### */

struct paseq *pat_addpaseq (lastpaseq)

struct paseq *lastpaseq;	/* pointer on the last paseq structure	*/

  {
  struct paseq  *ptseq   ;
  unsigned char  err_flg = 0;

  ptseq            = (struct paseq *) mbkalloc (sizeof(struct paseq));

  ptseq->NAME      = NULL        ;
  ptseq->PAGRP     = NULL        ;
  ptseq->PAIOL     = NULL        ;
  ptseq->DECCOM    = NULL        ;
  ptseq->OLDPAT    = NULL        ;
  ptseq->OLDCOM    = NULL        ;
  ptseq->CURPAT    = NULL        ;
  ptseq->CURCOM    = NULL        ;
  ptseq->NXTPAT    = NULL        ;
  ptseq->MODEL     = NULL        ;
  ptseq->BUFFER    = NULL        ;
  ptseq->PAINS     = NULL        ;
  ptseq->PATNBR    = 0           ;
  ptseq->IOLNBR    = 0           ;
  ptseq->SUBSEQ    = 0           ;
  ptseq->LINENO    = 1           ;
  ptseq->SAVFLG    = 'N'         ;
  ptseq->ENDFLG    = 'N'         ;
  ptseq->DRVSEQ    = 0           ;
  ptseq->ERRFLG    = err_flg     ;
  ptseq->FLAGS     = 0           ;
  ptseq->TIME_UNIT = PAT_TIMEUNIT;
  ptseq->TIME_STEP = PAT_TIMESTEP;

  ptseq->NEXT      = lastpaseq   ;

  return (ptseq);
  }
