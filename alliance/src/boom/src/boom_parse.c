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
| Tool    :                     BOOM                          |
|                                                             |
| File    :                   boom_parse.c                    |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   06.06.00                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "abe.h"
# include "abv.h"
# include "abt.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "boom_error.h"
# include "boom_parse.h"
# include "boom_debug.h"
# include "boom_shared.h"
# include "boom_abl.h"

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
|                      BoomParseBehFigure                     |
|                                                             |
\------------------------------------------------------------*/

befig_list *BoomParseBehFigure( FileName )

  char *FileName;
{
  befig_list    *BehFigure;
  beaux_list    *BehAux;
  beout_list    *BehOut;
  bereg_list    *BehReg;
  bebux_list    *BehBux;
  bebus_list    *BehBus;

  BehFigure = vhdlloadbefig( (befig_list *)0, FileName, 2 );

  if ( BehFigure->ERRFLG )
  {
    fprintf( stderr, "\t### Unable to compile file %s.vbe\n", FileName );
    autexit( 1 );
  }

  BoomAddBehFigInfo( BehFigure );

  for ( BehAux  = BehFigure->BEAUX;
        BehAux != (beaux_list *)0;
        BehAux  = BehAux->NEXT )
  {
    BoomAddBehAuxInfo( BehAux );
  }

  for ( BehBux  = BehFigure->BEBUX;
        BehBux != (bebux_list *)0;
        BehBux  = BehBux->NEXT )
  {
    BoomAddBehBuxInfo( BehBux );
  }

  for ( BehBus  = BehFigure->BEBUS;
        BehBus != (bebus_list *)0;
        BehBus  = BehBus->NEXT )
  {
    BoomAddBehBusInfo( BehBus );
  }

  for ( BehOut  = BehFigure->BEOUT;
        BehOut != (beout_list *)0;
        BehOut  = BehOut->NEXT )
  {
    BoomAddBehOutInfo( BehOut );
  }

  for ( BehReg  = BehFigure->BEREG;
        BehReg != (bereg_list *)0;
        BehReg  = BehReg->NEXT )
  {
    BoomAddBehRegInfo( BehReg );
  }

  beh_frebeaux( BehFigure->BEDLY );
  BehFigure->BEDLY = (beaux_list *)0;

  BoomBehMapAbl( BehFigure, simpablexpr, 1 );

  return( BehFigure );
}
