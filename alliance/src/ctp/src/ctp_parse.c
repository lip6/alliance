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
| Tool    :                     CTP                           |
|                                                             |
| File    :                  ctp_parse.c                      |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   01.01.95                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include "mut.h"
# include "aut.h"
# include "vex.h"
# include "ctl.h"
# include "ctp.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

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

  extern FILE        *ctp_y_in;
  extern int          ctp_y_parse();
  extern char         CTP_ERRFLG;
  extern ctlfig_list *CTP_HEADFIG;
  extern char        *CTP_FILENAME;

/*------------------------------------------------------------\
|                                                             |
|                        vpnloadvpnfig                        |
|                                                             |
\------------------------------------------------------------*/

ctlfig_list *loadctlfig( InputFileName )

  char *InputFileName;
{
  int   Value;
  int   Index;

  ctp_y_in = mbkfopen( InputFileName, "ctl", "r" );

  if ( ctp_y_in == (FILE *)0 )
  {
    fprintf( stdout, "Error opening file %s\n", InputFileName );
    autexit( 1 );
  }

  CTP_FILENAME = InputFileName;

  Value = ctp_y_parse();

  fclose( ctp_y_in );

  if ( ( Value      ) ||
       ( CTP_ERRFLG ) )
  {
    fprintf( stdout, "Error parsing file %s\n", InputFileName );
    autexit( 1 );
  }

  return( CTP_HEADFIG );
}

