/*------------------------------------------------------------\
|                                                             |
| Tool    :                     Bdd                           |
|                                                             |
| File    :                  bddvartree.c                     |
|                                                             |
| Date    :                   03.12.96                        |
|                                                             |
| Author  :               Jacomme Ludovic                     |
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

# include <stdio.h>
# include "bddvartree.h"
# include "bdderror.h"

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
|                      Search Bdd Variable Tree               |
|                                                             |
\------------------------------------------------------------*/

bddvar searchbddvartree( BddVarTree, Variable )

  bddvartree *BddVarTree;
  bddvar      Variable;
{
  long     First;
  long     Last;
  long     Middle;

  if ( Variable > BddVarTree->LAST )
  {
    return( BDD_MIN_VAR );
  }

  if ( ( BddVarTree->FIRST != BDD_MIN_VAR ) &&
       ( BddVarTree->FIRST >  Variable    ) )
  {
    return( (bddvar)0 );
  }

  First = 0;
  Last  = BddVarTree->NUMBER_CHILD - 1;

  while ( First <= Last )
  {
    Middle = ( First + Last ) >> 1;

    if ( ( BddVarTree->CHILD[ Middle ]->FIRST <= Variable   ) &&
         ( BddVarTree->CHILD[ Middle ]->LAST  >= Variable ) )
    {
      return( (bddvar  )Middle );
    }

    if ( BddVarTree->CHILD[ Middle ]->FIRST > Variable )
    {
      Last  = Middle - 1;
    }
    else
    {
      First = Middle + 1;
    }
  }

  return ( (bddvar)First );
}

/*------------------------------------------------------------\
|                                                             |
|                       Delta Bdd Variable Tree               |
|                                                             |
\------------------------------------------------------------*/

