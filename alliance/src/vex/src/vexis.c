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
| Tool    :                     Vex                           |
|                                                             |
| File    :                   vexis.c                         |
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
# include "vexis.h"
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

  short VexOperBinary[ VEX_MAX_OPERATOR ] =
  {
    3, /* VEX_CONCAT    */
    0, /* VEX_NOT       */
    0, /* VEX_NEG       */
    0, /* VEX_EVENT     */
    3, /* VEX_OR        */
    3, /* VEX_AND       */
    3, /* VEX_XOR       */
    3, /* VEX_NOR       */
    3, /* VEX_NAND      */
    3, /* VEX_NXOR      */
    2, /* VEX_EQ        */
    2, /* VEX_NE        */
    2, /* VEX_LT        */
    2, /* VEX_LE        */
    2, /* VEX_GT        */
    2, /* VEX_GE        */
    3, /* VEX_ADD       */
    2, /* VEX_SUB       */
    3, /* VEX_MUL       */
    2, /* VEX_DIV       */
    2, /* VEX_EXP       */
    2, /* VEX_MOD       */
    2, /* VEX_REM       */
    3, /* VEX_TO        */
    3, /* VEX_DOWNTO    */
    2, /* VEX_INDEX     */
    0, /* VEX_LEFT      */
    0, /* VEX_RIGHT     */
    0, /* VEX_LOW       */
    0, /* VEX_HIGH      */
    0, /* VEX_LENGTH    */
    0, /* VEX_RANGE     */
    0, /* VEX_REV_RANGE */
    0, /* VEX_DRIVER    */
    2, /* VEX_IFT       */
    4, /* VEX_ARRAY     */
    3, /* VEX_INDEX_N   */
    0, /* VEX_OTHERS    */
    2, /* VEX_NUM_BIT   */
    0, /* VEX_ABS       */
    0, /* AF            */
    0, /* AG            */
    0, /* AX            */
    2, /* AU            */
    1, /* EF            */
    1, /* EG            */
    1, /* EX            */
    2, /* EU            */
    2, /* EQUIV         */
    2  /* IMPLY         */
  };

  short VexOperArith[ VEX_MAX_OPERATOR ] =
  {
    0, /* VEX_CONCAT    */
    0, /* VEX_NOT       */
    1, /* VEX_NEG       */
    0, /* VEX_EVENT     */
    0, /* VEX_OR        */
    0, /* VEX_AND       */
    0, /* VEX_XOR       */
    0, /* VEX_NOR       */
    0, /* VEX_NAND      */
    0, /* VEX_NXOR      */
    1, /* VEX_EQ        */
    1, /* VEX_NE        */
    1, /* VEX_LT        */
    1, /* VEX_LE        */
    1, /* VEX_GT        */
    1, /* VEX_GE        */
    1, /* VEX_ADD       */
    1, /* VEX_SUB       */
    1, /* VEX_MUL       */
    1, /* VEX_DIV       */
    1, /* VEX_EXP       */
    1, /* VEX_MOD       */
    1, /* VEX_REM       */
    0, /* VEX_TO        */
    0, /* VEX_DOWNTO    */
    0, /* VEX_INDEX     */
    0, /* VEX_LEFT      */
    0, /* VEX_RIGHT     */
    0, /* VEX_LOW       */
    0, /* VEX_HIGH      */
    0, /* VEX_LENGTH    */
    0, /* VEX_RANGE     */
    0, /* VEX_REV_RANGE */
    0, /* VEX_DRIVER    */
    0, /* VEX_IFT       */
    0, /* VEX_ARRAY     */
    0, /* VEX_INDEX_N   */
    1, /* VEX_OTHERS    */
    0, /* VEX_NUM_BIT   */
    1, /* VEX_ABS       */
    0, /* AF            */
    0, /* AG            */
    0, /* AX            */
    0, /* AU            */
    0, /* EF            */
    0, /* EG            */
    0, /* EX            */
    0, /* EU            */
    0, /* EQUIV         */
    0  /* IMPLY         */
  };

  short VexOperNegative[ VEX_MAX_OPERATOR ] =
  {
    0, /* VEX_CONCAT    */
    1, /* VEX_NOT       */
    0, /* VEX_NEG       */
    0, /* VEX_EVENT     */
    0, /* VEX_OR        */
    0, /* VEX_AND       */
    0, /* VEX_XOR       */
    1, /* VEX_NOR       */
    1, /* VEX_NAND      */
    1, /* VEX_NXOR      */
    0, /* VEX_EQ        */
    0, /* VEX_NE        */
    0, /* VEX_LT        */
    0, /* VEX_LE        */
    0, /* VEX_GT        */
    0, /* VEX_GE        */
    0, /* VEX_ADD       */
    0, /* VEX_SUB       */
    0, /* VEX_MUL       */
    0, /* VEX_DIV       */
    0, /* VEX_EXP       */
    0, /* VEX_MOD       */
    0, /* VEX_REM       */
    0, /* VEX_TO        */
    0, /* VEX_DOWNTO    */
    0, /* VEX_INDEX     */
    0, /* VEX_LEFT      */
    0, /* VEX_RIGHT     */
    0, /* VEX_LOW       */
    0, /* VEX_HIGH      */
    0, /* VEX_LENGTH    */
    0, /* VEX_RANGE     */
    0, /* VEX_REV_RANGE */
    0, /* VEX_DRIVER    */
    0, /* VEX_IFT       */
    0, /* VEX_ARRAY     */
    0, /* VEX_INDEX_N   */
    0, /* VEX_OTHERS    */
    0, /* VEX_NUM_BIT   */
    0, /* VEX_ABS       */
    0, /* AF            */
    0, /* AG            */
    0, /* AX            */
    0, /* AU            */
    0, /* EF            */
    0, /* EG            */
    0, /* EX            */
    0, /* EU            */
    0, /* EQUIV         */
    0  /* IMPLY         */
  };

  short VexTypeDivisible[ VEX_MAX_TYPE ] =
  {
    0, /* SEVERITY          */
    0, /* BOOLEAN           */
    0, /* CHARACTER         */
    1, /* STRING            */
    0, /* BIT               */
    0, /* INTEGER           */
    0, /* NATURAL           */
    1, /* BIT_VECTOR        */
    0, /* STD_ULOGIC        */
    0, /* STD_LOGIC         */
    1, /* STD_ULOGIC_VECTOR */
    1, /* STD_LOGIC_VECTOR  */
    0, /* X01               */
    0, /* X01Z              */
    0, /* UX01              */
    0, /* UX01Z             */
    1, /* UNSIGNED          */
    1, /* SIGNED            */
    0, /* SMALL_INT         */
    0, /* REG_BIT           */
    1, /* REG_VECTOR        */
    0, /* MUX_BIT           */
    1, /* MUX_VECTOR        */
    0, /* WOR_BIT           */
    1, /* WOR_VECTOR        */
    0, /* ENUMERATE         */
    1  /* ARRAY             */
  };

  short VexTypeVector[ VEX_MAX_TYPE ] =
  {
    0, /* SEVERITY          */
    0, /* BOOLEAN           */
    0, /* CHARACTER         */
    1, /* STRING            */
    0, /* BIT               */
    1, /* INTEGER           */
    1, /* NATURAL           */
    1, /* BIT_VECTOR        */
    0, /* STD_ULOGIC        */
    0, /* STD_LOGIC         */
    1, /* STD_ULOGIC_VECTOR */
    1, /* STD_LOGIC_VECTOR  */
    0, /* X01               */
    0, /* X01Z              */
    0, /* UX01              */
    0, /* UX01Z             */
    1, /* UNSIGNED          */
    1, /* SIGNED            */
    0, /* SMALL_INT         */
    0, /* REG_BIT           */
    1, /* REG_VECTOR        */
    0, /* MUX_BIT           */
    1, /* MUX_VECTOR        */
    0, /* WOR_BIT           */
    1, /* WOR_VECTOR        */
    1, /* ENUMERATE         */
    1  /* ARRAY             */
  };

  static char *VexIsName = (char *)0;
  static long  VexIsOper = (long  )0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Is  Functions                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     Is Vex Type Vector                      |
