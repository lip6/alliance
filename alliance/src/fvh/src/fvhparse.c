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
| Tool    :                     FVH                           |
|                                                             |
| File    :                   fvhparse.c                      |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   04.12.96                        |
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
# include "fsm.h"
# include "fvh.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "fvherror.h"
# include "fvhfbh2fsm.h"
# include "fvhparse.h"

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

  extern FILE *fbl_y_in;
  extern char  FBL_ERRFLG;

  bddcircuit *FblBddCircuit = (bddcircuit *)0;
  bddsystem  *FblBddSystem  = (bddsystem  *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        vhdlloadfsmfig                       |
|                                                             |
\------------------------------------------------------------*/

static void loc_trap_signal()
{
  fprintf( stderr, "fvh: Internal error !\n" );
  autexit( 1 );
}

void vhdlloadfsmfig( FsmFigure, FigureName )

  fsmfig_list *FsmFigure;
  char        *FigureName;
{
  bddsystem   *SaveBddSystem;
  bddcircuit  *SaveBddCircuit;
  int          Error;
  void        (*PreviousHandler)();

  fbl_y_in = mbkfopen( FigureName, "fsm", "r" );

  if ( fbl_y_in == (FILE *)0 )
  {
    fvherror( FVH_ERROR_OPEN_FILE, FigureName, 0 );
  }

  SaveBddSystem  = BddLocalSystem;
  SaveBddCircuit = BddLocalCircuit;

  if ( FblBddSystem == (bddsystem *)0 )
  {
    FblBddSystem  = createbddsystem( 50, 1000, 200, 50000000 );
    FblBddCircuit = createbddcircuit( "-select-", 200, 0, FblBddSystem );
  }

  BddLocalSystem  = FblBddSystem;
  BddLocalCircuit = FblBddCircuit;

  PreviousHandler = signal( SIGSEGV, loc_trap_signal );
  Error = fbl_y_parse();
  signal( SIGSEGV, PreviousHandler );

  BddLocalSystem  = SaveBddSystem;
  BddLocalCircuit = SaveBddCircuit;

  if ( ( Error      ) ||
       ( FBL_ERRFLG ) )
  {
    fvherror( FVH_ERROR_PARSE_FILE, FigureName, 0 );
  }

  FvhFbh2Fsm( FBL_HEADFIG, FsmFigure );

  if ( ! IsFsmFigMixedRtl( FsmFigure ) )
  {
    fbh_delfbfig( FBL_HEADFIG, FBL_HEADFIG, 'Y' );
  }
}
