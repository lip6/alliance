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
| File    :                   mocha_check.c                   |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   05.13.02                        |
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
# include "btr.h"
# include "fsm.h"
# include "vex.h"
# include "ctl.h"
# include "ctp.h"
# include "abe.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "mocha_debug.h"
# include "mocha_shared.h"
# include "mocha_ctl.h"
# include "mocha_check.h"

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

 static mochafig_list *MochaMochaFigure = (mochafig_list *)0;

 static bddnode *MochaCheckCtlAblBoolean();
 static bddnode *MochaCheckCtlAblAF();
 static bddnode *MochaCheckCtlAblAG();
 static bddnode *MochaCheckCtlAblAX();
 static bddnode *MochaCheckCtlAblAU();
 static bddnode *MochaCheckCtlAblEF();
 static bddnode *MochaCheckCtlAblEG();
 static bddnode *MochaCheckCtlAblEX();
 static bddnode *MochaCheckCtlAblEU();
 static bddnode *MochaCheckCtlAbl();

 static bddnode *(*MochaCheckCtlFunc[ ABL_MAX_OPERATOR ])() =
 {
   MochaCheckCtlAblBoolean,   /* ABL_OR      */
   MochaCheckCtlAblBoolean,   /* ABL_AND     */
   MochaCheckCtlAblBoolean,   /* ABL_XOR     */
   NULL,                      /* ABL_NOT     */
   MochaCheckCtlAblBoolean,   /* ABL_NOR     */
   MochaCheckCtlAblBoolean,   /* ABL_NAND    */
   MochaCheckCtlAblBoolean,   /* ABL_NXOR    */
   NULL,                      /* ABL_STABLE  */
   MochaCheckCtlAblAF,        /* ABL_AF      */
   MochaCheckCtlAblAG,        /* ABL_AG      */
   MochaCheckCtlAblAX,        /* ABL_AX      */
   MochaCheckCtlAblAU,        /* ABL_AU      */
   MochaCheckCtlAblEF,        /* ABL_EF      */
   MochaCheckCtlAblEG,        /* ABL_EG      */
   MochaCheckCtlAblEX,        /* ABL_EX      */
   MochaCheckCtlAblEU         /* ABL_EU      */
 };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

static void MochaCheckViewBddNode( BddNode )
   
   bddnode *BddNode;
{
  ablexpr *Expr;
     
  Expr = convertbddcircuitabl( (bddcircuit *)0, BddNode );
  viewablexpr( Expr, ABL_VIEW_VHDL );
  freeablexpr( Expr );
           
  fprintf( stdout, "\n" );
}
 
void MochaCheckViewTransFunc( VarFunc )
   
    btrvarfunc *VarFunc;
{
  fprintf( stdout, "\nVAR : " );
  MochaCheckViewBddNode( VarFunc->VAR );
  fprintf( stdout, "FUNC: " );
  MochaCheckViewBddNode( VarFunc->FUNC );
}

/*------------------------------------------------------------\
|                                                             |
|                     MochaCheckBuildTransFunc                |
|                                                             |
\------------------------------------------------------------*/

