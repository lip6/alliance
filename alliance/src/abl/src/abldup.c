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
| File    :                   abldup.c                        |
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
# include "abldup.h"
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
|                        Dup  Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Duplicate Abl Expr                     |
|                                                             |
\------------------------------------------------------------*/

ablexpr *dupablexpr( Expr )

  ablexpr *Expr;
{
  ablexpr *ExprDup;
  ablexpr *ExprQueue;

  if ( Expr == (ablexpr *)0 )
  {
    return ( Expr );
  }

  if ( ABL_ATOM( Expr ) )
  {
    return( addchain( (ablexpr *)0, ABL_CAR( Expr ) ) );
  }

  ExprDup = addchain( (ablexpr *)0,
                      (void *)addchain( (ablexpr *)0,
                                        (void *)ABL_OPER( Expr ) ) );
  ExprQueue = ExprDup;

  while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
  {
    ABL_CDR( ExprQueue ) = addchain( (ablexpr *)0,
                                     (void *)dupablexpr( ABL_CAR( Expr ) ) );
    ExprQueue = ABL_CDR( ExprQueue );
  }

  return( ExprDup );
}
