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
| File    :                  autfree.c                        |
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
# include <malloc.h>
# include <memory.h>
# include <string.h>

# include <mut.h>
# include "aut.h"

# include "autfree.h"
# include "autenv.h"
# include "autalloc.h"
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
|                     Aut Free Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Aut Free                           |
|                                                             |
\------------------------------------------------------------*/

void  autfree( Pointer, Size )

      char         *Pointer;
      unsigned int  Size;
{
  unsigned int Index;

  if ( ! AUT_ALLOC_DEBUG )
  {
    Index = ( Size + 3 ) >> 2;
  
    if ( ( Size  <  sizeof(char *) ) ||
         ( Index > 255             ) )
    {
      free( Pointer );
    }
    else
    {
      memset( Pointer, 0, Size );
  
      *((char **)Pointer) = AutHeapHeadArray[ Index ];
  
      AutHeapHeadArray[ Index ] = Pointer;
    }
  }
  else
  {
    autallocdebug *AllocDebug;

    AllocDebug = (autallocdebug *)(Pointer - sizeof(autallocdebug));
    Pointer    = (char *)AllocDebug;

    if ( AllocDebug->POINTER != AllocDebug )
    {
      fprintf( stdout, "ERROR autfree %lx != %lx !!!!\n", AllocDebug->POINTER, AllocDebug );
      autexit( 1 );
    }

    if ( ( AllocDebug->HEAP ) &&
         ( Size == 0        ) )
    {
      fprintf( stdout, "ERROR autfree heap and size = 0 !!!!\n" );
      autexit( 1 );
    }

    if ( ( ! AllocDebug->HEAP ) &&
         (   Size != 0        ) )
    {
      fprintf( stdout, "ERROR autfree block and size != 0 !!!!\n" );
      autexit( 1 );
    }

    if ( ( AllocDebug->HEAP         ) &&
         ( AllocDebug->SIZE != Size ) )
    {
      fprintf( stdout, "ERROR autfree heap mode and size %ld != %ld !!!!\n",
               AllocDebug->SIZE, Size );
      autexit( 1 );
    }

    memset( AllocDebug->POINTER, 0, AllocDebug->SIZE + sizeof( autallocdebug ) );

    free( Pointer );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Aut Free Debug                       |
|                                                             |
\------------------------------------------------------------*/

void freeautdebug( Debug )

  autdebug_list *Debug;
{
  autfreeheap( (char *)Debug, sizeof( autdebug_list ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Aut Free Hash Element                |
|                                                             |
\------------------------------------------------------------*/

void freeauthelem( Element )

  authelem *Element;
{
  autfreeblock( (char *)Element );
}

/*------------------------------------------------------------\
|                                                             |
|                        Aut Free Hash Table                  |
|                                                             |
\------------------------------------------------------------*/

void freeauthtable( HashTable )

  authtable *HashTable;
{
  autfreeblock( (char *)HashTable );
}

/*------------------------------------------------------------\
|                                                             |
|                        Aut Free Hash Element 2              |
|                                                             |
\------------------------------------------------------------*/

void freeauth2elem( Element )

  auth2elem *Element;
{
  autfreeblock( (char *)Element );
}

/*------------------------------------------------------------\
|                                                             |
|                        Aut Free Hash Table 2                |
|                                                             |
\------------------------------------------------------------*/

void freeauth2table( HashTable )

  auth2table *HashTable;
{
  autfreeblock( (char *)HashTable );
}
