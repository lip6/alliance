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
| Tool    :                     MOCHA                         |
|                                                             |
| File    :                  mocha_bdd.c                      |
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
# include "mocha_bdd.h"
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

 static befig_list *MochaBddBehFigure = (befig_list *)0;
 static chain_list *MochaBddHeadName  = (chain_list *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

static bddnode *MochaBddAbl2BddNode( Expr )

  ablexpr *Expr;
{
  bddnode    *BddNode;
  bddnode    *BddFirst;
  char       *AtomValue;
  beaux_list *ScanAux;
  chain_list *ScanChain;
  long        Oper;
  int         Negative;

  if ( ABL_ATOM( Expr ) )
  {
    AtomValue = ABL_ATOM_VALUE( Expr );

    if ( AtomValue == ABL_ATOM_NAME_ONE )
    {
      return( BddLocalSystem->ONE );
    }

    if ( ( AtomValue == ABL_ATOM_NAME_ZERO ) ||
         ( AtomValue == ABL_ATOM_NAME_DC   ) )
    {
      return( BddLocalSystem->ZERO );
    }

    BddNode = searchbddcircuitin( (bddcircuit *)0, AtomValue );

    if ( BddNode != (bddnode *)0 )
    {
      return( incbddrefext( BddNode ) );
    }

    for ( ScanAux  = MochaBddBehFigure->BEAUX;
          ScanAux != (beaux_list *)0;
          ScanAux  = ScanAux->NEXT )
    {
      if ( ScanAux->NAME == AtomValue ) break;
    }

    if ( ScanAux == (beaux_list *)0 )
    {
      MochaPrintf( stdout, "Unknown atom %s !\n", AtomValue );
      autexit( 1 );
    }

    for ( ScanChain  = MochaBddHeadName;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      if ( ScanChain->DATA == AtomValue )
      {
        MochaPrintf( stdout, "Loop in BEH figure with symbol %s\n", AtomValue );
        autexit( 1 );
      }
    }

    if ( ScanAux->NODE == (void *)0 )
    {
      MochaBddHeadName = addchain( MochaBddHeadName, AtomValue );
      ScanAux->NODE    = MochaBddAbl2BddNode( ScanAux->ABL );
      MochaBddHeadName = delchain( MochaBddHeadName, MochaBddHeadName );
    }

    return( incbddrefext( ScanAux->NODE ) );
  }

  Oper = ABL_OPER( Expr );
  Expr = ABL_CDR( Expr );

  if ( Oper == ABL_NOT )
  {
    BddFirst = MochaBddAbl2BddNode( ABL_CAR( Expr ) );
    BddNode  = applybddnodenot( (bddsystem *)0, decbddrefext( BddFirst ) );

    return( BddNode );
  }

  if ( Oper == ABL_STABLE )
  {
    Expr = ABL_CAR( Expr );

    if ( ! ABL_ATOM( Expr ) )
    {
      MochaPrintf( stdout, "Illegal use of STABLE attribute !\n" );
      autexit( 1 );
    }

    AtomValue = ABL_ATOM_VALUE( Expr );
    BddFirst  = searchbddcircuitin( (bddcircuit *)0, AtomValue );

    if ( BddFirst == (bddnode *)0 )
    {
      MochaPrintf( stdout, "Atom %s is not a primary input !\n", AtomValue );
      autexit( 1 );
    }

    AtomValue = getbddstablename( AtomValue );

    BddNode = searchbddcircuitin( (bddcircuit *)0, AtomValue );

    if ( BddNode == (bddnode *)0 )
    {
      BddNode = addbddcircuitin( (bddcircuit *)0, AtomValue,
                                 (bddindex    )0, BDD_IN_MODE_LAST );
    }

    BddNode = applybddnode( (bddsystem *)0, ABL_NXOR, BddNode, BddFirst );

    return( BddNode );
  }

  if ( ( getabloperpolar( Oper ) == ABL_POLAR_POSITIVE ) ||
       ( ABL_CDDR( Expr )        == (chain_list *)0    ) )
  {
    Negative = 0;
  }
  else
  {
    Negative = 1;
    Oper     = getablopernot( Oper );
  }

  BddFirst = MochaBddAbl2BddNode( ABL_CAR( Expr ) );

  while ( ( Expr = ABL_CDR( Expr ) ) != (chain_list *)0 )
  {
    BddNode  = MochaBddAbl2BddNode( ABL_CAR( Expr ) );
    BddFirst = applybddnode( (bddsystem *)0, Oper,
                             decbddrefext( BddFirst ),
                             decbddrefext( BddNode  ) );
  }

  if ( Negative )
  {
    BddFirst = applybddnodenot( (bddsystem *)0,
                                decbddrefext( BddFirst ) );
  }

  return( BddFirst );
}

/*------------------------------------------------------------\
|                                                             |
|                        MochaBddAssign2BddNode               |
|                                                             |
\------------------------------------------------------------*/

static bddnode *MochaBddAssign2BddNode( Name, Expr )

  char     *Name;
  ablexpr  *Expr;
{
  bddnode *BddNode;

  if ( Name != (char *)0 )
  {
    MochaBddHeadName = addchain( MochaBddHeadName, Name );
  }

  BddNode = MochaBddAbl2BddNode( Expr );

  if ( Name != (char *)0 )
  {
    MochaBddHeadName = delchain( MochaBddHeadName, MochaBddHeadName );
    BddNode = addbddcircuitout( (bddcircuit *)0, Name, BddNode );
  }

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                        MochaBddBuildBddNode                 |
|                                                             |
\------------------------------------------------------------*/

static void MochaBddBuildBddNode( MochaFigure )

  mochafig_list *MochaFigure;
{
  befig_list  *BehFigure;
  authelem    *Element;
  beaux_list  *BehAux;
  beout_list  *BehOut;
  bereg_list  *BehReg;
  biabl_list  *BiAbl;
  binode_list *BiNode;
  bddnode     *BddNode;

  BehFigure         = MochaFigure->BEH_FIGURE;
  MochaBddBehFigure = BehFigure;
  MochaBddHeadName  = (chain_list *)0;

  for ( BehAux  = BehFigure->BEAUX;
        BehAux != (beaux_list *)0;
        BehAux  = BehAux->NEXT )
  {
    BehAux->NODE = MochaBddAssign2BddNode( BehAux->NAME, BehAux->ABL );
  }

  for ( BehOut  = BehFigure->BEOUT;
        BehOut != (beout_list *)0;
        BehOut  = BehOut->NEXT )
  {
    BehOut->NODE = MochaBddAssign2BddNode( BehOut->NAME, BehOut->ABL );
  }

  BddNode = (bddnode *)0;

  for ( BehReg  = BehFigure->BEREG;
        BehReg != (bereg_list *)0;
        BehReg  = BehReg->NEXT )
  {
    BiAbl  = BehReg->BIABL;
    BiNode = BehReg->BINODE;

    if ( ( BiAbl       == (biabl_list *)0 ) ||
         ( BiAbl->NEXT != (biabl_list *)0 ) )
    {
      MochaPrintf( stdout, "register %s has multiple clock conditions !\n", BehReg->NAME );
      autexit( 1 );
    }

    BiNode->CNDNODE = MochaBddAssign2BddNode( (char *)0   , BiAbl->CNDABL );
    BiNode->VALNODE = MochaBddAssign2BddNode( BehReg->NAME, BiAbl->VALABL );

    if ( BddNode == (bddnode *)0 )
    {
      BddNode = BiNode->CNDNODE;
    }
    else
    if ( BddNode != BiNode->CNDNODE )
    {
      MochaPrintf( stdout, "register %s has not the same clock condition as others !\n",
                   BehReg->NAME );
      autexit( 1 );
    }
  }

  for ( BehAux  = BehFigure->BEAUX;
        BehAux != (beaux_list *)0;
        BehAux  = BehAux->NEXT )
  {
    Element = searchauthelem( MochaFigure->HASH_CTL_SUPPORT, BehAux->NAME );

    if ( Element == (authelem *)0 )
    {
      decbddrefext( BehAux->NODE );
      BehAux->NODE = (bddnode *)0;
      delbddcircuitout( (bddcircuit *)0, BehAux->NAME );
    }
  }

  reorderbddsystemsimple( (bddsystem *)0 );
}


/*------------------------------------------------------------\
|                                                             |
|                        MochaBuildBddCircuit                 |
|                                                             |
\------------------------------------------------------------*/

static void MochaBddBuildCircuit( MochaFigure )

  mochafig_list *MochaFigure;
{
  befig_list  *BehFigure;

  berin_list  *BehRin;
  bddcircuit  *BddCircuit;
  bddsystem   *BddSystem;
  long         NumberIn;
  long         NumberOut;

  BehFigure = MochaFigure->BEH_FIGURE;
  BddSystem = MochaFigure->BDD_SYSTEM;

  NumberIn = 0;

  for ( BehRin   = BehFigure->BERIN;
        BehRin  != (berin_list *)0;
        BehRin   = BehRin->NEXT ) NumberIn++;

  NumberOut = MochaFigure->HASH_BEH_OUT->NUMBER_ELEM + 
              MochaFigure->HASH_BEH_AUX->NUMBER_ELEM + 
              MochaFigure->HASH_BEH_REG->NUMBER_ELEM;


  BddSystem = createbddsystem( NumberOut + NumberIn, 1000, 1000, 900000 );
  SetBddSystemNoWarning( BddSystem );
  reorderbddsystemdynamic( BddSystem, reorderbddsystemsimple, 100000, 50 );

  MochaFigure->BDD_SYSTEM = BddSystem;

  BddCircuit = createbddcircuit( MochaFigure->NAME, NumberIn, NumberOut, BddSystem );

  MochaFigure->BDD_CIRCUIT = BddCircuit;

  for ( BehRin  = BehFigure->BERIN;
        BehRin != (berin_list *)0;
        BehRin  = BehRin->NEXT )
  {
    addbddcircuitin( (bddcircuit *)0, BehRin->NAME,
                     (bddindex    )0, BDD_IN_MODE_LAST );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        MochaBuildBdd                        |
|                                                             |
\------------------------------------------------------------*/

void MochaBuildBdd( MochaFigure, FlagVerbose )

  mochafig_list *MochaFigure;
  int            FlagVerbose;
{
  MochaBddBuildCircuit( MochaFigure );
  MochaBddBuildBddNode( MochaFigure );

# ifdef DEBUG
  testbddcircuit( (bddcircuit *)0 );
# endif
}
