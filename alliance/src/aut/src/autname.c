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
| File    :                   autname.c                       |
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


# include "autname.h"
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

  static chain_list *AutNameHashTable[ AUT_NAME_HASH_SIZE ];
  static char        AutNameBuffer[ AUT_NAME_BUFFER_SIZE ]; 

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Aut Name Alloc                     |
|                                                             |
\------------------------------------------------------------*/

char *autnamealloc( Name )

   char *Name;
{
  static char *LastName1 = (char *)0;
  static char *LastName2 = (char *)0;

  chain_list *ScanChain;
  char       *SourceName;
  char       *TargetName;
  int         HashKey;
  
  if ( Name != (char *)0 )
  {
    if ( Name == LastName1 ) return( LastName1 );
    else
    if ( Name == LastName2 ) return( LastName2 );

    HashKey    = 0;
    TargetName = AutNameBuffer;
    SourceName = Name;

    while ( *SourceName != '\0' ) 
    {
      *TargetName = *SourceName;

      HashKey += ( HashKey ^ ( HashKey >> 1 ) );
      HashKey += AUT_NAME_HASH_MULT * (unsigned char)*TargetName;

      TargetName++;
      SourceName++;

      while ( HashKey >= AUT_NAME_HASH_PRIME )
      {
        HashKey -= AUT_NAME_HASH_PRIME;
      }
    }

    *TargetName = '\0';
    HashKey %= AUT_NAME_HASH_SIZE;

    for ( ScanChain  = AutNameHashTable[ HashKey ];
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      if (! strcmp( AutNameBuffer, (char *)ScanChain->DATA ) )
      {
        Name = (char *)ScanChain->DATA; break;
      }
    }

    if ( ScanChain == (chain_list *)0 )
    {
      Name = mbkstrdup( AutNameBuffer );
      AutNameHashTable[ HashKey ] = addchain( AutNameHashTable[ HashKey ], (void *)Name );
    }

    LastName1 = LastName2;
    LastName2 = Name;
  }

  return( Name );
}
