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
| Tool    :                     MOCHA                         |
|                                                             |
| File    :                  mocha_syf.c                      |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   07.08.02                        |
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
# include "fvh.h"
# include "ftl.h"
# include "vex.h"
# include "ctl.h"
# include "ctp.h"
# include "abe.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "mocha_debug.h"
# include "mocha_syf.h"

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
|                          Privates                           |
|                                                             |
\------------------------------------------------------------*/

  static char *MochaSyfClockKeyword         = (char *)0;
  static char *MochaSyfCurrentStateKeyword  = (char *)0;
  static char *MochaSyfNextStateKeyword     = (char *)0;
  static char *MochaSyfToolsKeyword         = (char *)0;

  static char *MochaSyfClockName;
  static char *MochaSyfCurrentStateName;
  static char *MochaSyfNextStateName;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         MochaSyfaddfsmsyfinfo               |
|                                                             |
\------------------------------------------------------------*/

static mochasyfinfo *MochaSyfaddfsmsyfinfo( FsmFigure )

  fsmfig_list *FsmFigure;
{
  mochasyfinfo *MochaSyfInfo;

  MochaSyfInfo = (void *)autallocblock( sizeof( mochasyfinfo ) );
  FsmFigure->USER = (void *)MochaSyfInfo;

  return( MochaSyfInfo );
}

/*------------------------------------------------------------\
|                                                             |
|                         MochaSyfaddfsmfig                   |
|                                                             |
\------------------------------------------------------------*/

static fsmfig_list *MochaSyfaddfsmfig( Name )

  char *Name;
{
  fsmfig_list *Figure;
  
  Figure = addfsmfig( Name );
  MochaSyfaddfsmsyfinfo( Figure );

  return( Figure );
}

/*------------------------------------------------------------\
|                                                             |
|                         MochaSyfaddfsmsyfout                |
|                                                             |
\------------------------------------------------------------*/

static mochasyfout *MochaSyfaddfsmsyfout( Output )

  fsmout_list *Output;
{
  mochasyfout *MochaSyfOut;

  MochaSyfOut = (mochasyfout *)autallocblock( sizeof( mochasyfout ) );
  Output->USER = (void *)MochaSyfOut;

  return( MochaSyfOut );
}

/*------------------------------------------------------------\
|                                                             |
|                      MochaSyfaddfsmsyfstate                 |
|                                                             |
\------------------------------------------------------------*/

static mochasyfstate *MochaSyfaddfsmsyfstate( Figure, State )

  fsmfig_list   *Figure;
  fsmstate_list *State;
{
  mochasyfstate *MochaSyfState;
  mochasyfinfo  *MochaSyfInfo;
  char           Buffer[ 1024 ];

  MochaSyfInfo = MOCHA_SYF_INFO( Figure );

  MochaSyfState = (void *)autallocblock( sizeof( mochasyfstate ) );
  State->USER   = (void *)MochaSyfState;

  sprintf( Buffer, "%s_%s", MochaSyfInfo->CURRENT_STATE, State->NAME );
  MochaSyfState->CURRENT_NAME = namealloc( Buffer );

  sprintf( Buffer, "%s_%s", MochaSyfInfo->NEXT_STATE, State->NAME );
  MochaSyfState->NEXT_NAME = namealloc( Buffer );

  sprintf( Buffer, "%s_in_%s", MochaSyfInfo->NEXT_STATE, State->NAME );
  MochaSyfState->NEXT_IN_NAME = namealloc( Buffer );

  return( MochaSyfState );
}

/*------------------------------------------------------------\
|                                                             |
|                       MochaSyfInitializeKeyword             |
|                                                             |
\------------------------------------------------------------*/

static void MochaSyfInitializeKeyword()
{
  if ( MochaSyfClockKeyword == (char *)0 )
  {
    MochaSyfClockKeyword        = namealloc( "clock"         );
    MochaSyfCurrentStateKeyword = namealloc( "current_state" );
    MochaSyfNextStateKeyword    = namealloc( "next_state"    );
    MochaSyfToolsKeyword        = namealloc( "syf"           );
  }

  MochaSyfClockName        = MochaSyfClockKeyword;
  MochaSyfCurrentStateName = MochaSyfCurrentStateKeyword;
  MochaSyfNextStateName    = MochaSyfNextStateKeyword;
}

/*------------------------------------------------------------\
|                                                             |
|                       MochaSyfFbhTreatPort                  |
|                                                             |
\------------------------------------------------------------*/