static void MochaCheckBuildTransFunc( MochaFigure )

  mochafig_list *MochaFigure;
{
  mochafsm_list *MochaFsm;
  befig_list    *BehFigure;
  ctlfig_list   *CtlFigure;
  btrtransfunc  *BtrTransFunc;
  bddassoc      *BddStateAssoc;
  bddassoc      *BddRegAssoc;
  bereg_list    *BehReg;
  binode_list   *BiNode;
  bddnode       *BddNode;
  bddvar         Variable;
  char           Buffer[ 512 ];
  long           NumberReg;
  long           Index;
  long           Index2;
  long           Step;
  long           Width;

  BehFigure = MochaFigure->BEH_FIGURE;
  CtlFigure = MochaFigure->CTL_FIGURE;

  setbddlocalcircuit( MochaFigure->BDD_CIRCUIT );

  NumberReg    = MochaFigure->HASH_BEH_REG->NUMBER_ELEM;
  BtrTransFunc = createbtrtransfunc( (bddsystem *)0, NumberReg );
  BddRegAssoc  = addbddassoc( (bddsystem *)0 );

  MochaFigure->BTR_TRANS_FUNC = BtrTransFunc;
  MochaFigure->BDD_ASSOC_REG  = BddRegAssoc;
 
  for ( BehReg  = BehFigure->BEREG;
        BehReg != (bereg_list *)0;
        BehReg  = BehReg->NEXT )
  {
    BiNode = BehReg->BINODE;
 
    BddNode = searchbddcircuitin( (bddcircuit *)0, BehReg->NAME );
    addbtrtransfunc( BtrTransFunc, BddNode, BiNode->VALNODE );
 
    Variable = BddLocalSystem->INDEX_TO_VAR[ BddNode->INDEX ];
    addbddnodeassoc( (bddsystem *)0, BddRegAssoc, Variable, BddLocalSystem->ONE );
  }

  for ( MochaFsm  = MochaFigure->FSM;
        MochaFsm != (mochafsm_list *)0;
        MochaFsm  = MochaFsm->NEXT )
  {
    BddStateAssoc = addbddassoc( (bddsystem *)0 );
    MochaFsm->BDD_ASSOC_STATE = BddStateAssoc;

    if ( MochaFsm->LEFT > MochaFsm->RIGHT ) Step = -1;
    else                                    Step =  1;

    Index  = MochaFsm->LEFT;
    Index2 = 0;

    for ( Width = MochaFsm->NUMBER_BIT; Width > 0; Width-- )
    {
      sprintf( Buffer, "%s %ld", MochaFsm->CURRENT_STATE, Index );

      BddNode  = searchbddcircuitin( (bddcircuit *)0, Buffer );
      Variable = BddLocalSystem->INDEX_TO_VAR[ BddNode->INDEX ];
      addbddnodeassoc( (bddsystem *)0, BddStateAssoc, Variable, BddLocalSystem->ONE );

      Index += Step;
      Index2 = Index2 + 1;
    }
  }

# ifdef DEBUG
  viewbtrtransfunc( BtrTransFunc, MochaCheckViewTransFunc );
# endif
}

/*------------------------------------------------------------\
|                                                             |
|                     MochaCheckComputeFirstState             |
|                                                             |
\------------------------------------------------------------*/

