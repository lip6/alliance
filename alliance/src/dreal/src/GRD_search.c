/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU General Public |
| License as  published by the Free Software Foundation;      |
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

/*------------------------------------------------------------\
|                                                             |
| File    :                  Search.c                         |
|                                                             |
| Author  :                Jacomme Ludovic                    |
|                                                             |
| Date    :                  28.03.95                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "rfm.h"
# include "GRD.h"
# include "GSB.h"
# include "GRD_search.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  drealsearch *DrealHeadSearch = (drealsearch *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Alloc Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        DrealAllocSearch                     |
|                                                             |
\------------------------------------------------------------*/

drealsearch *DrealAllocSearch()
{
  return((drealsearch *)rdsalloc(sizeof(drealsearch), 1));
}

/*------------------------------------------------------------\
|                                                             |
|                        Free Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         DrealFreeSearch                     |
|                                                             |
\------------------------------------------------------------*/

void DrealFreeSearch( FreeSearch )

   drealsearch *FreeSearch;
{
  rdsfree((char *)FreeSearch, sizeof(drealsearch));
}

/*------------------------------------------------------------\
|                                                             |
|                       DrealAddSearch                        |
|                                                             |
\------------------------------------------------------------*/

void DrealAddSearch( Rectangle )

   rdsrec_list *Rectangle;
{
  drealsearch *Search;

  rdsbegin();

  Search = DrealAllocSearch();

  Search->RECTANGLE = Rectangle;
  Search->NEXT      = DrealHeadSearch;
  DrealHeadSearch   = Search;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       DrealDelSearch                        |
|                                                             |
\------------------------------------------------------------*/

void DrealDelSearch()

{
  drealsearch *Search;

  rdsbegin();

  while ( DrealHeadSearch != (drealsearch *)NULL )
  {
    Search          = DrealHeadSearch;
    DrealHeadSearch = DrealHeadSearch->NEXT;

    DrealFreeSearch( Search );
  }

  rdsend();
}
