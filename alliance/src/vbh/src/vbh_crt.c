/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
/* file		: vbh_crtvbrin.c					*/
/* date		: Aug 24 1992						*/
/* version	: v102							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: contains a specific function used to create a		*/
/*		  behaviour data structure				*/
/*									*/
/* ###--------------------------------------------------------------### */

#include "mut.h" 
#include "aut.h"
#include "vex.h" 
#include "vbh.h" 

/* ###--------------------------------------------------------------### */
/* function	: vbh_crtvbtyp						*/
/* description	: create an array of BETYPs from a list of BETYPs	*/
/* called func.	: namealloc, autallocblock					*/
/* ###--------------------------------------------------------------### */

struct vbtyp *vbh_crtvbtyp (listvbtyp)

struct vbtyp *listvbtyp;	/* pointer on the list of vbtyps	*/

{
  struct vbtyp *lclvbtyp = listvbtyp;
  struct vbtyp *ptvbtyp  = NULL;
  int           size     = 0;

  while (lclvbtyp != NULL)
  {
    lclvbtyp = lclvbtyp->NEXT;
    size++;
  }

  if (size != 0)
  {
    ptvbtyp  = (struct vbtyp *) autallocblock (size * sizeof(struct vbtyp));
    lclvbtyp = ptvbtyp;

    while (listvbtyp != NULL)
    {
      lclvbtyp->NAME   = listvbtyp->NAME;
      lclvbtyp->LEFT   = listvbtyp->LEFT;
      lclvbtyp->RIGHT  = listvbtyp->RIGHT;
      lclvbtyp->VALUE  = listvbtyp->VALUE;
      lclvbtyp->SIZE   = listvbtyp->SIZE;
      lclvbtyp->BYTE   = listvbtyp->BYTE;
      lclvbtyp->BASE   = listvbtyp->BASE;
      lclvbtyp->INDEX  = listvbtyp->INDEX;
      lclvbtyp->RESOLV = listvbtyp->RESOLV;
      lclvbtyp->CLASS  = listvbtyp->CLASS;

      lclvbtyp->NEXT   = lclvbtyp + 1;

      listvbtyp        = listvbtyp->NEXT;
      lclvbtyp++;
    }

    (lclvbtyp - 1)->NEXT = NULL;
  }

  return (ptvbtyp);
}
