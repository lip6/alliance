/*-----------------------------------------------------------\
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
| Tool    :                     BOOM                          |
|                                                             |
| File    :                   boom_dc.c                       |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   02.11.01                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/
                                                                                    
#include "mut.h"
#include "aut.h"
#include "abl.h"
#include "bdd.h"
#include "abe.h"
#include "abv.h"
#include "abt.h"
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "boom_error.h"
#include "boom_debug.h"
#include "boom_shared.h"
#include "boom_aux.h"
#include "boom_abl.h"
#include "boom_bdd.h"
#include "boom_cost.h"
#include "boom_optim.h"
#include "boom_auxinit.h"
#include "boom_dc.h"
 

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

  static bddnode *BoomBddDontCare = (bddnode *)0;
  static int      BoomFoundAblDc  = 0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                         				      |
|                        BoomReplaceDontCareAbl               |
|                                                             |
\------------------------------------------------------------*/

static void BoomReplaceDontCareAbl( Expr )

  ablexpr *Expr;
{
  if ( ABL_ATOM( Expr ) )
  {
    if ( ABL_ATOM_VALUE( Expr ) == ABL_ATOM_NAME_DC )
    {
      BoomFoundAblDc = 1;
      ABL_CAR_L( Expr ) = ABL_ATOM_NAME_ZERO;
    }
  }

  while ( ( Expr = ABL_CDR( Expr ) ) )
  {
    BoomReplaceDontCareAbl( ABL_CAR( Expr ) );
  }
}

/*------------------------------------------------------------\
|                         				      |
|                        BoomOptimDontCareAbl                 |
|                                                             |
\------------------------------------------------------------*/

static ablexpr *BoomOptimDontCareAbl( Expr )

  ablexpr *Expr;
{
  BoomFoundAblDc = 0;
  BoomReplaceDontCareAbl( Expr );

  if ( BoomFoundAblDc )
  {
    Expr = simpablexpr( Expr );
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                         				      |
|                        BoomOptimDontCareBdd                 |
|                                                             |
\------------------------------------------------------------*/

static bddnode *BoomOptimDontCareBdd( BddNode )

  bddnode *BddNode;
{
  bddnode *BddOnSet;
  bddnode *BddOffSet;
  bddnode *BddDcSet;
  bddnode *BddOptim;
  bddnode *BddOptimOn;
  bddnode *BddOptimOff;
  bddvar   VarDontCare;
  int      OnSize;
  int      OptimSize;
  int      OptimSizeOn;
  int      OptimSizeOff;

  VarDontCare = getbddvarbyindex( (bddsystem *)0, BoomBddDontCare->INDEX );

  BddOnSet = restrictbddnode( (bddsystem *)0, BddNode, VarDontCare,
                              BddLocalSystem->ZERO );

  if ( BddOnSet == BddNode )
  {
    decbddrefext( BddOnSet );

    return ( BddNode );
  }

  BddOffSet = applybddnodenot( (bddsystem *)0, BddOnSet );

  BddDcSet = restrictbddnode( (bddsystem *)0, BddNode, VarDontCare,
                              BddLocalSystem->ONE  );
  BddDcSet = applybddnode( (bddsystem *)0, ABL_AND, decbddrefext( BddDcSet ), BddOffSet );

  BddOptimOn   = simpbddnodedcon(  (bddsystem *)0, BddOnSet, BddDcSet );
  BddOptimOff  = simpbddnodedcoff( (bddsystem *)0, BddOnSet, BddDcSet );

  decbddrefext( BddOnSet    );
  decbddrefext( BddOffSet   );
  decbddrefext( BddDcSet    );
  decbddrefext( BddOptimOn  );
  decbddrefext( BddOptimOff );
  decbddrefext( BddNode     );

  OnSize       = getbddnodenum( (bddsystem *)0, BddOnSet    );
  OptimSizeOn  = getbddnodenum( (bddsystem *)0, BddOptimOn  );
  OptimSizeOff = getbddnodenum( (bddsystem *)0, BddOptimOff );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "OnSize %d, OptimSizeOn %d, OptimSizeOff %d\n",
                OnSize, OptimSizeOn, OptimSizeOff );
  }

  if ( OptimSizeOn > OptimSizeOff )
  {
    BddOptim  = BddOptimOff; 
    OptimSize = OptimSizeOff;
  }
  else
  {
    BddOptim = BddOptimOn;
    OptimSize = OptimSizeOn;
  }

  if ( OptimSize < OnSize ) BddNode = incbddrefext( BddOptim );
  else                      BddNode = incbddrefext( BddOnSet );

  return( BddNode );
}

/*------------------------------------------------------------\
|                         				      |
|                        BoomOptimDontCare                    |
|                                                             |
\------------------------------------------------------------*/        

void BoomOptimDontCare( BehFigure )

  befig_list *BehFigure;     
{
  boombefiginfo *FigInfo;

  FigInfo = GetBoomBehFigInfo( BehFigure );

  BoomBddDontCare = FigInfo->BDD_DONT_CARE;

  BoomBehMapBdd( BehFigure, BoomOptimDontCareBdd );

  garbagebddsystem( (bddsystem *)0 );

  BoomBehMapAbl( BehFigure, BoomOptimDontCareAbl, 1 );
}
