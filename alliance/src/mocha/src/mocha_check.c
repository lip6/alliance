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
/*------------------------------------------------------------\
|                                                             |
|                      MochaCheckViewBddNode                  |
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
|                      MochaCheckViewBddStateNode             |
|                                                             |
\------------------------------------------------------------*/

static void MochaCheckViewBddStateNode( BddNode )
   
   bddnode *BddNode;
{
  ablexpr *Expr;
     
  Expr = convertbddcircuitabl( (bddcircuit *)0, BddNode );
  viewablexpr( Expr, ABL_VIEW_VHDL );
  freeablexpr( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                      MochaCheckViewState                    |
|                                                             |
\------------------------------------------------------------*/

static int MochaCheckViewState( MochaFigure, BddStateSet )

  mochafig_list *MochaFigure;
  bddnode       *BddStateSet;
{
  mochafsm_list   *MochaFsm;
  mochastate_list *MochaState;
  bddnode         *BddState;
  bddnode         *BddReached;
  bddnode         *BddFirst;
  bddnode         *BddFsm;
  bddnode         *BddCheck;
  bddnode         *BddOther;
  bddassoc        *BddAssoc;
  bddassoc        *BddRegAssoc;
  int              First;

  setbddlocalcircuit( MochaFigure->BDD_CIRCUIT );

  BddRegAssoc = MochaFigure->BDD_ASSOC_REG;
  BddReached  = MochaFigure->BDD_REACHED_STATE;
  BddFirst    = MochaFigure->BDD_FIRST_STATE;

  if ( BddStateSet == BddLocalSystem->ZERO )
  {
    fprintf( stdout, "\t    > state set empty !\n" );
    return ( 0 );
  }

  if ( BddStateSet == BddReached ) 
  {
    fprintf( stdout, "\t    > all reachable states\n" );
    return ( 1 );
  }

  BddCheck = applybddnode( (bddsystem *)0, ABL_AND, BddStateSet, BddFirst );
  decbddrefext( BddCheck );

  if ( BddCheck == BddFirst ) First = 1;
  else                        First = 0;

  fprintf( stdout, "\t    > states set:\n" );

  while ( BddStateSet != BddLocalSystem->ZERO )
  {
    BddCheck = satisfybddnodeassoc( (bddsystem *)0, BddStateSet, BddRegAssoc ); 
    BddOther = incbddrefext( BddCheck );

    fprintf( stdout, "\t       " );

    for ( MochaFsm  = MochaFigure->FSM;
          MochaFsm != (mochafsm_list *)0;
          MochaFsm  = MochaFsm->NEXT )
    {
      BddAssoc = MochaFsm->BDD_ASSOC_STATE;
      BddFsm   = existbddnodemissassoc( (bddsystem *)0, BddCheck, BddAssoc );

      BddOther = existbddnodeassoc( (bddsystem *)0, 
                                    decbddrefext( BddOther ), BddAssoc );

      for ( MochaState  = MochaFsm->STATE;
            MochaState != (mochastate_list *)0;
            MochaState  = MochaState->NEXT )
      {
        BddState = applybddnode( (bddsystem *)0, ABL_AND,
                                 BddFsm, MochaState->BDD_STATE );
        decbddrefext( BddState );

        if ( BddState == MochaState->BDD_STATE )
        {
          fprintf( stdout, "%s[%s] ", MochaFsm->NAME, MochaState->NAME );
          break;
        }
      }
    }

    if ( ( BddOther != BddLocalSystem->ONE  ) &&
         ( BddOther != BddLocalSystem->ZERO ) )
    {
      MochaCheckViewBddStateNode( BddOther );
      decbddrefext( BddOther );
    }

    if ( BddCheck == BddFirst )
    {
      fprintf( stdout, " (first state)\n" );
    }
    else
    {
      fprintf( stdout, "\n" );
    }

    BddCheck    = applybddnodenot( (bddsystem *)0, decbddrefext( BddCheck ) );
    BddStateSet = applybddnode( (bddsystem *)0, ABL_AND,
                                decbddrefext( BddStateSet ), decbddrefext( BddCheck ) );
  }
  
  return( First );
}

/*------------------------------------------------------------\
|                                                             |
|                     MochaCheckBuildTransFunc                |
|                                                             |
\------------------------------------------------------------*/

static void MochaCheckBuildTransFunc( MochaFigure )

  mochafig_list *MochaFigure;
{
  mochafsm_list   *MochaFsm;
  mochastate_list *MochaState;
  befig_list      *BehFigure;
  btrtransfunc    *BtrTransFunc;
  bddassoc        *BddStateAssoc;
  bddassoc        *BddRegAssoc;
  bereg_list      *BehReg;
  binode_list     *BiNode;
  chain_list      *ScanList;
  chain_list      *HeadList;
  bddnode         *BddNode;
  bddnode         *BddState;
  bddvar           Variable;
  char             Buffer[ 512 ];
  long             NumberReg;
  long             Index;
  long             Step;
  long             BitMask;
  long             Width;

  BehFigure = MochaFigure->BEH_FIGURE;

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

    HeadList = (chain_list *)0;

    for ( Width = MochaFsm->NUMBER_BIT; Width > 0; Width-- )
    {
      sprintf( Buffer, "%s %ld", MochaFsm->CURRENT_STATE, Index );

      BddNode  = searchbddcircuitin( (bddcircuit *)0, Buffer );
      Variable = BddLocalSystem->INDEX_TO_VAR[ BddNode->INDEX ];
      addbddnodeassoc( (bddsystem *)0, BddStateAssoc, Variable, BddLocalSystem->ONE );

      Index += Step;

      HeadList = addchain( HeadList, (void *)BddNode );
    }

    for ( MochaState  = MochaFsm->STATE;
          MochaState != (mochastate_list *)0;
          MochaState  = MochaState->NEXT )
    {
      BddState = BddLocalSystem->ONE;
      BitMask  = 1;
      ScanList = HeadList;

      for ( Width = MochaFsm->NUMBER_BIT; Width > 0; Width-- )
      {
        BddNode = (bddnode *)( ScanList->DATA );

        if ( ! ( MochaState->CODE & BitMask ) )
        {
          BddNode = applybddnodenot( (bddsystem *)0, BddNode );
          decbddrefext( BddNode );
        }

        BddState = applybddnode( (bddsystem *)0, ABL_AND,
                                 decbddrefext( BddState ), BddNode );

        BitMask  = BitMask << 1;
        ScanList = ScanList->NEXT;
      }

      MochaState->BDD_STATE = BddState;
    }

    freechain( HeadList );
    HeadList = (chain_list *)0;
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
  bddassoc      *BddRegAssoc;
  bddnode       *BddNode;
  bddnode       *BddCurrentSet;
  bddnode       *BddReg;
  bddnode       *BddInit;
  bddnode       *BddReset;
  bddnode       *BddAssume;
  ablexpr       *AblInit;
  ablexpr       *AblReset;
  ablexpr       *AblAssume;
  bereg_list    *BehReg;
  binode_list   *BiNode;
  int            BitZero;

  BehFigure   = MochaFigure->BEH_FIGURE;
  CtlFigure   = MochaFigure->CTL_FIGURE;
  BddRegAssoc = MochaFigure->BDD_ASSOC_REG;

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
** Should verify :  TO BE DONE
**
** a) BddInit Describe only one state (by using Bdd satisfy method)
** b) BddInit support contains only registers atom (BehReg)
*/

/*
** Convert the reset conditions to a Bdd node
*/
  BddReset = BddLocalSystem->ONE;

  for ( CtlDeclar  = CtlFigure->DECLAR[ CTL_DECLAR_RESET ];
        CtlDeclar != (ctldecl_list *)0;
        CtlDeclar  = CtlDeclar->NEXT )
  {
    BiAblArray = MOCHA_CTL_BIABLA( CtlDeclar );
    AblArray   = BiAblArray->EXPR;
    AblReset   = AblArray->ARRAY[ 0 ];

    BddNode  = addbddcircuitabl( (bddcircuit *)0, AblReset  );
    BddReset = applybddnode( (bddsystem *)0, ABL_AND, 
                              decbddrefext( BddInit  ), decbddrefext( BddNode ) );
  }

  if ( BddReset == BddLocalSystem->ZERO )
  {
    MochaPrintf( stdout, "The reset condition is equal to zero !\n" );
    autexit( 1 );
  }
/*
** Should verify :  TO BE DONE
**
** a) BddReset support contains only primary input atom
*/

/*
** Search the initial state !
*/
  BddCurrentSet = BddInit;

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
    BddNode = cofactorbddnode( (bddsystem *)0, BiNode->VALNODE, BddReset );

    if ( BddNode == BddLocalSystem->ONE  ) BitZero = 0;
    else
    if ( BddNode != BddLocalSystem->ZERO )
    {
      decbddrefext( BddNode ); continue;
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

  BddNode = satisfybddnodeassoc( (bddsystem *)0, BddCurrentSet, BddRegAssoc ); 
  decbddrefext( BddNode );

  if ( BddNode != BddCurrentSet )
  {
    MochaPrintf( stdout, "Bad initial/reset condition, all registers are not initialized !\n" );
    autexit( 1 );
  }

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
  ctlfig_list  *CtlFigure;
  btrtransfunc *BtrTransFunc;
  bddnode      *BddNewSet;
  bddnode      *BddCurrentSet;
  bddnode      *BddReachedSet;
  bddnode      *BddNode;

  CtlFigure    = MochaFigure->CTL_FIGURE;
  BtrTransFunc = MochaFigure->BTR_TRANS_FUNC;

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

  if ( MochaFigure->FLAG_DEBUG )
  {
    fprintf( stdout, "First state " );
    MochaCheckViewBddNode( MochaFigure->BDD_FIRST_STATE );
    fprintf( stdout, "Reachable states " );
    MochaCheckViewBddNode( BddReachedSet );
  }
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
|                    MochaCheckCtlBddQuantify                 |
|                                                             |
\------------------------------------------------------------*/

static bddnode *MochaCheckCtlBddQuantify( BddNode )

  bddnode *BddNode;
{
  bddnode      *BddReached; 
  bddnode      *BddAssume;
  bddnode      *BddAssumeNot;
  bddassoc     *BddAssoc;
  bddnode      *BddNew;

  BddAssoc     = MochaMochaFigure->BDD_ASSOC_REG;
  BddReached   = MochaMochaFigure->BDD_REACHED_STATE;
  BddAssume    = MochaMochaFigure->BDD_ASSUME;

  if ( MochaMochaFigure->FLAG_DEBUG )
  {
    fprintf( stdout, "BDD FORALL[\n" );
    MochaCheckViewBddNode( BddNode );
    fprintf( stdout, "]\n" );
  }

  BddNew = applybddnode( (bddsystem *)0, ABL_AND, BddNode, BddAssume );

  BddAssumeNot = applybddnodenot( (bddsystem *)0, BddAssume );

  BddNew = applybddnode( (bddsystem *)0, ABL_OR,
                         decbddrefext( BddNew ), decbddrefext( BddAssumeNot ) );

  BddNew = forallbddnodemissassoc( (bddsystem *)0,
                                   decbddrefext( BddNew ), BddAssoc );

  BddNew  = applybddnode( (bddsystem *)0, ABL_AND,
                           decbddrefext( BddNew ), BddReached );

  if ( MochaMochaFigure->FLAG_DEBUG )
  {
    fprintf( stdout, "-> " );
    MochaCheckViewBddNode( BddNew );
    fprintf( stdout, "\n" );
  }

  return( BddNew );
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
  btrtransfunc *BtrTransFunc;

  BtrTransFunc = MochaMochaFigure->BTR_TRANS_FUNC;
  BddNew       = preimagebtrtransfunc( BtrTransFunc, BddNode );

  if ( MochaMochaFigure->FLAG_DEBUG )
  {
    fprintf( stdout, "BDD EX[\n" );
    MochaCheckViewBddNode( BddNode );
    fprintf( stdout, "]\n" );
  }

  BddNew = MochaCheckCtlBddQuantify( decbddrefext( BddNew ) );

  if ( MochaMochaFigure->FLAG_DEBUG )
  {
    fprintf( stdout, "-> " );
    MochaCheckViewBddNode( BddNew );
    fprintf( stdout, "\n" );
  }

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
  btrtransfunc *BtrTransFunc;

  BtrTransFunc = MochaMochaFigure->BTR_TRANS_FUNC;
  BddOld       = BddLocalSystem->ONE;
  BddNew       = incbddrefext( BddNode );

  if (  MochaMochaFigure->FLAG_DEBUG )
  {
    fprintf( stdout, "BDD EG[\n" );

    MochaCheckViewBddNode( BddNode );
    fprintf( stdout, "]\n" );
  }

  while ( BddNew != BddOld )
  {
    BddOld = BddNew;

    BddNew = preimagebtrtransfunc( BtrTransFunc, BddOld );
    BddNew = applybddnode( (bddsystem *)0, ABL_AND,
                            decbddrefext( BddNew ), BddNode );

    BddNew = MochaCheckCtlBddQuantify( decbddrefext( BddNew ) );

# ifdef DEBUG
    fprintf( stdout, "MochaCheckCtlBddEG:\n" );
    MochaCheckViewBddNode( BddNew );
# endif

    decbddrefext( BddOld );
  }

  if ( MochaMochaFigure->FLAG_DEBUG )
  {
    fprintf( stdout, "-> " );
    MochaCheckViewBddNode( BddNew );
    fprintf( stdout, "\n" );
  }

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
  btrtransfunc *BtrTransFunc;

  BtrTransFunc = MochaMochaFigure->BTR_TRANS_FUNC;
  BddOld       = BddLocalSystem->ZERO;
  BddNew       = incbddrefext( BddNodeQ );

  if ( MochaMochaFigure->FLAG_DEBUG )
  {
    fprintf( stdout, "BDD EU[\n" );
    MochaCheckViewBddNode( BddNodeP );
    fprintf( stdout, ", \n" );
    MochaCheckViewBddNode( BddNodeQ );
    fprintf( stdout, "]\n" );
  }

  while ( BddNew != BddOld )
  {
    BddOld = BddNew;

    BddNew = preimagebtrtransfunc( BtrTransFunc, BddOld );

    BddNew = applybddnode( (bddsystem *)0, ABL_AND,
                            decbddrefext( BddNew ), BddNodeP );

    BddNew = applybddnode( (bddsystem *)0, ABL_OR,
                           decbddrefext( BddNew ), BddNodeQ );

    BddNew = MochaCheckCtlBddQuantify( decbddrefext( BddNew ) );

# ifdef DEBUG
    fprintf( stdout, "MochaCheckCtlBddEU:\n" );
    MochaCheckViewBddNode( BddNew );
# endif

    decbddrefext( BddOld );
  }

  if ( MochaMochaFigure->FLAG_DEBUG )
  {
    fprintf( stdout, "-> " );
    MochaCheckViewBddNode( BddNew );
    fprintf( stdout, "\n" );
  }

  return( BddNew );
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

  /* AF p <=> ! EG( ! p ) */

  Expr = ABL_CDR( Expr );

  BddNode = MochaCheckCtlAbl( ABL_CAR( Expr ) );

  if ( MochaMochaFigure->FLAG_DEBUG )
  {
    fprintf( stdout, "ABL AF[\n" );
    MochaCheckViewBddNode( BddNode );
    fprintf( stdout, "]\n" );
  }

  BddNode   = MochaCheckCtlBddQuantify( decbddrefext( BddNode ) );
  BddNode   = applybddnodenot( (bddsystem *)0, decbddrefext( BddNode ) );
  BddNode   = MochaCheckCtlBddQuantify( decbddrefext( BddNode ) );
  BddResult = MochaCheckCtlBddEG( BddNode );
  decbddrefext( BddNode );
  BddResult = applybddnodenot( (bddsystem *)0, decbddrefext( BddResult ) );
  BddResult = MochaCheckCtlBddQuantify( decbddrefext( BddResult ) );

  if ( MochaMochaFigure->FLAG_DEBUG )
  {
    fprintf( stdout, "-> " );
    MochaCheckViewBddNode( BddResult );
    fprintf( stdout, "\n" );
  }

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

  /* AG p <=> ! EF( ! p ) <=> ! EU( 1, ! p ) */

  Expr = ABL_CDR( Expr );

  BddNode = MochaCheckCtlAbl( ABL_CAR( Expr ) );

  if ( MochaMochaFigure->FLAG_DEBUG )
  {
    fprintf( stdout, "ABL AG[\n" );
    MochaCheckViewBddNode( BddNode );
    fprintf( stdout, "]\n" );
  }

  BddNode   = MochaCheckCtlBddQuantify( decbddrefext( BddNode ) );
  BddNode   = applybddnodenot( (bddsystem *)0, decbddrefext( BddNode ) );
  BddNode   = MochaCheckCtlBddQuantify( decbddrefext( BddNode ) );
  BddResult = MochaCheckCtlBddEU( BddLocalSystem->ONE, BddNode );
  decbddrefext( BddNode );
  BddResult = applybddnodenot( (bddsystem *)0, decbddrefext( BddResult ) );
  BddResult = MochaCheckCtlBddQuantify( decbddrefext( BddResult ) );

  if ( MochaMochaFigure->FLAG_DEBUG )
  {
    fprintf( stdout, "-> " );
    MochaCheckViewBddNode( BddResult );
    fprintf( stdout, "\n" );
  }

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

  /* AX p <=> ! EX( ! p ) */

  Expr = ABL_CDR( Expr );

  BddNode = MochaCheckCtlAbl( ABL_CAR( Expr ) );

  if ( MochaMochaFigure->FLAG_DEBUG )
  {
    fprintf( stdout, "ABL AX[\n" );
    MochaCheckViewBddNode( BddNode );
    fprintf( stdout, "]\n" );
  }

  BddNode   = MochaCheckCtlBddQuantify( decbddrefext( BddNode ) );
  BddNode   = applybddnodenot( (bddsystem *)0, decbddrefext( BddNode ) );
  BddNode   = MochaCheckCtlBddQuantify( decbddrefext( BddNode ) );
  BddResult = MochaCheckCtlBddEX( BddNode );
  decbddrefext( BddNode );
  BddResult = applybddnodenot( (bddsystem *)0, decbddrefext( BddResult ) );
  BddResult = MochaCheckCtlBddQuantify( decbddrefext( BddResult ) );

  if ( MochaMochaFigure->FLAG_DEBUG )
  {
    fprintf( stdout, "-> " );
    MochaCheckViewBddNode( BddResult );
    fprintf( stdout, "\n" );
  }

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

  /* AU( p, q ) <=> ! EU( ! q, ! p * ! q ) * ! EG( ! q ) */

  Expr      = ABL_CDR( Expr );
  BddNode1  = MochaCheckCtlAbl( ABL_CAR( Expr ) );

  Expr      = ABL_CDR( Expr );
  BddNode2  = MochaCheckCtlAbl( ABL_CAR( Expr ) );

  if ( MochaMochaFigure->FLAG_DEBUG )
  {
    fprintf( stdout, "ABL AU[\n" );
    MochaCheckViewBddNode( BddNode1 );
    fprintf( stdout, ",\n" );
    MochaCheckViewBddNode( BddNode2 );
    fprintf( stdout, "]\n" );
  }

  BddNode1 = MochaCheckCtlBddQuantify( decbddrefext( BddNode1 ) );
  BddNode2 = MochaCheckCtlBddQuantify( decbddrefext( BddNode2 ) );

  BddNode1  = applybddnodenot( (bddsystem *)0, decbddrefext( BddNode1 ) );
  BddNode2  = applybddnodenot( (bddsystem *)0, decbddrefext( BddNode2 ) );
  BddNode3  = applybddnode( (bddsystem *)0, ABL_AND, BddNode1, BddNode2 );

  BddNode1 = MochaCheckCtlBddQuantify( decbddrefext( BddNode1 ) );
  BddNode2 = MochaCheckCtlBddQuantify( decbddrefext( BddNode2 ) );
  BddNode3 = MochaCheckCtlBddQuantify( decbddrefext( BddNode3 ) );

  /* ! EG( ! q ) */
  BddResult1 = MochaCheckCtlBddEG( BddNode2 );
  BddResult1 = applybddnodenot( (bddsystem *)0, decbddrefext( BddResult1 ) );

  /* ! EU( ! q, ! p * ! q ) */
  BddResult2 = MochaCheckCtlBddEU( BddNode2, BddNode3 );
  BddResult2 = applybddnodenot( (bddsystem *)0, decbddrefext( BddResult2 ) );

  decbddrefext( BddNode1 );
  decbddrefext( BddNode2 );
  decbddrefext( BddNode3 );

  BddResult = applybddnode( (bddsystem *)0, ABL_AND,
                            decbddrefext( BddResult1 ),
                            decbddrefext( BddResult2 ) );

  BddResult = MochaCheckCtlBddQuantify( decbddrefext( BddResult ) );

  if ( MochaMochaFigure->FLAG_DEBUG )
  {
    fprintf( stdout, "-> " );
    MochaCheckViewBddNode( BddResult );
    fprintf( stdout, "\n" );
  }

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

  BddNode = MochaCheckCtlAbl( ABL_CAR( Expr ) );

  if ( MochaMochaFigure->FLAG_DEBUG )
  {
    fprintf( stdout, "ABL EF[\n" );
    MochaCheckViewBddNode( BddNode );
    fprintf( stdout, "]\n" );
  }

  BddNode   = MochaCheckCtlBddQuantify( decbddrefext( BddNode ) );
  BddResult = MochaCheckCtlBddEU( BddLocalSystem->ONE, BddNode );
  decbddrefext( BddNode );

  if ( MochaMochaFigure->FLAG_DEBUG )
  {
    fprintf( stdout, "-> " );
    MochaCheckViewBddNode( BddResult );
    fprintf( stdout, "\n" );
  }

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

  BddNode = MochaCheckCtlAbl( ABL_CAR( Expr ) );

  if ( MochaMochaFigure->FLAG_DEBUG )
  {
    fprintf( stdout, "ABL EX[\n" );
    MochaCheckViewBddNode( BddNode );
    fprintf( stdout, "]\n" );
  }

  BddNode   = MochaCheckCtlBddQuantify( decbddrefext( BddNode ) );
  BddResult = MochaCheckCtlBddEX( BddNode );
  decbddrefext( BddNode );

  if ( MochaMochaFigure->FLAG_DEBUG )
  {
    fprintf( stdout, "-> " );
    MochaCheckViewBddNode( BddResult );
    fprintf( stdout, "\n" );
  }

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

  Expr = ABL_CDR( Expr );

  BddNode = MochaCheckCtlAbl( ABL_CAR( Expr ) );

  if ( MochaMochaFigure->FLAG_DEBUG )
  {
    fprintf( stdout, "ABL EG[\n" );
    MochaCheckViewBddNode( BddNode );
    fprintf( stdout, "]\n" );
  }

  BddNode   = MochaCheckCtlBddQuantify( decbddrefext( BddNode ) );
  BddResult = MochaCheckCtlBddEG( BddNode );
  decbddrefext( BddNode );

  if ( MochaMochaFigure->FLAG_DEBUG )
  {
    fprintf( stdout, "-> " );
    MochaCheckViewBddNode( BddResult );
    fprintf( stdout, "\n" );
  }

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

  if ( MochaMochaFigure->FLAG_DEBUG )
  {
    fprintf( stdout, "ABL EU[\n" );
    MochaCheckViewBddNode( BddNode1 );
    fprintf( stdout, ",\n" );
    MochaCheckViewBddNode( BddNode2 );
    fprintf( stdout, "]\n" );
  }

  BddNode1  = MochaCheckCtlBddQuantify( decbddrefext( BddNode1 ) );
  BddNode2  = MochaCheckCtlBddQuantify( decbddrefext( BddNode2 ) );
  BddResult = MochaCheckCtlBddEU( BddNode1, BddNode2 );

  decbddrefext( BddNode1 );
  decbddrefext( BddNode2 );

  if ( MochaMochaFigure->FLAG_DEBUG )
  {
    fprintf( stdout, "-> " );
    MochaCheckViewBddNode( BddResult );
    fprintf( stdout, "\n" );
  }

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
  biablarray   *BiAblArray;
  ablarray     *AblArray;
  bddnode      *BddNode;
  bddnode      *BddFirst;
  bddnode      *BddReached;

  CtlFigure    = MochaFigure->CTL_FIGURE;
  BddReached   = MochaFigure->BDD_REACHED_STATE;
  BddFirst     = MochaFigure->BDD_FIRST_STATE;

  setbddlocalcircuit( MochaFigure->BDD_CIRCUIT );

  MochaMochaFigure = MochaFigure;

  if ( MochaMochaFigure->FLAG_DEBUG )
  {
    fprintf( stdout, "Reachable states:\n" );
    MochaCheckViewBddNode( BddReached );

    fprintf( stdout, "First state:\n" );
    MochaCheckViewBddNode( BddFirst );
  }

  for ( CtlForm  = CtlFigure->FORM;
        CtlForm != (ctlform_list *)0;
        CtlForm  = CtlForm->NEXT )
  {
    fprintf( stdout, "\t    Checking formula %s\n", CtlForm->NAME );
    fprintf( stdout, "\t    " );
    viewvexexprln( CtlForm->VEX_EXPR );

    BiAblArray = MOCHA_CTL_BIABLA( CtlForm );
    AblArray   = BiAblArray->EXPR;

    BddNode = MochaCheckCtlAbl( AblArray->ARRAY[ 0 ] );
    BddNode = MochaCheckCtlBddQuantify( decbddrefext( BddNode ) );

    if ( ! MochaCheckViewState( MochaFigure, BddNode ) )
    {
      fprintf( stdout, "\t      Not verified for the first state !\n" );
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
