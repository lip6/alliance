/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
| File    :                  bddcheck.c                       |
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
# include "bddcheck.h"
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
|                      Check Bdd Variable                     |
|                                                             |
\------------------------------------------------------------*/

int checkbddvar( BddSystem, Variable, Severity )

  bddsystem *BddSystem;
  bddvar     Variable;
  int        Severity;
{
  setbddlocalsystem( BddSystem ); 

  if ( Variable >= BddLocalSystem->NUMBER_VAR )
  {
    if ( Severity )
    {
      bdderror( BDD_BAD_VAR_ERROR, Variable );
    }

    return( 0 );
  }

  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                       Check Bdd Index                       |
|                                                             |
\------------------------------------------------------------*/

int checkbddindex( BddSystem, Index, Severity )

  bddsystem *BddSystem;
  bddindex   Index;
  int        Severity;
{
  setbddlocalsystem( BddSystem ); 

  if ( ( Index <  BDD_INDEX_MIN                ) ||
       ( Index >= BddLocalSystem->NUMBER_INDEX ) )
  {
    if ( Severity )
    {
      bdderror( BDD_BAD_INDEX_ERROR, Index );
    }

    return( 0 );
  }

  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                       Check Bdd Operator                    |
|                                                             |
\------------------------------------------------------------*/

int checkbddoper( BddSystem, Oper, Severity )

  bddsystem *BddSystem;
  long       Oper;
  int        Severity;
{
  setbddlocalsystem( BddSystem ); 

  if ( ( Oper < 0         ) ||
       ( Oper >= ABL_NXOR ) )
  {
    if ( Severity )
    {
      bdderror( BDD_OPERATOR_ERROR, Oper );
    }

    return( 0 );
  }

  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                      Check Bdd Assoc                        |
|                                                             |
\------------------------------------------------------------*/

int checkbddassoc( BddSystem, BddAssoc, Severity )

  bddsystem *BddSystem;
  bddassoc  *BddAssoc;
  int        Severity;
{
  setbddlocalsystem( BddSystem );

  if ( BddLocalSystem->ASSOC_RESET )
  {
    resetbddhopertable( BddLocalSystem->HASH_OPER );
  }

  if ( ( BddAssoc->FIRST == BDD_MAX_VAR ) &&
       ( BddAssoc->LAST  == BDD_MAX_VAR ) ) 
  {
    return( 2 );
  }

  if ( BddAssoc->FIRST >= BDD_MAX_VAR )
  {
    if ( Severity )
    {
      bdderror( BDD_BAD_ASSOC_ERROR, BddAssoc->FIRST );
    }

    return( 0 );
  }

  if ( BddAssoc->LAST >= BDD_MAX_VAR )
  {
    if ( Severity )
    {
      bdderror( BDD_BAD_ASSOC_ERROR, BddAssoc->LAST );
    }

    return( 0 );
  }

  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                       Check Bdd Max Node                    |
|                                                             |
\------------------------------------------------------------*/

int checkbddmaxnode( BddSystem, Severity )

  bddsystem *BddSystem;
  int        Severity;
{
  long ReorderLimit;
  long FirstNumberNode;

  setbddlocalsystem( BddSystem );

  if ( IsBddSystemReorderOn( BddLocalSystem ) )
  {
    if ( ( BddLocalSystem->NUMBER_NODE  > BddLocalSystem->MAX_NODE      ) ||
         ( BddLocalSystem->NUMBER_NODE  > BddLocalSystem->REORDER_LIMIT ) )
    {
      FirstNumberNode = BddLocalSystem->NUMBER_NODE;

      SetBddSystemReorder( BddLocalSystem );

      BddLocalSystem->REORDER_FUNC( (bddsystem *)0 );

      if ( ! IsBddSystemNoWarning( BddLocalSystem ) )
      {
        bddwarning( BDD_DYNAMIC_REORDER_WARNING, 
                    FirstNumberNode, BddLocalSystem->NUMBER_NODE );
      }

      ReorderLimit = ( BddLocalSystem->NUMBER_NODE   * 
                       BddLocalSystem->REORDER_RATIO ) / 100;

      if ( ReorderLimit < BddLocalSystem->REORDER_LOW )
      {
        BddLocalSystem->REORDER_LIMIT = BddLocalSystem->REORDER_LOW;
      }
      else
      {
        BddLocalSystem->REORDER_LIMIT = ReorderLimit;
      }
    }
  }

  if ( BddLocalSystem->NUMBER_NODE > BddLocalSystem->MAX_NODE )
  {
    if ( Severity )
    {
      bdderror( BDD_MAX_NODE_ERROR, BddLocalSystem->NUMBER_NODE );
    }

    return( 0 );
  }

  return( 1 );
}
