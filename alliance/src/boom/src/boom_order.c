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
| Tool    :                     Boom                          |
|                                                             |
| File    :                  boom_order.c                     |
|                                                             |
| Date    :                   06.06.00                        |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
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
# include "abt.h"

# include <stdio.h>
# include "boom_error.h"
# include "boom_shared.h"
# include "boom_order.h"

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
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Boom Beh Make Bdd Order               |
|                                                             |
\------------------------------------------------------------*/

void BoomBehMakeBddOrder( BehFigure, OrderFlag, DontCareFlag  )

  befig_list *BehFigure;
  int         OrderFlag;
  int         DontCareFlag;
{
  berin_list    *BehRin;
  boombefiginfo *FigInfo;
  chain_list    *ScanOrder;
  bddindex       BddIndex;
  authtable     *HahsRin;
  authelem      *Element;
  /*
  beaux_list  *BehAux;
  beout_list  *BehOut;
  bebus_list  *BehBus;
  bebux_list  *BehBux;
  bereg_list  *BehReg;
  biabl_list  *BiAbl;
  */

  FigInfo = GetBoomBehFigInfo( BehFigure ); 

  HahsRin = createauthtable( 100 );

  for ( BehRin  = BehFigure->BERIN;
        BehRin != (berin_list *)0;
        BehRin  = BehRin->NEXT )
  {
    addauthelem( HahsRin, BehRin->NAME, (long)BehRin );
  }

  for ( ScanOrder  = FigInfo->BDD_ORDER_LIST;
        ScanOrder != (chain_list *)0;
        ScanOrder  = ScanOrder->NEXT )
  {
    Element = searchauthelem( HahsRin, (char *)ScanOrder->DATA );

    if ( Element != (authelem *)0 )
    {
      BehRin   = (berin_list *)Element->VALUE;
      SetBoomBehRinOrder( BehRin );

      BddIndex = BddLocalCircuit->NUMBER_NAME_IN + BDD_INDEX_MIN;
      addbddcircuitin( (bddcircuit *)0, BehRin->NAME, BddIndex,
                       BDD_IN_MODE_IMPOSE | BDD_IN_MODE_LAST );
    }
  }

  destroyauthtable( HahsRin );

  if ( OrderFlag )
  {
    BehFigure->BERIN = (berin_list *)reverse( (chain_list *)BehFigure->BERIN );
  }

  for ( BehRin  = BehFigure->BERIN;
        BehRin != (berin_list *)0;
        BehRin  = BehRin->NEXT )
  {
    if ( IsBoomBehRinOrder( BehRin ) ) continue;

    BddIndex = BddLocalCircuit->NUMBER_NAME_IN + BDD_INDEX_MIN;
    addbddcircuitin( (bddcircuit *)0, BehRin->NAME, BddIndex,
                     BDD_IN_MODE_IMPOSE | BDD_IN_MODE_LAST );
  }

  if ( OrderFlag )
  {
    BehFigure->BERIN = (berin_list *)reverse( (chain_list *)BehFigure->BERIN );
  }

  if ( DontCareFlag )
  {
     FigInfo->BDD_DONT_CARE = addbddcircuitin( (bddcircuit *)0, 
                                                ABL_ATOM_NAME_DC, 0, BDD_IN_MODE_LAST );
  }
}
