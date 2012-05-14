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
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     Abl                           |
|                                                             |
| File    :                  ablunflat.c                      |
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
# include "ablunflat.h"
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

ablexpr *unflatablexpr( Expr )

  ablexpr *Expr;
{
  ablexpr *ScanExpr;
  long        Oper;
  int         Binary;
  int         Polar;

  if ( ! ABL_ATOM( Expr ) )
  {
    Oper = ABL_OPER( Expr );

    Binary = isablbinaryoper( Oper );

    if ( Binary != 0 )
    {
      Polar = getabloperpolar( Oper );

      if ( ABL_CDR( ABL_CDDR( Expr ) ) != (ablexpr *)0 )
      {
        if ( Polar != ABL_POLAR_POSITIVE )
        {
          Oper = getablopernot( Oper );
          SET_ABL_OPER( Expr, Oper );

          ScanExpr = createabloper( ABL_NOT );
          addablhexpr( ScanExpr, Expr );

          Expr = ScanExpr;
        }
        else
        {
          ScanExpr = createabloper( Oper );
          ABL_CDR( ScanExpr ) = ABL_CDDR( Expr );
          ABL_CDDR( Expr ) = addchain( (ablexpr *)0, (void *)ScanExpr );
        }
      }
    }

    ScanExpr = Expr;

    while ( ( ScanExpr = ABL_CDR( ScanExpr ) ) != (ablexpr *)0 )
    {
      ABL_CAR_L( ScanExpr ) = unflatablexpr( ABL_CAR( ScanExpr ) );
    }
  }

  return( Expr );
}
