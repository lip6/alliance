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
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     Abl                           |
|                                                             |
| File    :                   abldel.c                        |
|                                                             |
| Date    :                   03.12.96                        |
|                                                             |
| Author  :               Jacomme Ludovic                     |
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

# include <stdio.h>
# include "abldel.h"
# include "ablerror.h"

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
|                        Del  Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Del Abl Expr                        |
|                                                             |
\------------------------------------------------------------*/

void delablexpr( Expr )

  ablexpr *Expr;
{
  ablexpr *ScanExpr;

  if ( ABL_ATOM( Expr ) )
  {
    freechain( Expr );
  }
  else
  {
    ScanExpr = Expr;

    freechain( ABL_CAR( Expr ) );

    while ( ( ScanExpr = ABL_CDR( ScanExpr ) ) != (ablexpr *)0 )
    {
      delablexpr( ABL_CAR( ScanExpr ) );
    }

    freechain( Expr );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                         Del Abl Expr Number                 |
|                                                             |
\------------------------------------------------------------*/

void delablexprnum( Expr, Number )

  ablexpr *Expr;
  int         Number;
{
  ablexpr *PrevExpr;
  int         Counter;

  if ( ! ABL_ATOM( Expr ) )
  {
    PrevExpr = Expr;
    Counter  = 0;

    while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
    {
      if ( Counter == Number )
      {
        ABL_CDR( PrevExpr ) = ABL_CDR( Expr );
        ABL_CDR( Expr     ) = (ablexpr *)0;

        delablexpr( ABL_CAR( Expr ) );
        freechain( Expr );

        return;
      }
      
      Counter  = Counter + 1;
      PrevExpr = Expr;
    }
  }
}
