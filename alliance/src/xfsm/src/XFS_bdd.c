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
| Tool    :                   XFSM                            |
|                                                             |
| File    :                   Bdd.c                           |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   04.12.96                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <string.h>
# include <setjmp.h>
# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "ftl.h"
# include "XSB.h" 
# include "XFS.h"
# include "XFS_bdd.h"

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
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        XfsmMakeBddCircuit                   |
|                                                             |
\------------------------------------------------------------*/

bddcircuit *XfsmMakeBddCircuit( BddSystem, FsmFigure )

  bddsystem   *BddSystem;
  fsmfig_list *FsmFigure;
{
  fsmin_list  *ScanIn;
  fsmout_list *ScanOut;
  bddcircuit  *BddCircuit;

  BddCircuit = createbddcircuit( FsmFigure->NAME,
                                 FsmFigure->NUMBER_IN + FsmFigure->NUMBER_OUT,
                                 FsmFigure->NUMBER_STATE, BddSystem );

  for ( ScanIn  = FsmFigure->IN;
        ScanIn != (fsmin_list *)0;
        ScanIn  = ScanIn->NEXT )
  {
    addbddcircuitin( (bddcircuit *)0, ScanIn->NAME,
                     (bddindex    )0, BDD_IN_MODE_LAST );
  }

  for ( ScanOut  = FsmFigure->OUT;
        ScanOut != (fsmout_list *)0;
        ScanOut  = ScanOut->NEXT )
  {
    addbddcircuitin( (bddcircuit *)0, ScanOut->NAME,
                     (bddindex    )0, BDD_IN_MODE_LAST );
  }

  addbddcircuitin( (bddcircuit *)0, "'d'",
                   (bddindex    )0, BDD_IN_MODE_LAST );

  FsmFigure->CIRCUIT = BddCircuit;

  makefsmbddnode( FsmFigure );

  return( BddCircuit );
}

/*------------------------------------------------------------\
|                                                             |
|                        XfsmSimplifyFigure                   |
|                                                             |
\------------------------------------------------------------*/

void XfsmSimplifyFigure( FsmFigure )

  fsmfig_list *FsmFigure;
{
  bddsystem      *BddSystem;
  bddcircuit     *BddCircuit;

  BddSystem = createbddsystem( XFSM_BDD_VAR_NODE, 
                               XFSM_BDD_OPER_NODE, 1000, XFSM_BDD_MAX_NODE );

  reorderbddsystemdynamic( BddSystem, XFSM_BDD_REORDER_FUNC,
                           XFSM_BDD_REORDER_LOW, XFSM_BDD_REORDER_RATIO );

  BddCircuit = XfsmMakeBddCircuit( BddSystem, FsmFigure );
  convertfsmbddnodeabl( FsmFigure );

  destroybddcircuit( BddCircuit );
  destroybddsystem( BddSystem );

   FsmFigure->CIRCUIT = (bddcircuit *)0;
}
