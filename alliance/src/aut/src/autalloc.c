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
| Tool    :                     Aut                           |
|                                                             |
| File    :                  autalloc.c                       |
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
# include <stdlib.h>
# include <memory.h>
# include <string.h>

# include <mut.h>
# include "aut.h"

# include "autalloc.h"
# include "autenv.h"
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

  char  *AutHeapHeadArray[ 256 ];
  char   InitAutHeapHeadArray = 1;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Alloc Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Aut Alloc                          |
|                                                             |
\------------------------------------------------------------*/

char *autalloc( Size, Heap )

   unsigned int Size; 
   char         Heap;
{
  int           Counter;
  unsigned int  Index;
  int           HeapSize;
  char         *Pointer;
  char        **Prev;
  char         *Scan;

  if ( ( Heap != AUT_ALLOC_BLOCK ) &&
       ( Heap != AUT_ALLOC_HEAP  ) )
  {
    auterror( AUT_ALLOC_ERROR, (char *)0 );
  }

  if ( Size == 0 )
  {
    auterror( AUT_ALLOC_SIZE_ERROR, (char *)0 );
  }

  if ( ! AUT_ALLOC_DEBUG )
  {
    if ( InitAutHeapHeadArray == 1 )
    { 
      InitAutHeapHeadArray = 0;
  
      for ( Counter = 0; Counter < 256 ; Counter++ )
      {
        AutHeapHeadArray[ Counter ] = (char *)0;
      }
    }
  
    Index = ( Size + 3 ) >> 2;
  
    if ( ( Heap    == AUT_ALLOC_BLOCK ) ||
         ( Size    <  sizeof( char *) ) ||
         ( Index   >  255             ) )
    {
      if ( ! ( Pointer = malloc( Size ) ) ) 
      {
        auterror( AUT_ALLOC_ERROR, (char *)0 );
      }
  
      memset( Pointer, 0, Size );
    
      return( Pointer );
    }
  
    Size = Index << 2;
  
    if ( AutHeapHeadArray[ Index ] == (char *)0 )
    {
      HeapSize = ( Size << AUT_ALLOC_SHIFT );
  
      if ( ! ( Pointer = malloc( HeapSize ) ) ) 
      {
        auterror( AUT_ALLOC_ERROR, (char *)0 );
      }
  
      memset( Pointer, 0, HeapSize );
  
      Scan = (char *)(Pointer + Size);
  
      Prev = &AutHeapHeadArray[ Index ];
  
      for ( Counter = 1; Counter < AUT_ALLOC_SIZE; Counter++ )
      {
        *Prev = Scan; Prev = (char **)Scan; Scan = (char *)(Scan + Size);
      }
  
      *Prev = (char *)0;
  
      AutHeapHeadArray[ Index ] = (char *)(Pointer + Size);
  
      return( Pointer );
    }
  
    Pointer = AutHeapHeadArray[ Index ];
  
    AutHeapHeadArray[ Index ] = *((char **)AutHeapHeadArray[ Index ]);
  
    *(char **)(Pointer ) = (char *)0;
  
    return( Pointer );
  }
  else
  {
    if ( ! ( Pointer = malloc( Size + sizeof(autallocdebug) ) ) ) 
    {
      auterror( AUT_ALLOC_ERROR, (char *)0 );
    }
  
    {
      autallocdebug *AllocDebug;
  
      AllocDebug = (autallocdebug *)Pointer;
      AllocDebug->POINTER = AllocDebug;
      AllocDebug->SIZE    = Size;
      AllocDebug->HEAP    = Heap;
  
      Pointer += sizeof( autallocdebug );
    }
  
    memset( Pointer, 0, Size );
    
    return( Pointer );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Aut Alloc Debug                      |
|                                                             |
\------------------------------------------------------------*/

autdebug_list *allocautdebug()
{
  return( (autdebug_list *)(autallocheap( sizeof( autdebug_list ) ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Aut Alloc Hash Element               |
|                                                             |
\------------------------------------------------------------*/

authelem *allocauthelem( Number )

  unsigned int Number;
{
  return( (authelem  *)autallocblock( sizeof( authelem ) * Number ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Aut Alloc Hash Table                 |
|                                                             |
\------------------------------------------------------------*/

authtable *allocauthtable()
{
  return( (authtable  *)autallocblock( sizeof( authtable ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Aut Alloc Hash Element               |
|                                                             |
\------------------------------------------------------------*/

auth2elem *allocauth2elem( Number )

  unsigned int Number;
{
  return( (auth2elem  *)autallocblock( sizeof( auth2elem ) * Number ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Aut Alloc Hash Table                 |
|                                                             |
\------------------------------------------------------------*/

auth2table *allocauth2table()
{
  return( (auth2table  *)autallocblock( sizeof( auth2table ) ) );
}