static void MochaCheckComputeFirstState( MochaFigure )

  mochafig_list *MochaFigure;
{
  befig_list    *BehFigure;
  ctlfig_list   *CtlFigure;
  ctldecl_list  *CtlDeclar;
  biablarray    *BiAblArray;
  ablarray      *AblArray;
  bddnode       *BddNode;
  bddnode       *BddCurrentSet;
  bddnode       *BddReg;
  bddnode       *BddInit;
  bddnode       *BddAssume;
  ablexpr       *AblInit;
  ablexpr       *AblAssume;
  bereg_list    *BehReg;
  binode_list   *BiNode;
  int            BitZero;

  BehFigure = MochaFigure->BEH_FIGURE;
  CtlFigure = MochaFigure->CTL_FIGURE;

  setbddlocalcircuit( MochaFigure->BDD_CIRCUIT );
/*
** Convert assumptions to a Bdd node
*/
  BddAssume = BddLocalSystem->ONE;

  for ( CtlDeclar  = CtlFigure->DECLAR[ CTL_DECLAR_ASSUME ];
        CtlDeclar != (ctldecl_list *)0;
        CtlDeclar  = CtlDeclar->NEXT )
  {
    BiAblArray = MOCHA_CTL_BIABLA( CtlDeclar );
    AblArray   = BiAblArray->EXPR;
    AblAssume  = AblArray->ARRAY[ 0 ];

    BddNode    = addbddcircuitabl( (bddcircuit *)0, AblAssume  );
    BddAssume  = applybddnode( (bddsystem *)0, ABL_AND, 
                                decbddrefext( BddAssume ), decbddrefext( BddNode ) );
  }

  MochaFigure->BDD_ASSUME = BddAssume;
/*
** Convert the initial conditions to a Bdd node
*/
  BddInit = BddLocalSystem->ONE;

  for ( CtlDeclar  = CtlFigure->DECLAR[ CTL_DECLAR_INITIAL ];
        CtlDeclar != (ctldecl_list *)0;
        CtlDeclar  = CtlDeclar->NEXT )
  {
    BiAblArray = MOCHA_CTL_BIABLA( CtlDeclar );
    AblArray   = BiAblArray->EXPR;
    AblInit    = AblArray->ARRAY[ 0 ];

    BddNode  = addbddcircuitabl( (bddcircuit *)0, AblInit  );
    BddInit  = applybddnode( (bddsystem *)0, ABL_AND, 
                              decbddrefext( BddInit  ), decbddrefext( BddNode ) );
  }

  if ( BddInit == BddLocalSystem->ZERO )
  {
    MochaPrintf( stdout, "The initial condition is equal to zero !\n" );
    autexit( 1 );
  }
/*
** Search the initial state !
*/
  BddCurrentSet = BddLocalSystem->ONE;

  for ( BehReg  = BehFigure->BEREG;
        BehReg != (bereg_list *)0;
        BehReg  = BehReg->NEXT )
  {
    BiNode  = BehReg->BINODE;
    BddReg  = searchbddcircuitin( (bddcircuit *)0, BehReg->NAME );
    BitZero = 1;
/*
** Search the initial value of the state registers
*/
    BddNode = cofactorbddnode( (bddsystem *)0, BiNode->VALNODE, BddInit );

    if ( BddNode == BddLocalSystem->ONE  ) BitZero = 0;
    else
    if ( BddNode != BddLocalSystem->ZERO )
    {
      MochaPrintf( stdout, "Invalid initial conditions: %s not initialized !\n", BehReg->NAME );
      MochaCheckViewBddNode( BddNode );
      autexit( 1 );
    }

    if ( BitZero ) BddNode = applybddnodenot( (bddsystem *)0, BddReg );
    else           BddNode = BddReg;

    BddCurrentSet = applybddnode( (bddsystem *)0, ABL_AND,
                                  decbddrefext( BddCurrentSet ),
                                  decbddrefext( BddNode       ) );
  }

# ifdef DEBUG
  addbddcircuitout( (bddcircuit *)0, "init", BddCurrentSet );
  testbddcircuit( (bddcircuit *)0 );
# endif

  MochaFigure->BDD_FIRST_STATE = BddCurrentSet;
}

/*------------------------------------------------------------\
|                                                             |
|                    MochaCheckComputeReachableStates         |
|                                                             |
\------------------------------------------------------------*/

static void MochaCheckComputeReachableStates( MochaFigure )

  mochafig_list *MochaFigure;
{
  befig_list   *BehFigure;
  ctlfig_list  *CtlFigure;
  btrtransfunc *BtrTransFunc;
  bddassoc     *BddAssoc;
  bddnode      *BddNewSet;
  bddnode      *BddCurrentSet;
  bddnode      *BddReachedSet;
  bddnode      *BddNode;

  BehFigure    = MochaFigure->BEH_FIGURE;
  CtlFigure    = MochaFigure->CTL_FIGURE;
  BtrTransFunc = MochaFigure->BTR_TRANS_FUNC;
  BddAssoc     = MochaFigure->BDD_ASSOC_REG;

  setbddlocalcircuit( MochaFigure->BDD_CIRCUIT );

  BddReachedSet = incbddrefext( MochaFigure->BDD_FIRST_STATE );
  BddCurrentSet = incbddrefext( MochaFigure->BDD_FIRST_STATE );

  do
  {
    BddCurrentSet = applybddnode( (bddsystem *)0, ABL_AND,
                                  decbddrefext( BddCurrentSet ), MochaFigure->BDD_ASSUME );
    BddNewSet     = imagebtrtransfunc( BtrTransFunc, BddCurrentSet );
    decbddrefext( BddCurrentSet );

    BddNode   = applybddnodenot( (bddsystem *)0, BddReachedSet );
    BddNewSet = applybddnode( (bddsystem *)0, ABL_AND,
                              decbddrefext( BddNode ), decbddrefext( BddNewSet ) );
                              
    BddReachedSet = applybddnode( (bddsystem *)0, ABL_OR, 
                                  decbddrefext( BddReachedSet ), BddNewSet );

    BddCurrentSet = BddNewSet;
  }
  while ( BddCurrentSet != BddLocalSystem->ZERO );

  MochaFigure->BDD_REACHED_STATE = BddReachedSet;

# ifdef DEBUG
  addbddcircuitout( (bddcircuit *)0, "reached", BddReachedSet );
  testbddcircuit( (bddcircuit *)0 );
# endif
  fprintf( stdout, "First state " );
  MochaCheckViewBddNode( MochaFigure->BDD_FIRST_STATE );
  fprintf( stdout, "Reachable states " );
  MochaCheckViewBddNode( BddReachedSet );
}

