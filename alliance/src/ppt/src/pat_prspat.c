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
/* file		: pat_prspat.c						*/
/* date		: Mar 10 2000						*/
/* version	: v109							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* content	: parser-driver functions				*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "pat.h"
#include "ppt.h"

/* ###--------------------------------------------------------------### */
/* function	: pat_prspat						*/
/* description	: parse a file in pat format and return a PASEQ		*/
/* called func.	: namealloc, pat_declpat, pat_descpat			*/
/* ###--------------------------------------------------------------### */

struct paseq *pat_prspat (fp, name, ptseq, maxpat, mode)

FILE          *fp    ;		/* pointer of file to be parsed		*/
char          *name  ;		/* name of the new sequence		*/
struct paseq  *ptseq ;		/* list of existing sequences		*/
unsigned int   maxpat;		/* # of pattern in a sub-sequence	*/
unsigned char  mode  ;		/* complier mode			*/

  {
  extern struct paseq *pat_declpat ();
  extern struct paseq *pat_descpat ();

  if ((ptseq == NULL) || (ptseq->ENDFLG == 'Y'))
    {
	/* ###------------------------------------------------------### */
	/* begin parsing a new pattern file				*/
	/* ###------------------------------------------------------### */

    ptseq = pat_declpat (fp, ptseq);

    if ((ptseq != NULL) && (ptseq->ERRFLG == 0))
      {
      ptseq->NAME = namealloc (name);
      ptseq       = pat_descpat (fp, ptseq, maxpat, mode);
      }

    }
  else
    {
	/* ###------------------------------------------------------### */
	/* continue parsing						*/
	/* ###------------------------------------------------------### */

    ptseq = pat_descpat (fp, ptseq, maxpat, mode);
    }

  return (ptseq);
  }
