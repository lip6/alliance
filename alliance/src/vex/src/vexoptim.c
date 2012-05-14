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
| Tool    :                     Vex                           |
|                                                             |
| File    :                  vexoptim.c                       |
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
# include "vex.h"

# include <stdio.h>
# include "vexoptim.h"
# include "vexerror.h"

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
|                          Varivexes                          |
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
|                       Optim Vex Not Expr                    |
|                                                             |
\------------------------------------------------------------*/

vexexpr *optimvexnotexpr( Expr )

  vexexpr *Expr;
{
  vexexpr *Expr1;
  char    *Value;
  char     Buffer[ 128 ];
  short    Index;
  long     Oper;
  long     NotOper;

  if ( IsVexNodeAtom( Expr ) )
  {
    if ( IsVexAtomLiteral( Expr ) )
    {
      Value = GetVexAtomValue( Expr );

      Buffer[ 0 ] = Value[ 0 ];

      for ( Index = 1; Index <= Expr->WIDTH; Index++ )
      {
        evalvexnotliteral( Value[ Index ], &Buffer[ Index ] );
      }

      Buffer[ Index++ ] = Value[ 0 ];
      Buffer[ Index++ ] = '\0';

      SetVexAtomValue( Expr, namealloc( Buffer ) );

      return( Expr );
    }
  }
  else
  if ( IsVexNodeOper( Expr ) )
  {
    Oper    = GetVexOperValue( Expr );
    NotOper = getvexnotoper( Oper );

    if ( Oper == VEX_NOT )
    {
      Expr1 = GetVexOperand( Expr->OPERAND );
      SetVexOperand( Expr->OPERAND, (vexexpr *)0 );
      freevexexpr( Expr );

      return( Expr1 );
    }
    else
    if ( NotOper != -1 )
    {
      SetVexOperValue( Expr, NotOper );

      return( Expr );
    }
  }

  Expr1 = createvexoper( VEX_NOT, Expr->WIDTH );
  addvexhexpr( Expr1, Expr );

  return( Expr1 );
}

/*------------------------------------------------------------\
|                                                             |
|                       Optim Vex Bin Expr                    |
|                                                             |
\------------------------------------------------------------*/

vexexpr *optimvexbinexpr( Oper, Width, Expr1, Expr2 )

  long     Oper;
  short    Width;
  vexexpr *Expr1;
  vexexpr *Expr2;
{
  vexexpr    *Expr;
  chain_list *ScanOper;
  long        Oper1;
  long        Oper2;
  int         Binary;
  int         Negative;

  Binary = isvexbinaryoper( Oper );

  if ( ! Binary )
  {
    vexerror( VEX_OPERATOR_ERROR, Oper );
  }

  Negative = isvexnegativeoper( Oper );

  if ( IsVexNodeOper( Expr1 ) ) Oper1 = GetVexOperValue( Expr1 );
  else                          Oper1 = -1;

  if ( IsVexNodeOper( Expr2 ) ) Oper2 = GetVexOperValue( Expr2 );
  else                          Oper2 = -1;

  if ( ( Negative                       ) ||
       ( Binary == 2                    ) ||
       ( Oper2        != Oper           ) ||
       ( ( Expr2->WIDTH != Width      ) && 
         ( Oper         != VEX_CONCAT ) ) )
  {
    Expr = createvexoper( Oper, Width );
    addvexhexpr( Expr, Expr2 );
  }
  else
  {
    Expr = Expr2;
  }

  if ( ( Negative                       ) ||
       ( Binary == 2                    ) ||
       ( Oper1        != Oper           ) ||
       ( ( Expr2->WIDTH != Width      ) && 
         ( Oper         != VEX_CONCAT ) ) )
  {
    addvexhexpr( Expr, Expr1 );
  }
  else
  {
    ScanOper = Expr1->OPERAND;

    while ( ScanOper->NEXT != (chain_list *)0 )
    {
      ScanOper = ScanOper->NEXT;
    }

    ScanOper->NEXT = Expr->OPERAND;
    Expr->OPERAND  = Expr1->OPERAND;
    Expr1->OPERAND = (chain_list *)0;

    freevexexpr( Expr1 );
  }

  return( Expr );
}
