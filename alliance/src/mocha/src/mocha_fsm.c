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
| Tool    :                     MOCHA                         |
|                                                             |
| File    :                  mocha_fsm.c                      |
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
# include "ftl.h"
# include "abe.h"
# include "vex.h"
# include "ctl.h"
# include "ctp.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "mocha_debug.h"
# include "mocha_shared.h"
# include "mocha_fsm.h"
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
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        MochaCompileFsm                      |
|                                                             |
\------------------------------------------------------------*/

void MochaCompileFsm( MochaFigure, FileName, FlagVerbose )

  mochafig_list *MochaFigure;
  char          *FileName;
  int            FlagVerbose;
{
  fsmfig_list     *FsmFigure;
  fsmfig_list     *ScanFigure;
  befig_list      *BehFigure;
  chain_list      *ScanChain;
  mochafsm_list   *MochaFsm;
  fsmstate_list   *ScanState;
  mochastate_list *MochaState;
  mochasyfinfo    *MochaSyfInfo;
  mochasyfstate   *MochaSyfState;

  FsmFigure = MochaSyfFbhParse( FileName );

  for ( ScanChain  = FsmFigure->MULTI;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    FsmFigure = (fsmfig_list *)ScanChain->DATA;
    MochaSyfFsmSimplify( FsmFigure );
  }

  if ( FlagVerbose )
  {
    if ( IsFsmFigMixedRtl( FsmFigure ) )
    {
      fprintf( stdout, "\t\tMixed DataFlow / Fsm\n\n" );
    }
  }

  for ( ScanChain  = FsmFigure->MULTI;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    ScanFigure = (fsmfig_list *)ScanChain->DATA;

    if ( FlagVerbose )
    {
      fprintf( stdout, "\t    --> Encoding FSM figure %s\n", ScanFigure->NAME );
    }

    MochaSyfFsmEncode( ScanFigure, FlagVerbose );
  }

  for ( ScanChain  = FsmFigure->MULTI;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    ScanFigure = (fsmfig_list *)ScanChain->DATA;
    MochaSyfSynthFsm( ScanFigure );
  }
 
  BehFigure = MochaSyfFsm2Beh( FsmFigure );

  MochaFigure->BEH_FIGURE = BehFigure;
/*
** Extract FSM informations for Mocha, such as state encoding ...
*/
  for ( ScanChain  = FsmFigure->MULTI;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    ScanFigure   = (fsmfig_list *)ScanChain->DATA;
    MochaSyfInfo = MOCHA_SYF_INFO( ScanFigure );

    MochaFsm                = MochaAddFsm( MochaFigure, ScanFigure->NAME );
    MochaFsm->NUMBER_STATE  = ScanFigure->NUMBER_STATE;
    MochaFsm->CURRENT_STATE = MochaSyfInfo->CURRENT_STATE;
    MochaFsm->NEXT_STATE    = MochaSyfInfo->NEXT_STATE;
    MochaFsm->NUMBER_BIT    = MochaSyfInfo->NUMBER_BIT;
    MochaFsm->LEFT          = MochaFsm->NUMBER_BIT - 1;
    MochaFsm->RIGHT         = 0;

    for ( ScanState  = ScanFigure->STATE;
          ScanState != (fsmstate_list *)0;
          ScanState  = ScanState->NEXT )
    {
      MochaSyfState = MOCHA_SYF_STATE( ScanState );
      MochaState    = MochaAddState( MochaFigure, MochaFsm, ScanState->NAME,
                                     MochaSyfState->CODE->VALUE );
    }
  }
/*
** Destroy now all syf and fsm data structure !
*/
  MochaSyfFreeFsm( FsmFigure );
}
