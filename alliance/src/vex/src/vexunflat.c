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
| File    :                   vexunflat.c                     |
|                                                             |
| Date    :                   03.12.99                        |
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
# include "vexunflat.h"
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
|                      Unflat Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Unflat Vex Expr                       |
|                                                             |
\------------------------------------------------------------*/

vexexpr *unflatvexexpr( Expr )

  vexexpr *Expr;
{
  vexexpr    *ScanExpr;
  chain_list *ScanChain;
  long        Oper;

  if ( ( Expr == (vexexpr *)0  ) ||
       ( IsVexNodeAtom( Expr ) ) )
  {
    return( Expr );
  }


  if ( IsVexNodeOper( Expr ) )
  {
    Oper = GetVexOperValue( Expr );

    if ( ( Oper == VEX_OR   ) ||
         ( Oper == VEX_AND  ) ||
         ( Oper == VEX_XOR  ) ||
         ( Oper == VEX_NOR  ) ||
         ( Oper == VEX_NAND ) ||
         ( Oper == VEX_NXOR ) ||
         ( Oper == VEX_ADD  ) ||
         ( Oper == VEX_MUL  ) )
    {
      if ( Expr->OPERAND->NEXT->NEXT != (chain_list *)0 )
      {
        if ( isvexnegativeoper( Oper ) )
        {
          Oper = getvexnotoper( Oper );
          SetVexOperValue( Expr, Oper );

          ScanExpr = createvexoper( VEX_NOT, Expr->WIDTH );
          addvexhexpr( ScanExpr, Expr );
          Expr = ScanExpr;
        }
        else
        {
          ScanExpr = createvexoper( Oper, Expr->WIDTH );
          ScanExpr->OPERAND = Expr->OPERAND->NEXT;
          Expr->OPERAND->NEXT = addchain( (chain_list *)0, (void *)ScanExpr );
        }
      }
    }
  }

  for ( ScanChain  = Expr->OPERAND;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    ScanExpr = GetVexOperand( ScanChain );
    ScanExpr = unflatvexexpr( ScanExpr );
    SetVexOperand( ScanChain, ScanExpr );
  }

  return( Expr );
}
