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
| Tool    :                     VASY                          |
|                                                             |
| File    :                 vasy_drvalc.c                     |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   25.08.97                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <string.h>

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "vex.h"
# include "vpn.h"
# include "vtl.h"
# include "rtn.h"

# include "vasy_debug.h"
# include "vasy_error.h"
# include "vasy_shared.h"
# include "vasy_simprtl.h"
# include "vasy_drvvex.h"
# include "vasy_drvalc.h"

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
|                      Private variables                      |
|                                                             |
\------------------------------------------------------------*/

  static FILE       *VasyFile        = (FILE *)0;
  static FILE       *VasyLaxFile     = (FILE *)0;
  static FILE       *VasyBoomFile    = (FILE *)0;

  static long        VasyNumberBlock = 0;
  static chain_list *VasyKeepSignal  = (chain_list *)0;

  static short VasyConvertType[ VEX_MAX_TYPE ] =
  {
    VEX_TYPE_SEVERITY          , /* SEVERITY          */
    VEX_TYPE_BIT               , /* BOOLEAN           */
    VEX_TYPE_CHARACTER         , /* CHARACTER         */
    VEX_TYPE_STRING            , /* STRING            */
    VEX_TYPE_BIT               , /* BIT               */
    VEX_TYPE_BIT_VECTOR        , /* INTEGER           */
    VEX_TYPE_BIT_VECTOR        , /* NATURAL           */
    VEX_TYPE_BIT_VECTOR        , /* BIT_VECTOR        */
    VEX_TYPE_BIT               , /* STD_ULOGIC        */
    VEX_TYPE_BIT               , /* STD_LOGIC         */
    VEX_TYPE_BIT_VECTOR        , /* STD_ULOGIC_VECTOR */
    VEX_TYPE_BIT_VECTOR        , /* STD_LOGIC_VECTOR  */
    VEX_TYPE_BIT               , /* X01               */
    VEX_TYPE_BIT               , /* X01Z              */
    VEX_TYPE_BIT               , /* UX01              */
    VEX_TYPE_BIT               , /* UX01Z             */
    VEX_TYPE_BIT_VECTOR        , /* UNSIGNED          */
    VEX_TYPE_BIT_VECTOR        , /* SIGNED            */
    VEX_TYPE_BIT               , /* SMALL_INT         */
    VEX_TYPE_REG_BIT           , /* REG_BIT           */
    VEX_TYPE_REG_VECTOR        , /* REG_VECTOR        */
    VEX_TYPE_MUX_BIT           , /* MUX_BIT           */
    VEX_TYPE_MUX_VECTOR        , /* MUX_VECTOR        */
    VEX_TYPE_WOR_BIT           , /* WOR_BIT           */
    VEX_TYPE_WOR_VECTOR        , /* WOR_VECTOR        */
    VEX_TYPE_BIT_VECTOR          /* ENUMERATE         */
  };

  static rtlfig_list *VasyRtlFigure = (rtlfig_list *)0;

  static rtlasg_list *VasyRtlAsg    = (rtlasg_list *)0;
  static long         VasyNumberDef = 0;

  static char         VasyBuffer[ 512 ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   VasyAllianceDriveLaxFile                  |
|                                                             |
\------------------------------------------------------------*/

static void VasyAllianceDriveLaxFile()
{
  chain_list *ScanList;
  vexexpr    *VexAtom;
  char       *Name;
  int        Step;
  int        Index;
  int        Width;

  fprintf( VasyLaxFile, "##\n" );
  fprintf( VasyLaxFile, "## Generated by VASY\n" );
  fprintf( VasyLaxFile, "##\n" );

  if ( VasyKeepSignal != (chain_list *)0 )
  {
    fprintf( VasyLaxFile, "#S{\n" );

    for ( ScanList  = VasyKeepSignal;
          ScanList != (chain_list *)0;
          ScanList  = ScanList->NEXT )
    {
      VexAtom = (vexexpr *)ScanList->DATA;
      Name    = GetVexAtomValue( VexAtom );

      if ( IsVexAtomVector( VexAtom ) )
      {
        if ( VexAtom->LEFT > VexAtom->RIGHT ) Step = -1;
        else                                  Step =  1;

        Index = VexAtom->LEFT;

        for ( Width = VexAtom->WIDTH; Width > 0; Width-- )
        {
          fprintf( VasyLaxFile, "%s_%d;\n", Name, Index );
          Index += Step;
        }
      }
      else
      {
        fprintf( VasyLaxFile, "%s;\n", Name );
      }
    }

    fprintf( VasyLaxFile, "}\n" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyAllianceDriveBoomFile                 |
|                                                             |
\------------------------------------------------------------*/

static void VasyAllianceDriveBoomFile()
{
  chain_list *ScanList;
  vexexpr    *VexAtom;
  char       *Name;

  fprintf( VasyBoomFile, "#\n" );
  fprintf( VasyBoomFile, "# Generated by VASY\n" );
  fprintf( VasyBoomFile, "#\n" );

  if ( VasyKeepSignal != (chain_list *)0 )
  {
    fprintf( VasyBoomFile, "begin_keep\n" );

    for ( ScanList  = VasyKeepSignal;
          ScanList != (chain_list *)0;
          ScanList  = ScanList->NEXT )
    {
      VexAtom = (vexexpr *)ScanList->DATA;
      Name    = GetVexAtomValue( VexAtom );

      if ( IsVexAtomVector( VexAtom ) )
      {
        if ( VexAtom->WIDTH > 1 )
        {
          fprintf( VasyBoomFile, "%s[%d:%d]\n", Name, VexAtom->LEFT, VexAtom->RIGHT );
        }
        else
        {
          fprintf( VasyBoomFile, "%s[%d]\n", Name, VexAtom->LEFT );
        }
      }
      else
      {
        fprintf( VasyBoomFile, "%s\n", Name );
      }
    }

    fprintf( VasyBoomFile, "end_keep\n" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyAllianceTreatVexOperandAtom           |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyAllianceTreatVexOperandAtom( Operand )

  vexexpr *Operand;
{
  vexexpr      *VexAtom;
  rtldecl_list *RtlDeclar;
  rtlasg_list  *RtlAssign;
  char          Buffer[ 32 ];

  if ( ! IsVexNodeAtom( Operand ) )
  {
    sprintf( Buffer, "rtlatom_%ld", VasyNumberDef++ );
    VexAtom = createvexatomvec( Buffer, Operand->WIDTH - 1, 0 );

    if ( IsVexNodeSigned( Operand ) ) SetVexNodeSigned( VexAtom );

    RtlDeclar = addrtldecl( VasyRtlFigure, VexAtom, RTL_DECLAR_SIGNAL );
    RtlDeclar->BASE = VEX_TYPE_BIT_VECTOR;
    RtlDeclar->KIND = RTL_KIND_NONE;
    RtlDeclar->DIR  = RTL_DIR_INOUT;

    SetVasyRtlDeclarRead( RtlDeclar );
    SetVasyRtlDeclarAsg( RtlDeclar );

    RtlAssign = addrtlasgafter( VasyRtlFigure, VasyRtlAsg,
                                dupvexexpr( VexAtom ), RTL_ASG_COMBINATORIAL );
    RtlAssign->VEX_DATA = dupvexexpr( Operand );
    Operand = VexAtom;
  }

  return( Operand );
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyAllianceAddKeepSignal                 |
|                                                             |
\------------------------------------------------------------*/

static void VasyAllianceAddKeepSignal( VexAtom )

  vexexpr *VexAtom;
{
  if ( (   IsVexNodeAtom( VexAtom )    ) &&
       ( ! IsVexAtomLiteral( VexAtom ) ) )
  {
    VasyKeepSignal = addchain( VasyKeepSignal, (void *)dupvexexpr( VexAtom ) );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyAllianceFreeKeepSignal                |
|                                                             |
\------------------------------------------------------------*/

static void VasyAllianceFreeKeepSignal()
{
  chain_list *ScanChain;


  for ( ScanChain  = VasyKeepSignal;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    freevexexpr( (vexexpr *)ScanChain->DATA );
  }

  freechain( VasyKeepSignal );
  VasyKeepSignal = (chain_list *)0;
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyAllianceCreateVexCarry                |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyAllianceCreateVexCarry( Operand )

  vexexpr *Operand;
{
  vexexpr *VexAtom;
  short    Left;

  if ( ! IsVexAtomLiteral( Operand ) )
  {
    if ( IsVexNodeDown( Operand ) ) Left = Operand->LEFT - 1;
    else                            Left = Operand->LEFT + 1;

    VexAtom = createvexatomvec( GetVexAtomValue( Operand ), Left, Operand->RIGHT );
  }
  else
  {
    strcpy( VasyBuffer, GetVexAtomValue( Operand ) );

    if ( Operand->WIDTH > 2 ) VasyBuffer[ 1 ] = '"';
    else                      VasyBuffer[ 1 ] = '\'';

    VexAtom = createvexatomlit( &VasyBuffer[ 1 ] );
  }

  return( VexAtom );
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyAllianceTreatVexArith                 |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyAllianceTreatVexArith( VexExpr )

  vexexpr *VexExpr;
{
  rtldecl_list *RtlDeclCarry;
  rtldecl_list *RtlDeclSum;
  rtldecl_list *RtlDeclGen;
  rtldecl_list *RtlDeclProp;
  rtlasg_list  *RtlAsgCarry;
  rtlasg_list  *RtlAsgSum;
  rtlasg_list  *RtlAsgProp;
  rtlasg_list  *RtlAsgGen;
  vexexpr      *Operand1;
  vexexpr      *Operand2;
  vexexpr      *VexSum;
  vexexpr      *VexGen;
  vexexpr      *VexProp;
  vexexpr      *VexCarry;
  vexexpr      *VexAtomCarry;
  vexexpr      *VexAtomSum;
  vexexpr      *VexAtomGen;
  vexexpr      *VexAtomProp;
  vexexpr      *VexAnd1;
  vexexpr      *VexAnd2;
  vexexpr      *VexAnd3;
  vexexpr      *VexAtom0;
  vexexpr      *VexAtom1;
  vexexpr      *VexAtom2;
  vexexpr      *VexAtom3;
  vexexpr      *VexAtom4;
  char          Buffer[ 32 ];
  int           MaxWidth;
  int           Index;
  long          Oper;

  VexAtom1 = (vexexpr *)0;
  VexAtom2 = (vexexpr *)0;
  VexAtom3 = (vexexpr *)0;
  VexAtom4 = (vexexpr *)0;

  Oper = GetVexOperValue( VexExpr );

  Operand1 = GetVexOperand( VexExpr->OPERAND       );
  Operand2 = GetVexOperand( VexExpr->OPERAND->NEXT );

  MaxWidth = Operand1->WIDTH;
  if ( Operand2->WIDTH > MaxWidth ) MaxWidth = Operand2->WIDTH;

  sprintf( Buffer, "rtlcarry_%ld", VasyNumberDef );
  VexAtomCarry = createvexatomvec( Buffer,  MaxWidth - 1, 0 );

  RtlDeclCarry = addrtldecl( VasyRtlFigure, dupvexexpr( VexAtomCarry ), RTL_DECLAR_SIGNAL );
  RtlDeclCarry->BASE = VEX_TYPE_BIT_VECTOR;
  RtlDeclCarry->KIND = RTL_KIND_NONE;
  RtlDeclCarry->DIR  = RTL_DIR_INOUT;

  SetVasyRtlDeclarRead( RtlDeclCarry );
  SetVasyRtlDeclarAsg( RtlDeclCarry );

  sprintf( Buffer, "rtlsum_%ld", VasyNumberDef );
  VexAtomSum = createvexatomvec( Buffer,  MaxWidth - 1, 0 );

  RtlDeclSum = addrtldecl( VasyRtlFigure, dupvexexpr( VexAtomSum ), RTL_DECLAR_SIGNAL );
  RtlDeclSum->BASE = VEX_TYPE_BIT_VECTOR;
  RtlDeclSum->KIND = RTL_KIND_NONE;
  RtlDeclSum->DIR  = RTL_DIR_INOUT;

  SetVasyRtlDeclarRead( RtlDeclSum );
  SetVasyRtlDeclarAsg( RtlDeclSum );

  if ( ( MaxWidth > 1            ) &&
       ( VasyFlagCLA             ) &&
       ( MaxWidth >= VasyFlagCLA ) )
  {
/*
** Generation 
*/
    sprintf( Buffer, "rtlgen_%ld", VasyNumberDef );
    VexAtomGen = createvexatomvec( Buffer,  MaxWidth - 1, 1 );

    if ( VasyFlagLax ) VasyAllianceAddKeepSignal( VexAtomGen );

    RtlDeclGen = addrtldecl( VasyRtlFigure, dupvexexpr( VexAtomGen ), RTL_DECLAR_SIGNAL );
    RtlDeclGen->BASE = VEX_TYPE_BIT_VECTOR;
    RtlDeclGen->KIND = RTL_KIND_NONE;
    RtlDeclGen->DIR  = RTL_DIR_INOUT;

    SetVasyRtlDeclarRead( RtlDeclGen );
    SetVasyRtlDeclarAsg( RtlDeclGen );
/*
** Propagation
*/
    sprintf( Buffer, "rtlprop_%ld", VasyNumberDef );
    VexAtomProp = createvexatomvec( Buffer,  MaxWidth - 1, 1 );

    if ( VasyFlagLax ) VasyAllianceAddKeepSignal( VexAtomProp );

    RtlDeclProp = addrtldecl( VasyRtlFigure, dupvexexpr( VexAtomProp ), RTL_DECLAR_SIGNAL );
    RtlDeclProp->BASE = VEX_TYPE_BIT_VECTOR;
    RtlDeclProp->KIND = RTL_KIND_NONE;
    RtlDeclProp->DIR  = RTL_DIR_INOUT;

    SetVasyRtlDeclarRead( RtlDeclProp );
    SetVasyRtlDeclarAsg( RtlDeclProp );
  }

  VasyNumberDef++;
/*
** The two operands should be transformed to atomic expressions with the same widths
*/
  Operand1 = dupvexexpr( VasyAllianceTreatVexOperandAtom( Operand1 ) );
  Operand2 = dupvexexpr( VasyAllianceTreatVexOperandAtom( Operand2 ) );

  if ( MaxWidth > 1 )
  {
    VexAtom1 = VasyAllianceCreateVexCarry( Operand1 );
    VexAtom2 = VasyAllianceCreateVexCarry( Operand2 );
  }

  if ( Oper == VEX_SUB )
  {
    Operand2 = createvexunaryexpr( VEX_NOT, MaxWidth, Operand2 );

    if ( MaxWidth > 1 )
    {
      VexAtom2 = createvexunaryexpr( VEX_NOT, MaxWidth - 1, VexAtom2 );
    }
  }

  if ( MaxWidth > 1 )
  {
    if ( ( VasyFlagCLA             ) &&
         ( MaxWidth >= VasyFlagCLA ) )
    {
/*
** Generation 
*/
      VexGen = createvexbinexpr( VEX_AND, MaxWidth - 1,
                                 dupvexexpr( VexAtom1 ), dupvexexpr( VexAtom2 ) );

      RtlAsgGen = addrtlasg( VasyRtlFigure, dupvexexpr( VexAtomGen ), RTL_ASG_COMBINATORIAL );
      RtlAsgGen->VEX_DATA = simpvexexpr( VexGen );
/*
** Propagation
*/
      VexProp = createvexbinexpr( VEX_OR, MaxWidth - 1,
                                 VexAtom1, VexAtom2 );

      RtlAsgProp = addrtlasg( VasyRtlFigure, dupvexexpr( VexAtomProp ),
                              RTL_ASG_COMBINATORIAL );
      RtlAsgProp->VEX_DATA = simpvexexpr( VexProp );
/*
** Carry-Look ahead
*/
      for ( Index = 1; Index < MaxWidth; Index++ )
      {
        VexGen   = createvexatomvec( GetVexAtomValue( VexAtomGen   ), Index, Index );
        VexProp  = createvexatomvec( GetVexAtomValue( VexAtomProp  ), Index, Index );

        if ( Index == 1 )
        {
          VexCarry = createvexatomvec( GetVexAtomValue( VexAtomCarry ), 0, 0 );
        }
        else
        {
          VexCarry = dupvexexpr( VexCarry );
        }

        VexCarry = createvexbinexpr( VEX_AND, 1, VexProp , VexCarry );
        VexCarry = createvexbinexpr( VEX_OR , 1, VexCarry, VexGen   );

        VexAtom3 = createvexatomvec( GetVexAtomValue( VexAtomCarry ), Index , Index );

        if ( VasyFlagLax ) VasyAllianceAddKeepSignal( VexAtom3 );

        RtlAsgCarry = addrtlasg( VasyRtlFigure, VexAtom3, RTL_ASG_COMBINATORIAL );
        RtlAsgCarry->VEX_DATA = VexCarry;
      }
    }
    else
    {
/*
** Ripple Carry
*/
      VexAtom3 = createvexatomvec( GetVexAtomValue( VexAtomCarry ), MaxWidth - 2, 0 );
      VexAtom4 = createvexatomvec( GetVexAtomValue( VexAtomCarry ), MaxWidth - 1, 1 );

      VexAnd1 = createvexbinexpr( VEX_AND, MaxWidth - 1, dupvexexpr( VexAtom1 ),
                                                         dupvexexpr( VexAtom2 ) );
      VexAnd2 = createvexbinexpr( VEX_AND, MaxWidth - 1, VexAtom1, dupvexexpr( VexAtom3 ) );
      VexAnd3 = createvexbinexpr( VEX_AND, MaxWidth - 1, VexAtom2, VexAtom3 );

      VexCarry = createvexbinexpr( VEX_OR, MaxWidth - 1, VexAnd1 , VexAnd2 );
      VexCarry = createvexbinexpr( VEX_OR, MaxWidth - 1, VexCarry, VexAnd3 );

      if ( VasyFlagLax ) VasyAllianceAddKeepSignal( VexAtom4 );

      RtlAsgCarry = addrtlasg( VasyRtlFigure, VexAtom4, RTL_ASG_COMBINATORIAL );
      RtlAsgCarry->VEX_DATA = simpvexexpr( VexCarry );
    }
  }
/*
** Sum
*/
  VexSum = createvexbinexpr( VEX_XOR, MaxWidth, Operand1, Operand2 );
  VexSum = createvexbinexpr( VEX_XOR, MaxWidth,
                             VexSum, dupvexexpr( VexAtomCarry ) );

  RtlAsgSum = addrtlasg( VasyRtlFigure, dupvexexpr( VexAtomSum ), RTL_ASG_COMBINATORIAL );
  RtlAsgSum->VEX_DATA = simpvexexpr( VexSum );
/*
** Carry In
*/
  VexAtom0 = createvexatomvec( GetVexAtomValue( VexAtomCarry ), 0, 0 );
  RtlAsgCarry = addrtlasg( VasyRtlFigure, VexAtom0,  RTL_ASG_COMBINATORIAL );

  if ( Oper == VEX_ADD )
  {
    RtlAsgCarry->VEX_DATA = createvexatomlit( VEX_ATOM_ZERO );
  }
  else
  {
    RtlAsgCarry->VEX_DATA = createvexatomlit( VEX_ATOM_ONE );
  }

  freevexexpr( VexExpr );

  return( VexAtomSum );
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyAllianceTreatVexAbs                   |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyAllianceTreatVexAbs( VexExpr )

  vexexpr *VexExpr;
{
  rtldecl_list *RtlDeclAbs;
  rtlasg_list  *RtlAsgAbs;
  vexexpr      *Operand;
  vexexpr      *VexNeg;
  vexexpr      *VexSign;
  vexexpr      *VexAtomAbs;
  char          Buffer[ 32 ];
  short         Width;

  Width   = VexExpr->WIDTH;
  Operand = GetVexOperand( VexExpr->OPERAND );
  SetVexOperand( VexExpr->OPERAND, NULL );
  freevexexpr( VexExpr );

  Operand = dupvexexpr( VasyAllianceTreatVexOperandAtom( Operand ) );
  VexSign = createvexatomvec( GetVexAtomValue( Operand ), Width - 1, Width - 1 );

  VexNeg = createvexoper( VEX_SUB, Width );
  addvexhexpr( VexNeg, dupvexexpr( Operand ) );
  addvexhexpr( VexNeg, createvexatomveclit( VEX_ZERO, Width ) );
  VexNeg = VasyAllianceTreatVexArith( VexNeg );

  sprintf( Buffer, "rtlabs_%ld", VasyNumberDef++ );
  VexAtomAbs = createvexatomvec( Buffer,  Width - 1, 0 );

  RtlDeclAbs = addrtldecl( VasyRtlFigure, dupvexexpr( VexAtomAbs ), RTL_DECLAR_SIGNAL );
  RtlDeclAbs->BASE = VEX_TYPE_BIT_VECTOR;
  RtlDeclAbs->KIND = RTL_KIND_NONE;
  RtlDeclAbs->DIR  = RTL_DIR_INOUT;

  SetVasyRtlDeclarRead( RtlDeclAbs );
  SetVasyRtlDeclarAsg( RtlDeclAbs );

  RtlAsgAbs = addrtlasg( VasyRtlFigure, dupvexexpr( VexAtomAbs ), RTL_ASG_CONDITIONAL );

  addrtlasgbivex( VasyRtlFigure, RtlAsgAbs, VexSign     , VexNeg , RTL_BIVEX_CONDITIONAL );
  addrtlasgbivex( VasyRtlFigure, RtlAsgAbs, (vexexpr *)0, Operand, RTL_BIVEX_CONDITIONAL );

  return( VexAtomAbs );
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyAllianceTreatVexNeg                   |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyAllianceTreatVexNeg( VexExpr )

  vexexpr *VexExpr;
{
  vexexpr *Operand;
  short    Width;

  Width   = VexExpr->WIDTH;
  Operand = GetVexOperand( VexExpr->OPERAND );
  SetVexOperand( VexExpr->OPERAND, NULL );
  freevexexpr( VexExpr );

  VexExpr = createvexoper( VEX_SUB, Width );
  addvexhexpr( VexExpr, Operand );
  Operand = createvexatomveclit( VEX_ZERO, Width );
  addvexhexpr( VexExpr, Operand );

  return( VasyAllianceTreatVexArith( VexExpr ) );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasyAllianceExtendSignVex               |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyAllianceExtendSignVex( VexExpr, Width )

  vexexpr *VexExpr;
  int      Width;
{
  vexexpr      *VexAtom;
  vexexpr      *VexConcat;
  vexexpr      *VexBit;
  rtldecl_list *RtlDeclar;
  rtlasg_list  *RtlAssign;
  char         *AtomValue;
  int           Left;
  int           Index;

  if ( VexExpr->WIDTH != Width )
  {
    VexAtom = (vexexpr *)0;

    if ( ! IsVexNodeAtom( VexExpr ) )
    {
      sprintf( VasyBuffer, "alcexts_%ld", VasyNumberDef++ );
      VexAtom = createvexatomvec( VasyBuffer, VexExpr->WIDTH - 1, 0 );
    
      RtlDeclar = addrtldecl( VasyRtlFigure, VexAtom, RTL_DECLAR_SIGNAL );

      if ( IsVexNodeSigned( VexExpr ) ) RtlDeclar->BASE = VEX_TYPE_SIGNED;
      else                              RtlDeclar->BASE = VEX_TYPE_UNSIGNED;

      RtlDeclar->KIND = RTL_KIND_NONE;
      RtlDeclar->DIR  = RTL_DIR_INOUT;
    
      RtlAssign = addrtlasg( VasyRtlFigure, dupvexexpr( VexAtom ), RTL_ASG_COMBINATORIAL );
      RtlAssign->VEX_DATA = VexExpr;

      VexExpr = dupvexexpr( VexAtom );
    }

    if ( ! IsVexAtomLiteral( VexExpr ) )
    {
      sprintf( VasyBuffer, "alcexts_%ld", VasyNumberDef++ );
      VexAtom = createvexatomvec( VasyBuffer, Width - 1, 0 );
   
      RtlDeclar = addrtldecl( VasyRtlFigure, VexAtom, RTL_DECLAR_SIGNAL );
      if ( IsVexNodeSigned( VexExpr ) ) RtlDeclar->BASE = VEX_TYPE_SIGNED;
      else                              RtlDeclar->BASE = VEX_TYPE_UNSIGNED;

      RtlDeclar->KIND = RTL_KIND_NONE;
      RtlDeclar->DIR  = RTL_DIR_INOUT;
    }
    
    if ( VexExpr->WIDTH < Width )
    {
      VexConcat = createvexoper( VEX_CONCAT, Width );
      addvexhexpr( VexConcat, dupvexexpr( VexExpr ) );
  
      if ( IsVexNodeSigned( VexExpr ) )
      {
        AtomValue = GetVexAtomValue( VexExpr );
  
        if ( IsVexAtomLiteral( VexExpr ) )
        {
          VexBit = createvexatomveclit( AtomValue[ 1 ], Width - VexExpr->WIDTH );
          addvexhexpr( VexConcat, dupvexexpr( VexBit ) );
        }
        else
        {
          VexBit = createvexatomvec( AtomValue, VexExpr->LEFT, VexExpr->LEFT );
  
          for ( Index = VexExpr->WIDTH; Index < Width; Index++ )
          {
            addvexhexpr( VexConcat, dupvexexpr( VexBit ) );
          }
  
          freevexexpr( VexBit );
        }
  
        SetVexNodeSigned( VexConcat );
      }
      else
      {
        VexBit = createvexatomveclit( VEX_ZERO, Width - VexExpr->WIDTH );
        addvexhexpr( VexConcat, VexBit );
      }
  
      VexConcat = simpvexexpr( VexConcat ); 
    }
    else
    {
      if ( ! IsVexAtomLiteral( VexExpr ) )
      {
        if ( IsVexNodeDown( VexExpr ) )
        {
          Left  = Width + VexExpr->RIGHT - 1;
        }
        else
        {
          Left  = VexExpr->RIGHT - Width + 1;
        }

        VexConcat = createvexatomvec( GetVexAtomValue( VexExpr ), Left, VexExpr->RIGHT );
      }
      else
      {
        strcpy( VasyBuffer, GetVexAtomValue( VexExpr ) );

        Left = VexExpr->WIDTH - Width;
/*
** Should Verify the correctness of the sign ... TO BE DONE
*/
        if ( Width > 1 ) VasyBuffer[ Left ] = '"';
        else             VasyBuffer[ Left ] = '\'';

        VexConcat = createvexatomlit( &VasyBuffer[ Left ] );
      }
    }

    if ( ! IsVexAtomLiteral( VexExpr ) )
    {
      RtlAssign = addrtlasg( VasyRtlFigure, dupvexexpr( VexAtom ), RTL_ASG_COMBINATORIAL );
      RtlAssign->VEX_DATA = VexConcat;
      
      freevexexpr( VexExpr ); 
      VexExpr = dupvexexpr( VexAtom );
    }
    else
    {
      freevexexpr( VexExpr ); 
      VexExpr = VexConcat;
    }
  }

  return( VexExpr ); 
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyAllianceTreatVexRelational            |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyAllianceTreatVexRelational( VexExpr )

  vexexpr *VexExpr;
{
  rtldecl_list *RtlDecl;
  rtldecl_list *RtlDeclCarry;
  rtldecl_list *RtlDeclGen;
  rtldecl_list *RtlDeclProp;
  rtldecl_list *RtlDeclSub;
  rtlasg_list  *RtlAsgCarry;
  rtlasg_list  *RtlAsgSub;
  rtlasg_list  *RtlAsgGen;
  rtlasg_list  *RtlAsgProp;
  vexexpr      *Operand1;
  vexexpr      *Operand2;
  vexexpr      *Operand3;
  vexexpr      *VexSub;
  vexexpr      *VexProp;
  vexexpr      *VexGen;
  vexexpr      *VexCarry;
  vexexpr      *VexAtomCarry;
  vexexpr      *VexAtomGen;
  vexexpr      *VexAtomProp;
  vexexpr      *VexAtomSub;
  vexexpr      *VexAnd1;
  vexexpr      *VexAnd2;
  vexexpr      *VexAnd3;
  vexexpr      *VexAtom0;
  vexexpr      *VexAtom1;
  vexexpr      *VexAtom2;
  vexexpr      *VexAtom3;
  vexexpr      *VexAtom4;
  vexexpr      *VexAtom5;
  vexexpr      *VexAtom6;
  vexexpr      *VexAtom7;
  char          Buffer[ 32 ];
  int           MaxWidth;
  int           Index;
  long          Oper;

  Oper = GetVexOperValue( VexExpr );

  Operand1 = GetVexOperand( VexExpr->OPERAND       );
  Operand2 = GetVexOperand( VexExpr->OPERAND->NEXT );

  SetVexOperand( VexExpr->OPERAND      , (vexexpr *)0 );
  SetVexOperand( VexExpr->OPERAND->NEXT, (vexexpr *)0 );

  MaxWidth = Operand1->WIDTH;
  if ( Operand2->WIDTH > MaxWidth ) MaxWidth = Operand2->WIDTH;

  MaxWidth = MaxWidth + 1;

  Operand1 = VasyAllianceExtendSignVex( Operand1, MaxWidth );
  Operand2 = VasyAllianceExtendSignVex( Operand2, MaxWidth );

  if ( ( IsVexNodeAtom( Operand1 )      ) &&
       ( ! IsVexAtomLiteral( Operand1 ) ) )
  {
    RtlDecl = searchrtldecl( VasyRtlFigure, GetVexAtomValue( Operand1 ) );

    if ( RtlDecl != (rtldecl_list *)0 )
    {
      SetVasyRtlDeclarRead( RtlDecl );
      SetVasyRtlDeclarAsg( RtlDecl );
    }
  }

  if ( ( IsVexNodeAtom( Operand2 )      ) &&
       ( ! IsVexAtomLiteral( Operand2 ) ) )
  {
    RtlDecl = searchrtldecl( VasyRtlFigure, GetVexAtomValue( Operand2 ) );

    if ( RtlDecl != (rtldecl_list *)0 )
    {
      SetVasyRtlDeclarRead( RtlDecl );
      SetVasyRtlDeclarAsg( RtlDecl );
    }
  }

  sprintf( Buffer, "rtlcarry_%ld", VasyNumberDef );
  VexAtomCarry = createvexatomvec( Buffer,  MaxWidth - 1, 0 );

  RtlDeclCarry = addrtldecl( VasyRtlFigure, dupvexexpr( VexAtomCarry ), RTL_DECLAR_SIGNAL );
  RtlDeclCarry->BASE = VEX_TYPE_BIT_VECTOR;
  RtlDeclCarry->KIND = RTL_KIND_NONE;
  RtlDeclCarry->DIR  = RTL_DIR_INOUT;

  SetVasyRtlDeclarRead( RtlDeclCarry );
  SetVasyRtlDeclarAsg( RtlDeclCarry );

  sprintf( Buffer, "rtlltgt_%ld", VasyNumberDef );
  VexAtomSub = createvexatombit( Buffer );

  RtlDeclSub = addrtldecl( VasyRtlFigure, dupvexexpr( VexAtomSub ), RTL_DECLAR_SIGNAL );
  RtlDeclSub->BASE = VEX_TYPE_BIT;
  RtlDeclSub->KIND = RTL_KIND_NONE;
  RtlDeclSub->DIR  = RTL_DIR_INOUT;

  SetVasyRtlDeclarRead( RtlDeclSub );
  SetVasyRtlDeclarAsg( RtlDeclSub );

  if ( ( VasyFlagCLA             ) &&
       ( MaxWidth >= VasyFlagCLA ) )
  {
/*
** Generation 
*/
    sprintf( Buffer, "rtlgen_%ld", VasyNumberDef );
    VexAtomGen = createvexatomvec( Buffer,  MaxWidth - 1, 1 );

    if ( VasyFlagLax ) VasyAllianceAddKeepSignal( VexAtomGen );

    RtlDeclGen = addrtldecl( VasyRtlFigure, dupvexexpr( VexAtomGen ), RTL_DECLAR_SIGNAL );
    RtlDeclGen->BASE = VEX_TYPE_BIT_VECTOR;
    RtlDeclGen->KIND = RTL_KIND_NONE;
    RtlDeclGen->DIR  = RTL_DIR_INOUT;

    SetVasyRtlDeclarRead( RtlDeclGen );
    SetVasyRtlDeclarAsg( RtlDeclGen );
/*
** Propagation
*/
    sprintf( Buffer, "rtlprop_%ld", VasyNumberDef );
    VexAtomProp = createvexatomvec( Buffer,  MaxWidth - 1, 1 );

    if ( VasyFlagLax ) VasyAllianceAddKeepSignal( VexAtomProp );

    RtlDeclProp = addrtldecl( VasyRtlFigure, dupvexexpr( VexAtomProp ), RTL_DECLAR_SIGNAL );
    RtlDeclProp->BASE = VEX_TYPE_BIT_VECTOR;
    RtlDeclProp->KIND = RTL_KIND_NONE;
    RtlDeclProp->DIR  = RTL_DIR_INOUT;

    SetVasyRtlDeclarRead( RtlDeclProp );
    SetVasyRtlDeclarAsg( RtlDeclProp );
  }

  VasyNumberDef++;
/*
** The two operands should be transformed to atomic expressions with the same widths
*/
  Operand1 = VasyAllianceTreatVexOperandAtom( Operand1 );
  Operand2 = VasyAllianceTreatVexOperandAtom( Operand2 );

  if ( ( Oper == VEX_GT ) ||
       ( Oper == VEX_LE ) )
  {
    Operand3 = Operand1;
    Operand1 = Operand2;
    Operand2 = Operand3;
  }

  VexAtom5 = createvexatomvec( GetVexAtomValue( Operand1 ), MaxWidth - 1, MaxWidth - 1 );
  VexAtom6 = createvexatomvec( GetVexAtomValue( Operand2 ), MaxWidth - 1, MaxWidth - 1 );
  VexAtom6 = createvexunaryexpr( VEX_NOT, 1, VexAtom6 );
  VexAtom7 = createvexatomvec( GetVexAtomValue( VexAtomCarry ),  MaxWidth -1, MaxWidth - 1 );

  VexSub = createvexbinexpr( VEX_XOR, 1, VexAtom5, VexAtom6 );
  VexSub = createvexbinexpr( VEX_XOR, 1, VexSub  , VexAtom7 );

  if ( ( Oper == VEX_GE ) ||
       ( Oper == VEX_LE ) )
  {
    VexSub = createvexunaryexpr( VEX_NOT, 1, VexSub );
  }

  RtlAsgSub = addrtlasg( VasyRtlFigure, dupvexexpr( VexAtomSub ), RTL_ASG_COMBINATORIAL );
  RtlAsgSub->VEX_DATA = simpvexexpr( VexSub );

  VexAtom1 = VasyAllianceCreateVexCarry( Operand1 );
  VexAtom2 = VasyAllianceCreateVexCarry( Operand2 );
  VexAtom2 = createvexunaryexpr( VEX_NOT, MaxWidth - 1, VexAtom2 );
/*
** Carry
*/
  if ( ( VasyFlagCLA             ) &&
       ( MaxWidth >= VasyFlagCLA ) )
  {
/*
** Generation 
*/
    VexGen = createvexbinexpr( VEX_AND, MaxWidth - 1,
                               dupvexexpr( VexAtom1 ), dupvexexpr( VexAtom2 ) );

    RtlAsgGen = addrtlasg( VasyRtlFigure, dupvexexpr( VexAtomGen ), RTL_ASG_COMBINATORIAL );
    RtlAsgGen->VEX_DATA = simpvexexpr( VexGen );
/*
** Propagation
*/
    VexProp = createvexbinexpr( VEX_OR, MaxWidth - 1,
                               VexAtom1, VexAtom2 );

    RtlAsgProp = addrtlasg( VasyRtlFigure, dupvexexpr( VexAtomProp ),
                            RTL_ASG_COMBINATORIAL );
    RtlAsgProp->VEX_DATA = simpvexexpr( VexProp );
/*
** Carry-Look ahead
*/
    for ( Index = 1; Index < MaxWidth; Index++ )
    {
      VexGen   = createvexatomvec( GetVexAtomValue( VexAtomGen   ), Index, Index );
      VexProp  = createvexatomvec( GetVexAtomValue( VexAtomProp  ), Index, Index );

      if ( Index == 1 )
      {
        VexCarry = createvexatomvec( GetVexAtomValue( VexAtomCarry ), 0, 0 );
      }
      else
      {
        VexCarry = dupvexexpr( VexCarry );
      }

      VexCarry = createvexbinexpr( VEX_AND, 1, VexProp , VexCarry );
      VexCarry = createvexbinexpr( VEX_OR , 1, VexCarry, VexGen   );

      VexAtom3 = createvexatomvec( GetVexAtomValue( VexAtomCarry ), Index , Index );

      if ( VasyFlagLax ) VasyAllianceAddKeepSignal( VexAtom3 );

      RtlAsgCarry = addrtlasg( VasyRtlFigure, VexAtom3, RTL_ASG_COMBINATORIAL );
      RtlAsgCarry->VEX_DATA = VexCarry;
    }
  }
  else
  {
/*
** Ripple Carry
*/
    VexAtom3 = createvexatomvec( GetVexAtomValue( VexAtomCarry ), MaxWidth - 2, 0 );
    VexAtom4 = createvexatomvec( GetVexAtomValue( VexAtomCarry ), MaxWidth - 1, 1 );

    VexAnd1 = createvexbinexpr( VEX_AND, MaxWidth - 1, dupvexexpr( VexAtom1 ),
                                                       dupvexexpr( VexAtom2 ) );
    VexAnd2 = createvexbinexpr( VEX_AND, MaxWidth - 1, VexAtom1, dupvexexpr( VexAtom3 ) );
    VexAnd3 = createvexbinexpr( VEX_AND, MaxWidth - 1, VexAtom2, VexAtom3 );

    VexCarry = createvexbinexpr( VEX_OR, MaxWidth - 1, VexAnd1 , VexAnd2 );
    VexCarry = createvexbinexpr( VEX_OR, MaxWidth - 1, VexCarry, VexAnd3 );

    if ( VasyFlagLax ) VasyAllianceAddKeepSignal( VexAtom4 );

    RtlAsgCarry = addrtlasg( VasyRtlFigure, VexAtom4, RTL_ASG_COMBINATORIAL );
    RtlAsgCarry->VEX_DATA = simpvexexpr( VexCarry );
  }
/*
** Carry In
*/
  VexAtom0 = createvexatomvec( GetVexAtomValue( VexAtomCarry ), 0, 0 );
  RtlAsgCarry = addrtlasg( VasyRtlFigure, VexAtom0,  RTL_ASG_COMBINATORIAL );
  RtlAsgCarry->VEX_DATA = createvexatomlit( VEX_ATOM_ONE );

  freevexexpr( VexExpr );

  return( VexAtomSub );
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyAllianceTreatVexEqual                 |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyAllianceTreatVexEqual( VexExpr )

  vexexpr *VexExpr;
{
  rtldecl_list *RtlDeclNXor;
  rtlasg_list  *RtlAsgNXor;
  vexexpr      *Operand1;
  vexexpr      *Operand2;
  vexexpr      *VexNXor;
  vexexpr      *VexAnd;
  vexexpr      *VexAtomNXor;
  vexexpr      *VexAtom;
  char          Buffer[ 32 ];
  int           MaxWidth;
  int           Index;
  long          Oper;

  if ( ! VasyFlagEqual )
  {
    return( VexExpr ); 
  }

  Oper = GetVexOperValue( VexExpr );

  Operand1 = GetVexOperand( VexExpr->OPERAND       );
  Operand2 = GetVexOperand( VexExpr->OPERAND->NEXT );

  MaxWidth = Operand1->WIDTH;
  if ( Operand2->WIDTH > MaxWidth ) MaxWidth = Operand2->WIDTH;

  if  ( ( MaxWidth <= 1            ) ||
        ( MaxWidth < VasyFlagEqual ) )
  {
    return( VexExpr );
  }

  sprintf( Buffer, "rtlnxor_%ld", VasyNumberDef++ );
  VexAtomNXor = createvexatomvec( Buffer,  MaxWidth - 1, 0 );

  RtlDeclNXor = addrtldecl( VasyRtlFigure, dupvexexpr( VexAtomNXor ), RTL_DECLAR_SIGNAL );
  RtlDeclNXor->BASE = VEX_TYPE_BIT_VECTOR;
  RtlDeclNXor->KIND = RTL_KIND_NONE;
  RtlDeclNXor->DIR  = RTL_DIR_INOUT;

  SetVasyRtlDeclarRead( RtlDeclNXor );
  SetVasyRtlDeclarAsg( RtlDeclNXor );
/*
** The two operands should be transformed to atomic expressions with the same widths
*/
  Operand1 = VasyAllianceTreatVexOperandAtom( Operand1 );
  Operand2 = VasyAllianceTreatVexOperandAtom( Operand2 );

  VexNXor = createvexbinexpr( VEX_NXOR, MaxWidth, 
                              dupvexexpr( Operand1 ), dupvexexpr( Operand2 ) );

  if ( VasyFlagLax ) VasyAllianceAddKeepSignal( VexAtomNXor );

  RtlAsgNXor = addrtlasg( VasyRtlFigure, VexAtomNXor, RTL_ASG_COMBINATORIAL );
  RtlAsgNXor->VEX_DATA = VexNXor;

  if ( Oper == VEX_EQ ) VexAnd = createvexoper( VEX_AND , MaxWidth );
  else                  VexAnd = createvexoper( VEX_NAND, MaxWidth );
  
  for ( Index = 0; Index < MaxWidth; Index++ )
  {
    VexAtom = createvexatomvec( GetVexAtomValue( VexAtomNXor ), Index, Index );
    addvexhexpr( VexAnd, VexAtom ); 
  }

  freevexexpr( VexExpr );

  return( VexAnd );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasyAllianceTreatVexAtomDontCare        |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyAllianceTreatVexAtomDontCare( VexAtom )
  
  vexexpr *VexAtom;
{
  char *Value;
  int   Index;
  char  Char;

  Value = GetVexAtomValue( VexAtom );

  for ( Index = 0; Value[ Index ] != '\0'; Index++ )
  {
    Char = Value[ Index ];

    if ( ( Char != '0'  ) &&
         ( Char != '1'  ) &&
         ( Char != '\'' ) &&
         ( Char != '"'  ) )
    {
      for ( Index = 0; Value[ Index ] != '\0'; Index++ )
      {
        Char = Value[ Index ];

        if ( ( Char != '0'  ) &&
             ( Char != '1'  ) &&
             ( Char != '\'' ) &&
             ( Char != '"'  ) ) VasyBuffer[ Index ] = '0';
        else                    VasyBuffer[ Index ] = Char;
      }

      VasyBuffer[ Index ] = '\0';

      Value = namealloc( VasyBuffer );
      SetVexAtomValue( VexAtom, Value );

      break;
    }
  }

  return( VexAtom );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasyAllianceTreatVex                    |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyAllianceTreatVex( VexExpr )

  vexexpr *VexExpr;
{
  chain_list *ScanChain;
  vexexpr    *Operand;
  long        Oper;

  if ( VexExpr == (vexexpr *)0 )
  {
    return( VexExpr );
  }

  if ( IsVexNodeAtom( VexExpr ) )
  {
    if ( IsVexAtomLiteral( VexExpr ) )
    {
      VexExpr = VasyAllianceTreatVexAtomDontCare( VexExpr );
    }

    return( VexExpr );
  }
  else
  if ( IsVexNodeOper( VexExpr ) )
  {
    Oper = GetVexOperValue( VexExpr );

    if ( ( Oper == VEX_ADD ) ||
         ( Oper == VEX_SUB ) )
    {
      return( VasyAllianceTreatVexArith( VexExpr ) );
    }
    else
    if ( ( Oper == VEX_LT ) ||
         ( Oper == VEX_GT ) ||
         ( Oper == VEX_LE ) ||
         ( Oper == VEX_GE ) )
    {
      return( VasyAllianceTreatVexRelational( VexExpr ) );
    }
    else
    if ( Oper == VEX_NEG )
    {
      return( VasyAllianceTreatVexNeg( VexExpr ) );
    }
    else
    if ( ( Oper == VEX_EQ ) ||
         ( Oper == VEX_NE ) )
    {
      return( VasyAllianceTreatVexEqual( VexExpr ) );
    }
    else
    if ( Oper == VEX_ABS )
    {
      return( VasyAllianceTreatVexAbs( VexExpr ) );
    }
  }

  for ( ScanChain  = VexExpr->OPERAND;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    Operand = GetVexOperand( ScanChain );
    Operand = VasyAllianceTreatVex( Operand );
    SetVexOperand( ScanChain, Operand );
  }

  return( VexExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyAllianceTreatAsg                      |
|                                                             |
\------------------------------------------------------------*/

static void VasyAllianceTreatAsg( RtlAsg )

  rtlasg_list *RtlAsg;
{
  rtlbivex_list *RtlBiVex;
  rtldecl_list  *RtlDeclar;
  char          *Name;

  Name = getvexatomname( RtlAsg->VEX_ATOM );

  if ( Name == (char *)0 ) return;

  RtlDeclar = searchrtldecl( VasyRtlFigure, Name );

  if ( ( RtlDeclar == (rtldecl_list *)0     ) ||
       ( ! IsVasyRtlDeclarUsed( RtlDeclar ) ) ) return;

  VasyRtlAsg = RtlAsg;

  RtlAsg->VEX_DATA = VasyAllianceTreatVex( RtlAsg->VEX_DATA );

  for ( RtlBiVex  = RtlAsg->BIVEX;
        RtlBiVex != (rtlbivex_list *)0;
        RtlBiVex  = RtlBiVex->NEXT )
  {
    RtlBiVex->VEX_COND = VasyAllianceTreatVex( RtlBiVex->VEX_COND );
    RtlBiVex->VEX_DATA = VasyAllianceTreatVex( RtlBiVex->VEX_DATA );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyAllianceTreatMap                      |
|                                                             |
\------------------------------------------------------------*/

static void VasyAllianceTreatMap( RtlInst )

  rtlins_list *RtlInst;
{
  rtlmap_list **PrevRtlMap;
  rtlmap_list  *RtlHeadMap;
  rtlmap_list  *ScanRtlMap;
  rtlmap_list  *RtlMap;
  rtlport_list *RtlPort;
  rtldecl_list *RtlDeclar;
  rtlasg_list  *RtlAssign;
  vexexpr      *ScanVexFormal;
  vexexpr      *VexFormal;
  vexexpr      *ScanVexActual;
  vexexpr      *VexActual;
  vexexpr      *VexAtom;
  char         *AtomValue;
  char         *ScanAtomValue;
  char          Buffer[ 64 ];
  int           Type;
  int           Left;
  int           Right;
  int           ScanLeft;
  int           ScanRight;
  int           NextStep;
  int           Modified;
/*
** Check actual port map
*/
  for ( RtlMap  = RtlInst->MAP;
        RtlMap != (rtlmap_list *)0;
        RtlMap  = RtlMap->NEXT )
  {
    VexFormal = RtlMap->VEX_FORMAL;
    AtomValue = GetVexAtomValue( VexFormal ); 
    RtlPort   = searchrtlmodport( VasyRtlFigure, RtlInst->MODEL, AtomValue );
    VexAtom   = RtlPort->VEX_ATOM;
  
    VexActual = RtlMap->VEX_ACTUAL;
  
    if ( ( ! IsVexNodeAtom( VexActual )    ) ||
         (   IsVexAtomLiteral( VexActual ) ) )
    {
      sprintf( Buffer, "rtl_map_%ld", VasyNumberDef++ );
    
      if ( isvextypevector( Type ) )
      {
        VexAtom = createvexatomvec( Buffer, VexActual->WIDTH - 1, 0 );
      }
      else
      {
        VexAtom = createvexatombit( Buffer );
      }
    
      RtlDeclar = addrtldecl( VasyRtlFigure, dupvexexpr( VexAtom ), RTL_DECLAR_SIGNAL );
      RtlDeclar->BASE = RtlPort->BASE;
      RtlDeclar->KIND = RTL_KIND_NONE;
      RtlDeclar->DIR  = RTL_DIR_INOUT;
    
      SetVasyRtlDeclarRead( RtlDeclar );
      SetVasyRtlDeclarAsg( RtlDeclar );
  
      RtlAssign = addrtlasg( VasyRtlFigure, dupvexexpr( VexAtom ), RTL_ASG_COMBINATORIAL );
      RtlAssign->VEX_DATA = VexActual;
  
      VexActual = VexAtom;
    }

    RtlMap->VEX_ACTUAL = VexActual;
  }
/*
** Split formal port map list in two lists : Bit and Vector
*/
  RtlHeadMap = (rtlmap_list *)0;
  PrevRtlMap = &RtlInst->MAP;
  RtlMap     = RtlInst->MAP;

  while ( RtlMap != (rtlmap_list *)0 )
  {
    VexFormal = RtlMap->VEX_FORMAL;
    AtomValue = GetVexAtomValue( VexFormal ); 
    RtlPort   = searchrtlmodport( VasyRtlFigure, RtlInst->MODEL, AtomValue );
    VexAtom   = RtlPort->VEX_ATOM;

    if ( ( IsVexAtomBit( VexAtom ) ) ||
         ( VexAtom->WIDTH <= 1     ) )
    {
      ScanRtlMap   = RtlMap->NEXT;
      *PrevRtlMap  = ScanRtlMap;

      RtlMap->NEXT = RtlHeadMap;
      RtlHeadMap   = RtlMap;

      RtlMap = ScanRtlMap;
    }
    else
    {
      RtlMap->USER = (void *)VexAtom;

      PrevRtlMap = &RtlMap->NEXT;
      RtlMap     = RtlMap->NEXT;
    }
  }
/*
** Compact formal port map
*/
  for ( RtlMap  = RtlInst->MAP;
        RtlMap != (rtlmap_list *)0;
        RtlMap  = RtlMap->NEXT )
  {
    if ( RtlMap->FLAGS ) continue;

    VexFormal = RtlMap->VEX_FORMAL;
    AtomValue = GetVexAtomValue( VexFormal ); 
    Left      = VexFormal->LEFT;
    Right     = VexFormal->RIGHT;

    VexActual = RtlMap->VEX_ACTUAL;

    VexAtom = (vexexpr *)RtlMap->USER;

    if ( IsVexAtomDown( VexAtom ) ) NextStep = -1;
    else                            NextStep =  1;

    Modified = 0;

    for ( ScanRtlMap  = RtlInst->MAP;
          ScanRtlMap != (rtlmap_list *)0;
          ScanRtlMap  = ScanRtlMap->NEXT )
    {
      if ( ( ScanRtlMap->FLAGS    ) ||
           ( ScanRtlMap == RtlMap ) ) continue;

      ScanVexFormal = ScanRtlMap->VEX_FORMAL;
      ScanAtomValue = GetVexAtomValue( ScanVexFormal );
      ScanLeft      = ScanVexFormal->LEFT;
      ScanRight     = ScanVexFormal->RIGHT;

      ScanVexActual = ScanRtlMap->VEX_ACTUAL;

      if ( AtomValue == ScanAtomValue )
      {
        if ( Left  - NextStep == ScanRight )
        {
          Modified = 1;

          if ( IsVasyDebugLevel1() )
          {
            VasyPrintf( stdout, "Should Concat !\n" );
            viewvexexprln( ScanVexFormal );
            viewvexexprln( VexFormal );
          }

          VexFormal->LEFT   = ScanVexFormal->LEFT;
          VexFormal->WIDTH += ScanVexFormal->WIDTH;

          if ( NextStep == -1 ) SetVexAtomDown( VexFormal );
          else                  SetVexAtomUp  ( VexFormal );

          VexActual = optimvexbinexpr( VEX_CONCAT, VexActual->WIDTH + ScanVexActual->WIDTH,
                                       dupvexexpr( ScanVexActual ), VexActual );

          ScanRtlMap->FLAGS = 1;
        }
        else
        if ( Right + NextStep == ScanLeft  )
        {
          Modified = 1;

          if ( IsVasyDebugLevel1() )
          {
            fprintf( stdout, "Should Concat !\n" );
            viewvexexprln( VexFormal );
            viewvexexprln( ScanVexFormal );
          }

          VexFormal->RIGHT  = ScanVexFormal->RIGHT;
          VexFormal->WIDTH += ScanVexFormal->WIDTH;

          if ( NextStep == -1 ) SetVexAtomDown( VexFormal );
          else                  SetVexAtomUp  ( VexFormal );

          VexActual = optimvexbinexpr( VEX_CONCAT, VexActual->WIDTH + ScanVexActual->WIDTH,
                                       VexActual, dupvexexpr( ScanVexActual ) );

          ScanRtlMap->FLAGS = 1;
        }
      }
    }

    if ( Modified )
    {
      RtlMap->VEX_FORMAL = VexFormal;
      RtlMap->VEX_ACTUAL = simpvexexpr( VexActual );
    }
  }
/*
** Concat Bit and Vector port map lists
*/
  *PrevRtlMap = RtlHeadMap;
/*
** Delete old formal port map
*/
  PrevRtlMap = &RtlInst->MAP;
  RtlMap     = RtlInst->MAP;

  while ( RtlMap != RtlHeadMap )
  {
    if ( RtlMap->FLAGS ) 
    {
      ScanRtlMap  = RtlMap;
      RtlMap      = RtlMap->NEXT;
      *PrevRtlMap = RtlMap;

      freevexexpr( ScanRtlMap->VEX_FORMAL );
      freevexexpr( ScanRtlMap->VEX_ACTUAL );
      freertlmap( ScanRtlMap );
    }
    else
    {
      PrevRtlMap   = &RtlMap->NEXT;
      RtlMap->USER = (void *)0;
      RtlMap       = RtlMap->NEXT;
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyDriveAllianceTreatDeclar              |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveAllianceTreatDeclar( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtldecl_list  *RtlDeclar;
  rtlsym        *RtlSymbol;
  rtlasg_list   *RtlAsg;
  rtlasg_list   *NewAssign;
  vexexpr       *RegAtom;
  vexexpr       *VexAtom;
  vexexpr       *VexDeclar;
  vexexpr       *VexExpr;
  vexexpr       *VexInit;
  char          *AtomValue;
  char          *AtomName;
  char           Buffer[ 32 ];
  int            Type;
  int            ScanPos;
  char           Literal;

  for ( Type = 0; Type < RTL_MAX_DECLAR_TYPE; Type++ )
  {
    for ( RtlDeclar  = RtlFigure->DECLAR[ Type ];
          RtlDeclar != (rtldecl_list *)0;
          RtlDeclar  = RtlDeclar->NEXT )
    {
      RtlDeclar->BASE = VasyConvertType[ RtlDeclar->BASE ];
      RtlDeclar->KIND = RTL_KIND_NONE;

      if ( Type == RTL_DECLAR_PORT )
      {
        if ( RtlDeclar->BASE == VEX_TYPE_REG_VECTOR )
        {
          RtlDeclar->BASE = VEX_TYPE_BIT_VECTOR;
        }
        else
        if ( RtlDeclar->BASE == VEX_TYPE_REG_BIT )
        {
          RtlDeclar->BASE = VEX_TYPE_BIT;
        }
      }

      if ( ( RtlDeclar->DIR == RTL_DIR_IN           ) ||
           ( ( ! IsVasyRtlDeclarUsed( RtlDeclar ) ) &&
             ( Type != RTL_DECLAR_PORT            ) ) ) continue;

      VexDeclar  = RtlDeclar->VEX_ATOM;
      VexInit    = RtlDeclar->VEX_INIT;

      AtomName   = GetVexAtomValue( VexDeclar );

      if ( VexInit != (vexexpr *)0 ) AtomValue = GetVexAtomValue( VexInit   );
      else 
      {
#if 1  /** Francois Donnet: 16/01/2003: do not initialize by default
       *** because someone else could do it in an upper hierachy.
       *** It's better not to correct description and to let user
       *** with it's own error than to build some...
       **/
         if ( ( RtlDeclar->DIR == RTL_DIR_INOUT           ) ) continue;
#endif
         
         /*default init for outputs*/
         AtomValue = (char *)0;
      }
  
      for ( ScanPos = 0; ScanPos < VexDeclar->WIDTH; ScanPos++ )
      {
        RtlSymbol = &RtlDeclar->DECL_SYM[ ScanPos ];
  
        if ( ( ( IsVasyRtlDeclarRead( RtlSymbol  ) ) ||
               ( Type == RTL_DECLAR_PORT           ) ) &&
             ( ! IsVasyRtlDeclarAsg( RtlSymbol )     ) )
        {
          if ( Type == RTL_DECLAR_PORT )
          {
            if ( RtlSymbol->INDEX != -1 )
            {
              VasyPrintf( stdout, "WARNING add default assignment for port %s(%d)\n",
                        RtlSymbol->NAME, RtlSymbol->INDEX );
            }
            else
            {
              VasyPrintf( stdout, "WARNING add default assignment for port %s\n",
                        RtlSymbol->NAME );
            }
          }

          if ( AtomValue != (char *)0 ) Literal = AtomValue[ ScanPos + 1 ];
          else                          Literal = '0';
  
          sprintf( Buffer, "'%c'", Literal );
          VexExpr = createvexatombit( Buffer );
  
          if ( RtlSymbol->INDEX != -1 )
          {
            VexAtom = createvexatomvec( AtomName, RtlSymbol->INDEX, RtlSymbol->INDEX );
            RtlAsg  = addrtlasg( RtlFigure, VexAtom, RTL_ASG_COMBINATORIAL );
          }
          else
          {
            VexAtom = createvexatombit( AtomName );
            RtlAsg  = addrtlasg( RtlFigure, VexAtom, RTL_ASG_COMBINATORIAL );
          }
  
          RtlAsg->VEX_DATA = VexExpr;
        }
      }
    }
  }

  for ( RtlAsg  = RtlFigure->ASSIGN;
        RtlAsg != (rtlasg_list *)0;
        RtlAsg  = RtlAsg->NEXT )
  {
    if ( ( RtlAsg->TYPE == RTL_ASG_REGISTER ) ||
         ( RtlAsg->TYPE == RTL_ASG_TRISTATE ) )
    {
      VexAtom   = RtlAsg->VEX_ATOM;
      AtomValue = GetVexAtomValue( VexAtom );
      RtlDeclar = searchrtldecl( RtlFigure, AtomValue );

      if ( RtlAsg->TYPE == RTL_ASG_REGISTER )
      {
/*
** Special treatments for out port registers (they are not authorized in vbe !)
*/
        if ( RtlDeclar->TYPE == RTL_DECLAR_PORT )
        {
          sprintf( Buffer, "rtlalc_%ld", VasyNumberDef++ );

          if ( VexAtom->WIDTH > 1 )
          {
            RegAtom = createvexatomvec( Buffer, VexAtom->LEFT, VexAtom->RIGHT );
          }
          else
          {
            RegAtom = createvexatombit( Buffer );
          }

          RtlDeclar = addrtldecl( RtlFigure, dupvexexpr( RegAtom ), RTL_DECLAR_SIGNAL );
          SetVasyRtlDeclarRead( RtlDeclar );
          SetVasyRtlDeclarAsg( RtlDeclar );

          RtlDeclar->DIR  = RTL_DIR_INOUT;

          if ( VexAtom->WIDTH > 1 ) RtlDeclar->BASE = VEX_TYPE_BIT_VECTOR;
          else                      RtlDeclar->BASE = VEX_TYPE_BIT;

          RtlAsg->VEX_ATOM = dupvexexpr( RegAtom );

          NewAssign = addrtlasg( RtlFigure, VexAtom, RTL_ASG_COMBINATORIAL );
          NewAssign->VEX_DATA = RegAtom;

          VexAtom = RegAtom;
        }

        RtlDeclar->KIND = RTL_KIND_REGISTER;

        if ( isvextypevector( RtlDeclar->BASE ) ) RtlDeclar->BASE = VEX_TYPE_REG_VECTOR;
        else                                      RtlDeclar->BASE = VEX_TYPE_REG_BIT;
      }
      else
      {
        RtlDeclar->KIND = RTL_KIND_BUS;

        if ( isvextypevector( RtlDeclar->BASE ) ) RtlDeclar->BASE = VEX_TYPE_MUX_VECTOR;
        else                                      RtlDeclar->BASE = VEX_TYPE_MUX_BIT;
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyDriveAllianceTreatFigure              |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveAllianceTreatFigure( RtlFigure, FileName, PRtlFigureVbe, PRtlFigureVst )
                                          

  rtlfig_list  *RtlFigure;
  char         *FileName;
  rtlfig_list **PRtlFigureVbe;
  rtlfig_list **PRtlFigureVst;
{
  rtldecl_list  *RtlDeclar;
  rtldecl_list  *DelDeclar;
  rtldecl_list  *NewDeclar;
  rtldecl_list  *PortDeclar;
  rtldecl_list **PrevDeclar;
  rtlins_list   *RtlInst;
  rtlmap_list   *RtlMap;
  rtlmod_list   *RtlModel;
  rtlport_list  *RtlPort;
  rtlasg_list   *RtlAsg;
  vexexpr       *VexAtom;
  vexexpr       *VexActual;
  vexexpr       *VexFormal;
  char          *AtomValue;
  char          *NewName;
  char          *InsName;
  rtlfig_list   *NewFigure;
  int            Type;
  int            Dir;

  RtlFigure->NAME = namealloc( FileName );

  *PRtlFigureVbe = (rtlfig_list *)0;
  *PRtlFigureVst = (rtlfig_list *)0;

  VasyRtlFigure  = RtlFigure;
  VasyKeepSignal = (chain_list *)0;

  for ( RtlModel  = RtlFigure->MODEL;
        RtlModel != (rtlmod_list *)0;
        RtlModel  = RtlModel->NEXT )
  {
    for ( RtlPort  = RtlModel->PORT;
          RtlPort != (rtlport_list *)0;
          RtlPort  = RtlPort->NEXT )
    {
      RtlPort->BASE = VasyConvertType[ RtlPort->BASE ];
    }
  }

  for ( RtlInst  = RtlFigure->INSTANCE;
        RtlInst != (rtlins_list *)0;
        RtlInst  = RtlInst->NEXT )
  {
    VasyAllianceTreatMap( RtlInst );
  }

  for ( RtlAsg  = RtlFigure->ASSIGN;
        RtlAsg != (rtlasg_list *)0;
        RtlAsg  = RtlAsg->NEXT )
  {
    VasyAllianceTreatAsg( RtlAsg );
  }

  VasyDriveAllianceTreatDeclar( RtlFigure );

  if ( RtlFigure->MODEL != (rtlmod_list *)0 )
  {
    if ( RtlFigure->ASSIGN == (rtlasg_list *)0 )
    {
      *PRtlFigureVst  = RtlFigure;
      RtlFigure->NAME = namealloc( FileName );
    }
    else
    {
      *PRtlFigureVst = RtlFigure;

      NewName = autallocblock( strlen( FileName ) + 10 );
      sprintf( NewName, "%s_model", FileName );

      NewFigure = addrtlfig( NewName );

      autfreeblock( NewName );
      NewName = NewFigure->NAME;

      *PRtlFigureVbe = NewFigure;

      NewFigure->ASSIGN = RtlFigure->ASSIGN;
      RtlFigure->ASSIGN = (rtlasg_list *)0;

      for ( Type = 0; Type < RTL_MAX_DECLAR_TYPE; Type++ )
      {
        for ( RtlDeclar  = RtlFigure->DECLAR[ Type ];
              RtlDeclar != (rtldecl_list *)0;
              RtlDeclar  = RtlDeclar->NEXT )
        {
          ClearVasyRtlDeclarUsed( RtlDeclar );

          VexAtom   = dupvexexpr( RtlDeclar->VEX_ATOM );
          NewDeclar = addrtldecl( NewFigure, VexAtom, RtlDeclar->TYPE );

          NewDeclar->DIR   = RtlDeclar->DIR;
          NewDeclar->KIND  = RtlDeclar->KIND;
          NewDeclar->BASE  = RtlDeclar->BASE;
          NewDeclar->FLAGS = RtlDeclar->FLAGS;

          if ( ( Type == RTL_DECLAR_PORT   ) &&
               ( VasyFlagPower             ) &&
               ( IsVexNodeAtom( VexAtom  ) ) )
          {
            AtomValue = GetVexAtomValue( VexAtom );

            if ( ( AtomValue == VasyPowerVdd ) ||
                 ( AtomValue == VasyPowerVss ) )
            {
              SetVasyRtlDeclarRead( NewDeclar ); 
            }
          }
          else
          if ( Type == RTL_DECLAR_CONSTANT )
          {
            NewDeclar->VEX_INIT = dupvexexpr( RtlDeclar->VEX_INIT );
          }
        }
      }

      VasySimplifyRtlFig( NewFigure );
      VasySimplifyRtlFig( RtlFigure );

      PrevDeclar = &NewFigure->DECLAR[ RTL_DECLAR_SIGNAL ];
      NewDeclar  = NewFigure->DECLAR[ RTL_DECLAR_SIGNAL ];

      while ( NewDeclar != (rtldecl_list *)0 )
      {
        if ( IsVasyRtlDeclarUsed( NewDeclar ) )
        {
          VexAtom   = NewDeclar->VEX_ATOM;
          AtomValue = GetVexAtomValue( VexAtom );
          RtlDeclar = searchrtldecl( RtlFigure, AtomValue );

          if ( ( RtlDeclar != (rtldecl_list *)0   ) &&
               ( IsVasyRtlDeclarUsed( RtlDeclar ) ) )
          {
            Dir = RTL_DIR_OUT;

            if ( ( IsVasyRtlDeclarAsg( NewDeclar ) ) &&
                 ( IsVasyRtlDeclarRead( NewDeclar ) ) ) Dir = RTL_DIR_INOUT;
            else
            if ( IsVasyRtlDeclarRead( NewDeclar ) ) Dir = RTL_DIR_IN;

            PortDeclar       = NewDeclar;
            PortDeclar->DIR  = Dir;
            PortDeclar->TYPE = RTL_DECLAR_PORT;

            *PrevDeclar = NewDeclar->NEXT;
            NewDeclar   = NewDeclar->NEXT;

            PortDeclar->NEXT = NewFigure->DECLAR[ RTL_DECLAR_PORT ];
            NewFigure->DECLAR[ RTL_DECLAR_PORT ] = PortDeclar;

            continue;
          }
        }

        PrevDeclar = &NewDeclar->NEXT;
        NewDeclar  =  NewDeclar->NEXT;
      }

      InsName = autallocblock( strlen( FileName ) + 10 );
      sprintf( InsName, "%s_inst", FileName );

      RtlModel = addrtlmod( RtlFigure, NewName );
      RtlInst  = addrtlins( RtlFigure, InsName, RtlModel );

      autfreeblock( InsName );
      InsName = RtlInst->NAME;

      NewDeclar  = NewFigure->DECLAR[ RTL_DECLAR_PORT ];
      PrevDeclar = &NewFigure->DECLAR[ RTL_DECLAR_PORT ];

      while ( NewDeclar != (rtldecl_list *)0 )
      {
        if ( ! IsVasyRtlDeclarUsed( NewDeclar ) )
        {
          VexAtom   = NewDeclar->VEX_ATOM;
          AtomValue = GetVexAtomValue( VexAtom );

          freevexexpr( NewDeclar->VEX_ATOM );
          freevexexpr( NewDeclar->VEX_INIT );
          freertlsym( NewDeclar->DECL_SYM );

          DelDeclar = NewDeclar;
          NewDeclar = NewDeclar->NEXT;
          *PrevDeclar = NewDeclar;

          delauthelem( NewFigure->HASH_DECLAR, AtomValue );

          freertldecl( DelDeclar );
        }
        else
        {
          PrevDeclar = &NewDeclar->NEXT;
          NewDeclar  = NewDeclar->NEXT;
        }
      }

      VasyDriveAllianceTreatDeclar( NewFigure );

      for ( NewDeclar  = NewFigure->DECLAR[ RTL_DECLAR_PORT ];
            NewDeclar != (rtldecl_list *)0;
            NewDeclar  = NewDeclar->NEXT )
      {
        Dir = RTL_DIR_OUT;

        if ( ( IsVasyRtlDeclarAsg( NewDeclar ) ) &&
             ( IsVasyRtlDeclarRead( NewDeclar ) ) ) Dir = RTL_DIR_INOUT;
        else
        if ( IsVasyRtlDeclarRead( NewDeclar ) ) Dir = RTL_DIR_IN;

        NewDeclar->DIR = Dir;

        VexAtom = dupvexexpr( NewDeclar->VEX_ATOM );
        RtlPort = addrtlmodport( RtlFigure, RtlModel, VexAtom, 
                                 NewDeclar->BASE, NewDeclar->DIR );

        VexFormal = dupvexexpr( NewDeclar->VEX_ATOM );
        VexActual = dupvexexpr( NewDeclar->VEX_ATOM );

        RtlMap = addrtlinsmap( RtlFigure, RtlInst, VexFormal, VexActual );
      }
    }
/*
** In a vst figure REG_BIT and REG_VECTOR are not supported ! 
*/
    for ( Type = 0; Type < RTL_MAX_DECLAR_TYPE; Type++ )
    {
      for ( RtlDeclar  = (*PRtlFigureVst)->DECLAR[ Type ];
            RtlDeclar != (rtldecl_list *)0;
            RtlDeclar  = RtlDeclar->NEXT )
      {
        if ( RtlDeclar->BASE == VEX_TYPE_REG_VECTOR )
        {
          RtlDeclar->BASE = VEX_TYPE_BIT_VECTOR;
          RtlDeclar->KIND = RTL_KIND_NONE;
        }
        else
        if ( RtlDeclar->BASE == VEX_TYPE_REG_BIT )
        {
          RtlDeclar->BASE = VEX_TYPE_BIT;
          RtlDeclar->KIND = RTL_KIND_NONE;
        }
      }
    }
  }
  else
  {
    *PRtlFigureVbe = RtlFigure;
    RtlFigure->NAME = namealloc( FileName );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyDriveAlliancePort                     |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveAlliancePort( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtldecl_list *RtlDeclar;
  vexexpr      *VexPort;
  char         *PortName;
  char         *PortDir;
  char         *PortBase;
  char         *PortKind;
  char         *PortVecDir;

  RtlDeclar = RtlFigure->DECLAR[ RTL_DECLAR_PORT ];

  if ( RtlDeclar != (rtldecl_list *)0 )
  {
    fprintf( VasyFile, "PORT(\n" );

    while ( RtlDeclar != (rtldecl_list *)0 )
    {
      VexPort  = RtlDeclar->VEX_ATOM;
      PortName = GetVexAtomValue( VexPort );
      PortDir  = RTL_DIR_TYPE[ RtlDeclar->DIR ];
      PortKind = RTL_KIND_TYPE[ RtlDeclar->KIND ];
      PortBase = VEX_TYPE_NAME[ RtlDeclar->BASE ];

      fprintf( VasyFile, "  %s\t: %s %s", PortName, PortDir, PortBase );
  
      if ( isvextypevector( RtlDeclar->BASE ) )
      {
        if ( IsVexAtomUp( VexPort ) ) PortVecDir = "TO";
        else                          PortVecDir = "DOWNTO";
  
        fprintf( VasyFile, "(%d %s %d)", VexPort->LEFT, PortVecDir, VexPort->RIGHT );
      }

      if ( RtlDeclar->KIND != RTL_KIND_NONE ) fprintf( VasyFile, " %s", PortKind );

      if ( RtlDeclar->NEXT != (rtldecl_list *)0 )
      {
        fprintf( VasyFile, ";\n" );
      }
  
      RtlDeclar = RtlDeclar->NEXT;
    }

    fprintf( VasyFile, "\n);\n" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyDriveAllianceDeclar                   |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveAllianceDeclar( RtlFigure, Type )

  rtlfig_list *RtlFigure;
  int          Type;
{
  rtldecl_list *RtlDeclar;
  vexexpr      *VexDeclar;
  char         *AtomValue;
  vexexpr      *VexInit;
  char         *DeclarName;
  char         *DeclarDir;
  char         *DeclarBase;
  char         *DeclarVecDir;
  char         *DeclarKind;

  for ( RtlDeclar  = RtlFigure->DECLAR[ Type ];
        RtlDeclar != (rtldecl_list *)0;
        RtlDeclar  = RtlDeclar->NEXT )
  {
    if ( ! IsVasyRtlDeclarUsed( RtlDeclar ) ) continue;

    VexDeclar  = RtlDeclar->VEX_ATOM;
    VexInit    = RtlDeclar->VEX_INIT;
    DeclarName = GetVexAtomValue( VexDeclar );
    DeclarDir  = RTL_DIR_TYPE[ RtlDeclar->DIR ];
    DeclarBase = VEX_TYPE_NAME[ RtlDeclar->BASE ];
    DeclarKind = RTL_KIND_TYPE[ RtlDeclar->KIND ];

    if ( Type == RTL_DECLAR_CONSTANT )
    {
      fprintf( VasyFile, "  CONSTANT %s\t: %s", DeclarName, DeclarBase );
    }
    else
    {
      fprintf( VasyFile, "  SIGNAL %s\t: %s", DeclarName, DeclarBase );
    }
  
    if ( isvextypevector( RtlDeclar->BASE ) )
    {
      if ( IsVexAtomUp( VexDeclar ) ) DeclarVecDir = "TO";
      else                            DeclarVecDir = "DOWNTO";
  
      fprintf( VasyFile, "(%d %s %d)", VexDeclar->LEFT, DeclarVecDir, VexDeclar->RIGHT );
    }

    if ( RtlDeclar->KIND != RTL_KIND_NONE ) fprintf( VasyFile, " %s", DeclarKind );

    if ( Type == RTL_DECLAR_CONSTANT )
    {
      fprintf( VasyFile, " := " );

      if ( RtlDeclar->BASE == VEX_TYPE_BOOLEAN )
      {
        if ( IsVexNodeAtom( VexInit ) )
        {
          AtomValue = GetVexAtomValue( VexInit );

          if ( AtomValue == VEX_ATOM_ONE ) fprintf( VasyFile, " TRUE" );
          else                             fprintf( VasyFile, " FALSE" );
        }
        else
        {
          VasyDriveVexExprVhdl( VasyFile, RtlFigure, VexInit );
        }
      }
      else
      {
        VasyDriveVexExprVhdl( VasyFile, RtlFigure, VexInit );
      }
    }

    fprintf( VasyFile, ";\n" );

    if ( RtlDeclar->NEXT == (rtldecl_list *)0 )
    {
      fprintf( VasyFile, "\n" );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                 VasyDriveAllianceFsmDeclar                  |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveAllianceFsmDeclar( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtlfsm_list      *RtlFsm;
  rtlfsmstate_list *RtlState;

  for ( RtlFsm  = RtlFigure->FSM;
        RtlFsm != (rtlfsm_list *)0;
        RtlFsm  = RtlFsm->NEXT )
  {
    fprintf( VasyFile, "  TYPE STATE_TYPE_%s IS (", RtlFsm->NAME );

    for ( RtlState  = RtlFsm->STATE;
          RtlState != (rtlfsmstate_list *)0;
          RtlState  = RtlState->NEXT )
    {
      fprintf( VasyFile, " %s", RtlState->NAME );

      if ( RtlState->NEXT != (rtlfsmstate_list *)0 )
      {
        fprintf( VasyFile, "," );
      }
    }

    fprintf( VasyFile, " );\n" );
    fprintf( VasyFile, "  SIGNAL CS_%s : STATE_TYPE_%s;\n\n", RtlFsm->NAME, RtlFsm->NAME );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                 VasyDriveAllianceComponent                  |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveAllianceComponent( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtlmod_list  *RtlModel;
  rtlport_list *RtlPort;
  vexexpr      *VexPort;
  char         *PortName;
  char         *PortDir;
  char         *PortBase;
  char         *PortVecDir;

  for ( RtlModel  = RtlFigure->MODEL;
        RtlModel != (rtlmod_list *)0;
        RtlModel  = RtlModel->NEXT )
  {
    fprintf( VasyFile, "  COMPONENT %s\n", RtlModel->NAME );

    RtlPort = RtlModel->PORT;

    if ( RtlPort != (rtlport_list *)0 )
    {
      fprintf( VasyFile, "  PORT(\n" );
  
      while ( RtlPort != (rtlport_list *)0 )
      {
        VexPort  = RtlPort->VEX_ATOM;
        PortName = GetVexAtomValue( VexPort );
        PortDir  = RTL_DIR_TYPE[ RtlPort->DIR ];
        PortBase = VEX_TYPE_NAME[ RtlPort->BASE ];
  
        fprintf( VasyFile, "  %s\t: %s %s", PortName, PortDir, PortBase );
    
        if ( isvextypevector( RtlPort->BASE ) )
        {
          if ( IsVexAtomUp( VexPort ) ) PortVecDir = "TO";
          else                          PortVecDir = "DOWNTO";
    
          fprintf( VasyFile, "(%d %s %d)", VexPort->LEFT, PortVecDir, VexPort->RIGHT );
        }

        if ( ( RtlPort->BASE == VEX_TYPE_MUX_BIT    ) ||
             ( RtlPort->BASE == VEX_TYPE_MUX_VECTOR ) ||
             ( RtlPort->BASE == VEX_TYPE_WOR_BIT    ) || 
             ( RtlPort->BASE == VEX_TYPE_WOR_VECTOR ) )
        {
          fprintf( VasyFile, " %s ", RTL_KIND_TYPE[ RTL_KIND_BUS ] );
        }
    
        if ( RtlPort->NEXT != (rtlport_list *)0 )
        {
          fprintf( VasyFile, ";\n" );
        }
    
        RtlPort = RtlPort->NEXT;
      }

      fprintf( VasyFile, "\n  );\n" );
    }

    fprintf( VasyFile, "  END COMPONENT;\n\n" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                 VasyDriveAlliancePortMap                    |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveAlliancePortMap( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtlmod_list *RtlModel;
  rtlins_list *RtlInst;
  rtlmap_list *RtlMap;
  vexexpr     *VexFormal;
  vexexpr     *VexActual;

  for ( RtlInst  = RtlFigure->INSTANCE;
        RtlInst != (rtlins_list *)0;
        RtlInst  = RtlInst->NEXT )
  {
    RtlModel = RtlInst->MODEL;

    fprintf( VasyFile, "  %s : %s\n", RtlInst->NAME, RtlModel->NAME );
    fprintf( VasyFile, "  PORT MAP (\n" );
    RtlMap = RtlInst->MAP;

    while ( RtlMap != (rtlmap_list *)0 )
    {
      VexFormal = RtlMap->VEX_FORMAL;
      VexActual = RtlMap->VEX_ACTUAL;

      fprintf( VasyFile, "    " );
      VasyDriveVexExprVhdl( VasyFile, RtlFigure, VexFormal );
      fprintf( VasyFile, " => " );
      VasyDriveVexExprVhdl( VasyFile, RtlFigure, VexActual );

      if ( RtlMap->NEXT != (rtlmap_list *)0 )
      {
        fprintf( VasyFile, ",\n" );
      }
  
      RtlMap = RtlMap->NEXT;
    }

    fprintf( VasyFile, "\n  );\n" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyDriveAllianceEntity                   |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveAllianceEntity( RtlFigure )

  rtlfig_list *RtlFigure;
{
  fprintf( VasyFile, "--\n" );
  fprintf( VasyFile, "-- Generated by VASY\n" );
  fprintf( VasyFile, "--\n" );

  fprintf( VasyFile, "ENTITY %s IS\n", RtlFigure->NAME );

  VasyDriveAlliancePort( RtlFigure );

  fprintf( VasyFile, "END %s;\n\n", RtlFigure->NAME );
}

/*------------------------------------------------------------\
|                                                             |
|                VasyDriveAllianceGetVexCond                  |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveAllianceVexCond( RtlFigure, VexExpr, Positive )

  rtlfig_list *RtlFigure;
  vexexpr     *VexExpr;
  int          Positive;
{
  vexexpr *VexCond;
  long     Oper;
  char     CharEqual0;
  char     CharEqual1;
  char     CharEqual;

  VexCond    = VexExpr;

  if ( Positive ) 
  {
    CharEqual0 = '0';
    CharEqual1 = '1';
  }
  else
  {
    CharEqual0 = '1';
    CharEqual1 = '0';
  }

  CharEqual = CharEqual1;

  if ( IsVexNodeOper( VexCond ) )
  {
    Oper = GetVexOperValue( VexCond );

    if ( Oper == VEX_NOT )
    {
      CharEqual = CharEqual0;
      VexCond   = dupvexexpr( GetVexOperand( VexCond->OPERAND ) );
    }
    else
    if ( isvexnegativeoper( Oper ) )
    {
      CharEqual = CharEqual0;
      Oper      = getvexnotoper( Oper );
      VexCond   = dupvexexpr( VexCond );

      SetVexOperValue( VexCond, Oper );
    }
  }

  fprintf( VasyFile, "(" );
  VasyDriveVexExprVhdl( VasyFile, RtlFigure, VexCond );
  fprintf( VasyFile, " = '%c')", CharEqual );

  if ( VexCond != VexExpr )
  {
    freevexexpr( VexCond );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyDriveAllianceAssign                   |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveAllianceAssign( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtlasg_list   *RtlAsg;
  rtlbivex_list *ScanBiVex;
  vexexpr       *VexAtom;
  vexexpr       *VexSumAsync;
  vexexpr       *VexData;
  vexexpr       *VexAsync;
  vexexpr       *VexSync;

  for ( RtlAsg  = RtlFigure->ASSIGN;
        RtlAsg != (rtlasg_list *)0;
        RtlAsg  = RtlAsg->NEXT )
  {
    if ( RtlAsg->TYPE == RTL_ASG_COMBINATORIAL )
    {
      fprintf( VasyFile, "  " );
      VasyDriveVexExprVhdl( VasyFile, RtlFigure, RtlAsg->VEX_ATOM );
      fprintf( VasyFile, " <= " );
      VasyDriveVexExprVhdl( VasyFile, RtlFigure, RtlAsg->VEX_DATA );
      fprintf( VasyFile, ";\n" );
    }
    else
    if ( RtlAsg->TYPE == RTL_ASG_MULTIPLEXOR )
    {
      fprintf( VasyFile, "  WITH " );
      VasyDriveVexExprVhdl( VasyFile, RtlFigure, RtlAsg->VEX_DATA );
      fprintf( VasyFile, "  SELECT\n    " );
      VasyDriveVexExprVhdl( VasyFile, RtlFigure, RtlAsg->VEX_ATOM );
      fprintf( VasyFile, " <= " );

      for ( ScanBiVex  = RtlAsg->BIVEX;
            ScanBiVex != (rtlbivex_list *)0;
            ScanBiVex  = ScanBiVex->NEXT )
      {
        VasyDriveVexExprVhdl( VasyFile, RtlFigure, ScanBiVex->VEX_DATA );
        fprintf( VasyFile, " WHEN " );

        if ( ScanBiVex->VEX_COND != (vexexpr *)0 )
        {
          VasyDriveVexExprVhdl( VasyFile, RtlFigure, ScanBiVex->VEX_COND );
        }
        else
        {
          fprintf( VasyFile, " OTHERS " );
        }

        if ( ScanBiVex->NEXT != (rtlbivex_list *)0 )
        {
          fprintf( VasyFile, ",\n     " );
        }
      }

      fprintf( VasyFile, ";\n" );
    }
    else
    if ( RtlAsg->TYPE == RTL_ASG_CONDITIONAL )
    {
      fprintf( VasyFile, "  " );
      VasyDriveVexExprVhdl( VasyFile, RtlFigure, RtlAsg->VEX_ATOM );
      fprintf( VasyFile, " <= " );

      for ( ScanBiVex  = RtlAsg->BIVEX;
            ScanBiVex != (rtlbivex_list *)0;
            ScanBiVex  = ScanBiVex->NEXT )
      {
        VasyDriveVexExprVhdl( VasyFile, RtlFigure, ScanBiVex->VEX_DATA );

        if ( ScanBiVex->NEXT != (rtlbivex_list *)0 )
        {
          fprintf( VasyFile, " WHEN " );
          VasyDriveVexExprVhdl( VasyFile, RtlFigure, ScanBiVex->VEX_COND );
          fprintf( VasyFile, " ELSE\n     " );
        }
      }

      fprintf( VasyFile, ";\n" );
    }
    else
    if ( ( RtlAsg->TYPE == RTL_ASG_REGISTER ) ||
         ( RtlAsg->TYPE == RTL_ASG_TRISTATE ) )
    {
      VexAtom     = RtlAsg->VEX_ATOM;
      VexSumAsync = (vexexpr *)0;

      ScanBiVex = RtlAsg->BIVEX;

      while ( ScanBiVex != (rtlbivex_list *)0 )
      {
        VexData  = ScanBiVex->VEX_DATA;
        VexAsync = (vexexpr *)0;

        if ( ScanBiVex->TYPE == RTL_BIVEX_ASYNC_RESET )
        {
          VexAsync = ScanBiVex->VEX_COND;
          VexData  = createvexatomveclit( VEX_ZERO, VexAtom->WIDTH );
        }
        else
        if ( ScanBiVex->TYPE == RTL_BIVEX_ASYNC_SET )
        {
          VexAsync = ScanBiVex->VEX_COND;
          VexData = createvexatomveclit( VEX_ONE, VexAtom->WIDTH );
        }
        else
        if ( ScanBiVex->TYPE == RTL_BIVEX_ASYNC_WEN )
        {
          VexAsync = ScanBiVex->VEX_COND;
        }
        else break;

        fprintf( VasyFile, "  LABEL%ld : BLOCK ", VasyNumberBlock );
        VasyDriveAllianceVexCond( RtlFigure, VexAsync, 1 );
        fprintf( VasyFile, "\n" );
        fprintf( VasyFile, "  BEGIN\n" );
        fprintf( VasyFile, "    " );
        VasyDriveVexExprVhdl( VasyFile, RtlFigure, VexAtom );
        fprintf( VasyFile, " <= GUARDED " );
        VasyDriveVexExprVhdl( VasyFile, RtlFigure, VexData );
        fprintf( VasyFile, ";\n" );
        fprintf( VasyFile, "  END BLOCK LABEL%ld;\n", VasyNumberBlock );
        VasyNumberBlock++;

        if ( VexAsync != (vexexpr *)0 )
        {
          if ( VexSumAsync == (vexexpr *)0 )
          {
            VexSumAsync = dupvexexpr( VexAsync );
          }
          else
          {
            VexSumAsync = optimvexbinexpr( VEX_OR, 1, VexSumAsync, dupvexexpr( VexAsync ) );
          }
        }

        if ( VexData != ScanBiVex->VEX_DATA )
        {
          freevexexpr( VexData );
        }

        ScanBiVex = ScanBiVex->NEXT;
      }

      if ( ( ScanBiVex    != (rtlbivex_list *)0 ) &&
           ( RtlAsg->TYPE != RTL_ASG_TRISTATE   ) )
      {
        fprintf( VasyFile, "  LABEL%ld : BLOCK ", VasyNumberBlock );
        fprintf( VasyFile, " (" );

        if ( VexSumAsync != (vexexpr *)0 )
        {
          fprintf( VasyFile, " (" );
          VasyDriveAllianceVexCond( RtlFigure, VexSumAsync, 0 );
          fprintf( VasyFile, " AND " );
        }

        if ( ScanBiVex->TYPE == RTL_BIVEX_FALLING_EDGE )
        {
          VasyDriveAllianceVexCond( RtlFigure, ScanBiVex->VEX_COND, 0 );
          fprintf( VasyFile, " AND NOT(" );
          VasyDriveVexExprVhdl( VasyFile, RtlFigure, ScanBiVex->VEX_COND );
          fprintf( VasyFile, "'STABLE)" );
        }
        else
        if ( ScanBiVex->TYPE == RTL_BIVEX_RISING_EDGE  )
        {
          VasyDriveAllianceVexCond( RtlFigure, ScanBiVex->VEX_COND, 1 );
          fprintf( VasyFile, " AND NOT(" );
          VasyDriveVexExprVhdl( VasyFile, RtlFigure, ScanBiVex->VEX_COND );
          fprintf( VasyFile, "'STABLE)" );
        }

        if ( VexSumAsync != (vexexpr *)0 )
        {
          fprintf( VasyFile, " )" );
          freevexexpr( VexSumAsync );
        }

        fprintf( VasyFile, " )\n" );
        fprintf( VasyFile, "  BEGIN\n" );
        fprintf( VasyFile, "    " );
        VasyDriveVexExprVhdl( VasyFile, RtlFigure, VexAtom );
        fprintf( VasyFile, " <= GUARDED " );

        ScanBiVex = ScanBiVex->NEXT;

        while ( ScanBiVex != (rtlbivex_list *)0 )
        {
          VexData = ScanBiVex->VEX_DATA;

          if ( ScanBiVex->TYPE == RTL_BIVEX_SYNC_RESET )
          {
            VexSync = ScanBiVex->VEX_COND;
            VexData  = createvexatomveclit( VEX_ZERO, VexAtom->WIDTH );
          }
          else
          if ( ScanBiVex->TYPE == RTL_BIVEX_SYNC_SET )
          {
            VexSync = ScanBiVex->VEX_COND;
            VexData = createvexatomveclit( VEX_ONE, VexAtom->WIDTH );
          }
          else
          if ( ScanBiVex->TYPE == RTL_BIVEX_SYNC_WEN )
          {
            VexSync = ScanBiVex->VEX_COND;
          }
          else break;

          VasyDriveVexExprVhdl( VasyFile, RtlFigure, VexData );

          if ( VexSync != (vexexpr *)0 )
          {
            fprintf( VasyFile, " WHEN " );
            VasyDriveVexExprVhdl( VasyFile, RtlFigure, VexSync );

            if ( ScanBiVex->NEXT != (rtlbivex_list *)0 )
            {
              fprintf( VasyFile, " ELSE\n     " );
            }
            else
            {
              fprintf( VasyFile, " ELSE " );
              VasyDriveVexExprVhdl( VasyFile, RtlFigure, VexAtom );
            }
          }

          if ( VexData != ScanBiVex->VEX_DATA )
          {
            freevexexpr( VexData );
          }

          ScanBiVex = ScanBiVex->NEXT;
        }

        fprintf( VasyFile, ";\n" );
        fprintf( VasyFile, "  END BLOCK LABEL%ld;\n", VasyNumberBlock );
        VasyNumberBlock++;
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyDriveAllianceFsm                      |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveAllianceFsm( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtlfsm_list      *RtlFsm;
  rtlfsmstate_list *FsmState;
  rtlfsmtrans_list *FsmTrans;
  int               ElseIf;
  rtlfsmasg_list   *FsmAsg;
  chain_list       *Support;
  chain_list       *ScanChain;

  for ( RtlFsm  = RtlFigure->FSM;
        RtlFsm != (rtlfsm_list *)0;
        RtlFsm  = RtlFsm->NEXT )
  {
    VasyError( VASY_NOT_YET_IMPLEMENTED_ERROR, "Driver Alliance: FSM" );
/*
**  Drive The transition process
*/
    fprintf( VasyFile, "  PROCESS ( " );

    /* TO BE DONE
    Support = getvexexprsupport( RtlFsm->VEX_COND );
    Support = unionvexexprsupport( Support, RtlFsm->VEX_WEN );
    */
    Support = (chain_list *)0;

    for ( ScanChain  = Support;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      fprintf( VasyFile, "%s", (char *)ScanChain->DATA );

      if ( ScanChain->NEXT != (chain_list *)0 )
      {
        fprintf( VasyFile, ", " );
      }
    }

    freechain( Support );

    fprintf( VasyFile, " )\n" );
    fprintf( VasyFile, "  BEGIN\n" );

    /* TO BE DONE
    fprintf( VasyFile, "    IF (" );
    VasyDriveAllianceVexCond( RtlFigure, RtlFsm->VEX_COND );
    fprintf( VasyFile, " AND " );
    VasyDriveVexExprVhdl( VasyFile, RtlFigure, RtlFsm->VEX_EDGE );
    fprintf( VasyFile, ")\n" );
    fprintf( VasyFile, "    THEN\n" );

    if ( RtlFsm->VEX_WEN != (vexexpr *)0 )
    {
      fprintf( VasyFile, "\n" );
      fprintf( VasyFile, "      IF " );
      VasyDriveAllianceVexCond( RtlFigure, RtlFsm->VEX_WEN );
      fprintf( VasyFile, "\n" );
      fprintf( VasyFile, "      THEN\n" );
    }
    */

    fprintf( VasyFile, "      CASE ( CS_%s ) IS\n", RtlFsm->NAME );

    for ( FsmState  = RtlFsm->STATE;
          FsmState != (rtlfsmstate_list *)0;
          FsmState  = FsmState->NEXT )
    {
      fprintf( VasyFile, "\n        WHEN %s =>\n\n", FsmState->NAME );

      ElseIf = 0;

      for ( ScanChain  = FsmState->FROM;
            ScanChain != (chain_list *)0;
            ScanChain  = ScanChain->NEXT )
      {
        FsmTrans = GetRtlFsmTrans( ScanChain );
        
        if ( FsmTrans->VEX_COND != (vexexpr *)0 )
        {
          if ( ! ElseIf )
          {
            fprintf( VasyFile, "          IF " );
          }
          else
          if ( ScanChain->NEXT != (chain_list *)0 )
          {
            fprintf( VasyFile, "          ELSIF " );
          }
          else
          {
            fprintf( VasyFile, "          ELSE\n" );
          }

          if ( ScanChain->NEXT != (chain_list *)0 )
          {
            VasyDriveAllianceVexCond( RtlFigure, FsmTrans->VEX_COND );
            fprintf( VasyFile, "\n          THEN\n" );
          }

          ElseIf = 1;
        }

        fprintf( VasyFile, "            CS_%s <= %s;\n", RtlFsm->NAME, FsmTrans->TO->NAME );

        for ( FsmAsg  = FsmTrans->ASSIGN;
              FsmAsg != (rtlfsmasg_list *)0;
              FsmAsg  = FsmAsg->NEXT )
        {
          fprintf( VasyFile, "            " );
          VasyDriveVexExprVhdl( VasyFile, RtlFigure, FsmAsg->VEX_ATOM );
          fprintf( VasyFile, " <= " );
          VasyDriveVexExprVhdl( VasyFile, RtlFigure, FsmAsg->VEX_DATA );
          fprintf( VasyFile, ";\n" );
        }
      }

      if ( ElseIf )
      {
        fprintf( VasyFile, "          END IF;\n" );
      }
    }

    fprintf( VasyFile, "\n      END CASE;\n" );

    /* TO BE DONE
    if ( RtlFsm->VEX_WEN != (vexexpr *)0 )
    {
      fprintf( VasyFile, "      END IF;\n" );
    }
    */

    fprintf( VasyFile, "    END IF;\n" );
    fprintf( VasyFile, "  END PROCESS;\n\n" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyDriveAllianceArchi                    |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveAllianceArchi( RtlFigure, ArchName )

  rtlfig_list *RtlFigure;
  char        *ArchName;
{
  fprintf( VasyFile, "ARCHITECTURE %s OF %s IS\n\n", ArchName,  RtlFigure->NAME );

  VasyDriveAllianceDeclar( RtlFigure, RTL_DECLAR_CONSTANT );
  VasyDriveAllianceDeclar( RtlFigure, RTL_DECLAR_SIGNAL   );
  VasyDriveAllianceFsmDeclar( RtlFigure );
  VasyDriveAllianceComponent( RtlFigure );

  fprintf( VasyFile, "BEGIN\n\n" );

  VasyDriveAllianceAssign( RtlFigure );
  VasyDriveAllianceFsm( RtlFigure );
  VasyDriveAlliancePortMap( RtlFigure );

  fprintf( VasyFile, "END %s;\n", ArchName );
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyDriveAllianceRtlFig                   |
|                                                             |
\------------------------------------------------------------*/

void VasyDriveAllianceRtlFig( RtlFigure, FileName )

  rtlfig_list  *RtlFigure;
  char         *FileName;
{
  rtlfig_list *RtlFigureVbe;
  rtlfig_list *RtlFigureVst;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "--> VasyDriveAllianceRtlFig %s\n", FileName );
  }
/*
** Special treatments for Alliance
*/
  VasyDriveAllianceTreatFigure( RtlFigure, FileName, &RtlFigureVbe, &RtlFigureVst );
/*
** Drive behavioral description
*/
  if ( RtlFigureVbe != (rtlfig_list *)0 )
  {
    if ( IsVasyDebugDriveStdout() )
    {
      VasyFile = stdout;
    }
    else
    {
/*
** First checks to avoid overwriting an existing file !
*/
      if ( ! VasyFlagOver )
      {
        VasyFile = mbkfopen( RtlFigureVbe->NAME, "vbe", "r" );

        if ( VasyFile != (FILE *)0 )
        {
          fclose( VasyFile );
          VasyPrintf( stdout, "ERROR file %s.vbe already exists, remove it first !\n",
                      RtlFigureVbe->NAME );
          autexit( 1 );
        }
      }

      VasyFile = mbkfopen( RtlFigureVbe->NAME, "vbe", "w" );
    }
  
    if ( VasyFile != (FILE *)0 )
    {
      VasyDriveAllianceEntity( RtlFigureVbe );
      VasyDriveAllianceArchi( RtlFigureVbe, "VBE" );
  
      if ( ! IsVasyDebugDriveStdout() )
      {
        fclose( VasyFile );
      }
    }
    else
    {
      VasyPrintf( stdout, "ERROR unable to open file %s.vbe for writting\n", 
                 RtlFigureVbe->NAME );
      autexit( 1 );
    }

    if ( VasyFlagLax )
    {
      if ( IsVasyDebugDriveStdout() )
      {
        VasyLaxFile = stdout;
      }
      else
      {
        if ( ! VasyFlagOver )
        {
          VasyLaxFile = mbkfopen( RtlFigureVbe->NAME, "lax", "r" );
  
          if ( VasyLaxFile != (FILE *)0 )
          {
            fclose( VasyLaxFile );
            VasyPrintf( stdout, "ERROR file %s.lax already exists, remove it first !\n",
                        RtlFigureVbe->NAME );
            autexit( 1 );
          }
        }
  
        VasyLaxFile = mbkfopen( RtlFigureVbe->NAME, "lax", "w" );
      }

      if ( VasyLaxFile == (FILE *)0 ) 
      {
        VasyPrintf( stdout, "ERROR unable to open file %s.lax for writting\n",
                    RtlFigureVbe->NAME );
        autexit( 1 );
      }

      VasyAllianceDriveLaxFile();

      if ( ! IsVasyDebugDriveStdout() )
      {
        fclose( VasyLaxFile );
      }
    }

    if ( VasyFlagBoom )
    {
      if ( IsVasyDebugDriveStdout() )
      {
        VasyBoomFile = stdout;
      }
      else
      {
        if ( ! VasyFlagOver )
        {
          VasyBoomFile = mbkfopen( RtlFigureVbe->NAME, "boom", "r" );
  
          if ( VasyBoomFile != (FILE *)0 )
          {
            fclose( VasyBoomFile );
            VasyPrintf( stdout, "ERROR file %s.boom already exists, remove it first !\n",
                        RtlFigureVbe->NAME );
            autexit( 1 );
          }
        }
  
        VasyBoomFile = mbkfopen( RtlFigureVbe->NAME, "boom", "w" );
      }

      if ( VasyBoomFile == (FILE *)0 ) 
      {
        VasyPrintf( stdout, "ERROR unable to open file %s.boom for writting\n",
                    RtlFigureVbe->NAME );
        autexit( 1 );
      }

      VasyAllianceDriveBoomFile();

      if ( ! IsVasyDebugDriveStdout() )
      {
        fclose( VasyBoomFile );
      }
    }
  }

  if ( RtlFigureVst != (rtlfig_list *)0 )
  {
    if ( IsVasyDebugDriveStdout() )
    {
      VasyFile = stdout;
    }
    else
    {
/*
** First checks to avoid overwriting an existing file !
*/
      if ( ! VasyFlagOver )
      {
        VasyFile = mbkfopen( RtlFigureVst->NAME, "vst", "r" );

        if ( VasyFile != (FILE *)0 )
        {
          fclose( VasyFile );
          VasyPrintf( stdout, "ERROR file %s.vst already exists, remove it first !\n",
                      RtlFigureVst->NAME );
          autexit( 1 );
        }
      }

      VasyFile = mbkfopen( RtlFigureVst->NAME, "vst", "w" );
    }

    if ( VasyFile != (FILE *)0 )
    {
      VasyDriveAllianceEntity( RtlFigureVst );
      VasyDriveAllianceArchi( RtlFigureVst, "VST" );
  
      if ( ! IsVasyDebugDriveStdout() )
      {
        fclose( VasyFile );
      }
    }
    else
    {
      VasyPrintf( stdout, "ERROR unable to open file %s.vst for writting\n", 
                 RtlFigureVst->NAME );
      autexit( 1 );
    }
  }

  VasyAllianceFreeKeepSignal();

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "<-- VasyDriveAllianceRtlFig %s\n\n", FileName );
  }
}