/*------------------------------------------------------------\
|                                                             |
|                    MochaCheckCtlBddEX                       |
|                                                             |
\------------------------------------------------------------*/

static bddnode *MochaCheckCtlBddEX( BddNode )

  bddnode *BddNode;
{
  bddnode      *BddNew;
  bddnode      *BddReached; 
  bddnode      *BddAssume;
  btrtransfunc *BtrTransFunc;
  bddassoc     *BddAssoc;

  BtrTransFunc = MochaMochaFigure->BTR_TRANS_FUNC;
  BddAssoc     = MochaMochaFigure->BDD_ASSOC_REG;
  BddReached   = MochaMochaFigure->BDD_REACHED_STATE;
  BddAssume    = MochaMochaFigure->BDD_ASSUME;

  BddNew = preimagebtrtransfunc( BtrTransFunc, BddNode );

  BddNew = applybddnode( (bddsystem *)0, ABL_AND,
                         decbddrefext( BddNew ), BddAssume );

  BddNew = existbddnodemissassoc( (bddsystem *)0,
                                   decbddrefext( BddNew ), BddAssoc );

  BddNew  = applybddnode( (bddsystem *)0, ABL_AND,
                           decbddrefext( BddNew ), BddReached );

# if 1
  fprintf( stdout, "MochaCheckCtlBddEX  -> " );
  MochaCheckViewBddNode( BddNew );
# endif

  return( BddNew );
}

/*------------------------------------------------------------\
|                                                             |
|                    MochaCheckCtlBddEG                       |
|                                                             |
\------------------------------------------------------------*/

static bddnode *MochaCheckCtlBddEG( BddNode )

  bddnode *BddNode;
{
  bddnode      *BddOld;
  bddnode      *BddNew;
  bddnode      *BddReached; 
  bddnode      *BddAssume;
  btrtransfunc *BtrTransFunc;
  bddassoc     *BddAssoc;

  BtrTransFunc = MochaMochaFigure->BTR_TRANS_FUNC;
  BddAssoc     = MochaMochaFigure->BDD_ASSOC_REG;
  BddReached   = MochaMochaFigure->BDD_REACHED_STATE;
  BddAssume    = MochaMochaFigure->BDD_ASSUME;
  BddOld       = BddLocalSystem->ONE;
  BddNew       = incbddrefext( BddNode );

  fprintf( stdout, "> MochaCheckCtlBddEG\n" );
  fprintf( stdout, "EG:\n" );
  MochaCheckViewBddNode( BddNode );
# ifdef DEBUG
  fprintf( stdout, "REACHED:\n" );
  MochaCheckViewBddNode( BddReached );
# endif

  while ( BddNew != BddOld )
  {
    BddOld = BddNew;

    BddNew = preimagebtrtransfunc( BtrTransFunc, BddOld );

    BddNew = applybddnode( (bddsystem *)0, ABL_AND, 
                           decbddrefext( BddNew ), BddAssume );

    BddNew = applybddnode( (bddsystem *)0, ABL_AND,
                            decbddrefext( BddNew ), BddNode );

    BddNew = existbddnodemissassoc( (bddsystem *)0,
                                    decbddrefext( BddNew ), BddAssoc );

    BddNew = applybddnode( (bddsystem *)0, ABL_AND,
                            decbddrefext( BddNew ), BddReached );

    fprintf( stdout, "MochaCheckCtlBddEG:\n" );
    MochaCheckViewBddNode( BddNew );

    decbddrefext( BddOld );
  }

  fprintf( stdout, "< MochaCheckCtlBddEG\n" );

  return( BddNew );
}

