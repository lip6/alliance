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
/* file		: beh_chkbefig.c					*/
/* date		: Feb 29 2000						*/
/* version	: v111							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* description	: high level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "log.h"
#include "beh.h"
#include "bhl.h"

/* ###--------------------------------------------------------------### */
/* function	: beh_chkbefig						*/
/* description	: check the consistency of a behavioural description	*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

int beh_chkbefig (pt_befig, mode)

struct befig *pt_befig;
unsigned int  mode    ;

  {
  struct beout *pt_beout ;
  struct beaux *pt_beaux ;
  struct bebus *pt_bebus ;
  struct bebux *pt_bebux ;
  struct bereg *pt_bereg ;
  struct berin *pt_berin;
  struct beaux *pred_beaux ;
  struct berin *pred_berin ;
  int           err_flg  = 0;

	/* ###------------------------------------------------------### */
	/*   check that the description is not empty			*/
	/* ###------------------------------------------------------### */

  if ((pt_befig == NULL) || (pt_befig->ERRFLG != 0))
    err_flg = 1;
  else
    { 
	/* ###------------------------------------------------------### */
	/*   check that each output has at least one driver. Outputs	*/
	/* are simple output ports, bussed output ports, simple		*/
	/* internal signals, delayed internal signals, bussed internal	*/
	/* signals, and internal registers.				*/
	/* ###------------------------------------------------------### */

    if ((mode & BEH_CHK_DRIVERS) != 0)
      {
	/* ###------------------------------------------------------### */
	/*   simple output ports					*/
	/* ###------------------------------------------------------### */

      pt_beout = pt_befig->BEOUT;
      while (pt_beout != NULL)
        {
        if (pt_beout->ABL == NULL)
          err_flg += beh_error (40, pt_beout->NAME);
        pt_beout = pt_beout->NEXT;
        }

	/* ###------------------------------------------------------### */
	/*   simple internal signals					*/
	/* ###------------------------------------------------------### */

      pt_beaux = pt_befig->BEAUX;
      pred_beaux = NULL;
      while (pt_beaux != NULL)
        {
        if (pt_beaux->ABL == NULL)
        {
          fprintf (stderr, "BEH : Warning %d: ", 40);
          fprintf (stderr, "signal `%s` never assigned\n", pt_beaux->NAME);
          
          /*removing signal*/
          if ( pred_beaux ) pred_beaux->NEXT = pt_beaux->NEXT;
          else pt_befig->BEAUX = pt_beaux->NEXT;

          /*removing other ref to signal*/
          pred_berin = NULL;
          for ( pt_berin = pt_befig->BERIN; pt_berin; pt_berin = pt_berin->NEXT )
          {
             if ( pt_berin->NAME == pt_beaux->NAME )
             {
                 if ( pred_berin ) pred_berin->NEXT = pt_berin->NEXT;
                 else pt_befig->BERIN = pt_berin->NEXT;
                 break;
             }
             pred_berin = pt_berin;
          }
        }
        else pred_beaux = pt_beaux;
        pt_beaux = pt_beaux->NEXT;
        }

	/* ###------------------------------------------------------### */
	/*   delayed internal signals					*/
	/* ###------------------------------------------------------### */

      pt_beaux = pt_befig->BEDLY;
      while (pt_beaux != NULL)
        {
        if (pt_beaux->ABL == NULL)
          err_flg += beh_error (40, pt_beaux->NAME);
        pt_beaux = pt_beaux->NEXT;
        }

	/* ###------------------------------------------------------### */
	/*   bussed output ports					*/
	/* ###------------------------------------------------------### */

      pt_bebus = pt_befig->BEBUS;
      while (pt_bebus != NULL)
        {
        if (pt_bebus->BIABL == NULL)
          err_flg += beh_error (40, pt_bebus->NAME);
        pt_bebus = pt_bebus->NEXT;
        }

	/* ###------------------------------------------------------### */
	/*   bussed internal signals					*/
	/* ###------------------------------------------------------### */

      pt_bebux = pt_befig->BEBUX;
      while (pt_bebux != NULL)
        {
        if (pt_bebux->BIABL == NULL)
          err_flg += beh_error (40, pt_bebux->NAME);
        pt_bebux = pt_bebux->NEXT;
        }

	/* ###------------------------------------------------------### */
	/*   internal registers						*/
	/* ###------------------------------------------------------### */

      pt_bereg = pt_befig->BEREG;
      while (pt_bereg != NULL)
        {
        if (pt_bereg->BIABL == NULL)
          err_flg += beh_error (40, pt_bereg->NAME);
        pt_bereg = pt_bereg->NEXT;
        }

      }

	/* ###------------------------------------------------------### */
	/*   check that the descriptiona has an empty architecture.	*/
	/* that means, no internal signal is declared and output ports	*/
	/* have no dreivers.						*/
	/* ###------------------------------------------------------### */

    if ((mode & BEH_CHK_EMPTY) != 0)
      {
	/* ###------------------------------------------------------### */
	/*   check that simple output ports have no drivers		*/
	/* ###------------------------------------------------------### */

      pt_beout = pt_befig->BEOUT;
      while (pt_beout != NULL)
        {
        if (pt_beout->ABL != NULL)
          break;
        pt_beout = pt_beout->NEXT;
        }

	/* ###------------------------------------------------------### */
	/*   check that bussed output ports have no drivers		*/
	/* ###------------------------------------------------------### */

      pt_bebus = pt_befig->BEBUS;
      while (pt_bebus != NULL)
        {
        if (pt_bebus->BIABL != NULL)
          break;
        pt_bebus = pt_bebus->NEXT;
        }

	/* ###------------------------------------------------------### */
	/*   check that the architecture is empty			*/
	/* ###------------------------------------------------------### */

      if ((pt_befig->BEAUX != NULL) || (pt_befig->BEBUX != NULL) ||
          (pt_befig->BEDLY != NULL) || (pt_befig->BEREG != NULL) ||
          (pt_befig->BEMSG != NULL) || (pt_beout        != NULL) ||
          (pt_bebus        != NULL) )
        err_flg += beh_error (41, pt_befig->NAME);

      }

    pt_befig->ERRFLG = err_flg;
    }

  return (err_flg);
  }
