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
/* file		: pat_lodpaseq.c					*/
/* date		: Mar 10 2000						*/
/* version	: v109							*/
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
/* function	: pat_lodpaseq						*/
/* description	: load a sequence of patterns from a pattern file	*/
/* called func.	: makfopen, pat_error, pat_warning, pat_prspat		*/
/* ###--------------------------------------------------------------### */

struct paseq *pat_lodpaseq (name, ptseq, maxpat, mode)

char          *name  ;		/* name of the new sequence		*/
struct paseq  *ptseq ;		/* list of existing sequences		*/
unsigned int   maxpat;		/* # of pattern in a sub-sequence	*/
unsigned char  mode  ;		/* compiler mode			*/

  {
  static FILE         *fp      = NULL;
  static struct chain *sfx_lst = NULL;
  struct chain        *sfx_pnt ;
  char                *str     ;
  char                *tok     ;

  if ((ptseq == NULL) || (ptseq->ENDFLG == 'Y'))
    {
	/* ###------------------------------------------------------### */
	/*    read the environment variable VH_PATSFX to get the	*/
	/* pattern file extensions.					*/
	/* ###------------------------------------------------------### */

    if (sfx_lst == NULL)
      {
      if ((str = mbkgetenv ("VH_PATSFX")) != NULL)
        {
        tok = strtok (str, ":");
        while (tok != NULL)
          {
          sfx_lst = addchain (sfx_lst, tok);
          tok     = strtok (NULL, ":");
          }

        sfx_lst = (struct chain *) reverse (sfx_lst);
        }
      else
        sfx_lst = addchain (NULL, "pat");
      }

	/* ###------------------------------------------------------### */
	/*    open a new file. If the previous file has not been closed	*/
	/* produce a warning message.					*/
	/* ###------------------------------------------------------### */

    if (fp != NULL)
      pat_warning (1, "parser", ' ', 0);

    sfx_pnt = sfx_lst;
    while (sfx_pnt != NULL)
      {
      if ((fp = mbkfopen (name, sfx_pnt->DATA, READ_TEXT)) != NULL)
        break;
      sfx_pnt = sfx_pnt->NEXT;
      }

    if (fp == NULL)
      pat_error (102, name, ' ', 0);
    }

  if (fp != NULL)
    {
	/* ###------------------------------------------------------### */
	/*    parse the openned file. Close the file when the end is	*/
	/* reached or if an error has occured.				*/
	/* ###------------------------------------------------------### */

    ptseq = pat_prspat (fp, name, ptseq, maxpat, mode);

    if ((ptseq == NULL) || (ptseq->ENDFLG == 'Y') || (ptseq->ERRFLG != 0))
      {
      fclose (fp);
      fp = NULL;
      }
    }

  return (ptseq);
  }
