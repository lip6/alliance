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
| Tool    :                     VPD                           |
|                                                             |
| File    :                  vpd_parse.c                      |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   01.01.95                        |
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
# include "vpn.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "vpd_error.h"
# include "vpd_parse.h"

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

  static FILE  *VpnFile;
  static char   VpnBuffer[ 512 ];
  static long   VpnLine;

  static char  *VpnKeyFigure     = (char *)0;
  static char  *VpnKeyDeclar     = (char *)0;
  static char  *VpnKeyAttr       = (char *)0;
  static char  *VpnKeyAttrEnt    =  (char *)0;
  static char  *VpnKeyInit       = (char *)0;
  static char  *VpnKeySymbol     = (char *)0;
  static char  *VpnKeyProcess    = (char *)0;
  static char  *VpnKeyFunction   = (char *)0;
  static char  *VpnKeyPlace      = (char *)0;
  static char  *VpnKeyLink       = (char *)0;
  static char  *VpnKeyLine       = (char *)0;
  static char  *VpnKeyTrans      = (char *)0;
  static char  *VpnKeyGuard      = (char *)0;
  static char  *VpnKeyWait       = (char *)0;
  static char  *VpnKeyAct        = (char *)0;
  static char  *VpnKeyActExpr    = (char *)0;
  static char  *VpnKeyActAtom    = (char *)0;
  static char  *VpnKeyRet        = (char *)0;
  static char  *VpnKeyArcOut     = (char *)0;
  static char  *VpnKeyArcIn      = (char *)0;
  static char  *VpnKeyFirst      = (char *)0;
  static char  *VpnKeyElabo      = (char *)0;
  static char  *VpnKeyInstance   = (char *)0;
  static char  *VpnKeyInsMod     = (char *)0;
  static char  *VpnKeyMap        = (char *)0;
  static char  *VpnKeyMapFor     = (char *)0;
  static char  *VpnKeyMapAct     = (char *)0;
  static char  *VpnKeyModel      = (char *)0;
  static char  *VpnKeyPort       = (char *)0;
  static char  *VpnKeyGenMod     = (char *)0;
  static char  *VpnKeyGenMap     = (char *)0;
  static char  *VpnKeyGenExpr    = (char *)0;

  static char  *VpnScanString = (char *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         VpnInitKeyword                      |
|                                                             |
\------------------------------------------------------------*/

static void VpnInitKeyword()
{
  if ( VpnKeyFigure == (char *)0 )
  {
    VpnKeyFigure     = namealloc( "FIGURE"   );
    VpnKeyDeclar     = namealloc( "DECLAR"   );
    VpnKeyAttr       = namealloc( "ATTR"     );
    VpnKeyAttrEnt    = namealloc( "ATTR_ENT" );
    VpnKeyInit       = namealloc( "INIT"     );
    VpnKeySymbol     = namealloc( "SYMBOL"   );
    VpnKeyProcess    = namealloc( "PROCESS"  );
    VpnKeyFunction   = namealloc( "FUNCTION" );
    VpnKeyPlace      = namealloc( "PLACE"    );
    VpnKeyLink       = namealloc( "LINK"     );
    VpnKeyLine       = namealloc( "LINE"     );
    VpnKeyTrans      = namealloc( "TRANS"    );
    VpnKeyGuard      = namealloc( "GUARD"    );
    VpnKeyWait       = namealloc( "WAIT"     );
    VpnKeyAct        = namealloc( "ACT"      );
    VpnKeyActExpr    = namealloc( "ACT_EXPR" );
    VpnKeyActAtom    = namealloc( "ACT_ATOM" );
    VpnKeyRet        = namealloc( "RET"      );
    VpnKeyArcOut     = namealloc( "ARC_OUT"  );
    VpnKeyArcIn      = namealloc( "ARC_IN"   );
    VpnKeyFirst      = namealloc( "FIRST"    );
    VpnKeyElabo      = namealloc( "ELABO"    );
    VpnKeyInstance   = namealloc( "INSTANCE" );
    VpnKeyInsMod     = namealloc( "INS_MOD"  );
    VpnKeyMap        = namealloc( "MAP"      );
    VpnKeyMapFor     = namealloc( "MAP_FOR"  );
    VpnKeyMapAct     = namealloc( "MAP_ACT"  );
    VpnKeyModel      = namealloc( "MODEL"    );
    VpnKeyPort       = namealloc( "PORT"     );
    VpnKeyGenMap     = namealloc( "GEN_MAP"  );
    VpnKeyGenMod     = namealloc( "GEN_MOD"  );
    VpnKeyGenExpr    = namealloc( "GEN_EXPR" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     Vpn Read Name File                      |
|                                                             |
\------------------------------------------------------------*/

static char *VpnReadNameFile( Line )

  long Line;
{
  char *Name;
  char  SaveChar;

  while ( ( VpnScanString  == (char *)0 ) ||
          ( *VpnScanString == '\0'      ) ||
          ( *VpnScanString == '\n'      ) )
  {
    if ( fgets( VpnBuffer, 512, VpnFile ) == (char *)0 )
    {
      return( (char *)0 );
    }

    VpnScanString = VpnBuffer;
    VpnLine = VpnLine + 1;
  }

  if ( ( *VpnScanString == '~' ) ||
       ( *VpnScanString == '(' ) ||
       ( *VpnScanString == ')' ) )
  {
    return( VpnScanString );
  }

  Name = VpnScanString;

  while ( ( *VpnScanString != '~'  ) &&
          ( *VpnScanString != ')'  ) &&
          ( *VpnScanString != '\0' ) &&
          ( *VpnScanString != '\n' ) )
  {
    VpnScanString = VpnScanString + 1;
  }

  SaveChar = *VpnScanString;
  *VpnScanString = '\0';
  Name = namealloc( Name );
  *VpnScanString = SaveChar;

  return( Name );
}

/*------------------------------------------------------------\
|                                                             |
|                     Vpn Read Prefix Expr                    |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VpnReadPrefixExpr()
{
  vexexpr     *Expr;
  chain_list **PrevOper;
  chain_list  *ScanOper;
  vexexpr     *ExprStr;
  char        *Name;
  char        *Scan;
  long         Oper;
  long         Left;
  long         Right;
  long         Type;

  Scan = VpnReadNameFile();

  if ( ( Scan == (char *)0 ) ||
       ( *Scan == '~'      ) ||
       ( *Scan == ')'      ) )
  {
    return( (vexexpr *)0 );
  }

  if ( *VpnScanString == '(' )
  {
    VpnScanString = VpnScanString + 1;
    Name = VpnReadNameFile();

    if ( ( Name == (char *)0 ) ||
         ( Name[ 0 ] != '{'  ) )
    {
      return( (vexexpr *)0 );
    }

    Scan = strchr( Name, '}' );

    if ( Scan == (char *)0 )
    {
      return( (vexexpr *)0 );
    }

    sscanf( Name, "{%22ld,%22ld,%22lx}", &Left, &Right, &Type );
    Name = Scan + 1;

    Oper = getvexoperbyname( Name );
    Scan = VpnReadNameFile();

    if ( Scan == (char *)0 )
    {
      return( (vexexpr *)0 );
    }

    if ( *VpnScanString == ')' )
    {
      if ( Oper == -1 ) 
      {
        Expr = createvexatomvec( Name, Left, Right );
        Expr->TYPE = Type;
      }
      else
      {
        return( (vexexpr *)0 );
      }
    }
    else
    if ( *VpnScanString == '~' )
    {
      VpnScanString = VpnScanString + 1;

      if ( Oper == -1 ) 
      {
        Expr = createvexfunc( Name, 0 );
      }
      else
      {
        Expr = createvexoper( Oper, 0 );
      }

      Expr->LEFT  = Left;
      Expr->RIGHT = Right;
      Expr->TYPE  = Type;

      if ( Left > Right ) Expr->WIDTH = ( Left  - Right ) + 1;
      else                Expr->WIDTH = ( Right - Left  ) + 1;

      PrevOper = &Expr->OPERAND;

      do
      {
        ExprStr = VpnReadPrefixExpr();

        if ( ExprStr == (vexexpr *)0 )
        {
          freevexexpr( Expr );
          return( (vexexpr *)0 );
        }

        ScanOper  = addchain( (chain_list *)0, (void *)ExprStr );
        *PrevOper = ScanOper;
        PrevOper  = &ScanOper->NEXT;
  
        Scan = VpnReadNameFile();

        if ( Scan == (char *)0 )
        {
          freevexexpr( Expr );
          return( (vexexpr *)0 );
        }

        if ( *VpnScanString == '~' )
        {
          VpnScanString = VpnScanString + 1;
        }
      }
      while ( *VpnScanString != ')');
    }
    else
    {
      return( (vexexpr *)0 );
    }

    if ( *VpnScanString == ')' ) 
    {
      VpnScanString = VpnScanString + 1;
    }
    else
    {
      freevexexpr( Expr );
      return( (vexexpr *)0 );
    }
  }
  else
  {
    Name = Scan;

    if ( ( Name == (char *)0 ) ||
         ( Name[ 0 ] != '{'  ) )
    {
      return( (vexexpr *)0 );
    }

    Scan = strchr( Name, '}' );

    if ( Scan == (char *)0 )
    {
      return( (vexexpr *)0 );
    }

    sscanf( Name, "{%22ld,%22ld,%22lx}", &Left, &Right, &Type );
    Name = Scan + 1;

    Oper = getvexoperbyname( Name );

    if ( Oper == -1 )
    {
      Expr = createvexatomvec( Name, Left, Right );
      Expr->TYPE = Type;
    }
    else
    {
      return( (vexexpr *)0 );
    }
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                           VpnReadExpr                       |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VpnReadExpr()
{
  VpnScanString = (char *)0;

  return( VpnReadPrefixExpr() );
}

/*------------------------------------------------------------\
|                                                             |
|                         VpnLoadFigure                       |
|                                                             |
\------------------------------------------------------------*/

static int VpnLoadFigure( VpnFigure )

  vpnfig_list *VpnFigure;
{
  vpndecl_list  *VpnDeclar;
  vpnattr_list  *VpnAttr;
  vpnproc_list  *VpnProc;
  vpnfunc_list  *VpnFunc;
  vpntrans_list *VpnTrans;
  vpnplace_list *VpnPlace;
  vpnplace_list *VpnLink;
  vpnact_list   *VpnAct;
  vpngen_list   *VpnGen;
  vpnline_list **VpnPrevLine;
  vpnsym        *VpnSymbol;
  vpnarc        *VpnArc;
  vpnmod_list   *VpnModel;
  vpnport_list  *VpnPort;
  vpnins_list   *VpnInst;
  vpnmap_list   *VpnMap;
  vexexpr       *Expr;
  vexexpr       *Atom;
  char          *ScanBuffer;
  char          *Keyword;
  char          *Name;
  int            Index;
  int            Length;
  int            Error;
  long           Value1;
  long           Value2;
  long           Value3;
  long           Value4;
  long           Value5;
  long           Value6;

  VpnInitKeyword();

  VpnDeclar = (vpndecl_list  *)0;
  VpnAttr   = (vpnattr_list  *)0;
  VpnSymbol = (vpnsym        *)0;
  VpnProc   = (vpnproc_list  *)0;
  VpnFunc   = (vpnfunc_list  *)0;
  VpnTrans  = (vpntrans_list *)0;
  VpnPlace  = (vpnplace_list *)0;
  VpnAct    = (vpnact_list   *)0;
  VpnArc    = (vpnarc        *)0;
  VpnModel  = (vpnmod_list   *)0;
  VpnPort   = (vpnport_list  *)0;
  VpnInst   = (vpnins_list   *)0;
  VpnMap    = (vpnmap_list   *)0;
  VpnGen    = (vpngen_list   *)0;

  VpnLine     = 0;
  VpnPrevLine = (vpnline_list **)0;

  while ( fgets( VpnBuffer, 512, VpnFile ) != (char *)0 )
  {
    VpnLine = VpnLine + 1;

    if ( ( VpnBuffer[ 0 ] == '#'  ) ||
         ( VpnBuffer[ 0 ] == '\0' ) ||
         ( VpnBuffer[ 0 ] == '\n' ) ) continue;

    Length = strlen( VpnBuffer );
    if ( VpnBuffer[ Length - 1 ] == '\n' )
    {
      Length = Length - 1;
      VpnBuffer[ Length ] = '\0';
    }

    for ( Index = Length; Index >= 0; Index-- )
    {
      if ( VpnBuffer[ Index ] == '\t' ) break;
    }

    if ( Index < 0 ) return( 1 );
    Name = &VpnBuffer[ Index + 1 ];

    ScanBuffer = strchr( VpnBuffer, '\t' );
    if ( ScanBuffer == (char *)0 ) return( 1 );

    *ScanBuffer = '\0';
    Keyword     = namealloc( VpnBuffer );
    ScanBuffer  = ScanBuffer + 1;

    if ( Keyword == VpnKeyPlace  )
    {
      Error = 4 - sscanf( ScanBuffer, "%22ld\t%22ld\t%22ld\t%22ld",
                          &Value1, &Value2, &Value3, &Value4 );

      if ( Error ) return( 1 );

      VpnPlace = searchvpnplace( VpnFigure, Name );

      if ( VpnPlace == (vpnplace_list *)0 )
      {
        if ( VpnFunc == (vpnfunc_list *)0 )
        {
          VpnPlace = addvpnprocplace( VpnFigure, VpnProc, Name );
        }
        else
        {
          VpnPlace = addvpnfuncplace( VpnFigure, VpnFunc, Name );
        }
      }

      VpnPlace->FATHER = Value1;
      VpnPlace->TYPE   = Value2;
      VpnPlace->TOKEN  = Value3;
      VpnPlace->FLAGS  = Value4;

      VpnPrevLine = &VpnPlace->LINE;
    }
    else
    if ( Keyword == VpnKeyLink )
    {
      VpnLink = searchvpnplace( VpnFigure, Name );

      if ( VpnLink == (vpnplace_list *)0 )
      {
        if ( VpnFunc == (vpnfunc_list *)0 )
        {
          VpnLink = addvpnprocplace( VpnFigure, VpnProc, Name );
        }
        else
        {
          VpnLink = addvpnfuncplace( VpnFigure, VpnFunc, Name );
        }
      }

      VpnPlace->LINK = VpnLink;
    }
    else
    if ( Keyword == VpnKeyLine )
    {
      Error = 1 - sscanf( ScanBuffer, "%22ld", &Value1 );

      if ( Error ) return( 1 );

      if ( Name[ 0 ] != '\0' )
      {
        addvpnfileline( VpnFigure, VpnPrevLine, Name, Value1 );
      }
      else
      {
        addvpnline( VpnFigure, VpnPrevLine, Value1 );
      }
    }
    else
    if ( Keyword == VpnKeyTrans  )
    {
      Error = 3 - sscanf( ScanBuffer, "%22ld\t%22ld\t%22ld",
                          &Value1, &Value2, &Value3 );

      if ( Error ) return( 1 );

      if ( VpnFunc == (vpnfunc_list *)0 )
      {
        VpnTrans = addvpnproctrans( VpnFigure, VpnProc, Name );
      }
      else
      {
        VpnTrans = addvpnfunctrans( VpnFigure, VpnFunc, Name );
      }

      VpnTrans->FATHER = Value1;
      VpnTrans->TYPE   = Value2;
      VpnTrans->FLAGS  = Value3;
    }
    else
    if ( Keyword == VpnKeyAct )
    {
      Error = 2 - sscanf( ScanBuffer, "%22ld\t%22ld",
                          &Value1, &Value2 );

      if ( Error ) return( 1 );

      VpnAct = addvpnact( VpnFigure, VpnTrans, (vexexpr *)0, (vexexpr *)0, Value1 );
      VpnAct->FLAGS = Value2;

      VpnPrevLine = &VpnAct->LINE;
    }
    else
    if ( Keyword == VpnKeyActExpr   )
    {
      Expr = VpnReadExpr();

      if ( Expr == (vexexpr *)0 ) return( 1 );

      VpnAct->VEX_EXPR = Expr;
    }
    else
    if ( Keyword == VpnKeyActAtom )
    {
      Atom = VpnReadExpr();

      if ( Atom == (vexexpr *)0 ) return( 1 );

      VpnAct->VEX_ATOM = Atom;
    }
    else
    if ( Keyword == VpnKeyArcIn )
    {
      Error = 2 - sscanf( ScanBuffer, "%22ld\t%22ld",
                          &Value1, &Value2 );

      if ( Error ) return( 1 );

      VpnPlace = searchvpnplace( VpnFigure, Name );
      VpnArc   = addvpnarcplace( VpnFigure, VpnPlace, VpnTrans );

      VpnArc->TYPE  = Value1;
      VpnArc->FLAGS = Value2;
    }
    else
    if ( Keyword == VpnKeyArcOut  )
    {
      Error = 2 - sscanf( ScanBuffer, "%22ld\t%22ld",
                          &Value1, &Value2 );

      if ( Error ) return( 1 );

      VpnPlace = searchvpnplace( VpnFigure, Name );
      VpnArc   = addvpnarctrans( VpnFigure, VpnTrans, VpnPlace );

      VpnArc->TYPE  = Value1;
      VpnArc->FLAGS = Value2;
    }
    else
    if ( Keyword == VpnKeyGuard  )
    {
      Expr = VpnReadExpr();

      if ( Expr == (vexexpr *)0 ) return( 1 );

      VpnTrans->VEX_GUARD = Expr;
    }
    else
    if ( Keyword == VpnKeyWait   )
    {
      Error = 1 - sscanf( ScanBuffer, "%22ld", &Value1 );

      if ( Error ) return( 1 );

      VpnSymbol = searchvpnsymall( VpnFigure, Name, Value1 );
      VpnTrans->WAIT_SYM = addchain( VpnTrans->WAIT_SYM, VpnSymbol );
    }
    else
    if ( Keyword == VpnKeyDeclar )
    {
      Error = 6 - sscanf( ScanBuffer, "%22ld\t%22ld\t%22ld\t%22ld\t%22ld\t%22ld",
        &Value1, &Value2, &Value3, &Value4, &Value5, &Value6 );

      if ( Error ) return( 1 );

      Atom = VpnReadExpr();

      if ( Atom == (vexexpr *)0 ) return( 1 );

      if ( ( Value2 == VPN_DECLAR_PORT     ) ||
           ( Value2 == VPN_DECLAR_CONSTANT ) ||
           ( Value2 == VPN_DECLAR_SIGNAL   ) )
      {
        VpnDeclar = addvpndecl( VpnFigure, Atom, Value2, Value5 );
      }
      else
      if ( VpnProc != (vpnproc_list *)0 )
      {
        if ( Value2 == VPN_DECLAR_DEFINE )
        {
          VpnDeclar = addvpndeclprocdef( VpnFigure, VpnProc, Atom, Value5 );
        }
        else
        if ( Value2 == VPN_DECLAR_VARIABLE )
        {
          VpnDeclar = addvpndeclprocvar( VpnFigure, VpnProc, Atom, Value5 );
        }
      }
      else
      if ( VpnFunc != (vpnfunc_list *)0 )
      {
        if ( Value2 == VPN_DECLAR_DEFINE )
        {
          VpnDeclar = addvpndeclfuncdef( VpnFigure, VpnFunc, Atom, Value5 );
        }
        else
        if ( Value2 == VPN_DECLAR_VARIABLE )
        {
          VpnDeclar = addvpndeclfuncvar( VpnFigure, VpnFunc, Atom, Value5 );
        }
        else
        if ( Value2 == VPN_DECLAR_ARGUMENT )
        {
          VpnDeclar = addvpndeclfuncarg( VpnFigure, VpnFunc, Atom, Value5, 
                                         Value1, Value2 );
        }
      }
      else
      {
        if ( Value2 == VPN_DECLAR_DEFINE )
        {
          VpnDeclar = addvpndecldef( VpnFigure, Atom, Value5 );
        }
        else
        if ( Value2 == VPN_DECLAR_VARIABLE )
        {
          VpnDeclar = addvpndeclvar( VpnFigure, Atom, Value5 );
        }
      }

      VpnDeclar->DIR   = Value1;
      VpnDeclar->TYPE  = Value2;
      VpnDeclar->KIND  = Value3;
      VpnDeclar->CLASS = Value4;
      VpnDeclar->BASE  = Value5;
      VpnDeclar->FLAGS = Value6;

      VpnPrevLine = &VpnDeclar->LINE;
    }
    else
    if ( Keyword == VpnKeyInit )
    {
      Expr = VpnReadExpr();

      if ( Expr == (vexexpr *)0 ) return( 1 );

      VpnDeclar->VEX_INIT = Expr;
    }
    else
    if ( Keyword == VpnKeySymbol )
    {
      Error = 6 - sscanf( ScanBuffer, "%22ld\t%22ld\t%22ld\t%22ld\t%22ld\t%22ld",
        &Value1, &Value2, &Value3, &Value4, &Value5, &Value6 );

      if ( Error ) return( 1 );

      VpnSymbol = searchvpnsymall( VpnFigure, Name, Value1 );

      VpnSymbol->INIT  = Value2;
      VpnSymbol->DRIVE = Value3;
      VpnSymbol->EFFEC = Value4;
      VpnSymbol->EVENT = Value5;
      VpnSymbol->FLAGS = Value6;
    }
    else
    if ( Keyword == VpnKeyRet )
    {
      Atom = VpnReadExpr();

      if ( Atom == (vexexpr *)0 ) return( 1 );

      VpnFunc->VEX_RET = Atom;
    }
    else
    if ( Keyword == VpnKeyProcess )
    {
      Error = 2 - sscanf( ScanBuffer, "%22ld %22ld", &Value1, &Value2 );

      if ( Error ) return( 1 );
      
      VpnProc = addvpnproc( VpnFigure, Name );

      VpnProc->TYPE  = Value1;
      VpnProc->FLAGS = Value2;
      VpnFunc = (vpnfunc_list *)0;

      VpnPrevLine = &VpnProc->LINE;
    }
    else
    if ( Keyword == VpnKeyFunction )
    {
      Error = 2 - sscanf( ScanBuffer, "%22ld %22ld", &Value1, &Value2 );

      if ( Error ) return( 1 );
      
      VpnFunc = addvpnfunc( VpnFigure, Name );

      VpnFunc->TYPE  = Value1;
      VpnFunc->FLAGS = Value2;
      VpnProc = (vpnproc_list *)0;

      VpnPrevLine = &VpnFunc->LINE;
    }
    else
    if ( Keyword == VpnKeyFirst )
    {
      if ( VpnFunc == (vpnfunc_list *)0 )
      {
        VpnProc->FIRST = searchvpntrans( VpnFigure, Name );
      }
      else
      {
        VpnFunc->FIRST = searchvpntrans( VpnFigure, Name );
      }
    }
    else
    if ( Keyword == VpnKeyElabo  )
    {
      if ( VpnFunc == (vpnfunc_list *)0 )
      {
        VpnProc->ELABO = searchvpntrans( VpnFigure, Name );
      }
      else
      {
        VpnFunc->ELABO = searchvpntrans( VpnFigure, Name );
      }
    }
    else
    if ( Keyword == VpnKeyInstance )
    {
      Error = 1 - sscanf( ScanBuffer, "%22ld", &Value1 );

      if ( Error ) return( 1 );

      VpnInst = addvpnins( VpnFigure, Name, (vpnmod_list *)0 );

      VpnInst->FLAGS = Value1;

      VpnPrevLine = &VpnInst->LINE;
    }
    else
    if ( Keyword == VpnKeyInsMod )
    {
      VpnModel = searchvpnmod( VpnFigure, Name );

      if ( VpnModel == (vpnmod_list *)0 ) return( 1 );

      VpnInst->MODEL = VpnModel;
    }
    else
    if ( Keyword == VpnKeyMap )
    {
      Error = 1 - sscanf( ScanBuffer, "%22ld", &Value1 );

      if ( Error ) return( 1 );

      VpnMap = addvpninsmap( VpnFigure, VpnInst, (vexexpr *)0, (vexexpr *)0 );

      VpnMap->FLAGS = Value1;

      VpnPrevLine = &VpnMap->LINE;
    }
    else
    if ( Keyword == VpnKeyMapFor )
    {
      Atom = VpnReadExpr();

      if ( Atom == (vexexpr *)0 ) return( 1 );

      VpnMap->VEX_FORMAL = Atom;
    }
    else
    if ( Keyword == VpnKeyMapAct )
    {
      Atom = VpnReadExpr();

      if ( Atom == (vexexpr *)0 ) return( 1 );

      VpnMap->VEX_ACTUAL = Atom;
    }
    else
    if ( Keyword == VpnKeyModel )
    {
      Error = 1 - sscanf( ScanBuffer, "%22ld", &Value1 );

      if ( Error ) return( 1 );

      VpnModel = addvpnmod( VpnFigure, Name );

      VpnModel->FLAGS = Value1;

      VpnPrevLine = &VpnModel->LINE;
    }
    else
    if ( Keyword == VpnKeyPort )
    {
      Error = 3 - sscanf( ScanBuffer, "%22ld\t%22ld\t%22ld", &Value1, &Value2, &Value3 );

      if ( Error ) return( 1 );

      Atom = VpnReadExpr();

      if ( Atom == (vexexpr *)0 ) return( 1 );

      VpnPort = addvpnmodport( VpnFigure, VpnModel, Atom, Value2, Value1 );
      VpnPort->FLAGS = Value3;

      VpnPrevLine = &VpnPort->LINE;
    }
    else
    if ( Keyword == VpnKeyFigure )
    {
      Error = 1 - sscanf( ScanBuffer, "%22ld", &Value1 );

      if ( Error ) return( 1 );

      VpnFigure->NAME  = namealloc( Name );
      VpnFigure->FLAGS = Value1;
    }
    else
    if ( Keyword == VpnKeyAttr )
    {
      Error = 2 - sscanf( ScanBuffer, "%22ld\t%22ld", &Value1, &Value2 );

      if ( Error ) return( 1 );

      VpnAttr = addvpnattr( VpnFigure, Name, Name, Value1, Name );

      VpnAttr->FLAGS = Value2;

      VpnPrevLine = &VpnAttr->LINE;
    }
    else
    if ( Keyword == VpnKeyAttrEnt )
    {
      VpnAttr->ENTITY = namealloc( Name );

      Name = strchr( ScanBuffer, '\t' );
      if ( ScanBuffer == (char *)0 ) return( 1 );

      *Name = '\0';
      VpnAttr->VALUE = namealloc( ScanBuffer );
    }
    else
    if ( Keyword == VpnKeyGenMap )
    {
      Error = 2 - sscanf( ScanBuffer, "%22ld\t%22ld", &Value1, &Value2 );

      if ( Error ) return( 1 );

      Atom = VpnReadExpr();

      if ( Atom == (vexexpr *)0 ) return( 1 );

      VpnGen = addvpninsgen( VpnFigure, VpnInst, Atom, (vexexpr *)0 );

      VpnGen->BASE  = Value1;
      VpnGen->FLAGS = Value2;

      VpnPrevLine = &VpnGen->LINE;
    }
    else
    if ( Keyword == VpnKeyGenMod )
    {
      Error = 2 - sscanf( ScanBuffer, "%22ld\t%22ld", &Value1, &Value2 );

      if ( Error ) return( 1 );

      Atom = VpnReadExpr();

      if ( Atom == (vexexpr *)0 ) return( 1 );

      VpnGen = addvpnmodgen( VpnFigure, VpnModel, Atom, (vexexpr *)0, Value1 );

      VpnGen->FLAGS = Value2;

      VpnPrevLine = &VpnGen->LINE;
    }
    else
    if ( Keyword == VpnKeyGenExpr )
    {
      Expr = VpnReadExpr();

      if ( Expr == (vexexpr *)0 ) return( 1 );

      VpnGen->VEX_EXPR = Expr;
    }
    else return( 1 );
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                        vpnloadvpnfig                        |
|                                                             |
\------------------------------------------------------------*/

void vpnloadvpnfig( VpnFigure, FigureName )

  vpnfig_list *VpnFigure;
  char        *FigureName;
{
  int Value;

  VpnFile = mbkfopen( FigureName, "vpn", "r" );

  if ( VpnFile == (FILE *)0 )
  {
    vpderror( VPD_ERROR_OPEN_FILE, FigureName, 0 );
  }

  Value = VpnLoadFigure( VpnFigure );

  if ( Value ) vpderror( VPD_ERROR_PARSE_FILE, FigureName, VpnLine );

  fclose( VpnFile );
}
