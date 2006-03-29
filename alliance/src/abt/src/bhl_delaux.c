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
| File    :                 bhl_delaux.c                      |
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
# include "abe.h"
# include "abt.h"

# include <stdio.h>
# include <string.h>
# include "bhl_error.h"
# include "bhl_delaux.h"

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
  static authtable  *BhlHashTableInput  = (authtable  *)0;
  static authtable  *BhlHashTableAux    = (authtable  *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Bhl DelAux Expr                      |
|                                                             |
\------------------------------------------------------------*/

static chain_list *bhl_delauxexpr( Expr )

  chain_list *Expr;
{
  char       *AtomValue;
  chain_list *FirstExpr;
  beaux_list *ScanAux;
  chain_list *ScanChain;
  authelem   *Element;
  long        Oper;

  FirstExpr = Expr;

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
      return( FirstExpr );
    }

    Element = searchauthelem( BhlHashTableInput, AtomValue );

    if ( Element != (authelem *)0 )
    {
      return( FirstExpr );
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

    Element = searchauthelem( BhlHashTableAux, AtomValue );

    if ( Element == (authelem *)0 )
    {
      BhlHeadName  = addchain( BhlHeadName, AtomValue );
      ScanAux->ABL = bhl_delauxexpr( ScanAux->ABL );
      BhlHeadName  = delchain( BhlHeadName, BhlHeadName );

      addauthelem( BhlHashTableAux, AtomValue, (long)ScanAux->ABL );
    }

    freeablexpr( FirstExpr );
    return( dupablexpr( ScanAux->ABL ) );
  }

  Oper  = ABL_OPER( Expr );
  Expr  = ABL_CDR( Expr );

  if ( Oper == ABL_NOT )
  {
    if ( ABL_CDR( Expr ) != (chain_list *)0 )
    {
      bhlerror( BHL_OPERATOR_ERROR, Oper );
    }

    ABL_CAR_L( Expr ) = bhl_delauxexpr( ABL_CAR( Expr ) );

    return( FirstExpr );
  }

  if ( Oper == ABL_STABLE )
  {
    Expr = ABL_CAR( Expr );

    if ( ! ABL_ATOM( Expr ) )
    {
      bhlerror( BHL_ILLEGAL_STABLE_ERROR, (char *)0 );
    }

    AtomValue = ABL_ATOM_VALUE( Expr );
    Element   = searchauthelem( BhlHashTableInput, AtomValue );

    if ( Element == (authelem *)0 )
    {
      bhlerror( BHL_UNKNOWN_ATOM_ERROR, AtomValue );
    }

    BhlDelayedName = autresizeblock( BhlDelayedName, 0, strlen( AtomValue ) + 9 );
    sprintf( BhlDelayedName, "%s'delayed", AtomValue );
    AtomValue = namealloc( BhlDelayedName );

    Element = searchauthelem( BhlHashTableInput, AtomValue );

    if ( Element == (authelem *)0 )
    {
      bhlerror( BHL_UNKNOWN_ATOM_ERROR, AtomValue );
    }

    return( FirstExpr );
  }

  if ( ( isablunaryoper( Oper )             ) ||
       ( ABL_CDR( Expr ) == (chain_list *)0 ) )
  {
    bhlerror( BHL_OPERATOR_ERROR, Oper );
  }

  ABL_CAR_L( Expr ) = bhl_delauxexpr( ABL_CAR( Expr ) );

  while ( ( Expr = ABL_CDR( Expr ) ) != (chain_list *)0 )
  {
    ABL_CAR_L( Expr ) = bhl_delauxexpr( ABL_CAR( Expr ) );
  }

  return( FirstExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                        Beh DelAux Bdd                       |
|                                                             |
\------------------------------------------------------------*/

static chain_list *bhl_delauxabl( Name, Expr )

  char       *Name;
  chain_list *Expr;
{
  if ( Name != (char *)0 )
  {
    BhlHeadName = addchain( BhlHeadName, Name );
  }

  Expr = bhl_delauxexpr( Expr );

  if ( Name != (char *)0 )
  {
    BhlHeadName = delchain( BhlHeadName, BhlHeadName );
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                      Beh Del Aux Abl                        |
|                                                             |
\------------------------------------------------------------*/

void beh_delauxabl( BehFigure )

  befig_list *BehFigure;
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
  long           NumberIn;
  long           NumberAux;

  BhlBehFigure = BehFigure;
  BhlHeadName  = (chain_list *)0;
  NumberIn     = 1;
  NumberAux    = 1;

  BehAux = BehFigure->BEAUX;

  while ( BehAux != NULL )
  {
    BehFigure->BERIN = beh_rmvberin( BehFigure->BERIN, BehAux->NAME );

    NumberAux = NumberAux + 1;
    BehAux    = BehAux->NEXT;
  }

  BehRin = BehFigure->BERIN;

  while ( BehRin != NULL )
  {
    NumberIn = NumberIn + 1;
    BehRin   = BehRin->NEXT;
  }

  BhlHashTableInput = createauthtable( NumberIn  << 1 );
  BhlHashTableAux   = createauthtable( NumberAux << 1 );

  BehRin = BehFigure->BERIN;

  while ( BehRin != NULL )
  {
    addauthelem( BhlHashTableInput, BehRin->NAME, 0 );

    BehRin = BehRin->NEXT;
  }

  BehAux = BehFigure->BEAUX;

  while ( BehAux != NULL )
  {
    Element = searchauthelem( BhlHashTableAux, BehAux->NAME );

    if ( Element == (authelem *)0 )
    {
      BehAux->ABL = bhl_delauxabl( BehAux->NAME, BehAux->ABL );
      addauthelem( BhlHashTableAux, BehAux->NAME, (long)BehAux->ABL );
    }

    BehAux = BehAux->NEXT;
  }

  BehDly = BehFigure->BEDLY;

  while ( BehDly != NULL )
  {
    BehDly->ABL = bhl_delauxabl( BehDly->NAME, BehDly->ABL );

    BehDly = BehDly->NEXT;
  }

  BehMsg = BehFigure->BEMSG;

  while ( BehMsg != NULL )
  {
    BehMsg->ABL = bhl_delauxabl( (char *)0, BehMsg->ABL );

    BehMsg = BehMsg->NEXT;
  }

  BehOut = BehFigure->BEOUT;

  while ( BehOut != NULL )
  {
    BehOut->ABL = bhl_delauxabl( BehOut->NAME, BehOut->ABL );

    BehOut = BehOut->NEXT;
  }

  BehBus = BehFigure->BEBUS;

  while ( BehBus != NULL )
  {
    BiAbl  = BehBus->BIABL;

    while ( BiAbl != NULL )
    {
      BiAbl->CNDABL = bhl_delauxabl( (char *)0   , BiAbl->CNDABL );
      BiAbl->VALABL = bhl_delauxabl( BehBus->NAME, BiAbl->VALABL );

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
      BiAbl->CNDABL = bhl_delauxabl( (char *)0, BiAbl->CNDABL );
      BiAbl->VALABL = bhl_delauxabl( (char *)0, BiAbl->VALABL );

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
      BiAbl->CNDABL = bhl_delauxabl( (char *)0, BiAbl->CNDABL );
      BiAbl->VALABL = bhl_delauxabl( (char *)0, BiAbl->VALABL );

      BiAbl  = BiAbl->NEXT;
    }

    BehReg = BehReg->NEXT;
  }

  destroyauthtable( BhlHashTableInput );
  destroyauthtable( BhlHashTableAux   );

  beh_frebeaux( BehFigure->BEAUX );
  BehFigure->BEAUX = (beaux_list *)NULL;
}
