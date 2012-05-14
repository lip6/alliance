/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     MOCHA                         |
|                                                             |
| File    :                  mocha_beh.c                      |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   07.08.02                        |
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
# include "btr.h"
# include "fsm.h"
# include "ftl.h"
# include "abe.h"
# include "abv.h"
# include "vex.h"
# include "ctl.h"
# include "ctp.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "mocha_debug.h"
# include "mocha_shared.h"
# include "mocha_beh.h"

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
|                     MochaPostTreatVerifyBeh                 |
|                                                             |
\------------------------------------------------------------*/

void MochaPostTreatVerifyBeh( MochaFigure, FlagVerbose )

  mochafig_list *MochaFigure;
  int            FlagVerbose;
{
  befig_list  *BehFigure;
  berin_list **PrevBehRin;
  berin_list  *DelBehRin;
  berin_list  *BehRin;
  beaux_list  *BehAux;
  beout_list  *BehOut;
  bereg_list  *BehReg;
  authtable   *BehAuxHashTable;
  authtable   *BehRegHashTable;
  authtable   *BehOutHashTable;
  authelem    *Element;

  BehFigure = MochaFigure->BEH_FIGURE;

  /* TO BE DONE */

  if ( ( BehFigure->BEBUS != (bebus_list *)0 ) ||
       ( BehFigure->BEBUX != (bebux_list *)0 ) )
  {
    MochaPrintf( stdout, "Bus signals are not supported !\n" );
    autexit( 1 );
  }

  /* VERIFY: TO BE DONE */

  BehAuxHashTable = createauthtable( 50 );
  BehRegHashTable = createauthtable( 50 );
  BehOutHashTable = createauthtable( 50 );

  for ( BehAux  = BehFigure->BEAUX;
        BehAux != (beaux_list *)0;
        BehAux  = BehAux->NEXT )
  {
    addauthelem( BehAuxHashTable, BehAux->NAME, (long)BehAux );
  }

  for ( BehReg  = BehFigure->BEREG;
        BehReg != (bereg_list *)0;
        BehReg  = BehReg->NEXT )
  {
    addauthelem( BehRegHashTable, BehReg->NAME, (long)BehReg );
  }

  for ( BehOut  = BehFigure->BEOUT;
        BehOut != (beout_list *)0;
        BehOut  = BehOut->NEXT )
  {
    addauthelem( BehOutHashTable, BehOut->NAME, (long)BehOut );
  }

  PrevBehRin = &BehFigure->BERIN;
  BehRin     = BehFigure->BERIN;

  while ( BehRin != (berin_list *)0 )
  {
    Element = searchauthelem( BehAuxHashTable, BehRin->NAME );
    if ( Element != (authelem *)0 )
    {
      DelBehRin    = BehRin;
      BehRin       = BehRin->NEXT;

      *PrevBehRin     = DelBehRin->NEXT;
      DelBehRin->NEXT = (berin_list *)0;

      beh_rmvberin( DelBehRin, DelBehRin->NAME );
    }
    else
    {
      PrevBehRin = &BehRin->NEXT;
      BehRin     = BehRin->NEXT;
    }
  }

  MochaFigure->HASH_BEH_AUX = BehAuxHashTable;
  MochaFigure->HASH_BEH_REG = BehRegHashTable;
  MochaFigure->HASH_BEH_OUT = BehOutHashTable;
}


/*------------------------------------------------------------\
|                                                             |
|                        MochaCompileBeh                      |
|                                                             |
\------------------------------------------------------------*/

void MochaCompileBeh( MochaFigure, FileName, FlagVerbose )

  mochafig_list *MochaFigure;
  char          *FileName;
  int            FlagVerbose;
{
  befig_list  *BehFigure;

  BehFigure = vhdlloadbefig( (befig_list *)0, FileName, 0 );

  MochaFigure->BEH_FIGURE = BehFigure;
}
