/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU General Public |
| License as  published by the Free Software Foundation;      |
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
| Tool    :                     MOCHA                         |
|                                                             |
| File    :                  mocha_ctl.c                      |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   05.13.02                        |
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
# include "bdd.h"
# include "btr.h"
# include "fsm.h"
# include "ftl.h"
# include "vex.h"
# include "ctl.h"
# include "ctp.h"
# include "abe.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "mocha_debug.h"
# include "mocha_shared.h"
# include "mocha_ctl.h"

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

  static ctlfig_list   *MochaCtlFigure     = (ctlfig_list   *)0;
  static mochafig_list *MochaMochaFigure   = (mochafig_list *)0;
  static authtable     *MochaCtlHashDeclar = (authtable     *)0;

  static authtable     *MochaCtlHashSupport = (authtable    *)0;

  static short          MochaCtlNoCtlOper   = 0;
  static char           MochaCtlBuffer[ 512 ];

  static short MochaCtlTransVex2AblOper[ VEX_MAX_OPERATOR ] =
  {
    -1      , /* VEX_CONCAT    */
    ABL_NOT , /* VEX_NOT       */
    -1      , /* VEX_NEG       */
    -1      , /* VEX_EVENT     */
    ABL_OR  , /* VEX_OR        */
    ABL_AND , /* VEX_AND       */
    ABL_XOR , /* VEX_XOR       */
    ABL_NOR , /* VEX_NOR       */
    ABL_NAND, /* VEX_NAND      */
    ABL_NXOR, /* VEX_NXOR      */
    -1      , /* VEX_EQ        */
    -1      , /* VEX_NE        */
    -1      , /* VEX_LT        */
    -1      , /* VEX_LE        */
    -1      , /* VEX_GT        */
    -1      , /* VEX_GE        */
    -1      , /* VEX_ADD       */
    -1      , /* VEX_SUB       */
    -1      , /* VEX_MUL       */
    -1      , /* VEX_DIV       */
    -1      , /* VEX_EXP       */
    -1      , /* VEX_MOD       */
    -1      , /* VEX_REM       */
    -1      , /* VEX_TO        */
    -1      , /* VEX_DOWNTO    */
    -1      , /* VEX_INDEX     */
    -1      , /* VEX_LEFT      */
    -1      , /* VEX_RIGHT     */
    -1      , /* VEX_LOW       */
    -1      , /* VEX_HIGH      */
    -1      , /* VEX_LENGTH    */
    -1      , /* VEX_RANGE     */
    -1      , /* VEX_REV_RANGE */
    -1      , /* VEX_DRIVER    */
    -1      , /* VEX_IFT       */
    -1      , /* VEX_ARRAY     */
    -1      , /* VEX_INDEX_N   */
    -1      , /* VEX_OTHERS    */
    -1      , /* VEX_NUM_BIT   */
    -1      , /* VEX_ABS       */
    ABL_AF  , /* AF            */
    ABL_AG  , /* AG            */
    ABL_AX  , /* AX            */
    ABL_AU  , /* AU            */
    ABL_EF  , /* EF            */
    ABL_EG  , /* EG            */
    ABL_EX  , /* EX            */
    ABL_EU  , /* EU            */
    ABL_NXOR, /* EQUIV         */
    -1        /* IMPLY         */
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       MochaCtlAblHashSupport                |
|                                                             |
\------------------------------------------------------------*/

static void MochaCtlAblArrayHashSupport();

static void MochaCtlAblHashSupport( Expr )

  ablexpr *Expr;
{
  ctldecl_list *CtlDeclar;
  ctlform_list *CtlForm;
  biablarray   *BiAblArray;
  authelem     *Element;
  char         *AtomValue;

  if ( ABL_ATOM( Expr ) )
  {
    AtomValue = ABL_ATOM_VALUE( Expr );

    if ( ( AtomValue != ABL_ATOM_NAME_ONE      ) &&
         ( AtomValue != ABL_ATOM_NAME_ZERO     ) &&
         ( AtomValue != ABL_ATOM_NAME_DC       ) &&
         ( AtomValue != ABL_ATOM_NAME_TRISTATE ) )
    {
      Element = searchauthelem( MochaCtlHashSupport, AtomValue );

      if ( Element != (authelem *)0 ) return;

      CtlDeclar = searchctldeclall( MochaCtlFigure, AtomValue );

      if ( CtlDeclar != (ctldecl_list *)0 )
      {
        if ( ( CtlDeclar->DECL_TYPE != CTL_DECLAR_VARIABLE ) &&
             ( ! IsMochaCtlHashSupport( CtlDeclar )        ) )
        {
          SetMochaCtlHashSupport( CtlDeclar );
          BiAblArray = (biablarray *)CtlDeclar->USER;
          MochaCtlAblArrayHashSupport( BiAblArray->EXPR );
        }
      }
      else
      {
        CtlForm = searchctlform( MochaCtlFigure, AtomValue );

        if ( ( CtlForm != (ctlform_list *)0       ) &&
             ( ! IsMochaCtlHashSupport( CtlForm ) ) )
        {
          SetMochaCtlHashSupport( CtlForm );
          BiAblArray = (biablarray *)CtlForm->USER;
          MochaCtlAblArrayHashSupport( BiAblArray->EXPR );
        }
      }

      addauthelem( MochaCtlHashSupport, AtomValue, 0 );
    }
  }
  else
  {
    while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
    {
      MochaCtlAblHashSupport( ABL_CAR( Expr ) );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       MochaCtlAblArrayHashSupport           |
|                                                             |
\------------------------------------------------------------*/

static void MochaCtlAblArrayHashSupport( AblArray )

  ablarray *AblArray;
{
  long Index;

  if ( AblArray == (ablarray *)0 ) return;

  for ( Index = 0; Index < AblArray->WIDTH; Index++ )
  {
    MochaCtlAblHashSupport( AblArray->ARRAY[ Index ] );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       MochaCtlFigureHashSupport             |
|                                                             |
\------------------------------------------------------------*/

static void MochaCtlFigureHashSupport( MochaFigure )

  mochafig_list *MochaFigure;
{
  ctldecl_list *CtlDeclar;
  ctlform_list *CtlForm;
  biablarray   *BiAblArray;
  short         DeclType;

  MochaCtlFigure      = MochaFigure->CTL_FIGURE;
  MochaCtlHashSupport = createauthtable( 100 );

  for ( DeclType  = CTL_DECLAR_DEFINE; DeclType < CTL_MAX_DECLAR_TYPE; DeclType++ )
  {
    for ( CtlDeclar  = MochaCtlFigure->DECLAR[ DeclType ];
          CtlDeclar != (ctldecl_list *)0;
          CtlDeclar  = CtlDeclar->NEXT )
    {
      if ( ! IsMochaCtlHashSupport( CtlDeclar ) )
      {
        SetMochaCtlHashSupport( CtlDeclar );
        BiAblArray = (biablarray *)CtlDeclar->USER;
        MochaCtlAblArrayHashSupport( BiAblArray->EXPR );
      }
    }
  }

  for ( CtlForm  = MochaCtlFigure->FORM;
        CtlForm != (ctlform_list *)0;
        CtlForm  = CtlForm->NEXT )
  {
    if ( ! IsMochaCtlHashSupport( CtlForm ) )
    {
      SetMochaCtlHashSupport( CtlForm );
      BiAblArray = (biablarray *)CtlForm->USER;
      MochaCtlAblArrayHashSupport( BiAblArray->EXPR );
    }
  }

  for ( DeclType  = CTL_DECLAR_DEFINE; DeclType < CTL_MAX_DECLAR_TYPE; DeclType++ )
  {
    for ( CtlDeclar  = MochaCtlFigure->DECLAR[ DeclType ];
          CtlDeclar != (ctldecl_list *)0;
          CtlDeclar  = CtlDeclar->NEXT )
    {
      ClearMochaCtlHashSupport( CtlDeclar );
    }
  }

  for ( CtlForm  = MochaCtlFigure->FORM;
        CtlForm != (ctlform_list *)0;
        CtlForm  = CtlForm->NEXT )
  {
    ClearMochaCtlHashSupport( CtlForm );
  }

  MochaFigure->HASH_CTL_SUPPORT = MochaCtlHashSupport;
  MochaCtlHashSupport = (authtable *)0; 
}

/*------------------------------------------------------------\
|                                                             |
|                        MochaCtlAllocAblArray                |
|                                                             |
\------------------------------------------------------------*/

static ablarray *MochaCtlAllocAblArray( Width )

  long Width;
{
  ablarray *AblArray;

  if ( Width <= 0 )
  {
    MochaPrintf( stdout, "Invalid Width %ld in MochaCtlAllocAblArray !\n", Width );
    autexit( 1 );
  }

  AblArray = (ablarray *)autallocheap( sizeof( ablarray ) );
  AblArray->ARRAY = (ablexpr **)autallocheap( sizeof( ablexpr *) * Width );
  AblArray->WIDTH = Width;

  return( AblArray );
}

/*------------------------------------------------------------\
|                                                             |
|                        MochaCtlFreeAblArray                 |
|                                                             |
\------------------------------------------------------------*/

static void MochaCtlFreeAblArray( AblArray )

  ablarray *AblArray;
{
  long Index;

  if ( AblArray == (ablarray *)0 ) return;

  for ( Index = 0; Index < AblArray->WIDTH; Index++ )
  {
    freeablexpr( AblArray->ARRAY[ Index ] );
  }

  autfreeheap( (void *)AblArray->ARRAY, sizeof( ablexpr *) * AblArray->WIDTH );
  autfreeheap( (void *)AblArray, sizeof( ablarray ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        MochaCtlDupAblArray                  |
|                                                             |
\------------------------------------------------------------*/

static ablarray *MochaCtlDupAblArray( AblArray )

  ablarray *AblArray;
{
  ablarray *DupAblArray;
  long      Index;

  if ( AblArray == (ablarray *)0 ) return( (ablarray *)0 );

  DupAblArray = MochaCtlAllocAblArray( AblArray->WIDTH );

  for ( Index = 0; Index < AblArray->WIDTH; Index++ )
  {
    DupAblArray->ARRAY[ Index ] = dupablexpr( AblArray->ARRAY[ Index ] );
  }

  return( DupAblArray );
}

# if 0
/*------------------------------------------------------------\
|                                                             |
|                        MochaCtlViewAblArray                 |
|                                                             |
\------------------------------------------------------------*/

static void MochaCtlViewAblArray( AblArray )

  ablarray *AblArray;
{
  long   Index;

  if ( AblArray == (ablarray *)0 )
  {
    fprintf( stdout, "    NULL\n" );
  }
  else
  {
    fprintf( stdout, "    WIDTH: %ld\n", AblArray->WIDTH );

    for ( Index = 0; Index < AblArray->WIDTH; Index++ )
    {
      fprintf( stdout, "    %-4ld : ", Index );
      viewablexprln( AblArray->ARRAY[ Index ], ABL_VIEW_VHDL );
    }
  }
}

# endif

/*------------------------------------------------------------\
|                                                             |
|                        MochaCtlAllocBiAblArray              |
|                                                             |
\------------------------------------------------------------*/

static biablarray *MochaCtlAllocBiAblArray( Target, Expr )

  ablarray *Target;
  ablarray *Expr;
{
  biablarray *BiAblArray;

  BiAblArray = (biablarray *)autallocheap( sizeof( biablarray ) );
  BiAblArray->TARGET = Target;
  BiAblArray->EXPR   = Expr;

  return( BiAblArray );
}

/*------------------------------------------------------------\
|                                                             |
|                      MochaCtlFreeBiAblArray                 |
|                                                             |
\------------------------------------------------------------*/

# if 0
static void MochaCtlFreeBiAblArray( BiAblArray )

  biablarray *BiAblArray;
{
  if ( BiAblArray == (biablarray *)0 ) return;

  MochaCtlFreeAblArray( BiAblArray->TARGET );
  MochaCtlFreeAblArray( BiAblArray->EXPR   );

  autfreeheap( (void *)BiAblArray, sizeof( biablarray ) );
}
# endif

/*------------------------------------------------------------\
|                                                             |
|                        MochaCtlViewBiAblArray               |
|                                                             |
\------------------------------------------------------------*/

# ifdef DEBUG

static void MochaCtlViewBiAblArray( BiAblArray )

  biablarray *BiAblArray;
{
  fprintf( stdout, "> BiAblArray\n" );
  if ( BiAblArray == (biablarray *)0 )
  {
    fprintf( stdout, "  NULL\n" );
  }
  else
  {
    fprintf( stdout, "  TARGET\n" );
    MochaCtlViewAblArray( BiAblArray->TARGET );
    fprintf( stdout, "  EXPR  \n" );
    MochaCtlViewAblArray( BiAblArray->EXPR   );
  }

  fprintf( stdout, "< BiAblArray\n" );
}
# endif

/*------------------------------------------------------------\
|                                                             |
|                     MochaCtlConvertVexAtomEnumerate         |
|                                                             |
\------------------------------------------------------------*/

static biablarray *MochaCtlAssign2BiAblArray();

static vexexpr *MochaCtlConvertVexAtomEnumerate( VexAtom )

  vexexpr *VexAtom;
{
  mochafsm_list   *MochaFsm;
  mochastate_list *MochaState;
  ctldecl_list    *CtlDeclar;
  ctltype_list    *CtlType;
  authelem        *Element;
  char            *Value;

  if ( IsVexAtomLiteral( VexAtom ) )
  {
    return( VexAtom );
  }

  Value = GetVexAtomValue( VexAtom );

  Element = searchauthelem( MochaCtlHashDeclar, Value );

  if ( Element != (authelem *)0 )
  {
    return( (vexexpr *)(Element->VALUE) );
  }

  CtlDeclar = searchctldeclall( MochaCtlFigure, Value );

  if ( CtlDeclar != (ctldecl_list *)0 )
  {
    CtlType = CtlDeclar->TYPE;

    if ( CtlType->INDEX < VEX_MAX_TYPE )
    {
      addauthelem( MochaCtlHashDeclar, Value, (long)VexAtom );

      return( VexAtom );
    }
  }

  for ( MochaFsm  = MochaMochaFigure->FSM;
        MochaFsm != (mochafsm_list *)0;
        MochaFsm  = MochaFsm->NEXT )
  {
    if ( ( Value  == MochaFsm->CURRENT_STATE ) ||
         ( Value  == MochaFsm->NEXT_STATE    ) )
    {
      VexAtom = createvexatomvec( Value, MochaFsm->LEFT, MochaFsm->RIGHT );
      addauthelem( MochaCtlHashDeclar, Value, (long)VexAtom );

      return( VexAtom );
    }

    Element = searchauthelem( MochaFsm->HASH_STATE, Value );

    if ( Element != (authelem *)0 )
    {
      MochaState = (mochastate_list *)(Element->VALUE);
      VexAtom    = createvexatomlong( MochaState->CODE, MochaFsm->NUMBER_BIT, 0 );
      addauthelem( MochaCtlHashDeclar, Value, (long)VexAtom );

      return( VexAtom );
    }
  }

  addauthelem( MochaCtlHashDeclar, Value, (long)VexAtom );

  return( VexAtom );
}

/*------------------------------------------------------------\
|                                                             |
|                      MochaCtlVexAtom2AblArray               |
|                                                             |
\------------------------------------------------------------*/

static ablarray *MochaCtlVexAtom2AblArray( VexAtom, Subst )

  vexexpr     *VexAtom;
  int          Subst;
{
  ctldecl_list *CtlDeclar;
  ctlform_list *CtlForm;
  ablarray     *AblArray;
  biablarray   *BiAblArray;
  char         *Value;
  char         *ScanChar;
  long          Index;
  long          Index2;
  long          Step;
  long          Width;

  if ( ! IsVexNodeAtom( VexAtom ) )
  {
    MochaPrintf( stdout, "Invalid atom : " );
    viewvexexprboundln( VexAtom );
    autexit( 1 );
  }

  VexAtom  = MochaCtlConvertVexAtomEnumerate( VexAtom );
  Value    = GetVexAtomValue( VexAtom );

  if ( (   Subst                       ) &&
       ( ! IsVexAtomLiteral( VexAtom ) ) )
  {
    CtlDeclar = searchctldeclall( MochaCtlFigure, Value );

    if ( CtlDeclar != (ctldecl_list *)0 )
    {
      if ( CtlDeclar->DECL_TYPE != CTL_DECLAR_VARIABLE )
      {
        BiAblArray = (biablarray *)CtlDeclar->USER;
  
        if ( BiAblArray == (biablarray *)0 )
        {
          BiAblArray = MochaCtlAssign2BiAblArray( CtlDeclar->VEX_ATOM, CtlDeclar->VEX_INIT );
          CtlDeclar->USER = (void *)BiAblArray;
        }
  
        AblArray = MochaCtlDupAblArray( BiAblArray->EXPR );
  
        return( AblArray );
      }
    }
    else
    {
      CtlForm = searchctlform( MochaCtlFigure, Value );

      if ( CtlForm != (ctlform_list *)0 )
      {
        BiAblArray = (biablarray *)CtlForm->USER;
  
        if ( BiAblArray == (biablarray *)0 )
        {
          BiAblArray = MochaCtlAssign2BiAblArray( (vexexpr *)0, CtlForm->VEX_EXPR );
          CtlForm->USER = (void *)BiAblArray;
        }
  
        AblArray = MochaCtlDupAblArray( BiAblArray->EXPR );
  
        return( AblArray );
      }
    }
  }

  AblArray = MochaCtlAllocAblArray( VexAtom->WIDTH );

  if ( IsVexAtomLiteral( VexAtom ) )
  {
    for ( Index = 1; Index <= VexAtom->WIDTH; Index++ )
    {
      if ( Value[ Index ] == '0' )
      {
        AblArray->ARRAY[ Index - 1 ] = createablatomzero();
      }
      else
      if ( Value[ Index ] == '1' )
      {
        AblArray->ARRAY[ Index - 1 ] = createablatomone();
      }
      else
      {
        MochaPrintf( stdout, "Invalid literal string: " );
        viewvexexprboundln( VexAtom );
        autexit( 1 );
      }
    }

    return( AblArray );
  }

  if ( IsVexAtomBit( VexAtom ) )
  {
    AblArray->ARRAY[ 0 ] = createablatom( Value );
  }
  else
  {
    if ( VexAtom->LEFT > VexAtom->RIGHT ) Step = -1;
    else                                  Step =  1;

    Index  = VexAtom->LEFT;
    Index2 = 0;

    strcpy( MochaCtlBuffer, Value );
    ScanChar = MochaCtlBuffer + strlen( MochaCtlBuffer );

    for ( Width = VexAtom->WIDTH; Width > 0; Width-- )
    {
      sprintf( ScanChar, " %ld", Index );

      AblArray->ARRAY[ Index2 ] = createablatom( MochaCtlBuffer );

      Index += Step;
      Index2 = Index2 + 1;
    }
  }

  return( AblArray );
}

/*------------------------------------------------------------\
|                                                             |
|                      MochaCtlVex2AblArray                   |
|                                                             |
\------------------------------------------------------------*/

static ablarray *MochaCtlVex2AblArray( Expr )

  vexexpr     *Expr;
{
  ablarray     *AblArray;
  ablarray     *AblArray1;
  ablarray     *AblArray2;
  ablexpr      *AblExpr;
  chain_list   *ScanOper;
  vexexpr      *Operand;
  long          Oper;
  long          Index;
  long          AblOper;
  short         Negative;

  if ( IsVexNodeAtom( Expr ) )
  {
    return( MochaCtlVexAtom2AblArray( Expr, 1 ) );
  }

  if ( ! IsVexNodeOper( Expr ) )
  {
    MochaPrintf( stdout, "Invalid expression : " );
    viewvexexprboundln( Expr );
    autexit( 1 );
  }

  Oper = GetVexOperValue( Expr );

  if ( MochaCtlNoCtlOper ) 
  {
    if ( ( Oper >= VEX_AF ) &&
         ( Oper <= VEX_EU ) )
    {
      MochaPrintf( stdout,
                   "CTL operator can not be used in INITIAL, RESET or ASSUME expressions : " );
      viewvexexprboundln( Expr );
      autexit( 1 );
    }
  }


  if ( isvexunaryoper( Oper ) )
  {
    Operand  = GetVexOperand( Expr->OPERAND );
    AblArray = MochaCtlVex2AblArray( Operand );

    AblOper = MochaCtlTransVex2AblOper[ Oper ];

    if ( AblOper == -1 )
    {
      MochaPrintf( stdout, "operator in expression not supported : " );
      viewvexexprboundln( Expr );
      autexit( 1 );
    }

    for ( Index = 0; Index < AblArray->WIDTH; Index++ )
    {
      AblArray->ARRAY[ Index ] = optimablunaryexpr( AblOper, AblArray->ARRAY[ Index ] );
    }

    return( AblArray );
  }

  Negative = isvexnegativeoper( Oper );

  if ( Negative ) Oper = getvexnotoper( Oper );

  AblOper  = MochaCtlTransVex2AblOper[ Oper ];
  ScanOper = Expr->OPERAND;
  Operand  = GetVexOperand( ScanOper );
  AblArray = MochaCtlVex2AblArray( Operand );

  for ( ScanOper  = ScanOper->NEXT;
        ScanOper != (chain_list *)0;
        ScanOper  = ScanOper->NEXT )
  {
    Operand   = GetVexOperand( ScanOper );
    AblArray2 = MochaCtlVex2AblArray( Operand );


    switch ( Oper )
    {
      case VEX_CONCAT    : /* TO BE DONE */

        MochaPrintf( stdout, "concat operator not yet implemented : " );
        viewvexexprboundln( Expr );
        autexit( 1 );

      break;

      case VEX_OR        :
      case VEX_AND       :
      case VEX_XOR       :
      case VEX_NOR       :
      case VEX_NAND      :
      case VEX_NXOR      : 
      case VEX_EQUIV     :

        if ( AblArray2->WIDTH != AblArray->WIDTH )
        {
          MochaPrintf( stdout, "bad width in the boolean expression : " );
          viewvexexprboundln( Expr );
          autexit( 1 );
        }

        for ( Index = 0; Index < AblArray->WIDTH; Index++ )
        {
          AblArray->ARRAY[ Index ] = optimablbinexpr( AblOper,
                                     AblArray->ARRAY[ Index ], AblArray2->ARRAY[ Index ] );

          AblArray2->ARRAY[ Index ] = (ablexpr *)0;
        }

        MochaCtlFreeAblArray( AblArray2 );

      break;

      case VEX_EQ        :
      case VEX_NE        :

        AblArray1 = AblArray;
        AblArray  = MochaCtlAllocAblArray( 1 );

        if ( AblArray2->WIDTH != AblArray1->WIDTH )
        {
          MochaPrintf( stdout, "bad width in the relational expression : " );
          viewvexexprboundln( Expr );

          autexit( 1 );
        }

        AblExpr = (ablexpr *)0;

        for ( Index = 0; Index < AblArray1->WIDTH; Index++ )
        {
          if ( AblExpr == (ablexpr *)0 )
          {
            AblExpr = optimablbinexpr( ABL_NXOR, AblArray1->ARRAY[ Index ],
                                                 AblArray2->ARRAY[ Index ] );
          }
          else
          {
            AblExpr = optimablbinexpr( ABL_AND, AblExpr,
                          optimablbinexpr( ABL_NXOR, AblArray1->ARRAY[ Index ],
                                                     AblArray2->ARRAY[ Index ] ) );
          }

          AblArray1->ARRAY[ Index ] = (ablexpr *)0;
          AblArray2->ARRAY[ Index ] = (ablexpr *)0;
        }

        AblArray->ARRAY[ 0 ] = AblExpr;

        MochaCtlFreeAblArray( AblArray1 );
        MochaCtlFreeAblArray( AblArray2 );

      break;

      case VEX_AU        :
      case VEX_EU        :

        if ( ( AblArray2->WIDTH != AblArray->WIDTH ) ||
             ( AblArray2->WIDTH != 1               ) )
        {
          MochaPrintf( stdout, "bad width in the ctl expression : " );
          viewvexexprboundln( Expr );
          autexit( 1 );
        }

        AblArray->ARRAY[ 0 ] = optimablbinexpr( AblOper, AblArray->ARRAY[ 0 ],
                                                         AblArray2->ARRAY[ 0 ] );

        AblArray2->ARRAY[ 0 ] = (ablexpr *)0;

        MochaCtlFreeAblArray( AblArray2 );

      break;

      case VEX_IMPLY     :

        if ( ( AblArray2->WIDTH != AblArray->WIDTH ) ||
             ( AblArray2->WIDTH != 1               ) )
        {
          MochaPrintf( stdout, "bad width in the boolean expression : " );
          viewvexexprboundln( Expr );
          autexit( 1 );
        }

        AblArray->ARRAY[ 0 ] = optimablunaryexpr( ABL_NOT, AblArray->ARRAY[ 0 ] );
        AblArray->ARRAY[ 0 ] = optimablbinexpr(   ABL_OR , AblArray->ARRAY[ 0 ],
                                                          AblArray2->ARRAY[ 0 ] );

        AblArray2->ARRAY[ 0 ] = (ablexpr *)0;

        MochaCtlFreeAblArray( AblArray2 );
        
      break;

      default            :

        MochaPrintf( stdout, "operator not yet implemented : " );
        viewvexexprboundln( Expr );
        autexit( 1 );
    }
  }

  if ( Negative )
  {
    for ( Index = 0; Index < AblArray->WIDTH; Index++ )
    {
      AblArray->ARRAY[ Index ] = optimablunaryexpr( ABL_NOT, AblArray->ARRAY[ Index ] );
    }
  }


  return( AblArray );
}

/*------------------------------------------------------------\
|                                                             |
|                      MochaCtlAssign2BiAblArray              |
|                                                             |
\------------------------------------------------------------*/

static biablarray *MochaCtlAssign2BiAblArray( Atom, Expr )

  vexexpr *Atom;
  vexexpr *Expr;
{
  ablarray   *AblArrayTarget;
  ablarray   *AblArrayExpr;
  biablarray *BiAblArray;
  long        Index;

  if ( Atom != (vexexpr *)0 )
  {
    AblArrayTarget = MochaCtlVexAtom2AblArray( Atom, 0 );
  }
  else
  {
    AblArrayTarget = (ablarray *)0;
  }

  AblArrayExpr   = MochaCtlVex2AblArray( Expr );

  for ( Index = 0; Index < AblArrayExpr->WIDTH; Index++ )
  {
    AblArrayExpr->ARRAY[ Index ] = simpablexpr( AblArrayExpr->ARRAY[ Index ] );
  }

  BiAblArray = MochaCtlAllocBiAblArray( AblArrayTarget, AblArrayExpr );

  return( BiAblArray );
}

/*------------------------------------------------------------\
|                                                             |
|                      MochaCtlFigureVex2AblArray             |
|                                                             |
\------------------------------------------------------------*/

static void MochaCtlFigureVex2AblArray( MochaFigure )

  mochafig_list *MochaFigure;
{
  ctldecl_list *CtlDeclar;
  ctlform_list *CtlForm;
  biablarray   *BiAblArray;
  ablarray     *AblArrayTarget;
  short         DeclType;

  MochaMochaFigure = MochaFigure;
  MochaCtlFigure   = MochaFigure->CTL_FIGURE;

  if ( MochaCtlHashDeclar == (authtable *)0 )
  {
    MochaCtlHashDeclar = createauthtable( 100 );
  }
  else
  {
    resetauthtable( MochaCtlHashDeclar );
  }

/*
** First of all we create a biablarray for each variables
*/
  for ( CtlDeclar  = MochaCtlFigure->DECLAR[ CTL_DECLAR_VARIABLE ];
        CtlDeclar != (ctldecl_list *)0;
        CtlDeclar  = CtlDeclar->NEXT )
  {
    AblArrayTarget  = MochaCtlVexAtom2AblArray( CtlDeclar->VEX_ATOM );
    BiAblArray      = MochaCtlAllocBiAblArray( AblArrayTarget, (ablarray *)0 );
    CtlDeclar->USER = (void *)BiAblArray; 

# ifdef DEBUG
    MochaCtlViewBiAblArray( BiAblArray );
# endif
  }

/*
** Next we create an extended biablarray for each define/assume and init
*/
  for ( DeclType  = CTL_DECLAR_DEFINE; DeclType < CTL_MAX_DECLAR_TYPE; DeclType++ )
  {
    if ( ( DeclType == CTL_DECLAR_ASSUME  ) ||
         ( DeclType == CTL_DECLAR_RESET   ) ||
         ( DeclType == CTL_DECLAR_INITIAL ) ) MochaCtlNoCtlOper = 1;
    else                                      MochaCtlNoCtlOper = 0;

    for ( CtlDeclar  = MochaCtlFigure->DECLAR[ DeclType ];
          CtlDeclar != (ctldecl_list *)0;
          CtlDeclar  = CtlDeclar->NEXT )
    {
      if ( CtlDeclar->USER == (void *)0 )
      {
        BiAblArray = MochaCtlAssign2BiAblArray( CtlDeclar->VEX_ATOM, CtlDeclar->VEX_INIT );
        CtlDeclar->USER = (void *)BiAblArray; 
      }
      else
      {
        BiAblArray = (biablarray *)CtlDeclar->USER;
      }

# ifdef DEBUG
      fprintf( stdout, "DECLAR %s\n", CTL_DECLAR_TYPE[ DeclType ] );
      viewvexexprboundln( CtlDeclar->VEX_ATOM );
      viewvexexprboundln( CtlDeclar->VEX_INIT );
      MochaCtlViewBiAblArray( BiAblArray );
# endif
    }
  }

  MochaCtlNoCtlOper = 0;

  for ( CtlForm  = MochaCtlFigure->FORM;
        CtlForm != (ctlform_list *)0;
        CtlForm  = CtlForm->NEXT )
  {
    if ( CtlForm->USER == (void *)0 )
    {
      BiAblArray = MochaCtlAssign2BiAblArray( (vexexpr *)0, CtlForm->VEX_EXPR );
      CtlForm->USER = (void *)BiAblArray;
    }
    else
    {
      BiAblArray = (biablarray *)CtlForm->USER;
    }

# ifdef DEBUG
    fprintf( stdout, "FORM %s\n", CtlForm->NAME );
    viewvexexprboundln( CtlForm->VEX_EXPR );
    MochaCtlViewBiAblArray( BiAblArray );
# endif
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        MochaCompileCtl                      |
|                                                             |
\------------------------------------------------------------*/

void MochaCompileCtl( MochaFigure, FileName, FlagVerbose )

  mochafig_list *MochaFigure;
  char          *FileName;
  int            FlagVerbose;
{
  ctlfig_list *CtlFigure;

  CtlFigure  = loadctlfig( FileName );

  if ( ( CtlFigure->DECLAR[ CTL_DECLAR_INITIAL ] == (ctldecl_list *)0 ) &&
       ( CtlFigure->DECLAR[ CTL_DECLAR_RESET   ] == (ctldecl_list *)0 ) ) 
  {
    MochaPrintf( stdout, "Missing initial/reset condition in CTL figure %s\n", CtlFigure->NAME  );
    autexit( 1 );
  }

  MochaFigure->CTL_FIGURE = CtlFigure;

  MochaCtlFigureVex2AblArray( MochaFigure );
  MochaCtlFigureHashSupport( MochaFigure );
}
