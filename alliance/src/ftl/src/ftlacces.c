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
| Tool    :                     Ftl                           |
|                                                             |
| File    :                  ftlacces.c                       |
|                                                             |
| Date    :                   04.12.96                        |
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

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "ftl.h"

# include "ftlacces.h"
# include "ftlerror.h"

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

  char *FSM_IN;
  char *FSM_OUT;

  char *FSM_KISS_FORMAT = (char *)0;
  char *FSM_VHDL_FORMAT = (char *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            fsmenv                           |
|                                                             |
\------------------------------------------------------------*/

void fsmenv()
{
  FSM_IN = namealloc( mbkgetenv( "FSM_IN") );

  if ( FSM_IN == (char *)0 )
  {
    FSM_IN = namealloc( FTL_DEFAULT_IN );
  }
 
  FSM_OUT = namealloc( mbkgetenv( "FSM_OUT") );

  if ( FSM_OUT == (char *)0 )
  {
    FSM_OUT = namealloc( FTL_DEFAULT_OUT );
  }

  FSM_KISS_FORMAT = namealloc( "kiss2" );
  FSM_VHDL_FORMAT = namealloc( "fsm"   );
}

/*------------------------------------------------------------\
|                                                             |
|                          loadfsmfig                         |
|                                                             |
\------------------------------------------------------------*/

void loadfsmfig( Figure, Name )

  fsmfig_list *Figure;
  char        *Name;
{
  if ( FSM_IN == FSM_KISS_FORMAT ) 
  {
    kissloadfsmfig( Figure, Name );
  }
  else
  if ( FSM_IN == FSM_VHDL_FORMAT )
  {
    vhdlloadfsmfig( Figure, Name );
  }
  else
  {
    ftlerror( FTL_UNKNOWN_FSM_IN, FSM_IN );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          savefsmfig                         |
|                                                             |
\------------------------------------------------------------*/

void savefsmfig( Figure )

  fsmfig_list *Figure;
{
  if ( FSM_OUT == FSM_KISS_FORMAT )
  {
    kisssavefsmfig( Figure );
  }
  else
  if ( FSM_OUT == FSM_VHDL_FORMAT )
  {
    vhdlsavefsmfig( Figure );
  }
  else
  {
    ftlerror( FTL_UNKNOWN_FSM_OUT, FSM_OUT );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          getfsmfig                          |
|                                                             |
\------------------------------------------------------------*/

fsmfig_list *getfsmfig( Name )

   char *Name;
{
  fsmfig_list *Figure;

  Name = namealloc( Name );

  for ( Figure  = HEAD_FSMFIG;
        Figure != (fsmfig_list *)0;
        Figure  = Figure->NEXT )
  {
    if ( Figure->NAME == Name ) break;
  }

  if ( Figure == (fsmfig_list *)0 )
  {
    Figure = addfsmfig( Name );

    loadfsmfig( Figure, Name );
  }

  return( Figure );
}
