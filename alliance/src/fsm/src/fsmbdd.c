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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     Fsm                           |
|                                                             |
| File    :                   fsmbdd.c                        |
|                                                             |
| Date    :                   04.12.96                        |
|                                                             |
| Author  :               Jacomme Ludovic                     |
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

# include <stdio.h>
# include "fsmbdd.h"
# include "fsmerror.h"

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
|                           makefsmbddnode                    |
|                                                             |
\------------------------------------------------------------*/

void makefsmbddnode( FsmFigure )

  fsmfig_list *FsmFigure;
{
  fsmstate_list  *ScanState;
  fsmtrans_list  *ScanTrans;
  fsmstack_list  *ScanStack;
  fsmlocout_list *ScanLocout;

  setbddlocalcircuit( FsmFigure->CIRCUIT );
  setbddlocalsystem( BddLocalCircuit->BDD_SYSTEM );

  for ( ScanTrans  = FsmFigure->TRANS;
        ScanTrans != (fsmtrans_list *)0;
        ScanTrans  = ScanTrans->NEXT )
  {
    if ( ScanTrans->ABL != (ablexpr *)0 )
    {
      ScanTrans->BDD = addbddcircuitabl( (bddcircuit *)0, ScanTrans->ABL );
    }
  }

  for ( ScanStack  = FsmFigure->STACK;
        ScanStack != (fsmstack_list *)0;
        ScanStack  = ScanStack->NEXT )
  {
    if ( ScanStack->ABL != (ablexpr *)0 )
    {
      ScanStack->BDD = addbddcircuitabl( (bddcircuit *)0, ScanStack->ABL );
    }
  }

  ScanState = FsmFigure->STATE;

  while ( ScanState != (fsmstate_list *)0 )
  {
    for ( ScanLocout  = ScanState->LOCOUT;
          ScanLocout != (fsmlocout_list *)0;
          ScanLocout  = ScanLocout->NEXT )
    {
      if ( ScanLocout->ABL != (ablexpr *)0 )
      {
        ScanLocout->BDD = addbddcircuitabl( (bddcircuit *)0, ScanLocout->ABL );
      }

      if ( ScanLocout->ABL_DC != (ablexpr *)0 )
      {
        ScanLocout->BDD_DC = addbddcircuitabl( (bddcircuit *)0, ScanLocout->ABL_DC );
      }
    }

    ScanState  = ScanState->NEXT;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        convertfsmbddnodeabl                 |
|                                                             |
\------------------------------------------------------------*/

void convertfsmbddnodeabl( FsmFigure )

  fsmfig_list *FsmFigure;
{
  fsmstate_list  *ScanState;
  fsmtrans_list  *ScanTrans;
  fsmstack_list  *ScanStack;
  fsmlocout_list *ScanLocout;

  setbddlocalcircuit( FsmFigure->CIRCUIT );
  setbddlocalsystem( BddLocalCircuit->BDD_SYSTEM );

  for ( ScanTrans  = FsmFigure->TRANS;
        ScanTrans != (fsmtrans_list *)0;
        ScanTrans  = ScanTrans->NEXT )
  {
    if ( ScanTrans->BDD != (bddnode *)0 )
    {
      if ( ScanTrans->ABL != (ablexpr *)0 )
      {
        freeablexpr( ScanTrans->ABL );
      }

      ScanTrans->ABL = convertbddcircuitabl( (bddcircuit *)0, ScanTrans->BDD );
    }
  }

  for ( ScanStack  = FsmFigure->STACK;
        ScanStack != (fsmstack_list *)0;
        ScanStack  = ScanStack->NEXT )
  {
    if ( ScanStack->BDD != (bddnode *)0 )
    {
      if ( ScanStack->ABL != (ablexpr *)0 )
      {
        freeablexpr( ScanStack->ABL );
      }
      
      ScanStack->ABL = convertbddcircuitabl( (bddcircuit *)0, ScanStack->BDD );
    }
  }

  ScanState = FsmFigure->STATE;

  while ( ScanState != (fsmstate_list *)0 )
  {
    for ( ScanLocout  = ScanState->LOCOUT;
          ScanLocout != (fsmlocout_list *)0;
          ScanLocout  = ScanLocout->NEXT )
    {
      if ( ScanLocout->BDD != (bddnode *)0 )
      {
        if ( ScanLocout->ABL != (ablexpr *)0 )
        {
          freeablexpr( ScanLocout->ABL );
        }

        ScanLocout->ABL = convertbddcircuitabl( (bddcircuit *)0, ScanLocout->BDD );
      }

      if ( ScanLocout->BDD_DC != (bddnode *)0 )
      {
        if ( ScanLocout->ABL_DC != (ablexpr *)0 )
        {
          freeablexpr( ScanLocout->ABL_DC );
        }

        ScanLocout->ABL_DC = convertbddcircuitabl( (bddcircuit *)0, ScanLocout->BDD_DC );
      }
    }

    ScanState  = ScanState->NEXT;
  }
}
