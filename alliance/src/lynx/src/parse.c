/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU General Public |
| License as  published by the Free Software Foundation;      |
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
| Tool    :                   LYNX                            |
|                                                             |
| File    :                  Parse.c                          |
|                                                             |
| Author  :               Jacomme Ludovic                     |
|                                                             |
| Date    :                  01.08.93                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>

# include "mut.h"
# include "mlo.h"
# include "mph.h"
# include "rds.h"

# include "parse.h"

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

  chain_list *LynxScanEqui      = (chain_list *)0;

  static char LynxBuffer[ 256 ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        IsLynxScanEqui                       |
|                                                             |
\------------------------------------------------------------*/

char IsLynxScanEqui( LynxEqui, Name )

   chain_list *LynxEqui;
   char       *Name;
{
  chain_list *Index;

  rdsbegin();

  for ( Index  = LynxEqui;
        Index != (chain_list *)0;
        Index  = Index->NEXT )
  {
    if ( (char *)(Index->DATA) == Name ) break;
  }

  rdsend();
  return ( Index != (chain_list *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                         LynxParseEqui                       |
|                                                             |
\------------------------------------------------------------*/

char  LynxParseEqui()

{
  FILE *LynxFile;

  rdsbegin();

  if ( ( LynxFile = fopen( "LYNX_EQUI", "r" ) ) == (FILE *)0 )
  {
    rdsend();
    return( 0 );
  }

  while (  fgets( LynxBuffer, LYNX_BUFFER_SIZE, LynxFile ) != (char *)0 )
  {
    LynxBuffer[ strlen( LynxBuffer ) - 1 ] = '\0';

    LynxScanEqui = addchain( LynxScanEqui,
                             (void *)namealloc( LynxBuffer ) );
  }

  fclose( LynxFile );

  rdsend();
  return ( LynxScanEqui != (chain_list *)0 );
}
