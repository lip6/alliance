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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                     VASY                          |
|                                                             |
| File    :                  vasy_redinst.c                   |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
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
# include <stdlib.h>
# include <string.h>

# include "mut.h"
# include "aut.h"
# include "vex.h"
# include "vpn.h"
# include "vtl.h"

# include "vasy_redact.h"
# include "vasy_redinst.h"
# include "vasy_debug.h"
# include "vasy_error.h"

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

  static vpnfig_list   *VasyFigure    = (vpnfig_list   *)0;
  static vpnproc_list  *VasyProcess   = (vpnproc_list  *)0;
  static vpntrans_list *VasyPrevTrans = (vpntrans_list *)0;

  static auth2table    *VasyHash2Assign    = (auth2table *)0;
  static auth2table    *VasyHash2BitVecAsg = (auth2table *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

static void VasyRedInstVpnPlace();

static void VasyRedInstViewAuth2Elem( Element )

  auth2elem *Element;
{
  vpntrans_list *VpnTrans;
  vpnsym        *VpnSymbol;
  vpnact_list   *VpnAct;

  VpnTrans  = (vpntrans_list *)Element->KEY1;
  VpnSymbol = (vpnsym        *)Element->KEY2;
  VpnAct    = (vpnact_list   *)Element->VALUE;

  VasyPrintf( stdout, "%s, %s %d, ",
           VpnTrans->NAME, VpnSymbol->NAME, VpnSymbol->INDEX );
  viewvexexprbound( VpnAct->VEX_ATOM );
  fprintf( stdout, "  <= " );
  viewvexexprboundln( VpnAct->VEX_EXPR );
}

/*------------------------------------------------------------\
|                                                             |
|                    VasyRedInstHashVpnAct                    |
|                                                             |
\------------------------------------------------------------*/

static int VasyRedInstHashVpnAct( VpnTrans, VpnAct )

  vpntrans_list *VpnTrans;
  vpnact_list   *VpnAct;
{
  vpnact_list   *NewAct;
  vpndecl_list  *AsgDeclar;
  vpndecl_list  *DefDeclar;
  vexexpr       *AsgAtom;
  vexexpr       *DefAtom;
  vexexpr       *AsgExpr;
  vpnsym        *AsgSymbol;
  char          *AtomValue;
  int            AsgMin;
  int            AsgMax;
  int            AsgIndex;

  AsgAtom = VpnAct->VEX_ATOM;
  AsgExpr = VpnAct->VEX_EXPR;
      
  if ( ! IsVexNodeAtom( AsgAtom ) )
  {
    VasyErrorLine( VASY_ILLEGAL_ASSIGN_ERROR, VpnAct->LINE, getvexatomname( AsgAtom ) );
  }

  AtomValue = GetVexAtomValue( AsgAtom );
  AsgDeclar = searchvpndeclall( VasyFigure, AtomValue );

  if ( ! IsVexNodeAtom( AsgExpr ) )
  {
    if ( IsVasyDebugLevel2() )
    {
      VasyPrintf( stdout, "  +++ Hash assign is not atomic, create define !\n" );
    }

    DefDeclar = VasyRedActAddVpnDefine( VasyFigure, VasyProcess,
                                       AsgAtom->WIDTH, AsgDeclar->BASE );

    DefAtom = dupvexnode( DefDeclar->VEX_ATOM );
    VpnAct->VEX_EXPR = dupvexnode( DefAtom );

    NewAct = addvpnactasgbefore( VasyFigure, VpnTrans, VpnAct, DefAtom, AsgExpr );
    unionvpnline( VasyFigure, &NewAct->LINE, VpnAct->LINE );

    AsgExpr = VpnAct->VEX_EXPR;
  }

  addauth2elem( VasyHash2Assign, (char*)VpnTrans, (char*)AtomValue, (long)VpnAct );

  if ( isvextypedivisible( AsgDeclar->BASE ) )
  {
    AsgMin = getvexvectormin( AsgAtom );
    AsgMax = getvexvectormax( AsgAtom );

    for ( AsgIndex = AsgMin; AsgIndex <= AsgMax; AsgIndex++ )
    {
      AsgSymbol = getvpnsymdecl( AsgDeclar, AsgIndex );
      addauth2elem( VasyHash2BitVecAsg, (char*)VpnTrans, (char*)AsgSymbol, (long)VpnAct );
    }

    return( 1 );
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyRedInstVertical                   |
|                                                             |
\------------------------------------------------------------*/

static vpntrans_list *VasyRedInstVertical( BeginPlace, EndTrans )

  vpnplace_list *BeginPlace;
  vpntrans_list *EndTrans;
{
  vpntrans_list *VpnTrans;
  vpnplace_list *VpnPlace;
  vpnplace_list *EndPlace;
  vpnarc        *VpnArc;

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  --> VasyRedInstVertical %s %s\n", 
             BeginPlace->NAME, EndTrans->NAME );
  }

  if ( ( EndTrans->TYPE != VPN_TRANS_ACT_GUARDED ) &&
       ( EndTrans->TYPE != VPN_TRANS_ACT_EXEC    ) &&
       ( EndTrans->TYPE != VPN_TRANS_IMMEDIATE   ) )
  {
    return( EndTrans );
  }

  while ( EndTrans->NUMBER_IN == 1 )
  {
    VpnArc    = GetVpnArc( EndTrans->PLACE_OUT );
    EndPlace  = GetVpnArcTargetPlace( VpnArc );

    VpnArc    = GetVpnArc( EndTrans->PLACE_IN );
    VpnPlace  = GetVpnArcSourcePlace( VpnArc );

    if ( ( VpnPlace == BeginPlace    ) ||
         ( VpnPlace->NUMBER_IN  != 1 ) ||
         ( VpnPlace->NUMBER_OUT != 1 ) )
    {
      break;
    }

    VpnArc   = GetVpnArc( VpnPlace->TRANS_IN );
    VpnTrans = GetVpnArcSourceTrans( VpnArc );

    if ( EndTrans->TYPE != VPN_TRANS_IMMEDIATE )
    {
      if ( ( VpnTrans->TYPE  != VPN_TRANS_GUARDED     ) &&
           ( VpnTrans->TYPE  != VPN_TRANS_ACT_GUARDED ) &&
           ( VpnTrans->TYPE  != VPN_TRANS_ACT_EXEC    ) &&
           ( VpnTrans->TYPE  != VPN_TRANS_IMMEDIATE   ) )
      {
        break;
      }

      if ( VpnTrans->TYPE == VPN_TRANS_IMMEDIATE )
      {
        VpnTrans->TYPE  = EndTrans->TYPE;

        if ( EndTrans->ACT != (vpnact_list *)0 )
        {
          VpnTrans->ACT      = EndTrans->ACT;
          VpnTrans->LAST_ACT = EndTrans->LAST_ACT;
        }

        VpnTrans->VEX_GUARD = EndTrans->VEX_GUARD;

        EndTrans->ACT       = (vpnact_list *)0;
        EndTrans->LAST_ACT  = &EndTrans->ACT;
        EndTrans->VEX_GUARD = (vexexpr     *)0;
      }
      else
      {
        if ( ( VpnTrans->TYPE == VPN_TRANS_GUARDED     ) &&
             ( EndTrans->TYPE == VPN_TRANS_ACT_GUARDED ) )
        {
          VpnTrans->TYPE = VPN_TRANS_ACT_GUARDED;

          if ( EndTrans->ACT != (vpnact_list *)0 )
          {
            VpnTrans->ACT      = EndTrans->ACT;
            VpnTrans->LAST_ACT = EndTrans->LAST_ACT;
          }

          VpnTrans->VEX_GUARD = optimvexbinexpr( VEX_AND, 1,
                                                 EndTrans->VEX_GUARD,
                                                 VpnTrans->VEX_GUARD );

          EndTrans->ACT       = (vpnact_list *)0;
          EndTrans->LAST_ACT  = &EndTrans->ACT;
          EndTrans->VEX_GUARD = (vexexpr     *)0;
        }
        else
        {
          if ( EndTrans->ACT != (vpnact_list *)0 )
          {
            *VpnTrans->LAST_ACT = EndTrans->ACT;
            EndTrans->ACT->PREV = VpnTrans->LAST_ACT;
            VpnTrans->LAST_ACT  = EndTrans->LAST_ACT;
            EndTrans->ACT       = (vpnact_list *)0;
            EndTrans->LAST_ACT  = &EndTrans->ACT;

            VasyRedActVpnTrans( VasyFigure, VasyProcess, VpnTrans, 1 );
          }

          if ( VpnTrans->TYPE == VPN_TRANS_GUARDED )
          {
            VpnTrans->TYPE = VPN_TRANS_ACT_GUARDED;
          }
        }
      }
    }

    if ( ( EndTrans->TYPE == VPN_TRANS_ACT_GUARDED ) ||
         ( EndTrans->TYPE == VPN_TRANS_ACT_EXEC    ) ||
         ( EndTrans->TYPE == VPN_TRANS_IMMEDIATE   ) )
    {
      delvpnplace( VasyFigure, VpnPlace );
      delvpntrans( VasyFigure, EndTrans );

      addvpnarctrans( VasyFigure, VpnTrans, EndPlace );
    }

    EndTrans = VpnTrans;
  }

  if ( IsVasyDebugLevel2() )
  {
    VasyDebugSaveVpnFig( VasyFigure );

    VasyPrintf( stdout, "  <-- VasyRedInstVertical %s %s\n", 
             BeginPlace->NAME, EndTrans->NAME );
  }

  return( EndTrans );
}

/*------------------------------------------------------------\
|                                                             |
|                          VasyRedInstGuarded                 |
|                                                             |
\------------------------------------------------------------*/

static int VasyRedInstGuarded( BeginPlace )

  vpnplace_list *BeginPlace;
{
  chain_list    *ScanTrans;
  chain_list    *ScanTrans2;
  vpntrans_list *VpnTrans;
  vpntrans_list *VpnTrans2;
  vpnplace_list *VpnPlace;
  vpnarc        *VpnArc;
  vexexpr       *Guard;
  int            Reduce;

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  --> VasyRedInstGuarded %s\n", BeginPlace->NAME );
  }

  ScanTrans = BeginPlace->TRANS_OUT;
  Reduce    = 0;

  while ( ScanTrans != (chain_list *)0 )
  {
    VpnArc   = GetVpnArc( ScanTrans );
    VpnTrans = GetVpnArcTargetTrans( VpnArc );

    ScanTrans = ScanTrans->NEXT;

    if ( VpnTrans->TYPE == VPN_TRANS_GUARDED )
    {
      VpnArc    = GetVpnArc( VpnTrans->PLACE_OUT );
      VpnPlace  = GetVpnArcTargetPlace( VpnArc );

      if ( ( VpnPlace->NUMBER_IN == 1      ) &&
           ( ! IsVasyRedInstTag( VpnPlace ) ) )
      {
        for ( ScanTrans2  = VpnPlace->TRANS_OUT;
              ScanTrans2 != (chain_list *)0;
              ScanTrans2  = ScanTrans2->NEXT )
        {
          VpnArc    = GetVpnArc( ScanTrans2 );
          VpnTrans2 = GetVpnArcTargetTrans( VpnArc );

          if ( ( VpnTrans2->TYPE != VPN_TRANS_ACT_EXEC    ) &&
               ( VpnTrans2->TYPE != VPN_TRANS_ACT_GUARDED ) &&
               ( VpnTrans2->TYPE != VPN_TRANS_GUARDED     ) &&
               ( VpnTrans2->TYPE != VPN_TRANS_IMMEDIATE   ) ) break;
        }

        if ( ScanTrans2 != (chain_list *)0 ) continue;

        ScanTrans2 = VpnPlace->TRANS_OUT;

        while ( ScanTrans2 != (chain_list *)0 )
        {
          VpnArc    = GetVpnArc( ScanTrans2 );
          VpnTrans2 = GetVpnArcTargetTrans( VpnArc );

          Guard = dupvexexpr( VpnTrans->VEX_GUARD );

          if ( VpnTrans2->TYPE == VPN_TRANS_IMMEDIATE )
          {
            VpnTrans2->TYPE  = VPN_TRANS_GUARDED;
            VpnTrans2->VEX_GUARD = Guard;
          }
          else
          if ( VpnTrans2->TYPE == VPN_TRANS_ACT_EXEC )
          {
            VpnTrans2->TYPE  = VPN_TRANS_ACT_GUARDED;
            VpnTrans2->VEX_GUARD = Guard;
          }
          else
          {
            VpnTrans2->VEX_GUARD = optimvexbinexpr( VEX_AND, 1,
                                                    Guard, VpnTrans2->VEX_GUARD );
          }

          addvpnarcplace( VasyFigure, BeginPlace, VpnTrans2 );

          ScanTrans2 = ScanTrans2->NEXT;
        }

        delvpnplace( VasyFigure, VpnPlace );
        delvpntrans( VasyFigure, VpnTrans );

        Reduce = 1;
      }
    }
  }

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  <-- VasyRedInstGuarded\n" );

    if ( Reduce ) VasyDebugSaveVpnFig( VasyFigure );
  }

  return( Reduce );
}