/*------------------------------------------------------------\
|                                                             |
|                    MochaCheckCtlBddEU                       |
|                                                             |
\------------------------------------------------------------*/

static bddnode *MochaCheckCtlBddEU( BddNodeP, BddNodeQ )

  bddnode *BddNodeP;
  bddnode *BddNodeQ;
{
  bddnode      *BddOld;
  bddnode      *BddNew;
  bddnode      *BddReached; 
  bddnode      *BddAssume;
  btrtransfunc *BtrTransFunc;
  bddassoc     *BddAssoc;

  BtrTransFunc = MochaMochaFigure->BTR_TRANS_FUNC;
  BddAssoc     = MochaMochaFigure->BDD_ASSOC_REG;
  BddReached   = MochaMochaFigure->BDD_REACHED_STATE;
  BddAssume    = MochaMochaFigure->BDD_ASSUME;
  BddOld       = BddLocalSystem->ZERO;
  BddNew       = incbddrefext( BddNodeQ );

  while ( BddNew != BddOld )
  {
    BddOld = BddNew;

    BddNew = preimagebtrtransfunc( BtrTransFunc, BddOld );

    BddNew = applybddnode( (bddsystem *)0, ABL_AND,
                            decbddrefext( BddNew ), BddAssume );

    BddNew = applybddnode( (bddsystem *)0, ABL_AND,
                            decbddrefext( BddNew ), BddNodeP );

    BddNew  = applybddnode( (bddsystem *)0, ABL_OR,
                            decbddrefext( BddNew ), BddNodeQ );

    BddNew = existbddnodemissassoc( (bddsystem *)0,
                                     decbddrefext( BddNew ), BddAssoc );

    BddNew = applybddnode( (bddsystem *)0, ABL_AND,
                            decbddrefext( BddNew ), BddReached );

    fprintf( stdout, "MochaCheckCtlBddEU:\n" );
    MochaCheckViewBddNode( BddNew );

    decbddrefext( BddOld );
  }

  return( BddNew );
}

/*------------------------------------------------------------\
|                                                             |
|                    MochaCheckCtlAblBoolean                  |
|                                                             |
\------------------------------------------------------------*/

