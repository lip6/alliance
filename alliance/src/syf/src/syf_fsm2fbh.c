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
| Tool    :                     SYF                           |
|                                                             |
| File    :                 syf_fsm2fbh.c                     |
|                                                             |
| Authors :                  C. Sarwary                       |
|                     Modified by Jacomme Ludovic             |
|                                                             |
| Date    :                   01.11.94                        |
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
# include "fsm.h"
# include "fvh.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "syf_fsm.h"
# include "syf_error.h"
# include "syf_fsm2fbh.h"

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
|                       Private Variables                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       SyfFsmTreatPort                       |
|                                                             |
\------------------------------------------------------------*/

void SyfFsmTreatPort( FsmFigure, FbhFigure )

   fsmfig_list *FsmFigure;
   fbfig_list  *FbhFigure;
{
  syfinfo         *SyfInfo;
  fbpor_list      *FbhPort;
  fbrin_list      *FbhRin;
  fsmport_list    *ScanPort;

  if ( IsFsmFigMixedRtl( FsmFigure ) )
  {
    return;
  }

  SyfInfo = FSM_SYF_INFO( FsmFigure );
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
|                       SyfFsmTreatOutput                     |
|                                                             |
\------------------------------------------------------------*/

void SyfFsmTreatOutput( FsmFigure, FbhFigure )

   fsmfig_list    *FsmFigure;
   fbfig_list *FbhFigure;
{
  syfinfo        *SyfInfo;
  fsmout_list    *ScanOut;
  syfout         *ScanSyfOut;
  syfregout      *OutArray;
  fbout_list     *FbhOut;
  fbrin_list     *FbhRin;
  fbreg_list     *FbhReg;
  biabl_list     *FbhBiAbl;
  binode_list    *FbhBiNode;
  ablexpr        *Equation;
  ablexpr        *AblExpr;
  long            Index;

  SyfInfo   = FSM_SYF_INFO( FsmFigure );
  OutArray  = SyfInfo->OUT_ARRAY;

  FbhOut    = FbhFigure->BEOUT;
  FbhRin    = FbhFigure->BERIN;
  FbhReg    = FbhFigure->BEREG;


  for ( ScanOut  = FsmFigure->OUT;
        ScanOut != (fsmout_list *)0;
        ScanOut  = ScanOut->NEXT )
  {
    ScanSyfOut = FSM_SYF_OUT( ScanOut );

    if ( ScanSyfOut->ABL != (chain_list *)0 )
    {
      AblExpr = dupablexpr( ScanSyfOut->ABL );

      if ( ScanSyfOut->POLARITY )
      {
        if ( ! SyfInfo->REG_OUT )
        {
          AblExpr = optimablnotexpr( AblExpr );
        }
        else
        {
          Equation = OutArray[ ScanSyfOut->INDEX ].ABL;
          Equation = optimablnotexpr( Equation );
          OutArray[ ScanSyfOut->INDEX ].ABL = Equation;
        }
      }

      FbhOut = fbh_addfbout( FbhOut, ScanOut->NAME, AblExpr, (bddnode *)0, 0 );
    }
  }

  if ( SyfInfo->REG_OUT )
  {
    for ( Index = 0; Index < SyfInfo->NUMBER_OUT; Index++ )
    {
      if ( OutArray[ Index ].ABL == (chain_list *)0 ) continue;
    
      Equation = dupablexpr( FsmFigure->CLOCK_ABL );
      AblExpr  = dupablexpr( OutArray[ Index ].ABL );

      FbhBiAbl = fbh_addbiabl( (biabl_list *)0,
                                    OutArray[ Index ].NAME_MASTER, Equation, AblExpr );
  
      FbhBiNode = fbh_addbinode( (binode_list *)0,
                                     (bddnode *)0, 
                                     (bddnode *)0 );

      FbhReg = fbh_addfbreg( FbhReg, OutArray[ Index ].NAME_MASTER,
                             FbhBiAbl, FbhBiNode );

      FbhRin = fbh_addfbrin( FbhRin, OutArray[ Index ].NAME_MASTER );
    }

    FbhFigure->BEREG = FbhReg;
  }

  FbhFigure->BERIN = FbhRin;
  FbhFigure->BEOUT = FbhOut;
}

/*------------------------------------------------------------\
|                                                             |
|                       SyfFsmTreatRegister                   |
|                                                             |
\------------------------------------------------------------*/

void SyfFsmTreatRegister( FsmFigure, FbhFigure )

   fsmfig_list    *FsmFigure;
   fbfig_list *FbhFigure;
{
  syfinfo         *SyfInfo;
  syfregstate     *RegArray;
  syfregstack     *StackArray;
  int              Index;
  fbreg_list      *FbhReg;
  fbrin_list      *FbhRin;
  biabl_list      *FbhBiAbl;
  binode_list     *FbhBiNode;
  chain_list      *Equation;

  SyfInfo  = FSM_SYF_INFO( FsmFigure );
  RegArray = SyfInfo->REG_ARRAY;
  FbhReg   = FbhFigure->BEREG;
  FbhRin   = FbhFigure->BERIN;

  for ( Index = SyfInfo->NUMBER_REG - 1; Index >= 0; Index-- )
  {
    Equation = dupablexpr( FsmFigure->CLOCK_ABL );
    FbhBiAbl = fbh_addbiabl( (biabl_list *)0,
                             RegArray[ Index ].NAME_MASTER,
                             Equation,
                             dupablexpr( RegArray[ Index ].ABL_IN ) );

    FbhBiNode = fbh_addbinode( (binode_list *)0, (bddnode *)0, (bddnode *)0 );

    FbhReg = fbh_addfbreg( FbhReg, RegArray[ Index ].NAME_MASTER,
                           FbhBiAbl, FbhBiNode );

    FbhRin = fbh_addfbrin( FbhRin, RegArray[ Index ].NAME_MASTER );
  }

  if ( SyfInfo->STACK )
  {
    StackArray = SyfInfo->STACK_ARRAY;

    for ( Index = SyfInfo->NUMBER_STACK -1; Index >= 0; Index-- )
    {
      if ( ! StackArray[ Index ].FLAGS )
      {
        Equation = dupablexpr( FsmFigure->CLOCK_ABL );
        FbhBiAbl = fbh_addbiabl( (biabl_list *)0,
                                      StackArray[ Index ].NAME_MASTER,
                                      Equation,
                                      dupablexpr( StackArray[ Index ].ABL ) );

        FbhBiNode = fbh_addbinode( (binode_list *)0,
                                       (bddnode *)0,
                                       (bddnode *)0 );

        FbhReg = fbh_addfbreg( FbhReg, StackArray[ Index ].NAME_MASTER,
                               FbhBiAbl, FbhBiNode );

        FbhRin = fbh_addfbrin( FbhRin, StackArray[ Index ].NAME_MASTER );
      }
    }
  }

  FbhFigure->BERIN = FbhRin;
  FbhFigure->BEREG = FbhReg;
}

/*------------------------------------------------------------\
|                                                             |
|                       SyfFsmTreatAux                        |
|                                                             |
\------------------------------------------------------------*/

void SyfFsmTreatAux( FsmFigure, FbhFigure )

   fsmfig_list    *FsmFigure;
   fbfig_list *FbhFigure;
{
  syfinfo         *SyfInfo;
  fbaux_list      *FbhAux;
  fbrin_list      *FbhRin;
  fsmstate_list   *ScanState;
  syfstate        *ScanSyfState;
  syfctrl         *CtrlArray;
  syfregstack     *StackArray;
  syfregstate     *RegArray;
  int              Index;

  SyfInfo = FSM_SYF_INFO( FsmFigure );
  FbhAux  = FbhFigure->BEAUX;
  FbhRin  = FbhFigure->BERIN;

  if ( SyfInfo->STACK )
  {
    StackArray = SyfInfo->STACK_ARRAY;

    for ( Index = SyfInfo->NUMBER_REG - 1; Index >= 0; Index-- )
    {
      if ( StackArray[ Index ].FLAGS )
      {
        FbhAux = fbh_addfbaux( FbhAux, StackArray[ Index ].NAME_OUT,
                               dupablexpr( StackArray[ Index ].ABL ), (bddnode *)0, 0 );
      }
    }

    CtrlArray = SyfInfo->CTRL_ARRAY;

    for ( Index = 0; Index < SyfInfo->NUMBER_CTRL; Index++ )
    {
      FbhAux = fbh_addfbaux( FbhAux, CtrlArray[ Index ].NAME,
                             dupablexpr( CtrlArray[ Index ].ABL ), (bddnode *)0, 0 );

      FbhRin = fbh_addfbrin( FbhRin, CtrlArray[ Index ].NAME );
    }
  }

  for ( ScanState  = FsmFigure->STATE;
        ScanState != (fsmstate_list *)0;
        ScanState  = ScanState->NEXT )
  {
    ScanSyfState = FSM_SYF_STATE( ScanState );

    if ( ( SyfInfo->STACK   ) && 
         ( SyfInfo->REG_OUT ) )
    {
      FbhAux = fbh_addfbaux( FbhAux, ScanSyfState->NEXT_IN_NAME, 
                             dupablexpr( ScanSyfState->CODE->ABL_OUT ), (bddnode *)0, 0 );
      FbhRin = fbh_addfbrin( FbhRin, ScanSyfState->NEXT_IN_NAME );
    }

    FbhAux = fbh_addfbaux( FbhAux, ScanSyfState->CURRENT_NAME, 
                           dupablexpr( ScanSyfState->CODE->ABL ), (bddnode *)0, 0 );
    FbhRin = fbh_addfbrin( FbhRin, ScanSyfState->CURRENT_NAME );

    FbhAux = fbh_addfbaux( FbhAux, ScanSyfState->NEXT_NAME, 
                           dupablexpr( ScanSyfState->ABL_TRANS ), (bddnode *)0, 0 );
    FbhRin = fbh_addfbrin( FbhRin, ScanSyfState->NEXT_NAME );

    if ( ScanSyfState->ABL_RETURN != (chain_list *)0 )
    {
      FbhAux = fbh_addfbaux( FbhAux, ScanSyfState->RETURN_NAME,
                             dupablexpr( ScanSyfState->ABL_RETURN ), (bddnode *)0, 0 );
      FbhRin = fbh_addfbrin( FbhRin, ScanSyfState->RETURN_NAME );
    }
  }

  RegArray = SyfInfo->REG_ARRAY;

  for ( Index = SyfInfo->NUMBER_REG - 1; Index >= 0; Index-- )
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
|                          SyfFsm2Fbh                         |
|                                                             |
\------------------------------------------------------------*/

fbfig_list *SyfFsm2Fbh( FsmFigure )

  fsmfig_list *FsmFigure;
{
  fbfig_list *FbhFigure;

  if ( ! IsFsmFigMixedRtl( FsmFigure ) )
  {
    FbhFigure = fbh_addfbfig( (fbfig_list *)0, FsmFigure->NAME );
  }
  else
  {
    FbhFigure = (fbfig_list *)FsmFigure->FIGURE;
  }

  SyfFsmTreatPort( FsmFigure, FbhFigure );
  SyfFsmTreatOutput( FsmFigure, FbhFigure );
  SyfFsmTreatRegister( FsmFigure, FbhFigure );
  SyfFsmTreatAux( FsmFigure, FbhFigure );

  return( FbhFigure );
}
