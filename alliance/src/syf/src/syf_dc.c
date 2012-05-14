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
| Tool    :                     SYF                           |
|                                                             |
| File    :                  syf_synth.c                      |
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

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "syf_fsm.h"
# include "syf_error.h"
# include "syf_env.h"
# include "syf_dc.h"

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
|                     SyfSynthCodeDc2Abl                      |
|                                                             |
\------------------------------------------------------------*/

void SyfSynthCodeDc2Abl( FsmFigure )

  fsmfig_list *FsmFigure;
{
  syfinfo     *SyfInfo;
  syfcode     *CodeArray;
  syfregstate *RegArray;
  bddsystem   *BddSystem;
  bddcircuit  *BddCircuit;
  bddnode     *BddNode;
  bddnode     *BddNodeDc;
  bddnode     *BddNodeOut;
  bddnode     *BddNodeOutDc;
  bddnode     *BddSimp;
  bddnode     *BddNot;
  long         NodeSize;
  long         SimpSize;
  long         Index;
  long         BitMask;
  long         ScanBit;
  long         NumberIn;
  char         DecodeOut;

  SyfInfo   = FSM_SYF_INFO( FsmFigure );
  CodeArray = SyfInfo->CODE_ARRAY;
  RegArray  = SyfInfo->REG_ARRAY;
  DecodeOut = ( SyfInfo->REG_OUT ) && ( SyfInfo->STACK );
  NumberIn  = SyfInfo->NUMBER_REG;

  if ( DecodeOut )
  {
    NumberIn = NumberIn << 1;
  }

  BddSystem  = createbddsystem( SYF_BDD_VAR_NODE, SYF_BDD_OPER_NODE, NumberIn, SYF_BDD_MAX_NODE );
  BddCircuit = createbddcircuit( FsmFigure->NAME, NumberIn, 1, BddSystem );

  for ( ScanBit = 0; ScanBit < SyfInfo->NUMBER_REG; ScanBit++ )
  {
    BddNode = addbddcircuitin( (bddcircuit *)0, RegArray[ ScanBit ].NAME_OUT,
                               (bddindex    )0, BDD_IN_MODE_LAST );

    RegArray[ ScanBit ].NODE_OUT = BddNode;

    if ( DecodeOut ) 
    {
      BddNode = addbddcircuitin( (bddcircuit *)0, RegArray[ ScanBit ].NAME_IN,
                                 (bddindex    )0, BDD_IN_MODE_LAST );

      RegArray[ ScanBit ].NODE_IN = BddNode;
    }
  }

  BddNodeDc    = BddSystem->ZERO;
  BddNodeOutDc = BddSystem->ZERO;

  for ( Index = 0; Index < SyfInfo->NUMBER_CODE; Index++ )
  {
    BddNode    = BddSystem->ONE;
    BddNodeOut = BddSystem->ONE;
    BitMask    = 1;

    for ( ScanBit = 0; ScanBit < SyfInfo->NUMBER_REG; ScanBit++ )
    {
      if ( CodeArray[ Index ].VALUE & BitMask )
      {
        BddNode = applybddnode( (bddsystem *)0, ABL_AND,
                                decbddrefext( BddNode ),
                                RegArray[ ScanBit ].NODE_OUT );
        
        if ( DecodeOut )
        {
          BddNodeOut = applybddnode( (bddsystem *)0, ABL_AND,
                                     decbddrefext( BddNodeOut ),
                                     RegArray[ ScanBit ].NODE_IN );
        }
      }
      else
      {
        BddNot  = applybddnodenot( (bddsystem *)0, RegArray[ ScanBit ].NODE_OUT );
        BddNode = applybddnode( (bddsystem *)0, ABL_AND, 
                                decbddrefext( BddNode ),
                                decbddrefext( BddNot  ) );
        if ( DecodeOut )
        {
          BddNot     = applybddnodenot( (bddsystem *)0, RegArray[ ScanBit ].NODE_IN );
          BddNodeOut = applybddnode( (bddsystem *)0, ABL_AND,
                                     decbddrefext( BddNodeOut ), 
                                     decbddrefext( BddNot  ) );
        }
      }

      BitMask = BitMask << 1;
    }

    if ( ! CodeArray[ Index ].USED )
    {
      BddNodeDc = applybddnode( (bddsystem *)0, ABL_OR,
                                decbddrefext( BddNodeDc ), BddNode );

      if ( DecodeOut )
      {
        BddNodeOutDc = applybddnode( (bddsystem *)0, ABL_OR,
                                     decbddrefext( BddNodeOutDc ), BddNodeOut );
      }
    }
    else
    {
      CodeArray[ Index ].ABL = (chain_list *)BddNode;

      if ( DecodeOut )
      {
        CodeArray[ Index ].ABL_OUT = (chain_list *)BddNodeOut;
      }
    }
  }

  for ( Index = 0; Index < SyfInfo->NUMBER_CODE; Index++ )
  {
    if ( CodeArray[ Index ].USED )
    {
      BddNode = (bddnode *)CodeArray[ Index ].ABL;
      BddSimp = simpbddnodedcoff( (bddsystem *)0, BddNode, BddNodeDc );

      SimpSize = getbddnodesize( (bddsystem *)0, BddSimp );
      NodeSize = getbddnodesize( (bddsystem *)0, BddNode );

      if ( SimpSize < NodeSize )
      {
        BddNode = BddSimp;
      }

      CodeArray[ Index ].ABL = 

         convertbddcircuitabl( (bddcircuit *)0, BddNode );

      if ( DecodeOut )
      {
        BddNodeOut = (bddnode *)CodeArray[ Index ].ABL_OUT;
        BddSimp    = simpbddnodedcoff( (bddsystem *)0, BddNodeOut, BddNodeOutDc );

        SimpSize = getbddnodesize( (bddsystem *)0, BddSimp    );
        NodeSize = getbddnodesize( (bddsystem *)0, BddNodeOut );

        if ( SimpSize < NodeSize )
        {
          BddNodeOut = BddSimp;
        }

        CodeArray[ Index ].ABL_OUT = 

         convertbddcircuitabl( (bddcircuit *)0, BddNodeOut );
      }
    }
  }

  destroybddcircuit( BddCircuit );
  destroybddsystem( BddSystem );
}
