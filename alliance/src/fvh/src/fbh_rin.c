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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: fbh_crtfbrin.c					*/
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
/* function	: fbh_crtfbrin						*/
/* description	: create an array of BETYPs from a list of BETYPs	*/
/* called func.	: namealloc, mbkalloc					*/
/* ###--------------------------------------------------------------### */

struct fbrin *fbh_crtfbrin (listfbrin)

struct fbrin *listfbrin;	/* pointer on the list of fbrins	*/

  {
  struct fbrin *lclfbrin = listfbrin;
  struct fbrin *ptfbrin  = 0;
  int           size     = 0;

  while (lclfbrin != 0)
    {
    lclfbrin = lclfbrin->NEXT;
    size++;
    }

  if (size != 0)
    {
    ptfbrin  = (struct fbrin *) mbkalloc (size * sizeof(struct fbrin));
    lclfbrin = ptfbrin;

    while (listfbrin != 0)
      {
      lclfbrin->NAME   = listfbrin->NAME;
      lclfbrin->NEXT   = lclfbrin + 1;

      listfbrin        = listfbrin->NEXT;
      lclfbrin++;
      }

    (lclfbrin - 1)->NEXT = 0;
    }

  return (ptfbrin);
  }
