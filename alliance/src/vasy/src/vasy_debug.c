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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                    VASY                           |
|                                                             |
| File    :                 vasy_debug.c                      |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   25.08.97                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <string.h>

# include "mut.h"
# include "aut.h"
# include "vex.h"
# include "vpn.h"
# include "vtl.h"

# include "vasy_error.h"
# include "vasy_debug.h"

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

   int           VasyDebugFlag   = 0;
   int           VasyDebugOption = 0;
   time_t        VasyDebugChrono[ VASY_DEBUG_MAX_CHRONO ];

/*------------------------------------------------------------\
|                                                             |
|                      Private variables                      |
|                                                             |
\------------------------------------------------------------*/

  static char *VasyDebugItemName[ ] =
  {
     "vasy_analys.c",
     "vasy_array.c",
     "vasy_debug.c",
     "vasy_drvalc.c",
     "vasy_drvrtl.c",
     "vasy_drvsyn.c",
     "vasy_drvvlog.c",
     "vasy_elabo.c",
     "vasy_error.c",
     "vasy_func.c",
     "vasy_generate.c",
     "vasy_generic.c",
     "vasy_loop.c",
     "vasy_main.c",
     "vasy_mulwait.c",
     "vasy_onewait.c",
     "vasy_parse.c",
     "vasy_preanal.c",
     "vasy_redact.c",
     "vasy_redinst.c",
     "vasy_reduce.c",
     "vasy_redwait.c",
     "vasy_shared.c",
     "vasy_simprtl.c",
     "vasy_simul.c",
     "vasy_support.c",
     "vasy_synth.c",
     "vasy_vexbdd.c",
    (char *)0
  };

  static char *VasyDebugItemFlag = (char *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       VasyDebugSetOption                    |
|                                                             |
\------------------------------------------------------------*/

int VasyDebugSetOption( Name )

  char *Name;
{
  if ( ! strcmp( Name, "no_redinst" ) )
  {
    SetVasyDebugNoRedInst(); return( 1 );
  }
  else
  if ( ! strcmp( Name, "stat" ) )
  {
    SetVasyDebugStatistics(); return( 1 );
  }
  else
  if ( ! strcmp( Name, "stdout" ) )
  {
    SetVasyDebugDriveStdout(); return( 1 );
  }
  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyDebugAddItem                      |
|                                                             |
\------------------------------------------------------------*/

int VasyDebugAddItem( Name, Mode )

  char *Name;
  int   Mode;
{
  char  Buffer[ 64 ];
  char *Item;
  int   Index;

  if ( VasyDebugItemFlag == (char *)0 )
  {
    Index = 0; 

    while ( VasyDebugItemName[ Index ] != (char *)0 )
    {
      VasyDebugItemName[ Index ] = namealloc( VasyDebugItemName[ Index ] );

      Index++;
    }

    VasyDebugItemFlag = autallocblock( Index );
  }

  if ( ! strcmp( Name, "all" ) )
  {
    VasyDebugFlag |= Mode;

    Index = 0;

    while ( VasyDebugItemName[ Index ] != (char *)0 )
    {
      VasyDebugItemFlag[ Index ] |= Mode;

      Index++;
    }

    return( 1 );
  }
  else
  {
    sprintf( Buffer, "vasy_%s.c", Name );
    Item  = namealloc( Buffer );
    Index = 0; 

    while ( VasyDebugItemName[ Index ] != (char *)0 )
    {
      if ( Item == VasyDebugItemName[ Index ] )
      {
        VasyDebugItemFlag[ Index ] |= Mode;
        VasyDebugFlag |= Mode;

        return( 1 );
      }
  
      Index++;
    }
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                      VasyDebugSearchItem                    |
|                                                             |
\------------------------------------------------------------*/

int VasyDebugSearchItem( Item, Mode )

  char *Item;
  int   Mode;
{
  int Index;

  if ( ( Mode & VasyDebugFlag ) == Mode )
  {
    Item  = namealloc( Item );
    Index = 0; 

    while ( VasyDebugItemName[ Index ] != (char *)0 )
    {
      if ( Item == VasyDebugItemName[ Index ] )
      {
        return( ( VasyDebugItemFlag[ Index ] & Mode ) == Mode );
      }

      Index++;
    }
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyDebugLocSaveVpnFig                |
|                                                             |
\------------------------------------------------------------*/

void VasyDebugLocSaveVpnFig( VpnFigure, FileName, Line )

  vpnfig_list *VpnFigure;
  char        *FileName;
  int          Line;
{
  static char Buffer[ 32 ];
  static int  Number = 0;

  char       *Name;
  char       *VpnOut;
  int         Length;

  Name   = VpnFigure->NAME;
  VpnOut = VPN_OUT;

  sprintf( Buffer, "%03d_%04d", Number++, Line );
  Length = strlen( Buffer );

  FileName = FileName + 4;
  strcpy( Buffer + Length, FileName );
  Length += strlen( FileName );
  Buffer[ Length - 2 ] = '\0';

  VpnFigure->NAME = namealloc( Buffer );
  VPN_OUT = namealloc( "vpn" );

  fprintf( stdout, "  +++ VasyDebugSaveVpnFig %s.vpn\n", VpnFigure->NAME );
  savevpnfig( VpnFigure );

  VpnFigure->NAME = Name;
  VPN_OUT         = VpnOut;
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyDebugPrint                        |
|                                                             |
\------------------------------------------------------------*/

void VasyDebugPrint( FileName, Line )

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
