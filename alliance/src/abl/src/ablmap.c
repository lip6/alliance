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
| File    :                   ablmap.c                        |
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
# include "ablmap.h"
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
|                        Map  Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Map Abl Expression                     |
|                                                             |
\------------------------------------------------------------*/

void mapablexpr( Function, Expr )

  void      (*Function)();
  ablexpr *Expr;
{
  while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
  {
    (*Function)( ABL_CAR( Expr ) );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      Map Abl Sub Expression                 |
|                                                             |
\------------------------------------------------------------*/

ablexpr *mapabloperexpr( Function, Oper, Expr )

  ablexpr *(*Function)();
  long        Oper;
  ablexpr *Expr;
{
  ablexpr *MapExpr;

  MapExpr = createabloper( Oper );

  while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
  {
    addablhexpr( MapExpr, (*Function)( ABL_CAR( Expr ) ) );
  }

  return( MapExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                      Map Abl Any Expression                 |
|                                                             |
\------------------------------------------------------------*/

int mapablanyexpr( Function, Expr )

  int       (*Function)();
  ablexpr *Expr;
{
  while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
  {
    if ( (*Function)( ABL_CAR( Expr ) ) ) return( 1 );
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                      Map Abl Every Expression               |
|                                                             |
\------------------------------------------------------------*/

int mapableveryexpr( Function, Expr )

  int       (*Function)();
  ablexpr *Expr;
{
  while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
  {
    if ( ! (*Function)( ABL_CAR( Expr ) ) ) return( 0 );
  }

  return( 1 );
}
