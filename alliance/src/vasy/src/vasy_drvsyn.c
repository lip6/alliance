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
| File    :                 vasy_drvsyn.c                     |
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
# include "vasy_simprtl.h"
# include "vasy_shared.h"
# include "vasy_drvvex.h"
# include "vasy_drvsyn.h"

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

  static FILE        *VasyFile = (FILE *)0;

  static short VasyConvertDeclType[ VEX_MAX_TYPE ] =
  {
    VEX_TYPE_SEVERITY          , /* SEVERITY          */
    VEX_TYPE_BOOLEAN           , /* BOOLEAN           */
    VEX_TYPE_CHARACTER         , /* CHARACTER         */
    VEX_TYPE_STRING            , /* STRING            */
    VEX_TYPE_BIT               , /* BIT               */
    VEX_TYPE_SIGNED            , /* INTEGER           */
    VEX_TYPE_UNSIGNED          , /* NATURAL           */
    VEX_TYPE_BIT_VECTOR        , /* BIT_VECTOR        */
    VEX_TYPE_STD_ULOGIC        , /* STD_ULOGIC        */
    VEX_TYPE_STD_LOGIC         , /* STD_LOGIC         */
    VEX_TYPE_STD_ULOGIC_VECTOR , /* STD_ULOGIC_VECTOR */
    VEX_TYPE_STD_LOGIC_VECTOR  , /* STD_LOGIC_VECTOR  */
    VEX_TYPE_X01               , /* X01               */
    VEX_TYPE_X01Z              , /* X01Z              */
    VEX_TYPE_UX01              , /* UX01              */
    VEX_TYPE_UX01Z             , /* UX01Z             */
    VEX_TYPE_UNSIGNED          , /* UNSIGNED          */
    VEX_TYPE_SIGNED            , /* SIGNED            */
    VEX_TYPE_BIT               , /* SMALL_INT         */
    VEX_TYPE_BIT               , /* REG_BIT           */
    VEX_TYPE_BIT_VECTOR        , /* REG_VECTOR        */
    VEX_TYPE_STD_LOGIC         , /* MUX_BIT           */
    VEX_TYPE_STD_LOGIC_VECTOR  , /* MUX_VECTOR        */
    VEX_TYPE_STD_LOGIC         , /* WOR_BIT           */
    VEX_TYPE_STD_LOGIC_VECTOR  , /* WOR_VECTOR        */
    VEX_TYPE_BIT_VECTOR          /* ENUMERATE         */
  };

  static short VasyConvertVexType[ VEX_MAX_TYPE ] =
  {
    VEX_TYPE_SEVERITY          , /* SEVERITY          */
    VEX_TYPE_BOOLEAN           , /* BOOLEAN           */
    VEX_TYPE_CHARACTER         , /* CHARACTER         */
    VEX_TYPE_STRING            , /* STRING            */
    VEX_TYPE_BIT               , /* BIT               */
    VEX_TYPE_SIGNED            , /* INTEGER           */
    VEX_TYPE_UNSIGNED          , /* NATURAL           */
    VEX_TYPE_BIT_VECTOR        , /* BIT_VECTOR        */
    VEX_TYPE_STD_LOGIC         , /* STD_ULOGIC        */
    VEX_TYPE_STD_LOGIC         , /* STD_LOGIC         */
    VEX_TYPE_STD_LOGIC_VECTOR  , /* STD_ULOGIC_VECTOR */
    VEX_TYPE_STD_LOGIC_VECTOR  , /* STD_LOGIC_VECTOR  */
    VEX_TYPE_STD_LOGIC         , /* X01               */
    VEX_TYPE_STD_LOGIC         , /* X01Z              */
    VEX_TYPE_STD_LOGIC         , /* UX01              */
    VEX_TYPE_STD_LOGIC         , /* UX01Z             */
    VEX_TYPE_UNSIGNED          , /* UNSIGNED          */
    VEX_TYPE_SIGNED            , /* SIGNED            */
    VEX_TYPE_BIT               , /* SMALL_INT         */
    VEX_TYPE_BIT               , /* REG_BIT           */
    VEX_TYPE_BIT_VECTOR        , /* REG_VECTOR        */
    VEX_TYPE_STD_LOGIC         , /* MUX_BIT           */
    VEX_TYPE_STD_LOGIC_VECTOR  , /* MUX_VECTOR        */
    VEX_TYPE_STD_LOGIC         , /* WOR_BIT           */
    VEX_TYPE_STD_LOGIC_VECTOR  , /* WOR_VECTOR        */
    VEX_TYPE_BIT_VECTOR          /* ENUMERATE         */
  };

  static short VasyConvertVasyType[ VEX_MAX_TYPE ] =
  {
    VASY_TYPE_ERROR    , /* SEVERITY          */
    VASY_TYPE_BOOLEAN  , /* BOOLEAN           */
    VASY_TYPE_ERROR    , /* CHARACTER         */
    VASY_TYPE_ERROR    , /* STRING            */
    VASY_TYPE_BIT      , /* BIT               */
    VASY_TYPE_SIGNED   , /* INTEGER           */
    VASY_TYPE_UNSIGNED , /* NATURAL           */
    VASY_TYPE_BIT      , /* BIT_VECTOR        */
    VASY_TYPE_LOGIC    , /* STD_ULOGIC        */
    VASY_TYPE_LOGIC    , /* STD_LOGIC         */
    VASY_TYPE_LOGIC    , /* STD_ULOGIC_VECTOR */
    VASY_TYPE_LOGIC    , /* STD_LOGIC_VECTOR  */
    VASY_TYPE_LOGIC    , /* X01               */
    VASY_TYPE_LOGIC    , /* X01Z              */
    VASY_TYPE_LOGIC    , /* UX01              */
    VASY_TYPE_LOGIC    , /* UX01Z             */
    VASY_TYPE_UNSIGNED , /* UNSIGNED          */
    VASY_TYPE_SIGNED   , /* SIGNED            */
    VASY_TYPE_BIT      , /* SMALL_INT         */
    VASY_TYPE_BIT      , /* REG_BIT           */
    VASY_TYPE_BIT      , /* REG_VECTOR        */
    VASY_TYPE_LOGIC    , /* MUX_BIT           */
    VASY_TYPE_LOGIC    , /* MUX_VECTOR        */
    VASY_TYPE_LOGIC    , /* WOR_BIT           */
    VASY_TYPE_LOGIC    , /* WOR_VECTOR        */
    VASY_TYPE_BIT        /* ENUMERATE         */
  };

  static rtlfig_list *VasyRtlFigure  = (rtlfig_list *)0;
  static rtlasg_list *VasyRtlAsg     = (rtlasg_list *)0;
  static short        VasyTargetType = 0;

  static vexexpr *VasySynopsysConvertVexConcat();
  static vexexpr *VasySynopsysConvertVexNot();
  static vexexpr *VasySynopsysConvertVexNeg();
  static vexexpr *VasySynopsysConvertVexAbs();
  static vexexpr *VasySynopsysConvertVexBool2();
  static vexexpr *VasySynopsysConvertVexEvent();
  static vexexpr *VasySynopsysConvertVexArith();
  static vexexpr *VasySynopsysConvertVexError();
  static vexexpr *VasySynopsysConvertVexEqual();
  static vexexpr *VasySynopsysConvertVexComp();

  static vexexpr *(*VasyConvertFuncArray[ VEX_MAX_OPERATOR ])() =
  {
     VasySynopsysConvertVexConcat,   /* VEX_CONCAT    */
     VasySynopsysConvertVexNot,      /* VEX_NOT       */
     VasySynopsysConvertVexNeg,      /* VEX_NEG       */
     VasySynopsysConvertVexEvent,    /* VEX_EVENT     */
     VasySynopsysConvertVexBool2,    /* VEX_OR        */
     VasySynopsysConvertVexBool2,    /* VEX_AND       */
     VasySynopsysConvertVexBool2,    /* VEX_XOR       */
     VasySynopsysConvertVexBool2,    /* VEX_NOR       */
     VasySynopsysConvertVexBool2,    /* VEX_NAND      */
     VasySynopsysConvertVexBool2,    /* VEX_NXOR      */
     VasySynopsysConvertVexEqual,    /* VEX_EQ        */
     VasySynopsysConvertVexEqual,    /* VEX_NE        */
     VasySynopsysConvertVexComp,     /* VEX_LT        */
     VasySynopsysConvertVexComp,     /* VEX_LE        */
     VasySynopsysConvertVexComp,     /* VEX_GT        */
     VasySynopsysConvertVexComp,     /* VEX_GE        */
     VasySynopsysConvertVexArith,    /* VEX_ADD       */
     VasySynopsysConvertVexArith,    /* VEX_SUB       */
     VasySynopsysConvertVexArith,    /* VEX_MUL       */
     VasySynopsysConvertVexArith,    /* VEX_DIV       */
     VasySynopsysConvertVexArith,    /* VEX_EXP       */
     VasySynopsysConvertVexArith,    /* VEX_MOD       */
     VasySynopsysConvertVexArith,    /* VEX_REM       */
     VasySynopsysConvertVexError,    /* VEX_TO        */
     VasySynopsysConvertVexError,    /* VEX_DOWNTO    */
     VasySynopsysConvertVexError,    /* VEX_INDEX     */
     VasySynopsysConvertVexError,    /* VEX_LEFT      */
     VasySynopsysConvertVexError,    /* VEX_RIGHT     */
     VasySynopsysConvertVexError,    /* VEX_LOW       */
     VasySynopsysConvertVexError,    /* VEX_HIGH      */
     VasySynopsysConvertVexError,    /* VEX_LENGTH    */
     VasySynopsysConvertVexError,    /* VEX_RANGE     */
     VasySynopsysConvertVexError,    /* VEX_REV_RANGE */
     VasySynopsysConvertVexError,    /* VEX_DRIVER    */
     VasySynopsysConvertVexError,    /* VEX_IFT       */
     VasySynopsysConvertVexError,    /* VEX_ARRAY     */
     VasySynopsysConvertVexError,    /* VEX_INDEX_N   */
     VasySynopsysConvertVexError,    /* VEX_OTHERS    */
     VasySynopsysConvertVexError,    /* VEX_NUM_BIT   */
     VasySynopsysConvertVexAbs,      /* VEX_ABS       */
     VasySynopsysConvertVexError,    /* AF            */
     VasySynopsysConvertVexError,    /* AG            */
     VasySynopsysConvertVexError,    /* AX            */
     VasySynopsysConvertVexError,    /* AU            */
     VasySynopsysConvertVexError,    /* EF            */
     VasySynopsysConvertVexError,    /* EG            */
     VasySynopsysConvertVexError,    /* EX            */
     VasySynopsysConvertVexError,    /* EU            */
     VasySynopsysConvertVexError,    /* EQUIV         */
     VasySynopsysConvertVexError     /* IMPLY         */           

  };

  static short VasyConvertBoolType[ VASY_MAX_TYPE ][ VASY_MAX_TYPE ] =
  {
      /* BOOL          BIT                LOGIC             SIGNED          UNSIGNED      */
    {VASY_TYPE_BOOLEAN,VASY_TYPE_BOOLEAN ,VASY_TYPE_BOOLEAN ,VASY_TYPE_BOOLEAN,VASY_TYPE_BOOLEAN },
    {VASY_TYPE_BOOLEAN,VASY_TYPE_BIT     ,VASY_TYPE_LOGIC   ,VASY_TYPE_SIGNED ,VASY_TYPE_UNSIGNED},
    {VASY_TYPE_BOOLEAN,VASY_TYPE_LOGIC   ,VASY_TYPE_LOGIC   ,VASY_TYPE_SIGNED ,VASY_TYPE_UNSIGNED},
    {VASY_TYPE_BOOLEAN,VASY_TYPE_SIGNED  ,VASY_TYPE_SIGNED  ,VASY_TYPE_SIGNED ,VASY_TYPE_SIGNED  },
    {VASY_TYPE_BOOLEAN,VASY_TYPE_UNSIGNED,VASY_TYPE_UNSIGNED,VASY_TYPE_SIGNED ,VASY_TYPE_UNSIGNED}
  };

  static short VasyConvertArithType[ VASY_MAX_TYPE ][ VASY_MAX_TYPE ] =
  {
      /* BOOL          BIT                LOGIC             SIGNED          UNSIGNED      */
    {VASY_TYPE_ERROR,VASY_TYPE_ERROR   ,VASY_TYPE_ERROR   ,VASY_TYPE_ERROR  ,VASY_TYPE_ERROR  },
    {VASY_TYPE_ERROR,VASY_TYPE_UNSIGNED,VASY_TYPE_UNSIGNED,VASY_TYPE_SIGNED,VASY_TYPE_UNSIGNED},
    {VASY_TYPE_ERROR,VASY_TYPE_UNSIGNED,VASY_TYPE_UNSIGNED,VASY_TYPE_SIGNED,VASY_TYPE_UNSIGNED},
    {VASY_TYPE_ERROR,VASY_TYPE_SIGNED  ,VASY_TYPE_SIGNED  ,VASY_TYPE_SIGNED,VASY_TYPE_SIGNED  },
    {VASY_TYPE_ERROR,VASY_TYPE_UNSIGNED,VASY_TYPE_UNSIGNED,VASY_TYPE_SIGNED,VASY_TYPE_UNSIGNED}
  };

  static short VasyConvertPropagateVecType[ VASY_MAX_TYPE ] =
  {
    VEX_TYPE_BOOLEAN,
    VEX_TYPE_BIT_VECTOR, 
    VEX_TYPE_STD_LOGIC_VECTOR,
    VEX_TYPE_SIGNED,
    VEX_TYPE_UNSIGNED
  };

  static short VasyConvertPropagateBitType[ VASY_MAX_TYPE ] =
  {
    VEX_TYPE_BOOLEAN,
    VEX_TYPE_BIT, 
    VEX_TYPE_STD_LOGIC,
    VEX_TYPE_SIGNED,
    VEX_TYPE_UNSIGNED
  };

  static short VasyConvertConcatType[ VASY_MAX_TYPE ][ VASY_MAX_TYPE ] =
  {
      /* BOOL        BIT             LOGIC           SIGNED          UNSIGNED      */
 {VASY_TYPE_ERROR,VASY_TYPE_ERROR,VASY_TYPE_ERROR,VASY_TYPE_ERROR,VASY_TYPE_ERROR},
 {VASY_TYPE_ERROR,VASY_TYPE_BIT  ,VASY_TYPE_LOGIC,VASY_TYPE_LOGIC,VASY_TYPE_LOGIC},
 {VASY_TYPE_ERROR,VASY_TYPE_LOGIC,VASY_TYPE_LOGIC,VASY_TYPE_LOGIC,VASY_TYPE_LOGIC},
 {VASY_TYPE_ERROR,VASY_TYPE_LOGIC,VASY_TYPE_LOGIC,VASY_TYPE_LOGIC,VASY_TYPE_LOGIC},
 {VASY_TYPE_ERROR,VASY_TYPE_LOGIC,VASY_TYPE_LOGIC,VASY_TYPE_LOGIC,VASY_TYPE_LOGIC}
  };

  static long        VasyNumberDef    = 0;

  static ptype_list *VasyListBoolean      = (ptype_list *)0;
  static ptype_list *VasyBooleanLastMatch = (ptype_list *)0;
  static ptype_list *VasyListConvert      = (ptype_list *)0;
  static ptype_list *VasyConvertLastMatch = (ptype_list *)0;

  static char        VasyBuffer[ 512 ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 VasyDriveSynopsysConvertDeclType            |
|                                                             |
\------------------------------------------------------------*/

static int VasyDriveSynopsysConvertDeclType( Base )

  int Base;
{
  int Type;

  Type = VasyConvertDeclType[ Base ];

  if ( VasyFlagStdLogic )
  {
    if ( Type == VEX_TYPE_BIT        ) Type = VEX_TYPE_STD_LOGIC;
    else
    if ( Type == VEX_TYPE_BIT_VECTOR ) Type = VEX_TYPE_STD_LOGIC_VECTOR;
  }

  return( Type );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySynopsysPropagateVexType            |
|                                                             |
\------------------------------------------------------------*/

static void VasySynopsysPropagateVexType( VexExpr, VasyType )

  vexexpr *VexExpr;
  int      VasyType;
{
  chain_list *ScanChain;
  vexexpr    *Operand;
  int         Type;

  if ( VexExpr == (vexexpr *)0 ) return;

  if ( IsVexNodeAtom( VexExpr ) )
  {
    if ( IsVexAtomLiteral( VexExpr ) )
    {
      if ( VexExpr->WIDTH > 1 ) Type = VasyConvertPropagateVecType[ VasyType ];
      else                      Type = VasyConvertPropagateBitType[ VasyType ];

      SetVexVhdlType( VexExpr, Type );
    }
  }
  else
  {
    for ( ScanChain  = VexExpr->OPERAND;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      Operand = GetVexOperand( ScanChain );
      VasySynopsysPropagateVexType( Operand, VasyType );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySynopsysConvertBooleanToBit         |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasySynopsysConvertBooleanToBit( VexExpr )

  vexexpr *VexExpr;
{
  vexexpr      *VexAtom;
  vexexpr      *VexAtomOne;
  vexexpr      *VexAtomZero;
  rtldecl_list *RtlDeclar;
  rtlasg_list  *RtlAssign;
  char          Buffer[ 64 ];
  ptype_list   *ScanList;
  short         Type;

  VexAtom = (vexexpr *)0;

  if ( ( VasyBooleanLastMatch != (ptype_list *)0                          ) &&
       ( isvexequalexpr( VexExpr, (vexexpr *)VasyBooleanLastMatch->TYPE ) ) )
  {
    freevexexpr( VexExpr );
    VexAtom = (vexexpr *)VasyBooleanLastMatch->DATA;

    ScanList = VasyBooleanLastMatch;
  }
  else
  for ( ScanList  = VasyListBoolean;
        ScanList != (ptype_list *)0;
        ScanList  = ScanList->NEXT )
  {
    if ( isvexequalexpr( VexExpr, (vexexpr *)ScanList->TYPE ) )
    {
      freevexexpr( VexExpr );
      VexAtom = (vexexpr *)ScanList->DATA; break;
    }
  }

  if ( ScanList == (ptype_list *)0 )
  {
    if ( VasyFlagStdLogic ) Type = VEX_TYPE_STD_LOGIC;
    else                    Type = VEX_TYPE_BIT;

    sprintf( Buffer, "rtl_%s_%ld", VEX_TYPE_NAME[ Type ], VasyNumberDef++ );
    VexAtom = createvexatombit( Buffer );

    SetVexVhdlType( VexAtom, Type );
  
    RtlDeclar = addrtldecl( VasyRtlFigure, VexAtom, RTL_DECLAR_SIGNAL );
    RtlDeclar->BASE = Type;
    RtlDeclar->KIND = RTL_KIND_NONE;
    RtlDeclar->DIR  = RTL_DIR_INOUT;
  
    SetVasyRtlDeclarRead( RtlDeclar );
    SetVasyRtlDeclarAsg( RtlDeclar );
  
    RtlAssign = addrtlasgafter( VasyRtlFigure, VasyRtlAsg,
                                dupvexexpr( VexAtom ), RTL_ASG_CONDITIONAL );
  
    VexAtomOne  = createvexatomlit( VEX_ATOM_ONE  );
    VexAtomZero = createvexatomlit( VEX_ATOM_ZERO );
  
    addrtlasgbivex( VasyRtlFigure, RtlAssign, VexExpr     , VexAtomOne , RTL_BIVEX_CONDITIONAL );
    addrtlasgbivex( VasyRtlFigure, RtlAssign, (vexexpr *)0, VexAtomZero, RTL_BIVEX_CONDITIONAL );

    VasyListBoolean = addptype( VasyListBoolean, (long)VexExpr, (void *)VexAtom );
    ScanList = VasyListBoolean;
  }

  VasyBooleanLastMatch = ScanList;

  return( dupvexexpr( VexAtom ) );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySynopsysPostTreatVex                |
|                                                             |
\------------------------------------------------------------*/

static short VasySynopsysPostTreatVex( VexExpr )

  vexexpr *VexExpr;
{
  vexexpr    *Operand;
  chain_list *ScanOper;
  char       *Value;
  short       Type;
  short       FuncId;
  short       Literal;
  int         Index;

  Literal = 0;

  if ( VexExpr != (vexexpr *)0 )
  {
    Type = GetVexVhdlType( VexExpr );

    if ( IsVexNodeAtom( VexExpr ) )
    {
      if ( IsVexAtomLiteral( VexExpr ) )
      {
        if ( ( VexExpr->WIDTH == 1                   ) &&
             ( ( Type == VEX_TYPE_UNSIGNED         ) ||
               ( Type == VEX_TYPE_SIGNED           ) ||
               ( Type == VEX_TYPE_STD_LOGIC_VECTOR ) ) )
        {
          Value = GetVexAtomValue( VexExpr );

          VasyBuffer[ 0 ] = '\"';
          VasyBuffer[ 1 ] = Value[ 1 ];
          VasyBuffer[ 2 ] = '\"';
          VasyBuffer[ 3 ] = '\0';

          Value = namealloc( VasyBuffer );
          SetVexAtomValue( VexExpr, Value );
        }
        else
        if ( ( Type == VEX_TYPE_BIT        ) ||
             ( Type == VEX_TYPE_BIT_VECTOR ) )
        {
          Value = GetVexAtomValue( VexExpr );

          for ( Index = 0; Value[ Index ] != '\0'; Index++ )
          {
            if ( Value[ Index ] == '1' )     VasyBuffer[ Index ] = '1';
            else
            if ( ( Value[ Index ] != '\'' ) &&
                 ( Value[ Index ] != '"'  ) ) VasyBuffer[ Index ] = '0';
            else                              VasyBuffer[ Index ] = Value[ Index ];
          }

          VasyBuffer[ Index ] = '\0';
          Value = namealloc( VasyBuffer );
          SetVexAtomValue( VexExpr, Value );
        }

        Literal = 1;
      }
    }
    else
    {
      Literal = 1;

      for ( ScanOper  = VexExpr->OPERAND;
            ScanOper != (chain_list *)0;
            ScanOper  = ScanOper->NEXT )
      {
        Operand  = GetVexOperand( ScanOper );
        Literal &= VasySynopsysPostTreatVex( Operand );
      }

      if ( ( Literal                  ) &&
           ( IsVexNodeFunc( VexExpr ) ) )
      {
        Value  = GetVexFuncValue( VexExpr );
        FuncId = getvexstdfuncid( Value );

        if ( FuncId == VEX_STD_SIGNED )
        {
          Value = namealloc( "signed'" );
        }
        else
        if ( FuncId == VEX_STD_UNSIGNED )
        {
          Value = namealloc( "unsigned'" );
        }
        else
        if ( FuncId == VEX_STD_STD_LOGIC_VECTOR )
        {
          Value = namealloc( "std_logic_vector'" );
        }

        SetVexFuncValue( VexExpr, Value );
        Literal = 0;
      }
    }
  }

  return( Literal );
}


/*------------------------------------------------------------\
|                                                             |
|                     VasySynopsysAddRtlAssign                |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasySynopsysAddRtlAssign( VexExpr, Type )

    vexexpr *VexExpr;
    int      Type;
{
  vexexpr      *VexAtom;
  rtldecl_list *RtlDeclar;
  rtlasg_list  *RtlAssign;
  char          Buffer[ 64 ];
  ptype_list   *ScanList;

  SetVexVhdlType( VexExpr, Type );

  VexAtom = (vexexpr *)0;

  if ( ( VasyConvertLastMatch != (ptype_list *)0                          ) &&
       ( isvexequalexpr( VexExpr, (vexexpr *)VasyConvertLastMatch->TYPE ) ) )
  {
    freevexexpr( VexExpr );
    VexAtom = (vexexpr *)VasyConvertLastMatch->DATA;

    ScanList = VasyConvertLastMatch;
  }
  else
  for ( ScanList  = VasyListConvert;
        ScanList != (ptype_list *)0;
        ScanList  = ScanList->NEXT )
  {
    if ( isvexequalexpr( VexExpr, (vexexpr *)ScanList->TYPE ) )
    {
      freevexexpr( VexExpr );
      VexAtom = (vexexpr *)ScanList->DATA; break;
    }
  }

  if ( ScanList == (ptype_list *)0 )
  {
    sprintf( Buffer, "rtl_%s_%ld", VEX_TYPE_NAME[ Type ], VasyNumberDef++ );
  
    if ( isvextypevector( Type ) )
    {
      VexAtom = createvexatomvec( Buffer, VexExpr->WIDTH - 1, 0 );
    }
    else
    {
      VexAtom = createvexatombit( Buffer );
    }
  
    if ( Type == VEX_TYPE_SIGNED ) SetVexNodeSigned( VexAtom );
    SetVexVhdlType( VexAtom, Type );
  
    RtlDeclar = addrtldecl( VasyRtlFigure, VexAtom, RTL_DECLAR_SIGNAL );
    RtlDeclar->BASE = Type;
    RtlDeclar->KIND = RTL_KIND_NONE;
    RtlDeclar->DIR  = RTL_DIR_INOUT;
  
    SetVasyRtlDeclarRead( RtlDeclar );
    SetVasyRtlDeclarAsg( RtlDeclar );

    VasySynopsysPostTreatVex( VexExpr );
  
    RtlAssign = addrtlasg( VasyRtlFigure, dupvexexpr( VexAtom ), RTL_ASG_COMBINATORIAL );
    RtlAssign->VEX_DATA = dupvexexpr( VexExpr );

    VasyListConvert = addptype( VasyListConvert, (long)VexExpr, (void *)VexAtom );
    ScanList = VasyListConvert;
  }

  VasyConvertLastMatch = ScanList;

  return( dupvexexpr( VexAtom ) );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySynopsysConvertVexType              |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasySynopsysConvertVexType( VexExpr, ConvertType )

  vexexpr *VexExpr;
  int      ConvertType;
{
  vexexpr *VexFunc;
  int      Type;

  if ( VexExpr == (vexexpr *)0 ) return( (vexexpr *)0 );

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "--> VasySynopsysConvertVexType %d\n", ConvertType );
    viewvexexprboundln( VexExpr );
  }

  Type = GetVexVhdlType( VexExpr );

  if ( ConvertType == VASY_TYPE_ERROR )
  {
    VasyError( VASY_ERROR_CONVERT_WRONG_TYPE, VEX_TYPE_NAME[ Type ] );
  }

  if ( ConvertType == VASY_TYPE_BOOLEAN )
  {
    if ( Type != VEX_TYPE_BOOLEAN )
    {
      VexExpr = createvexbinexpr( VEX_EQ, 1, VexExpr, createvexatombit( VEX_ATOM_ONE ) );
      Type    = VEX_TYPE_BOOLEAN;

      SetVexVhdlType( VexExpr, Type );
    }
  }
  else
  {
    if ( Type == VEX_TYPE_BOOLEAN )
    {
      VexExpr = VasySynopsysConvertBooleanToBit( VexExpr );
      Type = GetVexVhdlType( VexExpr );
    }

    if ( ConvertType == VASY_TYPE_BIT )
    {
      if ( ( Type != VEX_TYPE_BIT        ) &&
           ( Type != VEX_TYPE_BIT_VECTOR ) )
      {
        if ( ( Type == VEX_TYPE_SIGNED   ) ||
             ( Type == VEX_TYPE_UNSIGNED ) )
        {
          VexFunc = createvexfunc( VEX_STD_FUNC_NAME[ VEX_STD_STD_LOGIC_VECTOR ], VexExpr->WIDTH );
          addvexhexpr( VexFunc, VexExpr );
  
          VexExpr = VasySynopsysAddRtlAssign( VexFunc, VEX_TYPE_STD_LOGIC_VECTOR );
          Type    = VEX_TYPE_STD_LOGIC_VECTOR;
        }
  
        if ( Type == VEX_TYPE_STD_LOGIC )
        {
          VexFunc = createvexfunc( VEX_STD_FUNC_NAME[ VEX_STD_TO_BIT ], 1 );
          addvexhexpr( VexFunc, VexExpr );
  
          VexExpr = VasySynopsysAddRtlAssign( VexFunc, VEX_TYPE_BIT );
        }
        else
        if ( Type == VEX_TYPE_STD_LOGIC_VECTOR )
        {
          VexFunc = createvexfunc( VEX_STD_FUNC_NAME[ VEX_STD_TO_BITVECTOR ], VexExpr->WIDTH );
          addvexhexpr( VexFunc, VexExpr );
  
          VexExpr = VasySynopsysAddRtlAssign( VexFunc, VEX_TYPE_BIT_VECTOR );
        }
        else
        if ( Type == VEX_MAX_TYPE )
        {
          VasySynopsysPropagateVexType( VexExpr, VASY_TYPE_BIT );
        }
        else
        {
          VasyError( VASY_ERROR_CONVERT_WRONG_TYPE, VEX_TYPE_NAME[ Type ] );
        }
      }
    }
    else
    if ( ConvertType == VASY_TYPE_LOGIC )
    {
      if ( ( Type != VEX_TYPE_STD_LOGIC        ) &&
           ( Type != VEX_TYPE_STD_LOGIC_VECTOR ) )
      {
        if ( ( Type == VEX_TYPE_SIGNED   ) ||
             ( Type == VEX_TYPE_UNSIGNED ) )
        {
          VexFunc = createvexfunc( VEX_STD_FUNC_NAME[ VEX_STD_STD_LOGIC_VECTOR ], VexExpr->WIDTH );
          addvexhexpr( VexFunc, VexExpr );
  
          VexExpr = VasySynopsysAddRtlAssign( VexFunc, VEX_TYPE_STD_LOGIC_VECTOR );
        }
        else
        if ( Type == VEX_TYPE_BIT )
        {
          VexFunc = createvexfunc( VEX_STD_FUNC_NAME[ VEX_STD_TO_STDULOGIC ], 1 );
          addvexhexpr( VexFunc, VexExpr );
  
          VexExpr = VasySynopsysAddRtlAssign( VexFunc, VEX_TYPE_STD_LOGIC );
        }
        else
        if ( Type == VEX_TYPE_BIT_VECTOR )
        {
          VexFunc = createvexfunc( VEX_STD_FUNC_NAME[ VEX_STD_TO_STDLOGICVECTOR ], VexExpr->WIDTH );
          addvexhexpr( VexFunc, VexExpr );
  
          VexExpr = VasySynopsysAddRtlAssign( VexFunc, VEX_TYPE_STD_LOGIC_VECTOR );
        }
        else
        if ( Type == VEX_MAX_TYPE )
        {
          VasySynopsysPropagateVexType( VexExpr, VASY_TYPE_LOGIC );
        }
        else
        {
          VasyError( VASY_ERROR_CONVERT_WRONG_TYPE, VEX_TYPE_NAME[ Type ] );
        }
      }
    }
    else
    if ( ConvertType == VASY_TYPE_SIGNED )
    {
      if ( Type != VEX_TYPE_SIGNED )
      {
        if ( Type == VEX_TYPE_BIT )
        {
          VexFunc = createvexfunc( VEX_STD_FUNC_NAME[ VEX_STD_TO_STDULOGIC ], 1 );
          addvexhexpr( VexFunc, VexExpr );
  
          VexExpr = VasySynopsysAddRtlAssign( VexFunc, VEX_TYPE_STD_LOGIC );
          Type = VEX_TYPE_STD_LOGIC;
        }
        else
        if ( Type == VEX_TYPE_BIT_VECTOR )
        {
          VexFunc = createvexfunc( VEX_STD_FUNC_NAME[ VEX_STD_TO_STDLOGICVECTOR ], VexExpr->WIDTH );
          addvexhexpr( VexFunc, VexExpr );
  
          VexExpr = VasySynopsysAddRtlAssign( VexFunc, VEX_TYPE_STD_LOGIC_VECTOR );
          Type = VEX_TYPE_STD_LOGIC_VECTOR;
        }
  
        if ( Type == VEX_TYPE_STD_LOGIC )
        {
          VexExpr = VasySynopsysAddRtlAssign( VexExpr, VEX_TYPE_SIGNED );
          Type    = VEX_TYPE_SIGNED;
        }
        else
        if ( ( Type == VEX_TYPE_STD_LOGIC_VECTOR ) ||
             ( Type == VEX_TYPE_UNSIGNED         ) ||
             ( Type == VEX_MAX_TYPE              ) )
        {
          VexFunc = createvexfunc( VEX_STD_FUNC_NAME[ VEX_STD_SIGNED ], VexExpr->WIDTH );
          addvexhexpr( VexFunc, VexExpr );
  
          if ( Type == VEX_MAX_TYPE )
          {
            VasySynopsysPropagateVexType( VexFunc, VASY_TYPE_SIGNED );
          }
  
          VexExpr = VasySynopsysAddRtlAssign( VexFunc, VEX_TYPE_SIGNED );
        }
        else
        {
          VasyError( VASY_ERROR_CONVERT_WRONG_TYPE, VEX_TYPE_NAME[ Type ] );
        }
      }
    }
    else
    if ( ConvertType == VASY_TYPE_UNSIGNED ) 
    {
      if ( Type != VEX_TYPE_UNSIGNED )
      {
        if ( Type == VEX_TYPE_BIT )
        {
          VexFunc = createvexfunc( VEX_STD_FUNC_NAME[ VEX_STD_TO_STDULOGIC ], 1 );
          addvexhexpr( VexFunc, VexExpr );
  
          VexExpr = VasySynopsysAddRtlAssign( VexFunc, VEX_TYPE_STD_LOGIC );
          Type    = VEX_TYPE_STD_LOGIC;
        }
        else
        if ( Type == VEX_TYPE_BIT_VECTOR )
        {
          VexFunc = createvexfunc( VEX_STD_FUNC_NAME[ VEX_STD_TO_STDLOGICVECTOR ], VexExpr->WIDTH );
          addvexhexpr( VexFunc, VexExpr );
  
          VexExpr = VasySynopsysAddRtlAssign( VexFunc, VEX_TYPE_STD_LOGIC_VECTOR );
          Type    = VEX_TYPE_STD_LOGIC_VECTOR;
        }
  
        if ( Type == VEX_TYPE_STD_LOGIC )
        {
          VexExpr = VasySynopsysAddRtlAssign( VexExpr, VEX_TYPE_UNSIGNED );
          Type    = VEX_TYPE_UNSIGNED;
        }
        else
        if ( ( Type == VEX_TYPE_STD_LOGIC_VECTOR ) ||
             ( Type == VEX_TYPE_SIGNED           ) ||
             ( Type == VEX_MAX_TYPE              ) )
        {
          VexFunc = createvexfunc( VEX_STD_FUNC_NAME[ VEX_STD_UNSIGNED ], VexExpr->WIDTH );
          addvexhexpr( VexFunc, VexExpr );
  
          if ( Type == VEX_MAX_TYPE )
          {
            VasySynopsysPropagateVexType( VexFunc, VASY_TYPE_UNSIGNED );
          }
  
          VexExpr = VasySynopsysAddRtlAssign( VexFunc, VEX_TYPE_UNSIGNED );
        }
        else
        {
          VasyError( VASY_ERROR_CONVERT_WRONG_TYPE, VEX_TYPE_NAME[ Type ] );
        }
      }
    }
  }

  if ( IsVasyDebugLevel2() )
  {
    Type = GetVexVhdlType( VexExpr );
    viewvexexprboundln( VexExpr );
    VasyPrintf( stdout, "<-- VasySynopsysConvertVexType %s\n", VEX_TYPE_NAME[ Type ] );
  }

  return( VexExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySynopsysConvertVexError             |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasySynopsysConvertVexError( VexExpr )

  vexexpr *VexExpr;
{
  long Oper;

  Oper = GetVexOperValue( VexExpr );
  VasyError( VASY_NOT_YET_IMPLEMENTED_ERROR, getvexoperuppername( Oper ) );

  return( VexExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySynopsysConvertVexConcat            |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasySynopsysConvertVexConcat( VexExpr )

  vexexpr *VexExpr;
{
  chain_list *ScanChain;
  vexexpr    *Operand;
  short       Unknown;
  short       SameType;
  short       FirstType;
  short       ScanType;
  short       VasyType;
  short       Type;
  short       NewType;

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "--> VasySynopsysConvertVexConcat " );
    viewvexexprboundln( VexExpr ); 
  }
/*
** Verify if all operands have the same type 
*/
  Unknown   = 0;
  SameType  = 1;
  FirstType = 1;
  ScanType  = VASY_TYPE_ERROR;

  for ( ScanChain  = VexExpr->OPERAND;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    Operand  = GetVexOperand( ScanChain );
    Type     = GetVexVhdlType( Operand );

    if ( Type == VEX_MAX_TYPE ) Unknown = 1;
    else
    {
      VasyType = VasyConvertVasyType[ Type ];
      VasyType = VasyConvertConcatType[ VasyType ][ VasyType ];

      if ( VasyType == VASY_TYPE_ERROR )
      {
        VasyError( VASY_ERROR_CONVERT_WRONG_TYPE, VEX_TYPE_NAME[ Type ] );
      }

      if ( FirstType )
      {
        ScanType  = VasyType;
        FirstType = 0;
      }
      else
      if ( ScanType != VasyType )
      {
        SameType = 0;
        ScanType = VasyConvertConcatType[ VasyType ][ ScanType ];
      }
    }
  }

  if ( ScanType == VASY_TYPE_BIT ) NewType = VEX_TYPE_BIT_VECTOR;
  else                             NewType = VEX_TYPE_STD_LOGIC_VECTOR;

  SetVexVhdlType( VexExpr, NewType );

  if ( Unknown || ( ! SameType ) )
  {
    for ( ScanChain  = VexExpr->OPERAND;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      Operand = GetVexOperand( ScanChain );
      Type    = GetVexVhdlType( Operand );

      if ( ! SameType )
      {
        VasyType = VasyConvertVasyType[ Type ];
        VasyType = VasyConvertConcatType[ VasyType ][ VasyType ];

        if ( VasyType != ScanType )
        {
          Operand  = VasySynopsysConvertVexType( Operand, ScanType );
          SetVexOperand( ScanChain, Operand );
        }
      }
      else
      if ( Type == VEX_MAX_TYPE )
      {
        VasySynopsysPropagateVexType( Operand, ScanType );
      }
    }
  }

  VexExpr = VasySynopsysAddRtlAssign( VexExpr, NewType );

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "<-- VasySynopsysConvertVexConcat " );
    viewvexexprboundln( VexExpr ); 
  }

  return( VexExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySynopsysConvertVexNot               |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasySynopsysConvertVexNot( VexExpr )

  vexexpr *VexExpr;
{
  vexexpr *Operand;
  int      Type;

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "--> VasySynopsysConvertVexNot\n" );
    viewvexexprboundln( VexExpr ); 
  }

  Operand = GetVexOperand( VexExpr->OPERAND );
  Type    = GetVexVhdlType( Operand );
  SetVexVhdlType( VexExpr, Type );

  if ( IsVasyDebugLevel1() )
  {
    viewvexexprboundln( VexExpr ); 
    VasyPrintf( stdout, "<-- VasySynopsysConvertVexNot\n" );
  }

  return( VexExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySynopsysConvertVexEvent             |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasySynopsysConvertVexEvent( VexExpr )

  vexexpr *VexExpr;
{
  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "--> VasySynopsysConvertVexEvent\n" );
    viewvexexprboundln( VexExpr ); 
  }

  SetVexVhdlType( VexExpr, VEX_TYPE_BOOLEAN );

  if ( IsVasyDebugLevel1() )
  {
    viewvexexprboundln( VexExpr ); 
    VasyPrintf( stdout, "<-- VasySynopsysConvertVexEvent\n" );
  }

  return( VexExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySynopsysConvertVexBool2             |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasySynopsysConvertVexBool2( VexExpr )

  vexexpr *VexExpr;
{
  vexexpr *Operand1;
  vexexpr *Operand2;
  int      Type1;
  int      Type2;
  int      Type;
  short    VasyType;
  short    VasyType1;
  short    VasyType2;

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "--> VasySynopsysConvertVexBool2\n" );
    viewvexexprboundln( VexExpr ); 
  }

  Operand1 = GetVexOperand( VexExpr->OPERAND       );
  Type1    = GetVexVhdlType( Operand1 );
  Operand2 = GetVexOperand( VexExpr->OPERAND->NEXT );
  Type2    = GetVexVhdlType( Operand2 );

  if ( ( Type1 == VEX_MAX_TYPE ) ||
       ( Type2 == VEX_MAX_TYPE ) )
  {
    if ( Type1 == VEX_MAX_TYPE ) Type = Type2;
    else                         Type = Type1;
  }
  else
  {
    VasyType1 = VasyConvertVasyType[ Type1 ];
    VasyType2 = VasyConvertVasyType[ Type2 ];

    if ( ( VasyType1 == VASY_TYPE_BOOLEAN ) ||
         ( VasyType2 == VASY_TYPE_BOOLEAN ) )
    {
      if ( VasyTargetType != VASY_TYPE_BOOLEAN )
      {
        if ( VasyType1 == VASY_TYPE_BOOLEAN )
        {
          VasyType1 = VASY_TYPE_BIT;
          Operand1  = VasySynopsysConvertVexType( Operand1, VasyType1 );
        }

        if ( VasyType2 == VASY_TYPE_BOOLEAN )
        {
          VasyType2 = VASY_TYPE_BIT;
          Operand2  = VasySynopsysConvertVexType( Operand2, VasyType2 );
        }
      }
    }

    VasyType  = VasyConvertBoolType[ VasyType1 ][ VasyType2 ];

    Operand1 = VasySynopsysConvertVexType( Operand1, VasyType );
    Operand2 = VasySynopsysConvertVexType( Operand2, VasyType );

    SetVexOperand( VexExpr->OPERAND      , Operand1 );
    SetVexOperand( VexExpr->OPERAND->NEXT, Operand2 );

    Type = GetVexVhdlType( Operand1 );
  }

  SetVexVhdlType( VexExpr, Type );

  if ( IsVasyDebugLevel1() )
  {
    viewvexexprboundln( VexExpr ); 
    VasyPrintf( stdout, "<-- VasySynopsysConvertVexBool2\n" );
  }

  return( VexExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySynopsysConvertVexEqual             |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasySynopsysConvertVexEqual( VexExpr )

  vexexpr *VexExpr;
{
  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "--> VasySynopsysConvertVexEqual\n" );
    viewvexexprboundln( VexExpr ); 
  }

  VexExpr = VasySynopsysConvertVexBool2( VexExpr );
  SetVexVhdlType( VexExpr, VEX_TYPE_BOOLEAN );

  if ( IsVasyDebugLevel1() )
  {
    viewvexexprboundln( VexExpr ); 
    VasyPrintf( stdout, "<-- VasySynopsysConvertVexEqual\n" );
  }

  return( VexExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySynopsysConvertVexAbs               |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasySynopsysConvertVexAbs( VexExpr )

  vexexpr *VexExpr;
{
  vexexpr *Operand1;
  int      Type1;
  int      Type;
  short    VasyType;
  short    VasyType1;

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "--> VasySynopsysConvertVexAbs\n" );
    viewvexexprboundln( VexExpr ); 
  }

  Operand1 = GetVexOperand( VexExpr->OPERAND );
  Type1    = GetVexVhdlType( Operand1 );

  if ( Type1 == VEX_MAX_TYPE )
  {
    Type1 = VEX_TYPE_SIGNED;
    SetVexVhdlType( Operand1, Type1 );
  }

  VasyType1 = VasyConvertVasyType[ Type1 ];

  if ( VasyType1 == VASY_TYPE_BOOLEAN ) VasyType = VASY_TYPE_ERROR;
  else                                  VasyType = VASY_TYPE_SIGNED;

  Operand1  = VasySynopsysConvertVexType( Operand1, VasyType );
  SetVexOperand( VexExpr->OPERAND, Operand1 );

  Type = GetVexVhdlType( Operand1 );
  SetVexVhdlType( VexExpr, Type );

  if ( IsVasyDebugLevel1() )
  {
    viewvexexprboundln( VexExpr ); 
    VasyPrintf( stdout, "<-- VasySynopsysConvertVexAbs\n" );
  }

  return( VexExpr );
}


/*------------------------------------------------------------\
|                                                             |
|                     VasySynopsysConvertVexNeg               |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasySynopsysConvertVexNeg( VexExpr )

  vexexpr *VexExpr;
{
  vexexpr *Operand1;
  int      Type1;
  int      Type;
  short    VasyType;
  short    VasyType1;

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "--> VasySynopsysConvertVexNeg\n" );
    viewvexexprboundln( VexExpr ); 
  }

  Operand1 = GetVexOperand( VexExpr->OPERAND );
  Type1    = GetVexVhdlType( Operand1 );

  if ( Type1 == VEX_MAX_TYPE )
  {
    Type1 = VEX_TYPE_UNSIGNED;
    SetVexVhdlType( Operand1, Type1 );
  }

  VasyType1 = VasyConvertVasyType[ Type1 ];

  if ( VasyType1 == VASY_TYPE_BOOLEAN ) VasyType = VASY_TYPE_ERROR;
  else                                  VasyType = VASY_TYPE_SIGNED;

  Operand1  = VasySynopsysConvertVexType( Operand1, VasyType );
  SetVexOperand( VexExpr->OPERAND, Operand1 );

  Type = GetVexVhdlType( Operand1 );
  SetVexVhdlType( VexExpr, Type );

  if ( IsVasyDebugLevel1() )
  {
    viewvexexprboundln( VexExpr ); 
    VasyPrintf( stdout, "<-- VasySynopsysConvertVexNeg\n" );
  }

  return( VexExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySynopsysConvertVexArith             |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasySynopsysConvertVexArith( VexExpr )

  vexexpr *VexExpr;
{
  vexexpr *Operand1;
  vexexpr *Operand2;
  int      Type1;
  int      Type2;
  int      Type;
  short    VasyType;
  short    VasyType1;
  short    VasyType2;

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "--> VasySynopsysConvertVexArith\n" );
    viewvexexprboundln( VexExpr ); 
  }

  Operand1 = GetVexOperand( VexExpr->OPERAND       );
  Type1    = GetVexVhdlType( Operand1 );
  Operand2 = GetVexOperand( VexExpr->OPERAND->NEXT );
  Type2    = GetVexVhdlType( Operand2 );

  if ( Type1 == VEX_MAX_TYPE )
  {
    Operand1 = VasySynopsysConvertVexType( Operand1, VASY_TYPE_UNSIGNED );
    Type1    = GetVexVhdlType( Operand1 );
  }

  if ( Type2 == VEX_MAX_TYPE )
  {
    Operand2 = VasySynopsysConvertVexType( Operand2, VASY_TYPE_UNSIGNED );
    Type2    = GetVexVhdlType( Operand2 );
  }
 
  VasyType1 = VasyConvertVasyType[ Type1 ];
  VasyType2 = VasyConvertVasyType[ Type2 ];
  VasyType  = VasyConvertArithType[ VasyType1 ][ VasyType2 ];

  Operand1 = VasySynopsysConvertVexType( Operand1, VasyType );
  Operand2 = VasySynopsysConvertVexType( Operand2, VasyType );

  SetVexOperand( VexExpr->OPERAND      , Operand1 );
  SetVexOperand( VexExpr->OPERAND->NEXT, Operand2 );

  Type = GetVexVhdlType( Operand1 );
  SetVexVhdlType( VexExpr, Type );

  if ( IsVasyDebugLevel1() )
  {
    viewvexexprboundln( VexExpr ); 
    VasyPrintf( stdout, "<-- VasySynopsysConvertVexArith\n" );
  }

  return( VexExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySynopsysConvertVexComp              |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasySynopsysConvertVexComp( VexExpr )

  vexexpr *VexExpr;
{
  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "--> VasySynopsysConvertVexComp\n" );
    viewvexexprboundln( VexExpr ); 
  }

  VexExpr = VasySynopsysConvertVexArith( VexExpr );
  SetVexVhdlType( VexExpr, VEX_TYPE_BOOLEAN );

  if ( IsVasyDebugLevel1() )
  {
    viewvexexprboundln( VexExpr ); 
    VasyPrintf( stdout, "<-- VasySynopsysConvertVexComp\n" );
  }

  return( VexExpr );
}


/*------------------------------------------------------------\
|                                                             |
|                     VasySynopsysTreatVex                    |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasySynopsysTreatVex( VexExpr )

  vexexpr *VexExpr;
{
  rtldecl_list *RtlDeclar;
  char         *AtomValue;
  chain_list   *ScanChain;
  vexexpr      *Operand;
  int           Type;
  long          Oper;

  if ( VexExpr != (vexexpr *)0 )
  {
    if ( IsVexNodeAtom( VexExpr ) )
    {
      if ( IsVexAtomLiteral( VexExpr ) )
      {
        if ( IsVexNodeSigned( VexExpr ) ) Type = VEX_TYPE_SIGNED;
        else                              Type = VEX_MAX_TYPE;
      }
      else
      {
        AtomValue = GetVexAtomValue( VexExpr ); 
        RtlDeclar = searchrtldecl( VasyRtlFigure, AtomValue );
        Type      = VasyConvertVexType[ RtlDeclar->BASE ];

        if ( isvextypevector( Type ) )
        {
          if ( VexExpr->WIDTH == 1 )
          {
            if ( Type == VEX_TYPE_BIT_VECTOR       ) Type = VEX_TYPE_BIT;
            else
            if ( Type == VEX_TYPE_STD_LOGIC_VECTOR ) Type = VEX_TYPE_STD_LOGIC;
          }
        }
      }
  
      SetVexVhdlType( VexExpr, Type );
    }
    else
    {
      for ( ScanChain  = VexExpr->OPERAND;
            ScanChain != (chain_list *)0;
            ScanChain  = ScanChain->NEXT )
      {
        Operand = GetVexOperand( ScanChain );
        Operand = VasySynopsysTreatVex( Operand );
        SetVexOperand( ScanChain, Operand );
      }
  
      if ( IsVexNodeOper( VexExpr ) )
      {
        Oper    = GetVexOperValue( VexExpr );
        VexExpr = (*VasyConvertFuncArray[ Oper ])( VexExpr );
      }
    }
  }

  return( VexExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySynopsysTreatVexType                |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasySynopsysTreatVexType( VexExpr, VasyType )

  vexexpr *VexExpr;
  short    VasyType;
{
  VasyTargetType = VasyType;

  VexExpr = unflatvexexpr( VexExpr );
  VexExpr = VasySynopsysTreatVex( VexExpr ); 

  return( VexExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                   VasySynopsysTreatAsg                      |
|                                                             |
\------------------------------------------------------------*/

static void VasySynopsysTreatAsg( RtlAsg )

  rtlasg_list *RtlAsg;
{
  rtldecl_list  *RtlDeclar;
  rtlbivex_list *RtlBiVex;
  vexexpr       *VexAtom;
  vexexpr       *VexExpr;
  char          *AtomValue;
  int            Type;
  short          VasyType;
  short          RtlType;

  VasyRtlAsg = RtlAsg;

  VexAtom   = RtlAsg->VEX_ATOM;
  AtomValue = GetVexAtomValue( VexAtom ); 
  RtlDeclar = searchrtldecl( VasyRtlFigure, AtomValue );
  Type      = VasyConvertVexType[ RtlDeclar->BASE ];
  VasyType  = VasyConvertVasyType[ Type ];

  VexExpr = VasySynopsysTreatVexType( RtlAsg->VEX_DATA, VasyType );
  VexExpr = VasySynopsysConvertVexType( VexExpr, VasyType );
  RtlAsg->VEX_DATA = VexExpr;

  VasySynopsysPostTreatVex( VexExpr );

  for ( RtlBiVex  = RtlAsg->BIVEX;
        RtlBiVex != (rtlbivex_list *)0;
        RtlBiVex  = RtlBiVex->NEXT )
  {
    RtlType = RtlBiVex->TYPE;

    if ( ( RtlType != RTL_BIVEX_RISING_EDGE  ) &&
         ( RtlType != RTL_BIVEX_FALLING_EDGE ) )
    {
      VexExpr = VasySynopsysTreatVexType( RtlBiVex->VEX_COND, VASY_TYPE_BOOLEAN );
      VexExpr = VasySynopsysConvertVexType( VexExpr, VASY_TYPE_BOOLEAN );
      RtlBiVex->VEX_COND = VexExpr; 

      VasySynopsysPostTreatVex( VexExpr );
    }

    VexExpr = VasySynopsysTreatVexType( RtlBiVex->VEX_DATA, VasyType );
    VexExpr = VasySynopsysConvertVexType( VexExpr, VasyType );
    RtlBiVex->VEX_DATA = VexExpr; 

    VasySynopsysPostTreatVex( VexExpr );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   VasySynopsysTreatMap                      |
|                                                             |
\------------------------------------------------------------*/

static void VasySynopsysTreatMap( RtlInst, RtlMap )

  rtlins_list *RtlInst;
  rtlmap_list *RtlMap;
{
  rtlport_list *RtlPort;
  rtldecl_list *RtlDeclar;
  rtlasg_list  *RtlAssign;
  vexexpr      *VexFormal;
  vexexpr      *VexActual;
  vexexpr      *VexAtom;
  char         *AtomValue;
  char          Buffer[ 64 ];
  int           Type;
  short         VasyType;

  VasyRtlAsg = (rtlasg_list *)0;

  VexFormal = RtlMap->VEX_FORMAL;

  if ( ! IsVexNodeAtom( VexFormal ) )
  {
    viewvexexprln( VexFormal );
    autexit(1);
  }

  AtomValue = GetVexAtomValue( VexFormal ); 
  RtlPort   = searchrtlmodport( VasyRtlFigure, RtlInst->MODEL, AtomValue );
  Type      = VasyConvertVexType[ RtlPort->BASE ];
  VasyType  = VasyConvertVasyType[ Type ];

  SetVexVhdlType( VexFormal, Type );

  VexActual = RtlMap->VEX_ACTUAL;
  VasyType  = VasyConvertVasyType[ Type ];
  VexActual = VasySynopsysTreatVexType( VexActual, VasyType );
  VexActual = VasySynopsysConvertVexType( VexActual, VasyType );

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
  
    if ( Type == VEX_TYPE_SIGNED ) SetVexNodeSigned( VexAtom );
    SetVexVhdlType( VexAtom, Type );
  
    RtlDeclar = addrtldecl( VasyRtlFigure, dupvexexpr( VexAtom ), RTL_DECLAR_SIGNAL );
    RtlDeclar->BASE = Type;
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

/*------------------------------------------------------------\
|                                                             |
|                   VasyDriveSynopsysTreatFigure              |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveSynopsysTreatFigure( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtlmod_list   *RtlModel;
  rtlins_list   *RtlInst;
  rtlport_list  *RtlPort;
  rtlmap_list   *RtlMap;
  rtldecl_list  *RtlDeclar;
  rtlasg_list   *RtlAsg;
  int            Index;
  int            Type;

  for ( Index = 0; Index < RTL_MAX_DECLAR_TYPE; Index++ )
  {
    for ( RtlDeclar  = RtlFigure->DECLAR[ Index ];
          RtlDeclar != (rtldecl_list *)0;
          RtlDeclar  = RtlDeclar->NEXT )
    {
      Type = VasyDriveSynopsysConvertDeclType( RtlDeclar->BASE );

      RtlDeclar->BASE = Type;
      RtlDeclar->KIND = RTL_KIND_NONE;

      SetVexVhdlType( RtlDeclar->VEX_ATOM, Type );
    }
  }

  for ( RtlModel  = RtlFigure->MODEL;
        RtlModel != (rtlmod_list *)0;
        RtlModel  = RtlModel->NEXT )
  {
    for ( RtlPort  = RtlModel->PORT;
          RtlPort != (rtlport_list *)0;
          RtlPort  = RtlPort->NEXT )
    {
      Type = VasyDriveSynopsysConvertDeclType( RtlPort->BASE );

      RtlPort->BASE = Type;

      SetVexVhdlType( RtlPort->VEX_ATOM, Type );
    }
  }

  VasyConvertLastMatch = (ptype_list *)0;
  VasyBooleanLastMatch = (ptype_list *)0;

  VasyRtlFigure = RtlFigure;

  for ( RtlAsg  = RtlFigure->ASSIGN;
        RtlAsg != (rtlasg_list *)0;
        RtlAsg  = RtlAsg->NEXT )
  {
    VasySynopsysTreatAsg( RtlAsg );
  }

  for ( RtlInst  = RtlFigure->INSTANCE;
        RtlInst != (rtlins_list *)0;
        RtlInst  = RtlInst->NEXT )
  {
    for ( RtlMap  = RtlInst->MAP;
          RtlMap != (rtlmap_list *)0;
          RtlMap  = RtlMap->NEXT )
    {
      VasySynopsysTreatMap( RtlInst, RtlMap );
    }
  }

  freeptype( VasyListConvert );
  freeptype( VasyListBoolean );

  VasyListBoolean = (ptype_list *)0;
  VasyListConvert = (ptype_list *)0;
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyDriveSynopsysPort                     |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveSynopsysPort( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtldecl_list *RtlDeclar;
  vexexpr      *VexPort;
  char         *PortName;
  char         *PortDir;
  char         *PortBase;
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
      PortBase = VEX_TYPE_NAME[ RtlDeclar->BASE ];

      fprintf( VasyFile, "  %s\t: %s %s", PortName, PortDir, PortBase );
  
      if ( isvextypevector( RtlDeclar->BASE ) )
      {
        if ( IsVexAtomUp( VexPort ) ) PortVecDir = "TO";
        else                          PortVecDir = "DOWNTO";
  
        fprintf( VasyFile, "(%d %s %d)", VexPort->LEFT, PortVecDir, VexPort->RIGHT );
      }
  
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
|                   VasyDriveSynopsysDeclar                   |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveSynopsysDeclar( RtlFigure, Type )

  rtlfig_list *RtlFigure;
  int          Type;
{
  rtldecl_list *RtlDeclar;
  vexexpr      *VexDeclar;
  vexexpr      *VexInit;
  char         *AtomValue;
  char         *DeclarName;
  char         *DeclarDir;
  char         *DeclarBase;
  char         *DeclarVecDir;

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

    if ( ( Type    == RTL_DECLAR_CONSTANT   ) ||
         ( ( VexInit != (vexexpr *)0      ) &&
           ( VasyFlagInitial              ) ) )
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
|                 VasyDriveSynopsysFsmDeclar                  |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveSynopsysFsmDeclar( RtlFigure )

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
|                 VasyDriveSynopsysComponent                  |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveSynopsysComponent( RtlFigure )

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
    
        if ( RtlPort->NEXT != (rtlport_list *)0 )
        {
          fprintf( VasyFile, ";\n" );
        }
    
        RtlPort = RtlPort->NEXT;
      }

      fprintf( VasyFile, "\n   );\n" );
    }

    fprintf( VasyFile, "  END COMPONENT;\n\n" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyDriveSynopsysEntity                   |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveSynopsysEntity( RtlFigure )

  rtlfig_list *RtlFigure;
{
  fprintf( VasyFile, "--\n" );
  fprintf( VasyFile, "-- Generated by VASY\n" );
  fprintf( VasyFile, "--\n" );
  fprintf( VasyFile, "LIBRARY IEEE;\n" );
  fprintf( VasyFile, "USE IEEE.std_logic_1164.ALL;\n" );
  fprintf( VasyFile, "USE IEEE.numeric_std.ALL;\n\n" );

  fprintf( VasyFile, "ENTITY %s IS\n", RtlFigure->NAME );

  VasyDriveSynopsysPort( RtlFigure );

  fprintf( VasyFile, "END %s;\n\n", RtlFigure->NAME );
}

/*------------------------------------------------------------\
|                                                             |
|                 VasyDriveSynopsysPortMap                    |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveSynopsysPortMap( RtlFigure )

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
|                   VasyDriveSynopsysAssign                   |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveSynopsysAssign( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtldecl_list  *RtlDeclar;
  rtlsym        *RtlSymbol;
  rtlasg_list   *RtlAsg;
  rtlbivex_list *ScanBiVex;
  char          *AtomValue;
  char          *AtomName;
  chain_list    *AllName;
  chain_list    *ScanChain;
  vexexpr       *VexDeclar;
  vexexpr       *VexInit;
  int            Type;
  int            ElseIf;
  int            EndIf;
  int            Edge;
  int            RtlType;
  int            ScanPos;
  char           Literal;
  int            First;

  for ( Type = 0; Type < RTL_MAX_DECLAR_TYPE; Type++ )
  {
    for ( RtlDeclar  = RtlFigure->DECLAR[ Type ];
          RtlDeclar != (rtldecl_list *)0;
          RtlDeclar  = RtlDeclar->NEXT )
    {
      if ( ( RtlDeclar->DIR == RTL_DIR_IN      ) ||
           ( ! IsVasyRtlDeclarUsed( RtlDeclar ) ) ) continue;

      VexDeclar  = RtlDeclar->VEX_ATOM;
      VexInit    = RtlDeclar->VEX_INIT;

      if ( VexInit == (vexexpr *)0 ) continue;

      AtomName   = GetVexAtomValue( VexDeclar );
      AtomValue  = GetVexAtomValue( VexInit   );

      for ( ScanPos = 0; ScanPos < VexDeclar->WIDTH; ScanPos++ )
      {
        RtlSymbol = &RtlDeclar->DECL_SYM[ ScanPos ];

        if ( ( IsVasyRtlDeclarRead( RtlSymbol  ) ) &&
             ( ! IsVasyRtlDeclarAsg( RtlSymbol ) ) )
        {
          Literal = AtomValue[ ScanPos + 1 ];

          fprintf( VasyFile, "  " );

          if ( RtlSymbol->INDEX != -1 )
          {
            fprintf( VasyFile, "  %s(%d) <= '%c';\n", AtomName, RtlSymbol->INDEX, Literal );
          }
          else
          {
            fprintf( VasyFile, "  %s <= '%c';\n", AtomName, Literal );
          }
        }
      }
    }
  }

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
    if ( ( RtlAsg->TYPE == RTL_ASG_REGISTER  ) ||
         ( RtlAsg->TYPE == RTL_ASG_TRISTATE  ) ||
         ( RtlAsg->TYPE == RTL_ASG_PULL_UP   ) ||
         ( RtlAsg->TYPE == RTL_ASG_PULL_DOWN ) )
    {
      AllName = (chain_list *)0;

      for ( ScanBiVex  = RtlAsg->BIVEX;
            ScanBiVex != (rtlbivex_list *)0;
            ScanBiVex  = ScanBiVex->NEXT )
      {
        RtlType = ScanBiVex->TYPE;

        if ( ( RtlType == RTL_BIVEX_ASYNC_RESET  ) ||
             ( RtlType == RTL_BIVEX_ASYNC_SET    ) ||
             ( RtlType == RTL_BIVEX_ASYNC_WEN    ) ||
             ( RtlType == RTL_BIVEX_RISING_EDGE  ) ||
             ( RtlType == RTL_BIVEX_FALLING_EDGE ) )
        {
          AllName = unionvexexprallname( AllName, ScanBiVex->VEX_COND );
          AllName = unionvexexprallname( AllName, ScanBiVex->VEX_DATA );
        }
      }

      fprintf( VasyFile, "  PROCESS ( " );

      First = 1;

      for ( ScanChain  = AllName;
            ScanChain != (chain_list *)0;
            ScanChain  = ScanChain->NEXT )
      {
        RtlDeclar = searchrtldecl( RtlFigure, (char *)ScanChain->DATA );

        if ( RtlDeclar->TYPE == RTL_DECLAR_CONSTANT ) continue;

        if ( ! First ) fprintf( VasyFile, ", " );
        First = 0;

        fprintf( VasyFile, "%s", (char *)ScanChain->DATA );
      }

      freechain( AllName );

      fprintf( VasyFile, " )\n" );
      fprintf( VasyFile, "  BEGIN\n" );

      ElseIf = 0;
      EndIf  = 0;
      Edge   = 0;

      for ( ScanBiVex  = RtlAsg->BIVEX;
            ScanBiVex != (rtlbivex_list *)0;
            ScanBiVex  = ScanBiVex->NEXT )
      {
        RtlType = ScanBiVex->TYPE;

        if ( ScanBiVex->VEX_COND != (vexexpr *)0 )
        {
          if ( Edge ) fprintf( VasyFile, "\n" );

          Edge = 0;

          if ( ElseIf == 1 ) fprintf( VasyFile, "    ELSIF " );
          else               fprintf( VasyFile, "    IF "    );

          if ( RtlType == RTL_BIVEX_RISING_EDGE )
          {
            fprintf( VasyFile, " ((" );
            VasyDriveVexExprVhdl( VasyFile, RtlFigure, ScanBiVex->VEX_COND );
            fprintf( VasyFile, " = '1') AND " );
            VasyDriveVexExprVhdl( VasyFile, RtlFigure, ScanBiVex->VEX_COND );
            fprintf( VasyFile, "'EVENT)" );

            ElseIf = 0;
            Edge   = 1;
          }
          else
          if ( RtlType == RTL_BIVEX_FALLING_EDGE )
          {
            fprintf( VasyFile, " ((" );
            VasyDriveVexExprVhdl( VasyFile, RtlFigure, ScanBiVex->VEX_COND );
            fprintf( VasyFile, " = '0') AND " );
            VasyDriveVexExprVhdl( VasyFile, RtlFigure, ScanBiVex->VEX_COND );
            fprintf( VasyFile, "'EVENT)" );

            ElseIf = 0;
            Edge   = 1;
          }
          else
          {
            VasyDriveVexExprVhdl( VasyFile, RtlFigure, ScanBiVex->VEX_COND );

            ElseIf = 1;
          }

          fprintf( VasyFile, "\n" );
          fprintf( VasyFile, "    THEN " );
        }

        if ( ( RtlType != RTL_BIVEX_FALLING_EDGE ) &&
             ( RtlType != RTL_BIVEX_RISING_EDGE  ) )
        {
          VasyDriveVexExprVhdl( VasyFile, RtlFigure, RtlAsg->VEX_ATOM );
        }

        if ( ( RtlType == RTL_BIVEX_ASYNC_SET ) ||
             ( RtlType == RTL_BIVEX_SYNC_SET  ) )
        {
          if ( RtlAsg->VEX_ATOM->WIDTH > 1 ) fprintf( VasyFile, " <= (OTHERS => '1');\n" );
          else                               fprintf( VasyFile, " <= '1';\n" );
        }
        else
        if ( ( RtlType == RTL_BIVEX_ASYNC_RESET ) ||
             ( RtlType == RTL_BIVEX_SYNC_RESET  ) )
        {
          if ( RtlAsg->VEX_ATOM->WIDTH > 1 ) fprintf( VasyFile, " <= (OTHERS => '0');\n" );
          else                               fprintf( VasyFile, " <= '0';\n" );
        }
        else
        if ( ( RtlType == RTL_BIVEX_ASYNC_WEN ) ||
             ( RtlType == RTL_BIVEX_SYNC_WEN  ) )
        {
          fprintf( VasyFile, " <= " );
          VasyDriveVexExprVhdl( VasyFile, RtlFigure, ScanBiVex->VEX_DATA );
          fprintf( VasyFile, ";\n" );
        }

        if ( ( RtlType == RTL_BIVEX_SYNC_WEN   ) ||
             ( RtlType == RTL_BIVEX_SYNC_RESET ) ||
             ( RtlType == RTL_BIVEX_SYNC_SET   ) )
        {
          if ( ScanBiVex->VEX_COND != (vexexpr *)0 )
          {
            EndIf = 1;
          }
        }
      }

      if ( ( RtlAsg->TYPE == RTL_ASG_TRISTATE  ) ||
           ( RtlAsg->TYPE == RTL_ASG_PULL_UP   ) ||
           ( RtlAsg->TYPE == RTL_ASG_PULL_DOWN ) )
      {
        fprintf( VasyFile, "    ELSE " );
        VasyDriveVexExprVhdl( VasyFile, RtlFigure, RtlAsg->VEX_ATOM );

        if ( RtlAsg->TYPE == RTL_ASG_PULL_UP   ) Literal = 'H';
        else
        if ( RtlAsg->TYPE == RTL_ASG_PULL_DOWN ) Literal = 'L';
        else
                                                 Literal = 'Z';

        if ( RtlAsg->VEX_ATOM->WIDTH > 1 )
        {
          fprintf( VasyFile, " <= (OTHERS => '%c');\n", Literal );
        }
        else
        {
          fprintf( VasyFile, " <= '%c';\n", Literal );
        }
      }

      if ( EndIf )
      {
        fprintf( VasyFile, "    END IF;\n" );
      }

      fprintf( VasyFile, "    END IF;\n" );
      fprintf( VasyFile, "  END PROCESS;\n" );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyDriveSynopsysFsm                      |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveSynopsysFsm( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtlfsm_list      *RtlFsm;
  rtlfsmstate_list *FsmState;
  rtlfsmtrans_list *FsmTrans;
  int               ElseIf;
  rtlfsmasg_list   *FsmAsg;
  rtlbivex_list    *ScanBiVex;
  int               RtlType;
  chain_list       *AllName;
  chain_list       *ScanChain;

  for ( RtlFsm  = RtlFigure->FSM;
        RtlFsm != (rtlfsm_list *)0;
        RtlFsm  = RtlFsm->NEXT )
  {
/*
**  Drive The transition process
*/
    ScanBiVex = RtlFsm->BIVEX;
    RtlType   = ScanBiVex->TYPE;
    AllName   = getvexexprallname( ScanBiVex->VEX_COND );

    fprintf( VasyFile, "  PROCESS ( " );

    for ( ScanChain  = AllName;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      fprintf( VasyFile, "%s", (char *)ScanChain->DATA );

      if ( ScanChain->NEXT != (chain_list *)0 )
      {
        fprintf( VasyFile, ", " );
      }
    }

    freechain( AllName );

    fprintf( VasyFile, " )\n" );
    fprintf( VasyFile, "  BEGIN\n" );
    fprintf( VasyFile, "    IF ("     );

    if ( RtlType == RTL_BIVEX_RISING_EDGE )
    {
      VasyDriveVexExprVhdl( VasyFile, RtlFigure, ScanBiVex->VEX_COND );
      fprintf( VasyFile, " AND " );
      VasyDriveVexExprVhdl( VasyFile, RtlFigure, ScanBiVex->VEX_COND );
      fprintf( VasyFile, "'EVENT)" );
    }
    else
    if ( RtlType == RTL_BIVEX_FALLING_EDGE )
    {
      VasyDriveVexExprVhdl( VasyFile, RtlFigure, ScanBiVex->VEX_COND );
      fprintf( VasyFile, " AND " );
      VasyDriveVexExprVhdl( VasyFile, RtlFigure, ScanBiVex->VEX_COND );
      fprintf( VasyFile, "'EVENT)" );
    }

    fprintf( VasyFile, ")\n" );
    fprintf( VasyFile, "    THEN\n" );

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
            VasyDriveVexExprVhdl( VasyFile, RtlFigure, FsmTrans->VEX_COND );
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
    fprintf( VasyFile, "    END IF;\n" );
    fprintf( VasyFile, "  END PROCESS;\n\n" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyDriveSynopsysArchi                    |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveSynopsysArchi( RtlFigure )

  rtlfig_list *RtlFigure;
{
  fprintf( VasyFile, "ARCHITECTURE RTL OF %s IS\n", RtlFigure->NAME );

  VasyDriveSynopsysDeclar( RtlFigure, RTL_DECLAR_CONSTANT );
  VasyDriveSynopsysDeclar( RtlFigure, RTL_DECLAR_SIGNAL   );
  VasyDriveSynopsysFsmDeclar( RtlFigure );
  VasyDriveSynopsysComponent( RtlFigure );

  fprintf( VasyFile, "BEGIN\n" );

  VasyDriveSynopsysAssign( RtlFigure );
  VasyDriveSynopsysFsm( RtlFigure );
  VasyDriveSynopsysPortMap( RtlFigure );

  fprintf( VasyFile, "END RTL;\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyDriveSynopsysRtlFig                   |
|                                                             |
\------------------------------------------------------------*/

void VasyDriveSynopsysRtlFig( RtlFigure, FileName )

  rtlfig_list  *RtlFigure;
  char         *FileName;
{
  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "--> VasyDriveSynopsysRtlFig %s\n", FileName );
  }

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
      VasyFile = mbkfopen( FileName, "vhd", "r" );

      if ( VasyFile != (FILE *)0 )
      {
        fclose( VasyFile );
        VasyPrintf( stdout, "ERROR file %s.vhd already exists, remove it first !\n", 
                    FileName );
        autexit( 1 );
      }
    }

    VasyFile = mbkfopen( FileName, "vhd", "w" );
  }

  if ( VasyFile != (FILE *)0 )
  {
/*
** Special treatments for Synopsys
*/
    VasyDriveSynopsysTreatFigure( RtlFigure );
    VasyDriveSynopsysEntity( RtlFigure );
    VasyDriveSynopsysArchi( RtlFigure );

    if ( ! IsVasyDebugDriveStdout() )
    {
      fclose( VasyFile );
    }
  }
  else
  {
    VasyPrintf( stdout, "ERROR unable to open file %s.vhd for writting\n", FileName );
    autexit( 1 );
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "<-- VasyDriveSynopsysRtlFig %s\n\n", FileName );
  }
}