|                                                             |
\------------------------------------------------------------*/

int isvextypevector( Type )

  int Type;
{
  if ( ( Type < 0             ) ||
       ( Type >= VEX_MAX_TYPE ) )
  {
    return( -1 );
  }

  return( VexTypeVector[ Type ] );
}

/*------------------------------------------------------------\
|                                                             |
|                     Is Vex Type Divisible                   |
|                                                             |
\------------------------------------------------------------*/

int isvextypedivisible( Type )

  int Type;
{
  if ( ( Type < 0             ) ||
       ( Type >= VEX_MAX_TYPE ) )
  {
    return( -1 );
  }

  return( VexTypeDivisible[ Type ] );
}

/*------------------------------------------------------------\
|                                                             |
|                     Is Vex Positive Operator                |
|                                                             |
\------------------------------------------------------------*/

int isvexpositiveoper( Oper )

  long  Oper;
{
  if ( ( Oper < 0                 ) ||
       ( Oper >= VEX_MAX_OPERATOR ) )
  {
    return( -1 );
  }

  return( ! VexOperNegative[ Oper ] );
}

/*------------------------------------------------------------\
|                                                             |
|                     Is Vex Negative Operator                |
|                                                             |
\------------------------------------------------------------*/

int isvexnegativeoper( Oper )

  long  Oper;
{
  if ( ( Oper < 0                 ) ||
       ( Oper >= VEX_MAX_OPERATOR ) )
  {
    return( -1 );
  }

  return( VexOperNegative[ Oper ] );
}

