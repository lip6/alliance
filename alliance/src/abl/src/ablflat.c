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
| Tool    :                     Abl                           |
|                                                             |
| File    :                   ablflat.c                       |
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
# include "ablflat.h"
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
|                         Flat Abl Expr                       |
|                                                             |
\------------------------------------------------------------*/

void flatablexpr( Expr )

  ablexpr *Expr;
{
  ablexpr *ScanExpr;
  ablexpr *ExprFlat;
  ablexpr *ExprPrev;
  long        Oper;
  int         Binary;
  int         Polar;

  if ( ! ABL_ATOM( Expr ) )
  {
    Oper = ABL_OPER( Expr );

    Binary = isablbinaryoper( Oper );

    if ( Binary == 0 )
    {
      flatablexpr( ABL_CADR( Expr ) );
    }
    else
    {
      Polar    = getabloperpolar( Oper );
      ExprPrev = ScanExpr = Expr;

      while ( ( ScanExpr = ABL_CDR( ScanExpr ) ) != (ablexpr *)0 )
      {
        ExprFlat = ABL_CAR( ScanExpr );

        if ( ! ABL_ATOM( ExprFlat ) )
        {
          if ( ( Polar  == ABL_POLAR_POSITIVE ) &&
               ( Binary == 1                  ) &&
               ( ABL_OPER( ExprFlat ) == Oper ) )
          {
            ABL_CDR( ExprPrev ) = ABL_CDR( ExprFlat );
            
            while ( ABL_CDR( ExprFlat ) != (ablexpr *)0 )
            {
              ExprFlat = ABL_CDR( ExprFlat );
            }

            ABL_CDR( ExprFlat ) = ABL_CDR( ScanExpr );
            ABL_CDR( ScanExpr ) = (ablexpr *)0;
            ExprFlat            = ABL_CAR( ScanExpr );
            ABL_CDR( ExprFlat ) = (ablexpr *)0;

            freechain( ScanExpr            );
            freechain( ABL_CAR( ExprFlat ) );
            freechain( ExprFlat            );

            ScanExpr = ExprPrev;
          }
          else
          {
            flatablexpr( ExprFlat );
            ExprPrev = ScanExpr;
          }
        }
        else
        {
          ExprPrev = ScanExpr;
        }
      }
    }
  }
}
