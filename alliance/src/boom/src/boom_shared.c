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
| Tool    :                    BOOM                           |
|                                                             |
| File    :                 boom_shared.c                     |
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

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "abe.h"
# include "abv.h"
# include "abt.h"

# include "boom_error.h"
# include "boom_shared.h"

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

  int   BoomFlagDebug   = 0;
  int   BoomFlagOption  = 0;

  char *BoomOptimAlgorithmName[ BOOM_MAX_OPTIM_ALGORITHM ] =
  {
    "top bdd reorder",
    "window bdd reorder",
    "simple bdd reorder",
    "one pass",
    "random bdd reorder",
    "procrastination",
    "Luc Burgun",
    "no optimization algorithm",
    "gradient",
    "just do it",
    "simulated annealing"
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      BoomAddBehAuxInfo                      |
|                                                             |
\------------------------------------------------------------*/

boombeauxinfo *BoomAddBehAuxInfo( BehAux )

   beaux_list *BehAux;
{
  boombeauxinfo *AuxInfo;

  AuxInfo = (boombeauxinfo *)autallocheap( sizeof( boombeauxinfo ) );

  SetBoomBehAuxInfo( BehAux, AuxInfo );

  return( AuxInfo );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomDelBehAuxInfo                      |
|                                                             |
\------------------------------------------------------------*/

void BoomDelBehAuxInfo( BehAux )

   beaux_list *BehAux;
{
  boombeauxinfo *AuxInfo;

  AuxInfo = GetBoomBehAuxInfo( BehAux );

  if ( AuxInfo != (boombeauxinfo *)0 )
  {
    autfreeheap( AuxInfo, sizeof( boombeauxinfo ) );
    SetBoomBehAuxInfo( BehAux, NULL );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomBehFreeBehAux                      |
|                                                             |
\------------------------------------------------------------*/

void BoomBehFreeBehAux( BehAux )

  beaux_list *BehAux;
{
   beaux_list *ScanBehAux;

   for ( ScanBehAux  = BehAux;
         ScanBehAux != (beaux_list *)0;
         ScanBehAux  = ScanBehAux->NEXT )
   {
     BoomDelBehAuxInfo( ScanBehAux );
   }

   beh_frebeaux( BehAux );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomAddBehOutInfo                      |
|                                                             |
\------------------------------------------------------------*/

boombeoutinfo *BoomAddBehOutInfo( BehOut )

   beout_list *BehOut;
{
  boombeoutinfo *OutInfo;

  OutInfo = (boombeoutinfo *)autallocheap( sizeof( boombeoutinfo ) );

  SetBoomBehOutInfo( BehOut, OutInfo );

  return( OutInfo );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomDelBehOutInfo                      |
|                                                             |
\------------------------------------------------------------*/

void BoomDelBehOutInfo( BehOut )

   beout_list *BehOut;
{
  boombeoutinfo *OutInfo;

  OutInfo = GetBoomBehOutInfo( BehOut );

  if ( OutInfo != (boombeoutinfo *)0 )
  {
    autfreeheap( OutInfo, sizeof( boombeoutinfo ) );
    SetBoomBehOutInfo( BehOut, NULL );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomBehFreeBehOut                      |
|                                                             |
\------------------------------------------------------------*/

void BoomBehFreeBehOut( BehOut )

  beout_list *BehOut;
{
   beout_list *ScanBehOut;

   for ( ScanBehOut  = BehOut;
         ScanBehOut != (beout_list *)0;
         ScanBehOut  = ScanBehOut->NEXT )
   {
     BoomDelBehOutInfo( ScanBehOut );
   }

   beh_frebeout( BehOut );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomAddBehRegInfo                      |
|                                                             |
\------------------------------------------------------------*/

boombereginfo *BoomAddBehRegInfo( BehReg )

   bereg_list *BehReg;
{
  boombereginfo *RegInfo;

  RegInfo = (boombereginfo *)autallocheap( sizeof( boombereginfo ) );

  SetBoomBehRegInfo( BehReg, RegInfo );

  return( RegInfo );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomDelBehRegInfo                      |
|                                                             |
\------------------------------------------------------------*/

void BoomDelBehRegInfo( BehReg )

   bereg_list *BehReg;
{
  boombereginfo *RegInfo;

  RegInfo = GetBoomBehRegInfo( BehReg );

  if ( RegInfo != (boombereginfo *)0 )
  {
    autfreeheap( RegInfo, sizeof( boombereginfo ) );
    SetBoomBehRegInfo( BehReg, NULL );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomBehFreeBehReg                      |
|                                                             |
\------------------------------------------------------------*/

void BoomBehFreeBehReg( BehReg )

  bereg_list *BehReg;
{
   bereg_list *ScanBehReg;

   for ( ScanBehReg  = BehReg;
         ScanBehReg != (bereg_list *)0;
         ScanBehReg  = ScanBehReg->NEXT )
   {
     BoomDelBehRegInfo( ScanBehReg );
   }

   beh_frebereg( BehReg );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomAddBehBusInfo                      |
|                                                             |
\------------------------------------------------------------*/

boombebusinfo *BoomAddBehBusInfo( BehBus )

   bebus_list *BehBus;
{
  boombebusinfo *BusInfo;

  BusInfo = (boombebusinfo *)autallocheap( sizeof( boombebusinfo ) );

  SetBoomBehBusInfo( BehBus, BusInfo );

  return( BusInfo );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomDelBehBusInfo                      |
|                                                             |
\------------------------------------------------------------*/

void BoomDelBehBusInfo( BehBus )

   bebus_list *BehBus;
{
  boombebusinfo *BusInfo;

  BusInfo = GetBoomBehBusInfo( BehBus );

  if ( BusInfo != (boombebusinfo *)0 )
  {
    autfreeheap( BusInfo, sizeof( boombebusinfo ) );
    SetBoomBehBusInfo( BehBus, NULL );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomBehFreeBehBus                      |
|                                                             |
\------------------------------------------------------------*/

void BoomBehFreeBehBus( BehBus )

  bebus_list *BehBus;
{
   bebus_list *ScanBehBus;

   for ( ScanBehBus  = BehBus;
         ScanBehBus != (bebus_list *)0;
         ScanBehBus  = ScanBehBus->NEXT )
   {
     BoomDelBehBusInfo( ScanBehBus );
   }

   beh_frebebus( BehBus );
}


/*------------------------------------------------------------\
|                                                             |
|                      BoomAddBehBuxInfo                      |
|                                                             |
\------------------------------------------------------------*/

boombebuxinfo *BoomAddBehBuxInfo( BehBux )

   bebux_list *BehBux;
{
  boombebuxinfo *BuxInfo;

  BuxInfo = (boombebuxinfo *)autallocheap( sizeof( boombebuxinfo ) );

  SetBoomBehBuxInfo( BehBux, BuxInfo );

  return( BuxInfo );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomDelBehBuxInfo                      |
|                                                             |
\------------------------------------------------------------*/

void BoomDelBehBuxInfo( BehBux )

   bebux_list *BehBux;
{
  boombebuxinfo *BuxInfo;

  BuxInfo = GetBoomBehBuxInfo( BehBux );

  if ( BuxInfo != (boombebuxinfo *)0 )
  {
    autfreeheap( BuxInfo, sizeof( boombebuxinfo ) );
    SetBoomBehBuxInfo( BehBux, NULL );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomBehFreeBehBux                      |
|                                                             |
\------------------------------------------------------------*/

void BoomBehFreeBehBux( BehBux )

  bebux_list *BehBux;
{
   bebux_list *ScanBehBux;

   for ( ScanBehBux  = BehBux;
         ScanBehBux != (bebux_list *)0;
         ScanBehBux  = ScanBehBux->NEXT )
   {
     BoomDelBehBuxInfo( ScanBehBux );
   }

   beh_frebebux( BehBux );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomAddBehFigInfo                      |
|                                                             |
\------------------------------------------------------------*/

boombefiginfo *BoomAddBehFigInfo( BehFig )

   befig_list *BehFig;
{
  boombefiginfo *FigInfo;

  FigInfo = (boombefiginfo *)autallocheap( sizeof( boombefiginfo ) );

  SetBoomBehFigInfo( BehFig, FigInfo );

  return( FigInfo );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomDelBehFigInfo                      |
|                                                             |
\------------------------------------------------------------*/

void BoomDelBehFigInfo( BehFig )

   befig_list *BehFig;
{
  boombefiginfo *FigInfo;

  FigInfo = GetBoomBehFigInfo( BehFig );

  if ( FigInfo != (boombefiginfo *)0 )
  {
    freechain( FigInfo->KEEP_LIST );
    freechain( FigInfo->DONT_TOUCH_LIST );
    freechain( FigInfo->BDD_ORDER_LIST );

    autfreeheap( FigInfo, sizeof( boombefiginfo ) );
    SetBoomBehFigInfo( BehFig, NULL );
  }
}


/*------------------------------------------------------------\
|                                                             |
|                      BoomBehFreeBehFig                      |
|                                                             |
\------------------------------------------------------------*/

void BoomBehFreeBehFig( BehFig )

  befig_list *BehFig;
{
  BoomBehFreeBehReg( BehFig->BEREG );
  BoomBehFreeBehOut( BehFig->BEOUT );
  BoomBehFreeBehAux( BehFig->BEAUX );
  BoomBehFreeBehBus( BehFig->BEBUS );
  BoomBehFreeBehBux( BehFig->BEBUX );

  BehFig->BEOUT = (beout_list *)0;
  BehFig->BEREG = (bereg_list *)0;
  BehFig->BEAUX = (beaux_list *)0;
  BehFig->BEBUS = (bebus_list *)0;
  BehFig->BEBUX = (bebux_list *)0;

  BoomDelBehFigInfo( BehFig );

  BehFig->NEXT = (befig_list *)0;
  beh_frebefig( BehFig );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomComputeLog2                        |
|                                                             |
\------------------------------------------------------------*/

long BoomComputeLog2( Value )

  unsigned long Value;
{
  unsigned long Log2;

  Log2 = 0;
  
  while ( Value != 0 )
  {
    Value = Value >> 1;

    Log2++;
  }

  if ( Log2 > 0 ) Log2--;

  return( Log2 );
}
