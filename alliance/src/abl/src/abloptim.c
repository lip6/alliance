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
| File    :                  abloptim.c                       |
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
# include "abloptim.h"
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
|                         Optim Functions                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Optim Abl Bin Expr                  |
|                                                             |
\------------------------------------------------------------*/

ablexpr *optimablbinexpr( Oper, Expr1, Expr2 )

  long        Oper;
  ablexpr *Expr1;
  ablexpr *Expr2;
{
  ablexpr *Expr;
  ablexpr *ScanExpr;
  int         Negative;
  int         Binary;

  Binary = isablbinaryoper( Oper );

  if ( Binary == 0 )
  {
    ablerror( ABL_OPERATOR_ERROR, Oper );
  }

  Negative = ( getabloperpolar( Oper ) == ABL_POLAR_NEGATIVE );

  if ( ( Negative                  ) ||
       ( Binary == 2               ) ||
       ( ABL_ATOM( Expr2 )         ) ||
       ( ABL_OPER( Expr2 ) != Oper ) )
  {
    Expr = createabloper( Oper );
    addablhexpr( Expr, Expr2 );
  }
  else
  {
    Expr = Expr2;
  }
 
  if ( ( Negative                  ) ||
       ( Binary == 2               ) ||
       ( ABL_ATOM( Expr1 )         ) ||
       ( ABL_OPER( Expr1 ) != Oper ) )
  {
    addablhexpr( Expr, Expr1 );
  }
  else
  {
    ScanExpr = Expr1;

    while ( ABL_CDR( ScanExpr ) != (ablexpr *)0 )
    {
      ScanExpr = ABL_CDR( ScanExpr );
    }

    ABL_CDR( ScanExpr ) = ABL_CDR( Expr );
    ABL_CDR( Expr     ) = ABL_CDR( Expr1 );
    ABL_CDR( Expr1    ) = (ablexpr *)0;

    freechain( ABL_CAR( Expr1 ) );
    freechain( Expr1 );
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                       Optim Abl Not Expr                    |
|                                                             |
\------------------------------------------------------------*/

ablexpr *optimablnotexpr( Expr )

  ablexpr *Expr;
{
  ablexpr *Expr1;
  char       *AtomValue;
  long        Oper;
  long        NotOper;

  if ( ABL_ATOM( Expr ) )
  {
    AtomValue = ABL_ATOM_VALUE( Expr );

    if ( AtomValue == ABL_ATOM_NAME_ONE )
    {
      ABL_ATOM_VALUE( Expr ) = (void *)ABL_ATOM_NAME_ZERO;

      return( Expr );
    }
    else
    if ( AtomValue == ABL_ATOM_NAME_ZERO )
    {
      ABL_ATOM_VALUE( Expr ) = (void *)ABL_ATOM_NAME_ONE;

      return( Expr );
    }
  }
  else
  {
    Oper    = ABL_OPER( Expr );
    NotOper = getablopernot( Oper );

    if ( Oper == ABL_NOT )
    {
      Expr1 = ABL_CADR( Expr );
      freechain( ABL_CAR( Expr ) );
      freechain( Expr );

      return( Expr1 );
    }
    else
    if ( NotOper != -1 )
    {
      ABL_OPER( Expr ) = NotOper;
      return( Expr );
    }
  }

  Expr1 = createabloper( ABL_NOT );
  addablhexpr( Expr1, Expr );

  return( Expr1 );
}

/*------------------------------------------------------------\
|                                                             |
|                      Optim Abl Xor Expr                     |
|                                                             |
\------------------------------------------------------------*/

ablexpr *optimablxorbinexpr( Oper, Expr1, Expr2 )

  long        Oper;
  ablexpr *Expr1;
  ablexpr *Expr2;
{
  ablexpr *Expr;
  ablexpr *NotExpr1;
  ablexpr *NotExpr2;

  if ( ( Oper != ABL_XOR  ) &&
       ( Oper != ABL_NXOR ) )
  {
    ablerror( ABL_OPERATOR_ERROR, Oper );
  }

  NotExpr1 = optimablnotexpr( dupablexpr( Expr1 ) );
  NotExpr2 = optimablnotexpr( dupablexpr( Expr2 ) );

  if ( Oper == ABL_XOR )
  {
    Expr = optimablbinexpr( ABL_OR,
                            optimablbinexpr( ABL_AND, NotExpr1, Expr2    ),
                            optimablbinexpr( ABL_AND, Expr1   , NotExpr2 ) );
  }
  else
  {
    Expr = optimablbinexpr( ABL_OR,
                            optimablbinexpr( ABL_AND, NotExpr1, NotExpr2 ),
                            optimablbinexpr( ABL_AND, Expr1   , Expr2    ) );
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                      Optim Abl Unary Expr                   |
|                                                             |
\------------------------------------------------------------*/

ablexpr *optimablunaryexpr( Oper, Expr )

  long     Oper;
  ablexpr *Expr;
{
  ablexpr *Expr1;

  if ( ! isablunaryoper( Oper ) )
  {
    ablerror( ABL_NOT_UNARY_ERROR, Oper );
  }

  if ( Oper == ABL_NOT )
  {
    return( optimablnotexpr( Expr ) );
  }

  Expr1 = createabloper( Oper );
  addablhexpr( Expr1, Expr );

  return( Expr1 );
}
