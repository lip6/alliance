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
/* file		: pat_getusage.c					*/
/* date		: Apr 22 1996						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: high level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "pat.h"
#include "phl.h"

void pat_getusage (ptseq, ptuse)

struct paseq *ptseq;
struct pause *ptuse;

  {
  struct paseq  *seq_pnt;
  struct paiol  *iol_pnt;
  struct pagrp  *grp_pnt;
  struct papat  *pat_pnt;
  struct paevt  *evt_pnt;
  struct paini  *ini_pnt;
  struct pacom  *com_pnt;
  struct pains  *ins_pnt;

	/* ###------------------------------------------------------### */
	/*    count pattern sequences					*/
	/* ###------------------------------------------------------### */

  seq_pnt = ptseq;
  while (seq_pnt != NULL)
    {

    if (seq_pnt->SUBSEQ == 1)
      {
      ptuse->PASEQ++;

      if (seq_pnt->NAME != NULL)
        ptuse->CHARACTER += strlen (seq_pnt->NAME);

	/* ###------------------------------------------------------### */
	/*    inside each sequence count arraies of input-outputs	*/
	/* ###------------------------------------------------------### */

      grp_pnt = seq_pnt->PAGRP;
      while (grp_pnt != NULL)
        {
        ptuse->PAGRP++;

        if (grp_pnt->NAME != NULL)
          ptuse->CHARACTER += strlen (grp_pnt->NAME);

        grp_pnt = grp_pnt->NEXT;
        }

	/* ###------------------------------------------------------### */
	/*    inside each sequence count input-outputs			*/
	/* ###------------------------------------------------------### */

      iol_pnt = seq_pnt->PAIOL;
      while (iol_pnt != NULL)
        {
        ptuse->PAIOL++;

        if (iol_pnt->NAME != NULL)
          ptuse->CHARACTER += strlen (iol_pnt->NAME);

        iol_pnt = iol_pnt->NEXT;
        }

	/* ###------------------------------------------------------### */
	/*    inside each sequence count inspected instances		*/
	/* ###------------------------------------------------------### */

      ins_pnt = seq_pnt->PAINS;
      while (ins_pnt != NULL)
        {
        ptuse->PAINS++;
        if (ins_pnt->INSNAME != NULL)
          ptuse->CHARACTER += strlen (ins_pnt->INSNAME);

        if (ins_pnt->FILNAME != NULL)
          ptuse->CHARACTER += strlen (ins_pnt->FILNAME);

        ins_pnt = ins_pnt->NEXT;
        }

	/* ###------------------------------------------------------### */
	/*    inside each sequence count (declaration) comments		*/
	/* ###------------------------------------------------------### */

      com_pnt = seq_pnt->DECCOM;
      while (com_pnt != NULL)
        {
        ptuse->PACOM++;

        if (com_pnt->TEXT != NULL)
          ptuse->CHARACTER += strlen (com_pnt->TEXT);

        com_pnt = com_pnt->NEXT;
        }

      }

	/* ###------------------------------------------------------### */
	/*    inside each sequence count (description) comments		*/
	/* ###------------------------------------------------------### */

    com_pnt = seq_pnt->CURCOM;
    while (com_pnt != NULL)
      {
      ptuse->PACOM++;

      if (com_pnt->TEXT != NULL)
        ptuse->CHARACTER += strlen (com_pnt->TEXT);

      com_pnt = com_pnt->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*    inside each sequence count patterns			*/
	/* ###------------------------------------------------------### */

    pat_pnt = seq_pnt->CURPAT;
    while ((pat_pnt != NULL) && (pat_pnt != seq_pnt->NXTPAT))
      {
      ptuse->PAPAT++;

	/* ###------------------------------------------------------### */
	/*    for each pattern count events				*/
	/* ###------------------------------------------------------### */

      evt_pnt = pat_pnt->PAEVT;
      while (evt_pnt != NULL)
        {
        ptuse->PAEVT++;
        evt_pnt = evt_pnt->NEXT;
        }

	/* ###------------------------------------------------------### */
	/*    for each pattern count initializations			*/
	/* ###------------------------------------------------------### */

      ini_pnt = pat_pnt->PAINI;
      while (ini_pnt != NULL)
        {
        ptuse->PAINI++;

        if (ini_pnt->NAME != NULL)
          ptuse->CHARACTER += strlen (ini_pnt->NAME);

        ini_pnt = ini_pnt->NEXT;
        }

      pat_pnt = pat_pnt->NEXT;
      }

    seq_pnt = seq_pnt->NEXT;
    }

  }
