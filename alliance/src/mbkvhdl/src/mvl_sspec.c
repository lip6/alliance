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
/*									*/
/* file		: mvl_sspec.yac						*/
/* date		: Jan 29 1992						*/
/* author	: P.   BAZARGAN						*/
/*		  L.A. TABUSSE						*/
/*		  VUONG H.N.						*/
/*									*/
/* content	: these functions are used to build up MBK's structures	*/
/*									*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include <mut.h>
#include <mlo.h>
#include "mvl_stype.h"
#include "mvl_sspec.h"
#include "mvl_stdef.h"

/* ###--------------------------------------------------------------### */
/* function	: mvl_addlosig						*/
/* description	: create one or more losig structures (for an array a	*/
/*		  losig is created for each bit)			*/
/* called func.	: addlosig, addchain					*/
/* ###--------------------------------------------------------------### */

struct losig *mvl_addlosig (ptfig, index, type, ptype, name, left, right)

lofig_list   *ptfig;		/* pointer on LOFIG structure		*/
int	      index;		/* index of the first signal		*/
char          type ;		/* signal's type			*/
char          ptype;		/* signal's resolution function		*/
char         *name ;		/* signal's name			*/
short         left ;		/* array's left bound (= -1 if scalar)	*/
short         right;		/* array's right bound (= -1 if scalar)	*/

  {
  char          extname[100];
  short         i           ;
  short         inc         = 1;
  struct chain *pt_chlst    ;
  struct losig *ptsig       ;

  if ((left==-1) && (right==-1))
    {
    pt_chlst = addchain (NULL, name);
    ptsig    = addlosig (ptfig, index, pt_chlst, type);

    if (ptype != '0')
      ptsig->USER = addptype (ptsig->USER, ptype, NULL);
    }
  else
    {
    if (left >= right)			/* array */
      inc = -1;

    for (i=left ; i!=(right+inc) ; i+=inc)
      {
      sprintf (extname, "%s %d", name, i);
      pt_chlst = addchain (NULL, extname);
      ptsig    = addlosig (ptfig, index, pt_chlst, type);

      if (ptype != '0')
        ptsig->USER = addptype (ptsig->USER, ptype, NULL);

      index++;
      }
    }
  return (ptsig);
  }

/* ###--------------------------------------------------------------### */
/* function	: mvl_addlocon						*/
/* description	: create one or more locon structures (for an array a	*/
/*		  locon is created for each bit)			*/
/* called func.	: addlocon, addchain					*/
/* ###--------------------------------------------------------------### */

struct locon *mvl_addlocon (ptfig, ptsig, dir, name, left, right)

struct lofig   *ptfig;		/* BEFIG structure's pointer		*/
struct losig   *ptsig;		/* external signal's pointer		*/
char            dir  ;		/* port's mode				*/
char           *name ;		/* port's name				*/
short           left ;		/* array left bound (= -1 if scalar)	*/
short           right;		/* array right bound (= -1 if scalar)	*/

  {
  char          extname[100];
  short         i           ;
  short         inc         = 1;
  struct locon *ptcon       ;
  struct locon *ptcontmp    ;

  if ((left==-1) && (right==-1))
    {
    ptcon = addlocon (ptfig, name, ptsig, dir);
    }
  else
    {
    if (left >= right)
      inc = -1;

    for (i=left ; i!=(right+inc) ; i+=inc)
      {
      sprintf (extname, "%s %d", name, i);
      ptcon = addlocon (ptfig, extname, NULL, dir);
      }

    if (ptsig != NULL)
      {
      ptcontmp = ptcon;
      for (i=left ; i!=(right+inc) ; i+=inc)
        {
        ptcontmp->SIG = ptsig;
        ptcontmp      = ptcontmp->NEXT;
        ptsig         = ptsig->NEXT;
        }
      }
    }
  return (ptcon);
  }