static bddnode *MochaCheckCtlAblBoolean( Expr )

  ablexpr *Expr;
{
  bddnode    *BddNode;
  bddnode    *BddFirst;
  long        Oper;
  int         Negative;

  Oper = ABL_OPER( Expr );
  Expr = ABL_CDR( Expr );

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
  
  BddFirst = MochaCheckCtlAbl( ABL_CAR( Expr ) );
  
  while ( ( Expr = ABL_CDR( Expr ) ) != (chain_list *)0 )
  {
    BddNode  = MochaCheckCtlAbl( ABL_CAR( Expr ) );
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
|                    MochaCheckCtlAblAF                       |
|                                                             |
\------------------------------------------------------------*/

static bddnode *MochaCheckCtlAblAF( Expr )

  ablexpr *Expr;
{
  bddnode *BddNode;
  bddnode *BddResult;
  bddnode *BddReached; 

  /* AF p <=> ! EG( ! p ) */

  Expr = ABL_CDR( Expr );

  BddNode   = MochaCheckCtlAbl( ABL_CAR( Expr ) );
  BddNode   = applybddnodenot( (bddsystem *)0, decbddrefext( BddNode ) );
  BddResult = MochaCheckCtlBddEG( BddNode );
  decbddrefext( BddNode );
  BddResult = applybddnodenot( (bddsystem *)0, decbddrefext( BddResult ) );

  BddReached = MochaMochaFigure->BDD_REACHED_STATE;
  BddResult  = applybddnode( (bddsystem *)0, ABL_AND,
                              decbddrefext( BddResult ), BddReached ); 

  return( BddResult );
}

/*------------------------------------------------------------\
|                                                             |
|                    MochaCheckCtlAblAG                       |
|                                                             |
\------------------------------------------------------------*/

static bddnode *MochaCheckCtlAblAG( Expr )

  ablexpr *Expr;
{
  bddnode *BddNode;
  bddnode *BddResult;
  bddnode *BddReached; 

  /* AG p <=> ! EF( ! p ) <=> ! EU( 1, ! p ) */

  Expr = ABL_CDR( Expr );

  BddNode   = MochaCheckCtlAbl( ABL_CAR( Expr ) );
  BddNode   = applybddnodenot( (bddsystem *)0, decbddrefext( BddNode ) );
  BddResult = MochaCheckCtlBddEU( BddLocalSystem->ONE, BddNode );
  decbddrefext( BddNode );
  BddResult = applybddnodenot( (bddsystem *)0, decbddrefext( BddResult ) );

  BddReached = MochaMochaFigure->BDD_REACHED_STATE;
  BddResult  = applybddnode( (bddsystem *)0, ABL_AND,
                              decbddrefext( BddResult ), BddReached ); 

  return( BddResult );
}

/*------------------------------------------------------------\
|                                                             |
|                    MochaCheckCtlAblAX                       |
|                                                             |
\------------------------------------------------------------*/

static bddnode *MochaCheckCtlAblAX( Expr )

  ablexpr *Expr;
{
  bddnode *BddNode;
  bddnode *BddResult;
  bddnode *BddReached; 

  /* AX p <=> ! EX( ! p ) */

  Expr = ABL_CDR( Expr );

  BddNode   = MochaCheckCtlAbl( ABL_CAR( Expr ) );
  BddNode   = applybddnodenot( (bddsystem *)0, decbddrefext( BddNode ) );
  BddResult = MochaCheckCtlBddEX( BddNode );
  decbddrefext( BddNode );
  BddResult = applybddnodenot( (bddsystem *)0, decbddrefext( BddResult ) );

  BddReached = MochaMochaFigure->BDD_REACHED_STATE;
  BddResult  = applybddnode( (bddsystem *)0, ABL_AND,
                              decbddrefext( BddResult ), BddReached ); 

  return( BddResult );
}

/*------------------------------------------------------------\
|                                                             |
|                    MochaCheckCtlAblAU                       |
|                                                             |
\------------------------------------------------------------*/

static bddnode *MochaCheckCtlAblAU( Expr )

  ablexpr *Expr;
{
  bddnode *BddNode1;
  bddnode *BddNode2;
  bddnode *BddNode3;
  bddnode *BddResult1;
  bddnode *BddResult2;
  bddnode *BddResult;
  bddnode *BddReached; 

  /* AU( p, q ) <=> ! EU( ! q, ! p * ! q ) * ! EG( ! q ) */

  Expr      = ABL_CDR( Expr );
  BddNode1  = MochaCheckCtlAbl( ABL_CAR( Expr ) );
  Expr      = ABL_CDR( Expr );
  BddNode2  = MochaCheckCtlAbl( ABL_CAR( Expr ) );

  BddNode1  = applybddnodenot( (bddsystem *)0, decbddrefext( BddNode1 ) );
  BddNode2  = applybddnodenot( (bddsystem *)0, decbddrefext( BddNode2 ) );

  /* ! EG( ! q ) */
  BddResult1 = MochaCheckCtlBddEG( BddNode2 );
  BddResult1 = applybddnodenot( (bddsystem *)0, decbddrefext( BddResult1 ) );

  /* ! EU( ! q, ! p * ! q ) */
  BddNode3  = applybddnode( (bddsystem *)0, ABL_AND, BddNode1, BddNode2 );
  BddResult2 = MochaCheckCtlBddEU( BddNode2, BddNode3 );
  BddResult2 = applybddnodenot( (bddsystem *)0, decbddrefext( BddResult2 ) );

  decbddrefext( BddNode1 );
  decbddrefext( BddNode2 );
  decbddrefext( BddNode3 );

  BddResult = applybddnode( (bddsystem *)0, ABL_AND,
                            decbddrefext( BddResult1 ),
                            decbddrefext( BddResult2 ) );

  BddReached = MochaMochaFigure->BDD_REACHED_STATE;
  BddResult  = applybddnode( (bddsystem *)0, ABL_AND,
                              decbddrefext( BddResult ), BddReached ); 

  return( BddResult );
}

/*------------------------------------------------------------\
|                                                             |
|                    MochaCheckCtlAblEF                       |
|                                                             |
\------------------------------------------------------------*/

static bddnode *MochaCheckCtlAblEF( Expr )

  ablexpr *Expr;
{
  bddnode *BddNode;
  bddnode *BddResult;

  /* EF( p ) <=> EU( 1, p ) */

  Expr = ABL_CDR( Expr );

  BddNode   = MochaCheckCtlAbl( ABL_CAR( Expr ) );
  BddResult = MochaCheckCtlBddEU( BddLocalSystem->ONE, BddNode );
  decbddrefext( BddNode );

  return( BddResult );
}

/*------------------------------------------------------------\
|                                                             |
|                    MochaCheckCtlAblEX                       |
|                                                             |
\------------------------------------------------------------*/

static bddnode *MochaCheckCtlAblEX( Expr )

  ablexpr *Expr;
{
  bddnode *BddNode;
  bddnode *BddResult;

  Expr = ABL_CDR( Expr );

  BddNode   = MochaCheckCtlAbl( ABL_CAR( Expr ) );
  BddResult = MochaCheckCtlBddEX( BddNode );
  decbddrefext( BddNode );

  return( BddResult );
}

/*------------------------------------------------------------\
|                                                             |
|                    MochaCheckCtlAblEG                       |
|                                                             |
\------------------------------------------------------------*/

static bddnode *MochaCheckCtlAblEG( Expr )

  ablexpr *Expr;
{
  bddnode *BddNode;
  bddnode *BddResult;

# ifdef DEBUG
  fprintf( stdout, "MochaCheckCtlAblEG: " );
  viewablexprln( Expr, ABL_VIEW_VHDL );
# endif

  Expr = ABL_CDR( Expr );

  BddNode   = MochaCheckCtlAbl( ABL_CAR( Expr ) );
  BddResult = MochaCheckCtlBddEG( BddNode );
  decbddrefext( BddNode );

  return( BddResult );
}

/*------------------------------------------------------------\
|                                                             |
|                    MochaCheckCtlAblEU                       |
|                                                             |
\------------------------------------------------------------*/

static bddnode *MochaCheckCtlAblEU( Expr )

  ablexpr *Expr;
{
  bddnode *BddNode1;
  bddnode *BddNode2;
  bddnode *BddResult;

  Expr      = ABL_CDR( Expr );
  BddNode1  = MochaCheckCtlAbl( ABL_CAR( Expr ) );
  Expr      = ABL_CDR( Expr );
  BddNode2  = MochaCheckCtlAbl( ABL_CAR( Expr ) );

  BddResult = MochaCheckCtlBddEU( BddNode1, BddNode2 );

  decbddrefext( BddNode1 );
  decbddrefext( BddNode2 );

  return( BddResult );
}

/*------------------------------------------------------------\
|                                                             |
|                    MochaCheckCtlAbl                         |
|                                                             |
\------------------------------------------------------------*/

static bddnode *MochaCheckCtlAbl( Expr )

  ablexpr *Expr;
{
  bddnode    *BddNode;
  bddnode    *BddFirst;
  char       *AtomValue;
  long        Oper;

  BddFirst = (bddnode *)0;
  BddNode  = (bddnode *)0;

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

    if ( BddNode == (bddnode *)0 )
    {
      BddNode = searchbddcircuitout( (bddcircuit *)0, AtomValue );

      if ( BddNode == (bddnode *)0 )
      { 
        MochaPrintf( stdout, "Unknown atom %s\n", AtomValue );
        autexit( 1 );
      }

      incbddrefext( BddNode );
    }

    return( BddNode );
  }

  Oper = ABL_OPER( Expr );

  if ( Oper == ABL_NOT )
  {
    Expr     = ABL_CDR( Expr );
    BddFirst = MochaCheckCtlAbl( ABL_CAR( Expr ) );
    BddNode  = applybddnodenot( (bddsystem *)0, decbddrefext( BddFirst ) );
  }
  else
  if ( Oper == ABL_STABLE )
  {
    MochaPrintf( stdout, "Operator STABLE not supported in CTL formulae !\n" );
    autexit( 1 );
  }
  else
  if ( MochaCheckCtlFunc[ Oper ] != NULL )
  {
    BddNode = (*MochaCheckCtlFunc[ Oper ])( Expr );
  }
  else
  {
    MochaPrintf( stdout, "Operator not supported in CTL formulae: " );
    viewablexprln( Expr, ABL_VIEW_VHDL );
    autexit( 1 );
  }

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                    MochaCheckCtlFormulae                    |
|                                                             |
\------------------------------------------------------------*/

static void MochaCheckCtlFormulae( MochaFigure, FlagVerbose )

  mochafig_list *MochaFigure;
  int            FlagVerbose;
{
  ctlfig_list  *CtlFigure;
  ctlform_list *CtlForm;
  btrtransfunc *BtrTransFunc;
  biablarray   *BiAblArray;
  ablarray     *AblArray;
  bddnode      *BddNode;
  bddnode      *BddFirst;
  bddnode      *BddReached;
  bddnode      *BddAssume;
  bddassoc     *BddAssoc;

  CtlFigure    = MochaFigure->CTL_FIGURE;
  BtrTransFunc = MochaFigure->BTR_TRANS_FUNC;
  BddAssoc     = MochaFigure->BDD_ASSOC_REG;
  BddAssume    = MochaFigure->BDD_ASSUME;
  BddReached   = MochaFigure->BDD_REACHED_STATE;
  BddFirst     = MochaFigure->BDD_FIRST_STATE;

  setbddlocalcircuit( MochaFigure->BDD_CIRCUIT );

  MochaMochaFigure = MochaFigure;

# ifdef DEBUG
  fprintf( stdout, "Reachable states:\n" );
  MochaCheckViewBddNode( BddReached );

  fprintf( stdout, "First state:\n" );
  MochaCheckViewBddNode( BddFirst );
# endif

  for ( CtlForm  = CtlFigure->FORM;
        CtlForm != (ctlform_list *)0;
        CtlForm  = CtlForm->NEXT )
  {
    if ( FlagVerbose )
    {
      fprintf( stdout, "\t    Verifying formula %s\n", CtlForm->NAME );
    }

    BiAblArray = MOCHA_CTL_BIABLA( CtlForm );
    AblArray   = BiAblArray->EXPR;

    BddNode = MochaCheckCtlAbl( AblArray->ARRAY[ 0 ] );
    BddNode = applybddnode( (bddsystem *)0, ABL_AND,
                            decbddrefext( BddNode ), BddAssume );

    BddNode = existbddnodemissassoc( (bddsystem *)0,
                                     decbddrefext( BddNode ), BddAssoc );

    BddNode = applybddnode( (bddsystem *)0, ABL_AND,
                            decbddrefext( BddNode ), BddReached ); 

    BddNode = applybddnode( (bddsystem *)0, ABL_AND, 
                            decbddrefext( BddNode ), BddFirst );

    decbddrefext( BddNode );

# ifdef DEBUG
    MochaCheckViewBddNode( BddNode );
# endif

    if ( BddNode != BddFirst  )
    {
      fprintf( stdout, "\t    Formula %s IS FALSE !\n", CtlForm->NAME );
      MochaCheckViewBddNode( BddNode );
    }
    else
    if ( FlagVerbose )
    {
      fprintf( stdout, "\t    OK\n" );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        MochaCheckModel                      |
|                                                             |
\------------------------------------------------------------*/

int MochaCheckModel( MochaFigure, FlagVerbose )

  mochafig_list *MochaFigure;
  int            FlagVerbose;
{
  MochaCheckBuildTransFunc( MochaFigure );
  MochaCheckComputeFirstState( MochaFigure );
  MochaCheckComputeReachableStates( MochaFigure );
  MochaCheckCtlFormulae( MochaFigure, FlagVerbose );

  return( 0 );
}
