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
| File    :                  ablsubst.c                       |
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
# include "ablsubst.h"
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

  static char       *AblSubstName = (char *)0;
  static ablexpr *AblSubstExpr = (ablexpr *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    Substitue Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                Substitue Local Name By Expr                 |
|                                                             |
\------------------------------------------------------------*/

static ablexpr *loc_substablexpr( Expr )

  ablexpr *Expr;
{
  ablexpr *ExprSubst;

  ExprSubst = Expr;

  if ( ABL_ATOM( Expr ) )
  {
    if ( ABL_ATOM_VALUE( Expr ) == AblSubstName )
    {
      freechain( Expr );
      ExprSubst = dupablexpr( AblSubstExpr );
    }

    return( ExprSubst );
  }

  while ( ( Expr = ABL_CDR( Expr ) ) )
  {
    ABL_CAR( Expr ) = loc_substablexpr( ABL_CAR( Expr ) );
  }

  return( ExprSubst );
}

/*------------------------------------------------------------\
|                                                             |
|                     Substitue Name Expr                     |
|                                                             |
\------------------------------------------------------------*/

ablexpr *substablexpr( Expr1, Name, Expr2 )

  ablexpr *Expr1;
  char       *Name;
  ablexpr *Expr2;
{
  AblSubstName = namealloc( Name );
  AblSubstExpr = Expr2;
  
  return( loc_substablexpr( Expr1 ) );
}

/*------------------------------------------------------------\
|                                                             |
|          Substitue Local Duplicate Name By Expr             |
|                                                             |
\------------------------------------------------------------*/

static ablexpr *loc_substdupablexpr( Expr )

  ablexpr *Expr;
{
  ablexpr *ExprDup;
  ablexpr *ExprQueue;

  if ( ABL_ATOM( Expr ) )
  {
    if ( ABL_ATOM_VALUE( Expr ) == AblSubstName )
    {
      return( dupablexpr( AblSubstExpr ) );
    }

    return( addchain( (ablexpr *)0, ABL_CAR( Expr ) ) );
  }

  ExprDup = addchain( (ablexpr *)0,
                      (void *)addchain( (ablexpr *)0,
                                        (void *)ABL_OPER( Expr ) ) );
  ExprQueue = ExprDup;

  while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
  {
    ABL_CDR( ExprQueue ) = 

       addchain( (ablexpr *)0, (void *)loc_substdupablexpr( ABL_CAR( Expr ) ) );

    ExprQueue = ABL_CDR( ExprQueue );
  }

  return( ExprDup );
}

/*------------------------------------------------------------\
|                                                             |
|                     Substitue Duplicate Name Expr           |
|                                                             |
\------------------------------------------------------------*/

ablexpr *substdupablexpr( Expr1, Name, Expr2 )

  ablexpr *Expr1;
  char       *Name;
  ablexpr *Expr2;
{
  AblSubstName = namealloc( Name );
  AblSubstExpr = Expr2;

  return( loc_substdupablexpr( Expr1 ) );
}