/*------------------------------------------------------------\
|                                                             |
|                    VasyRedInstLateralVpnAct                 |
|                                                             |
\------------------------------------------------------------*/

static void VasyRedInstLateralVpnAct( BeginPlace )

  vpnplace_list *BeginPlace;
{
  chain_list    *ScanTrans;
  chain_list    *ScanTrans2;
  vpntrans_list *VpnTrans;
  vpntrans_list *VpnTrans2;
  vpnarc        *VpnArc;
  vpnarc        *VpnArc2;
  vpnact_list   *VpnAct;
  vpnact_list   *VpnAct2;
  vpnact_list   *VpnAct3;
  vpndecl_list  *AsgDeclar;
  vpnsym        *AsgSymbol;
  vexexpr       *AsgAtom;
  vexexpr       *AsgAtom2;
  vexexpr       *AsgAtom3;
  vexexpr       *AsgExpr;
  vexexpr       *AsgExpr2;
  vexexpr       *AsgExpr3;
  char          *AtomValue;
  char          *AtomValue2;
  auth2elem     *Element;
  int            AsgIndex;
  int            AsgMin;
  int            AsgMax;
  int            AsgMin2;
  int            AsgMax2;
  int            AsgPos2;
  int            AsgPos3;
  int            AsgIndex2;
  int            AsgIndex3;
  int            FoundBitVec;
/*
** For all transitions Ti, put all ASGi in hash table
*/
  FoundBitVec = 0;

  for ( ScanTrans  = BeginPlace->TRANS_OUT;
        ScanTrans != (chain_list *)0;
        ScanTrans  = ScanTrans->NEXT )
  {
    VpnArc   = GetVpnArc( ScanTrans );
    VpnTrans = GetVpnArcTargetTrans( VpnArc );

    for ( VpnAct  = VpnTrans->ACT;
          VpnAct != (vpnact_list *)0;
          VpnAct  = VpnAct->NEXT )
    {
      if ( VpnAct->TYPE == VPN_ACT_ASG_DEFINE ) continue;

      FoundBitVec |= VasyRedInstHashVpnAct( VpnTrans, VpnAct );
    }
  }

  if ( ! FoundBitVec ) return;
/*
**  For All Bit vector assign, built disjoint assignement
*/
  for ( ScanTrans  = BeginPlace->TRANS_OUT;
        ScanTrans != (chain_list *)0;
        ScanTrans  = ScanTrans->NEXT )
  {
    VpnArc   = GetVpnArc( ScanTrans );
    VpnTrans = GetVpnArcTargetTrans( VpnArc );

    for ( VpnAct  = VpnTrans->ACT;
          VpnAct != (vpnact_list *)0;
          VpnAct  = VpnAct->NEXT )
    {
      if ( VpnAct->TYPE == VPN_ACT_ASG_DEFINE ) continue;
/*
**  Assign statement
*/
      AsgAtom = VpnAct->VEX_ATOM;
      AsgExpr = VpnAct->VEX_EXPR;
      
      AtomValue = GetVexAtomValue( AsgAtom );
      AsgDeclar = searchvpndeclall( VasyFigure, AtomValue );

      if ( ! isvextypedivisible( AsgDeclar->BASE ) ) continue;

      if ( IsVasyDebugLevel2() )
      {
        VasyPrintf( stdout, "  +++ Check Assign in others transition !\n" );
        viewvexexprbound( AsgAtom );
        viewvexexprboundln( AsgExpr );
      }

      AsgMin = getvexvectormin( AsgAtom );
      AsgMax = getvexvectormax( AsgAtom );
/*
**  Check all assign to bit_vector symbol V in others transitions
*/
      for ( ScanTrans2  = BeginPlace->TRANS_OUT;
            ScanTrans2 != (chain_list *)0;
            ScanTrans2  = ScanTrans2->NEXT )
      {
        VpnArc2   = GetVpnArc( ScanTrans2 );
        VpnTrans2 = GetVpnArcTargetTrans( VpnArc2 );

        if ( VpnTrans2 == VpnTrans ) continue;

        for ( AsgIndex = AsgMin; AsgIndex <= AsgMax; AsgIndex++ )
        {
          AsgSymbol = getvpnsymdecl( AsgDeclar, AsgIndex );
          Element   = searchauth2elem( VasyHash2BitVecAsg, (char*)VpnTrans2, (char*)AsgSymbol );
/*
**  Assign to symbol V appears in transistion VpnTrans2 !
*/
          if ( Element != (auth2elem *)0 )
          {
            VpnAct2 = (vpnact_list *)Element->VALUE;

            AsgAtom2 = VpnAct2->VEX_ATOM;
            AsgExpr2 = VpnAct2->VEX_EXPR;

            if ( IsVasyDebugLevel2() )
            {
              VasyPrintf( stdout, "  +++ Prev asg %s %d trans %s\n",
                       AsgSymbol->NAME, AsgIndex, VpnTrans2->NAME );
              viewvexexprbound( AsgAtom2 );
              viewvexexprboundln( AsgExpr2 );
            }

            AtomValue2 = GetVexAtomValue( AsgAtom2 );

            AsgMin2 = getvexvectormin( AsgAtom2 );
            AsgMax2 = getvexvectormax( AsgAtom2 );

            if ( IsVasyDebugLevel2() )
            {
              VasyPrintf( stdout, "  +++ Check instersection [%d %d] -> [%d %d]\n", 
                       AsgMin, AsgMax, AsgMin2, AsgMax2 );
            }
/*
**  Check the kind of range's intersection 
*/
            if ( ( AsgMin2 <  AsgMin ) &&
                 ( AsgMax2 >= AsgMin ) )
            {
/*
**           [        ]   Asg      Partialy included on left
**       [<-[]------->]   Asg2
*/
              if ( IsVasyDebugLevel2() )
              {
                VasyPrintf( stdout, "  +++ Intersection partialy on left\n" );
              }

              AsgAtom3 = dupvexexpr( AsgAtom2 );
              AsgExpr3 = dupvexexpr( AsgExpr2 );

              if ( IsVexAtomDown( AsgAtom2 ) )
              {
                AsgPos2   = getvexvectorpos( AsgAtom2, AsgMin );
                AsgIndex2 = getvexvectorindex( AsgExpr2, AsgPos2 );

                slicevexatomvec( AsgExpr2, AsgExpr2->LEFT, AsgIndex2 );
                slicevexatomvec( AsgAtom2, AsgAtom2->LEFT, AsgMin    );

                AsgPos3   = getvexvectorpos( AsgAtom3, AsgMin - 1 );
                AsgIndex3 = getvexvectorindex( AsgExpr3, AsgPos3 );

                slicevexatomvec( AsgExpr3, AsgIndex3 , AsgExpr3->RIGHT );
                slicevexatomvec( AsgAtom3, AsgMin - 1, AsgAtom3->RIGHT );
              }
              else
              {
                AsgPos2   = getvexvectorpos( AsgAtom2, AsgMin );
                AsgIndex2 = getvexvectorindex( AsgExpr2, AsgPos2 );

                slicevexatomvec( AsgExpr2, AsgIndex2, AsgExpr2->RIGHT );
                slicevexatomvec( AsgAtom2, AsgMin   , AsgAtom2->RIGHT );

                AsgPos3   = getvexvectorpos( AsgAtom3, AsgMin - 1 );
                AsgIndex3 = getvexvectorindex( AsgExpr3, AsgPos3 );

                slicevexatomvec( AsgExpr3, AsgExpr3->LEFT, AsgIndex3   );
                slicevexatomvec( AsgAtom3, AsgAtom3->LEFT, AsgMin - 1 );
              }

              VpnAct3 = addvpnactasgbefore( VasyFigure, VpnTrans2, 
                                            VpnAct2, AsgAtom3, AsgExpr3 );
              unionvpnline( VasyFigure, &VpnAct3->LINE, VpnAct2->LINE );

              VasyRedInstHashVpnAct( VpnTrans2, VpnAct3 );

              AsgMin2 = getvexvectormin( AsgAtom2 );
              AsgMax2 = getvexvectormax( AsgAtom2 );

              if ( IsVasyDebugLevel2() )
              {
                VasyPrintf( stdout, "  +++ New assign \n" );
                viewvexexprbound( AsgAtom2 );
                fprintf( stdout, " <= " );
                viewvexexprboundln( AsgExpr2 );
                viewvexexprbound( AsgAtom3 );
                fprintf( stdout, " <= " );
                viewvexexprboundln( AsgExpr3 );
              }
            }

            if ( ( AsgMax2 >  AsgMax ) &&
                 ( AsgMin2 <= AsgMax ) )
            {
/*
**       [        ]       Asg      Partialy included on right
**       [<-------[]->]   Asg2
*/
              if ( IsVasyDebugLevel2() )
              {
                VasyPrintf( stdout, "  +++ Intersection partialy on right\n" );
              }

              AsgAtom3 = dupvexexpr( AsgAtom2 );
              AsgExpr3 = dupvexexpr( AsgExpr2 );

              if ( IsVexAtomDown( AsgAtom2 ) )
              {
                AsgPos3   = getvexvectorpos( AsgAtom3, AsgMax + 1 );
                AsgIndex3 = getvexvectorindex( AsgExpr3, AsgPos3  );

                slicevexatomvec( AsgExpr3, AsgExpr3->LEFT, AsgIndex3  );
                slicevexatomvec( AsgAtom3, AsgAtom3->LEFT, AsgMax + 1 );

                AsgPos2   = getvexvectorpos( AsgAtom2, AsgMax );
                AsgIndex2 = getvexvectorindex( AsgExpr2, AsgPos2 );

                slicevexatomvec( AsgExpr2, AsgIndex2, AsgExpr2->RIGHT );
                slicevexatomvec( AsgAtom2, AsgMax   , AsgAtom2->RIGHT );
              }
              else
              {
                AsgPos3   = getvexvectorpos( AsgAtom3, AsgMax + 1 );
                AsgIndex3 = getvexvectorindex( AsgExpr3, AsgPos3 );

                slicevexatomvec( AsgExpr3, AsgIndex3 , AsgExpr3->RIGHT );
                slicevexatomvec( AsgAtom3, AsgMax + 1, AsgAtom3->RIGHT );

                AsgPos2   = getvexvectorpos( AsgAtom2, AsgMax );
                AsgIndex2 = getvexvectorindex( AsgExpr2, AsgPos2 );

                slicevexatomvec( AsgExpr2, AsgExpr2->LEFT, AsgIndex2 );
                slicevexatomvec( AsgAtom2, AsgAtom2->LEFT, AsgMax    );
              }

              VpnAct3 = addvpnactasgbefore( VasyFigure, VpnTrans2, 
                                            VpnAct2, AsgAtom3, AsgExpr3 );
              unionvpnline( VasyFigure, &VpnAct3->LINE, VpnAct2->LINE );

              VasyRedInstHashVpnAct( VpnTrans2, VpnAct3 );

              AsgMin2 = getvexvectormin( AsgAtom2 );
              AsgMax2 = getvexvectormax( AsgAtom2 );

              if ( IsVasyDebugLevel2() )
              {
                VasyPrintf( stdout, "  +++ New assign \n" );
                viewvexexprbound( AsgAtom2 );
                fprintf( stdout, " <= " );
                viewvexexprboundln( AsgExpr2 );
                viewvexexprbound( AsgAtom3 );
                fprintf( stdout, " <= " );
                viewvexexprboundln( AsgExpr3 );
                VasyDebugSaveVpnFig( VasyFigure );
              }
            }

            if ( ( AsgMin2 == AsgMin ) &&
                 ( AsgMax2 == AsgMax ) )
            {
/*
**       [            ]   Asg       Completetly Equal
**       [<-[      ]->]   Asg2
*/
              if ( IsVasyDebugLevel2() )
              {
                VasyPrintf( stdout, "  +++ Intersection Completely equal !!\n" );
              }

              AsgIndex = AsgMax;
            }
            else
            if ( ( AsgMin2 >= AsgMin ) &&
                 ( AsgMax2 <= AsgMax ) )
            {
/*
**       [            ]   Asg       Completely included
**       [<-[      ]->]   Asg2
*/
              if ( IsVasyDebugLevel2() )
              {
                VasyPrintf( stdout, "  +++ Intersection completely included !!\n" );
              }

              AsgIndex = AsgMax2;
            }
            else
            {
              AsgIndex = AsgMax;
            }
          }
        }
      }
    }
  }

  if ( IsVasyDebugLevel2() )
  {
    viewauth2table( VasyHash2BitVecAsg, VasyRedInstViewAuth2Elem );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    IsVasyRedInstGuardAtomic                  |
|                                                             |
\------------------------------------------------------------*/

int IsVasyRedInstGuardAtomic( VexGuard )

  vexexpr *VexGuard;
{
  long Oper;

  if ( IsVexNodeAtom( VexGuard ) )
  {
    return( 1 );
  }

  if ( IsVexNodeOper( VexGuard ) ) 
  {
    Oper = GetVexOperValue( VexGuard );

    if ( Oper == VEX_NOT )
    {
      VexGuard = GetVexOperand( VexGuard->OPERAND );

      return( IsVasyRedInstGuardAtomic( VexGuard ) );
    }
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyRedInstLateral                    |
|                                                             |
\------------------------------------------------------------*/

static int VasyRedInstLateral( BeginPlace, EndPlace, KeepGuard )

  vpnplace_list *BeginPlace;
  vpnplace_list *EndPlace;
  short          KeepGuard;
{
  chain_list    *ScanTrans;
  chain_list    *ScanTrans2;
  vpntrans_list *FirstTrans;
  vpntrans_list *VpnTrans;
  vpntrans_list *VpnTrans2;
  vpnplace_list *VpnPlace;
  vpnarc        *VpnArc;
  vpnarc        *VpnArc2;
  vpnact_list   *VpnAct;
  vpnact_list   *DelAct;
  vpnact_list   *PrevAct;
  vpnact_list   *VpnAct2;
  vpnact_list   *NewAct;
  vpnact_list   *NewAct2;
  vpndecl_list  *AsgDeclar;
  vpndecl_list  *DefDeclar;
  vpnsym        *AsgSymbol;
  vexexpr       *AsgAtom;
  vexexpr       *VexGuard;
  vexexpr       *VexDriver;
  vexexpr       *VexExpr;
  vexexpr       *DefAtom;
  char          *AtomValue;
  auth2elem     *Element;

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  --> VasyRedInstLateral %s %s\n", BeginPlace->NAME, EndPlace->NAME );
  }

  if ( BeginPlace->NUMBER_OUT <= 1 ) return( 0 );
/*
** For all transitions Ti, Tj, verify Ti->OUT == Tj->OUT, and Ti & Tj Guarded.
*/
  for ( ScanTrans  = BeginPlace->TRANS_OUT;
        ScanTrans != (chain_list *)0;
        ScanTrans  = ScanTrans->NEXT )
  {
    VpnArc   = GetVpnArc( ScanTrans );
    VpnTrans = GetVpnArcTargetTrans( VpnArc );

    if ( ( VpnTrans->TYPE != VPN_TRANS_GUARDED     ) &&
         ( VpnTrans->TYPE != VPN_TRANS_ACT_GUARDED ) ) return( 0 );

    VpnArc   = GetVpnArc( VpnTrans->PLACE_OUT );
    VpnPlace = GetVpnArcTargetPlace( VpnArc );

    if ( EndPlace != VpnPlace ) return( 0 );
  }

  if ( VasyHash2Assign == (auth2table *)0 )
  {
    VasyHash2Assign    = createauth2table( 100 );
    VasyHash2BitVecAsg = createauth2table( 100 );
  }
/*
**  Make only disjunct assigment for all ASGi in Ti
*/
  VasyRedInstLateralVpnAct( BeginPlace );
/*
**  Make all guard's expression atomic, and move define on first trans
*/
  FirstTrans = (vpntrans_list *)0;

  for ( ScanTrans  = BeginPlace->TRANS_OUT;
        ScanTrans != (chain_list *)0;
        ScanTrans  = ScanTrans->NEXT )
  {
    VpnArc   = GetVpnArc( ScanTrans );
    VpnTrans = GetVpnArcTargetTrans( VpnArc );
    VexGuard = VpnTrans->VEX_GUARD;

    if ( FirstTrans == (vpntrans_list *)0 )
    {
      FirstTrans = VpnTrans;
    }
    else
    {
/*
**  Move all define assignment on the first transition
*/
      VpnAct  = VpnTrans->ACT;
      PrevAct = (vpnact_list *)0;

      while ( VpnAct != (vpnact_list *)0 )
      {
        DelAct = VpnAct;
        VpnAct = VpnAct->NEXT;

        if ( DelAct->TYPE == VPN_ACT_ASG_DEFINE )
        {
          DefAtom = DelAct->VEX_ATOM;
          VexExpr = DelAct->VEX_EXPR;

          NewAct  = addvpnactasgafter( VasyFigure, FirstTrans, PrevAct, DefAtom, VexExpr );
          PrevAct = NewAct;

          NewAct->LINE = DelAct->LINE;

          DelAct->VEX_ATOM = (vexexpr      *)0;
          DelAct->VEX_EXPR = (vexexpr      *)0;
          DelAct->LINE     = (vpnline_list *)0;

          delvpnact( VasyFigure, VpnTrans, DelAct );
        }
      }
    }

    if ( ! IsVasyRedInstGuardAtomic( VexGuard ) )
    {
      if ( IsVasyDebugLevel2() )
      {
        VasyPrintf( stdout, "  +++ Lateral Guard is not atomic, create define !\n" );
        viewvexexprboundln( VexGuard );
      }

      DefDeclar = VasyRedActAddVpnDefine( VasyFigure, VasyProcess, 1, VEX_TYPE_BOOLEAN );

      DefAtom = dupvexnode( DefDeclar->VEX_ATOM );
      VpnTrans->VEX_GUARD = dupvexnode( DefAtom );

      NewAct = addvpnactasgfirst( VasyFigure, FirstTrans, DefAtom, VexGuard );
      unionvpnline( VasyFigure, &NewAct->LINE, BeginPlace->LINE );

      if ( IsVasyDebugLevel2() )
      {
        viewvexexprboundln( VpnTrans->VEX_GUARD );
      }
    }
  }

  if ( IsVasyDebugLevel2() )
  {
    VasyDebugSaveVpnFig( VasyFigure );
  }

  FirstTrans = (vpntrans_list *)0;

  for ( ScanTrans  = BeginPlace->TRANS_OUT;
        ScanTrans != (chain_list *)0;
        ScanTrans  = ScanTrans->NEXT )
  {
    VpnArc   = GetVpnArc( ScanTrans );
    VpnTrans = GetVpnArcTargetTrans( VpnArc );

    VpnAct = VpnTrans->ACT;

    if ( FirstTrans == (vpntrans_list *)0 )
    {
      FirstTrans = VpnTrans;

      FirstTrans->ACT = (vpnact_list *)0;
      FirstTrans->LAST_ACT = &FirstTrans->ACT;
    }

    while ( VpnAct != (vpnact_list *)0 )
    {
      NewAct = VpnAct;
      VpnAct = VpnAct->NEXT;

      *FirstTrans->LAST_ACT =  NewAct;
      NewAct->PREV          =  FirstTrans->LAST_ACT;
      NewAct->NEXT          = (vpnact_list *)0;
      FirstTrans->LAST_ACT  = &NewAct->NEXT;

      if ( NewAct->TYPE != VPN_ACT_ASG_DEFINE )
      {
        AsgAtom   = NewAct->VEX_ATOM;
        VexDriver = (vexexpr *)0;
      
        AtomValue = GetVexAtomValue( AsgAtom );
        AsgDeclar = searchvpndeclall( VasyFigure, AtomValue );
        AsgSymbol = getvpnsymdecl( AsgDeclar, AsgAtom->LEFT );
        VexGuard  = dupvexexpr( VpnTrans->VEX_GUARD );

        NewAct->VEX_EXPR = optimvexbinexpr( VEX_IFT, AsgAtom->WIDTH, 
                                            VexGuard, NewAct->VEX_EXPR );

        for ( ScanTrans2  = BeginPlace->TRANS_OUT;
              ScanTrans2 != (chain_list *)0;
              ScanTrans2  = ScanTrans2->NEXT )
        {
          VpnArc2   = GetVpnArc( ScanTrans2 );
          VpnTrans2 = GetVpnArcTargetTrans( VpnArc2 );

          if ( VpnTrans2 == VpnTrans ) continue;

          if ( isvextypedivisible( AsgDeclar->BASE ) )
          {
            Element = searchauth2elem( VasyHash2BitVecAsg, (char*)VpnTrans2, (char*)AsgSymbol );
          }
          else
          {
            Element = searchauth2elem( VasyHash2Assign, (char*)VpnTrans2, AtomValue );
          }

          VexGuard = dupvexexpr( VpnTrans2->VEX_GUARD );

          if ( IsVasyDebugLevel2() )
          {
            VasyPrintf( stdout, "  +++ Guard of %s is ", VpnTrans2->NAME );
            viewvexexprboundln( VexGuard );
          }

          if ( Element == (auth2elem *)0 )
          {
            if ( VexDriver == (vexexpr *)0 )
            {
              VexDriver = VexGuard;
            }
            else
            {
              VexDriver = optimvexbinexpr( VEX_OR, 1, VexDriver, VexGuard );
            }
          }
          else
          {
            VpnAct2 = (vpnact_list *)Element->VALUE;
            VexExpr = dupvexexpr( VpnAct2->VEX_EXPR );
            VexExpr = optimvexbinexpr( VEX_IFT, AsgAtom->WIDTH, VexGuard, VexExpr );

            NewAct->VEX_EXPR = optimvexbinexpr( VEX_OR, AsgAtom->WIDTH,
                                                NewAct->VEX_EXPR, VexExpr );
            unionvpnline( VasyFigure, &NewAct->LINE, VpnAct2->LINE );

            delvpnact( VasyFigure, VpnTrans2, VpnAct2 );
          }
        }

        if ( VexDriver != (vexexpr *)0 )
        {
          VexExpr = dupvexexpr( AsgAtom );

          if ( ( NewAct->TYPE == VPN_ACT_ASG_PORT    ) ||
               ( NewAct->TYPE == VPN_ACT_ASG_SIGNAL  ) )
          {
            VexExpr = createvexunaryexpr( VEX_DRIVER, VexExpr->WIDTH, VexExpr );
            VexExpr->TYPE = AsgAtom->TYPE;

            SetVexNodeOper( VexExpr );
          }

          VexExpr = optimvexbinexpr( VEX_IFT, AsgAtom->WIDTH, VexDriver, VexExpr );

          NewAct->VEX_EXPR = optimvexbinexpr( VEX_OR, AsgAtom->WIDTH, 
                                              NewAct->VEX_EXPR, VexExpr );
        }

        if ( NewAct->TYPE == VPN_ACT_ASG_VARIABLE )
        {
          VexExpr   = NewAct->VEX_EXPR;
          DefDeclar = VasyRedActAddVpnDefine( VasyFigure, VasyProcess, 
                                             AsgAtom->WIDTH, AsgDeclar->BASE );
          DefAtom   = dupvexnode( DefDeclar->VEX_ATOM );
          NewAct->VEX_EXPR = dupvexnode( DefAtom );

          NewAct2 = addvpnactasgbefore( VasyFigure, FirstTrans, NewAct, DefAtom, VexExpr );
          unionvpnline( VasyFigure, &NewAct2->LINE, NewAct->LINE );
        }
      }
    }

    if ( VpnTrans != FirstTrans )
    {
      VpnTrans->ACT = (vpnact_list *)0;
      VpnTrans->LAST_ACT = &VpnTrans->ACT;
    }
  }

  if ( ! KeepGuard )
  {
    freevexexpr( FirstTrans->VEX_GUARD );
    FirstTrans->VEX_GUARD = (vexexpr *)0;
  }

  while ( BeginPlace->TRANS_OUT->NEXT != (chain_list *)0 )
  {
    VpnArc   = GetVpnArc( BeginPlace->TRANS_OUT->NEXT );
    VpnTrans = GetVpnArcTargetTrans( VpnArc );

    if ( KeepGuard )
    {
      FirstTrans->VEX_GUARD = optimvexbinexpr( VEX_OR, 1,
                                               FirstTrans->VEX_GUARD,
                                               VpnTrans->VEX_GUARD );
      VpnTrans->VEX_GUARD = (vexexpr *)0;
    }
    
    delvpntrans( VasyFigure, VpnTrans );
  }

  if ( KeepGuard )
  {
    if ( FirstTrans->ACT != (vpnact_list *)0 ) FirstTrans->TYPE = VPN_TRANS_ACT_GUARDED;
    else                                       FirstTrans->TYPE = VPN_TRANS_GUARDED;
  }
  else
  {
    if ( FirstTrans->ACT != (vpnact_list *)0 ) FirstTrans->TYPE = VPN_TRANS_ACT_EXEC;
    else                                       FirstTrans->TYPE = VPN_TRANS_IMMEDIATE;
  }

  resetauth2table( VasyHash2Assign    );
  resetauth2table( VasyHash2BitVecAsg );

  if ( IsVasyDebugLevel2() )
  {
    VasyDebugSaveVpnFig( VasyFigure );
    VasyPrintf( stdout, "  <-- VasyRedInstLateral\n" );
  }

  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasyRedInstLateralWait                  |
|                                                             |
\------------------------------------------------------------*/

int VasyRedInstLateralWait( VpnFigure, VpnProc, BeginPlace, EndPlace )

  vpnfig_list   *VpnFigure;
  vpnproc_list  *VpnProc;
  vpnplace_list *BeginPlace;
  vpnplace_list *EndPlace;
{
  VasyFigure  = VpnFigure;
  VasyProcess = VpnProc;

  return( VasyRedInstLateral( BeginPlace, EndPlace, 1 ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyRedInstCase                       |
|                                                             |
\------------------------------------------------------------*/

static void VasyRedInstCase( BeginPlace )

  vpnplace_list *BeginPlace;
{
  vpnplace_list *EndPlace;
  vpnplace_list *CasePlace;
  vpntrans_list *CaseTrans;
  vpntrans_list *EndTrans;
  vpnarc        *VpnArc;
  chain_list    *ScanChain;

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  --> VasyRedInstCase %s\n", BeginPlace->NAME );
  }

  EndPlace = BeginPlace->LINK;
  BeginPlace->LINK = (vpnplace_list *)0;

  SetVasyRedInstTag( EndPlace );

  VpnArc   = GetVpnArc( EndPlace->TRANS_OUT );
  EndTrans = GetVpnArcTargetTrans( VpnArc );

  for ( ScanChain  = BeginPlace->TRANS_OUT;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    VpnArc    = GetVpnArc( ScanChain );
    CaseTrans = GetVpnArcTargetTrans( VpnArc );
    VpnArc    = GetVpnArc( CaseTrans->PLACE_OUT );
    CasePlace = GetVpnArcTargetPlace( VpnArc );

    VasyPrevTrans = CaseTrans;

    VasyRedInstVpnPlace( CasePlace );

    VasyPrevTrans = VasyRedInstVertical( BeginPlace, VasyPrevTrans );
  }

  VasyPrevTrans = EndTrans;

  VasyRedInstGuarded( BeginPlace );
  VasyRedInstLateral( BeginPlace, EndPlace, 0 );

  ClearVasyRedInstTag( EndPlace );

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  <-- VasyRedInstCase\n" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyRedInstAsg                        |
|                                                             |
\------------------------------------------------------------*/

static void VasyRedInstAsg( BeginPlace )

  vpnplace_list *BeginPlace;
{
  vpnarc *VpnArc;

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  --> VasyRedInstAsg %s\n", BeginPlace->NAME );
  }

  VpnArc       = GetVpnArc( BeginPlace->TRANS_OUT );
  VasyPrevTrans = GetVpnArcTargetTrans( VpnArc );

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  <-- VasyRedInstAsg\n" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyRedInstProc                       |
|                                                             |
\------------------------------------------------------------*/

static void VasyRedInstProc( BeginPlace )

  vpnplace_list *BeginPlace;
{
  vpnarc *VpnArc;

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  --> VasyRedInstProc %s\n", BeginPlace->NAME );
  }

  VpnArc       = GetVpnArc( BeginPlace->TRANS_OUT );
  VasyPrevTrans = GetVpnArcTargetTrans( VpnArc );

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  <-- VasyRedInstProc\n" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyRedInstWait                       |
|                                                             |
\------------------------------------------------------------*/

static void VasyRedInstWait( BeginPlace )

  vpnplace_list *BeginPlace;
{
  vpnplace_list *EndPlace;
  vpnarc        *VpnArc;

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  --> VasyRedInstWait %s\n", BeginPlace->NAME );
  }

  EndPlace = BeginPlace->LINK;
  BeginPlace->LINK = (vpnplace_list *)0;

  if ( EndPlace == (vpnplace_list *)0 )
  {
    VpnArc       = GetVpnArc( BeginPlace->TRANS_OUT );
    VasyPrevTrans = GetVpnArcTargetTrans( VpnArc );
  }
  else
  {
    VpnArc       = GetVpnArc( EndPlace->TRANS_OUT );
    VasyPrevTrans = GetVpnArcTargetTrans( VpnArc );
  }
    
  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  <-- VasyRedInstWait\n" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyRedInstExit                       |
|                                                             |
\------------------------------------------------------------*/

static int VasyRedInstExit( BeginPlace )

  vpnplace_list *BeginPlace;
{
  vpnplace_list *EndPlace;
  vpnarc        *VpnArc;

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  --> VasyRedInstExit %s\n", BeginPlace->NAME );
  }

  EndPlace = BeginPlace->LINK;
  BeginPlace->LINK = (vpnplace_list *)0;

  if ( EndPlace == (vpnplace_list *)0 )
  {
    VpnArc       = GetVpnArc( BeginPlace->TRANS_OUT );
    VasyPrevTrans = GetVpnArcTargetTrans( VpnArc );
  }
  else
  {
    VpnArc       = GetVpnArc( EndPlace->TRANS_OUT );
    VasyPrevTrans = GetVpnArcTargetTrans( VpnArc );
  }

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  <-- VasyRedInstExit\n" );
  }

  return( EndPlace == (vpnplace_list *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyRedInstNext                       |
|                                                             |
\------------------------------------------------------------*/

static int VasyRedInstNext( BeginPlace )

  vpnplace_list *BeginPlace;
{
  vpnplace_list *EndPlace;
  vpnarc        *VpnArc;

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  --> VasyRedInstNext %s\n", BeginPlace->NAME );
  }

  EndPlace = BeginPlace->LINK;
  BeginPlace->LINK = (vpnplace_list *)0;

  if ( EndPlace == (vpnplace_list *)0 )
  {
    VpnArc       = GetVpnArc( BeginPlace->TRANS_OUT );
    VasyPrevTrans = GetVpnArcTargetTrans( VpnArc );
  }
  else
  {
    VpnArc       = GetVpnArc( EndPlace->TRANS_OUT );
    VasyPrevTrans = GetVpnArcTargetTrans( VpnArc );
  }

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  <-- VasyRedInstNext\n" );
  }

  return( EndPlace == (vpnplace_list *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyRedInstLoop                       |
|                                                             |
\------------------------------------------------------------*/

static void VasyRedInstLoop( BeginPlace )

  vpnplace_list *BeginPlace;
{
  vpntrans_list *LoopTrans;
  vpnplace_list *LoopPlace;
  vpnplace_list *EndPlace;
  vpnarc        *VpnArc;

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  --> VasyRedInstLoop %s\n", BeginPlace->NAME );
  }

  EndPlace = BeginPlace->LINK;
  BeginPlace->LINK = (vpnplace_list *)0;

  if ( EndPlace != (vpnplace_list *)0 )
  {
    SetVasyRedInstTag( BeginPlace );
    SetVasyRedInstTag( EndPlace   );

    VpnArc    = GetVpnArc( BeginPlace->TRANS_OUT );
    LoopTrans = GetVpnArcTargetTrans( VpnArc );
    VpnArc    = GetVpnArc( LoopTrans->PLACE_OUT );
    LoopPlace = GetVpnArcTargetPlace( VpnArc );

    VasyPrevTrans = LoopTrans;

    VasyRedInstVpnPlace( LoopPlace );

    VasyPrevTrans = VasyRedInstVertical( LoopPlace, VasyPrevTrans );
    ClearVasyRedInstTag( EndPlace   );

    VpnArc       = GetVpnArc( EndPlace->TRANS_OUT );
    VasyPrevTrans = GetVpnArcTargetTrans( VpnArc );
  }
  else
  {
    SetVasyRedInstTag( BeginPlace );

    VpnArc       = GetVpnArc( BeginPlace->TRANS_OUT );
    VasyPrevTrans = GetVpnArcTargetTrans( VpnArc );
    EndPlace     = BeginPlace;
  }

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  <-- VasyRedInstLoop %s\n", EndPlace->NAME );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyRedInstWhile                      |
|                                                             |
\------------------------------------------------------------*/

static void VasyRedInstWhile( BeginPlace )

  vpnplace_list *BeginPlace;
{
  vpnplace_list *TestPlace;
  vpnplace_list *EndPlace;
  vpnplace_list *CasePlace;
  vpntrans_list *CaseTrans;
  vpntrans_list *TestTrans;
  vpnarc        *VpnArc;
  chain_list    *ScanChain;

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  --> VasyRedInstWhile %s\n", BeginPlace->NAME );
  }

  EndPlace = BeginPlace->LINK;
  BeginPlace->LINK = (vpnplace_list *)0;

  SetVasyRedInstTag( BeginPlace );
  SetVasyRedInstTag( EndPlace   );

  VpnArc    = GetVpnArc( BeginPlace->TRANS_OUT );
  TestTrans = GetVpnArcTargetTrans( VpnArc );
  VpnArc    = GetVpnArc( TestTrans->PLACE_OUT );
  TestPlace = GetVpnArcTargetPlace( VpnArc );

  for ( ScanChain  = TestPlace->TRANS_OUT;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    VpnArc    = GetVpnArc( ScanChain );
    CaseTrans = GetVpnArcTargetTrans( VpnArc );
    VpnArc    = GetVpnArc( CaseTrans->PLACE_OUT );
    CasePlace = GetVpnArcTargetPlace( VpnArc );

    VasyPrevTrans = CaseTrans;

    VasyRedInstVpnPlace( CasePlace );

    VasyPrevTrans = VasyRedInstVertical( TestPlace, VasyPrevTrans );
  }

  ClearVasyRedInstTag( EndPlace   );

  VpnArc       = GetVpnArc( EndPlace->TRANS_OUT );
  VasyPrevTrans = GetVpnArcTargetTrans( VpnArc );

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  <-- VasyRedInstWhile %s\n", EndPlace->NAME );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      VasyRedInstFor                         |
|                                                             |
\------------------------------------------------------------*/

static void VasyRedInstFor( BeginPlace )

  vpnplace_list *BeginPlace;
{
  vpnplace_list *TestPlace;
  vpnplace_list *LoopPlace;
  vpnplace_list *EndPlace;
  vpnplace_list *IncPlace;
  vpnplace_list *CasePlace;
  vpntrans_list *CaseTrans;
  vpntrans_list *SkipTrans;
  vpnarc        *VpnArc;
  chain_list    *ScanChain;

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  --> VasyRedInstFor %s\n", BeginPlace->NAME );
  }

  EndPlace = BeginPlace->LINK;
  BeginPlace->LINK = (vpnplace_list *)0;

  SetVasyRedInstTag( BeginPlace );
  SetVasyRedInstTag( EndPlace   );

  VpnArc    = GetVpnArc( BeginPlace->TRANS_OUT );
  SkipTrans = GetVpnArcTargetTrans( VpnArc );
  VpnArc    = GetVpnArc( SkipTrans->PLACE_OUT );
  LoopPlace = GetVpnArcTargetPlace( VpnArc );

  SetVasyRedInstTag( LoopPlace  );
  IncPlace = LoopPlace->LINK;
  IncPlace->LINK = (vpnplace_list *)0;

  if ( IncPlace != (vpnplace_list *)0 )
  {
    SetVasyRedInstTag( IncPlace );
  }

  VpnArc    = GetVpnArc( LoopPlace->TRANS_OUT );
  SkipTrans = GetVpnArcTargetTrans( VpnArc );
  VpnArc    = GetVpnArc( SkipTrans->PLACE_OUT );
  TestPlace = GetVpnArcTargetPlace( VpnArc );

  for ( ScanChain  = TestPlace->TRANS_OUT;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    VpnArc    = GetVpnArc( ScanChain );
    CaseTrans = GetVpnArcTargetTrans( VpnArc );
    VpnArc    = GetVpnArc( CaseTrans->PLACE_OUT );
    CasePlace = GetVpnArcTargetPlace( VpnArc );

    VasyPrevTrans = CaseTrans;

    VasyRedInstVpnPlace( CasePlace );

    VasyPrevTrans = VasyRedInstVertical( TestPlace, VasyPrevTrans );
  }

  if ( IncPlace != (vpnplace_list *)0 )
  {
    ClearVasyRedInstTag( IncPlace );
  }

  ClearVasyRedInstTag( LoopPlace  );
  ClearVasyRedInstTag( EndPlace   );

  VpnArc       = GetVpnArc( EndPlace->TRANS_OUT );
  VasyPrevTrans = GetVpnArcTargetTrans( VpnArc );

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  <-- VasyRedInstFor %s\n", EndPlace->NAME );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyRedInstVpnPlace                   |
|                                                             |
\------------------------------------------------------------*/

static void VasyRedInstVpnPlace( BeginPlace )

  vpnplace_list *BeginPlace;
{
  vpnplace_list *ScanPlace;
  vpnplace_list *NextPlace;
  vpnarc        *VpnArc;
  vpntrans_list *VpnTrans;
  short          Stop;

  ScanPlace = BeginPlace;

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "--> VasyRedInstVpnPlace %s\n", BeginPlace->NAME );
  }

  Stop = 0;

  while ( ! IsVasyRedInstTag( ScanPlace ) )
  {
    if ( ScanPlace->LINK != (vpnplace_list *)0 ) NextPlace = ScanPlace->LINK;
    else                                         NextPlace = ScanPlace;

    VpnArc    = GetVpnArc( NextPlace->TRANS_OUT );
    VpnTrans  = GetVpnArcTargetTrans( VpnArc );
/*
    if ( VpnTrans->PLACE_OUT == (chain_list *)0 ) break;
*/
    VpnArc    = GetVpnArc( VpnTrans->PLACE_OUT );
    NextPlace = GetVpnArcTargetPlace( VpnArc );

    if ( ScanPlace->TYPE == VPN_PLACE_ASSIGN )
    {
      VasyRedInstAsg( ScanPlace );
    }
    else
    if ( ( ScanPlace->TYPE == VPN_PLACE_IF   ) ||
         ( ScanPlace->TYPE == VPN_PLACE_CASE ) )
    {
      VasyRedInstCase( ScanPlace );
    }
    else
    if ( ScanPlace->TYPE == VPN_PLACE_WHILE )
    {
      VasyRedInstWhile( ScanPlace );
    }
    else
    if ( ScanPlace->TYPE == VPN_PLACE_FOR )
    {
      VasyRedInstFor( ScanPlace );
    }
    else
    if ( ScanPlace->TYPE == VPN_PLACE_LOOP )
    {
      VasyRedInstLoop( ScanPlace );
    }
    else
    if ( ScanPlace->TYPE == VPN_PLACE_WAIT )
    {
      VasyRedInstWait( ScanPlace );
    }
    else
    if ( ScanPlace->TYPE == VPN_PLACE_EXIT )
    {
      Stop = VasyRedInstExit( ScanPlace );
    }
    else
    if ( ScanPlace->TYPE == VPN_PLACE_NEXT )
    {
      Stop = VasyRedInstNext( ScanPlace );
    }
    else
    if ( ScanPlace->TYPE == VPN_PLACE_PROCESS )
    {
      VasyRedInstProc( ScanPlace );
    }
    else
/*\
    if ( ( ScanPlace->TYPE == VPN_PLACE_END_IF    ) ||
         ( ScanPlace->TYPE == VPN_PLACE_END_CASE  ) ||
         ( ScanPlace->TYPE == VPN_PLACE_END_FOR   ) ||
         ( ScanPlace->TYPE == VPN_PLACE_END_LOOP  ) ||
         ( ScanPlace->TYPE == VPN_PLACE_END_WHILE ) )
\*/
    {
      VasyPrintf( stdout, "  +++ Should not happen %s!!!\n",
                 VPN_PLACE_TYPE[ ScanPlace->TYPE ] );
      autexit( 1 );
    }
/*\
    else
    {
      if ( IsVasyDebugLevel2() )
      {
        VasyPrintf( stdout, "  +++ Skip %s %s !!!\n", 
                 ScanPlace->NAME, VPN_PLACE_TYPE[ ScanPlace->TYPE ] );
      }
    }
\*/

    VasyPrevTrans = VasyRedInstVertical( BeginPlace, VasyPrevTrans );

    if ( Stop ) break;

    ScanPlace = NextPlace;
  }

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "<-- VasyRedInstVpnPlace %s %d\n", BeginPlace->NAME, Stop );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    VasyRedInstStartVpnProc                  |
|                                                             |
\------------------------------------------------------------*/

static void VasyRedInstStartVpnProc( VpnFigure, VpnProc )

  vpnfig_list  *VpnFigure;
  vpnproc_list *VpnProc;
{
  vpntrans_list *BeginTrans;
  vpnplace_list *ScanPlace;
  vpnplace_list *BeginPlace;
  vpnarc        *VpnArc;

  BeginTrans = VpnProc->FIRST;
  VpnArc     = GetVpnArc( BeginTrans->PLACE_IN );
  BeginPlace = GetVpnArcSourcePlace( VpnArc );

  VpnProc->FIRST = VpnProc->ELABO;
  VpnArc     = GetVpnArc( BeginTrans->PLACE_OUT );
  ScanPlace  = GetVpnArcTargetPlace( VpnArc );

  VasyFigure    = VpnFigure;
  VasyProcess   = VpnProc;
  VasyPrevTrans = BeginTrans;

  SetVasyRedInstTag( BeginPlace );
  VasyRedInstVpnPlace( ScanPlace );

  VasyPrevTrans = VasyRedInstVertical( BeginPlace, VasyPrevTrans );
}

/*------------------------------------------------------------\
|                                                             |
|                 VasyRedInstImmediateVpnProc                 |
|                                                             |
\------------------------------------------------------------*/

static void VasyRedInstImmediateVpnProc( VpnFigure, VpnProc )

  vpnfig_list  *VpnFigure;
  vpnproc_list *VpnProc;
{
  vpntrans_list *VpnTrans;
  vpnplace_list *PrevPlace;
  vpnplace_list *NextPlace;
  vpntrans_list *PrevTrans;
  vpntrans_list *DelTrans;
  chain_list    *ScanTrans;
  vpnarc        *VpnArc;

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "  --> VasyRedInstImmediateVpnProc\n" );
  }

  VpnTrans = VpnProc->TRANS;

  while ( VpnTrans != (vpntrans_list *)0 )
  {
    if ( ( VpnTrans->TYPE == VPN_TRANS_IMMEDIATE ) &&
         ( VpnTrans->NUMBER_IN  == 1             ) &&
         ( VpnTrans->NUMBER_OUT == 1             ) )
    {
      VpnArc    = GetVpnArc( VpnTrans->PLACE_IN );
      PrevPlace = GetVpnArcSourcePlace( VpnArc );
      VpnArc    = GetVpnArc( VpnTrans->PLACE_OUT );
      NextPlace = GetVpnArcTargetPlace( VpnArc );

      DelTrans = VpnTrans;
      VpnTrans = VpnTrans->NEXT;

      if ( PrevPlace->NUMBER_OUT == 1 )
      {
        delvpntrans( VpnFigure, DelTrans );

        for ( ScanTrans  = PrevPlace->TRANS_IN;
              ScanTrans != (chain_list *)0;
              ScanTrans  = ScanTrans->NEXT )
        {
          VpnArc    = GetVpnArc( ScanTrans );
          PrevTrans = GetVpnArcSourceTrans( VpnArc );

          addvpnarctrans( VpnFigure, PrevTrans, NextPlace );
        }

        delvpnplace( VpnFigure, PrevPlace );
      }
    }
    else
    {
      VpnTrans = VpnTrans->NEXT;
    }
  }

  if ( IsVasyDebugLevel1() )
  {
    VasyDebugSaveVpnFig( VpnFigure );

    VasyPrintf( stdout, "  <-- VasyRedInstImmediateVpnProc\n" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    VasyRedInstLocalVpnProc                  |
|                                                             |
\------------------------------------------------------------*/

static void VasyRedInstLocalVpnProc( VpnFigure, VpnProc )

   vpnfig_list  *VpnFigure;
   vpnproc_list *VpnProc;
{
  vpnplace_list *ScanPlace;
  vpnplace_list *NextPlace;
  vpnplace_list *PrevPlace;
  vpnplace_list *EndPlace;
  vpntrans_list *ScanTrans;
  vpntrans_list *PrevTrans;
  vpntrans_list *NewTrans;
  vpnarc        *VpnArc;
  chain_list    *ScanChain;
  int            Modified;

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "--> VasyRedInstLocalVpnProc %s\n", VpnProc->NAME );
    VasyDebugSaveVpnFig( VasyFigure );
  }

  VasyFigure    = VpnFigure;
  VasyProcess   = VpnProc;

  Modified = 1;

  while ( Modified )
  {
    Modified = 0;

    ScanPlace = VpnProc->PLACE;

    while ( ScanPlace != (vpnplace_list *)0 )
    {
      NextPlace = ScanPlace->NEXT;

      if ( ScanPlace->NUMBER_OUT > 1 )
      {
        VpnArc    = GetVpnArc( ScanPlace->TRANS_OUT );
        ScanTrans = GetVpnArcTargetTrans( VpnArc );
        VpnArc    = GetVpnArc( ScanTrans->PLACE_OUT );
        EndPlace  = GetVpnArcTargetPlace( VpnArc );

        if ( VasyRedInstLateral( ScanPlace, EndPlace, 0 ) )
        {
          Modified = 1;
        }
      }

      if ( ( ScanPlace->NUMBER_IN  == 1 ) &&
           ( ScanPlace->NUMBER_OUT == 1 ) )
      {
        VpnArc    = GetVpnArc( ScanPlace->TRANS_OUT );
        ScanTrans = GetVpnArcTargetTrans( VpnArc );
        VpnArc    = GetVpnArc( ScanPlace->TRANS_IN );
        PrevTrans = GetVpnArcSourceTrans( VpnArc );

        VpnArc    = GetVpnArc( PrevTrans->PLACE_IN );
        PrevPlace = GetVpnArcSourcePlace( VpnArc );
        NewTrans  = VasyRedInstVertical( PrevPlace, ScanTrans );

        if ( NewTrans != ScanTrans )
        {
          Modified = 1;
        }
      }
      else
      if ( ScanPlace->NUMBER_IN == 0 )
      {
        ScanChain = ScanPlace->TRANS_OUT;

        while ( ScanChain != (chain_list *)0 )
        {
          VpnArc    = GetVpnArc( ScanChain );
          ScanChain = ScanChain->NEXT;
          ScanTrans = GetVpnArcTargetTrans( VpnArc );

          delvpntrans( VpnFigure, ScanTrans );
        }

        delvpnplace( VpnFigure, ScanPlace );

        Modified = 1;
      }

      ScanPlace = NextPlace; 
    }

    if ( Modified && IsVasyDebugLevel1() )
    {
      VasyDebugSaveVpnFig( VasyFigure );
    }
  }

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "<-- VasyRedInstLocalVpnProc %s\n", VpnProc->NAME );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyRedInstVpnProc                    |
|                                                             |
\------------------------------------------------------------*/

void VasyRedInstVpnProc( VpnFigure, VpnProc )

  vpnfig_list  *VpnFigure;
  vpnproc_list *VpnProc;
{
  vpntrans_list *ScanTrans;
  vpnplace_list *ScanPlace;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  --> VasyRedInstVpnProc %s\n", VpnProc->NAME );
    VasyDebugSaveVpnFig( VpnFigure );
  }

/*
**  Only one assigment to each symbols for all actions
*/
  for ( ScanTrans  = VpnProc->TRANS;
        ScanTrans != (vpntrans_list *)0;
        ScanTrans  = ScanTrans->NEXT )
  {
    VasyRedActVpnTrans( VpnFigure, VpnProc, ScanTrans, 0 );
  }

  if ( IsVasyDebugLevel1() )
  {
    VasyDebugSaveVpnFig( VpnFigure );
  }
/*
**  Reduction on all instruction
*/
  if ( ! IsVasyDebugNoRedInst() )
  {
    VasyRedInstStartVpnProc( VpnFigure, VpnProc ); 
  }
/*
**  Sets all place's type to unknown !
*/
  for ( ScanPlace  = VpnProc->PLACE;
        ScanPlace != (vpnplace_list *)0;
        ScanPlace  = ScanPlace->NEXT )
  {
    ScanPlace->TYPE = VPN_PLACE_UNKNOWN;
    ScanPlace->LINK = (vpnplace_list *)0;
    ClearVasyRedInstTag( ScanPlace );
  }

  VasyRedInstImmediateVpnProc( VpnFigure, VpnProc );
/*
**  Apply a local reduction, this is the last chance !
*/
  if ( ! IsVasyDebugNoRedInst() )
  {
    VasyRedInstLocalVpnProc( VpnFigure, VpnProc );
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyDebugSaveVpnFig( VpnFigure );
    VasyPrintf( stdout, "  <-- VasyRedInstVpnProc %s\n", VpnProc->NAME );
  }
}
