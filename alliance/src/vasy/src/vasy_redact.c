/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
| File    :                  vasy_redact.c                    |
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

  static vpnfig_list   *VasyFigure   = (vpnfig_list   *)0;
  static vpnproc_list  *VasyProcess  = (vpnproc_list  *)0;

  static authtable     *VasyHashAssign    = (authtable  *)0;
  static authtable     *VasyHashBitVecAsg = (authtable  *)0;

  static authtable     *VasyHashVariable  = (authtable  *)0;
  static authtable     *VasyHashBitVecVar = (authtable  *)0;

  static authtable     *VasyHashDriver    = (authtable  *)0;
  static authtable     *VasyHashBitVecDrv = (authtable  *)0;

  static vpntrans_list *VasySubstTrans = (vpntrans_list *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    VasyRedActAddVpnDefine                   |
|                                                             |
\------------------------------------------------------------*/

vpndecl_list *VasyRedActAddVpnDefine( VpnFigure, VpnProc, Width, DefBase )

  vpnfig_list    *VpnFigure;
  vpnproc_list   *VpnProc;
  unsigned short  Width;
  unsigned char   DefBase;
{
  vpndecl_list *DefDeclar;
  vexexpr      *DefAtom;
  char         *FatherName;
  char          Buffer[ 128 ];
  unsigned char NewBase;
  long          Number;

  FatherName = VpnProc->NAME;
  Number     = GetVpnNumDeclDef( VpnFigure );

  sprintf( Buffer, "%s_reddef_%ld", FatherName, Number );

  if ( ( DefBase == VEX_TYPE_REG_BIT ) ||
       ( DefBase == VEX_TYPE_MUX_BIT ) ||
       ( DefBase == VEX_TYPE_WOR_BIT ) )
  {
    DefBase = VEX_TYPE_BIT;
  }
  else
  if ( ( DefBase == VEX_TYPE_REG_VECTOR ) ||
       ( DefBase == VEX_TYPE_MUX_VECTOR ) ||
       ( DefBase == VEX_TYPE_WOR_VECTOR ) )
  {
    DefBase = VEX_TYPE_BIT_VECTOR;
  }

  NewBase = getvextypescalar( DefBase );

  if ( ( isvextypevector( DefBase ) ) &&
       ( ( Width   >  1           ) ||
         ( NewBase == DefBase     ) ) )
  {
    DefAtom = createvexatomvec( Buffer, Width - 1, 0 );
  }
  else
  {
    DefAtom = createvexatombit( Buffer );
    DefBase = NewBase;
  }

  DefDeclar = addvpndeclprocdef( VpnFigure, VpnProc, DefAtom, DefBase );

  return( DefDeclar );
}

/*------------------------------------------------------------\
|                                                             |
|                      VasyRedActSubstVexAtom                 |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyRedActSubstVexAtom( Expr, SubstDriver )

  vexexpr   *Expr;
  short      SubstDriver;
{
  authtable    *HashBitVec;
  char         *AtomValue;
  char         *DefName;
  authelem     *Element;
  vexexpr      *SubstExpr;
  vexexpr      *DefAtom;
  vexexpr      *AsgAtom;
  vexexpr      *AsgExpr;
  vpnact_list  *ScanAsg;
  vpnact_list  *NewAct;
  vpndecl_list *ScanDeclar;
  vpndecl_list *DefDeclar;
  vpnsym       *ScanSym;
  int           DefIndex;
  int           DefPos;
  int           Scan;
  int           Index;
  int           Width;
  int           Step;

  SubstExpr = (vexexpr *)0;
  AtomValue = GetVexAtomValue( Expr );

  if ( SubstDriver )
  {
    Element    = searchauthelem( VasyHashDriver, AtomValue );
    HashBitVec = VasyHashBitVecDrv;
  }
  else
  {
    Element    = searchauthelem( VasyHashVariable, AtomValue );
    HashBitVec = VasyHashBitVecVar;
  }

  ScanDeclar = searchvpndeclall( VasyFigure, AtomValue );
/*
**  Check if symbol should be substituted
*/
  if ( Element != (authelem *)0 )
  {
    if ( ! isvextypedivisible( ScanDeclar->BASE ) )
    {
/*
**  Symbol is not a bit_vector, just replace vex
*/
      ScanAsg = (vpnact_list *)Element->VALUE;

      freevexexpr( Expr );
      Expr = dupvexexpr( ScanAsg->VEX_EXPR );
    }
    else
    {
/*
**  Symbol is a bit_vector, make a concatenation
*/
      if ( Expr->LEFT > Expr->RIGHT ) Step = -1;
      else                            Step =  1;
      
      Index = Expr->LEFT;
      Width = Expr->WIDTH;
      
      if ( Width > 1 ) SubstExpr = createvexoper( VEX_CONCAT, Width );
      
      for ( Scan = 0; Scan < Width; Scan++ )
      {
        ScanSym = getvpnsymdecl( ScanDeclar, Index );
        Element = searchauthelem( HashBitVec, (char *)ScanSym );
      
        if ( Element != (authelem *)0 )
        {
          ScanAsg = (vpnact_list *)Element->VALUE;
          AsgExpr = ScanAsg->VEX_EXPR;
          AsgAtom = ScanAsg->VEX_ATOM;

          if ( ( ! IsVexNodeAtom( AsgExpr )   ) ||
               (  IsVexAtomLiteral( AsgExpr ) ) )
          {
            if ( IsVasyDebugLevel1() )
            {
              VasyPrintf( stdout, "  +++ Substitute assign is not atomic, create define !\n" );
            }

            DefDeclar = VasyRedActAddVpnDefine( VasyFigure, VasyProcess,
                                               AsgAtom->WIDTH, ScanDeclar->BASE );

            DefAtom = dupvexnode( DefDeclar->VEX_ATOM );
            ScanAsg->VEX_EXPR = dupvexnode( DefAtom );

            NewAct = addvpnactasgbefore( VasyFigure, VasySubstTrans, ScanAsg, DefAtom, AsgExpr );
            unionvpnline( VasyFigure, &NewAct->LINE, ScanAsg->LINE );

            AsgExpr = ScanAsg->VEX_EXPR;
          }

          DefName  = GetVexAtomValue( AsgExpr );
          DefPos   = getvexvectorpos( AsgAtom, Index );
          DefIndex = getvexvectorindex( AsgExpr, DefPos );
      
          DefAtom  = createvexatomvec( DefName, DefIndex, DefIndex );
        }
        else
        {
          DefAtom = createvexatomvec( AtomValue, Index, Index );
        }
      
        if ( Width == 1 ) SubstExpr = DefAtom;
        else              addvexqexpr( SubstExpr, DefAtom );
      
        Index += Step;
      }
      
      freevexexpr( Expr );
      Expr = simpvexexpr( SubstExpr );
    }

    if ( IsVasyDebugLevel2() )
    {
      VasyPrintf( stdout, "  --> VasyRedActSubstVexAtom\n" );
      VasyPrintf( stdout, "  +++ Replace %s by ", AtomValue );
      viewvexexprboundln( Expr );
      VasyPrintf( stdout, "  <-- VasyRedActSubstVexAtom\n" );
    }
  }
  else
  {
    Expr = (vexexpr *)0;
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                      VasyRedActSubstVex                     |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyRedActSubstVex( Expr )

  vexexpr *Expr;
{
  chain_list *ScanOper;
  vexexpr    *SubstExpr;
  long        Oper;

  if ( IsVexNodeAtom( Expr ) )
  {
    if ( ! IsVexAtomLiteral( Expr ) )
    {
      SubstExpr = VasyRedActSubstVexAtom( Expr, 0 );

      if ( SubstExpr != (vexexpr *)0 ) Expr = SubstExpr;
    }

    return( Expr );
  }
  else
  if ( IsVexNodeOper( Expr ) )
  {
    Oper = GetVexOperValue( Expr );

    if ( Oper == VEX_DRIVER )
    {
      SubstExpr = GetVexOperand( Expr->OPERAND );
      SubstExpr = VasyRedActSubstVexAtom( SubstExpr, 1 );

      if ( SubstExpr != (vexexpr *)0 )
      {
        Expr->OPERAND->DATA = (void *)0;
        freevexexpr( Expr );

        Expr = SubstExpr;
      }

      return( Expr );
    }
  }

  for ( ScanOper  = Expr->OPERAND;
        ScanOper != (chain_list *)0;
        ScanOper  = ScanOper->NEXT )
  {
    SubstExpr = VasyRedActSubstVex( GetVexOperand( ScanOper ) );
    SetVexOperand( ScanOper, SubstExpr );
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                      VasyRedActSubstVpnAct                  |
|                                                             |
\------------------------------------------------------------*/

static void VasyRedActSubstVpnAct( VpnFigure, VpnProc, VpnTrans, VpnAct )

  vpnfig_list   *VpnFigure;
  vpnproc_list  *VpnProc;
  vpntrans_list *VpnTrans;
  vpnact_list   *VpnAct;
{
  VasyFigure     = VpnFigure;
  VasyProcess    = VpnProc;
  VasySubstTrans = VpnTrans;

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  --> VasyRedActSubstVpnAct\n" );
    VasyPrintf( stdout, "  +++ " );
    viewvexexprboundln( VpnAct->VEX_EXPR );
  }

  VpnAct->VEX_EXPR = VasyRedActSubstVex( VpnAct->VEX_EXPR );

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  +++ " );
    viewvexexprboundln( VpnAct->VEX_EXPR );
    VasyPrintf( stdout, "  <-- VasyRedActSubstVpnAct\n" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyRedActVpnTrans                    |
|                                                             |
\------------------------------------------------------------*/

void VasyRedActVpnTrans( VpnFigure, VpnProc, VpnTrans, SubstDriver )

  vpnfig_list   *VpnFigure;
  vpnproc_list  *VpnProc;
  vpntrans_list *VpnTrans;
  short          SubstDriver;
{
  vpnact_list   *VpnAct;
  vpnact_list   *DelAct;
  vpnact_list   *NewAct;
  unsigned char  AsgVar;
  unsigned char  AsgDriver;
  vpndecl_list  *AsgDeclar;
  vpnsym        *AsgSymbol;
  vpnsym        *DelSymbol;
  vexexpr       *AsgAtom;
  vexexpr       *AsgExpr;
  vpndecl_list  *DefDeclar;
  vexexpr       *DefAtom;
  vexexpr       *DelAtom;
  vexexpr       *DelExpr;
  vexexpr       *DelAtom2;
  vexexpr       *DelExpr2;
  authelem      *Element;
  char          *AtomValue;
  int            AsgIndex;
  int            AsgMin;
  int            AsgMax;
  int            DelMin;
  int            DelMax;
  int            DelPos;
  int            DelIndex;
/*
**  Reduction for all assign's action in a transition
*/
  if ( VpnTrans->ACT == (vpnact_list *)0 ) return;

  if ( VasyHashAssign == (authtable *)0 )
  {
    VasyHashAssign    = createauthtable( 100 );
    VasyHashBitVecAsg = createauthtable( 100 );

    VasyHashVariable  = createauthtable( 100 );
    VasyHashBitVecVar = createauthtable( 100 );

    VasyHashDriver    = createauthtable( 100 );
    VasyHashBitVecDrv = createauthtable( 100 );
  }

  VpnAct = VpnTrans->ACT;

  while ( VpnAct != (vpnact_list *)0 )
  {
    VasyRedActSubstVpnAct( VpnFigure, VpnProc, VpnTrans, VpnAct );
/*
**  Assign statement
*/
    if ( VpnAct->TYPE == VPN_ACT_ASG_VARIABLE )
    {
      AsgVar    = 1;
      AsgDriver = 0;
    }
    else
    {
      AsgVar = 0;

      if ( (             SubstDriver                  ) &&
           ( ( VpnAct->TYPE == VPN_ACT_ASG_PORT    ) ||
             ( VpnAct->TYPE == VPN_ACT_ASG_SIGNAL  ) ) ) AsgDriver = 1;
      else                                               AsgDriver = 0;
    }

    AsgAtom   = VpnAct->VEX_ATOM;
    AsgExpr   = VpnAct->VEX_EXPR;

    if ( ! IsVexNodeAtom( AsgAtom ) )
    {
      VasyErrorLine( VASY_ILLEGAL_ASSIGN_ERROR, VpnAct->LINE, getvexatomname( AsgAtom ) );
    }

    AtomValue = GetVexAtomValue( AsgAtom );
    AsgDeclar = searchvpndeclall( VpnFigure, AtomValue );

    AsgMin = getvexvectormin( AsgAtom );
    AsgMax = getvexvectormax( AsgAtom );
/*
**  Delete action if S <= S --> there is nothing more to do !
*/
    if ( isvexequalexpr( AsgAtom, AsgExpr ) )
    {
      if ( IsVasyDebugLevel1() )
      {
        VasyPrintf( stdout, "  +++ %s <= %s just delete and continue !\n", AtomValue, AtomValue );
      }

      DelAct = VpnAct;
      VpnAct = VpnAct->NEXT;

      delvpnact( VpnFigure, VpnTrans, DelAct );

      continue;
    }
/*
**  Variable Assign statement
*/
    if ( ( AsgVar ) && ( ! SubstDriver ) )
    {
/*
**  Modify assign V := f --> Define #= f, V := Define 
*/
      if ( IsVasyDebugLevel1() )
      {
        VasyPrintf( stdout, "  +++ First Variable assignment %s := define !\n", AtomValue );
      }

      DefDeclar = VasyRedActAddVpnDefine( VpnFigure, VpnProc,
                                         AsgAtom->WIDTH, AsgDeclar->BASE );

      DefAtom = dupvexnode( DefDeclar->VEX_ATOM );
      VpnAct->VEX_EXPR = dupvexnode( DefAtom );

      NewAct = addvpnactasgbefore( VpnFigure, VpnTrans, VpnAct, DefAtom, AsgExpr );
      unionvpnline( VpnFigure, &NewAct->LINE, VpnAct->LINE );
    }
/*
**  Variable or Signal Assign statement
*/
    Element = searchauthelem( VasyHashAssign, AtomValue );

    if ( Element != (authelem *)0 )
    {
      if ( IsVasyDebugLevel1() )
      {
        VasyPrintf( stdout, "  +++ previous assignment to %s !\n", AtomValue );
      }

      if ( ! isvextypedivisible( AsgDeclar->BASE ) )
      {
/*
**  Delete previous assign to symbol V 
*/
        if ( IsVasyDebugLevel1() )
        {
          VasyPrintf( stdout, "  +++ just delete previous assignment to %s !\n", AtomValue );
        }

        DelAct = (vpnact_list *)Element->VALUE;
        delvpnact( VpnFigure, VpnTrans, DelAct );
      }
      else
      {
/*
**  Delete previous assign to bit_vector symbol V 
*/
        for ( AsgIndex = AsgMin; AsgIndex <= AsgMax; AsgIndex++ )
        {
          AsgSymbol = getvpnsymdecl( AsgDeclar, AsgIndex );
          Element   = searchauthelem( VasyHashBitVecAsg, (char *)AsgSymbol );
/*
**  Check if there is a previous assign to symbol V
*/
          if ( Element != (authelem *)0 )
          {
            if ( IsVasyDebugLevel1() )
            {
              VasyPrintf( stdout, "  +++ Previous bit_vector assign to %s %d\n",
                       AsgSymbol->NAME, AsgIndex );
            }

            DelAct  = (vpnact_list *)Element->VALUE;
            DelAtom = DelAct->VEX_ATOM;

            DelMin = getvexvectormin( DelAtom );
            DelMax = getvexvectormax( DelAtom );

            if ( IsVasyDebugLevel1() )
            {
              VasyPrintf( stdout, "  +++ Check instersection [%d %d] -> [%d %d]\n",
                       AsgMin, AsgMax, DelMin, DelMax );
            }
/*
**  Check the kind of range's intersection 
*/
            if ( ( DelMin >= AsgMin ) &&
                 ( DelMax <= AsgMax ) )
            {
/*
**       [            ]   Asg       Completely included
**       [<-[      ]->]   Del
*/
              if ( IsVasyDebugLevel1() )
              {
                VasyPrintf( stdout, "  +++ Intersection completely included just delete\n" );
              }

              AsgIndex = DelMax;
              delvpnact( VpnFigure, VpnTrans, DelAct );
            }
            else
            {
              DelAtom = DelAct->VEX_ATOM;
              DelExpr = DelAct->VEX_EXPR;
/*
**  Previous assign to symbol V must be an atomic expression !
*/
              if ( ( ! IsVexNodeAtom( DelExpr )    ) ||
                   (   IsVexAtomLiteral( DelExpr ) ) )
              {
                if ( IsVasyDebugLevel1() )
                {
                  VasyPrintf( stdout, "  +++ Previous assign is not atomic, create define !\n" );
                }

                DefDeclar = VasyRedActAddVpnDefine( VpnFigure, VpnProc,
                                                    DelAtom->WIDTH, AsgDeclar->BASE );
                DefAtom = dupvexnode( DefDeclar->VEX_ATOM );
                DelAct->VEX_EXPR = dupvexnode( DefAtom );

                NewAct = addvpnactasgbefore( VpnFigure, VpnTrans, DelAct, DefAtom, DelExpr );
                unionvpnline( VpnFigure, &NewAct->LINE, DelAct->LINE );

                DelExpr = DelAct->VEX_EXPR;
              }
                
              if ( ( DelMin <  AsgMin ) &&
                   ( DelMax <= AsgMax ) )
              {
/*
**           [        ]   Asg      Partialy included on left
**       [<-[]------->]   Del
*/
                if ( IsVasyDebugLevel1() )
                {
                  VasyPrintf( stdout, "  +++ Intersection partialy on left\n" );
                }

                if ( IsVexAtomDown( DelAtom ) )
                {
                  DelPos   = getvexvectorpos( DelAtom, AsgMin - 1 );
                  DelIndex = getvexvectorindex( DelExpr, DelPos );

                  slicevexatomvec( DelExpr, DelIndex  , DelExpr->RIGHT );
                  slicevexatomvec( DelAtom, AsgMin - 1, DelAtom->RIGHT );
                }
                else
                {
                  DelPos   = getvexvectorpos( DelAtom, AsgMin - 1 );
                  DelIndex = getvexvectorindex( DelExpr, DelPos );

                  slicevexatomvec( DelExpr, DelExpr->LEFT, DelIndex   );
                  slicevexatomvec( DelAtom, DelAtom->LEFT, AsgMin - 1 );
                }

                if ( IsVasyDebugLevel1() )
                {
                  VasyPrintf( stdout, "  +++ New previous assign \n" );
                  viewvexexprbound( DelAtom );
                  fprintf( stdout, " <= " );
                  viewvexexprboundln( DelExpr );
                }
              }
              else
              if ( ( DelMax >  AsgMax ) &&
                   ( DelMin >= AsgMin ) )
              {
/*
**       [        ]       Asg      Partialy included on right
**       [<-------[]->]   Del
*/
                if ( IsVasyDebugLevel1() )
                {
                  VasyPrintf( stdout, "  +++ Intersection partialy on right\n" );
                }

                if ( IsVexAtomDown( DelAtom ) )
                {
                  DelPos   = getvexvectorpos( DelAtom, AsgMax + 1 );
                  DelIndex = getvexvectorindex( DelExpr, DelPos );

                  slicevexatomvec( DelExpr, DelExpr->LEFT, DelIndex   );
                  slicevexatomvec( DelAtom, DelAtom->LEFT, AsgMax + 1 );
                }
                else
                {
                  DelPos   = getvexvectorpos( DelAtom, AsgMax + 1 );
                  DelIndex = getvexvectorindex( DelExpr, DelPos );

                  slicevexatomvec( DelExpr, DelIndex  , DelExpr->RIGHT );
                  slicevexatomvec( DelAtom, AsgMax + 1, DelAtom->RIGHT );
                }

                if ( IsVasyDebugLevel1() )
                {
                  VasyPrintf( stdout, "  +++ New previous assign \n" );
                  viewvexexprbound( DelAtom );
                  fprintf( stdout, " <= " );
                  viewvexexprboundln( DelExpr );
                }
              }
              else
              {
/*
**           [    ]       Asg      Partialy included on the two sides
**       [<-[]----[]->]   Del
*/
                if ( IsVasyDebugLevel1() )
                {
                  VasyPrintf( stdout, "  +++ Intersection partialy on two sides\n" );
                }

                DelAtom2 = dupvexnode( DelAtom );
                DelExpr2 = dupvexnode( DelExpr );

                if ( IsVexAtomDown( DelAtom ) )
                {
                  DelPos   = getvexvectorpos( DelAtom, AsgMin - 1 );
                  DelIndex = getvexvectorindex( DelExpr, DelPos );

                  slicevexatomvec( DelExpr, DelIndex  , DelExpr->RIGHT );
                  slicevexatomvec( DelAtom, AsgMin - 1, DelAtom->RIGHT );
                }
                else
                {
                  DelPos   = getvexvectorpos( DelAtom, AsgMin - 1 );
                  DelIndex = getvexvectorindex( DelExpr, DelPos );

                  slicevexatomvec( DelExpr, DelExpr->LEFT, DelIndex   );
                  slicevexatomvec( DelAtom, DelAtom->LEFT, AsgMin - 1 );
                }

                if ( IsVasyDebugLevel1() )
                {
                  VasyPrintf( stdout, "  +++ New previous assign \n" );
                  viewvexexprbound( DelAtom );
                  fprintf( stdout, " <= " );
                  viewvexexprboundln( DelExpr );
                }

                if ( IsVexAtomDown( DelAtom ) )
                {
                  DelPos   = getvexvectorpos( DelAtom2, AsgMax + 1 );
                  DelIndex = getvexvectorindex( DelExpr2, DelPos );

                  slicevexatomvec( DelExpr2, DelExpr2->LEFT, DelIndex   );
                  slicevexatomvec( DelAtom2, DelAtom2->LEFT, AsgMax + 1 );
                }
                else
                {
                  DelPos   = getvexvectorpos( DelAtom2, AsgMax + 1 );
                  DelIndex = getvexvectorindex( DelExpr2, DelPos );

                  slicevexatomvec( DelExpr2, DelIndex  , DelExpr2->RIGHT );
                  slicevexatomvec( DelAtom2, AsgMax + 1, DelAtom2->RIGHT );
                }

                NewAct = addvpnactasgbefore( VpnFigure, VpnTrans, 
                                             DelAct, DelAtom2, DelExpr2 );
                unionvpnline( VpnFigure, &NewAct->LINE, DelAct->LINE );
                DelAct = NewAct;

                if ( IsVasyDebugLevel1() )
                {
                  VasyPrintf( stdout, "  +++ New added previous assign \n" );
                  viewvexexprbound( DelAtom2 );
                  fprintf( stdout, " <= " );
                  viewvexexprboundln( DelExpr2 );
                }

                DelMin = getvexvectormin( DelAtom2 );
                DelMax = getvexvectormax( DelAtom2 );
/*
**  Add modified assign to bit_vector symbol V in hash table
*/
                for ( DelIndex = DelMin; DelIndex <= DelMax; DelIndex++ )
                {
                  DelSymbol = getvpnsymdecl( AsgDeclar, DelIndex );
                  addauthelem( VasyHashBitVecAsg, (char *)DelSymbol, (long)DelAct );

                  if ( AsgVar )
                  {
                    addauthelem( VasyHashBitVecVar, (char *)DelSymbol, (long)DelAct );
                  }
                  else
                  if ( AsgDriver )
                  {
                    addauthelem( VasyHashBitVecDrv, (char *)DelSymbol, (long)DelAct );
                  }
                }
              }

              AsgIndex = AsgMax;
            }
          }
        }
      }
    }
/*
**  Add assign to bit_vector symbol V in hash table
*/
    if ( isvextypedivisible( AsgDeclar->BASE ) )
    {
      for ( AsgIndex = AsgMin; AsgIndex <= AsgMax; AsgIndex++ )
      {
        AsgSymbol = getvpnsymdecl( AsgDeclar, AsgIndex );
        addauthelem( VasyHashBitVecAsg, (char *)AsgSymbol, (long)VpnAct );

        if ( AsgVar )
        {
          addauthelem( VasyHashBitVecVar, (char *)AsgSymbol, (long)VpnAct );
        }
        else
        if ( AsgDriver )
        {
          addauthelem( VasyHashBitVecDrv, (char *)AsgSymbol, (long)VpnAct );
        }
      }
    }
/*
**  Add assign to symbol V in hash table
*/
    addauthelem( VasyHashAssign, AtomValue, (long)VpnAct );
/*
**  Add assign to variable V in hash table
*/
    if ( AsgVar )
    {
      addauthelem( VasyHashVariable, AtomValue, (long)VpnAct );
    }
    else
    if ( AsgDriver )
    {
      addauthelem( VasyHashDriver, AtomValue, (long)VpnAct );
    }

    VpnAct = VpnAct->NEXT;
  }

  resetauthtable( VasyHashAssign    );
  resetauthtable( VasyHashBitVecAsg );

  resetauthtable( VasyHashVariable  );
  resetauthtable( VasyHashBitVecVar );

  resetauthtable( VasyHashDriver    );
  resetauthtable( VasyHashBitVecDrv );
}
