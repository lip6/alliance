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
| File    :                  syf_cost.c                       |
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
# include "syf_env.h"
# include "syf_fsm2fbh.h"
# include "syf_synth.h"
# include "syf_bdd.h"
# include "syf_error.h"
# include "syf_cost.h"

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
|                        SyfFbhAblGiveCost                    |
|                                                             |
\------------------------------------------------------------*/

long SyfFbhAblGiveCost( FbhFigure )

  fbfig_list *FbhFigure;
{
  long Cost;
  fbout_list *FbhOut;
  fbaux_list *FbhAux;
  fbreg_list *FbhReg;
  fbbiabl_list *FbhBiAbl;

  Cost = 0;

  for ( FbhOut  = FbhFigure->BEOUT;
        FbhOut != (fbout_list *)0;
        FbhOut  = FbhOut->NEXT )
  {
    if ( FbhOut->ABL != (chain_list *)0 )
    {
      Cost += getablexprnumatom( FbhOut->ABL );
    }
  }

  for ( FbhAux  = FbhFigure->BEAUX;
        FbhAux != (fbaux_list *)0;
        FbhAux  = FbhAux->NEXT )
  {
    if ( FbhAux->ABL != (chain_list *)0 )
    {
      Cost += getablexprnumatom( FbhAux->ABL );
    }
  }

  for ( FbhReg  = FbhFigure->BEREG;
        FbhReg != (fbreg_list *)0;
        FbhReg  = FbhReg->NEXT )
  {
    for ( FbhBiAbl  = FbhReg->BIABL;
          FbhBiAbl != (fbbiabl_list *)0;
          FbhBiAbl  = FbhBiAbl->NEXT )
    {
      if ( FbhBiAbl->VALABL != (chain_list *)0 )
      {
        Cost += getablexprnumatom( FbhBiAbl->VALABL );
      }

      if ( FbhBiAbl->CNDABL != (chain_list *)0 )
      {
        Cost += getablexprnumatom( FbhBiAbl->CNDABL );
      }
    }
  }

  return( Cost );
}

/*------------------------------------------------------------\
|                                                             |
|                          SyfBddGiveCost                     |
|                                                             |
\------------------------------------------------------------*/

static long SyfBddGiveCost( BddNode )

  bddnode *BddNode;
{
  bddnode *BddNot;

  if ( BddNode->INDEX >= BDD_INDEX_MIN )
  {
    if ( ! ( BddNode->FLAG & SYF_BDD_COST_FLAG ) )
    {
      if ( ( BddNode->HIGH->INDEX >= BDD_INDEX_MIN ) ||
           ( BddNode->LOW->INDEX  >= BDD_INDEX_MIN ) )
      {
        if ( BddNode->REF_INT > 1 )
        {
          BddNode->FLAG |= SYF_BDD_COST_FLAG;

          BddNot = applybddnodenot( (bddsystem *)0, BddNode );

          if ( BddNot->REF_INT > 1 )
          {
            BddNot->FLAG  |= SYF_BDD_COST_FLAG;
          }

          delbddnode( (bddsystem *)0, decbddrefext( BddNot ) );
        }

        return( SyfBddGiveCost( BddNode->LOW  ) + 1 +
                SyfBddGiveCost( BddNode->HIGH ) );
      }
    }

    return( 1 );
  }

  return( 0 );
}
