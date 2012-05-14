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
/*----------------------------------------------------------------------*/
/*                               Includes                               */
/*----------------------------------------------------------------------*/

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"

# include <stdio.h>
# include "bddoptimize.h"
# include "bdderror.h"

/*----------------------------------------------------------------------*/
/*                        Constant declarations                         */
/*----------------------------------------------------------------------*/

/* # define DEBUG */

/*----------------------------------------------------------------------*/
/*                          Type declarations                           */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*                        Stucture declarations                         */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*                        Variable declarations                         */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*                          Macro declarations                          */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*                      Static function prototypes                      */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*                    Definition of static functions                    */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*                   Definition of exported functions                   */
/*----------------------------------------------------------------------*/

void optimizebddcircuit( BddCircuit, CostFunc, MaxSizeFactor )
    bddcircuit *BddCircuit;
    long      (*CostFunc)();
    float       MaxSizeFactor;
{
  long   CurrSize;
  long   MaxSize;
  bddvar StartPos;
  bddvar CurrPos;
  bddvar BestPos;
  long   CurrCost;
  long   BestCost;
  bddindex BddIndex;	
  bddindex NumberIndex;
  bddvar   NumberVar;
  
  setbddlocalcircuit( BddCircuit );

# ifndef DEBUG
  printf( "   - optimizing: " );
# endif
  garbagebddsystem( BddLocalSystem );

  NumberIndex = BddLocalSystem->NUMBER_INDEX;
  NumberVar   = BddLocalSystem->NUMBER_VAR;
  
  for ( BddIndex = BDD_INDEX_MIN; BddIndex < NumberIndex; BddIndex++ ) {

# ifndef DEBUG
    putchar( '.' ); fflush( stdout );
# endif
    StartPos = BddLocalSystem->INDEX_TO_VAR[ BddIndex ];
    CurrPos  = StartPos;
    BestPos  = StartPos;
    CurrCost = CostFunc( BddLocalCircuit );
    BestCost = CurrCost;

    CurrSize = BddLocalSystem->NUMBER_NODE;
    MaxSize  = MaxSizeFactor * CurrSize;

    if ( BddLocalSystem->MAX_NODE != 0 && MaxSize > BddLocalSystem->MAX_NODE )
      MaxSize = BddLocalSystem->MAX_NODE;

# ifdef DEBUG
	    printf( "  swapping index %d [ bddvar = %d ]\n", BddIndex, StartPos );
	    printf( "  initial cost = %ld\n", CurrCost );
# endif
  
    while ( CurrPos < NumberVar - 1 && CurrSize < MaxSize ) {
        swapbddvar( BddLocalSystem, CurrPos );
        CurrPos += 1;
        CurrSize = BddLocalSystem->NUMBER_NODE;
        CurrCost = CostFunc( BddLocalCircuit ); 
        if ( CurrCost < BestCost ) {
# ifdef DEBUG
	    printf( "  --> BestCost = %ld\n", CurrCost );
# endif
	    BestPos = CurrPos;
	    BestCost = CurrCost;
	}
    }
# ifdef DEBUG
  if ( ! ( CurrSize < MaxSize ) )
    printf( "  --> Max Size Limit Reached (%ld)!!!\n", MaxSize );
# endif
    
    while ( CurrPos != StartPos ) {
      CurrPos -= 1;
      swapbddvar( BddLocalSystem, CurrPos );
    }
    
    CurrSize = BddLocalSystem->NUMBER_NODE;
    while ( CurrPos != 0 && CurrSize < MaxSize ) {
        CurrPos -= 1;
        swapbddvar( BddLocalSystem, CurrPos );
        CurrSize = BddLocalSystem->NUMBER_NODE;
        CurrCost = CostFunc( BddLocalCircuit ); 
        if ( CurrCost < BestCost ) {
# ifdef DEBUG
	    printf( "  --> BestCost = %ld\n", CurrCost );
# endif
	    BestPos = CurrPos;
	    BestCost = CurrCost;
	}
    }

# ifdef DEBUG
  if ( ! ( CurrSize < MaxSize ) )
    printf( "  --> Max Size Limit Reached (%ld)!!!\n", MaxSize );
# endif
	
    while ( CurrPos != BestPos) {
	swapbddvar( BddLocalSystem, CurrPos );
        CurrPos += 1;
    }
  }
  
  garbagebddsystem( BddLocalSystem );
# ifndef DEBUG
  putchar( '\n' );
# endif
}
