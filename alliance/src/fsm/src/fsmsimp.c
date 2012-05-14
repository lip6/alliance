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
| Tool    :                     Fsm                           |
|                                                             |
| File    :                   fsmsimp.c                       |
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

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "fsm.h"

# include <stdio.h>
# include "fsmsimp.h"
# include "fsmerror.h"

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
|                         simpfsmablexpr                      |
|                                                             |
\------------------------------------------------------------*/

void simpfsmablexpr( FsmFigure )

  fsmfig_list *FsmFigure;
{
  fsmstate_list  *ScanState;
  fsmtrans_list  *ScanTrans;
  fsmstack_list  *ScanStack;
  fsmlocout_list *ScanLocout;

  for ( ScanTrans  = FsmFigure->TRANS;
        ScanTrans != (fsmtrans_list *)0;
        ScanTrans  = ScanTrans->NEXT )
  {
    if ( ScanTrans->ABL != (ablexpr *)0 )
    {
      ScanTrans->ABL = simpablexpr( ScanTrans->ABL );
    }
  }

  for ( ScanStack  = FsmFigure->STACK;
        ScanStack != (fsmstack_list *)0;
        ScanStack  = ScanStack->NEXT )
  {
    if ( ScanStack->ABL != (ablexpr *)0 )
    {
      ScanStack->ABL = simpablexpr( ScanStack->ABL );
    }
  }

  for ( ScanState  = FsmFigure->STATE;
        ScanState != (fsmstate_list *)0;
        ScanState  = ScanState->NEXT )
  {
    for ( ScanLocout  = ScanState->LOCOUT;
          ScanLocout != (fsmlocout_list *)0;
          ScanLocout  = ScanLocout->NEXT )
    {
      if ( ScanLocout->ABL != (ablexpr *)0 )
      {
        ScanLocout->ABL = simpablexpr( ScanLocout->ABL );
      }

      if ( ScanLocout->ABL_DC != (ablexpr *)0 )
      {
        ScanLocout->ABL_DC = simpablexpr( ScanLocout->ABL_DC );
      }
    }
  }

  if ( FsmFigure->CLOCK_ABL != (ablexpr *)0 )
  {
    FsmFigure->CLOCK_ABL = simpablexpr( FsmFigure->CLOCK_ABL );
  }
}
