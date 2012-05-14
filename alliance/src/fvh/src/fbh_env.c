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
/* file		: fbh_getgenva.c					*/
/* date		: Aug 24 1992						*/
/* version	: v102							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: contains a specific function used to acces to the	*/
/*		  value of a fbhaviour data structure			*/
/*									*/
/* ###--------------------------------------------------------------### */

#include "mut.h" 
#include "aut.h"
#include "abl.h" 
#include "bdd.h"
#include "fvh.h"

/* ###--------------------------------------------------------------### */
/* function	: fbh_getgenval						*/
/* description	: Search an find a BEGEN structure (known from its name)*/
/*		  then return its value field. If the BEGEN is not found*/
/*		  a 0 pointer is returned				*/
/* called func.	:							*/
/* ###--------------------------------------------------------------### */

void *fbh_getgenval (listfbgen, fbgenname)

struct fbgen *listfbgen;	/* head of BEGEN list			*/
char         *fbgenname;	/* name of the structure 		*/

  {
  struct fbgen *ptfbgen;
  void         *value = 0;

	/* ###------------------------------------------------------### */
	/*    searching the object					*/
	/* ###------------------------------------------------------### */

  ptfbgen = listfbgen;
  while ((ptfbgen != 0) && (ptfbgen->NAME != fbgenname))
    ptfbgen = ptfbgen->NEXT;

	/* ###------------------------------------------------------### */
	/*    if found return its value field				*/
	/* ###------------------------------------------------------### */

  if (ptfbgen != 0)
    value = ptfbgen->VALUE;

  return (value);
  }
