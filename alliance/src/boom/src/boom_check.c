/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
| Tool    :                     BOOM                          |
|                                                             |
| File    :                   boom_check.c                    |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   06.06.00                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "abe.h"
# include "abv.h"
# include "abt.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "boom_error.h"
# include "boom_check.h"
# include "boom_debug.h"
# include "boom_shared.h"
# include "boom_abl.h"

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

static int BoomFoundDontCare = 0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      BoomCheckBehAblDontCare                |
|                                                             |
\------------------------------------------------------------*/

static void BoomCheckBehAblDontCare( Expr )

  ablexpr *Expr;
{
  if ( Expr != (ablexpr *)0 )
  {
    if ( ! BoomFoundDontCare )
    {
      BoomFoundDontCare = isablnameinexpr( Expr, ABL_ATOM_NAME_DC );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomCheckBehFigure                     |
|                                                             |
\------------------------------------------------------------*/

void BoomCheckBehFigure( BehFigure )

  befig_list *BehFigure;
{
  boombefiginfo *FigInfo;

  FigInfo = GetBoomBehFigInfo( BehFigure );

  BoomFoundDontCare = 0;

  BoomBehScanAbl( BehFigure, BoomCheckBehAblDontCare, 1 ) ;

  FigInfo->DONT_CARE = BoomFoundDontCare;

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Dont care %d\n", BoomFoundDontCare );
  }
 

  /* Check combinatory loop etc ... */
  /* TO BE DONE */
}
