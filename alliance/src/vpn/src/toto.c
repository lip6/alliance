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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     Vpn                           |
|                                                             |
| File    :                    main.c                         |
|                                                             |
| Date    :                   08.02.95                        |
|                                                             |
| Author  :               Jacomme Ludovic                     |
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
|                            Usage                            |
|                                                             |
\------------------------------------------------------------*/

void VpnUsage()
{
  fprintf( stderr, "\t\tvpntest [Options] Input_name [Output_name]\n\n" );
  fprintf( stdout, "\t\tOptions : -V Sets Verbose mode on\n" );
  fprintf( stdout, "\t\t          -S Saves Vpn figure\n" );
  fprintf( stdout, "\t\t          -I Vpn Input format\n" );
  fprintf( stdout, "\t\t          -O Vpn Output format\n" );
  fprintf( stdout, "\t\t          -s Simplifies vex Vpn figure\n" );
  fprintf( stdout, "\n" );

  exit( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

int main( argc, argv )

  int   argc;
  char *argv[];
{
  vpnfig_list *VpnFigure;
  char        *InputFileName;
  char        *OutputFileName;
  int          Number;
  int          Index;
  char         Option;

  int   FlagVerbose   = 0;
  int   FlagSave      = 0;
  int   FlagSimp      = 0;

  mbkenv();
  autenv();
  vexenv();
  vpnenv();

  InputFileName = (char *)0;

  if ( argc < 2 ) VpnUsage();

  InputFileName = (char *)0;

  for ( Number = 1; Number  < argc; Number++ )
  {
    if ( argv[ Number ][ 0 ] == '-' )
    {
      for ( Index = 1; argv[ Number ][ Index ] != '\0'; Index++ )
      {
        Option = argv[ Number ][ Index ];

        if ( Option == 'I' ) 
        {
          Number = Number + 1;
          VPN_IN = namealloc( argv[ Number ] );
          break;
        }
        else
        if ( Option == 'O' )
        {
          Number  = Number + 1;
          VPN_OUT = namealloc( argv[ Number ] );
          break;
        }

        switch ( Option )
        {
          case 'V' : FlagVerbose = 1;
          break;
          case 'S' : FlagSave    = 1;
          break;
          case 's' : FlagSimp    = 1;
          break;
          default  : VpnUsage();
        }
      }
    }
    else
    if ( InputFileName == (char *)0 ) InputFileName = argv[ Number ];
    else
    if ( OutputFileName == (char *)0 ) OutputFileName = argv[ Number ];
    else
    VpnUsage();
  }

  if ( InputFileName  == (char *)0 ) VpnUsage();
  if ( OutputFileName == (char *)0 ) OutputFileName = InputFileName;

  if ( ( FlagSave ) && ( InputFileName == OutputFileName ) )
  {
    VpnUsage();
  }

  fprintf( stdout, "VPN_IN  : %s\n", VPN_IN  );
  fprintf( stdout, "VPN_OUT : %s\n", VPN_OUT );

  VpnFigure = getvpnfig( InputFileName );

  if ( FlagSimp )
  {
    simpvpnfig( VpnFigure );
  }

  if ( FlagVerbose ) viewvpnfig( VpnFigure );

  if ( FlagSave )
  {
    VpnFigure->NAME = namealloc( OutputFileName );
    savevpnfig( VpnFigure );
  }

  delvpnfig( VpnFigure->NAME );

  return( 0 );
}
