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
| File    :                  vasy_synth.c                     |
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
# include "abl.h"
# include "vex.h"
# include "bdd.h"
# include "vpn.h"
# include "vtl.h"
# include "rtn.h"

# include "vasy_error.h"
# include "vasy_debug.h"
# include "vasy_support.h"
# include "vasy_shared.h"
# include "vasy_synth.h"
# include "vasy_simprtl.h"

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

  static rtlfig_list *VasyFigure     = (rtlfig_list *)0;
  static rtlasg_list *VasyAsg        = (rtlasg_list *)0;
  static authtable   *VasyHashRtlAsg = (authtable   *)0;
  static authtable   *VasyHashOutAsg = (authtable   *)0;
  static long         VasyNumberDef  = 0;

  static short VasyOperatorSupported[ VEX_MAX_OPERATOR ] =
  {
    1, /* VEX_CONCAT          */
    1, /* VEX_NOT             */
    1, /* VEX_NEG             */
    1, /* VEX_EVENT           */
    1, /* VEX_OR              */
    1, /* VEX_AND             */
    1, /* VEX_XOR             */
    1, /* VEX_NOR             */
    1, /* VEX_NAND            */
    1, /* VEX_NXOR            */
    1, /* VEX_EQ              */
    1, /* VEX_NE              */
    1, /* VEX_LT              */
    1, /* VEX_LE              */
    1, /* VEX_GT              */
    1, /* VEX_GE              */
    1, /* VEX_ADD             */
    1, /* VEX_SUB             */
    0, /* VEX_MUL             */
    0, /* VEX_DIV             */
    0, /* VEX_EXP             */
    1, /* VEX_MOD             */
    0, /* VEX_REM             */
    0, /* VEX_TO              */
    0, /* VEX_DOWNTO          */
    0, /* VEX_INDEX           */
    0, /* VEX_LEFT            */
    0, /* VEX_RIGHT           */
    0, /* VEX_LOW             */
    0, /* VEX_HIGH            */
    0, /* VEX_LENGTH          */
    0, /* VEX_RANGE           */
    0, /* VEX_REV_RANGE       */
    0, /* VEX_DRIVER          */
    1, /* VEX_IFT             */
    0, /* VEX_ARRAY           */
    0, /* VEX_INDEX_N         */
    0, /* VEX_OTHERS          */
    0, /* VEX_NUM_BIT         */
    1, /* VEX_ABS             */
    0, /* AF                  */
    0, /* AG                  */
    0, /* AX                  */
    0, /* AU                  */
    0, /* EF                  */
    0, /* EG                  */
    0, /* EX                  */
    0, /* EU                  */
    0, /* EQUIV               */
    0  /* IMPLY               */ 
  };

  static char VasyBuffer[ 512 ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  static void VasySimplifySubstAsg();

/*------------------------------------------------------------\
|                                                             |
|                  VasySimplifyHashRegisterAsg                |
|                                                             |
\------------------------------------------------------------*/

static void VasySimplifyHashRegisterAsg( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtldecl_list  *RtlDeclar;
  rtlsym        *RtlSymbol;
  rtlasg_list   *RtlAsg;
  vexexpr       *VexAtom;
  char          *AtomName;
  int            VexMin;
  int            VexMax;
  int            VexIndex;

  if ( VasyHashRtlAsg == (authtable *)0 )
  {
    VasyHashRtlAsg = createauthtable( 1000 );
  }
  else
  {
    resetauthtable( VasyHashRtlAsg );
  }

  for ( RtlAsg  = RtlFigure->ASSIGN;
        RtlAsg != (rtlasg_list *)0;
        RtlAsg  = RtlAsg->NEXT )
  {
    VexAtom   = RtlAsg->VEX_ATOM;
    AtomName  = GetVexAtomValue( VexAtom );
    RtlDeclar = searchrtldecl( RtlFigure, AtomName );

    VexMin   = getvexvectormin( VexAtom );
    VexMax   = getvexvectormax( VexAtom );

    if ( RtlAsg->TYPE == RTL_ASG_REGISTER )
    {
      for ( VexIndex = VexMin; VexIndex <= VexMax; VexIndex++ )
      {
        RtlSymbol = getrtlsymdecl( RtlDeclar, VexIndex );
        addauthelem( VasyHashRtlAsg, (char *)RtlSymbol, (long)RtlAsg );
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySimplifyCompactRegister             |
|                                                             |
\------------------------------------------------------------*/

static void VasySimplifyCompactRegister( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtlasg_list   *RtlAsg1;
  rtlasg_list   *RtlAsg2;
  rtlbivex_list *ScanBiVex1;
  rtlbivex_list *ScanBiVex2;
  rtldecl_list  *RtlDeclar;
  rtlsym        *RtlSymbol;
  vexexpr       *AsgAtom1;
  vexexpr       *AsgAtom2;
  vexexpr       *VexData1;
  vexexpr       *VexData2;
  vexexpr       *DeclAtom;
  char          *AtomName;
  authelem      *Element;
  int            AsgMax;
  int            DeclMax;
  int            VexMin;
  int            VexMax;
  int            VexWidth;
  int            VexIndex;
  int            Concat;

  RtlAsg1 = RtlFigure->ASSIGN;

  while ( RtlAsg1 != (rtlasg_list *)0 )
  {
    if ( RtlAsg1->TYPE != RTL_ASG_REGISTER )
    {
      RtlAsg1 = RtlAsg1->NEXT; continue;
    }

    AsgAtom1 = RtlAsg1->VEX_ATOM;
    AsgMax   = getvexvectormax( AsgAtom1 );

    AtomName  = GetVexAtomValue( AsgAtom1 );
    RtlDeclar = searchrtldecl( RtlFigure, AtomName );

    DeclAtom = RtlDeclar->VEX_ATOM;
    DeclMax  = getvexvectormax( DeclAtom );

    if ( AsgMax >= DeclMax )
    {
      RtlAsg1 = RtlAsg1->NEXT; continue;
    }

    RtlSymbol = getrtlsymdecl( RtlDeclar, AsgMax + 1 );
    Element   = searchauthelem( VasyHashRtlAsg, (char *)RtlSymbol );

    if ( Element == (authelem *)0 )
    {
      RtlAsg1 = RtlAsg1->NEXT; continue;
    }

    RtlAsg2 = (rtlasg_list *)Element->VALUE;

    if ( RtlAsg1->REG_TYPE != RtlAsg2->REG_TYPE )
    {
      RtlAsg1 = RtlAsg1->NEXT; continue;
    }

    Concat = 1;

    for ( ScanBiVex1 = RtlAsg1->BIVEX, 
          ScanBiVex2 = RtlAsg2->BIVEX;
          ( ( ScanBiVex1 != (rtlbivex_list *)0 ) &&
            ( ScanBiVex2 != (rtlbivex_list *)0 ) );
          ScanBiVex1 = ScanBiVex1->NEXT,
          ScanBiVex2 = ScanBiVex2->NEXT )
    {
      if ( ScanBiVex1->TYPE != ScanBiVex2->TYPE )
      {
        Concat = 0; break;
      }
      else
      if ( ( ScanBiVex1->VEX_COND != (vexexpr *)0 ) &&
           ( ScanBiVex2->VEX_COND != (vexexpr *)0 ) )
      {
        if ( ! isvexequalexpr( ScanBiVex1->VEX_COND, ScanBiVex2->VEX_COND ) )
        {
          Concat = 0; break;
        }
      }
      else
      if ( ScanBiVex1->VEX_COND != ScanBiVex2->VEX_COND )
      {
        Concat = 0; break;
      }
    }

    if ( ! Concat )
    {
      RtlAsg1 = RtlAsg1->NEXT; continue;
    }

    if ( IsVasyDebugLevel0() )
    {
      VasyPrintf( stdout, "Concat " );
      viewvexexpr( RtlAsg1->VEX_ATOM );
      fprintf( stdout, " &  " );
      viewvexexprln( RtlAsg2->VEX_ATOM );
    }

    AsgAtom2 = RtlAsg2->VEX_ATOM;

    if ( IsVexAtomUp( DeclAtom ) )
    {
      AsgAtom1 = createvexatomvec( AtomName, AsgAtom1->LEFT, AsgAtom2->RIGHT );
    }
    else
    {
      AsgAtom1 = createvexatomvec( AtomName, AsgAtom2->LEFT, AsgAtom1->RIGHT );
    }

    freevexexpr( RtlAsg1->VEX_ATOM );
    RtlAsg1->VEX_ATOM = AsgAtom1;

    for ( ScanBiVex1 = RtlAsg1->BIVEX, 
          ScanBiVex2 = RtlAsg2->BIVEX;
          ( ( ScanBiVex1 != (rtlbivex_list *)0 ) &&
            ( ScanBiVex2 != (rtlbivex_list *)0 ) );
          ScanBiVex1 = ScanBiVex1->NEXT,
          ScanBiVex2 = ScanBiVex2->NEXT )
    {
      VexData1 = ScanBiVex1->VEX_DATA;

      if ( VexData1 != (vexexpr *)0 )
      {
        VexData2 = dupvexexpr( ScanBiVex2->VEX_DATA );
        VexWidth = VexData1->WIDTH + VexData2->WIDTH;

        if ( IsVexAtomUp( DeclAtom ) )
        {
          VexData1 = optimvexbinexpr( VEX_CONCAT, VexWidth, VexData1, VexData2 );
        }
        else
        {
          VexData1 = optimvexbinexpr( VEX_CONCAT, VexWidth, VexData2, VexData1 );
        }

        ScanBiVex1->VEX_DATA = simpvexexpr( VexData1 );
      }
    }

    VexMin = getvexvectormin( AsgAtom1 );
    VexMax = getvexvectormax( AsgAtom1 );

    for ( VexIndex = VexMin; VexIndex <= VexMax; VexIndex++ )
    {
      RtlSymbol = getrtlsymdecl( RtlDeclar, VexIndex );
      addauthelem( VasyHashRtlAsg, (char *)RtlSymbol, (long)RtlAsg1 );
    }

    delrtlasg( RtlFigure, RtlAsg2 );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                  VasySimplifyViewHashTristateAsg            |
|                                                             |
\------------------------------------------------------------*/

static void VasySimplifyViewHashTristateAsg( Element )

  authelem *Element;
{
  rtlasg_list *RtlAsg;
  chain_list  *ScanChain;
  rtlsym      *RtlSymbol;

  RtlSymbol = (rtlsym     *)Element->KEY;
  ScanChain = (chain_list *)Element->VALUE;

  fprintf( stdout, "RtlSymbol %s[%d]\n", RtlSymbol->NAME, RtlSymbol->INDEX );

  while ( ScanChain != (chain_list *)0 )
  {
    RtlAsg = (rtlasg_list *)ScanChain->DATA;
    fprintf( stdout, "RtlAsg %lx\n", (long)RtlAsg );
    ScanChain = ScanChain->NEXT;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                  VasySimplifyHashTristateAsg                |
|                                                             |
\------------------------------------------------------------*/

static void VasySimplifyHashTristateAsg( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtldecl_list  *RtlDeclar;
  rtlsym        *RtlSymbol;
  rtlasg_list   *RtlAsg;
  authelem      *Element;
  chain_list    *HeadChain;
  vexexpr       *VexAtom;
  char          *AtomName;
  int            VexMin;
  int            VexMax;
  int            VexIndex;

  if ( VasyHashRtlAsg == (authtable *)0 )
  {
    VasyHashRtlAsg = createauthtable( 1000 );
  }
  else
  {
    resetauthtable( VasyHashRtlAsg );
  }

  for ( RtlAsg  = RtlFigure->ASSIGN;
        RtlAsg != (rtlasg_list *)0;
        RtlAsg  = RtlAsg->NEXT )
  {
    VexAtom   = RtlAsg->VEX_ATOM;
    AtomName  = GetVexAtomValue( VexAtom );
    RtlDeclar = searchrtldecl( RtlFigure, AtomName );

    VexMin   = getvexvectormin( VexAtom );
    VexMax   = getvexvectormax( VexAtom );

    if ( RtlAsg->TYPE == RTL_ASG_TRISTATE )
    {
      for ( VexIndex = VexMin; VexIndex <= VexMax; VexIndex++ )
      {
        RtlSymbol = getrtlsymdecl( RtlDeclar, VexIndex );
        Element   = searchauthelem( VasyHashRtlAsg, (char *)RtlSymbol );

        if ( Element == (authelem *)0 ) HeadChain = (chain_list *)0;
        else                            HeadChain = (chain_list *)Element->VALUE;

        HeadChain = addchain( HeadChain, (void *)RtlAsg );
        addauthelem( VasyHashRtlAsg, (char *)RtlSymbol, (long)HeadChain );
      }
    }
  }

  if ( IsVasyDebugLevel1() )
  {
    viewauthtable( VasyHashRtlAsg, VasySimplifyViewHashTristateAsg );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                  VasySimplifyFreeHashTristateAsg            |
|                                                             |
\------------------------------------------------------------*/

static void VasySimplifyFreeHashTristateAsg( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtldecl_list  *RtlDeclar;
  rtlsym        *RtlSymbol;
  rtlasg_list   *RtlAsg;
  vexexpr       *VexAtom;
  char          *AtomName;
  authelem      *Element;
  chain_list    *HeadChain;
  int            VexMin;
  int            VexMax;
  int            VexIndex;

  for ( RtlAsg  = RtlFigure->ASSIGN;
        RtlAsg != (rtlasg_list *)0;
        RtlAsg  = RtlAsg->NEXT )
  {
    VexAtom   = RtlAsg->VEX_ATOM;
    AtomName  = GetVexAtomValue( VexAtom );
    RtlDeclar = searchrtldecl( RtlFigure, AtomName );

    VexMin   = getvexvectormin( VexAtom );
    VexMax   = getvexvectormax( VexAtom );

    if ( RtlAsg->TYPE == RTL_ASG_TRISTATE )
    {
      for ( VexIndex = VexMin; VexIndex <= VexMax; VexIndex++ )
      {
        RtlSymbol = getrtlsymdecl( RtlDeclar, VexIndex );
        Element   = searchauthelem( VasyHashRtlAsg, (char *)RtlSymbol );

        if ( Element != (authelem *)0 )
        {
          HeadChain = (chain_list *)Element->VALUE;

          if ( HeadChain != (chain_list *)0 )
          {
            Element->VALUE = 0;
            freechain( HeadChain );
          }
        }
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySimplifyCompactTristate             |
|                                                             |
\------------------------------------------------------------*/

static void VasySimplifyCompactTristate( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtlasg_list   *RtlAsg1;
  rtlasg_list   *RtlAsg2;
  rtlbivex_list *ScanBiVex1;
  rtlbivex_list *ScanBiVex2;
  rtldecl_list  *RtlDeclar;
  rtlsym        *RtlSymbol;
  vexexpr       *AsgAtom1;
  vexexpr       *AsgAtom2;
  vexexpr       *VexData1;
  vexexpr       *VexData2;
  vexexpr       *DeclAtom;
  char          *AtomName;
  chain_list    *ScanChain;
  chain_list    *ReplChain;
  authelem      *Element;
  int            AsgMax;
  int            DeclMax;
  int            VexMin;
  int            VexMax;
  int            VexWidth;
  int            VexIndex;
  int            Concat;

  RtlAsg1 = RtlFigure->ASSIGN;

  while ( RtlAsg1 != (rtlasg_list *)0 )
  {
    if ( RtlAsg1->TYPE != RTL_ASG_TRISTATE )
    {
      RtlAsg1  = RtlAsg1->NEXT; continue;
    }

    AsgAtom1 = RtlAsg1->VEX_ATOM;
    AsgMax   = getvexvectormax( AsgAtom1 );

    AtomName  = GetVexAtomValue( AsgAtom1 );
    RtlDeclar = searchrtldecl( VasyFigure, AtomName );

    DeclAtom = RtlDeclar->VEX_ATOM;
    DeclMax  = getvexvectormax( DeclAtom );

    if ( AsgMax >= DeclMax )
    {
      RtlAsg1  = RtlAsg1->NEXT; continue;
    }

    RtlSymbol = getrtlsymdecl( RtlDeclar, AsgMax + 1 );
    Element   = searchauthelem( VasyHashRtlAsg, (char *)RtlSymbol );

    if ( Element == (authelem *)0 )
    {
      RtlAsg1  = RtlAsg1->NEXT; continue;
    }

    Concat = 0;

    for ( ScanChain  = (chain_list *)Element->VALUE;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      RtlAsg2 = (rtlasg_list *)ScanChain->DATA;
      Concat  = 1;
  
      for ( ScanBiVex1 = RtlAsg1->BIVEX, 
            ScanBiVex2 = RtlAsg2->BIVEX;
            ( ( ScanBiVex1 != (rtlbivex_list *)0 ) &&
              ( ScanBiVex2 != (rtlbivex_list *)0 ) );
            ScanBiVex1 = ScanBiVex1->NEXT,
            ScanBiVex2 = ScanBiVex2->NEXT )
      {
        if ( ScanBiVex1->TYPE != ScanBiVex2->TYPE )
        {
          Concat = 0; break;
        }
        else
        if ( ( ScanBiVex1->VEX_COND != (vexexpr *)0 ) &&
             ( ScanBiVex2->VEX_COND != (vexexpr *)0 ) )
        {
          if ( ! isvexequalexpr( ScanBiVex1->VEX_COND, ScanBiVex2->VEX_COND ) )
          {
            Concat = 0; break;
          }
        }
        else
        if ( ScanBiVex1->VEX_COND != ScanBiVex2->VEX_COND )
        {
          Concat = 0; break;
        }
      }
  
      if ( ! Concat ) continue;
  
      if ( IsVasyDebugLevel0() )
      {
        VasyPrintf( stdout, "Concat " );
        viewvexexpr( RtlAsg1->VEX_ATOM );
        fprintf( stdout, " &  " );
        viewvexexprln( RtlAsg2->VEX_ATOM );
      }
  
      AsgAtom2 = RtlAsg2->VEX_ATOM;
  
      if ( IsVexAtomUp( DeclAtom ) )
      {
        AsgAtom1 = createvexatomvec( AtomName, AsgAtom1->LEFT, AsgAtom2->RIGHT );
      }
      else
      {
        AsgAtom1 = createvexatomvec( AtomName, AsgAtom2->LEFT, AsgAtom1->RIGHT );
      }
  
      freevexexpr( RtlAsg1->VEX_ATOM );
      RtlAsg1->VEX_ATOM = AsgAtom1;
  
      for ( ScanBiVex1 = RtlAsg1->BIVEX, 
            ScanBiVex2 = RtlAsg2->BIVEX;
            ( ( ScanBiVex1 != (rtlbivex_list *)0 ) &&
              ( ScanBiVex2 != (rtlbivex_list *)0 ) );
            ScanBiVex1 = ScanBiVex1->NEXT,
            ScanBiVex2 = ScanBiVex2->NEXT )
      {
        VexData1 = ScanBiVex1->VEX_DATA;
  
        if ( VexData1 != (vexexpr *)0 )
        {
          VexData2 = dupvexexpr( ScanBiVex2->VEX_DATA );
          VexWidth = VexData1->WIDTH + VexData2->WIDTH;
  
          if ( IsVexAtomUp( DeclAtom ) )
          {
            VexData1 = optimvexbinexpr( VEX_CONCAT, VexWidth, VexData1, VexData2 );
          }
          else
          {
            VexData1 = optimvexbinexpr( VEX_CONCAT, VexWidth, VexData2, VexData1 );
          }
  
          ScanBiVex1->VEX_DATA = simpvexexpr( VexData1 );
        }
      }
  
      VexMin = getvexvectormin( AsgAtom1 );
      VexMax = getvexvectormax( AsgAtom1 );
  
      for ( VexIndex = VexMin; VexIndex <= VexMax; VexIndex++ )
      {
        RtlSymbol = getrtlsymdecl( RtlDeclar, VexIndex );
        Element   = searchauthelem( VasyHashRtlAsg, (char *)RtlSymbol );

        for ( ReplChain  = (chain_list *)Element->VALUE;
              ReplChain != (chain_list *)0;
              ReplChain  = ReplChain->NEXT )
        {
          if ( ReplChain->DATA == (void *)RtlAsg2 )
          {
            ReplChain->DATA = RtlAsg1; break;
          }
        }
      }
  
      delrtlasg( RtlFigure, RtlAsg2 );

      break;
    }

    if ( ! Concat )
    {
      RtlAsg1  = RtlAsg1->NEXT;
    }
  }

  VasySimplifyFreeHashTristateAsg( RtlFigure );
}

/*------------------------------------------------------------\
|                                                             |
|                  VasySimplifyDisplayRead                    |
|                                                             |
\------------------------------------------------------------*/

static void VasySimplifyDisplayRead( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtldecl_list *RtlDeclar;
  rtlsym       *RtlSymbol;
  vexexpr      *VexAtom;
  int           VexIndex;
  int           Type;

  for ( Type = 0; Type < RTL_MAX_DECLAR_TYPE; Type++ )
  {
    for ( RtlDeclar  = RtlFigure->DECLAR[ Type ];
          RtlDeclar != (rtldecl_list *)0;
          RtlDeclar  = RtlDeclar->NEXT )
    {
      VasyPrintf( stdout, " %s --> Asg: %ld, Read: %ld\n", 
                  RtlDeclar->DECL_SYM->NAME, 
                  IsVasyRtlDeclarAsg( RtlDeclar ),
                  IsVasyRtlDeclarRead( RtlDeclar ) );

      VexAtom = RtlDeclar->VEX_ATOM;

      for ( VexIndex = 0; VexIndex < VexAtom->WIDTH; VexIndex++ )
      {
        RtlSymbol = &RtlDeclar->DECL_SYM[ VexIndex ];

        VasyPrintf( stdout, " > %s[%d]\n +++ Asg: %ld, Read: %ld\n", 
                    RtlSymbol->NAME, RtlSymbol->INDEX,
                    IsVasyRtlDeclarAsg( RtlSymbol ),
                    IsVasyRtlDeclarRead( RtlSymbol ) );
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                  VasySimplifyDisplaySubst                   |
|                                                             |
\------------------------------------------------------------*/

static void VasySimplifyDisplaySubst( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtlasg_list *RtlAsg;

  for ( RtlAsg  = RtlFigure->ASSIGN;
        RtlAsg != (rtlasg_list *)0;
        RtlAsg  = RtlAsg->NEXT )
  {
    VasyPrintf( stdout, "NRead %ld ", GetVasyRtlAssignNumberRead( RtlAsg ) );
    viewvexexprln( RtlAsg->VEX_ATOM );

    if ( IsVasyRtlAssignSubst( RtlAsg ) )
    {
      VasyPrintf( stdout, "  +++ Should substitute\n" );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       VasySimplifyMarkReadVex               |
|                                                             |
\------------------------------------------------------------*/

static void VasySimplifyMarkReadVex( Expr )

   vexexpr *Expr;
{
  rtldecl_list *RtlDeclar;
  rtlsym       *RtlSymbol;
  chain_list   *ScanOper;
  char         *AtomName;
  int           VexMin;
  int           VexMax;
  int           VexIndex;

  if ( Expr == (vexexpr *)0 ) return;

  if ( IsVexNodeAtom( Expr ) )
  {
    if ( IsVexAtomLiteral( Expr ) ) return;

    AtomName = GetVexAtomValue( Expr );
    VexMin   = getvexvectormin( Expr );
    VexMax   = getvexvectormax( Expr );

    RtlDeclar = searchrtldecl( VasyFigure, AtomName );
    SetVasyRtlDeclarRead( RtlDeclar );

    for ( VexIndex = VexMin; VexIndex <= VexMax; VexIndex++ )
    {
      RtlSymbol = getrtlsymdecl( RtlDeclar, VexIndex );
      SetVasyRtlDeclarRead( RtlSymbol );
    }
  }
  else
  {
    for ( ScanOper  = Expr->OPERAND;
          ScanOper != (chain_list *)0;
          ScanOper  = ScanOper->NEXT )
    {
      VasySimplifyMarkReadVex( GetVexOperand( ScanOper ) );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       VasySimplifyMarkWriteVex              |
|                                                             |
\------------------------------------------------------------*/

static void VasySimplifyMarkWriteVex( Expr )

   vexexpr *Expr;
{
  rtldecl_list *RtlDeclar;
  rtlsym       *RtlSymbol;
  chain_list   *ScanOper;
  char         *AtomName;
  int           VexMin;
  int           VexMax;
  int           VexIndex;

  if ( Expr == (vexexpr *)0 ) return;

  if ( IsVexNodeAtom( Expr ) )
  {
    if ( IsVexAtomLiteral( Expr ) ) return;

    AtomName = GetVexAtomValue( Expr );
    VexMin   = getvexvectormin( Expr );
    VexMax   = getvexvectormax( Expr );

    RtlDeclar = searchrtldecl( VasyFigure, AtomName );
    SetVasyRtlDeclarAsg( RtlDeclar );

    for ( VexIndex = VexMin; VexIndex <= VexMax; VexIndex++ )
    {
      RtlSymbol = getrtlsymdecl( RtlDeclar, VexIndex );
      SetVasyRtlDeclarAsg( RtlSymbol );
    }
  }
  else
  {
    for ( ScanOper  = Expr->OPERAND;
          ScanOper != (chain_list *)0;
          ScanOper  = ScanOper->NEXT )
    {
      VasySimplifyMarkWriteVex( GetVexOperand( ScanOper ) );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySimplifyMarkAsgVex                  |
|                                                             |
\------------------------------------------------------------*/

static void VasySimplifyMarkAsgVex( Expr )

   vexexpr *Expr;
{
  rtldecl_list *RtlDeclar;
  rtlsym       *RtlSymbol;
  rtlasg_list  *RtlAsg;
  vexexpr      *AsgAtom;
  vexexpr      *AsgData;
  chain_list   *ScanOper;
  char         *AtomName;
  authelem     *Element;
  int           VexMin;
  int           VexMax;
  int           VexIndex;

  if ( Expr == (vexexpr *)0 ) return;

  if ( IsVexNodeAtom( Expr ) )
  {
    if ( IsVexAtomLiteral( Expr ) ) return;

    AtomName  = GetVexAtomValue( Expr );
    RtlDeclar = searchrtldecl( VasyFigure, AtomName );

    VexMin = getvexvectormin( Expr );
    VexMax = getvexvectormax( Expr );

    RtlSymbol = getrtlsymdecl( RtlDeclar, VexMin ); 
    Element   = searchauthelem( VasyHashRtlAsg, (char *)RtlSymbol );

    if ( Element != (authelem *)0 )
    {
      RtlAsg  = (rtlasg_list *)Element->VALUE;
      AsgAtom = RtlAsg->VEX_ATOM;
      AsgData = RtlAsg->VEX_DATA;

      IncVasyRtlAssignNumberRead( RtlAsg );

      if ( ( AsgAtom->LEFT  == Expr->LEFT  ) &&
           ( AsgAtom->RIGHT == Expr->RIGHT ) )
      {
        if ( GetVasyRtlAssignNumberRead( RtlAsg ) > 1 )
        {
          if ( ! IsVexNodeAtom( AsgData ) )
          {
            ClearVasyRtlAssignSubst( RtlAsg );
          }
        }
      }
      else
      {
        for ( VexIndex = VexMin; VexIndex <= VexMax; VexIndex++ )
        {
          RtlSymbol = getrtlsymdecl( RtlDeclar, VexIndex ); 
          Element   = searchauthelem( VasyHashRtlAsg, (char *)RtlSymbol );
  
          if ( Element != (authelem *)0 )
          {
            RtlAsg = (rtlasg_list *)Element->VALUE;
            ClearVasyRtlAssignSubst( RtlAsg );
          }
        }
      }
    }
  }
  else
  {
    for ( ScanOper  = Expr->OPERAND;
          ScanOper != (chain_list *)0;
          ScanOper  = ScanOper->NEXT )
    {
      VasySimplifyMarkAsgVex( GetVexOperand( ScanOper ) );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySimplifySubstVex                    |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasySimplifySubstVex( Expr )

  vexexpr *Expr;
{
  rtldecl_list *RtlDeclar;
  rtlsym       *RtlSymbol;
  rtlasg_list  *RtlAsg;
  vexexpr      *Operand;
  authelem     *Element; 
  chain_list   *ScanOper;
  char         *AtomName;

  if ( Expr != (vexexpr *)0 )
  {
    if ( IsVexNodeAtom( Expr ) )
    {
      if ( ! IsVexAtomLiteral( Expr ) )
      {
        AtomName  = GetVexAtomValue( Expr );
        RtlDeclar = searchrtldecl( VasyFigure, AtomName );

        RtlSymbol = getrtlsymdecl( RtlDeclar, Expr->LEFT ); 
        Element   = searchauthelem( VasyHashRtlAsg, (char *)RtlSymbol );

        if ( Element != (authelem *)0 )
        {
          RtlAsg = (rtlasg_list *)Element->VALUE;
    
          if ( IsVasyRtlAssignSubst( RtlAsg ) )
          {
            if ( IsVasyDebugLevel0() )
            {
              VasyPrintf( stdout, " Substitute %s[%d %d]\n", 
                          AtomName, Expr->LEFT, Expr->RIGHT );
            }
  
            if ( ! IsVasyRtlAssignSubstDone( RtlAsg ) )
            {
              VasySimplifySubstAsg( RtlAsg );
            }
  
            freevexexpr( Expr );
            Expr = dupvexexpr( RtlAsg->VEX_DATA );
          }
        }
      }
    }
    else
    {
      for ( ScanOper  = Expr->OPERAND;
            ScanOper != (chain_list *)0;
            ScanOper  = ScanOper->NEXT )
      {
        Operand = VasySimplifySubstVex( GetVexOperand( ScanOper ) );
        SetVexOperand( ScanOper, Operand );
      }
    }
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySimplifySubstAsg                    |
|                                                             |
\------------------------------------------------------------*/

static void VasySimplifySubstAsg( RtlAsg )

  rtlasg_list *RtlAsg;
{
  rtlbivex_list *ScanBiVex;

  if ( ! IsVasyRtlAssignSubstDone( RtlAsg ) )
  {
    SetVasyRtlAssignSubstDone( RtlAsg );

    RtlAsg->VEX_DATA = VasySimplifySubstVex( RtlAsg->VEX_DATA );

    for ( ScanBiVex  = RtlAsg->BIVEX;
          ScanBiVex != (rtlbivex_list *)0;
          ScanBiVex  = ScanBiVex->NEXT )
    {
      ScanBiVex->VEX_COND = VasySimplifySubstVex( ScanBiVex->VEX_COND );
      ScanBiVex->VEX_DATA = VasySimplifySubstVex( ScanBiVex->VEX_DATA );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySimplifyHashAsg                     |
|                                                             |
\------------------------------------------------------------*/

static void VasySimplifyHashDefineAsg( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtldecl_list  *RtlDeclar;
  rtlsym        *RtlSymbol;
  rtlasg_list   *RtlAsg;
  vexexpr       *VexAtom;
  char          *AtomName;
  int            VexMin;
  int            VexMax;
  int            VexIndex;

  if ( VasyHashRtlAsg == (authtable *)0 )
  {
    VasyHashRtlAsg = createauthtable( 1000 );
  }
  else
  {
    resetauthtable( VasyHashRtlAsg );
  }

  for ( RtlAsg  = RtlFigure->ASSIGN;
        RtlAsg != (rtlasg_list *)0;
        RtlAsg  = RtlAsg->NEXT )
  {
    VexAtom   = RtlAsg->VEX_ATOM;
    AtomName  = GetVexAtomValue( VexAtom );
    RtlDeclar = searchrtldecl( RtlFigure, AtomName );

    VexMin   = getvexvectormin( VexAtom );
    VexMax   = getvexvectormax( VexAtom );

    if ( ( RtlAsg->TYPE    == RTL_ASG_COMBINATORIAL ) &&
         ( RtlDeclar->TYPE == RTL_DECLAR_SIGNAL     ) &&
         ( IsVasyRtlDeclarDefine( RtlDeclar )       ) )
    {
      SetVasyRtlAssignSubst( RtlAsg );

      for ( VexIndex = VexMin; VexIndex <= VexMax; VexIndex++ )
      {
        RtlSymbol = getrtlsymdecl( RtlDeclar, VexIndex );

        addauthelem( VasyHashRtlAsg, (char *)RtlSymbol, (long)RtlAsg );
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySimplifySubstDefine                 |
|                                                             |
\------------------------------------------------------------*/

static void VasySimplifySubstDefine( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtlasg_list   *RtlAsg;
  rtlasg_list   *DelAsg;
  rtlbivex_list *ScanBiVex;

  for ( RtlAsg  = RtlFigure->ASSIGN;
        RtlAsg != (rtlasg_list *)0;
        RtlAsg  = RtlAsg->NEXT )
  {
    VasySimplifyMarkAsgVex( RtlAsg->VEX_DATA );

    for ( ScanBiVex  = RtlAsg->BIVEX;
          ScanBiVex != (rtlbivex_list *)0;
          ScanBiVex  = ScanBiVex->NEXT )
    {
      VasySimplifyMarkAsgVex( ScanBiVex->VEX_COND );
      VasySimplifyMarkAsgVex( ScanBiVex->VEX_DATA );
    }
  }

  if ( IsVasyDebugLevel1() )
  {
    VasySimplifyDisplaySubst( RtlFigure );
  }

  for ( RtlAsg  = RtlFigure->ASSIGN;
        RtlAsg != (rtlasg_list *)0;
        RtlAsg  = RtlAsg->NEXT )
  {
    VasySimplifySubstAsg( RtlAsg );
  }
/*
** Delete unused assignments
*/
  RtlAsg = RtlFigure->ASSIGN;

  while ( RtlAsg != (rtlasg_list *)0 )
  {
    DelAsg = RtlAsg;
    RtlAsg = RtlAsg->NEXT;

    if ( IsVasyRtlAssignSubst( DelAsg ) )
    {
      if ( IsVasyDebugLevel0() )
      {
        VasyPrintf( stdout, " Delete " );
        viewvexexprln( DelAsg->VEX_ATOM );
      }

      delrtlasg( RtlFigure, DelAsg );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySimplifyMarkRead                    |
|                                                             |
\------------------------------------------------------------*/

static void VasySimplifyMarkRead( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtldecl_list  *RtlDeclar;
  rtlport_list  *RtlPort;
  rtlsym        *RtlSymbol;
  rtlasg_list   *RtlAsg;
  rtlbivex_list *ScanBiVex;
  rtlins_list   *RtlInst;
  rtlmap_list   *RtlMap;
  vexexpr       *VexAtom;
  char          *AtomName;
  int            VexMin;
  int            VexMax;
  int            VexIndex;

  for ( RtlAsg  = RtlFigure->ASSIGN;
        RtlAsg != (rtlasg_list *)0;
        RtlAsg  = RtlAsg->NEXT )
  {
    VexAtom   = RtlAsg->VEX_ATOM;
    AtomName  = GetVexAtomValue( VexAtom );
    RtlDeclar = searchrtldecl( RtlFigure, AtomName );

    SetVasyRtlDeclarAsg( RtlDeclar ); 

    VexMin   = getvexvectormin( VexAtom );
    VexMax   = getvexvectormax( VexAtom );

    for ( VexIndex = VexMin; VexIndex <= VexMax; VexIndex++ )
    {
      RtlSymbol = getrtlsymdecl( RtlDeclar, VexIndex );
      SetVasyRtlDeclarAsg( RtlSymbol );
    }

    VasySimplifyMarkReadVex( RtlAsg->VEX_DATA );

    for ( ScanBiVex  = RtlAsg->BIVEX;
          ScanBiVex != (rtlbivex_list *)0;
          ScanBiVex  = ScanBiVex->NEXT )
    {
      VasySimplifyMarkReadVex( ScanBiVex->VEX_COND );
      VasySimplifyMarkReadVex( ScanBiVex->VEX_DATA );
    }
  }

  for ( RtlInst  = RtlFigure->INSTANCE;
        RtlInst != (rtlins_list *)0;
        RtlInst  = RtlInst->NEXT )
  {
    for ( RtlMap  = RtlInst->MAP;
          RtlMap != (rtlmap_list *)0;
          RtlMap  = RtlMap->NEXT )
    {
      VexAtom  = RtlMap->VEX_FORMAL;

      if ( IsVexNodeAtom( VexAtom ) )
      {
        AtomName = GetVexAtomValue( VexAtom );
        RtlPort  = searchrtlmodport( RtlFigure, RtlInst->MODEL, AtomName );

        if ( RtlPort->DIR != RTL_DIR_OUT )
        {
          VasySimplifyMarkReadVex( RtlMap->VEX_ACTUAL );
        }

        if ( RtlPort->DIR != RTL_DIR_IN )
        {
          VasySimplifyMarkWriteVex( RtlMap->VEX_ACTUAL );
        }
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySimplifySubstReadOutVex             |
|                                                             |
\------------------------------------------------------------*/

static void VasySimplifySubstReadOutVex( VexExpr )

  vexexpr *VexExpr;
{
  vexexpr    *Operand;
  char       *AtomValue;
  authelem   *Element;
  chain_list *ScanChain;

  if ( VexExpr != (vexexpr *)0 )
  {
    if ( IsVexNodeAtom( VexExpr ) )
    {
      if ( ! IsVexAtomLiteral( VexExpr ) )
      {
        AtomValue = GetVexAtomValue( VexExpr );
        Element   = searchauthelem( VasyHashOutAsg, AtomValue );

        if ( Element != (authelem *)0 )
        {
          AtomValue = (char *)Element->VALUE;
          SetVexAtomValue( VexExpr, AtomValue );
        }
      }
    }
    else
    {
      for ( ScanChain  = VexExpr->OPERAND;
            ScanChain != (chain_list *)0;
            ScanChain  = ScanChain->NEXT )
      {
        Operand = GetVexOperand( ScanChain );
        VasySimplifySubstReadOutVex( Operand );
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySimplifySubstReadOut                |
|                                                             |
\------------------------------------------------------------*/

static void VasySimplifySubstReadOut( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtldecl_list  *RtlDeclar;
  rtldecl_list  *NewDeclar;
  rtlasg_list   *RtlAsg;
  rtlasg_list   *NewAsg;
  rtlbivex_list *ScanBiVex;
  vexexpr       *VexAtom;
  vexexpr       *NewAtom;
  char          *AtomName;
  char          *NewName;
  int            Found;

  Found = 0;

  if ( VasyHashOutAsg == (authtable *)0 )
  {
    VasyHashOutAsg = createauthtable( 1000 );
  }
  else
  {
    resetauthtable( VasyHashOutAsg );
  }

  RtlAsg = RtlFigure->ASSIGN;

  for ( RtlDeclar  = RtlFigure->DECLAR[ RTL_DECLAR_PORT ];
        RtlDeclar != (rtldecl_list *)0;
        RtlDeclar  = RtlDeclar->NEXT )
  {
    if ( ( RtlDeclar->DIR == RTL_DIR_OUT    ) &&
         ( IsVasyRtlDeclarRead( RtlDeclar ) ) )
    {
      Found = 1;
/*
**  Add an internal signal
*/
      VexAtom  = RtlDeclar->VEX_ATOM;
      AtomName = GetVexAtomValue( VexAtom );

      sprintf( VasyBuffer, "rtl_internal_%s", AtomName );
      NewName = namealloc( VasyBuffer );
      NewAtom = dupvexexpr( VexAtom );
      SetVexAtomValue( NewAtom, NewName );
   
      NewDeclar = addrtldecl( VasyFigure, NewAtom, RTL_DECLAR_SIGNAL );
      NewDeclar->VEX_INIT = dupvexexpr( RtlDeclar->VEX_INIT );
      NewDeclar->BASE     = RtlDeclar->BASE;
      NewDeclar->KIND     = RtlDeclar->KIND;
      NewDeclar->DIR      = RTL_DIR_INOUT;

      SetVasyRtlDeclarAsg( NewDeclar );
      SetVasyRtlDeclarRead( NewDeclar );

      addauthelem( VasyHashOutAsg, AtomName, (long)NewName );
/*
**  Add an assignation to this signal out <= internal_out
*/
      NewAsg = addrtlasg( RtlFigure, dupvexexpr( VexAtom ), RTL_ASG_COMBINATORIAL );
      NewAsg->VEX_DATA = dupvexexpr( NewAtom );
    }
  }

  if ( Found )
  {
    while ( RtlAsg != (rtlasg_list *)0 ) 
    {
      VasySimplifySubstReadOutVex( RtlAsg->VEX_ATOM );
  
      for ( ScanBiVex  = RtlAsg->BIVEX;
            ScanBiVex != (rtlbivex_list *)0;
            ScanBiVex  = ScanBiVex->NEXT )
      {
        VasySimplifySubstReadOutVex( ScanBiVex->VEX_COND );
        VasySimplifySubstReadOutVex( ScanBiVex->VEX_DATA );
      }
  
      RtlAsg  = RtlAsg->NEXT;
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySimplifyExtendSignVex               |
|                                                             |
\------------------------------------------------------------*/

vexexpr *VasySimplifyExtendSignVex( VexExpr, Width )

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
      sprintf( VasyBuffer, "rtlexts_%ld", VasyNumberDef++ );
      VexAtom = createvexatomvec( VasyBuffer, VexExpr->WIDTH - 1, 0 );
    
      RtlDeclar = addrtldecl( VasyFigure, VexAtom, RTL_DECLAR_SIGNAL );

      if ( IsVexNodeSigned( VexExpr ) ) RtlDeclar->BASE = VEX_TYPE_SIGNED;
      else                              RtlDeclar->BASE = VEX_TYPE_UNSIGNED;

      RtlDeclar->KIND = RTL_KIND_NONE;
      RtlDeclar->DIR  = RTL_DIR_INOUT;
    
      RtlAssign = addrtlasgafter( VasyFigure, VasyAsg,
                                  dupvexexpr( VexAtom ), RTL_ASG_COMBINATORIAL );
      RtlAssign->VEX_DATA = VexExpr;

      VexExpr = dupvexexpr( VexAtom );
    }

    if ( ! IsVexAtomLiteral( VexExpr ) )
    {
      sprintf( VasyBuffer, "rtlexts_%ld", VasyNumberDef++ );
      VexAtom = createvexatomvec( VasyBuffer, Width - 1, 0 );
   
      RtlDeclar = addrtldecl( VasyFigure, VexAtom, RTL_DECLAR_SIGNAL );
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
      RtlAssign = addrtlasgafter( VasyFigure, VasyAsg,
                                  dupvexexpr( VexAtom ), RTL_ASG_COMBINATORIAL );
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
|                     VasySimplifyResizeWidthVex              |
|                                                             |
\------------------------------------------------------------*/

static int VasySimplifyGetLog2n( VexExpr )

  vexexpr *VexExpr;
{
  long Value;
  long Mask;
  int  Log2n;
  int  Index;

  if ( evalvexatomlong( VexExpr, &Value ) )
  {
    return( -1 );
  }

  Mask  =  1;
  Log2n = -1;

  for ( Index = 0; Index < 32; Index++ )
  {
    if ( Value & Mask )
    {
      if ( Log2n == -1 ) Log2n = Index;
      else               return( -1 );
    }

    Mask = Mask << 1L;
  }

  return( Log2n );
}

static vexexpr *VasySimplifyResizeWidthVex( VexExpr, Width )

  vexexpr *VexExpr;
  int      Width;
{
  vexexpr      *Operand;
  chain_list   *ScanOper;
  int           MaxWidth;
  long          IntValue;
  long          Oper;

  if ( VexExpr != (vexexpr *)0 )
  {
    if ( IsVexNodeAtom( VexExpr ) )
    {
      VexExpr = VasySimplifyExtendSignVex( VexExpr, Width );
    }
    else
    if ( IsVexNodeOper( VexExpr ) )
    {
      Oper = GetVexOperValue( VexExpr );

      if ( ! VasyOperatorSupported[ Oper ] )
      {
        VasyError( VASY_ERROR_OPERATOR_NOT_SUPPORTED, getvexoperuppername( Oper ) );
      }

      if ( Oper == VEX_CONCAT )
      {
        for ( ScanOper  = VexExpr->OPERAND;
              ScanOper != (chain_list *)0;
              ScanOper  = ScanOper->NEXT )
        {
          Operand = GetVexOperand( ScanOper );
          Operand = VasySimplifyResizeWidthVex( Operand, Operand->WIDTH );
          SetVexOperand( ScanOper, Operand );
        }

        VexExpr = VasySimplifyExtendSignVex( VexExpr, Width ); 
      }
      else
      if ( Oper == VEX_IFT )
      {
        Operand = GetVexOperand( VexExpr->OPERAND );
        Operand = VasySimplifyResizeWidthVex( Operand, 1 );
        SetVexOperand( VexExpr->OPERAND, Operand );

        Operand = GetVexOperand( VexExpr->OPERAND->NEXT );
        Operand = VasySimplifyResizeWidthVex( Operand, Width );
        SetVexOperand( VexExpr->OPERAND->NEXT, Operand );
      }
      else
      if ( ( Oper == VEX_EQ ) || 
           ( Oper == VEX_NE ) ||
           ( Oper == VEX_LT ) ||
           ( Oper == VEX_LE ) ||
           ( Oper == VEX_GT ) ||
           ( Oper == VEX_GE ) )
      {
        MaxWidth = 0;

        for ( ScanOper  = VexExpr->OPERAND; 
              ScanOper != (chain_list *)0;
              ScanOper  = ScanOper->NEXT )
        {
          Operand = GetVexOperand( ScanOper );

          if ( ScanOper == VexExpr->OPERAND ) MaxWidth = Operand->WIDTH;
          else
          if ( Operand->WIDTH > MaxWidth    ) MaxWidth = Operand->WIDTH;

          SetVexOperand( ScanOper, Operand );
        }

        for ( ScanOper  = VexExpr->OPERAND; 
              ScanOper != (chain_list *)0;
              ScanOper  = ScanOper->NEXT )
        {
          Operand = GetVexOperand( ScanOper );
          Operand = VasySimplifyResizeWidthVex( Operand, MaxWidth );
          SetVexOperand( ScanOper, Operand );
        }
      }
      else
      if ( Oper == VEX_MOD )
      {
        Operand  = GetVexOperand( VexExpr->OPERAND->NEXT );
        IntValue = VasySimplifyGetLog2n( Operand );

        if ( IntValue == -1 )
        {
          VasyError( VASY_ERROR_OPERATOR_NOT_SUPPORTED, getvexoperuppername( Oper ) );
        }

        Operand = GetVexOperand( VexExpr->OPERAND ); 
        Operand = VasySimplifyResizeWidthVex( Operand, IntValue );
        ClearVexNodeSigned( Operand );
        Operand = VasySimplifyExtendSignVex( Operand, Width ); 

        SetVexOperand( VexExpr->OPERAND, (vexexpr *)0 );
        freevexexpr( VexExpr );

        VexExpr = Operand;
      }
      else
      {
        for ( ScanOper  = VexExpr->OPERAND; 
              ScanOper != (chain_list *)0;
              ScanOper  = ScanOper->NEXT )
        {
          Operand = GetVexOperand( ScanOper );
          Operand = VasySimplifyResizeWidthVex( Operand, Width );
          SetVexOperand( ScanOper, Operand );
        }
      }

      VexExpr->WIDTH = Width;
    }
    else
    {
      for ( ScanOper  = VexExpr->OPERAND; 
            ScanOper != (chain_list *)0;
            ScanOper  = ScanOper->NEXT )
      {
        Operand = GetVexOperand( ScanOper );
        Operand = VasySimplifyResizeWidthVex( Operand, Operand->WIDTH );
        SetVexOperand( ScanOper, Operand );
      }

      VexExpr->WIDTH = Width;
    }
  }

  return( VexExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySimplifyResizeWidth                 |
|                                                             |
\------------------------------------------------------------*/

static void VasySimplifyResizeWidth( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtlins_list   *RtlInst;
  rtlmap_list   *RtlMap;
  rtlasg_list   *RtlAsg;
  rtlbivex_list *ScanBiVex;
  vexexpr       *VexAtom;
  int            Width;

  for ( RtlAsg  = RtlFigure->ASSIGN;
        RtlAsg != (rtlasg_list *)0;
        RtlAsg  = RtlAsg->NEXT )
  {
    VexAtom = RtlAsg->VEX_ATOM;
    Width   = VexAtom->WIDTH;

    RtlAsg->VEX_DATA = VasySimplifyResizeWidthVex( RtlAsg->VEX_DATA, Width );

    for ( ScanBiVex  = RtlAsg->BIVEX;
          ScanBiVex != (rtlbivex_list *)0;
          ScanBiVex  = ScanBiVex->NEXT )
    {
      ScanBiVex->VEX_COND = VasySimplifyResizeWidthVex( ScanBiVex->VEX_COND, 1     );
      ScanBiVex->VEX_DATA = VasySimplifyResizeWidthVex( ScanBiVex->VEX_DATA, Width );
    }
  }

  for ( RtlInst  = RtlFigure->INSTANCE;
        RtlInst != (rtlins_list *)0;
        RtlInst  = RtlInst->NEXT )
  {
    for ( RtlMap  = RtlInst->MAP;
          RtlMap != (rtlmap_list *)0;
          RtlMap  = RtlMap->NEXT )
    {
      VexAtom = RtlMap->VEX_FORMAL;
      Width   = VexAtom->WIDTH;

      RtlMap->VEX_ACTUAL = VasySimplifyResizeWidthVex( RtlMap->VEX_ACTUAL, Width );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySimplifyRtlFig                      |
|                                                             |
\------------------------------------------------------------*/

void VasySimplifyRtlFig( RtlFigure )

  rtlfig_list  *RtlFigure;
{
  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "--> VasySimplifyRtlFig %s\n", RtlFigure->NAME );
  }

  VasyFigure = RtlFigure;
/*
** All operands in all expressions should have the 'right' width
*/
  VasySimplifyResizeWidth( RtlFigure );
/*
** Hash all register assignments
*/
  VasySimplifyHashRegisterAsg( RtlFigure );
/*
** Compact all register assignments
*/
  VasySimplifyCompactRegister( RtlFigure );
/*
** Hash all tristates assignments
*/
  VasySimplifyHashTristateAsg( RtlFigure );
/*
** Compact all tristates assignments
*/
  VasySimplifyCompactTristate( RtlFigure );
/*
** Hash all define assignments
*/
  VasySimplifyHashDefineAsg( RtlFigure );
/*
** Find all define assignment to substitute and delete
*/
  VasySimplifySubstDefine( RtlFigure );
/*
** Find all symbol read and assigned
*/
  VasySimplifyMarkRead( RtlFigure );
/*
** Rename all read out ports
*/
  VasySimplifySubstReadOut( RtlFigure );

  if ( IsVasyDebugLevel2() )
  {
    VasySimplifyDisplayRead( RtlFigure );
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "<-- VasySimplifyRtlFig %s\n", RtlFigure->NAME );
  }
}
