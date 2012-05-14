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
| Tool    :                     BOOM                          |
|                                                             |
| File    :                   boom_aux.c                      |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   06.06.00                        |
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
# include "abv.h"
# include "abt.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "boom_error.h"
# include "boom_debug.h"
# include "boom_shared.h"
# include "boom_bdd.h"
# include "boom_abl.h"
# include "boom_cost.h"
# include "boom_aux.h"

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

  static befig_list *BoomBehFigure   = (befig_list *)0;
  static char       *BoomCurrentName = (char       *)0;
  static long        BoomAuxNumber   = 0;

  static char        BoomAuxName[ 128 ];
  static authtable  *BoomAuxHashTable     = (authtable *)0;
  static authtable  *BoomNotAuxHashTable  = (authtable *)0;
  static authtable  *BoomAuxNameHashTable = (authtable *)0;

  static authtable  *BoomUselessAuxHashTable  = (authtable *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern bddnode *loc_applybddnot();

/*------------------------------------------------------------\
|                                                             |
|                      BoomGetAblDepth                        |
|                                                             |
\------------------------------------------------------------*/

static long BoomGetAblDepth( Expr )

  ablexpr *Expr;
{
  long  NumberOper;
  long  MaxDepth;
  long  CarDepth;

  if ( ABL_ATOM( Expr ) )
  {
    return( 0 );
  }

  /*
  if ( ( ABL_ATOM( Expr )                 ) ||
       ( ( ABL_OPER( Expr ) == ABL_NOT  ) &&
         ( ABL_ATOM( ABL_CADR( Expr ) ) ) ) )
  {
    return( 0 );
  }
  */

  MaxDepth   = 0;
  NumberOper = 0;

  while ( ( Expr = ABL_CDR( Expr ) ) != (chain_list *)0 )
  {
    CarDepth = BoomGetAblDepth( ABL_CAR( Expr ) );

    if ( CarDepth > MaxDepth ) MaxDepth = CarDepth;

    /* NumberOper++; */
  }

  NumberOper++;

  if ( NumberOper > 1 ) NumberOper--;

  return( MaxDepth + NumberOper );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomCheckIfGoodAuxAbl                  |
|                                                             |
\------------------------------------------------------------*/

static int BoomCheckIfGoodAuxAbl( Expr )

  ablexpr *Expr;
{
  if ( ( ABL_ATOM( Expr )                 ) ||
       ( ( ABL_OPER( Expr ) == ABL_NOT  ) &&
         ( ABL_ATOM( ABL_CADR( Expr ) ) ) ) )
  {
    return( 0 );
  }

  return( 1 );
  /*
  long MaxDepth;

  MaxDepth = BoomGetAblDepth( Expr );

  return ( MaxDepth >= BOOM_AUX_MIN_DEPTH );
  */
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomCreateAuxAbl                       |
|                                                             |
\------------------------------------------------------------*/

static ablexpr *BoomCreateAuxAbl( Expr )

  ablexpr *Expr;
{
  bddnode    *BddNode;
  bddnode    *BddNot;
  chain_list *ScanExpr;
  authelem   *Element;
  char       *Name;
  long        Polar;

  if ( ! ABL_ATOM( Expr ) )
  {
    ScanExpr = Expr;

    while ( ( ScanExpr = ABL_CDR( ScanExpr ) ) != (chain_list *)0 )
    {
      ABL_CAR_L( ScanExpr ) = BoomCreateAuxAbl( ABL_CAR( ScanExpr ) );
    }

    BddNode = BoomGetBddMarkAbl( Expr );

    if ( BddNode == NULL )
    {
      return( Expr );
    }

    BddNot = decbddrefext( loc_applybddnot( BddNode ) );

    if ( IsBoomDebugLevel2() )
    {
      viewablexprln( ScanExpr, ABL_VIEW_VHDL );

      fprintf( stdout, "BddNode->MARK %d\n", BddNode->MARK );
      fprintf( stdout, "BddNot->MARK  %d\n", BddNot->MARK );
    }

    Polar   = 1;
    Element = searchauthelem( BoomAuxHashTable, (char *)BddNode );

    if ( Element == (authelem *)0 )
    {
      Element = searchauthelem( BoomNotAuxHashTable, (char *)BddNode );
      Polar   = 0;
    }

    if ( Element == (authelem *)0 )
    {
      if ( ( (BddNode->MARK + BddNot->MARK ) < 2 ) ||
           ( ! BoomCheckIfGoodAuxAbl( Expr )     ) )
      {
        return( Expr );
      }

      sprintf( BoomAuxName, "aux%ld", BoomAuxNumber++ );

      Name = namealloc( BoomAuxName );

      if ( BddNot->MARK < BddNode->MARK )
      {
        addauthelem( BoomAuxHashTable   , (char *)BddNode, (long)Name );
        addauthelem( BoomNotAuxHashTable, (char *)BddNot , (long)Name );
      }
      else
      {
        addauthelem( BoomAuxHashTable   , (char *)BddNot , (long)Name );
        addauthelem( BoomNotAuxHashTable, (char *)BddNode, (long)Name );

        BoomUnmarkBddNode( Expr );
        Expr = createablnotexpr( Expr );
      }

      BoomBehFigure->BEAUX = beh_addbeaux( BoomBehFigure->BEAUX, Name, Expr, (void *)0 );
      BoomAddBehAuxInfo( BoomBehFigure->BEAUX );

      if ( IsBoomDebugLevel0() )
      {
        BoomPrintf( stdout, "Add auxiliary signal %s P %d N %d\n",
                    Name, BddNode->MARK, BddNot->MARK );
        viewablexprln( Expr, ABL_VIEW_VHDL );
      }

      if ( BddNot->MARK < BddNode->MARK )
      {
        Expr = createablatom( Name );
      }
      else
      {
        Expr = createablnotexpr( createablatom( Name ) );
      }
    }
    else
    {
      Name = (char *)Element->VALUE;
      
      if ( Name != BoomCurrentName )
      {
        /* BIG BUG !! */
        Element = searchauthelem( BoomAuxNameHashTable, Name );

        if ( Element == (authelem *)0 )
        {
          BoomUnmarkBddNode( Expr );
          freeablexpr( Expr );

          Expr = createablatom( Name );

          if ( ! Polar ) Expr = createablnotexpr( Expr );
        }
      }

      return( Expr );
    }
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomCreateAuxAblWithName               |
|                                                             |
\------------------------------------------------------------*/

static ablexpr *BoomCreateAuxAblWithName( Name, Expr )

  char    *Name;
  ablexpr *Expr; 
{
  BoomCurrentName = Name;

  Expr = BoomCreateAuxAbl( Expr );

  return ( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomBehInitializeAux                   |
|                                                             |
\------------------------------------------------------------*/

void BoomBehInitializeAux( BehFigure )

  befig_list *BehFigure;
{
  beaux_list *BehAux;
  bddnode    *BddNode;
  bddnode    *BddNot;

  for ( BehAux  = BehFigure->BEAUX;
        BehAux != (beaux_list *)0;
        BehAux  = BehAux->NEXT )
  {
    if ( IsBoomBehAuxDontTouch( BehAux ) ) continue;

    BddNode = BehAux->NODE;
    BddNot  = decbddrefext( loc_applybddnot( BddNode ) );

    addauthelem( BoomAuxHashTable    , (char *)BddNode, (long)BehAux->NAME );
    addauthelem( BoomNotAuxHashTable , (char *)BddNot , (long)BehAux->NAME );
    addauthelem( BoomAuxNameHashTable, BehAux->NAME   , 0                  );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomCountAuxOccurAbl                   |
|                                                             |
\------------------------------------------------------------*/

static void BoomCountAuxOccurAbl( Expr )

  ablexpr *Expr;
{
  ablexpr  *ScanExpr;
  authelem *Element;
  char     *Value;

  if ( ABL_ATOM( Expr ) )
  {
    Value   = ABL_ATOM_VALUE( Expr );
    Element = searchauthelem( BoomUselessAuxHashTable, Value );

    if ( Element != (authelem *)0 )
    {
      Element->VALUE++;
    }
  }
  else
  {
    ScanExpr = Expr;

    while ( ( ScanExpr = ABL_CDR( ScanExpr ) ) != (chain_list *)0 )
    {
      BoomCountAuxOccurAbl( ABL_CAR( ScanExpr ) );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomReplaceUselessAuxAbl               |
|                                                             |
\------------------------------------------------------------*/

static ablexpr *BoomReplaceUselessAuxAbl( Expr )

  ablexpr *Expr;
{
  ablexpr  *ScanExpr;
  authelem *Element;
  char     *Value;

  if ( ABL_ATOM( Expr ) )
  {
    Value   = ABL_ATOM_VALUE( Expr );
    Element = searchauthelem( BoomUselessAuxHashTable, Value );

    if ( Element != (authelem *)0 )
    {
      if ( IsBoomDebugLevel1() )
      {
        BoomPrintf( stdout, "Replace %s by ", Value );
        viewablexprln( (ablexpr *)Element->VALUE, ABL_VIEW_VHDL );
      }

      freeablexpr( Expr ); 
      Expr = BoomReplaceUselessAuxAbl( dupablexpr( (ablexpr *)Element->VALUE ) );

      if ( IsBoomDebugLevel1() )
      {
        viewablexprln( Expr, ABL_VIEW_VHDL );
      }
    }
  }
  else
  {
    ScanExpr = Expr;

    while ( ( ScanExpr = ABL_CDR( ScanExpr ) ) != (chain_list *)0 )
    {
      ABL_CAR_L( ScanExpr ) = BoomReplaceUselessAuxAbl( ABL_CAR( ScanExpr ) );
    }
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomBehDeleteUselessAux                |
|                                                             |
\------------------------------------------------------------*/

void BoomBehDeleteUselessAux( BehFigure, InitialAux, RemoveBerin )

  befig_list *BehFigure;
  beaux_list *InitialAux;
{
  boombeauxinfo *AuxInfo;
  beaux_list    *BehAux;
  beaux_list    *DelBehAux;
  beaux_list    *HeadBehAux;
  beaux_list   **PrevBehAux;
  authelem      *Element;  

  if ( BoomUselessAuxHashTable == (authtable *)0 )
  {
    BoomUselessAuxHashTable = createauthtable( 10000 );
  }

  for ( BehAux  = BehFigure->BEAUX;
        BehAux != (beaux_list *)0;
        BehAux  = BehAux->NEXT )
  {
    if ( BehAux == InitialAux ) break;

    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "Add '%s'\n", BehAux->NAME );
    }

    addauthelem( BoomUselessAuxHashTable, BehAux->NAME, 0 );
  }

  if ( IsBoomDebugLevel1() )
  {
    BoomPrintf( stdout, "Count Occurences\n" );
  }

  BoomBehScanAbl( BehFigure, BoomCountAuxOccurAbl, 0 );

  BoomBehGiveCost( BehFigure );

  HeadBehAux = (beaux_list *)0;
  PrevBehAux = &BehFigure->BEAUX;
  BehAux     = BehFigure->BEAUX;

  while ( BehAux != (beaux_list *)0 )
  {
    if ( BehAux == InitialAux ) break;

    AuxInfo = GetBoomBehAuxInfo( BehAux );
    Element = searchauthelem( BoomUselessAuxHashTable, BehAux->NAME );

    if ( ( ! IsBoomBehAuxKeep( BehAux                              ) ) &&
         ( ( Element->VALUE <= 1                                   ) ||
           ( AuxInfo->LITERAL_COST   <= 1                          ) ||
           ( ( AuxInfo->LITERAL_COST < BOOM_AUX_MIN_LITERAL_COST ) &&
             ( Element->VALUE        < BOOM_AUX_MIN_OCCUR        ) ) ) )
    {
      if ( IsBoomDebugLevel0() )
      {
        BoomPrintf( stdout, "Should delete %s %ld %ld\n",
                    BehAux->NAME, Element->VALUE, AuxInfo->LITERAL_COST );
      }

      Element->VALUE = (long)BehAux->ABL;

      DelBehAux   = BehAux;
      BehAux      = BehAux->NEXT;
      *PrevBehAux = BehAux;

      DelBehAux->NEXT = HeadBehAux;
      HeadBehAux      = DelBehAux;

      if ( RemoveBerin )
      {
        BehFigure->BERIN = beh_rmvberin( BehFigure->BERIN, DelBehAux->NAME );
      }
    }
    else
    {
      delauthelem( BoomUselessAuxHashTable, BehAux->NAME );

      PrevBehAux = &BehAux->NEXT;
      BehAux     = BehAux->NEXT;
    }
  }

  BoomBehMapAbl( BehFigure, BoomReplaceUselessAuxAbl, 1 );

  BoomBehFreeBehAux( HeadBehAux );

  resetauthtable( BoomUselessAuxHashTable );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomBehCreateAux                       |
|                                                             |
\------------------------------------------------------------*/

void BoomBehCreateAux( BehFigure )

  befig_list *BehFigure;
{
  beaux_list  *InitialAux;
  bddcircuit  *BddCircuit;
  bddsystem   *BddSystem;
  long         Reverse;

  BddCircuit = BehFigure->CIRCUI;
  BddSystem  = BddCircuit->BDD_SYSTEM;

  setbddlocalcircuit( BddCircuit );
  setbddlocalsystem( BddSystem );

  BoomBehFigure = BehFigure;

# ifdef DEBUG
fprintf( stdout, "Before BoomBehMakeBddNode\n" );
testbddcircuit( (bddcircuit *)0 );
# endif

  InitialAux    = BehFigure->BEAUX;
  BoomAuxNumber = 0;

  if ( BoomAuxHashTable == (authtable *)0 )
  {
    BoomAuxHashTable     = createauthtable( 1000 );
    BoomNotAuxHashTable  = createauthtable( 1000 );
    BoomAuxNameHashTable = createauthtable( 1000 );
  }

  for ( Reverse = 0; Reverse < 4; Reverse++ )
  {
    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "BoomBehCreateAux Reverse %ld\n", Reverse );
    }

    if ( Reverse != 0 )
    {
      BoomBehReverseAbl( BehFigure );
    }

    /*
    garbagebddsystem( (bddsystem *)0 );
    fprintf( stdout, "Before %ld ", BddLocalSystem->NUMBER_NODE );
    */

    BoomBehUnflatAbl( BehFigure );
    BoomBehMakeBddNode( BehFigure, 1 );

    BoomBehInitializeAux( BehFigure );
    BoomBehMapAblWithName( BehFigure, BoomCreateAuxAblWithName );
    BoomBehUnmarkBddNode( BehFigure );

    /*
    garbagebddsystem( (bddsystem *)0 );
    fprintf( stdout, "After %ld\n", BddLocalSystem->NUMBER_NODE );
    */

    resetauthtable( BoomAuxHashTable     );
    resetauthtable( BoomNotAuxHashTable  );
    resetauthtable( BoomAuxNameHashTable );

    BoomBehFlatAbl( BehFigure );
    BoomBehSimplifyAbl( BehFigure );

    /*
    BoomBehDeleteUselessAux( BehFigure, InitialAux, 0 );
    */

    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "Reverse %ld Aux %ld\n", Reverse, BoomAuxNumber );
    }
  }

  /*
  for ( BoomAuxMinDepth = 15; BoomAuxMinDepth >= 2; BoomAuxMinDepth-- )
  BoomAuxMinDepth = 2;
  {
    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "Check Depth %ld\n", BoomAuxMinDepth );
    }
  }
  */

# ifdef DEBUG
fprintf( stdout, "After BoomCreateAuxAbl\n" );
testbddcircuit( (bddcircuit *)0 );
# endif

# ifdef DEBUG
fprintf( stdout, "After UnmarkAbl\n" );
testbddcircuit( (bddcircuit *)0 );

  BoomBehResetBddNode( BehFigure );
# endif
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomBehComputeAux                      |
|                                                             |
\------------------------------------------------------------*/

beaux_list *BoomBehComputeAux( BehFigure, BehAux )

  befig_list *BehFigure;
  beaux_list *BehAux;
{
  beaux_list  *ScanAux;
  beaux_list **PrevAux;

  PrevAux = &BehFigure->BEAUX;

  for ( ScanAux  = BehFigure->BEAUX;
        ScanAux != (beaux_list *)0;
        ScanAux  = ScanAux->NEXT )
  {
    if ( ScanAux == BehAux )
    {
      *PrevAux = (beaux_list *)0; break;
    }

    PrevAux = &ScanAux->NEXT;
  }

  BoomBehFreeBehAux( BehFigure->BEAUX );

  BehFigure->BEAUX = ScanAux;
  BehAux = ScanAux;

  BoomBehBddNode2Abl( BehFigure );

  if ( IsBoomDebugLevel2() )
  {
    BoomDebugSaveBehFigure( BehFigure, NULL );
  }

  BoomBehCreateAux( BehFigure );

  if ( IsBoomDebugLevel2() )
  {
    BoomDebugSaveBehFigure( BehFigure, NULL );
  }

  return( BehAux );
}
