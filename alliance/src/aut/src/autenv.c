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
| File    :                   autenv.c                        |
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
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>

# include <mut.h>
# include "aut.h"

# include "auterror.h"
# include "autenv.h"

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

     unsigned char AUT_EXIT_CORE   = 0;
     unsigned char AUT_ALLOC_DEBUG = 0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     Aut Exit Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Aut Exit                           |
|                                                             |
\------------------------------------------------------------*/

void autenv()
{
  if ( mbkgetenv( "AUT_EXIT_CORE" ) )
  {
    AUT_EXIT_CORE = 1;

    autwarning( AUT_EXIT_CORE_WARNING, 0 );
  }


  if ( mbkgetenv( "AUT_ALLOC_DEBUG" ) )
  {
    AUT_ALLOC_DEBUG = 1;

    autwarning( AUT_ALLOC_DEBUG_WARNING, 0 );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Aut Base Name                        |
|                                                             |
\------------------------------------------------------------*/

char *autbasename( Name, Extension )

  char *Name;
  char *Extension;
{
  int   Index;
  char *NewName;

  NewName = mbkstrdup( Name );
  Index   = strlen( NewName ) - (strlen( Extension ) + 1);

  if ( Index >= 0 )
  {
    if ( ! strcmp( NewName + Index + 1, Extension ) )
    {
      if ( NewName[ Index ] == '.' )
      {
        NewName[ Index ] = '\0';
      }
    }
  }

  return( NewName );
}
