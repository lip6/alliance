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
# include <string.h>
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
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  static char SimpBuffer[ 512 ];
  static int  SimpVexEqual = 0;

  static vexexpr *loc_simpvexconcat();
  static vexexpr *loc_simpvexnot();
  static vexexpr *loc_simpvexconcat();
  static vexexpr *loc_simpvexneg();
  static vexexpr *loc_simpvexabs();
  static vexexpr *loc_simpvexbool3();
  static vexexpr *loc_simpvexequal2();
  static vexexpr *loc_simpvexcomp2();
  static vexexpr *loc_simpvexarith3();
  static vexexpr *loc_simpvexarith2();
  static vexexpr *loc_simpvexslice();
  static vexexpr *loc_simpvexift();
  static vexexpr *loc_simpvexnop();
  static vexexpr *loc_simpvexnumbit();

  static vexexpr *(*SimpFuncArray[ VEX_MAX_OPERATOR ])() =
  {
     loc_simpvexconcat,   /* VEX_CONCAT    */
     loc_simpvexnot,      /* VEX_NOT       */
     loc_simpvexneg,      /* VEX_NEG       */
     loc_simpvexnop,      /* VEX_EVENT     */
     loc_simpvexbool3,    /* VEX_OR        */
     loc_simpvexbool3,    /* VEX_AND       */
     loc_simpvexbool3,    /* VEX_XOR       */
     loc_simpvexbool3,    /* VEX_NOR       */
     loc_simpvexbool3,    /* VEX_NAND      */
     loc_simpvexbool3,    /* VEX_NXOR      */
     loc_simpvexequal2,   /* VEX_EQ        */
     loc_simpvexequal2,   /* VEX_NE        */
     loc_simpvexcomp2,    /* VEX_LT        */
     loc_simpvexcomp2,    /* VEX_LE        */
     loc_simpvexcomp2,    /* VEX_GT        */
     loc_simpvexcomp2,    /* VEX_GE        */
     loc_simpvexarith3,   /* VEX_ADD       */
     loc_simpvexarith2,   /* VEX_SUB       */
     loc_simpvexarith3,   /* VEX_MUL       */
     loc_simpvexarith2,   /* VEX_DIV       */
     loc_simpvexarith2,   /* VEX_EXP       */
     loc_simpvexarith2,   /* VEX_MOD       */
     loc_simpvexarith2,   /* VEX_REM       */
     loc_simpvexslice,    /* VEX_TO        */
     loc_simpvexslice,    /* VEX_DOWNTO    */
     loc_simpvexslice,    /* VEX_INDEX     */
     loc_simpvexnop,      /* VEX_LEFT      */
     loc_simpvexnop,      /* VEX_RIGHT     */
     loc_simpvexnop,      /* VEX_LOW       */
     loc_simpvexnop,      /* VEX_HIGH      */
     loc_simpvexnop,      /* VEX_LENGTH    */
     loc_simpvexnop,      /* VEX_RANGE     */
     loc_simpvexnop,      /* VEX_REV_RANGE */
     loc_simpvexnop,      /* VEX_DRIVER    */
     loc_simpvexift,      /* VEX_IFT       */
     loc_simpvexnop,      /* VEX_ARRAY     */
     loc_simpvexnop,      /* VEX_INDEX_N   */
     loc_simpvexnop,      /* VEX_OTHERS    */
     loc_simpvexnumbit,   /* VEX_NUM_BIT   */
     loc_simpvexabs,      /* VEX_ABS       */
     loc_simpvexnop,      /* AF            */
     loc_simpvexnop,      /* AG            */
     loc_simpvexnop,      /* AX            */
     loc_simpvexnop,      /* AU            */
     loc_simpvexnop,      /* EF            */
     loc_simpvexnop,      /* EG            */
     loc_simpvexnop,      /* EX            */
     loc_simpvexnop,      /* EU            */
     loc_simpvexnop,      /* EQUIV         */
     loc_simpvexnop       /* IMPLY         */
  };

  static short SimpBool3Table[ 3 ][ 3 ] =
  {
    /* Scotch         Neutral       Polarity   */

    { VEX_ONE_ID,     VEX_ZERO_ID,  -1         }, /* VEX_OR  */
    { VEX_ZERO_ID,    VEX_ONE_ID,   -1         }, /* VEX_AND */
    { VEX_UNINIT_ID,  VEX_ZERO_ID,  VEX_ONE_ID }  /* VEX_XOR */
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Simplify Vex Nop                      |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *loc_simpvexnop( Expr )

  vexexpr *Expr;
{
  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                       Simplify Vex Concat                   |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *loc_simpvexconcat( Expr )

  vexexpr *Expr;
{
  chain_list  *ScanOper;
  chain_list  *DelOper;
  chain_list **PrevOper;
  vexexpr     *Operand1;
  vexexpr     *Operand2;
  char        *Value1;
  char        *Value2;
  long         Left1;
  long         Left2;
  long         Right1;
  long         Right2;
  int          Width;
  int          Index;

  Operand1 = (vexexpr *)0;

  for ( ScanOper  = Expr->OPERAND;
        ScanOper != (chain_list *)0;
        ScanOper  = ScanOper->NEXT )
  {
    Operand2 = GetVexOperand( ScanOper );

    if ( (   IsVexNodeAtom( Operand2 )      ) &&
         ( ( IsVexAtomVector( Operand2 )  ) ||
           ( IsVexAtomLiteral( Operand2 ) ) ) )
    {
      if ( Operand1 == (vexexpr *)0 )
      {
        Operand1 = Operand2;
      }
      else
/*
** Verify Operand1 and Operand2
*/
      if ( ( IsVexAtomLiteral( Operand1 ) ) &&
           ( IsVexAtomLiteral( Operand2 ) ) )
      {
/*
** Literal concatenation
*/
        Width = 1;
        SimpBuffer[ 0 ] = '"';
       
        Value1 = GetVexAtomValue( Operand1 );

        for ( Index = 1; Index <= Operand1->WIDTH; Index++ )
        {
          SimpBuffer[ Width++ ] = Value1[ Index ];
        }

        Value2 = GetVexAtomValue( Operand2 );

        for ( Index = 1; Index <= Operand2->WIDTH; Index++ )
        {
          SimpBuffer[ Width++ ] = Value2[ Index ];
        }

        SimpBuffer[ Width++ ] = '"';
        SimpBuffer[ Width   ] = '\0';

        Width = Width - 2;

        SetVexAtomValue( Operand1, namealloc( SimpBuffer ) );
        Operand1->WIDTH = Width;
        Operand1->LEFT  = 0;
        Operand1->RIGHT = Width - 1;

        SetVexNodeTag( Operand2 );
      }
      else
      if ( ( ! IsVexAtomLiteral( Operand1 ) ) &&
           ( ! IsVexAtomLiteral( Operand2 ) ) )
      {
/*
** Vector concatenation
*/
        Value1 = GetVexAtomValue( Operand1 );
        Value2 = GetVexAtomValue( Operand2 );

        if ( Value1 == Value2 )
        {
          Left1  = Operand1->LEFT;
          Right1 = Operand1->RIGHT;
          Left2  = Operand2->LEFT;
          Right2 = Operand2->RIGHT;

          if ( ( ( Right1 - 1 ) == Left2  ) &&
               (   Left1        >= Right1 ) &&
               (   Right1       >= Right2 ) )
          {
            Operand1->LEFT  = Left1;
            Operand1->RIGHT = Right2;
            Operand1->WIDTH = (Left1 - Right2) + 1;

            SetVexAtomDown( Operand1 );
            SetVexNodeTag( Operand2 );
          }
          else
          if ( ( ( Right1 + 1 ) == Left2  ) &&
               (   Left1        <= Right1 ) &&
               (   Right1       <= Right2 ) )
          {
            Operand1->LEFT  = Left1;
            Operand1->RIGHT = Right2;
            Operand1->WIDTH = (Right2 - Left1) + 1;

            SetVexAtomUp( Operand1 );
            SetVexNodeTag( Operand2 );
          }
          else
          {
            Operand1 = Operand2;
          }
        }
        else
        {
          Operand1 = Operand2;
        }
      }
      else
      {
        Operand1 = Operand2;
      }
    }
    else
    {
      Operand1 = (vexexpr *)0;
    }
  }

  PrevOper = &Expr->OPERAND;
  ScanOper = Expr->OPERAND;

  while ( ScanOper != (chain_list *)0 )
  {
    Operand1 = GetVexOperand( ScanOper );

    if ( IsVexNodeTag( Operand1 ) )
    {
      *PrevOper = ScanOper->NEXT;
      DelOper   = ScanOper;
      ScanOper  = ScanOper->NEXT;

      DelOper->NEXT = (chain_list *)0;
      freechain( DelOper );

      freevexexpr( Operand1 );
    }
    else
    {
      PrevOper = &ScanOper->NEXT;
      ScanOper = ScanOper->NEXT;
    }
  }

  if ( Expr->OPERAND->NEXT == (chain_list *)0 )
  {
    Operand1 = GetVexOperand( Expr->OPERAND );
    SetVexOperand( Expr->OPERAND, (vexexpr *)0 );

    freevexexpr( Expr );
    Expr = Operand1;
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                       Simplify Vex Not                      |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *loc_simpvexnot( Expr )

  vexexpr *Expr;
{
  vexexpr *Operand;
  vexexpr *Operand2;
  char    *Value;
  long     Oper;
  int      Index;

  Operand = GetVexOperand( Expr->OPERAND );

  if ( ( IsVexNodeAtom( Operand    ) ) &&
       ( IsVexAtomLiteral( Operand ) ) )
  {
    Value = GetVexAtomValue( Operand );

    SimpBuffer[ 0 ] = Value[ 0 ];

    for ( Index = 1; Index <= Operand->WIDTH; Index++ )
    {
      evalvexnotliteral( Value[ Index ], &SimpBuffer[ Index ] );
    }

    SimpBuffer[ Index++ ] = Value[ 0 ];
    SimpBuffer[ Index++ ] = '\0';

    SetVexAtomValue( Operand, namealloc( SimpBuffer ) );
    SetVexOperand( Expr->OPERAND, (vexexpr *)0 );
    freevexexpr( Expr );

    Expr = Operand;
  }
  else
  if ( IsVexNodeOper( Operand ) )
  {
    Oper = GetVexOperValue( Operand );

    if ( Oper == VEX_NOT )
    {
      Operand2 = GetVexOperand( Operand->OPERAND );
      SetVexOperand( Operand->OPERAND, (vexexpr *)0 );
      freevexexpr( Expr );

      Expr = Operand2;
    }
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                       Simplify Vex Abs                      |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *loc_simpvexabs( Expr )

  vexexpr *Expr;
{
  vexexpr      *Operand;
  long          Value;

  Operand = GetVexOperand( Expr->OPERAND );

  if ( evalvexatomlong( Operand, &Value ) != -1 )
  {
    if ( Value < 0 ) Value = - Value;
    freevexexpr( Expr );

    Expr = createvexatomlong( Value, 0, 1 );
  }

  return( Expr );
}


/*------------------------------------------------------------\
|                                                             |
|                       Simplify Vex Neg                      |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *loc_simpvexneg( Expr )

  vexexpr *Expr;
{
  vexexpr      *Operand;
  long          Value;

  Operand = GetVexOperand( Expr->OPERAND );

  if ( evalvexatomlong( Operand, &Value ) != -1 )
  {
    Value = - Value;
    freevexexpr( Expr );

    Expr = createvexatomlong( Value, 0, 1 );
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                       Is Operand Same Width                 |
|                                                             |
\------------------------------------------------------------*/

static int loc_isoperandsamewidth( Expr )

  vexexpr *Expr;
{
  chain_list *ScanOper;
  vexexpr    *Operand;
  int         Width;

  for ( ScanOper  = Expr->OPERAND;
        ScanOper != (chain_list *)0;
        ScanOper  = ScanOper->NEXT )
  {
    Operand = GetVexOperand( ScanOper );

    if ( ScanOper == Expr->OPERAND )
    {
      Width = Operand->WIDTH;
    }
    else
    if ( Width != Operand->WIDTH )
    {
      return( 0 );
    }
  }

  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                       Simplify Vex Boolean 3                |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *loc_simpvexbool3( Expr )

  vexexpr *Expr;
{
  chain_list  *ScanOper;
  chain_list **PrevOper;
  chain_list  *DelOper;
  vexexpr     *Operand;
  vexexpr     *VexSum;
  int          NumOper;
  int          NumSum;
  int          Index;
  int          AllSame;
  int          Negative;
  int          LiteralId;
  char         Literal;
  long         Oper;
  char        *Sum;
  char        *Value;

  Oper = GetVexOperValue( Expr );

  NumOper = 0;
  NumSum  = 0;
  Sum     = 0;
  VexSum  = (vexexpr *)0;

  if ( ! loc_isoperandsamewidth( Expr ) )
  {
    return( Expr );
  }

  for ( ScanOper  = Expr->OPERAND;
        ScanOper != (chain_list *)0;
        ScanOper  = ScanOper->NEXT )
  {
    Operand = GetVexOperand( ScanOper );
    NumOper++;

    if ( ( IsVexNodeAtom( Operand )    ) &&
         ( IsVexAtomLiteral( Operand ) ) )
    {
      Value = GetVexAtomValue( Operand );
      SetVexNodeTag( Operand );

      if ( NumSum == 0 )
      {
        Sum    = Value;
        VexSum = Operand;
      }
      else
      {
        SimpBuffer[ 0 ] = Value[ 0 ];

        for ( Index = 1; Index <= Operand->WIDTH; Index++ )
        {
          if ( ( Oper == VEX_AND ) || ( Oper == VEX_NAND ) )
            evalvexandliteral( Sum[ Index ], Value[ Index ], &SimpBuffer[ Index ] );
          else
          if ( ( Oper == VEX_OR  ) || ( Oper == VEX_NOR  ) )
            evalvexorliteral( Sum[ Index ], Value[ Index ], &SimpBuffer[ Index ] );
          else
          if ( ( Oper == VEX_XOR ) || ( Oper == VEX_NXOR ) )
            evalvexxorliteral( Sum[ Index ], Value[ Index ], &SimpBuffer[ Index ] );
          else
          vexerror( VEX_OPERATOR_ERROR, Oper );
        }

        SimpBuffer[ Index++ ] = Value[ 0 ];
        SimpBuffer[ Index++ ] = '\0';

        Sum = namealloc( SimpBuffer );
      }

      NumSum++;
    }
  }

  if ( NumSum > 1 )
  {
    VexSum = dupvexnode( VexSum );
    ClearVexNodeTag( VexSum );
    SetVexAtomValue( VexSum, Sum );

    if ( NumSum == NumOper )
    {
      if ( isvexnegativeoper( Oper ) )
      {
        VexSum = optimvexnotexpr( VexSum );
      }

      freevexexpr( Expr );

      Expr = VexSum;
    }
    else
    {
      PrevOper = &Expr->OPERAND;
      ScanOper = Expr->OPERAND;

      while ( ScanOper != (chain_list *)0 )
      {
        Operand = GetVexOperand( ScanOper );

        if ( IsVexNodeTag( Operand ) )
        {
          *PrevOper = ScanOper->NEXT;
          DelOper   = ScanOper;
          ScanOper  = ScanOper->NEXT;

          DelOper->NEXT = (chain_list *)0;
          freechain( DelOper );

          freevexexpr( Operand );
        }
        else
        {
          PrevOper = &ScanOper->NEXT;
          ScanOper = ScanOper->NEXT;
        }
      }

      addvexhexpr( Expr, VexSum );
    }
  }
  else
  if ( NumSum == 1 )
  {
    ClearVexNodeTag( VexSum );
  }

  if ( NumSum > 0 )
  {
    NumSum    = 0;
    Negative  = isvexnegativeoper( Oper );

    if ( Negative )
    {
      Oper = getvexnotoper( Oper );
      SetVexOperValue( Expr, Oper );
    }

    Oper     = Oper - VEX_OR;
    ScanOper = Expr->OPERAND;
    PrevOper = &Expr->OPERAND;

    while ( ScanOper != (chain_list *)0 )
    {
      Operand = GetVexOperand( ScanOper );

      if ( ( IsVexNodeAtom( Operand )    ) &&
           ( IsVexAtomLiteral( Operand ) ) )
      {
        Value   = GetVexAtomValue( Operand );
        Literal = Value[ 1 ];
        AllSame = 1;

        for ( Index = 2; Index <= Operand->WIDTH; Index++ )
        {
          if ( Literal != Value[ Index ] )
          {
            AllSame = 0; break;
          }
        }

        if ( AllSame )
        {
          LiteralId = getvexliteralid( Literal );
/*
**  The literal is equal to the "scotch" literal
*/
          if ( SimpBool3Table[ Oper ][ 0 ] == LiteralId )
          {
            SetVexOperand( ScanOper, (vexexpr *)0 );
            freevexexpr( Expr );

            if ( Negative )
            {
              Operand = optimvexnotexpr( Operand );
            }

            return( Operand );
          }
/*
**  The literal is equal to the neutral or polarity inverse literal
*/
          if ( ( SimpBool3Table[ Oper ][ 1 ] == LiteralId ) ||
               ( SimpBool3Table[ Oper ][ 2 ] == LiteralId ) )
          {
            *PrevOper = ScanOper->NEXT;
            DelOper   = ScanOper;
            ScanOper  = ScanOper->NEXT;

            DelOper->NEXT = (chain_list *)0;
            freechain( DelOper );

            freevexexpr( Operand );

            if ( SimpBool3Table[ Oper ][ 2 ] == LiteralId )
            {
              Negative = ! Negative;
            }

            continue;
          }
        }
      }

      NumSum++;

      PrevOper = &ScanOper->NEXT;
      ScanOper = ScanOper->NEXT;
    }

    if ( NumSum == 1 )
    {
      Operand = GetVexOperand( Expr->OPERAND );
      SetVexOperand( Expr->OPERAND, (vexexpr *)0 );
      freevexexpr( Expr );

      Expr = Operand;
    }

    if ( Negative )
    {
      Expr = optimvexnotexpr( Expr );
    }
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                       Simplify Vex Ift                      |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *loc_simpvexift( Expr )

  vexexpr *Expr;
{
  chain_list *ScanCond;
  chain_list *ScanData;
  vexexpr    *VexCond;
  vexexpr    *VexData;
  char       *Value;
  int         Index;
  int         LiteralId;
  int         AllSame;
  char        Literal;

  ScanCond = Expr->OPERAND;
  VexCond  = GetVexOperand( ScanCond );
  ScanData = ScanCond->NEXT;
  VexData  = GetVexOperand( ScanData );

  if ( VexData->WIDTH != Expr->WIDTH ) return( Expr );

  if ( ( IsVexNodeAtom( VexCond )    ) &&
       ( IsVexAtomLiteral( VexCond ) ) )
  {
    Value     = GetVexAtomValue( VexCond );
    Literal   = Value[ 1 ];
    LiteralId = getvexliteralid( Literal );

    if ( LiteralId == VEX_ONE_ID )
    {
      SetVexOperand( ScanData, (vexexpr *)0 );
      freevexexpr( Expr );
      
      return( VexData );
    }
    else
    if ( LiteralId == VEX_ZERO_ID )
    {
      SimpBuffer[ 0 ] = Value[ 0 ];

      for ( Index = 1; Index <= Expr->WIDTH; Index++ )
      {
        SimpBuffer[ Index ] = VEX_ZERO;
      }

      SimpBuffer[ Index++ ] = Value[ 0 ];
      SimpBuffer[ Index++ ] = '\0';

      freevexexpr( Expr );
      VexData = createvexatomlit( SimpBuffer );

      return( VexData );
    }
  }

  if ( ( IsVexNodeAtom( VexData )    ) &&
       ( IsVexAtomLiteral( VexData ) ) )
  {
    Value     = GetVexAtomValue( VexData );
    Literal   = Value[ 1 ];

    AllSame = 1; 

    for ( Index = 2; Index <= VexData->WIDTH; Index++ )
    {
      if ( Literal != Value[ Index ] )
      {
        AllSame = 0; break;
      }
    }

    if ( AllSame )
    {
      LiteralId = getvexliteralid( Literal );

      if ( LiteralId == VEX_ZERO_ID )
      {
        SetVexOperand( ScanData, (vexexpr *)0 );
        freevexexpr( Expr );
      
        return( VexData );
      }

      if ( ( LiteralId == VEX_ONE_ID ) &&
           ( VexData->WIDTH == 1     ) )
      {
        SetVexOperand( ScanCond, (vexexpr *)0 );
        freevexexpr( Expr );

        return( VexCond );
      }
    }
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                       Simplify Vex Compare 2                |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *loc_simpvexcomp2( Expr )

  vexexpr *Expr;
{
  vexexpr     *Operand1;
  vexexpr     *Operand2;
  long         Value1;
  long         Value2;
  long         Value;
  long         Oper;

  Oper  = GetVexOperValue( Expr );
  Value = 0;

  Operand1 = GetVexOperand( Expr->OPERAND       );
  Operand2 = GetVexOperand( Expr->OPERAND->NEXT );

  if ( ( evalvexatomlong( Operand1, &Value1 ) != -1 ) &&
       ( evalvexatomlong( Operand2, &Value2 ) != -1 ) )
  {
    if ( Oper == VEX_LT ) Value = (Value1 <  Value2);
    else
    if ( Oper == VEX_LE ) Value = (Value1 <= Value2);
    else
    if ( Oper == VEX_GT ) Value = (Value1 >  Value2);
    else
    if ( Oper == VEX_GE ) Value = (Value1 >= Value2);
    else
    vexerror( VEX_OPERATOR_ERROR, Oper );

    freevexexpr( Expr );

    if ( Value ) Expr = createvexatombit( VEX_ATOM_ONE  );
    else         Expr = createvexatombit( VEX_ATOM_ZERO );
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                       Simplify Vex Equal 2                  |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *loc_simpvexequal2( Expr )

  vexexpr *Expr;
{
  vexexpr     *Operand1;
  vexexpr     *Operand2;
  vexexpr     *NewExpr;
  vexexpr     *Atom1;
  vexexpr     *Atom2;
  vexexpr     *XorExpr;
  char        *Value1;
  char        *Value2;
  int          ScanPos;
  int          Index1;
  int          Index2;
  int          NumSum;
  char         Literal;
  long         Value;
  long         Oper;

  Oper = GetVexOperValue( Expr );

  Operand1 = GetVexOperand( Expr->OPERAND       );
  Operand2 = GetVexOperand( Expr->OPERAND->NEXT );

  if ( ( IsVexNodeAtom( Operand1    ) ) &&
       ( IsVexAtomLiteral( Operand1 ) ) &&
       ( IsVexNodeAtom( Operand2    ) ) &&
       ( IsVexAtomLiteral( Operand2 ) ) )
  {
    if ( Operand1->WIDTH != Operand2->WIDTH )
    {
      if ( Operand1->WIDTH > Operand2->WIDTH )
      {
        extendvexatomsign( Operand2, Operand1->WIDTH );
      }
      else
      {
        extendvexatomsign( Operand1, Operand2->WIDTH );
      }
    }

    Value1 = GetVexAtomValue( Operand1 );
    Value2 = GetVexAtomValue( Operand2 );

    Value = ! strcmp( Value1, Value2 );

    if ( Oper == VEX_NE ) Value = ! Value;
    else
    if ( Oper != VEX_EQ ) vexerror( VEX_OPERATOR_ERROR, Oper );

    freevexexpr( Expr );

    if ( Value ) Expr = createvexatombit( VEX_ATOM_ONE  );
    else         Expr = createvexatombit( VEX_ATOM_ZERO );
  }
  else
  if ( (          SimpVexEqual           ) &&
       ( ! IsVexNodeVarWidth( Operand1 ) ) &&
       ( ! IsVexNodeVarWidth( Operand2 ) ) )
  {
    if ( ( Operand1->WIDTH == 1 ) &&
         ( Operand2->WIDTH == 1 ) )
    {
      if ( Oper == VEX_EQ ) SetVexOperValue( Expr, VEX_NXOR );
      else                  SetVexOperValue( Expr, VEX_XOR  );

      Expr = loc_simpvexbool3( Expr );
    }
    else
    if ( ( IsVexNodeAtom( Operand1 ) ) &&
         ( IsVexNodeAtom( Operand2 ) ) )
    {
      if ( ( Operand1->WIDTH == Operand2->WIDTH ) ||
           ( IsVexAtomLiteral( Operand1 )       ) ||
           ( IsVexAtomLiteral( Operand2 )       ) )
      {
        if ( IsVexAtomLiteral( Operand2 ) )
        {
          NewExpr  = Operand1;
          Operand1 = Operand2;
          Operand2 = NewExpr;
        }

        if ( Operand1->WIDTH != Operand2->WIDTH )
        {
          extendvexatomsign( Operand1, Operand2->WIDTH );
        }
       
        Value1 = GetVexAtomValue( Operand1 );
        Value2 = GetVexAtomValue( Operand2 );
       
        if ( Oper == VEX_EQ ) NewExpr = createvexoper( VEX_AND, 1 );
        else                  NewExpr = createvexoper( VEX_OR , 1 );
       
        NumSum = 0;
       
        for ( ScanPos = 0; ScanPos < Operand2->WIDTH; ScanPos++ )
        {
          Index2 = getvexvectorindex( Operand2, ScanPos );
          Atom2  = createvexatomvec( Value2, Index2, Index2 );
       
          if ( IsVexAtomLiteral( Operand1 ) )
          {
            Literal = Value1[ ScanPos + 1 ];
       
            if ( Literal == VEX_DC  ) continue;
       
            XorExpr = Atom2;
       
            if ( Literal != VEX_ONE )
            {
              XorExpr = optimvexnotexpr( XorExpr );
            }
          }
          else
          {
            Index1 = getvexvectorindex( Operand1, ScanPos );
            Atom1  = createvexatomvec( Value1, Index1, Index1 );
       
            XorExpr = createvexbinexpr( VEX_NXOR, 1, Atom1, Atom2 );
          }
       
          addvexhexpr( NewExpr, XorExpr );
       
          NumSum++;
        }
       
        freevexexpr( Expr );
       
        if ( NumSum == 0 )
        {
          freevexexpr( NewExpr );
       
          if ( Oper == VEX_EQ ) Expr = createvexatomlit( VEX_ATOM_ONE );
          else                  Expr = createvexatomlit( VEX_ATOM_ZERO );
        }
        else
        if ( NumSum == 1 )
        {
          Expr = GetVexOperand( NewExpr->OPERAND );
       
          SetVexOperand( NewExpr->OPERAND, (vexexpr *)0 );
          freevexexpr( NewExpr );
        }
        else
        {
          Expr = NewExpr;
        }
      }
    }
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|             Simplify Vex Arithmetic > 2 Operand             |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *loc_simpvexarith3( Expr )

  vexexpr *Expr;
{
  chain_list  *ScanOper;
  chain_list **PrevOper;
  chain_list  *DelOper;
  vexexpr     *Operand;
  vexexpr     *VexInt;
  int          NumOper;
  int          NumSum;
  long         Oper;
  long         Sum;
  long         Value;

  Oper = GetVexOperValue( Expr );

  NumOper = 0;
  NumSum  = 0;
  Sum     = 0;
  VexInt  = (vexexpr *)0;

  for ( ScanOper  = Expr->OPERAND;
        ScanOper != (chain_list *)0;
        ScanOper  = ScanOper->NEXT )
  {
    Operand = GetVexOperand( ScanOper );
    NumOper++;

    if ( evalvexatomlong( Operand, &Value ) != -1 )
    {
      SetVexNodeTag( Operand );

      if ( NumSum == 0 )
      {
        Sum    = Value;
        VexInt = Operand;
      }
      else
      {
        if ( Oper == VEX_ADD ) Sum += Value;
        else                   
        if ( Oper == VEX_MUL ) Sum *= Value;
        else
        vexerror( VEX_OPERATOR_ERROR, Oper );
      }

      NumSum++;
    }
  }

  if ( NumSum > 1 )
  {
    VexInt = createvexatomlong( Sum, 0, IsVexAtomSigned( Expr ) );

    if ( NumSum == NumOper )
    {
      freevexexpr( Expr );

      Expr = VexInt;
    }
    else
    {
      PrevOper = &Expr->OPERAND;
      ScanOper = Expr->OPERAND;

      while ( ScanOper != (chain_list *)0 )
      {
        Operand = GetVexOperand( ScanOper );

        if ( IsVexNodeTag( Operand ) )
        {
          *PrevOper = ScanOper->NEXT;
          DelOper   = ScanOper;
          ScanOper  = ScanOper->NEXT;

          DelOper->NEXT = (chain_list *)0;
          freechain( DelOper );

          freevexexpr( Operand );
        }
        else
        {
          PrevOper = &ScanOper->NEXT;
          ScanOper = ScanOper->NEXT;
        }
      }

      addvexhexpr( Expr, VexInt );
    }
  }
  else
  if ( NumSum == 1 )
  {
    ClearVexNodeTag( VexInt );
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                   Simplify Vex Arithmetic 2 Operands        |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *loc_simpvexarith2( Expr )

  vexexpr *Expr;
{
  vexexpr     *Operand1;
  vexexpr     *Operand2;
  vexexpr     *VexInt;
  long         Value1;
  long         Value2;
  long         Value;
  long         Oper;

  Oper  = GetVexOperValue( Expr );
  Value = 0;

  Operand1 = GetVexOperand( Expr->OPERAND       );
  Operand2 = GetVexOperand( Expr->OPERAND->NEXT );

  if ( ( evalvexatomlong( Operand1, &Value1 ) != -1 ) &&
       ( evalvexatomlong( Operand2, &Value2 ) != -1 ) )
  {
    if ( Oper == VEX_SUB )
    {
      Value = Value1 - Value2;
    }
    else
    if ( Oper == VEX_EXP )
    {
      if ( Value1 != 2 ) vexerror( VEX_OPERATOR_ERROR, Oper );

      if ( Value2 == 0 ) Value = 1;
      else
      if ( Value2 <  0 ) Value = 0;
      else
                         Value = 2L << (Value2 - 1);
    }
    else
    if ( Oper == VEX_MOD )
    {
      if ( ( ( Value1 > 0 ) && 
             ( Value2 > 0 ) ) ||
           ( ( Value1 < 0 ) &&
             ( Value2 < 0 ) ) )
      {
        Value = Value1 % Value2;
      }
      else
      if ( Value1 < 0 )
      {
        Value = -((-Value1) % Value2);
      }
      else
      {
        Value = -(Value1 % (-Value2));
      }
    }
    else
    if ( Oper == VEX_REM )
    {
      Value = Value1 % Value2;
    }
    else
    if ( Oper == VEX_DIV )
    {
      if ( Value2 == 0 ) vexerror( VEX_OPERATOR_ERROR, Oper );

      Value = Value1 / Value2;
    }
    else vexerror( VEX_OPERATOR_ERROR, Oper );

    VexInt = createvexatomlong( Value, 0, IsVexAtomSigned( Expr ) );

    freevexexpr( Expr ); Expr = VexInt;
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                     Simplify Vex Number Bit                 |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *loc_simpvexnumbit( Expr )

  vexexpr *Expr;
{
  vexexpr *Operand1;
  vexexpr *Operand2;
  long     Value1;
  long     Value2;
  int      Size;

  Operand1 = GetVexOperand( Expr->OPERAND       );
  Operand2 = GetVexOperand( Expr->OPERAND->NEXT );

  if ( ( evalvexatomlong( Operand1, &Value1 ) != -1 ) &&
       ( evalvexatomlong( Operand2, &Value2 ) != -1 ) )
  {
    Size = getvexintervalnumbit( Value1, Value2 );

    freevexexpr( Expr );
    Expr = createvexatomlong( Size - 1, 0, 0 );
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                       Simplify Vex Slice                    |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *loc_simpvexslice( Expr )

  vexexpr *Expr;
{
  vexexpr *Atom;
  vexexpr *Operand1;
  vexexpr *Operand2;
  long     Oper;
  long     Value1;
  long     Value2;
  char    *Value;

  Oper     = GetVexOperValue( Expr );
  Atom     = GetVexOperand( Expr->OPERAND );
  Operand1 = GetVexOperand( Expr->OPERAND->NEXT );

  if ( Oper != VEX_INDEX )
  { 
    Operand2 = GetVexOperand( Expr->OPERAND->NEXT->NEXT );
  }
  else
  {
    Operand2 = Operand1;
  }

  if ( ( IsVexNodeAtom( Atom )                      ) &&
       ( evalvexatomlong( Operand1, &Value1 ) != -1 ) &&
       ( evalvexatomlong( Operand2, &Value2 ) != -1 ) )
  {
    if ( ( Oper == VEX_TO     ) ||
         ( Oper == VEX_DOWNTO ) ||
         ( Oper == VEX_INDEX  ) )
    {
      if ( ( Value1 < 0 ) ||
           ( Value2 < 0 ) )
      {
        vexerror( VEX_OPERATOR_ERROR, Oper );
      }
      
      if ( ( ( Oper   == VEX_TO     ) &&
             ( Value1 >  Value2     ) ) ||
           ( ( Oper   == VEX_DOWNTO ) &&
             ( Value1 < Value2      ) ) )
      {
        vexerror( VEX_OPERATOR_ERROR, Oper );
      }

      Value = GetVexAtomValue( Atom );
      freevexexpr( Expr );
      Expr = createvexatomvec( Value, Value1, Value2 );
    }
    else vexerror( VEX_OPERATOR_ERROR, Oper );
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                       Simplify Vex Expr                     |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *loc_simpvexexpr( Expr )

  vexexpr *Expr;
{
  vexexpr    *Operand;
  chain_list *ScanOper;
  long        Oper;

  if ( ! IsVexNodeAtom( Expr ) )
  {
    for ( ScanOper  = Expr->OPERAND;
          ScanOper != (chain_list *)0;
          ScanOper  = ScanOper->NEXT )
    {
      Operand = loc_simpvexexpr( GetVexOperand( ScanOper ) );
      SetVexOperand( ScanOper, Operand );
    }

    if ( IsVexNodeOper( Expr ) )
    {
      Oper = GetVexOperValue( Expr );

      if ( Oper < VEX_MAX_OPERATOR ) 
      {
        Expr = (*SimpFuncArray[ Oper ])( Expr );
      }
      else
      vexerror( VEX_OPERATOR_ERROR, Oper );
    }
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                       Simplify Vex Expr                     |
|                                                             |
\------------------------------------------------------------*/

vexexpr *simpvexexpr( Expr )

  vexexpr *Expr;
{
  if ( Expr != (vexexpr *)0 )
  {
    Expr = loc_simpvexexpr( Expr );
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                   Simplify Vex Expr Equal                   |
|                                                             |
\------------------------------------------------------------*/

vexexpr *simpvexexpreq( Expr )

  vexexpr *Expr;
{
  if ( Expr != (vexexpr *)0 )
  {
    SimpVexEqual = 1;
    Expr = loc_simpvexexpr( Expr );
    SimpVexEqual = 0;
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                   Simplify Vex Expr Others                  |
|                                                             |
\------------------------------------------------------------*/

vexexpr *simpvexexprothers( Expr, Width )

  vexexpr *Expr;
  short    Width;
{
  vexexpr    *Operand;
  vexexpr    *VexAtom;
  chain_list *ScanOper;
  char       *Value;
  short       ScanWidth;
  long        Oper;

  if ( ( Width  >= 0            ) &&
       ( Expr   != (vexexpr *)0 ) )
  {
    if ( IsVexNodeOper( Expr ) )
    {
      Oper = GetVexOperValue( Expr );

      if ( Oper == VEX_OTHERS )
      {
        Operand = GetVexOperand( Expr->OPERAND );

        if ( ( IsVexNodeAtom( Operand )    ) &&
             ( IsVexAtomLiteral( Operand ) ) )
        {
          Value   = GetVexAtomValue( Operand );
          VexAtom = createvexatomveclit( Value[ 1 ], Width );

          freevexexpr( Expr );
          Expr = VexAtom;
        }
      }
      else
      if ( Oper == VEX_CONCAT )
      {
        ScanWidth = 0;

        for ( ScanOper        = Expr->OPERAND;
              ScanOper->NEXT != (chain_list *)0;
              ScanOper        = ScanOper->NEXT )
        {
          Operand    = GetVexOperand( ScanOper );
          ScanWidth += Operand->WIDTH;
        }

        Operand = GetVexOperand( ScanOper );
        Operand = simpvexexprothers( Operand, Width - ScanWidth );
        SetVexOperand( ScanOper, Operand );
      }
    }
  }

  return( Expr );
}
