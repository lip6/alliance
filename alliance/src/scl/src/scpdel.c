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
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     Scp                           |
|                                                             |
| File    :                   scpdel.c                        |
|                                                             |
| Date    :                   28.04.98                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "scl.h"

# include <stdio.h>
# include "scp.h"
# include "scpdel.h"

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Scp Del Functions                      |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                        Scp Del Cell List                    |
|                                                             |
\------------------------------------------------------------*/

static void loc_delscpcel( HeadCell )
  
   scpcel_list *HeadCell;
{
  scpcel_list *ScanCell;
  scpcel_list *DelCell;

  ScanCell = HeadCell;
  
  while ( ScanCell != (scpcel_list *)0 )
  {
    DelCell  = ScanCell;
    ScanCell = ScanCell->NEXT;
    
    freescpcel( DelCell );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      Scp Del Column List                    |
|                                                             |
\------------------------------------------------------------*/

void delscpcol()
{
  scpcol_list *ScanColumn;
  scpcol_list *DelColumn;

  ScanColumn = HEAD_SCPCOL;
  
  while ( ScanColumn != (scpcol_list *)0 )
  {
    loc_delscpcel( ScanColumn->CELL );

    DelColumn  = ScanColumn;
    ScanColumn = ScanColumn->NEXT;
    
    freescpcol( DelColumn );
  }

  HEAD_SCPCOL = (scpcol_list *)0;
}
