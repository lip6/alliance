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
| Tool    :                     Boom                          |
|                                                             |
| File    :                   boom_bdd.c                      |
|                                                             |
| Date    :                   06.06.00                        |
|                                                             |
| Author  :                Jacomme Ludovic                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include MUT_H
# include AUT_H
# include ABL_H
# include BDD_H
# include ABE_H

# include <stdio.h>
# include <stdlib.h>

# include "boom_bdd.h"
# include "boom_abl.h"
# include "boom_order.h"
# include "boom_error.h"
# include "boom_debug.h"
# include "boom_shared.h"

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

  static chain_list *BoomBddHeadName    = (chain_list   *)0;
  static befig_list *BoomBddBehFigure   = (befig_list   *)0;
  static char        BoomBddMarkAbl     = 0;

  static long        BoomBddNumberNode  = 0;

  static authtable  *BoomBddHashAux       = (authtable *)0;
  static authtable  *BoomBddHashKeepAux   = (authtable *)0;
  static authtable  *BoomBddHashRenameAux = (authtable *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Boom Abl 2 Bdd                       |
|                                                             |
\------------------------------------------------------------*/

static bddnode *BoomExpr2BddNode( Expr )

  chain_list *Expr;
{
  bddnode    *BddNode;
  bddnode    *BddFirst;
  char       *AtomValue;
  beaux_list *ScanAux;
  chain_list *ScanChain;
  chain_list *ScanExpr;
  long        Oper;
  int         Negative;

  if ( Expr == (chain_list *)0 )
  {
    BoomError( BOOM_EXPR_NULL_ERROR, (char *)0 );
  }

  if ( ABL_ATOM( Expr ) )
  {
    AtomValue = ABL_ATOM_VALUE( Expr );

    if ( AtomValue == ABL_ATOM_NAME_ONE )
    {
      return( BddLocalSystem->ONE );
    }

    if ( AtomValue == ABL_ATOM_NAME_ZERO )
    {
      return( BddLocalSystem->ZERO );
    }

    BddNode = searchbddcircuitin( (bddcircuit *)0, AtomValue );

    if ( BddNode != (bddnode *)0 )
    {
      return( incbddrefext( BddNode ) );
    }

    for ( ScanAux  = BoomBddBehFigure->BEAUX;
          ScanAux != (beaux_list *)0;
          ScanAux  = ScanAux->NEXT )
    {
      if ( ScanAux->NAME == AtomValue ) break;
    }

    if ( ScanAux == (beaux_list *)0 )
    {
      BoomError( BOOM_UNKNOWN_ATOM_ERROR, AtomValue );
    }

    for ( ScanChain  = BoomBddHeadName;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      if ( ScanChain->DATA == AtomValue )
      {
        BoomError( BOOM_EXPR_LOOP_ERROR, AtomValue );
      }
    }

    if ( ScanAux->NODE == (void *)0 )
    {
      BoomBddHeadName = addchain( BoomBddHeadName, AtomValue );
      ScanAux->NODE  = (void *)BoomExpr2BddNode( ScanAux->ABL );
      BoomBddHeadName = delchain( BoomBddHeadName, BoomBddHeadName );
    }

    BddNode = ScanAux->NODE;

    return( incbddrefext( BddNode ) );
  }

  Oper     = ABL_OPER( Expr );
  ScanExpr = ABL_CDR( Expr );

  if ( Oper == ABL_NOT )
  {
    if ( ABL_CDR( ScanExpr ) != (chain_list *)0 )
    {
      BoomError( BOOM_OPERATOR_ERROR, Oper );
    }

    BddFirst = BoomExpr2BddNode( ABL_CAR( ScanExpr ) );

    BddNode = applybddnodenot( (bddsystem *)0,
                               decbddrefext( BddFirst ) );

    if ( BoomBddMarkAbl ) BoomSetBddMarkAbl( Expr, BddNode );

    return( BddNode );
  }

  if ( Oper == ABL_STABLE )
  {
    ScanExpr = ABL_CAR( ScanExpr );

    if ( ! ABL_ATOM( ScanExpr ) )
    {
      BoomError( BOOM_ILLEGAL_STABLE_ERROR, (char *)0 );
    }

    AtomValue = ABL_ATOM_VALUE( ScanExpr );
    BddFirst  = searchbddcircuitin( (bddcircuit *)0, AtomValue );

    if ( BddFirst == (bddnode *)0 )
    {
      BoomError( BOOM_UNKNOWN_ATOM_ERROR, AtomValue );
    }

    AtomValue = getbddstablename( AtomValue );
    BddNode   = searchbddcircuitin( (bddcircuit *)0, AtomValue );

    if ( BddNode == (bddnode *)0 )
    {
      BddNode = addbddcircuitin( (bddcircuit *)0, AtomValue,
                                 (bddindex    )0, BDD_IN_MODE_LAST );
    }

    if ( BoomBddMarkAbl ) BoomSetBddMarkAbl( Expr, BddNode );

    return( BddNode );
  }

  if ( ( isablunaryoper( Oper )                 ) ||
       ( ABL_CDR( ScanExpr ) == (chain_list *)0 ) )
  {
    BoomError( BOOM_OPERATOR_ERROR, Oper );
  }

  if ( ( getabloperpolar( Oper ) == ABL_POLAR_POSITIVE ) ||
       ( ABL_CDDR( ScanExpr )    == (chain_list *)0    ) )
  {
    Negative = 0;
  }
  else
  {
    Negative = 1;
    Oper     = getablopernot( Oper );
  }

  BddFirst = BoomExpr2BddNode( ABL_CAR( ScanExpr ) );

  while ( ( ScanExpr = ABL_CDR( ScanExpr ) ) != (chain_list *)0 )
  {
    BddNode  = BoomExpr2BddNode( ABL_CAR( ScanExpr ) );

    BddFirst = applybddnode( (bddsystem *)0, Oper, 
                             decbddrefext( BddFirst ),
                             decbddrefext( BddNode  ) );
  }

  if ( Negative )
  {
    BddFirst = applybddnodenot( (bddsystem *)0,
                                decbddrefext( BddFirst ) );
  }

  if ( BoomBddMarkAbl ) BoomSetBddMarkAbl( Expr, BddFirst );

  return( BddFirst );
}

/*------------------------------------------------------------\
|                                                             |
|                        Beh Abl 2 Bdd                        |
|                                                             |
\------------------------------------------------------------*/

static bddnode *BoomAbl2BddNode( Name, Expr )

  char       *Name;
  chain_list *Expr;
{
  bddnode *BddNode;

  if ( Name != (char *)0 )
  {
    BoomBddHeadName = addchain( BoomBddHeadName, Name );
  }

  BddNode = BoomExpr2BddNode( Expr );

  if ( Name != (char *)0 )
  {
    BoomBddHeadName = delchain( BoomBddHeadName, BoomBddHeadName );
  }

  if ( IsBddSystemExplosion( BddLocalSystem ) )
  {
    if ( IsBoomDebugLevel0() )
    {
      BoomPrintf( stdout, "Explosion !!! %ld\n", BddLocalSystem->NUMBER_NODE );
    }

    garbagebddsystem( BddLocalSystem );

    if ( IsBoomDebugLevel0() )
    {
      BoomPrintf( stdout, "Explosion !!! %ld\n", BddLocalSystem->NUMBER_NODE );
    }

    explosionbddsystemrestart( BddLocalSystem );

    explosionbddsystemdynamic( BddLocalSystem, NULL, BOOM_BDD_EXPLOSION_LIMIT );

    BddNode = (bddnode *)0;
  }
  else
  if ( Name != (char *)0 )
  {
    BddNode = addbddcircuitout( (bddcircuit *)0, Name, BddNode );
  }

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                         Beh Make Bdd                        |
|                                                             |
\------------------------------------------------------------*/

void BoomBehMakeBddNode( BehFigure, MarkAbl )

  befig_list *BehFigure;
  int         MarkAbl;
{
  beaux_list  *BehAux;
  beout_list  *BehOut;
  bebus_list  *BehBus;
  bebux_list  *BehBux;
  bereg_list  *BehReg;
  biabl_list  *BiAbl;
  binode_list *BiNode;

  BoomBddBehFigure = BehFigure;
  BoomBddHeadName  = (chain_list *)0;
  BoomBddMarkAbl   = MarkAbl;

  for ( BehAux  = BehFigure->BEAUX;
        BehAux != (beaux_list *)0;
        BehAux  = BehAux->NEXT )
  {
    if ( BehAux->NODE != (void *)0 ) decbddrefext( BehAux->NODE );

    if ( ! IsBoomBehAuxDontTouch( BehAux ) )
    {
      BehAux->NODE = (void *)BoomAbl2BddNode( BehAux->NAME, BehAux->ABL );
    }
    else
    {
      BehAux->NODE = (void *)0;
    }
  }

  for ( BehBus  = BehFigure->BEBUS;
        BehBus != (bebus_list *)0;
        BehBus  = BehBus->NEXT )
  {
    BiNode = BehBus->BINODE;

    for ( BiAbl  = BehBus->BIABL;
          BiAbl != (biabl_list *)0;
          BiAbl  = BiAbl->NEXT ) 
    {
      if ( BiNode->VALNODE != (void *)0 ) decbddrefext( BiNode->VALNODE );
      if ( BiNode->CNDNODE != (void *)0 ) decbddrefext( BiNode->CNDNODE );

      if ( ! IsBoomBehAuxDontTouch( BehBus ) )
      {
        BiNode->VALNODE = (void *)BoomAbl2BddNode( BehBus->NAME, BiAbl->VALABL );
        BiNode->CNDNODE = (void *)BoomAbl2BddNode( (char *)0, BiAbl->CNDABL );
      }
      else
      {
        BiNode->VALNODE = (void *)0;
        BiNode->CNDNODE = (void *)0;
      }

      BiNode = BiNode->NEXT;
    }
  }

  for ( BehBux  = BehFigure->BEBUX;
        BehBux != (bebux_list *)0;
        BehBux  = BehBux->NEXT )
  {
    BiNode = BehBux->BINODE;

    for ( BiAbl  = BehBux->BIABL;
          BiAbl != (biabl_list *)0;
          BiAbl  = BiAbl->NEXT ) 
    {
      if ( BiNode->VALNODE != (void *)0 ) decbddrefext( BiNode->VALNODE );
      if ( BiNode->CNDNODE != (void *)0 ) decbddrefext( BiNode->CNDNODE );

      if ( ! IsBoomBehAuxDontTouch( BehBux ) )
      {
        BiNode->VALNODE = (void *)BoomAbl2BddNode( BehBux->NAME, BiAbl->VALABL );
        BiNode->CNDNODE = (void *)BoomAbl2BddNode( (char *)0, BiAbl->CNDABL );
      }
      else
      {
        BiNode->VALNODE = (void *)0;
        BiNode->CNDNODE = (void *)0;
      }

      BiNode = BiNode->NEXT;
    }
  }

  for ( BehReg  = BehFigure->BEREG;
        BehReg != (bereg_list *)0;
        BehReg  = BehReg->NEXT )
  {
    BiNode = BehReg->BINODE;

    for ( BiAbl  = BehReg->BIABL;
          BiAbl != (biabl_list *)0;
          BiAbl  = BiAbl->NEXT ) 
    {
      if ( BiNode->VALNODE != (void *)0 ) decbddrefext( BiNode->VALNODE );
      if ( BiNode->CNDNODE != (void *)0 ) decbddrefext( BiNode->CNDNODE );

      if ( ! IsBoomBehAuxDontTouch( BehReg ) )
      {
        BiNode->VALNODE = (void *)BoomAbl2BddNode( BehReg->NAME, BiAbl->VALABL );
        BiNode->CNDNODE = (void *)BoomAbl2BddNode( (char *)0, BiAbl->CNDABL );
      }
      else
      {
        BiNode->VALNODE = (void *)0;
        BiNode->CNDNODE = (void *)0;
      }

      BiNode = BiNode->NEXT;
    }
  }

  for ( BehOut  = BehFigure->BEOUT;
        BehOut != (beout_list *)0;
        BehOut  = BehOut->NEXT )
  {
    if ( BehOut->NODE != (void *)0 ) decbddrefext( BehOut->NODE );

    if ( ! IsBoomBehAuxDontTouch( BehOut ) )
    {
      BehOut->NODE = (void *)BoomAbl2BddNode( BehOut->NAME, BehOut->ABL );
    }
    else
    {
      BehOut->NODE = (void *)0;
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       BoomBddSetExprKeepAux                 |
|                                                             |
\------------------------------------------------------------*/

static long BoomBddSetExprKeepAux( Expr )

  ablexpr *Expr;
{
  char          *Value;
  char          *Name;
  char          *Vector;
  beaux_list    *BehAux;
  bddnode       *BddNode;
  authelem      *Element;
  long           Index;
  long           Number;

  Number = 0;

  if ( ABL_ATOM( Expr ) )
  {
    Value = ABL_ATOM_VALUE( Expr );

    if ( ( Value != ABL_ATOM_NAME_ONE      ) &&
         ( Value != ABL_ATOM_NAME_ZERO     ) &&
         ( Value != ABL_ATOM_NAME_DC       ) &&
         ( Value != ABL_ATOM_NAME_TRISTATE ) )
    {
      Element = searchauthelem( BoomBddHashAux, Value );

      if ( Element != (authelem *)0 )
      {
        BehAux  = (beaux_list *)Element->VALUE;
        Element = searchauthelem( BoomBddHashKeepAux, Value );

        if ( Element == (authelem *)0 )
        {
          boombefiginfo *FigInfo;

          FigInfo = GetBoomBehFigInfo( BoomBddBehFigure );

          Name = vhdlablvector( BehAux->NAME, &Index );
  
          if ( Name != (char *)0 )
          {
            for ( BehAux  = BoomBddBehFigure->BEAUX;
                  BehAux != (beaux_list *)0;
                  BehAux  = BehAux->NEXT )
            {
              Vector = vhdlablvector( BehAux->NAME, &Index );
  
              if ( Vector == Name )
              {
                if ( IsBoomDebugLevel0() )
                {
                  BoomPrintf( stdout, "Keep %s\n", BehAux->NAME );
                }

                if ( FigInfo->TRACE )
                {
                  fprintf( stdout, "\t    Keep signal %s\n", BehAux->NAME );
                }

                addauthelem( BoomBddHashKeepAux, BehAux->NAME, 0 );
                SetBoomBehAuxKeep( BehAux );
                BddNode = addbddcircuitin( (bddcircuit *)0, BehAux->NAME,
                                           0, BDD_IN_MODE_LAST );
  
                Number++;
              }
            }
          }
          else
          {
            if ( IsBoomDebugLevel0() )
            {
              BoomPrintf( stdout, "Keep %s\n", BehAux->NAME );
            }

            if ( FigInfo->TRACE )
            {
              fprintf( stdout, "\t    Keep signal %s\n", BehAux->NAME );
            }

            addauthelem( BoomBddHashKeepAux, BehAux->NAME, 0 );
            SetBoomBehAuxKeep( BehAux );
            BddNode = addbddcircuitin( (bddcircuit *)0, BehAux->NAME, 0, BDD_IN_MODE_LAST );
  
            Number++;
          }
        }
        else
        {
          Number++;
        }
      }
    }
  }
  else
  {
    while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
    {
      Number += BoomBddSetExprKeepAux( ABL_CAR( Expr ) );
    }
  }

  return( Number ); 
}

/*------------------------------------------------------------\
|                                                             |
|                  Beh Make Bdd No Explosion                  |
|                                                             |
\------------------------------------------------------------*/

void BoomBehMakeBddNodeNoExplosion( BehFigure )

  befig_list *BehFigure;
{
  boombefiginfo *FigInfo;
  beaux_list    *BehAux;
  beout_list    *BehOut;
  bebus_list    *BehBus;
  bebux_list    *BehBux;
  bereg_list    *BehReg;
  biabl_list    *BiAbl;
  binode_list   *BiNode;
  bddnode       *BddNode;
  char          *Name;
  char          *Vector;
  long           Index;
  int            Retry;

  FigInfo = GetBoomBehFigInfo( BehFigure ); 

  BoomBddBehFigure = BehFigure;
  BoomBddHeadName  = (chain_list *)0;
  BoomBddMarkAbl   = 0;

  explosionbddsystemdynamic( (bddsystem *)0, NULL, BOOM_BDD_EXPLOSION_LIMIT );

  if ( BoomBddHashAux == (authtable *)0 )
  {
    BoomBddHashAux       = createauthtable( 100 );
    BoomBddHashKeepAux   = createauthtable( 100 );
    BoomBddHashRenameAux = createauthtable( 100 );
  }

  for ( BehAux  = BehFigure->BEAUX;
        BehAux != (beaux_list *)0;
        BehAux  = BehAux->NEXT )
  {
    addauthelem( BoomBddHashAux, BehAux->NAME, (long)BehAux );
  }

  Retry = 1;

  while ( Retry )
  {
    Retry = 0;

    for ( BehAux  = BehFigure->BEAUX;
          ( BehAux != (beaux_list *)0 ) && ( ! Retry );
          BehAux  = BehAux->NEXT )
    {
      if ( BehAux->NODE != (void *)0 ) decbddrefext( BehAux->NODE );
  
      if ( ! IsBoomBehAuxDontTouch( BehAux ) )
      {
        BehAux->NODE = (void *)BoomAbl2BddNode( BehAux->NAME, BehAux->ABL );
/*
** Explosion !
*/
        if ( BehAux->NODE == (void *)0 )
        {
          if ( IsBoomDebugLevel0() )
          {
            BoomPrintf( stdout, "Explosion while building %s\n", BehAux->NAME );
          }

          Retry = 1;

          Name = vhdlablvector( BehAux->NAME, &Index );

          if ( Name != (char *)0 ) 
          {
            for ( BehAux  = BehFigure->BEAUX;
                  BehAux != (beaux_list *)0;
                  BehAux  = BehAux->NEXT )
            {
              Vector = vhdlablvector( BehAux->NAME, &Index );

              if ( Vector == Name )
              {
                BddNode = addbddcircuitin( (bddcircuit *)0, BehAux->NAME,
                                            0, BDD_IN_MODE_LAST );

                if ( IsBoomBehAuxKeep( BehAux ) )
                {
                  SetBoomBehAuxDontTouch( BehAux );

                  if ( FigInfo->TRACE )
                  {
                    fprintf( stdout, "\t    Don't touch signal %s\n", BehAux->NAME );
                  }
                }
                else
                if ( FigInfo->TRACE )
                {
                  fprintf( stdout, "\t    Keep signal %s\n", BehAux->NAME );
                }

                SetBoomBehAuxKeep( BehAux );

                if ( BoomBddSetExprKeepAux( BehAux->ABL ) == 0 )
                {
                  SetBoomBehAuxDontTouch( BehAux );
                }
              }
            }
          }
          else
          {
            BddNode = addbddcircuitin( (bddcircuit *)0, BehAux->NAME, 0, BDD_IN_MODE_LAST );

            if ( IsBoomBehAuxKeep( BehAux ) )
            {
              SetBoomBehAuxDontTouch( BehAux );
            }

            SetBoomBehAuxKeep( BehAux );
            BoomBddSetExprKeepAux( BehAux->ABL );
          }

          break;
        }
      }
      else
      {
        BehAux->NODE = (void *)0;
      }
    }
  
    for ( BehBus  = BehFigure->BEBUS;
          ( BehBus != (bebus_list *)0 ) && ( ! Retry );
          BehBus  = BehBus->NEXT )
    {
      BiNode = BehBus->BINODE;
  
      for ( BiAbl  = BehBus->BIABL;
            BiAbl != (biabl_list *)0;
            BiAbl  = BiAbl->NEXT ) 
      {
        if ( BiNode->VALNODE != (void *)0 ) decbddrefext( BiNode->VALNODE );
        if ( BiNode->CNDNODE != (void *)0 ) decbddrefext( BiNode->CNDNODE );

        BiNode->VALNODE = (void *)0;
        BiNode->CNDNODE = (void *)0;
  
        if ( ! IsBoomBehAuxDontTouch( BehBus ) )
        {
          BiNode->VALNODE = (void *)BoomAbl2BddNode( BehBus->NAME, BiAbl->VALABL );

          if ( BiNode->VALNODE == (void *)0 )
          {
            if ( IsBoomDebugLevel0() )
            {
              BoomPrintf( stdout, "Explosion while building %s\n", BehBus->NAME );
            }

            Retry = 1;

            if ( IsBoomBehAuxKeep( BehBus ) )
            {
              if ( FigInfo->TRACE )
              {
                fprintf( stdout, "\t    Don't touch bus signal %s\n", BehBus->NAME );
              }

              SetBoomBehAuxDontTouch( BehBus );
            }
            else
            if ( FigInfo->TRACE )
            {
              fprintf( stdout, "\t    Keep bus signal %s\n", BehBus->NAME );
            }

            SetBoomBehAuxKeep( BehBus );

            if ( BoomBddSetExprKeepAux( BiAbl->VALABL ) == 0 )
            {
              SetBoomBehAuxDontTouch( BehBus );
            }

            break;
          }
  
          BiNode->CNDNODE = (void *)BoomAbl2BddNode( (char *)0, BiAbl->CNDABL );

          if ( BiNode->CNDNODE == (void *)0 )
          {
            if ( IsBoomDebugLevel0() )
            {
              BoomPrintf( stdout, "Explosion while building %s\n", BehBus->NAME );
            }

            Retry = 1;

            if ( IsBoomBehAuxKeep( BehBus ) )
            {
              SetBoomBehAuxDontTouch( BehBus );

              if ( FigInfo->TRACE )
              {
                fprintf( stdout, "\t    Don't touch bus signal %s\n", BehBus->NAME );
              }
            }
            else
            if ( FigInfo->TRACE )
            {
              fprintf( stdout, "\t    Keep bus signal %s\n", BehBus->NAME );
            }

            SetBoomBehAuxKeep( BehBus );

            if ( BoomBddSetExprKeepAux( BiAbl->CNDABL ) == 0 )
            {
              SetBoomBehAuxDontTouch( BehBus );
            }

            break;
          }
        }
        else
        {
          BiNode->VALNODE = (void *)0;
          BiNode->CNDNODE = (void *)0;
        }
  
        BiNode = BiNode->NEXT;
      }
    }

    for ( BehBux  = BehFigure->BEBUX;
          ( BehBux != (bebux_list *)0 ) && ( ! Retry );
          BehBux  = BehBux->NEXT )
    {
      BiNode = BehBux->BINODE;
  
      for ( BiAbl  = BehBux->BIABL;
            BiAbl != (biabl_list *)0;
            BiAbl  = BiAbl->NEXT ) 
      {
        if ( BiNode->VALNODE != (void *)0 ) decbddrefext( BiNode->VALNODE );
        if ( BiNode->CNDNODE != (void *)0 ) decbddrefext( BiNode->CNDNODE );
  
        BiNode->VALNODE = (void *)0;
        BiNode->CNDNODE = (void *)0;

        if ( ! IsBoomBehAuxDontTouch( BehBux ) )
        {
          BiNode->VALNODE = (void *)BoomAbl2BddNode( BehBux->NAME, BiAbl->VALABL );

          if ( BiNode->VALNODE == (void *)0 )
          {
            if ( IsBoomDebugLevel0() )
            {
              BoomPrintf( stdout, "Explosion while building %s\n", BehBux->NAME );
            }

            Retry = 1;

            if ( IsBoomBehAuxKeep( BehBux ) )
            {
              if ( FigInfo->TRACE )
              {
                fprintf( stdout, "\t    Don't touch bus signal %s\n", BehBux->NAME );
              }

              SetBoomBehAuxDontTouch( BehBux );
            }
            else
            if ( FigInfo->TRACE )
            {
              fprintf( stdout, "\t    Keep bux signal %s\n", BehBux->NAME );
            }

            SetBoomBehAuxKeep( BehBux );

            if ( BoomBddSetExprKeepAux( BiAbl->VALABL ) == 0 )
            {
              SetBoomBehAuxDontTouch( BehBux );
            }

            break;
          }
  
          BiNode->CNDNODE = (void *)BoomAbl2BddNode( (char *)0, BiAbl->CNDABL );

          if ( BiNode->CNDNODE == (void *)0 )
          {
            if ( IsBoomDebugLevel0() )
            {
              BoomPrintf( stdout, "Explosion while building %s\n", BehBux->NAME );
            }

            Retry = 1;

            if ( IsBoomBehAuxKeep( BehBux ) )
            {
              SetBoomBehAuxDontTouch( BehBux );

              if ( FigInfo->TRACE )
              {
                fprintf( stdout, "\t    Don't touch bus signal %s\n", BehBux->NAME );
              }
            }
            else
            if ( FigInfo->TRACE )
            {
              fprintf( stdout, "\t    Keep bus signal %s\n", BehBux->NAME );
            }

            SetBoomBehAuxKeep( BehBux );

            if ( BoomBddSetExprKeepAux( BiAbl->CNDABL ) == 0 )
            {
              SetBoomBehAuxDontTouch( BehBux );
            }

            break;
          }
        }
        else
        {
          BiNode->VALNODE = (void *)0;
          BiNode->CNDNODE = (void *)0;
        }
  
        BiNode = BiNode->NEXT;
      }
    }
  
    for ( BehReg  = BehFigure->BEREG;
          ( BehReg != (bereg_list *)0 ) && ( ! Retry );
          BehReg  = BehReg->NEXT )
    {
      BiNode = BehReg->BINODE;
  
      for ( BiAbl  = BehReg->BIABL;
            BiAbl != (biabl_list *)0;
            BiAbl  = BiAbl->NEXT ) 
      {
        if ( BiNode->VALNODE != (void *)0 ) decbddrefext( BiNode->VALNODE );
        if ( BiNode->CNDNODE != (void *)0 ) decbddrefext( BiNode->CNDNODE );
  
        BiNode->VALNODE = (void *)0;
        BiNode->CNDNODE = (void *)0;

        if ( ! IsBoomBehAuxDontTouch( BehReg ) )
        {
          BiNode->VALNODE = (void *)BoomAbl2BddNode( BehReg->NAME, BiAbl->VALABL );

          if ( BiNode->VALNODE == (void *)0 )
          {
            if ( IsBoomDebugLevel0() )
            {
              BoomPrintf( stdout, "Explosion while building %s\n", BehReg->NAME );
            }

            Retry = 1;

            if ( IsBoomBehAuxKeep( BehReg ) )
            {
              SetBoomBehAuxDontTouch( BehReg );

              if ( FigInfo->TRACE )
              {
                fprintf( stdout, "\t    Don't touch register signal %s\n", BehReg->NAME );
              }
            }
            else
            if ( FigInfo->TRACE )
            {
              fprintf( stdout, "\t    Keep register signal %s\n", BehReg->NAME );
            }

            SetBoomBehAuxKeep( BehReg );

            if ( BoomBddSetExprKeepAux( BiAbl->VALABL ) == 0 )
            {
              SetBoomBehAuxDontTouch( BehReg );
            }

            break;
          }
  
          BiNode->CNDNODE = (void *)BoomAbl2BddNode( (char *)0, BiAbl->CNDABL );

          if ( BiNode->CNDNODE == (void *)0 )
          {
            if ( IsBoomDebugLevel0() )
            {
              BoomPrintf( stdout, "Explosion while building %s\n", BehReg->NAME );
            }

            Retry = 1;

            if ( IsBoomBehAuxKeep( BehReg ) )
            {
              if ( FigInfo->TRACE )
              {
                fprintf( stdout, "\t    Don't touch register signal %s\n", BehReg->NAME );
              }

              SetBoomBehAuxDontTouch( BehReg );
            }
            else
            if ( FigInfo->TRACE )
            {
              fprintf( stdout, "\t    Keep register signal %s\n", BehReg->NAME );
            }

            SetBoomBehAuxKeep( BehReg );

            if ( BoomBddSetExprKeepAux( BiAbl->CNDABL ) == 0 )
            {
              SetBoomBehAuxDontTouch( BehReg );
            }

            break;
          }
        }
        else
        {
          BiNode->VALNODE = (void *)0;
          BiNode->CNDNODE = (void *)0;
        }
  
        BiNode = BiNode->NEXT;
      }
    }
  
    for ( BehOut  = BehFigure->BEOUT;
          ( BehOut != (beout_list *)0 ) && ( ! Retry );
          BehOut  = BehOut->NEXT )
    {
      if ( BehOut->NODE != (void *)0 ) decbddrefext( BehOut->NODE );
  
      if ( ! IsBoomBehAuxDontTouch( BehOut ) )
      {
        BehOut->NODE = (void *)BoomAbl2BddNode( BehOut->NAME, BehOut->ABL );

        if ( BehOut->NODE == (void *)0 )
        {
/*
** Explosion !
*/
          if ( IsBoomDebugLevel0() )
          {
            BoomPrintf( stdout, "Explosion while building %s\n", BehOut->NAME );
          }

          Retry = 1;

          if ( IsBoomBehAuxKeep( BehOut ) )
          {
            if ( FigInfo->TRACE )
            {
              fprintf( stdout, "\t    Don't touch output signal %s\n", BehOut->NAME );
            }

            SetBoomBehAuxDontTouch( BehOut );
          }
          else
          if ( FigInfo->TRACE )
          {
            fprintf( stdout, "\t    Keep output signal %s\n", BehOut->NAME );
          }

          SetBoomBehAuxKeep( BehOut );

          if ( BoomBddSetExprKeepAux( BehOut->ABL ) == 0 )
          {
            SetBoomBehAuxDontTouch( BehOut );
          }
        }
      }
      else
      {
        BehOut->NODE = (void *)0;
      }
    }
  }

  resetauthtable( BoomBddHashAux     );
  resetauthtable( BoomBddHashKeepAux );

  reorderbddsystemwindow( (bddsystem *)0 );

  explosionbddsystemdynamic( (bddsystem *)0, NULL, 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                  Beh Delete Useless Aux                     |
|                                                             |
\------------------------------------------------------------*/

static void BoomBddReplaceKeepAuxAbl( Expr )

  ablexpr *Expr;
{
  authelem   *Element;
  char       *Value;

  if ( ABL_ATOM( Expr ) )
  {
    Value = ABL_ATOM_VALUE( Expr );

    if ( ( Value != ABL_ATOM_NAME_ONE      ) &&
         ( Value != ABL_ATOM_NAME_ZERO     ) &&
         ( Value != ABL_ATOM_NAME_DC       ) &&
         ( Value != ABL_ATOM_NAME_TRISTATE ) )
    {
      Element = searchauthelem( BoomBddHashRenameAux, Value );

      if ( Element != (authelem *)0 )
      {
        ABL_ATOM_VALUE( Expr ) = (char *)Element->VALUE;
      }
    }
  }
  else
  {
    while ( ( Expr = ABL_CDR( Expr ) ) != (chain_list *)0 )
    {
      BoomBddReplaceKeepAuxAbl( ABL_CAR( Expr ) );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                  Beh Delete Useless Keep Aux                |
|                                                             |
\------------------------------------------------------------*/

static void BoomBehDeleteDuplicateKeepAux( BehFigure, OrderFlag, DontCareFlag )

  befig_list *BehFigure;
  int         OrderFlag;
  int         DontCareFlag;
{
  beaux_list  *BehAux;
  beaux_list  *BehAuxDup;
  beaux_list **PrevAux;
  beaux_list  *BehAuxDel;
  authelem    *Element;
  int          Found;

  if ( BoomBddHashAux == (authtable *)0 )
  {
    BoomBddHashAux       = createauthtable( 100 );
    BoomBddHashKeepAux   = createauthtable( 100 );
    BoomBddHashRenameAux = createauthtable( 100 );
  }
/*
** Should identify duplicate/useless auxiliar variables
*/
  Found = 0;

  for ( BehAux  = BehFigure->BEAUX;
        BehAux != (beaux_list *)0;
        BehAux  = BehAux->NEXT )
  {
    if ( ( BehAux->NODE == NULL         ) ||
         ( ! IsBoomBehAuxKeep( BehAux ) ) ) continue;

    Element = searchauthelem( BoomBddHashAux, (char *)BehAux->NODE );

    if ( Element == (authelem *)0 )
    {
      addauthelem( BoomBddHashAux, (char *)BehAux->NODE, (long)BehAux );
    }
    else
    {
      BehAuxDup = (beaux_list *)Element->VALUE;
      addauthelem( BoomBddHashRenameAux, BehAux->NAME, (long)BehAuxDup->NAME );

      if ( IsBoomDebugLevel0() )
      {
        BoomPrintf( stdout, "Should replace %s by %s\n", BehAux->NAME, BehAuxDup->NAME );
      }

      SetBoomBehAuxTag( BehAux );

      Found = 1;
    }
  }

  if ( Found )
  {
    BoomBehScanAbl( BehFigure, BoomBddReplaceKeepAuxAbl, 1 );

    PrevAux = &BehFigure->BEAUX;
    BehAux  = BehFigure->BEAUX;

    while ( BehAux != (beaux_list *)0 )
    {
      if ( IsBoomBehAuxTag( BehAux ) )
      {
        *PrevAux  = BehAux->NEXT;
        BehAuxDel = BehAux;
        BehAux    = BehAux->NEXT;

        BehFigure->BERIN = beh_rmvberin( BehFigure->BERIN, BehAuxDel->NAME );

        BehAuxDel->NEXT = (beaux_list *)0;
        BoomBehFreeBehAux( BehAuxDel );
      }
      else
      {
        PrevAux = &BehAux->NEXT;
        BehAux  = BehAux->NEXT;
      }
    }

    BoomBehResetBddNode( BehFigure );

    resetbddcircuit( (bddcircuit *)0 );
    resetbddsystem( (bddsystem *)0 );

    if ( ! BoomDebugFlag )
    {
      SetBddSystemNoWarning( BddLocalSystem );
    }

    BoomBehMakeBddOrder( BehFigure, OrderFlag, DontCareFlag );

    BoomBehMakeBddNode( BehFigure, 0 );
  }

  resetauthtable( BoomBddHashAux       );
  resetauthtable( BoomBddHashRenameAux );
}

/*------------------------------------------------------------\
|                                                             |
|                         Beh Make Bdd                        |
|                                                             |
\------------------------------------------------------------*/

void BoomBehAbl2BddNode( BehFigure, OrderFlag, DontCareFlag )

  befig_list *BehFigure;
  int         OrderFlag;
  int         DontCareFlag;
{
  berin_list    *BehRin;
  beaux_list    *BehAux;
  beaux_list    *DelAux;
  beaux_list   **PrevAux;
  beout_list    *BehOut;
  bebus_list    *BehBus;

  long  NumberIn  = 1;
  long  NumberOut = 1;

  if ( BehFigure->BEDLY == (beaux_list *)0 )
  {
    for ( BehAux  = BehFigure->BEAUX;
          BehAux != (beaux_list *)0;
          BehAux  = BehAux->NEXT )
    {
      if ( ! IsBoomBehAuxKeep( BehAux ) )
      {
        BehFigure->BERIN = beh_rmvberin( BehFigure->BERIN, BehAux->NAME );
        NumberOut++;
      }
      else
      {
        NumberIn++;
      }
    }
  }
  else
  {
    BoomError( BOOM_SIMPLIFY_ERROR, (char *)0 );
  }

  for ( BehRin  = BehFigure->BERIN;
        BehRin != (berin_list *)0;
        BehRin  = BehRin->NEXT )
  {
    NumberIn++;
  }

  if ( DontCareFlag ) NumberIn++;

  for ( BehOut  = BehFigure->BEOUT;
        BehOut != (beout_list *)0;
        BehOut  = BehOut->NEXT )
  {
    NumberOut++;
  }

  for ( BehBus  = BehFigure->BEBUS;
        BehBus != (bebus_list *)0;
        BehBus  = BehBus->NEXT )
  {
    NumberOut++;
  }

  BehFigure->CIRCUI = 
 
   (void *)createbddcircuit( BehFigure->NAME, NumberIn, NumberOut, (bddsystem *)0 );

  BoomBehMakeBddOrder( BehFigure, OrderFlag, DontCareFlag );

  BoomBehMakeBddNodeNoExplosion( BehFigure, 0 );

  BoomBehDeleteDuplicateKeepAux( BehFigure, OrderFlag, DontCareFlag );

  PrevAux = &BehFigure->BEAUX;
  BehAux  = BehFigure->BEAUX;

  while ( BehAux != (beaux_list *)0 )
  {
    if ( ! IsBoomBehAuxKeep( BehAux ) )
    {
      decbddrefext( BehAux->NODE );
      delbddcircuitout( (bddcircuit *)0, BehAux->NAME );

      *PrevAux = BehAux->NEXT;
      DelAux   = BehAux;
      BehAux   = BehAux->NEXT;

      DelAux->NEXT = (beaux_list *)0;
      BoomBehFreeBehAux( DelAux );
    }
    else
    {
      PrevAux = &BehAux->NEXT;
      BehAux  = BehAux->NEXT;
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        BoomUnmarkBddNode                    |
|                                                             |
\------------------------------------------------------------*/

chain_list *BoomUnmarkBddNode( Expr )

  chain_list *Expr;
{
  chain_list *ScanExpr;
  bddnode    *BddNode;

  if ( ! ABL_ATOM( Expr ) )
  {
    BddNode = (bddnode *)ABL_CDR( ABL_CAR( Expr ) );

    if ( BddNode != (bddnode *)0 )
    {
      if ( BddNode->MARK > 0 )
      {
        BddNode->MARK = 0; decbddrefext( BddNode );
      }

      ABL_CDR( ABL_CAR( Expr ) ) = (chain_list *)0;
    }

    ScanExpr = Expr;

    while( ( ScanExpr = ABL_CDR( ScanExpr ) ) != (chain_list *)0 )
    {
      BoomUnmarkBddNode( ABL_CAR( ScanExpr ) );
    }
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomBehUnmarkBddNode                   |
|                                                             |
\------------------------------------------------------------*/

void BoomBehUnmarkBddNode( BehFigure )

  befig_list *BehFigure;
{
  BoomBehMapAbl( BehFigure, BoomUnmarkBddNode, 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                     Boom Beh Reset Bdd Node                 |
|                                                             |
\------------------------------------------------------------*/

void BoomBehResetBddNode( BehFigure )

  befig_list *BehFigure;
{
  bddcircuit  *BddCircuit;
  bddsystem   *BddSystem;
  beaux_list  *BehAux;
  beout_list  *BehOut;
  bebus_list  *BehBus;
  bebux_list  *BehBux;
  bereg_list  *BehReg;
  binode_list *BiNode;

  BddCircuit = BehFigure->CIRCUI;
  BddSystem  = BddCircuit->BDD_SYSTEM;

  setbddlocalcircuit( BddCircuit );
  setbddlocalsystem( BddSystem );

  for ( BehAux  = BehFigure->BEAUX;
        BehAux != (beaux_list *)0;
        BehAux  = BehAux->NEXT )
  {
    if ( BehAux->NODE != (void *)0 )
    {
      decbddrefext( BehAux->NODE );
      delbddcircuitout( (bddcircuit *)0, BehAux->NAME );
    }

    BehAux->NODE = (void *)0;
  }

  for ( BehOut  = BehFigure->BEOUT;
        BehOut != (beout_list *)0;
        BehOut  = BehOut->NEXT )
  {
    if ( BehOut->NODE != (void *)0 )
    {
      decbddrefext( BehOut->NODE );
      delbddcircuitout( (bddcircuit *)0, BehOut->NAME );
    }

    BehOut->NODE = (void *)0;
  }

  for ( BehBus  = BehFigure->BEBUS;
        BehBus != (bebus_list *)0;
        BehBus  = BehBus->NEXT )
  {
    delbddcircuitout( (bddcircuit *)0, BehBus->NAME );

    for ( BiNode  = BehBus->BINODE;
          BiNode != (binode_list *)0;
          BiNode  = BiNode->NEXT ) 
    {
      if ( BiNode->VALNODE != (void *)0 )
      {
        decbddrefext( BiNode->VALNODE );
        BiNode->VALNODE = (void *)0;
      }

      if ( BiNode->CNDNODE != (void *)0 )
      {
        decbddrefext( BiNode->CNDNODE );
        BiNode->CNDNODE = (void *)0;
      }
    }
  }

  for ( BehBux  = BehFigure->BEBUX;
        BehBux != (bebux_list *)0;
        BehBux  = BehBux->NEXT )
  {
    delbddcircuitout( (bddcircuit *)0, BehBux->NAME );

    for ( BiNode  = BehBux->BINODE;
          BiNode != (binode_list *)0;
          BiNode  = BiNode->NEXT ) 
    {
      if ( BiNode->VALNODE != (void *)0 )
      {
        decbddrefext( BiNode->VALNODE );
        BiNode->VALNODE = (void *)0;
      }

      if ( BiNode->CNDNODE != (void *)0 )
      {
        decbddrefext( BiNode->CNDNODE );
        BiNode->CNDNODE = (void *)0;
      }
    }
  }

  for ( BehReg  = BehFigure->BEREG;
        BehReg != (bereg_list *)0;
        BehReg  = BehReg->NEXT )
  {
    delbddcircuitout( (bddcircuit *)0, BehReg->NAME );

    for ( BiNode  = BehReg->BINODE;
          BiNode != (binode_list *)0;
          BiNode  = BiNode->NEXT ) 
    {
      if ( BiNode->VALNODE != (void *)0 )
      {
        decbddrefext( BiNode->VALNODE );
        BiNode->VALNODE = (void *)0;
      }

      if ( BiNode->CNDNODE != (void *)0 )
      {
        decbddrefext( BiNode->CNDNODE );
        BiNode->CNDNODE = (void *)0;
      }
    }
  }

  garbagebddsystem( (bddsystem *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomBehMapBdd                          |
|                                                             |
\------------------------------------------------------------*/

void BoomBehMapBdd( BehFigure, Function )

  befig_list *BehFigure;
  bddnode   *(*Function)();
{
  bddcircuit  *BddCircuit;
  bddsystem   *BddSystem;
  beaux_list  *BehAux;
  beout_list  *BehOut;
  bebus_list  *BehBus;
  bebux_list  *BehBux;
  bereg_list  *BehReg;
  binode_list *BiNode;

  BddCircuit = BehFigure->CIRCUI;
  BddSystem  = BddCircuit->BDD_SYSTEM;

  setbddlocalcircuit( BddCircuit );
  setbddlocalsystem( BddSystem );

  for ( BehAux  = BehFigure->BEAUX;
        BehAux != (beaux_list *)0;
        BehAux  = BehAux->NEXT )
  {
    if ( BehAux->NODE != NULL )
    {
      BehAux->NODE = Function( BehAux->NODE );
      addbddcircuitout( (bddcircuit *)0, BehAux->NAME, BehAux->NODE );
    }
  }

  for ( BehOut  = BehFigure->BEOUT;
        BehOut != (beout_list *)0;
        BehOut  = BehOut->NEXT )
  {
    if ( BehOut->NODE != NULL )
    {
      BehOut->NODE = Function( BehOut->NODE );
      addbddcircuitout( (bddcircuit *)0, BehOut->NAME, BehOut->NODE );
    }
  }

  for ( BehBus  = BehFigure->BEBUS;
        BehBus != (bebus_list *)0;
        BehBus  = BehBus->NEXT )
  {
    for ( BiNode  = BehBus->BINODE;
          BiNode != (binode_list *)0;
          BiNode  = BiNode->NEXT ) 
    {
      if ( BiNode->VALNODE != NULL )
      {
        BiNode->VALNODE = Function( BiNode->VALNODE );
        addbddcircuitout( (bddcircuit *)0, BehBus->NAME, BiNode->VALNODE );
      }

      if ( BiNode->CNDNODE != NULL )
      {
        BiNode->CNDNODE = Function( BiNode->CNDNODE );
      }
    }
  }

  for ( BehBux  = BehFigure->BEBUX;
        BehBux != (bebux_list *)0;
        BehBux  = BehBux->NEXT )
  {
    for ( BiNode  = BehBux->BINODE;
          BiNode != (binode_list *)0;
          BiNode  = BiNode->NEXT ) 
    {
      if ( BiNode->VALNODE != NULL )
      {
        BiNode->VALNODE = Function( BiNode->VALNODE );
        addbddcircuitout( (bddcircuit *)0, BehBux->NAME, BiNode->VALNODE );
      }

      if ( BiNode->CNDNODE != NULL )
      {
        BiNode->CNDNODE = Function( BiNode->CNDNODE );
      }
    }
  }

  for ( BehReg  = BehFigure->BEREG;
        BehReg != (bereg_list *)0;
        BehReg  = BehReg->NEXT )
  {
    for ( BiNode  = BehReg->BINODE;
          BiNode != (binode_list *)0;
          BiNode  = BiNode->NEXT ) 
    {
      if ( BiNode->VALNODE != NULL )
      {
        BiNode->VALNODE = Function( BiNode->VALNODE );
        addbddcircuitout( (bddcircuit *)0, BehReg->NAME, BiNode->VALNODE );
      }

      if ( BiNode->CNDNODE != NULL )
      {
        BiNode->CNDNODE = Function( BiNode->CNDNODE );
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomBehScanBdd                         |
|                                                             |
\------------------------------------------------------------*/

void BoomBehScanBdd( BehFigure, Function )

  befig_list *BehFigure;
  void       (*Function)();
{
  bddcircuit  *BddCircuit;
  bddsystem   *BddSystem;
  beaux_list  *BehAux;
  beout_list  *BehOut;
  bebus_list  *BehBus;
  bebux_list  *BehBux;
  bereg_list  *BehReg;
  binode_list *BiNode;

  BddCircuit = BehFigure->CIRCUI;
  BddSystem  = BddCircuit->BDD_SYSTEM;

  setbddlocalcircuit( BddCircuit );
  setbddlocalsystem( BddSystem );

  for ( BehAux  = BehFigure->BEAUX;
        BehAux != (beaux_list *)0;
        BehAux  = BehAux->NEXT )
  {
    if ( BehAux->NODE != NULL ) Function( BehAux->NODE );
  }

  for ( BehOut  = BehFigure->BEOUT;
        BehOut != (beout_list *)0;
        BehOut  = BehOut->NEXT )
  {
    if ( BehOut->NODE != NULL ) Function( BehOut->NODE );
  }

  for ( BehBus  = BehFigure->BEBUS;
        BehBus != (bebus_list *)0;
        BehBus  = BehBus->NEXT )
  {
    for ( BiNode  = BehBus->BINODE;
          BiNode != (binode_list *)0;
          BiNode  = BiNode->NEXT ) 
    {
      if ( BiNode->VALNODE != NULL ) Function( BiNode->VALNODE );
      if ( BiNode->CNDNODE != NULL ) Function( BiNode->CNDNODE );
    }
  }

  for ( BehBux  = BehFigure->BEBUX;
        BehBux != (bebux_list *)0;
        BehBux  = BehBux->NEXT )
  {
    for ( BiNode  = BehBux->BINODE;
          BiNode != (binode_list *)0;
          BiNode  = BiNode->NEXT ) 
    {
      if ( BiNode->VALNODE != NULL ) Function( BiNode->VALNODE );
      if ( BiNode->CNDNODE != NULL ) Function( BiNode->CNDNODE );
    }
  }

  for ( BehReg  = BehFigure->BEREG;
        BehReg != (bereg_list *)0;
        BehReg  = BehReg->NEXT )
  {
    for ( BiNode  = BehReg->BINODE;
          BiNode != (binode_list *)0;
          BiNode  = BiNode->NEXT ) 
    {
      if ( BiNode->VALNODE != NULL ) Function( BiNode->VALNODE );
      if ( BiNode->CNDNODE != NULL ) Function( BiNode->CNDNODE );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        BoomGetNumberBddNode                 |
|                                                             |
\------------------------------------------------------------*/

static void BoomGetNumberBigBddNode( BddNode )

  bddnode *BddNode;
{
  long Number;

  Number = getbddnodenum( (bddsystem *)0, BddNode );

  if ( Number >= BOOM_BDD_NODE_BIG_SIZE ) BoomBddNumberNode += Number;
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomBehGetNumberBddNode                |
|                                                             |
\------------------------------------------------------------*/

long BoomBehGetNumberBigBddNode( BehFigure )

  befig_list *BehFigure;
{
  BoomBddNumberNode = 0;

  BoomBehScanBdd( BehFigure, BoomGetNumberBigBddNode );

  return( BoomBddNumberNode );
}

/*------------------------------------------------------------\
|                                                             |
|                       BoomBehGetBddOrder                    |
|                                                             |
\------------------------------------------------------------*/

chain_list *BoomBehGetBddOrder( BehFigure )

  befig_list *BehFigure;
{
  chain_list *OrderList;
  long        BddVar;
  long        NumberVar;
  long        BddIndex;
  char       *Name;

  NumberVar = BddLocalSystem->NUMBER_VAR;
  OrderList = (chain_list *)0;

  for ( BddVar = NumberVar - 1; BddVar >= 0; BddVar-- )
  {
    BddIndex = getbddvarindex( (bddsystem *)0, BddVar );
    Name     = getbddcircuitinname( (bddcircuit *)0, BddIndex );

    OrderList = addchain( OrderList, (void *)Name );
  }

  return( OrderList );
}

/*------------------------------------------------------------\
|                                                             |
|                     BoomBehRebuildBddNode                   |
|                                                             |
\------------------------------------------------------------*/

void BoomBehRebuildBddNode( BehFigure, OrderList )

  befig_list *BehFigure;
  chain_list *OrderList;
{
  chain_list *ScanList;

  BoomBehResetBddNode( BehFigure );

  resetbddcircuit( (bddcircuit *)0 );
  resetbddsystem( (bddsystem *)0 );

  if ( ! BoomDebugFlag )
  {
    SetBddSystemNoWarning( BddLocalSystem );
  }

  for ( ScanList  = OrderList; 
        ScanList != (chain_list *)0;
        ScanList  = ScanList->NEXT )
  {
    addbddcircuitin( (bddcircuit *)0, (char *)ScanList->DATA, 0, BDD_IN_MODE_LAST );
  }

  BoomBehMakeBddNode( BehFigure, 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                     BoomBehRandomBddOrder                   |
|                                                             |
\------------------------------------------------------------*/

void BoomBehRandomBddOrder( BehFigure, Amplitude )

  befig_list *BehFigure;
  long        Amplitude;
{
  boombefiginfo *FigInfo;
  long           NumberVar;
  long           MaxNode;
  long           BddVar;
  long           BddSwapVar;
  unsigned long  MoveTo;
  unsigned long  MoveIndex;

  static unsigned char InitRandom = 1;

  FigInfo = GetBoomBehFigInfo( BehFigure );

  if ( InitRandom )
  {
    srand( getpid() );
    InitRandom = 0;
  }

  if ( Amplitude <= 0 ) return;

  NumberVar = BddLocalSystem->NUMBER_VAR;

  for ( BddVar = 0; BddVar < NumberVar; BddVar++ )
  {
    MoveTo = (rand() >> 8) % Amplitude;

    if ( IsBoomDebugLevel2() )
    {
      BoomPrintf( stdout, "Move %ld amplitude %ld\n", BddVar, MoveTo );
    }

    MaxNode = BddLocalSystem->NUMBER_NODE + BOOM_BDD_SWAP_LIMIT;

    for ( MoveIndex = 0; MoveIndex < MoveTo; MoveIndex++ )
    {
      BddSwapVar = BddVar - MoveIndex;

      if ( ( BddSwapVar >= ( NumberVar - 1 ) ) ||
           ( BddSwapVar <= 0                 ) ) break;

      if ( IsBoomDebugLevel2() )
      {
        BoomPrintf( stdout, "> Swap %ld with %ld\n", BddSwapVar, BddSwapVar - 1 );
      }

      swapbddvar( (bddsystem *)0, BddSwapVar );

      if ( BddLocalSystem->NUMBER_NODE > MaxNode )
      {
        garbagebddsystem( (bddsystem *)0 );
      }

      if ( BddLocalSystem->NUMBER_NODE > MaxNode )
      {
        break;
      }
    }
  }
}
