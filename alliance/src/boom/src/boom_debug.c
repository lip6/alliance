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
| Tool    :                    BOOM                           |
|                                                             |
| File    :                 boom_debug.c                      |
|                                                             |
| Authors :                Jacomme Ludovic                    |
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
# include <string.h>

# include MUT_H
# include AUT_H
# include ABL_H
# include BDD_H
# include ABE_H
# include ABV_H
# include ABT_H

# include "boom_error.h"
# include "boom_debug.h"
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

   int           BoomDebugFlag   = 0;
   int           BoomDebugOption = 0;
   time_t        BoomDebugChrono[ BOOM_DEBUG_MAX_CHRONO ];

/*------------------------------------------------------------\
|                                                             |
|                      Private variables                      |
|                                                             |
\------------------------------------------------------------*/

  static char *BoomDebugItemName[ ] =
  {
    "boom_abl.c",
    "boom_anneal.c",
    "boom_aux.c",
    "boom_auxinit.c",
    "boom_bdd.c",
    "boom_burgun.c",
    "boom_check.c",
    "boom_cost.c",
    "boom_dc.c",
    "boom_debug.c",
    "boom_drive.c",
    "boom_error.c",
    "boom_gradient.c",
    "boom_hash.c",
    "boom_main.c",
    "boom_one.c",
    "boom_optim.c",
    "boom_order.c",
    "boom_param.c",
    "boom_parse.c",
    "boom_procrast.c",
    "boom_random.c",
    "boom_shared.c",
    "boom_simple.c",
    "boom_simul.c",
    "boom_top.c",
    "boom_window.c",
    (char *)0
  };

  static char *BoomDebugItemFlag = (char *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       BoomDebugSetOption                    |
|                                                             |
\------------------------------------------------------------*/

int BoomDebugSetOption( Name )

  char *Name;
{
  /*
  if ( ! strcmp( Name, "no_redinst" ) )
  {
    SetBoomDebugNoRedInst(); return( 1 );
  }
  else
  if ( ! strcmp( Name, "stat" ) )
  {
    SetBoomDebugStatistics(); return( 1 );
  }
  else
  if ( ! strcmp( Name, "stdout" ) )
  {
    SetBoomDebugDriveStdout(); return( 1 );
  }
  */
  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                       BoomDebugAddItem                      |
|                                                             |
\------------------------------------------------------------*/

int BoomDebugAddItem( Name, Mode )

  char *Name;
  int   Mode;
{
  char  Buffer[ 64 ];
  char *Item;
  int   Index;

  if ( BoomDebugItemFlag == (char *)0 )
  {
    Index = 0; 

    while ( BoomDebugItemName[ Index ] != (char *)0 )
    {
      BoomDebugItemName[ Index ] = namealloc( BoomDebugItemName[ Index ] );

      Index++;
    }

    BoomDebugItemFlag = autallocblock( Index );
  }

  if ( ! strcmp( Name, "all" ) )
  {
    BoomDebugFlag |= Mode;

    Index = 0;

    while ( BoomDebugItemName[ Index ] != (char *)0 )
    {
      BoomDebugItemFlag[ Index ] |= Mode;

      Index++;
    }

    return( 1 );
  }
  else
  {
    sprintf( Buffer, "boom_%s.c", Name );
    Item  = namealloc( Buffer );
    Index = 0; 

    while ( BoomDebugItemName[ Index ] != (char *)0 )
    {
      if ( Item == BoomDebugItemName[ Index ] )
      {
        BoomDebugItemFlag[ Index ] |= Mode;
        BoomDebugFlag |= Mode;

        return( 1 );
      }
  
      Index++;
    }
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomDebugSearchItem                    |
|                                                             |
\------------------------------------------------------------*/

int BoomDebugSearchItem( Item, Mode )

  char *Item;
  int   Mode;
{
  int Index;

  if ( ( Mode & BoomDebugFlag ) == Mode )
  {
    Item  = namealloc( Item );
    Index = 0; 

    while ( BoomDebugItemName[ Index ] != (char *)0 )
    {
      if ( Item == BoomDebugItemName[ Index ] )
      {
        return( ( BoomDebugItemFlag[ Index ] & Mode ) == Mode );
      }

      Index++;
    }
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                       BoomDebugLocSaveBehCost               |
|                                                             |
\------------------------------------------------------------*/

static void BoomDebugLocSaveBehCost( BehFigure, FileName )

  befig_list *BehFigure;
  char       *FileName;
{
  FILE          *XscFile;
  beaux_list    *BehAux;
  beout_list    *BehOut;
  bereg_list    *BehReg;
  boombeauxinfo *AuxInfo;
  boombeoutinfo *OutInfo;
  boombereginfo *RegInfo;
  long           Layer;

  XscFile = mbkfopen( BehFigure->NAME, "xsc", "w" );

  if ( XscFile != (FILE *)0 )
  {
    for ( BehAux  = BehFigure->BEAUX;
          BehAux != (beaux_list *)0;
          BehAux  = BehAux->NEXT )
    {
      AuxInfo = GetBoomBehAuxInfo( BehAux );

      if ( AuxInfo != (boombeauxinfo *)0 ) 
      {
        Layer = AuxInfo->LITERAL_COST;
        if ( Layer > 31 ) Layer = 32;

        fprintf( XscFile, "B:beaux_%s:%ld:LITERAL %ld DELAY %ld\n",
                 BehAux->NAME, Layer, AuxInfo->LITERAL_COST, AuxInfo->DELAY_COST );
      }
    }

    for ( BehOut  = BehFigure->BEOUT;
          BehOut != (beout_list *)0;
          BehOut  = BehOut->NEXT )
    {
      OutInfo = GetBoomBehOutInfo( BehOut );

      if ( OutInfo != (boombeoutinfo *)0 ) 
      {
        Layer = OutInfo->LITERAL_COST;
        if ( Layer > 31 ) Layer = 32;

        fprintf( XscFile, "B:beout_%s:%ld:LITERAL %ld DELAY %ld\n",
                 BehOut->NAME, Layer, OutInfo->LITERAL_COST, OutInfo->DELAY_COST );
      }
    }

    for ( BehReg  = BehFigure->BEREG;
          BehReg != (bereg_list *)0;
          BehReg  = BehReg->NEXT )
    {
      RegInfo = GetBoomBehRegInfo( BehReg );

      if ( RegInfo != (boombereginfo *)0 ) 
      {
        Layer = RegInfo->LITERAL_COST;
        if ( Layer > 31 ) Layer = 32;

        fprintf( XscFile, "B:bereg_%s:%ld:LITERAL %ld DELAY %ld\n",
                 BehReg->NAME, Layer, RegInfo->LITERAL_COST, RegInfo->DELAY_COST );
      }
    }

    fclose( XscFile );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       BoomDebugLocSaveBehFigure             |
|                                                             |
\------------------------------------------------------------*/

void BoomDebugLocSaveBehFigure( BehFigure, Text, FileName, Line )

  befig_list *BehFigure;
  char       *Text;
  char       *FileName;
  int         Line;
{
  static char Buffer[ 128 ];
  static int  Number = 0;

  char       *Name;
  int         Length;

  Name   = BehFigure->NAME;

  sprintf( Buffer, "beh%03d_%04d", Number++, Line );
  Length = strlen( Buffer );

  FileName = FileName + 4;
  strcpy( Buffer + Length, FileName );
  Length += strlen( FileName );

  if ( Text != (char *)0 )
  {
    strcpy( &Buffer[ Length - 2 ], Text );
  }
  else
  {
    Buffer[ Length - 2 ] = '\0';
  }

  BehFigure->NAME = namealloc( Buffer );

  fprintf( stdout, "  +++ BoomDebugSaveBehFigure %s.vbe\n", BehFigure->NAME );
  vhdlsavebefig( BehFigure, 0 );

  BoomDebugLocSaveBehCost( BehFigure, BehFigure->NAME );

  BehFigure->NAME = Name;
}

/*------------------------------------------------------------\
|                                                             |
|                       BoomDebugPrint                        |
|                                                             |
\------------------------------------------------------------*/

void BoomDebugPrint( FileName, Line )

  char *FileName;
  int   Line;
{
  char Buffer[ 32 ];
  int  Length;

  FileName = FileName + 5;
  Length   = strlen( FileName );
  strcpy( Buffer, FileName );
  Buffer[ Length - 2 ] = '\0';

  fprintf( stdout, "%-8s%4d ", Buffer, Line );
}
