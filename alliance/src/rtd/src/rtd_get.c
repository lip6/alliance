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
| Tool    :                     Vtl                           |
|                                                             |
| File    :                  rtd_get.c                        |
|                                                             |
| Date    :                   04.07.96                        |
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
# include <unistd.h>

# include MUT_H
# include AUT_H
# include VEX_H
# include RTN_H
# include RTD_H

# include "rtd_get.h"
# include "rtd_error.h"

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

  char *RTL_IN;
  char *RTL_OUT;

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
|                            rtlenv                           |
|                                                             |
\------------------------------------------------------------*/

void rtlenv()
{
}

/*------------------------------------------------------------\
|                                                             |
|                          getrtlfig                          |
|                                                             |
\------------------------------------------------------------*/

rtlfig_list *getrtlfig( Name )

   char *Name;
{
  rtlfig_list *Figure;
  char        *FigName;

  FigName = namealloc( Name );

  for ( Figure  = HEAD_RTLFIG;
        Figure != (rtlfig_list *)0;
        Figure  = Figure->NEXT )
  {
    if ( Figure->NAME == FigName ) break;
  }

  if ( Figure == (rtlfig_list *)0 )
  {
    Figure = addrtlfig( Name );

    loadrtlfig( Figure, Name );
  }

  return( Figure );
}
