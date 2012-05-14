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
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     Aut                           |
|                                                             |
| File    :                  autadebug.c                      |
|                                                             |
| Date    :                   03.12.96                        |
|                                                             |
| Author  :                Jacomme Ludovic                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <signal.h>

# include <mut.h>
# include "aut.h"

# include "autdebug.h"
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

 autdebug_list *HEAD_AUTDEBUG = (autdebug_list *)0;
 char           AUT_DEBUG_ON  = 0;

/*------------------------------------------------------------\
|                                                             |
|                          Privates                           |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          trapautdebug                       |
|                                                             |
\------------------------------------------------------------*/

void trapautdebug()
{
  autdebug_list *ScanDebug;

  for ( ScanDebug  = HEAD_AUTDEBUG;
        ScanDebug != (autdebug_list *)0;
        ScanDebug  = ScanDebug->NEXT )
  {
    fprintf( stdout, "autdebug: file %s line %d\n", 
             ScanDebug->NAME, ScanDebug->LINE );
  }

  signal( SIGQUIT, trapautdebug );
  signal( SIGSEGV, SIG_DFL      );
  signal( SIGBUS , SIG_DFL      );
  signal( SIGILL , SIG_DFL      );
}

/*------------------------------------------------------------\
|                                                             |
|                          autdebug                           |
|                                                             |
\------------------------------------------------------------*/

void autdebug()
{
  signal( SIGSEGV, trapautdebug );
  signal( SIGBUS,  trapautdebug );
  signal( SIGILL,  trapautdebug );
  signal( SIGQUIT, trapautdebug );

  AUT_DEBUG_ON = 1;
}

/*------------------------------------------------------------\
|                                                             |
|                          addautdebug                        |
|                                                             |
\------------------------------------------------------------*/

void addautdebug( Line, File )

  int   Line;
  char *File;
{
  autdebug_list *NewDebug;

  NewDebug       = allocautdebug();
  NewDebug->NEXT = HEAD_AUTDEBUG;
  NewDebug->LINE = Line;
  NewDebug->NAME = File;
  HEAD_AUTDEBUG  = NewDebug;
}

/*------------------------------------------------------------\
|                                                             |
|                          delautdebug                        |
|                                                             |
\------------------------------------------------------------*/

void delautdebug()
{
  autdebug_list *DelDebug;

  DelDebug = HEAD_AUTDEBUG;

  if ( DelDebug != (autdebug_list *)0 )
  {
    HEAD_AUTDEBUG = DelDebug->NEXT;
    freeautdebug( DelDebug );
  }
}
