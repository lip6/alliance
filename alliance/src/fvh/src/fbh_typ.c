/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU Library General|
| Public License as published by the Free Software Foundation |
| either version 2 of the License, or (at your option) any    |
| later version.                                              |
|                                                             |
| Alliance VLSI  CAD System  is distributed  in the hope that |
| it  will be useful, but WITHOUT  ANY WARRANTY;              |
| without even the  implied warranty of MERCHANTABILITY or    |
| FITNESS FOR A PARTICULAR PURPOSE. See the GNU General       |
| Public License for more details.                            |
|                                                             |
| You should have received a copy  of the GNU General Public  |
| License along with the GNU C Library; see the file COPYING. |
| If not, write to the Free Software Foundation, Inc.,        |
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: fbh_crtfbtyp.c					*/
/* date		: Aug 24 1992						*/
/* version	: v102							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: contains a specific function used to create a		*/
/*		  fbhaviour data structure				*/
/*									*/
/* ###--------------------------------------------------------------### */

#include "mut.h" 
#include "aut.h"
#include "abl.h" 
#include "bdd.h"
#include "fvh.h"

/* ###--------------------------------------------------------------### */
/* function	: fbh_crtfbtyp						*/
/* description	: create an array of BETYPs from a list of BETYPs	*/
/* called func.	: namealloc, mbkalloc					*/
/* ###--------------------------------------------------------------### */

struct fbtyp *fbh_crtfbtyp (listfbtyp)

struct fbtyp *listfbtyp;	/* pointer on the list of fbtyps	*/

  {
  struct fbtyp *lclfbtyp = listfbtyp;
  struct fbtyp *ptfbtyp  = 0;
  int           size     = 0;

  while (lclfbtyp != 0)
    {
    lclfbtyp = lclfbtyp->NEXT;
    size++;
    }

  if (size != 0)
    {
    ptfbtyp  = (struct fbtyp *) mbkalloc (size * sizeof(struct fbtyp));
    lclfbtyp = ptfbtyp;

    while (listfbtyp != 0)
      {
      lclfbtyp->NAME   = listfbtyp->NAME;
      lclfbtyp->LEFT   = listfbtyp->LEFT;
      lclfbtyp->RIGHT  = listfbtyp->RIGHT;
      lclfbtyp->VALUE  = listfbtyp->VALUE;
      lclfbtyp->SIZE   = listfbtyp->SIZE;
      lclfbtyp->BYTE   = listfbtyp->BYTE;
      lclfbtyp->BASE   = listfbtyp->BASE;
      lclfbtyp->INDEX  = listfbtyp->INDEX;
      lclfbtyp->RESOLV = listfbtyp->RESOLV;
      lclfbtyp->CLASS  = listfbtyp->CLASS;

      lclfbtyp->NEXT   = lclfbtyp + 1;

      listfbtyp        = listfbtyp->NEXT;
      lclfbtyp++;
      }

    (lclfbtyp - 1)->NEXT = 0;
    }

  return (ptfbtyp);
  }