/*------------------------------------------------------------\
|                                                             |
|                     Is Vex Unary Operator                   |
|                                                             |
\------------------------------------------------------------*/

int isvexunaryoper( Oper )

  long  Oper;
{
  if ( ( Oper < 0                 ) ||
       ( Oper >= VEX_MAX_OPERATOR ) )
  {
    return( -1 );
  }

  return( ! VexOperBinary[ Oper ] );
}

/*------------------------------------------------------------\
|                                                             |
|                     Is Vex Binary Operator                  |
|                                                             |
\------------------------------------------------------------*/

int isvexbinaryoper( Oper )

  long  Oper;
{
  if ( ( Oper < 0                 ) ||
       ( Oper >= VEX_MAX_OPERATOR ) )
  {
    return( -1 );
  }

  return( VexOperBinary[ Oper ] );
}

/*------------------------------------------------------------\
|                                                             |
|                     Is Vex Ternary Operator                 |
|                                                             |
\------------------------------------------------------------*/

int isvexternaryoper( Oper )

  long  Oper;
{
  if ( ( Oper < 0                 ) ||
       ( Oper >= VEX_MAX_OPERATOR ) )
  {
    return( -1 );
  }

  return( VexOperBinary[ Oper ] == 3 );
}

/*------------------------------------------------------------\
|                                                             |
|                     Is Local Operator In Expr               |
|                                                             |
\------------------------------------------------------------*/