static void MochaSyfFbhTreatPort( FsmFigure )

  fsmfig_list *FsmFigure;
{
  fsmout_list *ScanOut;

  for ( ScanOut  = FsmFigure->OUT;
        ScanOut != (fsmout_list *)0;
        ScanOut  = ScanOut->NEXT )
  {
    MochaSyfaddfsmsyfout( ScanOut );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       MochaSyfFbhTreatPragma                |
|                                                             |
\------------------------------------------------------------*/

static void MochaSyfFbhTreatPragma( FsmFigure )

  fsmfig_list *FsmFigure;
{
  fsmpragma_list *ScanPragma;
  mochasyfinfo   *MochaSyfInfo;
  char           *PragmaType;

  MochaSyfInfo = MOCHA_SYF_INFO( FsmFigure );

  for ( ScanPragma  = FsmFigure->PRAGMA;
        ScanPragma != (fsmpragma_list *)0;
        ScanPragma  = ScanPragma->NEXT )
  {
    PragmaType = ScanPragma->TYPE;

    if ( PragmaType == MochaSyfCurrentStateKeyword )
    {
      MochaSyfCurrentStateName = ScanPragma->NAME;
    }
    else 
    if ( PragmaType == MochaSyfNextStateKeyword )
    {
      MochaSyfNextStateName = ScanPragma->NAME;
    }
    else 
    if ( PragmaType == MochaSyfClockKeyword )
    {
      MochaSyfClockName = ScanPragma->NAME;
    }
  }

  MochaSyfInfo->CURRENT_STATE = MochaSyfCurrentStateName;
  MochaSyfInfo->NEXT_STATE    = MochaSyfNextStateName;
}

/*------------------------------------------------------------\
|                                                             |
|                        MochaSyfFbhParse                     |
|                                                             |
\------------------------------------------------------------*/

fsmfig_list *MochaSyfFbhParse( Name )

  char *Name;
{
  fsmfig_list    *FsmFigure;
  fsmfig_list    *ScanFigure;
  chain_list     *ScanChain;
  mochasyfinfo   *MochaSyfInfo;
  fsmstate_list  *ScanState;

  MochaSyfInitializeKeyword();

  FsmFigure = MochaSyfaddfsmfig( Name );

  loadfsmfig( FsmFigure, Name );

  if ( IsFsmFigMulti( FsmFigure ) )
  {
    for ( ScanChain  = FsmFigure->MULTI;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      ScanFigure = (fsmfig_list *)ScanChain->DATA; 
      MochaSyfInfo = MochaSyfaddfsmsyfinfo( ScanFigure );
    }
  }
  else
  {
    FsmFigure->MULTI = addchain( (chain_list *)0, FsmFigure );
  }

  for ( ScanChain  = FsmFigure->MULTI;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    ScanFigure   = (fsmfig_list *)ScanChain->DATA;
    MochaSyfInfo = MOCHA_SYF_INFO( ScanFigure );

    ScanState = ScanFigure->STAR_STATE;
  
    if ( ScanState != (fsmstate_list *)0 )
    {
      *ScanState->PREV = ScanState->NEXT;
  
      if ( ScanState->NEXT != (fsmstate_list *)0 )
      {
        ScanState->NEXT->PREV = ScanState->PREV;
      }
  
      ScanFigure->NUMBER_STATE--;
    }
  
    MochaSyfFbhTreatPragma( ScanFigure );
    MochaSyfFbhTreatPort( ScanFigure );
  
    for ( ScanState  = ScanFigure->STATE;
          ScanState != (fsmstate_list *)0;
          ScanState  = ScanState->NEXT )
    {
      MochaSyfaddfsmsyfstate( ScanFigure, ScanState );
    }
  }

  return( FsmFigure );
}

/*------------------------------------------------------------\
|                                                             |
|                        MochaSyfFsmSimplify                  |
|                                                             |
\------------------------------------------------------------*/

void MochaSyfFsmSimplify( FsmFigure )

  fsmfig_list *FsmFigure;
{
  mochasyfinfo   *MochaSyfInfo;
  fsmstate_list  *ScanState;
  fsmlocout_list *ScanLocout;
  fsmlocout_list *DelLocout;
  fsmtrans_list  *ScanTrans;
  fsmtrans_list  *DelTrans;

  MochaSyfInfo = MOCHA_SYF_INFO( FsmFigure );

  for ( ScanState  = FsmFigure->STATE;
        ScanState != (fsmstate_list *)0;
        ScanState  = ScanState->NEXT )
  {
    ScanLocout = ScanState->LOCOUT;

    while ( ScanLocout != (fsmlocout_list *)0 )
    {
      if ( ScanLocout->ABL != (chain_list *)0 )
      {
        ScanLocout->ABL = simpablexpr( ScanLocout->ABL );

        if ( ( ABL_ATOM( ScanLocout->ABL )                             ) &&
             ( ABL_ATOM_VALUE( ScanLocout->ABL ) == ABL_ATOM_NAME_ZERO ) )
        {
          freeablexpr( ScanLocout->ABL );
          ScanLocout->ABL = (chain_list *)0;
        }
      }

      if ( ScanLocout->ABL_DC != (chain_list *)0 )
      {
        ScanLocout->ABL_DC = simpablexpr( ScanLocout->ABL_DC );

        if ( ( ABL_ATOM( ScanLocout->ABL_DC )                             ) &&
             ( ABL_ATOM_VALUE( ScanLocout->ABL_DC ) == ABL_ATOM_NAME_ZERO ) )
        {
          freeablexpr( ScanLocout->ABL_DC );
          ScanLocout->ABL_DC = (chain_list *)0;
        }
      }

      if ( ( ScanLocout->ABL    == (chain_list *)0 ) &&
           ( ScanLocout->ABL_DC == (chain_list *)0 ) )
      {
        DelLocout  = ScanLocout;
        ScanLocout = ScanLocout->NEXT;

        delfsmlocout( ScanState, DelLocout );
      }
      else
      {
        ScanLocout = ScanLocout->NEXT;
      }
    }
  }

  ScanTrans  = FsmFigure->TRANS;

  while ( ScanTrans != (fsmtrans_list *)0 )
  {
    ScanTrans->ABL = simpablexpr( ScanTrans->ABL );

    if ( ( ABL_ATOM( ScanTrans->ABL )                             ) &&
         ( ABL_ATOM_VALUE( ScanTrans->ABL ) == ABL_ATOM_NAME_ZERO ) )
    {
      DelTrans  = ScanTrans;
      ScanTrans = ScanTrans->NEXT;

      delfsmtrans( FsmFigure, DelTrans );
    }
    else
    {
      ScanTrans = ScanTrans->NEXT;
    }
  }

  if ( FsmFigure->CLOCK_ABL != (chain_list *)0 )
  {
    FsmFigure->CLOCK_ABL = simpablexpr( FsmFigure->CLOCK_ABL );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       MochaSyfGetNumberBit                  |
|                                                             |
\------------------------------------------------------------*/

static int MochaSyfGetNumberBit( Value )

  long Value;
{
  long Mask;
  int  NumberBit;
  
  Mask = 1 << 31;

  for ( NumberBit = 32; NumberBit > 0; NumberBit-- )
  {
    if ( Mask & Value ) return( NumberBit );

    Mask = Mask >> 1;
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                        MochaSyfFsmEncode                    |
|                                                             |
\------------------------------------------------------------*/

void MochaSyfFsmEncode( FsmFigure, FlagVerbose )

  fsmfig_list *FsmFigure;
  int          FlagVerbose;
{
  mochasyfinfo  *MochaSyfInfo;
  mochasyfcode  *CodeArray;
  fsmstate_list *ScanState;
  long           CodeMax;
  long           NumberBit;
  long           NumberState;
  int            Index;

  MochaSyfInfo = MOCHA_SYF_INFO( FsmFigure );

  NumberState = FsmFigure->NUMBER_STATE;
  NumberBit   = MochaSyfGetNumberBit( NumberState - 1 );

  CodeMax     = ( 1 << NumberBit );

  CodeArray = (mochasyfcode *)autallocblock( sizeof( mochasyfcode ) * CodeMax );

  MochaSyfInfo->CODE_ARRAY  = CodeArray;
  MochaSyfInfo->NUMBER_CODE = CodeMax;
  MochaSyfInfo->NUMBER_BIT  = NumberBit;

  for ( Index = 0; Index < CodeMax; Index++ )
  {
    CodeArray[ Index ].VALUE = Index;
  }

  Index     = 0;
  ScanState = FsmFigure->STATE;

  while ( ScanState != (fsmstate_list *)0 )
  {
    MOCHA_SYF_STATE( ScanState )->CODE = &CodeArray[ NumberState - Index - 1 ];

    CodeArray[ NumberState - Index - 1 ].USED  = 1;

    ScanState = ScanState->NEXT;
    Index     = Index + 1;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       MochaSyfSynthOut2Abl                  |
|                                                             |
\------------------------------------------------------------*/

static void MochaSyfSynthOut2Abl( FsmFigure )

  fsmfig_list *FsmFigure;
{
  mochasyfinfo   *MochaSyfInfo;
  fsmout_list    *ScanOutput;
  fsmstate_list  *ScanState;
  fsmlocout_list *ScanLocout;
  mochasyfout    *ScanMochaSyfOut;
  mochasyfstate  *ScanMochaSyfState;
  mochasyfcode   *ScanMochaSyfCode;
  ablexpr        *Equation;
  ablexpr        *Atom;

  MochaSyfInfo = MOCHA_SYF_INFO( FsmFigure );
  Equation     = (ablexpr     *)0;

  for ( ScanOutput  = FsmFigure->OUT;
        ScanOutput != (fsmout_list *)0;
        ScanOutput  = ScanOutput->NEXT )
  {
    ScanMochaSyfOut = MOCHA_SYF_OUT( ScanOutput );
    ScanMochaSyfOut->ABL = createabloper( ABL_OR );
  }

  for ( ScanState  = FsmFigure->STATE;
        ScanState != (fsmstate_list *)0;
        ScanState  = ScanState->NEXT )
  {
    ScanMochaSyfState = MOCHA_SYF_STATE( ScanState );
    ScanMochaSyfCode  = ScanMochaSyfState->CODE;

    for ( ScanLocout  = ScanState->LOCOUT;
          ScanLocout != (fsmlocout_list *)0;
          ScanLocout  = ScanLocout->NEXT )
    {
      ScanMochaSyfOut = MOCHA_SYF_OUT( ScanLocout->OUT );
      Atom = createablatom( ScanMochaSyfState->CURRENT_NAME );


      if ( ScanLocout->ABL_DC != (ablexpr *)0 )
      {
        Equation = optimablbinexpr( ABL_AND, 
                                     dupablexpr( ScanLocout->ABL_DC ),
                                     createablatom( ABL_ATOM_NAME_DC ) );
      }

      if ( ScanLocout->ABL != (ablexpr *)0 )
      {
        if ( ScanLocout->ABL_DC != (ablexpr *)0 )
        {
          Equation = optimablbinexpr( ABL_OR, 
                                       dupablexpr( ScanLocout->ABL ),
                                       Equation );
        }
        else
        {
          Equation = dupablexpr( ScanLocout->ABL );
        }
      }

      Equation = optimablbinexpr( ABL_AND, Atom, Equation );

      addablhexpr( ScanMochaSyfOut->ABL, Equation );
    }
  }

  ScanState = FsmFigure->STAR_STATE;

  if ( ScanState != (fsmstate_list *)0 )
  {
    for ( ScanLocout  = ScanState->LOCOUT;
          ScanLocout != (fsmlocout_list *)0;
          ScanLocout  = ScanLocout->NEXT )
    {
      ScanMochaSyfOut = MOCHA_SYF_OUT( ScanLocout->OUT );

      if ( ScanLocout->ABL_DC != (ablexpr *)0 )
      {
        Equation = optimablbinexpr( ABL_AND, 
                                     dupablexpr( ScanLocout->ABL_DC ),
                                     createablatom( ABL_ATOM_NAME_DC ) );
      }

      if ( ScanLocout->ABL != (ablexpr *)0 )
      {
        if ( ScanLocout->ABL_DC != (ablexpr *)0 )
        {
          Equation = optimablbinexpr( ABL_OR, 
                                       dupablexpr( ScanLocout->ABL ),
                                       Equation );
        }
        else
        {
          Equation = dupablexpr( ScanLocout->ABL );
        }
      }

      addablhexpr( ScanMochaSyfOut->ABL, Equation );
    }
  }
 
  for ( ScanOutput  = FsmFigure->OUT;
        ScanOutput != (fsmout_list *)0;
        ScanOutput  = ScanOutput->NEXT )
  {
    ScanMochaSyfOut = MOCHA_SYF_OUT( ScanOutput );

    Equation = ScanMochaSyfOut->ABL;

    if ( ABL_CDR( Equation ) == (ablexpr *)0 )
    {
/*
**  Out(i) = OR Null -> Out(i) = 0
*/
      Atom = createablatomzero();

      delablexpr( Equation );
      ScanMochaSyfOut->ABL = Atom;
    }
    else
    if ( ABL_CDDR( Equation ) == (ablexpr *)0 )
    {
/*
**  Out(i) = OR Locout(j) -> Out(i) = Locout(j)
*/
      Atom = ABL_CADR( Equation );
      ABL_CADR_L( Equation ) = (ablexpr *)0;
      freeablexpr( Equation );
      ScanMochaSyfOut->ABL = Atom;
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    MochaSyfSynthFreeOutAbl                  |
|                                                             |
\------------------------------------------------------------*/

static void MochaSyfSynthFreeOutAbl( FsmFigure )

  fsmfig_list *FsmFigure;
{
  mochasyfinfo   *MochaSyfInfo;
  fsmout_list    *ScanOutput;
  mochasyfout    *ScanMochaSyfOut;

  MochaSyfInfo = MOCHA_SYF_INFO( FsmFigure );

  for ( ScanOutput  = FsmFigure->OUT;
        ScanOutput != (fsmout_list *)0;
        ScanOutput  = ScanOutput->NEXT )
  {
    ScanMochaSyfOut = MOCHA_SYF_OUT( ScanOutput );

    if ( ScanMochaSyfOut->ABL != (ablexpr *)0 )
    {
      delablexpr( ScanMochaSyfOut->ABL );
      ScanMochaSyfOut->ABL = (ablexpr *)0;
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       MochaSyfSynthReg2Abl                  |
|                                                             |
\------------------------------------------------------------*/

static void MochaSyfSynthReg2Abl( FsmFigure )

  fsmfig_list *FsmFigure;
{
  mochasyfinfo      *MochaSyfInfo;
  fsmstate_list     *ScanState;
  fsmstate_list     *StarState;
  fsmtrans_list     *StarTrans;
  chain_list        *ScanChain;
  ablexpr           *Equation;
  ablexpr           *Atom;
  ablexpr           *AblReset;
  ablexpr           *AblSet;
  mochasyfstate     *ScanMochaSyfState;
  mochasyfcode      *ScanCode;
  mochasyfregstate  *RegArray;
  long               Index;
  long               BitMask;
  long               ScanBit;

  MochaSyfInfo = MOCHA_SYF_INFO( FsmFigure );
  RegArray     = MochaSyfInfo->REG_ARRAY;

  for ( Index = 0; Index < MochaSyfInfo->NUMBER_REG; Index++ )
  {
    RegArray[ Index ].ABL       = createabloper( ABL_OR );
    RegArray[ Index ].ABL_SET   = createabloper( ABL_OR );
    RegArray[ Index ].ABL_RESET = createabloper( ABL_OR );
  }

/*
**  If Code[i] of E(j), Reg(i) <- Reg(i) OR E(j)
*/
  for ( ScanState  = FsmFigure->STATE;
        ScanState != (fsmstate_list *)0;
        ScanState  = ScanState->NEXT )
  {
    ScanMochaSyfState = MOCHA_SYF_STATE( ScanState );
    ScanCode          = ScanMochaSyfState->CODE;

    BitMask = 1;

    for ( ScanBit = 0; ScanBit < MochaSyfInfo->NUMBER_REG; ScanBit++ )
    {
      if ( ScanCode->VALUE & BitMask )
      {
        Atom = createablatom( ScanMochaSyfState->NEXT_NAME );

        addablhexpr( RegArray[ ScanBit ].ABL, Atom );
      }

      BitMask = BitMask << 1;
    }
  }

  for ( Index = 0; Index < MochaSyfInfo->NUMBER_REG; Index++ )
  {
    Equation = RegArray[ Index ].ABL;

    if ( ABL_CDR( Equation ) == (ablexpr *)0 )
    {
/*
**  Reg(i) = OR Null -> Reg(i) = 0
*/
      Atom = createablatomzero();
      delablexpr( Equation );
      RegArray[ Index ].ABL = Atom;
    }
    else
    if ( ABL_CDDR( Equation ) == (ablexpr *)0 )
    {
/*
**  Reg(i) = OR E(j) -> Reg(i) = E(j)
*/
      Atom = ABL_CADR( Equation );
      ABL_CADR_L( Equation ) = (ablexpr *)0;
      freeablexpr( Equation );
      RegArray[ Index ].ABL = Atom;
    }
  }
/*
** Compute the Set and Reset conditions
*/
  StarState = FsmFigure->STAR_STATE;

  if ( StarState != (fsmstate_list *)0 )
  {
    for ( ScanChain  = StarState->FROM;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      StarTrans = (fsmtrans_list *)ScanChain->DATA;
      ScanState = StarTrans->TO;

      ScanMochaSyfState = MOCHA_SYF_STATE( ScanState );
      ScanCode     = ScanMochaSyfState->CODE;

      BitMask = 1;

      for ( ScanBit = 0; ScanBit < MochaSyfInfo->NUMBER_REG; ScanBit++ )
      {
        if ( ScanCode->VALUE & BitMask )
        {
          addablhexpr( RegArray[ ScanBit ].ABL_SET, dupablexpr( StarTrans->ABL ) );
        }
        else
        {
          addablhexpr( RegArray[ ScanBit ].ABL_RESET, dupablexpr( StarTrans->ABL ) );
        }

        BitMask = BitMask << 1;
      }
    }
  }

/*
** Add Reset and Set conditions
*/
  for ( Index = 0; Index < MochaSyfInfo->NUMBER_REG; Index++ )
  {
    Equation = RegArray[ Index ].ABL_SET;

    if ( ABL_CDR( Equation ) == (ablexpr *)0 )
    {
      freeablexpr( Equation );
      RegArray[ Index ].ABL_SET = (chain_list *)0;
    }
    else
    if ( ABL_CDDR( Equation ) == (ablexpr *)0 )
    {
      Atom = ABL_CADR( Equation );
      ABL_CADR_L( Equation ) = (ablexpr *)0;
      freeablexpr( Equation );
      RegArray[ Index ].ABL_SET = Atom;
    }

    Equation = RegArray[ Index ].ABL_RESET;

    if ( ABL_CDR( Equation ) == (ablexpr *)0 )
    {
      freeablexpr( Equation );
      RegArray[ Index ].ABL_RESET = (chain_list *)0;
    }
    else
    if ( ABL_CDDR( Equation ) == (ablexpr *)0 )
    {
      Atom = ABL_CADR( Equation );
      ABL_CADR_L( Equation ) = (ablexpr *)0;
      freeablexpr( Equation );
      RegArray[ Index ].ABL_RESET = Atom;
    }
  }

  for ( Index = 0; Index < MochaSyfInfo->NUMBER_REG; Index++ )
  {
    AblSet   = RegArray[ Index ].ABL_SET;
    AblReset = RegArray[ Index ].ABL_RESET;

    RegArray[ Index ].ABL_SET   = (ablexpr *)0;
    RegArray[ Index ].ABL_RESET = (ablexpr *)0;

    Equation = createablatom( RegArray[ Index ].NAME_IN );

    if ( AblReset != (ablexpr *)0 )
    {
      AblReset = optimablnotexpr( AblReset );
      Equation = optimablbinexpr( ABL_AND, Equation, AblReset );
    }

    if ( AblSet != (ablexpr *)0 )
    {
      Equation = optimablbinexpr( ABL_OR, Equation, AblSet );
    }

    RegArray[ Index ].ABL_IN = Equation;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   MochaSyfSynthFreeRegAbl                   |
|                                                             |
\------------------------------------------------------------*/

static void MochaSyfSynthFreeRegAbl( FsmFigure )

  fsmfig_list *FsmFigure;
{
  mochasyfinfo     *MochaSyfInfo;
  mochasyfregstate *RegArray;
  long              Index;

  MochaSyfInfo  = MOCHA_SYF_INFO( FsmFigure );
  RegArray = MochaSyfInfo->REG_ARRAY;

  for ( Index = 0; Index < MochaSyfInfo->NUMBER_REG; Index++ )
  {
    if ( RegArray[ Index ].ABL != (ablexpr *)0 )
    {
      delablexpr( RegArray[ Index ].ABL );
      RegArray[ Index ].ABL = (ablexpr *)0;
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     MochaSyfSynthCode2Abl                   |
|                                                             |
\------------------------------------------------------------*/

static void MochaSyfSynthCode2Abl( FsmFigure )

  fsmfig_list *FsmFigure;
{
  mochasyfinfo     *MochaSyfInfo;
  mochasyfcode     *CodeArray;
  mochasyfregstate *RegArray;
  ablexpr          *Equation;
  long              Index;
  long              BitMask;
  long              ScanBit;

  MochaSyfInfo = MOCHA_SYF_INFO( FsmFigure );

  CodeArray = MochaSyfInfo->CODE_ARRAY;
  RegArray  = MochaSyfInfo->REG_ARRAY;

  for ( Index = 0; Index < MochaSyfInfo->NUMBER_CODE; Index++ )
  {
    if ( MochaSyfInfo->NUMBER_BIT < 2 )
    {
      Equation = createablatom( RegArray[ 0 ].NAME_OUT );

      if ( ! CodeArray[ Index ].VALUE )
      {
        Equation = optimablnotexpr( Equation );
      }
    }
    else
    {
      Equation = createabloper( ABL_AND );
      BitMask  = 1;

      for ( ScanBit = 0; ScanBit < MochaSyfInfo->NUMBER_REG; ScanBit++ )
      {
        if ( CodeArray[ Index ].VALUE & BitMask )
        {
          addablhexpr( Equation,  
                       createablatom( RegArray[ ScanBit ].NAME_OUT ) );
        }
        else
        {
          addablhexpr( Equation,
                       optimablnotexpr( createablatom( RegArray[ ScanBit ].NAME_OUT ) ) );
        }

        BitMask = BitMask << 1;
      }
    }

    CodeArray[ Index ].ABL = Equation;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   MochaSyfSynthFreeCodeAbl                  |
|                                                             |
\------------------------------------------------------------*/

static void MochaSyfSynthFreeCodeAbl( FsmFigure )

  fsmfig_list *FsmFigure;
{
  mochasyfinfo *MochaSyfInfo;
  mochasyfcode *CodeArray;
  long          Index;

  MochaSyfInfo = MOCHA_SYF_INFO( FsmFigure );
  CodeArray    = MochaSyfInfo->CODE_ARRAY;

  for ( Index = 0; Index < MochaSyfInfo->NUMBER_CODE; Index++ )
  {
    if ( CodeArray[ Index ].ABL != (ablexpr *)0 )
    {
      delablexpr( CodeArray[ Index ].ABL );
      CodeArray[ Index ].ABL = (ablexpr *)0;
    }

    if ( CodeArray[ Index ].ABL_OUT != (ablexpr *)0 )
    {
      delablexpr( CodeArray[ Index ].ABL_OUT );
      CodeArray[ Index ].ABL_OUT = (ablexpr *)0;
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      MochaSyfSynthTrans2Abl                 |
|                                                             |
\------------------------------------------------------------*/

static void MochaSyfSynthTrans2Abl( FsmFigure )

  fsmfig_list *FsmFigure;
{
  mochasyfinfo  *MochaSyfInfo;
  fsmstate_list *ScanState;
  mochasyfstate *ScanMochaSyfState;
  mochasyfstate *FromMochaSyfState;
  ablexpr       *ScanChain;
  ablexpr       *Atom;
  fsmtrans_list *ScanTrans;

  MochaSyfInfo = MOCHA_SYF_INFO( FsmFigure );

  for ( ScanState  = FsmFigure->STATE;
        ScanState != (fsmstate_list *)0;
        ScanState  = ScanState->NEXT )
  {
    ScanChain    = ScanState->TO;
    ScanMochaSyfState = MOCHA_SYF_STATE( ScanState );

    if ( ScanChain == (ablexpr *)0 )
    {
/*
**  No transition to E(i)
*/
      ScanMochaSyfState->ABL_TRANS = createablatomzero();
    }
    else
    if ( ScanChain->NEXT == (ablexpr *)0 )
    {
/*
**  One transition to E(i)
*/
      ScanTrans    = (fsmtrans_list *)( ScanChain->DATA );

      if ( IsFsmStarTrans( ScanTrans ) )
      {
        ScanMochaSyfState->ABL_TRANS = createablatomzero();
      }
      else
      {
        FromMochaSyfState = MOCHA_SYF_STATE( ScanTrans->FROM );

        Atom = createablatom( FromMochaSyfState->CURRENT_NAME );
      
        ScanMochaSyfState->ABL_TRANS = 

           optimablbinexpr( ABL_AND, Atom, dupablexpr( ScanTrans->ABL ) );
      }
    }
    else
    {
/*
**  More than one transition to E(i)
*/
      ScanMochaSyfState->ABL_TRANS = createabloper( ABL_OR );

      while ( ScanChain != (ablexpr *)0 )
      {
        ScanTrans = (fsmtrans_list *)( ScanChain->DATA );

        if ( IsFsmStarTrans( ScanTrans ) )
        {
          addablhexpr( ScanMochaSyfState->ABL_TRANS, createablatomzero() );
        }
        else
        {
          FromMochaSyfState = MOCHA_SYF_STATE( ScanTrans->FROM );

          Atom = createablatom( FromMochaSyfState->CURRENT_NAME );
      
          addablhexpr( ScanMochaSyfState->ABL_TRANS, 
                       optimablbinexpr( ABL_AND, 
                                         Atom, dupablexpr( ScanTrans->ABL ) ) );

        }

        ScanChain = ScanChain->NEXT;
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      MochaSyfSynthFreeTransAbl              |
|                                                             |
\------------------------------------------------------------*/

static void MochaSyfSynthFreeTransAbl( FsmFigure )

  fsmfig_list *FsmFigure;
{
  fsmstate_list *ScanState;
  mochasyfstate *ScanMochaSyfState;

  for ( ScanState  = FsmFigure->STATE;
        ScanState != (fsmstate_list *)0;
        ScanState  = ScanState->NEXT )
  {
    ScanMochaSyfState = MOCHA_SYF_STATE( ScanState );

    if ( ScanMochaSyfState->ABL_TRANS != (ablexpr *)0 )
    {
      delablexpr( ScanMochaSyfState->ABL_TRANS );
      ScanMochaSyfState->ABL_TRANS = (ablexpr *)0;
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       MochaSyfSynthFsm2Abl                  |
|                                                             |
\------------------------------------------------------------*/

static void MochaSyfSynthFsm2Abl( FsmFigure )

  fsmfig_list *FsmFigure;
{
  mochasyfinfo *MochaSyfInfo;
  
  MochaSyfInfo = MOCHA_SYF_INFO( FsmFigure );

  MochaSyfSynthTrans2Abl( FsmFigure );
  MochaSyfSynthOut2Abl( FsmFigure );
  MochaSyfSynthReg2Abl( FsmFigure );
}

/*------------------------------------------------------------\
|                                                             |
|                      MochaSyfSynthFreeFsmAbl                |
|                                                             |
\------------------------------------------------------------*/

void MochaSyfSynthFreeFsmAbl( FsmFigure )

  fsmfig_list *FsmFigure;
{
  MochaSyfSynthFreeTransAbl( FsmFigure );
  MochaSyfSynthFreeOutAbl( FsmFigure );
  MochaSyfSynthFreeRegAbl( FsmFigure );
}

/*------------------------------------------------------------\
|                                                             |
|                      MochaSyfSynthFsmInit                   |
|                                                             |
\------------------------------------------------------------*/

static void MochaSyfSynthFsmInit( FsmFigure )

  fsmfig_list *FsmFigure;
{
  mochasyfinfo     *MochaSyfInfo;
  char              Buffer[ 25 ];
  mochasyfregstate *RegArray;
  long              RegMax;
  long              Index;

  MochaSyfInfo = MOCHA_SYF_INFO( FsmFigure );

  RegMax   = MochaSyfInfo->NUMBER_BIT;
  RegArray = (mochasyfregstate  *)autallocblock( sizeof( mochasyfregstate ) * RegMax );

  for ( Index = 0; Index < RegMax; Index++ )
  {
    sprintf( Buffer, "%s %ld", MochaSyfInfo->CURRENT_STATE, Index );
    RegArray[ Index ].NAME_OUT = namealloc( Buffer );
    sprintf( Buffer, "%s %ld", MochaSyfInfo->NEXT_STATE, Index );
    RegArray[ Index ].NAME_IN = namealloc( Buffer );
    RegArray[ Index ].NAME_MASTER = RegArray[ Index ].NAME_OUT;
  }

  MochaSyfInfo->REG_ARRAY  = RegArray;
  MochaSyfInfo->NUMBER_REG = RegMax;
}

/*------------------------------------------------------------\
|                                                             |
|                       MochaSyfSynthFsm                      |
|                                                             |
\------------------------------------------------------------*/

void MochaSyfSynthFsm( FsmFigure )

  fsmfig_list *FsmFigure;
{
  MochaSyfSynthFsmInit( FsmFigure );
  MochaSyfSynthCode2Abl( FsmFigure );
  MochaSyfSynthFsm2Abl( FsmFigure );
}


/*------------------------------------------------------------\
|                                                             |
|                       MochaSyfFsmTreatPort                  |
|                                                             |
\------------------------------------------------------------*/

static void MochaSyfFsmTreatPort( FsmFigure, FbhFigure )

   fsmfig_list *FsmFigure;
   fbfig_list  *FbhFigure;
{
  fbpor_list      *FbhPort;
  fbrin_list      *FbhRin;
  fsmport_list    *ScanPort;

  if ( IsFsmFigMixedRtl( FsmFigure ) )
  {
    return;
  }

  FbhPort = (fbpor_list *)0;
  FbhRin  = FbhFigure->BERIN;

  for ( ScanPort  = FsmFigure->PORT;
        ScanPort != (fsmport_list *)0;
        ScanPort  = ScanPort->NEXT )
  {
    FbhPort = fbh_addfbpor( FbhPort, ScanPort->NAME,
                            ScanPort->DIR, ScanPort->TYPE );

    if ( ScanPort->DIR != FSM_DIR_OUT ) 
    {
      FbhRin  = fbh_addfbrin( FbhRin, ScanPort->NAME );
    }
  }

  FbhFigure->BERIN = FbhRin;
  FbhFigure->BEPOR = FbhPort;
}

/*------------------------------------------------------------\
|                                                             |
|                       MochaSyfFsmTreatOutput                |
|                                                             |
\------------------------------------------------------------*/

void MochaSyfFsmTreatOutput( FsmFigure, FbhFigure )

   fsmfig_list   *FsmFigure;
   fbfig_list    *FbhFigure;
{
  mochasyfinfo   *MochaSyfInfo;
  fsmout_list    *ScanOut;
  fbaux_list     *ScanAux;
  mochasyfout    *ScanSyfOut;
  mochasyfregout *OutArray;
  fbout_list     *FbhOut;
  ablexpr        *AblExpr;

  MochaSyfInfo = MOCHA_SYF_INFO( FsmFigure );
  OutArray     = MochaSyfInfo->OUT_ARRAY;

  FbhOut = FbhFigure->BEOUT;

  for ( ScanOut  = FsmFigure->OUT;
        ScanOut != (fsmout_list *)0;
        ScanOut  = ScanOut->NEXT )
  {
    ScanSyfOut = MOCHA_SYF_OUT( ScanOut );

    if ( ScanSyfOut->ABL != (chain_list *)0 )
    {
      AblExpr = dupablexpr( ScanSyfOut->ABL );

      for ( ScanAux  = FbhFigure->BEAUX;
            ScanAux != (fbaux_list *)0;
            ScanAux  = ScanAux->NEXT )
      {
        if ( ScanAux->NAME == ScanOut->NAME ) break;
      }

      if ( ScanAux == (fbaux_list *)0 )
      {
        FbhOut = fbh_addfbout( FbhOut, ScanOut->NAME, AblExpr, (bddnode *)0, 0 );
      }
      else
      {
        ScanAux->ABL = AblExpr;
      }
    }
  }

  FbhFigure->BEOUT = FbhOut;
}

/*------------------------------------------------------------\
|                                                             |
|                       MochaSyfFsmTreatRegister              |
|                                                             |
\------------------------------------------------------------*/

void MochaSyfFsmTreatRegister( FsmFigure, FbhFigure )

   fsmfig_list    *FsmFigure;
   fbfig_list *FbhFigure;
{
  mochasyfinfo     *MochaSyfInfo;
  mochasyfregstate *RegArray;
  int               Index;
  fbreg_list       *FbhReg;
  fbrin_list       *FbhRin;
  fbbiabl_list     *FbhBiAbl;
  fbbinode_list    *FbhBiNode;
  chain_list       *Equation;

  MochaSyfInfo  = MOCHA_SYF_INFO( FsmFigure );
  RegArray = MochaSyfInfo->REG_ARRAY;
  FbhReg   = FbhFigure->BEREG;
  FbhRin   = FbhFigure->BERIN;

  for ( Index = MochaSyfInfo->NUMBER_REG - 1; Index >= 0; Index-- )
  {
    Equation = dupablexpr( FsmFigure->CLOCK_ABL );
    FbhBiAbl = fbh_addbiabl( (fbbiabl_list *)0,
                             RegArray[ Index ].NAME_MASTER,
                             Equation,
                             dupablexpr( RegArray[ Index ].ABL_IN ) );

    FbhBiNode = fbh_addbinode( (fbbinode_list *)0, (bddnode *)0, (bddnode *)0 );

    FbhReg = fbh_addfbreg( FbhReg, RegArray[ Index ].NAME_MASTER,
                           FbhBiAbl, FbhBiNode );

    FbhRin = fbh_addfbrin( FbhRin, RegArray[ Index ].NAME_MASTER );
  }

  FbhFigure->BERIN = FbhRin;
  FbhFigure->BEREG = FbhReg;
}

/*------------------------------------------------------------\
|                                                             |
|                       MochaSyfFsmTreatAux                   |
|                                                             |
\------------------------------------------------------------*/

void MochaSyfFsmTreatAux( FsmFigure, FbhFigure )

   fsmfig_list    *FsmFigure;
   fbfig_list *FbhFigure;
{
  mochasyfinfo      *MochaSyfInfo;
  fbaux_list        *FbhAux;
  fbrin_list        *FbhRin;
  fsmstate_list     *ScanState;
  mochasyfstate     *ScanSyfState;
  mochasyfregstate  *RegArray;
  int                Index;

  MochaSyfInfo = MOCHA_SYF_INFO( FsmFigure );
  FbhAux  = FbhFigure->BEAUX;
  FbhRin  = FbhFigure->BERIN;

  for ( ScanState  = FsmFigure->STATE;
        ScanState != (fsmstate_list *)0;
        ScanState  = ScanState->NEXT )
  {
    ScanSyfState = MOCHA_SYF_STATE( ScanState );

    FbhAux = fbh_addfbaux( FbhAux, ScanSyfState->CURRENT_NAME, 
                           dupablexpr( ScanSyfState->CODE->ABL ), (bddnode *)0, 0 );
    FbhRin = fbh_addfbrin( FbhRin, ScanSyfState->CURRENT_NAME );

    FbhAux = fbh_addfbaux( FbhAux, ScanSyfState->NEXT_NAME, 
                           dupablexpr( ScanSyfState->ABL_TRANS ), (bddnode *)0, 0 );
    FbhRin = fbh_addfbrin( FbhRin, ScanSyfState->NEXT_NAME );
  }

  RegArray = MochaSyfInfo->REG_ARRAY;

  for ( Index = MochaSyfInfo->NUMBER_REG - 1; Index >= 0; Index-- )
  {
    FbhAux = fbh_addfbaux( FbhAux, RegArray[ Index ].NAME_IN,
                           dupablexpr( RegArray[ Index ].ABL ), (bddnode *)0, 0 );
    FbhRin = fbh_addfbrin( FbhRin, RegArray[ Index ].NAME_IN );
  }

  FbhFigure->BERIN = FbhRin;
  FbhFigure->BEAUX = FbhAux;
}

/*------------------------------------------------------------\
|                                                             |
|                       MochaSyfFbh2Beh                       |
|                                                             |
\------------------------------------------------------------*/

static befig_list *MochaSyfFbh2Beh( FbhFigure )

  fbfig_list *FbhFigure;
{
  befig_list    *BehFigure;
  fbrin_list    *FbhRin;
  fbaux_list    *FbhAux;
  fbreg_list    *FbhReg;
  fbbus_list    *FbhBus;
  fbbux_list    *FbhBux;
  fbpor_list    *FbhPor;
  fbout_list    *FbhOut;
  biabl_list    *BehBiAbl;
  fbbiabl_list  *FbhBiAbl;
  binode_list   *BehBiNode;

  BehFigure = beh_addbefig( (befig_list *)0, FbhFigure->NAME );

  for ( FbhPor  = FbhFigure->BEPOR;
        FbhPor != (fbpor_list *)0;
        FbhPor  = FbhPor->NEXT )
  {
    BehFigure->BEPOR = beh_addbepor( BehFigure->BEPOR, FbhPor->NAME,
                                     FbhPor->DIRECTION, FbhPor->TYPE );
  }
  BehFigure->BEPOR = (bepor_list *)reverse( (chain_list *)BehFigure->BEPOR );


  for ( FbhRin  = FbhFigure->BERIN;
        FbhRin != (fbrin_list *)0;
        FbhRin  = FbhRin->NEXT )
  {
    BehFigure->BERIN = beh_addberin( BehFigure->BERIN, FbhRin->NAME );
  }
  BehFigure->BERIN = (berin_list *)reverse( (chain_list *)BehFigure->BERIN );

  for ( FbhAux  = FbhFigure->BEAUX;
        FbhAux != (fbaux_list *)0;
        FbhAux  = FbhAux->NEXT )
  {
    BehFigure->BEAUX = beh_addbeaux( BehFigure->BEAUX, FbhAux->NAME, 
                                     dupablexpr( FbhAux->ABL ), NULL );
  }
  BehFigure->BEAUX = (beaux_list *)reverse( (chain_list *)BehFigure->BEAUX );

  for ( FbhOut  = FbhFigure->BEOUT;
        FbhOut != (fbout_list *)0;
        FbhOut  = FbhOut->NEXT )
  {
    BehFigure->BEOUT = beh_addbeout( BehFigure->BEOUT, FbhOut->NAME, 
                                     dupablexpr( FbhOut->ABL ), NULL );
  }
  BehFigure->BEOUT = (beout_list *)reverse( (chain_list *)BehFigure->BEOUT );

  for ( FbhReg  = FbhFigure->BEREG;
        FbhReg != (fbreg_list *)0;
        FbhReg  = FbhReg->NEXT )
  {
    BehBiAbl  = (biabl_list  *)0;
    BehBiNode = (binode_list *)0;

    for ( FbhBiAbl  = FbhReg->BIABL;
          FbhBiAbl != (fbbiabl_list *)0;
          FbhBiAbl  = FbhBiAbl->NEXT )
    {
      BehBiAbl  = beh_addbiabl( BehBiAbl, FbhBiAbl->LABEL,
                                dupablexpr( FbhBiAbl->CNDABL ),
                                dupablexpr( FbhBiAbl->VALABL ) );
      BehBiNode = beh_addbinode( BehBiNode, NULL, NULL );
    }

    BehBiAbl = (biabl_list *)reverse( (chain_list *)BehBiAbl );

    BehFigure->BEREG = beh_addbereg( BehFigure->BEREG, FbhReg->NAME, BehBiAbl, BehBiNode );
  }
  BehFigure->BEREG = (bereg_list *)reverse( (chain_list *)BehFigure->BEREG );

  for ( FbhBus  = FbhFigure->BEBUS;
        FbhBus != (fbbus_list *)0;
        FbhBus  = FbhBus->NEXT )
  {
    BehBiAbl  = (biabl_list  *)0;
    BehBiNode = (binode_list *)0;

    for ( FbhBiAbl  = FbhBus->BIABL;
          FbhBiAbl != (fbbiabl_list *)0;
          FbhBiAbl  = FbhBiAbl->NEXT )
    {
      BehBiAbl  = beh_addbiabl( BehBiAbl, FbhBiAbl->LABEL,
                                dupablexpr( FbhBiAbl->CNDABL ),
                                dupablexpr( FbhBiAbl->VALABL ) );
      BehBiNode = beh_addbinode( BehBiNode, NULL, NULL );
    }

    BehBiAbl = (biabl_list *)reverse( (chain_list *)BehBiAbl );

    BehFigure->BEBUS = beh_addbebus( BehFigure->BEBUS, FbhBus->NAME,
                                     BehBiAbl, BehBiNode, FbhBus->TYPE );
  }
  BehFigure->BEBUS = (bebus_list *)reverse( (chain_list *)BehFigure->BEBUS );

  for ( FbhBux  = FbhFigure->BEBUX;
        FbhBux != (fbbux_list *)0;
        FbhBux  = FbhBux->NEXT )
  {
    BehBiAbl  = (biabl_list  *)0;
    BehBiNode = (binode_list *)0;

    for ( FbhBiAbl  = FbhBux->BIABL;
          FbhBiAbl != (fbbiabl_list *)0;
          FbhBiAbl  = FbhBiAbl->NEXT )
    {
      BehBiAbl  = beh_addbiabl( BehBiAbl, FbhBiAbl->LABEL,
                                dupablexpr( FbhBiAbl->CNDABL ),
                                dupablexpr( FbhBiAbl->VALABL ) );
      BehBiNode = beh_addbinode( BehBiNode, NULL, NULL );
    }

    BehBiAbl = (biabl_list *)reverse( (chain_list *)BehBiAbl );

    BehFigure->BEBUX = beh_addbebux( BehFigure->BEBUX, FbhBux->NAME,
                                     BehBiAbl, BehBiNode, FbhBux->TYPE );
  }
  BehFigure->BEBUX = (bebux_list *)reverse( (chain_list *)BehFigure->BEBUX );

  return( BehFigure );
}

/*------------------------------------------------------------\
|                                                             |
|                       MochaSyfFsm2Beh                       |
|                                                             |
\------------------------------------------------------------*/

befig_list *MochaSyfFsm2Beh( FsmFigure )

  fsmfig_list   *FsmFigure;
{
  fsmfig_list *ScanFigure;
  chain_list  *ScanChain;
  fbfig_list  *FbhFigure;
  befig_list  *BehFigure;

  if ( ! IsFsmFigMixedRtl( FsmFigure ) )
  {
    FbhFigure = fbh_addfbfig( (fbfig_list *)0, FsmFigure->NAME );
    FsmFigure->FIGURE = (void *)FbhFigure;
  }
  else
  {
    FbhFigure = (fbfig_list *)FsmFigure->FIGURE;
  }

  MochaSyfFsmTreatPort( FsmFigure, FbhFigure );

  for ( ScanChain  = FsmFigure->MULTI;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    ScanFigure = (fsmfig_list *)ScanChain->DATA;

    MochaSyfFsmTreatOutput( ScanFigure, FbhFigure );
    MochaSyfFsmTreatRegister( ScanFigure, FbhFigure );
    MochaSyfFsmTreatAux( ScanFigure, FbhFigure );
  }

  BehFigure = MochaSyfFbh2Beh( FbhFigure );

  return( BehFigure );
}

/*------------------------------------------------------------\
|                                                             |
|                       MochaSyfFreeFsm                       |
|                                                             |
\------------------------------------------------------------*/

void MochaSyfFreeFsm( FsmFigure )

  fsmfig_list   *FsmFigure;
{
  chain_list    *ScanChain;
  fsmfig_list   *ScanFigure;
  fsmout_list   *ScanOut;
  fsmstate_list *ScanState;
  mochasyfstate *ScanMochaSyfState;
  mochasyfinfo  *ScanMochaSyfInfo;

  for ( ScanChain  = FsmFigure->MULTI;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    ScanFigure       = (fsmfig_list *)ScanChain->DATA;
    ScanMochaSyfInfo = MOCHA_SYF_INFO( ScanFigure );

    MochaSyfSynthFreeFsmAbl( ScanFigure );

    for ( ScanOut  = ScanFigure->OUT;
          ScanOut != (fsmout_list *)0;
          ScanOut  = ScanOut->NEXT )
    {
      MOCHA_SYF_OUT_L( ScanOut ) = NULL;
    }

    for ( ScanState  = ScanFigure->STATE;
          ScanState != (fsmstate_list *)0;
          ScanState  = ScanState->NEXT )
    {
      ScanMochaSyfState = MOCHA_SYF_STATE( ScanState );
      autfreeblock( (void *)ScanMochaSyfState );
      MOCHA_SYF_STATE_L( ScanState ) = NULL;
    }

    autfreeblock( ScanMochaSyfInfo->REG_ARRAY  );
    autfreeblock( ScanMochaSyfInfo->OUT_ARRAY  );
    autfreeblock( ScanMochaSyfInfo->CODE_ARRAY );

    autfreeblock( ScanMochaSyfInfo );
    MOCHA_SYF_INFO_L( ScanFigure ) = NULL;

    if ( IsFsmFigMulti( FsmFigure ) )
    {
      delfsmfig( ScanFigure->NAME );
    }
  }

  if ( FsmFigure->FIGURE != (void *)0 )
  {
    fbh_frefbfig( (fbfig_list *)FsmFigure->FIGURE );
  }

  if ( ! IsFsmFigMulti( FsmFigure ) )
  {
    delfsmfig( FsmFigure->NAME );
  }
}
