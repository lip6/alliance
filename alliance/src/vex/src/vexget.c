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
| File    :                   vexget.c                        |
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
# include <stdlib.h>
# include "vexget.h"
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

  long VEX_NOT_OPER[ VEX_MAX_OPERATOR ] =
  {
    -1,         /* VEX_CONCAT     */
    -1,         /* VEX_NOT        */
    -1,         /* VEX_NEG        */
    -1,         /* VEX_EVENT      */
    VEX_NOR,    /* VEX_OR         */
    VEX_NAND,   /* VEX_AND        */
    VEX_NXOR,   /* VEX_XOR        */
    VEX_OR,     /* VEX_NOR        */
    VEX_AND,    /* VEX_NAND       */
    VEX_XOR,    /* VEX_NXOR       */
    VEX_NE,     /* VEX_EQ         */
    VEX_EQ,     /* VEX_NE         */
    VEX_GE,     /* VEX_LT         */
    VEX_GT,     /* VEX_LE         */
    VEX_LE,     /* VEX_GT         */
    VEX_LT,     /* VEX_GE         */
    -1,         /* VEX_ADD        */
    -1,         /* VEX_SUB        */
    -1,         /* VEX_MUL        */
    -1,         /* VEX_DIV        */
    -1,         /* VEX_EXP        */
    -1,         /* VEX_MOD        */
    -1,         /* VEX_REM        */
    -1,         /* VEX_TO         */
    -1,         /* VEX_DOWNTO     */
    -1,         /* VEX_INDEX      */
    -1,         /* VEX_LEFT       */
    -1,         /* VEX_RIGHT      */
    -1,         /* VEX_LOW        */
    -1,         /* VEX_HIGH       */
    -1,         /* VEX_LENGTH     */
    -1,         /* VEX_RANGE      */
    -1,         /* VEX_REV_RANGE  */
    -1,         /* VEX_DRIVER     */
    -1,         /* VEX_IFT        */
    -1,         /* VEX_ARRAY      */
    -1,         /* VEX_INDEX_N    */
    -1,         /* VEX_OTHERS     */
    -1,         /* VEX_NUM_BIT    */
    -1,         /* VEX_ABS        */
    -1,         /* AF             */
    -1,         /* AG             */
    -1,         /* AX             */
    -1,         /* AU             */
    -1,         /* EF             */
    -1,         /* EG             */
    -1,         /* EX             */
    -1,         /* EU             */
    -1,         /* EQUIV          */
    -1          /* IMPLY          */
  };

  short VexTypeScalar[ VEX_MAX_TYPE ] =
  {
    VEX_TYPE_SEVERITY          , /* SEVERITY          */
    VEX_TYPE_BOOLEAN           , /* BOOLEAN           */
    VEX_TYPE_CHARACTER         , /* CHARACTER         */
    VEX_TYPE_CHARACTER         , /* STRING            */
    VEX_TYPE_BIT               , /* BIT               */
    VEX_TYPE_INTEGER           , /* INTEGER           */
    VEX_TYPE_NATURAL           , /* NATURAL           */
    VEX_TYPE_BIT               , /* BIT_VECTOR        */
    VEX_TYPE_STD_ULOGIC        , /* STD_ULOGIC        */
    VEX_TYPE_STD_LOGIC         , /* STD_LOGIC         */
    VEX_TYPE_STD_ULOGIC        , /* STD_ULOGIC_VECTOR */
    VEX_TYPE_STD_LOGIC         , /* STD_LOGIC_VECTOR  */
    VEX_TYPE_X01               , /* X01               */
    VEX_TYPE_X01Z              , /* X01Z              */
    VEX_TYPE_UX01              , /* UX01              */
    VEX_TYPE_UX01Z             , /* UX01Z             */
    VEX_TYPE_UNSIGNED          , /* UNSIGNED          */
    VEX_TYPE_SIGNED            , /* SIGNED            */
    VEX_TYPE_SMALL_INT         , /* SMALL_INT         */
    VEX_TYPE_REG_BIT           , /* REG_BIT           */
    VEX_TYPE_REG_BIT           , /* REG_VECTOR        */
    VEX_TYPE_MUX_BIT           , /* MUX_BIT           */
    VEX_TYPE_MUX_BIT           , /* MUX_VECTOR        */
    VEX_TYPE_WOR_BIT           , /* WOR_BIT           */
    VEX_TYPE_WOR_BIT           , /* WOR_VECTOR        */
    VEX_TYPE_ENUMERATE         , /* ENUMERATE         */
    VEX_TYPE_ARRAY               /* ARRAY             */
  };

  static chain_list *VexGetHeadChain = (chain_list *)0;
  static ptype_list *VexGetHeadPtype = (ptype_list *)0;
  static char        VexBuffer[ 128 ];

  static authtable  *VexHashStdFunc = (authtable *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Get Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Get Vex Int Number Bit                |
|                                                             |
\------------------------------------------------------------*/

int getvexintnumbit( Size )

  long Size;
{
  long Mask;
  int  NumberBit;

  if ( Size <  0 ) return( 0 );
  if ( Size == 0 ) return( 1 );

  Mask = 1U << 31;

  for ( NumberBit = 32; NumberBit > 0; NumberBit-- )
  {
    if ( Mask & Size ) break;

    Mask = Mask >> 1;
  }

  return( NumberBit );
}

/*------------------------------------------------------------\
|                                                             |
|                   Get Vex Interval Number Bits              |
|                                                             |
\------------------------------------------------------------*/

int getvexintervalnumbit( Left, Right )

  long Left;
  long Right;
{
  long NumberBit;
  long Swap;
  long Max;
 
  if ( Left > Right )
  {
    Swap  = Left;
    Left  = Right;
    Right = Swap;
  }

  if ( Left >= 0 )
  {
    NumberBit = 0;
    Max = Right;
  }
  else
  {
    NumberBit = 1;
    Max = - Left;

    if ( Max < Right ) Max = Right;
  }

  NumberBit += getvexintnumbit( Max );

  return( NumberBit );
}

/*------------------------------------------------------------\
|                                                             |
|                       Get Vex Type Scalar                   |
|                                                             |
\------------------------------------------------------------*/

int getvextypescalar( Type )

  int Type;
{
  if ( ( Type < 0             ) ||
       ( Type >= VEX_MAX_TYPE ) )
  {
    return( -1 );
  }

  return( VexTypeScalar[ Type ] );
}

/*------------------------------------------------------------\
|                                                             |
|                       Get Vex Vector Name                   |
|                                                             |
\------------------------------------------------------------*/

char *getvexvectorname( Vector, Index )

  char   *Vector;
  long   *Index;
{
  char *LeftParen;
  char *RightParen;

  if ( Vector == (char *)0 )
  {
    return( (char *)0 );
  }

  strcpy( VexBuffer, Vector );
  LeftParen = strchr( VexBuffer, '(' );

  if ( LeftParen == (char *)0 )
  {
    return( (char *)0 );
  }

  *LeftParen = '\0';
  RightParen = strchr( LeftParen + 1, ')' );

  if ( RightParen == (char *)0 )
  {
    return( (char *)0 );
  }

  *RightParen = '\0';

  if ( *(LeftParen + 1) == '?' )
  {
    *Index = -1;
  }
  else
  {
    *Index = atoi( LeftParen + 1 );
  }

  Vector = namealloc( VexBuffer );

  return( Vector );
}

/*------------------------------------------------------------\
|                                                             |
|                     Get Vex Atom Bit Name                   |
|                                                             |
\------------------------------------------------------------*/

chain_list *getvexatombitname( Expr )

  vexexpr *Expr;
{
  chain_list  *HeadList;
  chain_list  *NewChain;
  chain_list **PrevChain;
  char        *ScanChar;
  char        *Name;
  long         Index;
  long         Step;
  long         Width;

  if ( ! IsVexNodeAtom( Expr ) )
  {
    return( (chain_list *)0 );
  }

  Name = GetVexAtomValue( Expr );

  if ( IsVexNodeBit( Expr ) )
  {
    HeadList = addchain( (chain_list *)0, Name );
  }
  else
  {
    if ( Expr->LEFT > Expr->RIGHT ) Step = -1;
    else                            Step =  1;

    Index = Expr->LEFT;

    if ( IsVexAtomLiteral( Expr ) )
    {
      VexBuffer[ 0 ] = '\'';
      VexBuffer[ 2 ] = '\'';
      VexBuffer[ 3 ] = '\0';

      ScanChar = &VexBuffer[ 1 ];
    }
    else
    {
      strcpy( VexBuffer, Name );
      ScanChar = VexBuffer + strlen( VexBuffer );
    }

    PrevChain = &HeadList;

    for ( Width = Expr->WIDTH; Width > 0; Width-- )
    {
      if ( IsVexAtomLiteral( Expr ) )
      {
        if ( Step > 0 ) *ScanChar = Name[ Width               ];
        else            *ScanChar = Name[ Expr->WIDTH - Width ];
      }
      else
      if ( IsVexNodeVarWidth( Expr ) )
      {
        strcpy( ScanChar, "(??)" );
      }
      else
      {
        sprintf( ScanChar, "(%ld)", Index );
      }

      NewChain   = addchain( (chain_list *)0, namealloc( VexBuffer ) );
      *PrevChain = NewChain;
      PrevChain  = &NewChain->NEXT;

      Index += Step;
    }
  }

  return( HeadList );
}

/*------------------------------------------------------------\
|                                                             |
|                     Get Vex Atom Bit Name Ptype             |
|                                                             |
\------------------------------------------------------------*/

ptype_list *getvexatombitnameptype( Expr )

  vexexpr *Expr;
{
  ptype_list  *HeadList;
  ptype_list  *NewChain;
  ptype_list **PrevChain;
  char        *Name;
  long         Index;
  long         Step;
  long         Width;

  if ( ( ! IsVexNodeAtom( Expr    ) ) ||
       (   IsVexAtomLiteral( Expr ) ) )
  {
    return( (ptype_list *)0 );
  }

  Name = GetVexAtomValue( Expr );

  if ( IsVexNodeBit( Expr ) )
  {
    HeadList = addptype( (ptype_list *)0, -1, Name );
  }
  else
  {
    if ( Expr->LEFT > Expr->RIGHT ) Step = -1;
    else                            Step =  1;

    Index = Expr->LEFT;

    PrevChain = &HeadList;

    for ( Width = Expr->WIDTH; Width > 0; Width-- )
    {
      NewChain   = addptype( (ptype_list *)0, Index, Name );
      *PrevChain = NewChain;
      PrevChain  = &NewChain->NEXT;

      Index += Step;
    }
  }

  return( HeadList );
}

/*------------------------------------------------------------\
|                                                             |
|                     Get Vex Operator Name                   |
|                                                             |
\------------------------------------------------------------*/

char *getvexopername( Oper )

  long Oper;
{
  if ( ( Oper < 0                 ) ||
       ( Oper >= VEX_MAX_OPERATOR ) )
  {
    return( (char *)0 );
  }

  return( VEX_OPER_NAME[ Oper ] );
}

/*------------------------------------------------------------\
|                                                             |
|                     Get Vex Verilog Operator Name           |
|                                                             |
\------------------------------------------------------------*/

char *getvexoperverilogname( Oper )

  long Oper;
{
  if ( ( Oper < 0                 ) ||
       ( Oper >= VEX_MAX_OPERATOR ) )
  {
    return( (char *)0 );
  }

  return( VEX_OPER_VERILOG_NAME[ Oper ] );
}

/*------------------------------------------------------------\
|                                                             |
|                     Get Vex Upper Operator Name             |
|                                                             |
\------------------------------------------------------------*/

char *getvexoperuppername( Oper )

  long Oper;
{
  if ( ( Oper < 0                 ) ||
       ( Oper >= VEX_MAX_OPERATOR ) )
  {
    return( (char *)0 );
  }

  return( VEX_OPER_UPPER_NAME[ Oper ] );
}

/*------------------------------------------------------------\
|                                                             |
|                Get Vex Operator By Name                     |
|                                                             |
\------------------------------------------------------------*/

long getvexoperbyname( Name )

  char *Name;
{
  int Oper;

  Name = namealloc( Name );

  for ( Oper = 0; Oper < VEX_MAX_OPERATOR; Oper++ )
  {
    if ( VEX_OPER_NAME[ Oper ] == Name )
    {
      return( Oper );
    }
  }

  return( -1 );
}

/*------------------------------------------------------------\
|                                                             |
|                      Get Vex Expr Depth                     |
|                                                             |
\------------------------------------------------------------*/

long getvexexprdepth( Expr )

  vexexpr *Expr;
{
  chain_list *ScanOper;
  long        MaxDepth;
  long        CarDepth;

  if ( IsVexNodeAtom( Expr ) )
  {
    return( 0 );
  }

  MaxDepth = 0;

  for ( ScanOper  = Expr->OPERAND;
        ScanOper != (chain_list *)0;
        ScanOper  = ScanOper->NEXT )
  {
    CarDepth = getvexexprdepth( GetVexOperand( ScanOper ) );
    if ( CarDepth > MaxDepth ) MaxDepth = CarDepth;
  }

  return( MaxDepth + 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                      Get Vex Expr Number Node               |
|                                                             |
\------------------------------------------------------------*/

long getvexexprnumnode( Expr )

  vexexpr *Expr;
{
  chain_list *ScanOper;
  long        NumNode;

  if ( IsVexNodeAtom( Expr ) )
  {
    return( 1 );
  }

  NumNode = 1;

  for ( ScanOper  = Expr->OPERAND;
        ScanOper != (chain_list *)0;
        ScanOper  = ScanOper->NEXT )
  {
    NumNode += getvexexprnumnode( GetVexOperand( ScanOper ) );
  }

  return( NumNode );
}

/*------------------------------------------------------------\
|                                                             |
|                     Get Vex Not Operator                    |
|                                                             |
\------------------------------------------------------------*/

long getvexnotoper( Oper )

  long Oper;
{
  if ( ( Oper < 0                 ) ||
       ( Oper >= VEX_MAX_OPERATOR ) )
  {
    return( -1 );
  }

  return( VEX_NOT_OPER[ Oper ] );
}

/*------------------------------------------------------------\
|                                                             |
|                     Get Vex Atom Support                    |
|                                                             |
\------------------------------------------------------------*/

static void loc_getvexsupportatom( Expr )

  vexexpr *Expr;
{
  chain_list  *BitList;
  chain_list  *ScanChain;
  chain_list  *ScanName;
  chain_list **PrevName;

  if ( ! IsVexAtomLiteral( Expr ) )
  {
    BitList = getvexatombitname( Expr );

    for ( ScanChain  = BitList;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      PrevName = &VexGetHeadChain;

      for ( ScanName  = VexGetHeadChain;
            ScanName != (chain_list *)0;
            ScanName  = ScanName->NEXT )
      {
        if ( ScanName->DATA == ScanChain->DATA ) break;

        PrevName = &ScanName->NEXT;
      }

      if ( ScanName == (chain_list *)0 )
      {
        *PrevName = addchain( (chain_list *)0, ScanChain->DATA );
      }
    }

    freechain( BitList );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     Get Vex Expr Support                    |
|                                                             |
\------------------------------------------------------------*/

static void loc_getvexsupport( Expr )

  vexexpr *Expr;
{
  chain_list *ScanOper;

  if ( IsVexNodeAtom( Expr ) )
  {
    loc_getvexsupportatom( Expr );
  }
  else
  {
    for ( ScanOper  = Expr->OPERAND;
          ScanOper != (chain_list *)0;
          ScanOper  = ScanOper->NEXT )
    {
      loc_getvexsupport( GetVexOperand( ScanOper ) );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Get Vex Expr Support                 |
|                                                             |
\------------------------------------------------------------*/

chain_list *getvexexprsupport( Expr )

  vexexpr *Expr;
{
  VexGetHeadChain = (chain_list *)0;

   if ( Expr != (vexexpr *)0 ) loc_getvexsupport( Expr );
  
  return( VexGetHeadChain );
}

/*------------------------------------------------------------\
|                                                             |
|                     Get Vex Atom Support Ptype              |
|                                                             |
\------------------------------------------------------------*/

static void loc_getvexsupportatomptype( Expr )

  vexexpr *Expr;
{
  ptype_list  *BitList;
  ptype_list  *ScanChain;
  ptype_list  *ScanName;
  ptype_list **PrevName;

  if ( ! IsVexAtomLiteral( Expr ) )
  {
    BitList = getvexatombitnameptype( Expr );

    for ( ScanChain  = BitList;
          ScanChain != (ptype_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      PrevName = &VexGetHeadPtype;

      for ( ScanName  = VexGetHeadPtype;
            ScanName != (ptype_list *)0;
            ScanName  = ScanName->NEXT )
      {
        if ( ( ScanName->DATA == ScanChain->DATA ) &&
             ( ScanName->TYPE == ScanChain->TYPE ) ) break;

        PrevName = &ScanName->NEXT;
      }

      if ( ScanName == (ptype_list *)0 )
      {
        *PrevName = addptype( (ptype_list *)0, ScanChain->TYPE, ScanChain->DATA );
      }
    }

    freeptype( BitList );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     Get Vex Expr Support Ptype              |
|                                                             |
\------------------------------------------------------------*/

static void loc_getvexsupportptype( Expr )

  vexexpr *Expr;
{
  chain_list *ScanOper;

  if ( IsVexNodeAtom( Expr ) )
  {
    loc_getvexsupportatomptype( Expr );
  }
  else
  {
    for ( ScanOper  = Expr->OPERAND;
          ScanOper != (chain_list *)0;
          ScanOper  = ScanOper->NEXT )
    {
      loc_getvexsupportptype( GetVexOperand( ScanOper ) );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    Get Vex Expr Support Ptype               |
|                                                             |
\------------------------------------------------------------*/

ptype_list *getvexexprsupportptype( Expr )

  vexexpr *Expr;
{
  VexGetHeadPtype = (ptype_list *)0;

   if ( Expr != (vexexpr *)0 ) loc_getvexsupportptype( Expr );
  
  return( VexGetHeadPtype );
}

/*------------------------------------------------------------\
|                                                             |
|                     Union Vex Expr Support Ptype            |
|                                                             |
\------------------------------------------------------------*/

ptype_list *unionvexexprsupportptype( Support, Expr )

  ptype_list *Support;
  vexexpr    *Expr;
{
  VexGetHeadPtype = Support;

  if ( Expr != (vexexpr *)0 ) loc_getvexsupportptype( Expr );
  
  return( VexGetHeadPtype );
}


/*------------------------------------------------------------\
|                                                             |
|                        Get Vex Expr Support                 |
|                                                             |
\------------------------------------------------------------*/

chain_list *unionvexexprsupport( Support, Expr )

  chain_list *Support;
  vexexpr    *Expr;
{
  VexGetHeadChain = Support;

  if ( Expr != (vexexpr *)0 ) loc_getvexsupport( Expr );
  
  return( VexGetHeadChain );
}

/*------------------------------------------------------------\
|                                                             |
|                     Get Vex Atom All Name                   |
|                                                             |
\------------------------------------------------------------*/

static void loc_getvexallnameatom( Expr )

  vexexpr *Expr;
{
  chain_list  *ScanName;
  char        *AtomName; 

  if ( ! IsVexAtomLiteral( Expr ) )
  {
    AtomName = GetVexAtomValue( Expr );

    for ( ScanName  = VexGetHeadChain;
          ScanName != (chain_list *)0;
          ScanName  = ScanName->NEXT )
    {
      if ( ScanName->DATA == AtomName ) return;
    }

    VexGetHeadChain = addchain( VexGetHeadChain, AtomName );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     Get Vex Expr All Name                   |
|                                                             |
\------------------------------------------------------------*/

static void loc_getvexallname( Expr )

  vexexpr *Expr;
{
  chain_list *ScanOper;

  if ( IsVexNodeAtom( Expr ) )
  {
    loc_getvexallnameatom( Expr );
  }
  else
  {
    for ( ScanOper  = Expr->OPERAND;
          ScanOper != (chain_list *)0;
          ScanOper  = ScanOper->NEXT )
    {
      loc_getvexallname( GetVexOperand( ScanOper ) );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Get Vex Expr All Name                |
|                                                             |
\------------------------------------------------------------*/

chain_list *getvexexprallname( Expr )

  vexexpr *Expr;
{
  VexGetHeadChain = (chain_list *)0;

   if ( Expr != (vexexpr *)0 ) loc_getvexallname( Expr );
  
  return( VexGetHeadChain );
}

/*------------------------------------------------------------\
|                                                             |
|                       Union Vex Expr Support                |
|                                                             |
\------------------------------------------------------------*/

chain_list *unionvexexprallname( AllName, Expr )

  chain_list *AllName;
  vexexpr    *Expr;
{
  VexGetHeadChain = AllName;

  if ( Expr != (vexexpr *)0 ) loc_getvexallname( Expr );
  
  return( VexGetHeadChain );
}

/*------------------------------------------------------------\
|                                                             |
|                     Get Vex Atom Name                       |
|                                                             |
\------------------------------------------------------------*/

char *getvexatomname( Expr )

  vexexpr *Expr;
{
  long Oper;

  if ( IsVexNodeAtom( Expr ) )
  {
    return( GetVexAtomValue( Expr ) );
  }
  else
  if ( IsVexNodeOper( Expr ) )
  {
    Oper = GetVexOperValue( Expr );

    if ( ( Oper == VEX_ARRAY     ) ||
         ( Oper == VEX_INDEX     ) ||
         ( Oper == VEX_INDEX_N   ) ||
         ( Oper == VEX_DOWNTO    ) ||
         ( Oper == VEX_TO        ) )
    {
      return( getvexatomname( GetVexOperand( Expr->OPERAND ) ) );
    }
  }

  return( (char *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                     Get Vex Literal Id                      |
|                                                             |
\------------------------------------------------------------*/

int getvexliteralid( Literal )

  char Literal;
{
  int   Index;

  for ( Index = 0; Index < VEX_MAX_ID; Index++ )
  {
    if ( Literal == VEX_LITERAL_BY_ID[ Index ] ) break;
  }

  if ( Index == VEX_MAX_ID ) Index = -1;

  return( Index );
}

/*------------------------------------------------------------\
|                                                             |
|                     Get Vex Vector Position                 |
|                                                             |
\------------------------------------------------------------*/

int getvexvectorpos( Vector, Index )

  vexexpr *Vector;
  short    Index;
{
  int Bound;

  if ( IsVexNodeBit( Vector ) )
  {
    return( 0 );
  }

  Bound = Vector->WIDTH - 1;

  if ( IsVexNodeDown( Vector ) )
  {
    if ( ( Index <= Vector->LEFT  ) &&
         ( Index >= Vector->RIGHT ) )
    {
      Index -= Vector->RIGHT;

      return( Bound - Index );
    }
  }
  else
  {
    if ( ( Index >= Vector->LEFT  ) &&
         ( Index <= Vector->RIGHT ) )
   {
      Index -= Vector->LEFT;

      return( Index );
    }
  }

  return( -1 );
}

/*------------------------------------------------------------\
|                                                             |
|                    Get Vex Vector Index                     |
|                                                             |
\------------------------------------------------------------*/

int getvexvectorindex( Atom, Position )

  vexexpr *Atom;
  short    Position;
{
  if ( Position < Atom->WIDTH )
  {
    if ( IsVexNodeDown( Atom ) ) return( Atom->LEFT - Position );
    else                         return( Atom->LEFT + Position );
  }

  return( -1 );
}

/*------------------------------------------------------------\
|                                                             |
|                     Get Vex Vector Min                      |
|                                                             |
\------------------------------------------------------------*/

int getvexvectormin( Expr )

  vexexpr *Expr;
{
  if ( IsVexNodeDown( Expr ) ) return( Expr->RIGHT );

  return( Expr->LEFT );
}

/*------------------------------------------------------------\
|                                                             |
|                     Get Vex Vector Max                      |
|                                                             |
\------------------------------------------------------------*/

int getvexvectormax( Expr )

  vexexpr *Expr;
{
  if ( IsVexNodeDown( Expr ) ) return( Expr->LEFT );

  return( Expr->RIGHT );
}

/*------------------------------------------------------------\
|                                                             |
|                     Get Vex Std Func Id                     |
|                                                             |
\------------------------------------------------------------*/

int getvexstdfuncid( FuncName )

  char *FuncName;
{
  authelem *Element;
  long      Index;

  if ( VexHashStdFunc == (authtable *)0 )
  {
    VexHashStdFunc = createauthtable( VEX_MAX_STD_FUNC << 1 );

    for ( Index = 0; Index < VEX_MAX_STD_FUNC; Index++ )
    {
      addauthelem( VexHashStdFunc, VEX_STD_FUNC_NAME[ Index ], Index );
    }
  }

  Element = searchauthelem( VexHashStdFunc, FuncName );

  if ( Element != (authelem *)0 ) return( Element->VALUE );

  return( -1 );
}

/*------------------------------------------------------------\
|                                                             |
|                     Get Vex Array Name                      |
|                                                             |
\------------------------------------------------------------*/

char *getvexarrayname( Atom )

  vexexpr *Atom;
{
  char *Name;
  long  Oper;

  Name = (char *)0;

  if ( IsVexNodeAtom( Atom ) )
  {
    Name = GetVexAtomValue( Atom );
  }
  else
  if ( IsVexNodeOper( Atom ) )
  {
    Oper = GetVexOperValue( Atom );

    if ( ( Oper == VEX_ARRAY  ) ||
         ( Oper == VEX_DOWNTO ) ||
         ( Oper == VEX_TO     ) )
    {
      Atom = GetVexOperand( Atom->OPERAND );
      Name = getvexarrayname( Atom );
    }
  }

  return( Name );
}

/*------------------------------------------------------------\
|                                                             |
|                     Get Vex Array Atom                      |
|                                                             |
\------------------------------------------------------------*/

vexexpr *getvexarrayatom( Expr )

  vexexpr *Expr;
{
  vexexpr *Atom;
  long     Oper;

  Atom = (vexexpr *)0;

  if ( IsVexNodeAtom( Expr ) )
  {
    Atom = Expr;
  }
  else
  if ( IsVexNodeOper( Expr ) )
  {
    Oper = GetVexOperValue( Expr );

    if ( ( Oper == VEX_ARRAY  ) ||
         ( Oper == VEX_DOWNTO ) ||
         ( Oper == VEX_TO     ) )
    {
      Expr = GetVexOperand( Expr->OPERAND );
      Atom = getvexarrayatom( Expr );
    }
  }

  return( Atom );
}


