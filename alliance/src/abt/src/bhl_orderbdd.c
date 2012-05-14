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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     Bhl                           |
|                                                             |
| File    :                 bhl_orderbdd.c                    |
|                                                             |
| Date    :                   01.02.95                        |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
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
# include <string.h>
# include "bhl_error.h"
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

  static chain_list *BhlHeadName        = (chain_list *)0;
  static befig_list *BhlBehFigure       = (befig_list *)0;
  static char       *BhlDelayedName     = (char       *)0;
  static authtable  *BhlHashTableOrder  = (authtable  *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Bhl Order Expr Bdd                   |
|                                                             |
\------------------------------------------------------------*/

static void bhl_orderexprbdd( Expr )

  chain_list *Expr;
{
  char       *AtomValue;
  beaux_list *ScanAux;
  chain_list *ScanChain;
  authelem   *Element;
  long        Oper;

  if ( Expr == (chain_list *)0 )
  {
    bhlerror( BHL_EXPR_NULL_ERROR, (char *)0 );
  }

  if ( ABL_ATOM( Expr ) )
  {
    AtomValue = ABL_ATOM_VALUE( Expr );

    if ( ( AtomValue == ABL_ATOM_NAME_ONE  ) ||
         ( AtomValue == ABL_ATOM_NAME_ZERO ) ||
         ( AtomValue == ABL_ATOM_NAME_DC   ) )
    {
      return;
    }

    Element = searchauthelem( BhlHashTableOrder, AtomValue );

    if ( Element != (authelem *)0 )
    {
      Element->VALUE++;

      return;
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

    BhlHeadName   = addchain( BhlHeadName, AtomValue );
    bhl_orderexprbdd( ScanAux->ABL );
    BhlHeadName   = delchain( BhlHeadName, BhlHeadName );

    return;
  }

  Oper = ABL_OPER( Expr );
  Expr = ABL_CDR( Expr );

  if ( Oper == ABL_NOT )
  {
    if ( ABL_CDR( Expr ) != (chain_list *)0 )
    {
      bhlerror( BHL_OPERATOR_ERROR, Oper );
    }

    bhl_orderexprbdd( ABL_CAR( Expr ) );

    return;
  }

  if ( Oper == ABL_STABLE )
  {
    Expr = ABL_CAR( Expr );

    if ( ! ABL_ATOM( Expr ) )
    {
      bhlerror( BHL_ILLEGAL_STABLE_ERROR, (char *)0 );
    }

    AtomValue = ABL_ATOM_VALUE( Expr );
    Element   = searchauthelem( BhlHashTableOrder, AtomValue );

    if ( Element == (authelem *)0 )
    {
      bhlerror( BHL_UNKNOWN_ATOM_ERROR, AtomValue );
    }

    BhlDelayedName = autresizeblock( BhlDelayedName, 0, strlen( AtomValue ) + 9 );
    sprintf( BhlDelayedName, "%s'delayed", AtomValue );
    AtomValue = namealloc( BhlDelayedName );

    Element = searchauthelem( BhlHashTableOrder, AtomValue );

    if ( Element == (authelem *)0 )
    {
      bhlerror( BHL_UNKNOWN_ATOM_ERROR, AtomValue );
    }

    return;
  }

  if ( ( isablunaryoper( Oper )             ) ||
       ( ABL_CDR( Expr ) == (chain_list *)0 ) )
  {
    bhlerror( BHL_OPERATOR_ERROR, Oper );
  }

  bhl_orderexprbdd( ABL_CAR( Expr ) );

  while ( ( Expr = ABL_CDR( Expr ) ) != (chain_list *)0 )
  {
    bhl_orderexprbdd( ABL_CAR( Expr ) );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Beh Order Bdd                        |
|                                                             |
\------------------------------------------------------------*/

static void bhl_orderablbdd( Name, Expr )

  char       *Name;
  chain_list *Expr;
{
  if ( Name != (char *)0 )
  {
    BhlHeadName = addchain( BhlHeadName, Name );
  }

  bhl_orderexprbdd( Expr );

  if ( Name != (char *)0 )
  {
    BhlHeadName = delchain( BhlHeadName, BhlHeadName );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       Beh Sort Compare                      |
|                                                             |
\------------------------------------------------------------*/

static long bhl_sortcompare( ValueArray, Index1, Index2 )

  authelem *ValueArray;
  long      Index1;
  long      Index2;
{
  return( ValueArray[ Index1 ].VALUE - 
          ValueArray[ Index2 ].VALUE );
}

/*------------------------------------------------------------\
|                                                             |
|                       Beh Order Bdd Total                   |
|                                                             |
\------------------------------------------------------------*/

void bhl_orderbdd( BehFigure, AuxFlag, OrderFlag )

  befig_list *BehFigure;
  long        AuxFlag;
  long        OrderFlag;
{
  struct berin  *BehRin;
  struct beaux  *BehAux;
  struct beaux  *BehDly;
  struct bemsg  *BehMsg;
  struct beout  *BehOut;
  struct bebus  *BehBus;
  struct bebux  *BehBux;
  struct bereg  *BehReg;
  struct biabl  *BiAbl;
  authelem      *Element;
  authelem      *ValueArray;
  long          *IndexArray;
  long           SizeArray;
  long           Index;
  long           NumberIn;
  bddindex       BddIndex;

  BhlBehFigure = BehFigure;
  BhlHeadName  = (chain_list *)0;
  NumberIn     = BehFigure->CIRCUI->NAME_IN_SIZE;

  BehFigure->BERIN = (berin_list *)reverse( (chain_list*)BehFigure->BERIN );
  BehRin = BehFigure->BERIN;

  if ( ! OrderFlag )
  {
    while ( BehRin != NULL )
    {
      BddIndex = BddLocalCircuit->NUMBER_NAME_IN + BDD_INDEX_MIN;

      addbddcircuitin( (bddcircuit *)0, BehRin->NAME, BddIndex,
                       BDD_IN_MODE_IMPOSE | BDD_IN_MODE_LAST );

      BehRin = BehRin->NEXT;
    }

    BehFigure->BERIN = (berin_list *)reverse( (chain_list*)BehFigure->BERIN );

    return;
  }

  BhlHashTableOrder = createauthtable( NumberIn << 1 );

  while ( BehRin != NULL )
  {
    addauthelem( BhlHashTableOrder, BehRin->NAME, 0 );

    BehRin = BehRin->NEXT;
  }

  if ( AuxFlag )
  {
    BehAux = BehFigure->BEAUX;

    while ( BehAux != NULL )
    {
      bhl_orderablbdd( BehAux->NAME, BehAux->ABL );

      BehAux = BehAux->NEXT;
    }
  }

  BehDly = BehFigure->BEDLY;

  while ( BehDly != NULL )
  {
    bhl_orderablbdd( BehDly->NAME, BehDly->ABL );

    BehDly = BehDly->NEXT;
  }

  BehMsg = BehFigure->BEMSG;

  while ( BehMsg != NULL )
  {
    bhl_orderablbdd( (char *)0, BehMsg->ABL );

    BehMsg = BehMsg->NEXT;
  }

  BehOut = BehFigure->BEOUT;

  while ( BehOut != NULL )
  {
    bhl_orderablbdd( BehOut->NAME, BehOut->ABL );

    BehOut = BehOut->NEXT;
  }

  BehBus = BehFigure->BEBUS;

  while ( BehBus != NULL )
  {
    BiAbl  = BehBus->BIABL;

    while ( BiAbl != NULL )
    {
      bhl_orderablbdd( (char *)0   , BiAbl->CNDABL );
      bhl_orderablbdd( BehBus->NAME, BiAbl->VALABL );

      BiAbl = BiAbl->NEXT;
    }

    BehBus = BehBus->NEXT;
  }

  BehBux = BehFigure->BEBUX;

  while ( BehBux != NULL )
  {
    BiAbl  = BehBux->BIABL;

    while (BiAbl != NULL)
    {
      bhl_orderablbdd( (char *)0, BiAbl->CNDABL );
      bhl_orderablbdd( (char *)0, BiAbl->VALABL );

      BiAbl  = BiAbl->NEXT;
    }

    BehBux = BehBux->NEXT;
  }

  BehReg = BehFigure->BEREG;

  while ( BehReg != NULL )
  {
    BiAbl  = BehReg->BIABL;

    while (BiAbl != NULL)
    {
      bhl_orderablbdd( (char *)0, BiAbl->CNDABL );
      bhl_orderablbdd( (char *)0, BiAbl->VALABL );

      BiAbl  = BiAbl->NEXT;
    }

    BehReg = BehReg->NEXT;
  }

  BehRin = BehFigure->BERIN;

# ifdef BHL_ORDER_DEBUG
  viewauthtable( BhlHashTableOrder, viewauthelem );
# endif

  SizeArray  = BhlHashTableOrder->TABLE_SIZE;
  ValueArray = BhlHashTableOrder->TABLE;
  IndexArray = (long *)autallocblock( sizeof( long ) * SizeArray );

  sortautarray( ValueArray, IndexArray, SizeArray, bhl_sortcompare );

  for ( Index = 0; Index < SizeArray; Index++ )
  {
    Element = &ValueArray[ IndexArray[ Index ] ];

    if ( Element->KEY != AUT_HASH_KEY_EMPTY )
    {
      BddIndex = BddLocalCircuit->NUMBER_NAME_IN + BDD_INDEX_MIN;

      addbddcircuitin( (bddcircuit *)0, Element->KEY, BddIndex,
                       BDD_IN_MODE_IMPOSE | BDD_IN_MODE_LAST );

# ifdef BHL_ORDER_DEBUG
      fprintf( stdout, "Var: %s Order: %d\n", 
               Element->KEY, Element->VALUE );
# endif
    }
  }

  autfreeblock( IndexArray );
  destroyauthtable( BhlHashTableOrder );

  BehFigure->BERIN = (berin_list *)reverse( (chain_list*)BehFigure->BERIN );
}
