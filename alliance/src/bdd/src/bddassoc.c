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
| Tool    :                     Bdd                           |
|                                                             |
| File    :                   bddassoc.c                      |
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

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"

# include <stdio.h>
# include "bddassoc.h"
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

  bddassoc     *BddLocalAssoc     = (bddassoc     *)0;
  bddassocnode *BddLocalAssocNode = (bddassocnode *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Add Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     Add Bdd Association                     |
|                                                             |
\------------------------------------------------------------*/

bddassoc *addbddassoc( BddSystem )

  bddsystem  *BddSystem;
{
  bddassoc  **PrevAssoc;
  bddassoc   *ScanAssoc;
  bddassoc   *BddAssoc;
  bddshort    AssocIdent;

  setbddlocalsystem( BddSystem );

  ScanAssoc  =  BddLocalSystem->ASSOC;
  PrevAssoc  = &BddLocalSystem->ASSOC;
  AssocIdent =  0;

  while ( ( ScanAssoc        != (bddassoc *)0 ) &&
          ( ScanAssoc->IDENT >= AssocIdent    ) )
  {
    if ( AssocIdent == BDD_MAX_SHORT )
    {
      bdderror( BDD_NO_MORE_ASSOC_ERROR, 0 );
    }

    AssocIdent = AssocIdent + 1;
    PrevAssoc  = &ScanAssoc->NEXT;
    ScanAssoc  = ScanAssoc->NEXT;
  }

  BddAssoc = allocbddassoc();

  BddAssoc->ASSOC_NODE = allocbddassocnode( BddLocalSystem->MAX_VAR );
  BddAssoc->FIRST = BDD_MAX_VAR;
  BddAssoc->LAST  = BDD_MAX_VAR;
  BddAssoc->IDENT = AssocIdent;

  BddAssoc->NEXT = ScanAssoc;
  *PrevAssoc     = BddAssoc;

  return( BddAssoc );
}

/*------------------------------------------------------------\
|                                                             |
|                  Add Bdd Node In Association                |
|                                                             |
\------------------------------------------------------------*/

bddassoc *addbddnodeassoc( BddSystem, BddAssoc, Variable, BddNode )

  bddsystem  *BddSystem;
  bddassoc   *BddAssoc;
  bddvar      Variable;
  bddnode    *BddNode;
{
  bddassocnode *BddAssocNode;

  checkbddvar( BddSystem, Variable, 1 );

  BddAssocNode = BddAssoc->ASSOC_NODE;

  if ( BddAssocNode[ Variable ] != (bddnode *)0 )
  {
    decbddrefext( BddAssocNode[ Variable ] );
  }
  else
  {
    if ( ( BddAssoc->FIRST == BDD_MAX_VAR    ) ||
         ( Variable        < BddAssoc->FIRST ) ) BddAssoc->FIRST = Variable;

    if ( ( BddAssoc->LAST  == BDD_MAX_VAR    ) ||
         ( Variable        > BddAssoc->LAST  ) ) BddAssoc->LAST  = Variable;
  }

  BddAssocNode[ Variable ] = incbddrefext( BddNode );

  if ( BddAssoc->CACHE )
  {
    BddLocalSystem->ASSOC_RESET = 1;
  }

  return( BddAssoc );
}

/*------------------------------------------------------------\
|                                                             |
|                         Del Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Del Bdd Association                   |
|                                                             |
\------------------------------------------------------------*/

int delbddassoc( BddSystem, BddAssoc )

  bddsystem  *BddSystem;
  bddassoc   *BddAssoc;
{
  bddassoc    **PrevAssoc;
  bddassoc     *ScanAssoc;
  bddassocnode *BddAssocNode;
  bddvar        Variable;

  setbddlocalsystem( BddSystem );

  PrevAssoc = &BddLocalSystem->ASSOC;
  ScanAssoc = BddLocalSystem->ASSOC;

  while ( ScanAssoc != (bddassoc *)0 )
  {
    if ( ScanAssoc == BddAssoc )
    {
      *PrevAssoc = ScanAssoc->NEXT;

      BddAssocNode = BddAssoc->ASSOC_NODE;

      if ( BddAssoc->FIRST != BDD_MAX_VAR )
      {
        for ( Variable  = BddAssoc->FIRST;
              Variable <= BddAssoc->LAST;
              Variable++ )
        {
          if ( BddAssocNode[ Variable ] != (bddnode *)0 )
          {
            decbddrefext( BddAssocNode[ Variable ] );
          }
        }
      }

      if ( BddAssoc->CACHE )
      {
        BddLocalSystem->ASSOC_RESET = 1;
      }

      freebddassocnode( BddAssocNode );
      freebddassoc( BddAssoc );

      return( 1 );
    }

    PrevAssoc = &ScanAssoc->NEXT;
    ScanAssoc =  ScanAssoc->NEXT;
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                         Reset Functions                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Reset Bdd Association                 |
|                                                             |
\------------------------------------------------------------*/

void resetbddassoc( BddSystem, BddAssoc )

  bddsystem  *BddSystem;
  bddassoc   *BddAssoc;
{
  bddassocnode *BddAssocNode;
  bddvar        Variable;

  setbddlocalsystem( BddSystem );

  BddAssocNode = BddAssoc->ASSOC_NODE;

  if ( BddAssoc->FIRST != BDD_MAX_VAR )
  {
    for ( Variable  = BddAssoc->FIRST;
          Variable <= BddAssoc->LAST;
          Variable++ )
    {
      if ( BddAssocNode[ Variable ] != (bddnode *)0 )
      {
        decbddrefext( BddAssocNode[ Variable ] );
        BddAssocNode[ Variable ] = (bddnode *)0;
      }
    }

    BddAssoc->FIRST = BDD_MAX_VAR;
    BddAssoc->LAST  = BDD_MAX_VAR;

    if ( BddAssoc->CACHE )
    {
      BddLocalSystem->ASSOC_RESET = 1;
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                  Del Bdd Node In Association                |
|                                                             |
\------------------------------------------------------------*/

int delbddnodeassoc( BddSystem, BddAssoc, Variable )

  bddsystem  *BddSystem;
  bddassoc   *BddAssoc;
  bddvar      Variable;
{
  bddassocnode *BddAssocNode;

  checkbddvar( BddSystem, Variable, 1 );

  BddAssocNode = BddAssoc->ASSOC_NODE;

  if ( BddAssocNode[ Variable ] != (bddnode *)0 )
  {
    decbddrefext( BddAssocNode[ Variable ] );
    BddAssocNode[ Variable ] = (bddnode *)0;

    if ( BddAssoc->CACHE )
    {
      BddLocalSystem->ASSOC_RESET = 1;
    }

    if ( ( Variable == BddAssoc->FIRST ) &&
         ( Variable == BddAssoc->LAST  ) )
    {
      BddAssoc->FIRST = BDD_MAX_VAR;
      BddAssoc->LAST  = BDD_MAX_VAR;

      return( 1 );
    }

    if ( Variable == BddAssoc->FIRST )
    {
      do
      {
        Variable = Variable + 1;
      }
      while ( BddAssocNode[ Variable ] == (bddnode *)0 );

      BddAssoc->FIRST = Variable;

      return( 1 );
    }

    if ( Variable == BddAssoc->LAST )
    {
      do
      {
        Variable = Variable - 1;
      }
      while ( BddAssocNode[ Variable ] == (bddnode *)0 );

      BddAssoc->LAST  = Variable;

      return( 1 );
    }

    return( 1 );
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                      Destroy Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    Destroy Bdd Association                  |
|                                                             |
\------------------------------------------------------------*/

void destroybddassoc( BddSystem )

  bddsystem *BddSystem;
{
  bddassoc      *BddAssoc;
  bddassoc      *DelAssoc;

  setbddlocalsystem( BddSystem );

  BddAssoc = BddLocalSystem->ASSOC;
  BddLocalSystem->ASSOC = (bddassoc *)0;

  while ( BddAssoc != (bddassoc *)0 )
  {
    DelAssoc = BddAssoc;
    BddAssoc = BddAssoc->NEXT;

    freebddassocnode( DelAssoc->ASSOC_NODE );
    freebddassoc( DelAssoc );
  }

  BddLocalSystem->ASSOC_RESET = 0;
}

/*------------------------------------------------------------\
|                                                             |
|                        View Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      View Bdd Association                   |
|                                                             |
\------------------------------------------------------------*/

void viewbddassoc( BddSystem, BddAssoc, FuncView )

  bddsystem *BddSystem;
  bddassoc  *BddAssoc;
  void     (*FuncView)();
{
  bddassocnode *BddAssocNode;
  bddvar        Variable;

  if ( BddSystem == (bddsystem *)0 )
  {
    BddSystem = BddLocalSystem;
  }

  fprintf( stdout, "--> BddAssoc\n" );
  fprintf( stdout, "  FIRST : %ld\n", (long)BddAssoc->FIRST );
  fprintf( stdout, "  LAST  : %ld\n", (long)BddAssoc->LAST  );
  fprintf( stdout, "  IDENT : %ld\n", (long)BddAssoc->IDENT );
  fprintf( stdout, "  CACHE : %ld\n", (long)BddAssoc->CACHE );

  if ( BddAssoc->FIRST != BDD_MAX_VAR )
  {
    BddAssocNode = BddAssoc->ASSOC_NODE;

    for ( Variable   = BddAssoc->FIRST;
          Variable  <= BddAssoc->LAST;
          Variable   = Variable + 1 )
    {
      if ( BddAssocNode[ Variable ] != (bddnode *)0 )
      {
        fprintf( stdout, "  ASSOC_NODE[ %ld ] : ", (long)Variable );

        if ( FuncView )
        {
          (*FuncView)( Variable, BddAssocNode[ Variable ] );
        }
        else
        {
          fprintf( stdout, "\n" );
        }
      }
    }
  }

  fprintf( stdout, "<-- BddAssoc\n" );
}
