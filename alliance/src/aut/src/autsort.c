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
| Tool    :                     Aut                           |
|                                                             |
| File    :                   autsort.c                       |
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

# include <stdio.h>
# include <string.h>

# include <mut.h>
# include "aut.h"

# include "autsort.h"
# include "auterror.h"

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
|                       Aut Sort Compare                      |
|                                                             |
\------------------------------------------------------------*/

long sortautcompare( ValueArray, Index1, Index2 )

  long *ValueArray;
  long  Index1;
  long  Index2;
{
  return( ValueArray[ Index1 ] - ValueArray[ Index2 ] );
}

/*------------------------------------------------------------\
|                                                             |
|                        Sort Aut Array                       |
|                                                             |
\------------------------------------------------------------*/

void sortautarray( ValueArray, IndexArray, ArraySize, FuncCompare )

  void  *ValueArray;
  long  *IndexArray;
  long   ArraySize;
  long (*FuncCompare)();
{
  long Node;
  long NodeIndex;
  long LastNode;
  long Father;
  long FatherIndex;
  long LeftChild; 
  long LeftIndex;
  long RightChild;
  long RightIndex;
  long Child;
  long ChildIndex;

  if ( ! FuncCompare ) FuncCompare = sortautcompare;

  for ( LastNode = 0; LastNode < ArraySize; LastNode++ )
  {
    IndexArray[ LastNode ] = LastNode;

    NodeIndex = LastNode;
    Node      = LastNode;

    do
    {
      Father = HEAP_FATHER( Node );

      if ( Father != -1 )
      {
        FatherIndex = IndexArray[ Father ];

        if ( (*FuncCompare)( ValueArray, FatherIndex, NodeIndex ) > 0 )
        {
          IndexArray[ Father ] = NodeIndex;
          IndexArray[ Node   ] = FatherIndex;

          Node = Father;
        }
      }
    }
    while ( Node == Father );
  }

  for ( LastNode = ArraySize - 1; LastNode >= 0; LastNode-- )
  {
    Node = 0;

    NodeIndex              = IndexArray[ LastNode ];
    IndexArray[ LastNode ] = IndexArray[ 0 ];
    IndexArray[ 0 ]        = NodeIndex;

    do
    {
      LeftChild  = HEAP_LEFT_CHILD( Node );
      RightChild = HEAP_RIGHT_CHILD( Node );

      if ( LeftChild < LastNode )
      {
        LeftIndex  = IndexArray[ LeftChild  ];
        RightIndex = IndexArray[ RightChild ];

        if ( ( RightChild                   < LastNode  ) &&
             ( (*FuncCompare)( ValueArray, 
                               RightIndex,
                               LeftIndex  ) < 0         ) )
        {
          Child      = RightChild;
          ChildIndex = RightIndex;
        }
        else
        {
          Child      = LeftChild;
          ChildIndex = LeftIndex;
        }

        if ( (*FuncCompare)( ValueArray, NodeIndex, ChildIndex ) > 0 )
        {
          IndexArray[ Child ] = NodeIndex;
          IndexArray[ Node ]  = ChildIndex;

          Node = Child;
        }
        else 
        {
          LeftChild = LastNode;
        }
      }
    }
    while ( LeftChild < LastNode );
  }
}
