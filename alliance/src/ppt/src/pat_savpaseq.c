/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
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
/* file		: pat_savpaseq.c					*/
/* date		: Aug 25 1997						*/
/* version	: v107							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* content	: parser-driver functions				*/
/* ###--------------------------------------------------------------### */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mut.h"
#include "pat.h"
#include "ppt.h"

/* ###--------------------------------------------------------------### */
/* function	: pat_savpaseq						*/
/* description	: save a sequence of patterns into a pattern file	*/
/* called func.	: makfopen  , pat_error, pat_warning, pat_drvpat	*/
/* ###--------------------------------------------------------------### */

int pat_savpaseq (name, ptseq, labelsiz)

char         *name    ;		/* name of the drived file		*/
struct paseq *ptseq   ;		/* list of existing sequences		*/
unsigned int  labelsiz;		/* length of the longest label		*/

  {
  static FILE *fp      = NULL;
  static char *sfx     = NULL;
  char        *str     ;
  int          err_flg = 0   ;

	/* ###------------------------------------------------------### */
	/*    read the environment variable VH_PATSFX to get the	*/
	/* pattern file extension.					*/
	/* ###------------------------------------------------------### */

  if (sfx == NULL)
    {
    if ((str = mbkgetenv ("VH_PATSFX")) != NULL)
      sfx = strtok (str, ":");
    else
      sfx = "pat";
    }

  if (ptseq != NULL)
    {
	/* ###------------------------------------------------------### */
	/*    if the sequence exist ...					*/
	/* open a new file if it is the first call for the current	*/
	/* sequence. Produce a warning message if the previous file	*/
	/* has not been closed						*/
	/* ###------------------------------------------------------### */

    if (ptseq->DRVSEQ == 0)
      {
      if (fp != NULL)
        pat_warning (1, "driver", ' ', 0);
 
      if ((fp = mbkfopen (name, sfx, WRITE_TEXT)) == NULL)
        err_flg = pat_error (102, name, ' ', 0);
      }

    if (fp != NULL)
      {
	/* ###------------------------------------------------------### */
	/*    parse the openned file. Close the file when the end is	*/
	/* reached.							*/
	/* ###------------------------------------------------------### */

      err_flg = pat_drvpat (fp, ptseq, labelsiz);

      if (ptseq->ENDFLG == 'Y')
        {
        fclose (fp);
        fp = NULL;
        }
      }
    }

  return (err_flg);
  }