static int loc_isvexoperinexpr( Expr )

  vexexpr *Expr;
{
  chain_list *ScanOper;

  if ( IsVexNodeAtom( Expr ) )
  {
    return( 0 );
  }

  if ( IsVexNodeOper( Expr ) )
  {
    if ( GetVexOperValue( Expr ) == VexIsOper )
    {
      return( 1 );
    }
  }

  for ( ScanOper  = Expr->OPERAND;
        ScanOper != (chain_list *)0;
        ScanOper  = ScanOper->NEXT )
  {
    if ( loc_isvexoperinexpr( GetVexOperand( ScanOper ) ) )
    {
      return( 1 );
    }
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                     Is Vex Operator In Expr                 |
|                                                             |
\------------------------------------------------------------*/

int isvexoperinexpr( Expr, Oper )

  vexexpr *Expr;
  long     Oper;
{
  VexIsOper = Oper;

  return( loc_isvexoperinexpr( Expr ) );
}

/*------------------------------------------------------------\
|                                                             |
|              Is Local Arithmetic Operator In Expr           |
|                                                             |
\------------------------------------------------------------*/

static int loc_isvexarithoperinexpr( Expr )

  vexexpr *Expr;
{
  chain_list *ScanOper;
  vexexpr    *Operand1;
  vexexpr    *Operand2;
  long        Oper;

  if ( IsVexNodeAtom( Expr ) )
  {
    return( 0 );
  }

  if ( IsVexNodeOper( Expr ) )
  {
    Oper = GetVexOperValue( Expr );

    if ( VexOperArith[ Oper ] )
    {
      if ( ( Oper == VEX_EQ ) ||
           ( Oper == VEX_NE ) )
      {
        Operand1 = GetVexOperand( Expr->OPERAND );
        Operand2 = GetVexOperand( Expr->OPERAND->NEXT );

        if ( Operand1->WIDTH == Operand2->WIDTH )
        {
          return( 0 );
        }
      }

      return( 1 );
    }
  }

  for ( ScanOper  = Expr->OPERAND;
        ScanOper != (chain_list *)0;
        ScanOper  = ScanOper->NEXT )
  {
    if ( loc_isvexarithoperinexpr( GetVexOperand( ScanOper ) ) )
    {
      return( 1 );
    }
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                 Is Vex Arithmetic Operator In Expr          |
|                                                             |
\------------------------------------------------------------*/

int isvexarithoperinexpr( Expr )

  vexexpr *Expr;
{
  return( loc_isvexarithoperinexpr( Expr ) );
}


/*------------------------------------------------------------\
|                                                             |
|                     Is Local Name In Expr                   |
|                                                             |
\------------------------------------------------------------*/

static int loc_isvexnameinexpr( Expr )

  vexexpr *Expr;
{
  chain_list *ScanOper;

  if ( IsVexNodeAtom( Expr ) )
  {
    if ( GetVexAtomValue( Expr ) == VexIsName ) return( 1 );

    return( 0 );
  }

  for ( ScanOper  = Expr->OPERAND;
        ScanOper != (chain_list *)0;
        ScanOper  = ScanOper->NEXT )
  {
    if ( loc_isvexnameinexpr( GetVexOperand( ScanOper ) ) )
    {
      return( 1 );
    }
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                     Is Vex Name In Expr                     |
|                                                             |
\------------------------------------------------------------*/

int isvexnameinexpr( Expr, Name )

  vexexpr *Expr;
  char    *Name;
{
  VexIsName = namealloc( Name );

  return( loc_isvexnameinexpr( Expr ) );
}

/*------------------------------------------------------------\
|                                                             |
|                     Is Vex Equal Expr                       |
|                                                             |
\------------------------------------------------------------*/

int isvexequalexpr( Expr1, Expr2 )

  vexexpr *Expr1;
  vexexpr *Expr2;
{
  chain_list  *ScanOper1;
  chain_list  *ScanOper2;

  if ( ( Expr1 == (vexexpr *)0 ) ||
       ( Expr2 == (vexexpr *)0 ) )
  {
    return( 0 );
  }

  if ( ( Expr1->VALUE != Expr2->VALUE ) ||
       ( Expr1->TYPE  != Expr2->TYPE  ) ||
       ( Expr1->LEFT  != Expr2->LEFT  ) ||
       ( Expr1->RIGHT != Expr2->RIGHT ) )
  {
    return( 0 );
  }


  if ( ! IsVexNodeAtom( Expr1 ) )
  {
    ScanOper1 = Expr1->OPERAND;
    ScanOper2 = Expr2->OPERAND;

    while ( ( ScanOper1 != (chain_list *)0 ) &&
            ( ScanOper2 != (chain_list *)0 ) )
    {
      if ( ! isvexequalexpr( GetVexOperand( ScanOper1 ),
                             GetVexOperand( ScanOper2 ) ) )
      {
        return( 0 );
      }

      ScanOper1 = ScanOper1->NEXT;
      ScanOper2 = ScanOper2->NEXT;
    }

    if ( ( ScanOper1 != (chain_list *)0 ) ||
         ( ScanOper2 != (chain_list *)0 ) )
    {
      return( 0 );
    }
  }

  return( 1 );
}
