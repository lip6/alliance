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
| File    :                   abladd.c                        |
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
# include "abladd.h"
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

  static ablexpr *AblQueueExpr = (ablexpr *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Add  Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Add Abl Queue Expr                    |
|                                                             |
\------------------------------------------------------------*/

void addablqexpr( Expr1, Expr2 )

  ablexpr *Expr1;
  ablexpr *Expr2;
{
  if ( Expr2 == (ablexpr *)0 )
  {
    AblQueueExpr = Expr1; return;
  }

  if ( Expr1 == (ablexpr *)0 )
  {
    Expr1 = AblQueueExpr;
  }
  else
  {
    while ( ABL_CDR( Expr1 ) != (ablexpr *)0 )
    {
      Expr1 = ABL_CDR( Expr1 );
    }
  }

  AblQueueExpr     = addchain( (ablexpr *)0, (void *)Expr2 );
  ABL_CDR( Expr1 ) = AblQueueExpr;
}

/*------------------------------------------------------------\
|                                                             |
|                       Add Abl Head Expr                     |
|                                                             |
\------------------------------------------------------------*/

void addablhexpr( Expr1, Expr2 )

  ablexpr *Expr1;
  ablexpr *Expr2;
{
  ABL_CDR( Expr1 ) = addchain( ABL_CDR( Expr1 ), (void *)Expr2 );
}
