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
| Tool    :                     Bhl                           |
|                                                             |
| File    :                  bhl_makbdd.c                     |
|                                                             |
| Date    :                   01.02.95                        |
|                                                             |
| Author  :               Pirouz Bazargan Sabet               |
|                                                             |
|                      Modified by Jacomme Ludovic            |
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
# include "abe.h"
# include "abt.h"

# include <stdio.h>
# include "bhl_error.h"
# include "bhl_makbdd.h"
# include "bhl_orderbdd.h"

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

  static chain_list *BhlHeadName    = (chain_list   *)0;
  static befig_list *BhlBehFigure   = (befig_list   *)0;
  static char       *BhlDelayedName = (char         *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Bhl Abl 2 Bdd                        |
|                                                             |
\------------------------------------------------------------*/

static bddnode *bhl_expr2bdd( Expr )

  chain_list *Expr;
{
  bddnode    *BddNode;
  bddnode    *BddFirst;
  char       *AtomValue;
  beaux_list *ScanAux;
  chain_list *ScanChain;
  long        Oper;
  int         Negative;

  if ( Expr == (chain_list *)0 )
  {
    bhlerror( BHL_EXPR_NULL_ERROR, (char *)0 );
  }

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

    for ( ScanAux  = BhlBehFigure->BEAUX;
          ScanAux != (beaux_list *)0;
          ScanAux  = ScanAux->NEXT )
    {
      if ( ScanAux->NAME == AtomValue ) break;
    }

    if ( ScanAux == (beaux_list *)0 )
    {
      bhlerror( BHL_UNKNOWN_ATOM_ERROR, AtomValue );
    }

    for ( ScanChain  = BhlHeadName;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      if ( ScanChain->DATA == AtomValue )
      {
        bhlerror( BHL_EXPR_LOOP_ERROR, AtomValue );
      }
    }

    if ( ScanAux->NODE == (void *)0 )
    {
      BhlHeadName   = addchain( BhlHeadName, AtomValue );
      ScanAux->NODE = bhl_expr2bdd( ScanAux->ABL );
      BhlHeadName   = delchain( BhlHeadName, BhlHeadName );
    }

    return( incbddrefext( ScanAux->NODE ) );
  }

  Oper = ABL_OPER( Expr );
  Expr = ABL_CDR( Expr );

  if ( Oper == ABL_NOT )
  {
    if ( ABL_CDR( Expr ) != (chain_list *)0 )
    {
      bhlerror( BHL_OPERATOR_ERROR, Oper );
    }

    BddFirst = bhl_expr2bdd( ABL_CAR( Expr ) );
    BddNode  = applybddnodenot( (bddsystem *)0, decbddrefext( BddFirst ) );

    return( BddNode );
  }

  if ( Oper == ABL_STABLE )
  {
    Expr = ABL_CAR( Expr );

    if ( ! ABL_ATOM( Expr ) )
    {
      bhlerror( BHL_ILLEGAL_STABLE_ERROR, (char *)0 );
    }

    AtomValue = ABL_ATOM_VALUE( Expr );
    BddFirst  = searchbddcircuitin( (bddcircuit *)0, AtomValue );

    if ( BddFirst == (bddnode *)0 )
    {
      bhlerror( BHL_UNKNOWN_ATOM_ERROR, AtomValue );
    }

    BhlDelayedName = autresizeblock( BhlDelayedName, 0, strlen( AtomValue ) + 9 );
    sprintf( BhlDelayedName, "%s'delayed", AtomValue );
    AtomValue = namealloc( BhlDelayedName );

    BddNode = searchbddcircuitin( (bddcircuit *)0, AtomValue );

    if ( BddNode == (bddnode *)0 )
    {
      bhlerror( BHL_UNKNOWN_ATOM_ERROR, AtomValue );
    }

    BddNode = applybddnode( (bddsystem *)0, ABL_NXOR, BddNode, BddFirst );

    return( BddNode );
  }

  if ( ( isablunaryoper( Oper )             ) ||
       ( ABL_CDR( Expr ) == (chain_list *)0 ) )
  {
    bhlerror( BHL_OPERATOR_ERROR, Oper );
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

  BddFirst = bhl_expr2bdd( ABL_CAR( Expr ) );

  while ( ( Expr = ABL_CDR( Expr ) ) != (chain_list *)0 )
  {
    BddNode  = bhl_expr2bdd( ABL_CAR( Expr ) );
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
|                        Beh Abl 2 Bdd                        |
|                                                             |
\------------------------------------------------------------*/

static bddnode *bhl_abl2bdd( Name, Expr )

  char       *Name;
  chain_list *Expr;
{
  bddnode *BddNode;

  if ( Name != (char *)0 )
  {
    BhlHeadName = addchain( BhlHeadName, Name );
  }

  BddNode = bhl_expr2bdd( Expr );

  if ( Name != (char *)0 )
  {
    BhlHeadName = delchain( BhlHeadName, BhlHeadName );

    BddNode = addbddcircuitout( (bddcircuit *)0, Name, BddNode );
  }

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                         Beh Make Bdd Total                  |
|                                                             |
\------------------------------------------------------------*/

void bhl_mbddtot( BehFigure )

  befig_list *BehFigure;
{
  struct beaux  *BehAux;
  struct beaux  *BehDly;
  struct bemsg  *BehMsg;
  struct beout  *BehOut;
  struct bebus  *BehBus;
  struct bebux  *BehBux;
  struct bereg  *BehReg;
  struct biabl  *BiAbl;
  struct binode *BiNode;

  BhlBehFigure = BehFigure;
  BhlHeadName  = (chain_list *)0;

  BehAux = BehFigure->BEAUX;

  while ( BehAux != NULL )
  {
    if ( BehAux->NODE == NULL )
    {
      BehAux->NODE = bhl_abl2bdd( BehAux->NAME, BehAux->ABL );
    }
    BehAux = BehAux->NEXT;
  }

  BehDly = BehFigure->BEDLY;

  while ( BehDly != NULL )
  {
    if ( BehDly->NODE == NULL )
    {
      BehDly->NODE = bhl_abl2bdd( BehDly->NAME, BehDly->ABL );
    }
    BehDly = BehDly->NEXT;
  }

  BehMsg = BehFigure->BEMSG;

  while ( BehMsg != NULL )
  {
    if ( BehMsg->NODE == NULL )
    {
      BehMsg->NODE = bhl_abl2bdd( (char *)0, BehMsg->ABL );
    }
    BehMsg = BehMsg->NEXT;
  }

  BehOut = BehFigure->BEOUT;

  while ( BehOut != NULL )
  {
    if ( BehOut->NODE == NULL )
    {
      BehOut->NODE = bhl_abl2bdd( BehOut->NAME, BehOut->ABL );
    }

    BehOut = BehOut->NEXT;
  }

  BehBus = BehFigure->BEBUS;

  while ( BehBus != NULL )
  {
    BiAbl  = BehBus->BIABL;
    BiNode = BehBus->BINODE;

    while (BiAbl != NULL)
    {
      BiNode->CNDNODE = bhl_abl2bdd( (char *)0, BiAbl->CNDABL );
      BiNode->VALNODE = bhl_abl2bdd( BehBus->NAME, BiAbl->VALABL );

      BiAbl = BiAbl->NEXT;
      BiNode = BiNode->NEXT;
    }

    BehBus = BehBus->NEXT;
  }

  BehBux = BehFigure->BEBUX;

  while ( BehBux != NULL )
  {
    BiAbl  = BehBux->BIABL;
    BiNode = BehBux->BINODE;

    while ( BiAbl != NULL )
    {
      BiNode->CNDNODE = bhl_abl2bdd( (char *)0   , BiAbl->CNDABL );
      BiNode->VALNODE = bhl_abl2bdd( BehBux->NAME, BiAbl->VALABL );

      BiAbl  = BiAbl->NEXT;
      BiNode = BiNode->NEXT;
    }

    BehBux = BehBux->NEXT;
  }

  BehReg = BehFigure->BEREG;

  while ( BehReg != NULL )
  {
    BiAbl  = BehReg->BIABL;
    BiNode = BehReg->BINODE;

    while ( BiAbl != NULL )
    {
      BiNode->CNDNODE = bhl_abl2bdd( (char *)0   , BiAbl->CNDABL );
      BiNode->VALNODE = bhl_abl2bdd( BehReg->NAME, BiAbl->VALABL );

      BiAbl  = BiAbl->NEXT;
      BiNode = BiNode->NEXT;
    }

    BehReg = BehReg->NEXT;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                         Beh Make Bdd                        |
|                                                             |
\------------------------------------------------------------*/

void beh_makbdd( ptr_befig, aux_flag, order_flag )

  befig_list   *ptr_befig;
  char          aux_flag;
  char          order_flag;
{
  berin_list *ptr_rin;
  beaux_list *ptr_aux;
  beout_list *ptr_out;
  bebus_list *ptr_bus;

  long  count_rin = 1;
  long  count_out = 1;

  if ( ! aux_flag )
  {
    if ( ptr_befig->BEDLY == NULL )
    {
      ptr_aux = ptr_befig->BEAUX;
      while ( ptr_aux != NULL )
      {
        ptr_befig->BERIN = beh_rmvberin( ptr_befig->BERIN, ptr_aux->NAME );
        ptr_aux = ptr_aux->NEXT;
        count_out++;
      }
    }
    else
    {
      bhlerror( BHL_SIMPLIFY_ERROR, (char *)0 );
    }
  }
  else
  {
    ptr_aux = ptr_befig->BEAUX;
    while ( ptr_aux != NULL )
    {
      ptr_aux = ptr_aux->NEXT;
      count_out++;
    }
  }

  ptr_rin = ptr_befig->BERIN;
  while (ptr_rin != NULL)
  {
    ptr_rin = ptr_rin->NEXT;
    count_rin++;
  }

  ptr_out = ptr_befig->BEOUT;
  while (ptr_out != NULL)
  {
    ptr_out = ptr_out->NEXT;
    count_out++;
  }

  ptr_bus = ptr_befig->BEBUS;
  while (ptr_bus != NULL)
  {
    ptr_bus = ptr_bus->NEXT;
    count_out++;
  }

  ptr_befig->CIRCUI = createbddcircuit( ptr_befig->NAME, 
                                        count_rin, count_out, (bddsystem *)0 );

  bhl_orderbdd( ptr_befig, aux_flag, order_flag );

  bhl_mbddtot( ptr_befig );

  if ( ! aux_flag )
  {
    ptr_aux = ptr_befig->BEAUX;
    while ( ptr_aux != NULL )
    {
      if ( ptr_aux->NODE != (void *)0 )
      {
        decbddrefext( ptr_aux->NODE );
        delbddcircuitout( (bddcircuit *)0, ptr_aux->NAME );
      }
      ptr_aux = ptr_aux->NEXT;
    }

    beh_frebeaux( ptr_befig->BEAUX );
    ptr_befig->BEAUX = NULL;
  }

  sweepbddsystem( (bddsystem *)0 );
}