void deltabddvartree( BddVarTree, Delta )

  bddvartree *BddVarTree;
  long        Delta;
{
  long  Scan;

  BddVarTree->FIRST += Delta;
  BddVarTree->LAST  += Delta;

  for ( Scan = 0; Scan < BddVarTree->NUMBER_CHILD; Scan++ )
  {
    deltabddvartree( BddVarTree->CHILD[ Scan ], Delta );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       Shift Bdd Variable Tree               |
|                                                             |
\------------------------------------------------------------*/

bddvartree *shiftbddvartree( BddVarTree, Variable )

  bddvartree *BddVarTree;
  bddvar      Variable;
{
  bddvartree  *NewTree;
  bddvarchild *NewChild;
  bddvarchild *PrevChild;
  long         NumberChild;
  long         ScanChild;
  long         Child;

  if ( ( BddVarTree->FIRST != BDD_MIN_VAR ) &&
       ( BddVarTree->FIRST >= Variable    ) )
  {
    deltabddvartree( BddVarTree, 1 );

    return( BddVarTree );
  }

  if ( Variable > BddVarTree->LAST )
  {
    return( BddVarTree );
  }

  BddVarTree->LAST++;

  NumberChild = BddVarTree->NUMBER_CHILD;
  PrevChild   = BddVarTree->CHILD;
  Child       = searchbddvartree( BddVarTree, Variable );

  if ( ( Child == BddVarTree->NUMBER_CHILD          ) ||
       ( Child == BddVarTree->CHILD[ Child ]->FIRST ) )
  {
    NumberChild = NumberChild + 1;
    NewChild    = allocbddvarchild( NumberChild );

    if ( PrevChild != (bddvarchild *)0 )
    {
      for ( ScanChild = NumberChild - 1; ScanChild >= 0; ScanChild-- )
      {
        if ( ( ScanChild >= Child ) &&
             ( ScanChild >  0     ) )
        {
          NewChild[ ScanChild ] = shiftbddvartree( PrevChild[ ScanChild - 1 ], Variable );
        }
        else
        {
          NewChild[ ScanChild ] = PrevChild[ ScanChild ];
        }
      }

      freebddvarchild( PrevChild );
    }

    NewTree = allocbddvartree();
    NewTree->FIRST   = Variable;
    NewTree->LAST    = Variable;
    NewChild[ Child ] = NewTree;

    BddVarTree->NUMBER_CHILD = NumberChild;
    BddVarTree->CHILD        = NewChild;
  }
  else
  {
    while ( Child < NumberChild )
    {
      shiftbddvartree( PrevChild[ Child ], Variable );
      Child = Child + 1;
    }
  }

  return( BddVarTree );
}

/*------------------------------------------------------------\
|                                                             |
|                          Add Variable Tree                  |
|                                                             |
\------------------------------------------------------------*/

static void addvartree( BddVarTree, NewVarTree )

  bddvartree *BddVarTree;
  bddvartree *NewVarTree;
{
  bddvarchild *NewChild;
  bddvar       FirstChild;
  bddvar       LastChild;
  bddvar       ScanChild;
  bddvar       NumberChild;
  bddvar       NewNumberChild;

  if ( BddVarTree->NUMBER_CHILD )
  {
    FirstChild = searchbddvartree( BddVarTree, NewVarTree->FIRST );
    LastChild  = searchbddvartree( BddVarTree, NewVarTree->LAST  );

    if ( FirstChild == LastChild )
    {
      addvartree( BddVarTree->CHILD[ FirstChild ], NewVarTree );
    }
    else
    {
      NewNumberChild = LastChild - FirstChild + 1;
      NewChild       = allocbddvarchild( NewNumberChild );

      for ( ScanChild = 0; ScanChild < NewNumberChild; ScanChild++ )
      {
        NewChild[ ScanChild ] = BddVarTree->CHILD[ FirstChild + ScanChild ];
      }

      NumberChild = BddVarTree->NUMBER_CHILD - ( NewNumberChild - 1 );
      resizebddvarchild( BddVarTree->CHILD, BddVarTree->NUMBER_CHILD, NumberChild );
      BddVarTree->NUMBER_CHILD = NumberChild;

      NewVarTree->NUMBER_CHILD = NewNumberChild;
      NewVarTree->CHILD = NewChild;
    }
  }
  else
  {
    NewChild = allocbddvarchild( 1 );
    NewChild[ 0 ] = NewVarTree;

    BddVarTree->NUMBER_CHILD = 1;
    BddVarTree->CHILD = NewChild;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Add Bdd Variable Tree                |
|                                                             |
\------------------------------------------------------------*/

bddvartree *addbddvartree( BddSystem, VarFrom, VarTo )

  bddsystem *BddSystem;
  bddvar     VarFrom;
  bddvar     VarTo;
{
  bddvartree *BddVarTree;

  checkbddvar( BddSystem, VarFrom, 1 );
  checkbddvar( BddSystem, VarTo  , 1 );

  if ( VarFrom >= VarTo )
  {
    bdderror( BDD_BAD_VAR_ERROR, VarFrom );
  }

  BddVarTree = allocbddvartree();
  BddVarTree->FIRST = VarFrom;
  BddVarTree->LAST  = VarTo;

  addvartree( BddLocalSystem->VAR_TREE, BddVarTree );

  return( BddVarTree );
}

/*------------------------------------------------------------\
|                                                             |
|                       Swap Bdd Variable Tree                |
|                                                             |
\------------------------------------------------------------*/

void swapbddvartree( BddSystem, BddVarTree, Child )

  bddsystem  *BddSystem;
  bddvartree *BddVarTree;
  bddvar      Child;
{
  bddvartree *BddVarChild;
  bddvartree *BddVarNext;
  long        FirstIndex;
  long        DeltaIndex;
  long        DeltaNext;
  long        ScanIndex;
  long        ScanNext;
  bddvar      Variable;

  setbddlocalsystem( BddSystem );

  if ( Child + 1 >= BddVarTree->NUMBER_CHILD )
  {
    bdderror( BDD_BAD_CHILD_ERROR, Child );
  }

  BddVarChild = BddVarTree->CHILD[ Child     ];
  BddVarNext  = BddVarTree->CHILD[ Child + 1 ];

  DeltaIndex = BddVarChild->LAST - BddVarChild->FIRST;
  DeltaNext  = BddVarNext->LAST  - BddVarNext->FIRST;

  for ( ScanIndex = DeltaIndex; ScanIndex >= 0; ScanIndex-- )
  {
    FirstIndex = BddVarChild->FIRST + ScanIndex;

    for ( ScanNext = 0; ScanNext <= DeltaNext; ScanNext++ )
    {
      Variable = FirstIndex + ScanNext;

      swapbddvar( (bddsystem *)0, Variable );
    }
  }

  deltabddvartree( BddVarChild,   ( DeltaNext  + 1 ) );
  deltabddvartree( BddVarNext , - ( DeltaIndex + 1 ) );
  
  BddVarTree->CHILD[ Child     ] = BddVarNext;
  BddVarTree->CHILD[ Child + 1 ] = BddVarChild;

  resetbddhopertable( BddLocalSystem->HASH_OPER );
}

/*------------------------------------------------------------\
|                                                             |
|                        Destroy Variable Tree                |
|                                                             |
\------------------------------------------------------------*/

static void destroyvartree( BddVarTree )

  bddvartree *BddVarTree;
{
  long Child;

  for ( Child = 0; Child < BddVarTree->NUMBER_CHILD; Child++ )
  {
    destroyvartree( BddVarTree->CHILD[ Child ] );
  }

  if ( Child )
  {
    freebddvarchild( BddVarTree->CHILD );
  }

  freebddvartree( BddVarTree );
}

/*------------------------------------------------------------\
|                                                             |
|                    Bdd Create Variable Tree                 |
|                                                             |
\------------------------------------------------------------*/

bddvartree *createbddvartree( BddSystem )

  bddsystem  *BddSystem;
{
  bddvartree  *BddVarTree;

  setbddlocalsystem( BddSystem );

  BddVarTree = allocbddvartree();
  BddVarTree->FIRST   = BDD_MIN_VAR;
  BddVarTree->REORDER = 1;

  BddLocalSystem->VAR_TREE = BddVarTree;

  return( BddVarTree );
}

/*------------------------------------------------------------\
|                                                             |
|                    Bdd Reset Variable Tree                  |
|                                                             |
\------------------------------------------------------------*/

void resetbddvartree( BddSystem )

  bddsystem *BddSystem;
{
  setbddlocalsystem( BddSystem );

  destroyvartree( BddLocalSystem->VAR_TREE );
  createbddvartree( (bddsystem *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                    Bdd Destroy Variable Tree                |
|                                                             |
\------------------------------------------------------------*/

void destroybddvartree( BddSystem )

  bddsystem  *BddSystem;
{
  setbddlocalsystem( BddSystem );

  destroyvartree( BddLocalSystem->VAR_TREE );
  BddLocalSystem->VAR_TREE = (bddvartree *)0;
}

/*------------------------------------------------------------\
|                                                             |
|                        View Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   View Variable Tree Space                  |
|                                                             |
\------------------------------------------------------------*/

static void viewvartreespace( Space )

  long Space;
{
  while ( Space > 0 )
  {
    fprintf( stdout, " " );

    Space = Space - 1;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     View Variable Tree                      |
|                                                             |
\------------------------------------------------------------*/

static void viewvartree( BddVarTree, Space )

  bddvartree *BddVarTree;
  long        Space;
{
  long Scan;

  viewvartreespace( Space );
  fprintf( stdout, "--> BddVarTree\n" );
  viewvartreespace( Space );
  fprintf( stdout, "NUMBER_CHILD : %d\n", BddVarTree->NUMBER_CHILD );
  viewvartreespace( Space );
  fprintf( stdout, "FIRST : %d\n", BddVarTree->FIRST );

  for ( Scan = 0; Scan < BddVarTree->NUMBER_CHILD; Scan++ )
  {
    viewvartree( BddVarTree->CHILD[ Scan ], Space + 2 );
  }

  viewvartreespace( Space );
  fprintf( stdout, "LAST  : %d\n", BddVarTree->LAST  );
  viewvartreespace( Space );
  fprintf( stdout, "REORDER : %d\n", BddVarTree->REORDER );
  viewvartreespace( Space );
  fprintf( stdout, "<-- BddVarTree\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                      Bdd View Variable Tree                 |
|                                                             |
\------------------------------------------------------------*/

void viewbddvartree( BddVarTree )

  bddvartree *BddVarTree;
{
  viewvartree( BddVarTree, 0 );
}
