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
| File    :                   bddorder.c                      |
|                                                             |
| Date    :                   03.12.96                        |
|                                                             |
| Author  :               Jacomme Ludovic                     |
| Modif   : Arnaud Caron - 16 Oct 1998                        |
|           Suppression des bddvartrees.                      |
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
# include "bddreorder.h"
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
|                  Reorder Bdd Variable Window 2              |
|                                                             |
\------------------------------------------------------------*/

int
#ifdef __STDC__
reorderbddvarwindow2( bddsystem *BddSystem, bddvar BddVar )
#else
reorderbddvarwindow2( BddSystem, BddVar )
	
  bddsystem *BddSystem;
  bddvar     BddVar;
#endif
{
  long NumberNode;

  setbddlocalsystem( BddSystem );
  
  NumberNode = BddLocalSystem->NUMBER_NODE;

  swapbddvar( BddLocalSystem, BddVar );

  if ( BddLocalSystem->NUMBER_NODE < NumberNode )
  {
    return( 1 );
  }

  swapbddvar( BddLocalSystem, BddVar );

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                 Reorder Bdd Variable Window 3               |
|                                                             |
\------------------------------------------------------------*/

int
#ifdef __STDC__
reorderbddvarwindow3( bddsystem *BddSystem, bddvar BddVar )
#else
reorderbddvarwindow3( BddSystem, BddVar )

  bddsystem *BddSystem;
  bddvar     BddVar;
#endif
{
  long NumberNode;
  char BestOrder;

  setbddlocalsystem( BddSystem );

  BestOrder  = BDD_REORDER_123;
  NumberNode = BddLocalSystem->NUMBER_NODE;

  swapbddvar( BddLocalSystem, BddVar );

  if ( BddLocalSystem->NUMBER_NODE < NumberNode )
  {
    NumberNode = BddLocalSystem->NUMBER_NODE;
    BestOrder  = BDD_REORDER_213;
  }

  swapbddvar( BddLocalSystem, BddVar );

  if ( BddLocalSystem->NUMBER_NODE < NumberNode )
  {
    NumberNode = BddLocalSystem->NUMBER_NODE;
    BestOrder  = BDD_REORDER_231;
  }

  swapbddvar( BddLocalSystem, BddVar );

  if ( BddLocalSystem->NUMBER_NODE < NumberNode )
  {
    NumberNode = BddLocalSystem->NUMBER_NODE;
    BestOrder  = BDD_REORDER_321;
  }

  swapbddvar( BddLocalSystem, BddVar );

  if ( BddLocalSystem->NUMBER_NODE < NumberNode )
  {
    NumberNode = BddLocalSystem->NUMBER_NODE;
    BestOrder  = BDD_REORDER_312;
  }

  swapbddvar( BddLocalSystem, BddVar );

  if ( BddLocalSystem->NUMBER_NODE < NumberNode )
  {
    NumberNode = BddLocalSystem->NUMBER_NODE;
    BestOrder  = BDD_REORDER_132;
  }

  switch ( BestOrder )
  {
    case BDD_REORDER_123 : swapbddvar( BddLocalSystem, BddVar );
    break;

    case BDD_REORDER_213 : swapbddvar( BddLocalSystem, BddVar );
                           swapbddvar( BddLocalSystem, BddVar );
    break;

    case BDD_REORDER_231 : swapbddvar( BddLocalSystem, BddVar );
                           swapbddvar( BddLocalSystem, BddVar );
                           swapbddvar( BddLocalSystem, BddVar );
    break;

    case BDD_REORDER_321 : swapbddvar( BddLocalSystem, BddVar );
                           swapbddvar( BddLocalSystem, BddVar );
    break;

    case BDD_REORDER_312 : swapbddvar( BddLocalSystem, BddVar );
    break;
  }

  return( BestOrder != BDD_REORDER_123 );
}


/*------------------------------------------------------------\
|                                                             |
|                    Reorder Bdd System Simple                |
|                                                             |
\------------------------------------------------------------*/

void
#ifdef __STDC__
reorderbddsystemsimple( bddsystem *BddSystem )
#else
reorderbddsystemsimple( BddSystem )

  bddsystem *BddSystem;
#endif
{
  bddvar BddVar;
  
  setbddlocalsystem( BddSystem );

  garbagebddsystem( BddLocalSystem );

  for ( BddVar = 0; BddVar < BddLocalSystem->NUMBER_VAR - 1; BddVar++ )
  {
    if ( BddVar < BddLocalSystem->NUMBER_VAR - 2 )
    {
      reorderbddvarwindow3( BddLocalSystem, BddVar );
    }
    else
    {
      reorderbddvarwindow2( BddLocalSystem, BddVar );
    }
  }
  garbagebddsystem( BddLocalSystem );
}

/*------------------------------------------------------------\
|                                                             |
|                   Reorder Bdd System Window                 |
|                                                             |
\------------------------------------------------------------*/

void
#ifdef __STDC__
reorderbddsystemwindow( bddsystem *BddSystem )
#else
reorderbddsystemwindow( BddSystem )

  bddsystem *BddSystem;
#endif
{
  bddvar  NumberVar;
  bddvar  BddVar;
  char    Moved;
  char    AnySwap;
  char   *BddReorderLevel;

  setbddlocalsystem( BddSystem );

  BddReorderLevel = autallocblock( BddLocalSystem->NUMBER_VAR + 1 );

  garbagebddsystem( BddLocalSystem );

  NumberVar = BddLocalSystem->NUMBER_VAR;

  for ( BddVar = 0; BddVar < NumberVar - 1; BddVar++ )
  {
    BddReorderLevel[ BddVar ] = 1;
  }

  do
  {
    AnySwap = 0;

    for ( BddVar = 0; BddVar < NumberVar - 1; BddVar++ )
    {
      if ( BddReorderLevel[ BddVar ] )
      {
        if ( BddVar < NumberVar - 2 )
        {
          Moved = reorderbddvarwindow3( BddLocalSystem, BddVar );
        }
        else
        {
          Moved = reorderbddvarwindow2( BddLocalSystem, BddVar );
        }

        if ( Moved )
        {
          AnySwap = 1;

          if ( BddVar > 0 ) BddReorderLevel[ BddVar - 1 ] = 1;
          if ( BddVar > 1 ) BddReorderLevel[ BddVar - 2 ] = 1;

          BddReorderLevel[ BddVar + 1 ] = 1;

          if ( BddVar < NumberVar - 2 ) BddReorderLevel[ BddVar + 2 ] = 1;
          if ( BddVar < NumberVar - 3 ) BddReorderLevel[ BddVar + 3 ] = 1;
          if ( BddVar < NumberVar - 4 ) BddReorderLevel[ BddVar + 4 ] = 1;
        }
        else
        {
          BddReorderLevel[ BddVar ] = 0;
        }
      }
    }
  }
  while ( AnySwap );

  autfreeblock( BddReorderLevel );

  garbagebddsystem( BddLocalSystem );
}

/*------------------------------------------------------------\
|                                                             |
|                   Reorder Bdd System Top                    |
|                                                             |
\------------------------------------------------------------*/

void
#ifdef __STDC__
reorderbddsystemtop( bddsystem *BddSystem )
#else
reorderbddsystemtop( BddSystem )

  bddsystem *BddSystem;
#endif
{
  long    NumberNode;
  bddvar  NumberVar;
  bddvar  BestVar;
  bddvar  VarCount;
  bddvar  VarTo;
  bddvar  VarFrom;
  char    SwapReorder;
  char   *BddReorderLevel;

  setbddlocalsystem( BddSystem );

  BddReorderLevel = autallocblock( BddLocalSystem->NUMBER_VAR + 1 );

  garbagebddsystem( BddLocalSystem );

  NumberVar = BddLocalSystem->NUMBER_VAR;

  for ( VarFrom = 0; VarFrom < ( NumberVar - 1 ); VarFrom++ )
  {
    BddReorderLevel[ VarFrom ] = 1;
  }

  NumberNode = BddLocalSystem->NUMBER_NODE;
  VarCount   = NumberVar - 1;
  VarFrom    = 0;

  while ( VarCount > 0 )
  {
# ifdef BDD_DEBUG
  fprintf( stdout, "-> VarFrom %d\n", VarFrom );
# endif

    BddReorderLevel[ VarFrom ] = 0;

    BestVar  = VarFrom;

    for ( VarTo = VarFrom; VarTo < NumberVar - 1; VarTo++ )
    {
# ifdef BDD_DEBUG
  fprintf( stdout, "-> Swap %d and %d\n", VarTo, VarTo + 1 );
# endif

      swapbddvar( BddLocalSystem, VarTo );

      SwapReorder                  = BddReorderLevel[ VarTo     ];
      BddReorderLevel[ VarTo     ] = BddReorderLevel[ VarTo + 1 ];
      BddReorderLevel[ VarTo + 1 ] = SwapReorder;

      if ( BddLocalSystem->NUMBER_NODE < NumberNode )
      {
        BestVar = VarTo + 1; NumberNode = BddLocalSystem->NUMBER_NODE;
      }
    }

# ifdef BDD_DEBUG
  fprintf( stdout, "--> Best one %d\n", BestVar );
  fprintf( stdout, "--> Number nodes %d\n", NumberNode );
# endif

    for ( VarTo = NumberVar - 1; VarTo > BestVar; VarTo-- )
    {
# ifdef BDD_DEBUG
  fprintf( stdout, "-> UnSwap %d and %d\n", VarTo - 1, VarTo );
# endif

      SwapReorder                  = BddReorderLevel[ VarTo - 1 ];
      BddReorderLevel[ VarTo - 1 ] = BddReorderLevel[ VarTo     ];
      BddReorderLevel[ VarTo     ] = SwapReorder;

      swapbddvar( BddLocalSystem, VarTo - 1 );
    }

    while ( ! BddReorderLevel[ VarFrom ] )
    {
      VarFrom = VarFrom + 1;
    }

    VarCount = VarCount - 1;
  }

  autfreeblock( BddReorderLevel );

  garbagebddsystem( BddLocalSystem );
}

/*----------------------------------------------------------------------*/
/*                        Reorder Bdd System Sift                       */
/*----------------------------------------------------------------------*/

void
#ifdef __STDC__
reorderbddsystemsift( bddsystem *BddSystem )
#else
reorderbddsystemsift( BddSystem )

  bddsystem *BddSystem;
#endif
{
  setbddlocalsystem( BddSystem );

  garbagebddsystem( BddLocalSystem );

  fprintf( stderr, "reorderbddsystemsift: not implemented\n" );
	
  garbagebddsystem( BddLocalSystem );
}

/*------------------------------------------------------------\
|                                                             |
|                   Reorder Bdd System Dynamic                |
|                                                             |
\------------------------------------------------------------*/

void reorderbddsystemdynamic( BddSystem, ReorderFunc, ReorderLow, ReorderRatio )

  bddsystem *BddSystem;
  void     (*ReorderFunc)();
  long       ReorderLow;
  long       ReorderRatio;
{
  setbddlocalsystem( BddSystem );

  if ( ReorderFunc != (void *)0 )
  {
    SetBddSystemReorderOn( BddLocalSystem );
  }
  else
  {
    ClearBddSystemReorderOn( BddLocalSystem );
  }

  BddLocalSystem->REORDER_FUNC  = ReorderFunc;
  BddLocalSystem->REORDER_LOW   = ReorderLow;
  BddLocalSystem->REORDER_LIMIT = ReorderLow;
  BddLocalSystem->REORDER_RATIO = ReorderRatio + 100;
}
