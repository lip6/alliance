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
  fprintf( stdout, "\t\t          -P Partial Verbose mode on\n" );
  fprintf( stdout, "\t\t          -S Saves Vpn figure\n" );
  fprintf( stdout, "\t\t          -I Vpn Input format\n" );
  fprintf( stdout, "\t\t          -U Unreduced figure\n" );
  fprintf( stdout, "\t\t          -O Vpn Output format\n" );
  fprintf( stdout, "\t\t          -E Expand Vpn Figure\n" );
  fprintf( stdout, "\n" );

  exit( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                          Expand Vpn Trans                   |
|                                                             |
\------------------------------------------------------------*/

static void ExpandVpnTrans( VpnFigure, VpnFather, VpnTrans )

  vpnfig_list   *VpnFigure;
  void          *VpnFather;
  vpntrans_list *VpnTrans;
{
  vpntrans_list *ScanTrans;
  vpntrans_list *LastTrans;
  vpntrans_list *NewTrans;
  vpnplace_list *NewPlace;
  vpnact_list   *ScanAction;
  vpnact_list   *FirstAction;
  chain_list    *LastPlaceOut;
  char           Buffer[ 512 ];
  char          *Name;
  int            Number;

  for ( ScanTrans  = VpnTrans;
        ScanTrans != (vpntrans_list *)0;
        ScanTrans  = ScanTrans->NEXT )
  {
    if ( ( ScanTrans->TYPE != VPN_TRANS_ACT_GUARDED ) &&
         ( ScanTrans->TYPE != VPN_TRANS_ACT_EXEC    ) )  continue;
  
    if ( ScanTrans->TYPE == VPN_TRANS_ACT_GUARDED )
    {
      ScanTrans->TYPE = VPN_TRANS_GUARDED;
      ScanAction = ScanTrans->ACT;

      ScanTrans->ACT      = (vpnact_list *)0;
      ScanTrans->LAST_ACT = &ScanTrans->ACT;
    }
    else
    {
      FirstAction = ScanTrans->ACT;
      ScanAction  = FirstAction->NEXT;

      FirstAction->NEXT = (vpnact_list *)0;
      ScanTrans->LAST_ACT  = &FirstAction->NEXT;
    }

    Number    = 0;
    LastTrans = ScanTrans;
    NewTrans  = ScanTrans;

    LastPlaceOut = ScanTrans->PLACE_OUT;

    ScanTrans->NUMBER_OUT = 0;
    ScanTrans->PLACE_OUT = (chain_list *)0;

    while ( ScanAction != (vpnact_list *)0 )
    {
      sprintf( Buffer, "%s.expand.%d", ScanTrans->NAME, Number++ );
      Name = namealloc( Buffer );

      if ( ScanTrans->FATHER == VPN_FATHER_PROCESS )
      {
        NewTrans = addvpnproctrans( VpnFigure, VpnFather, Name );
        NewPlace = addvpnprocplace( VpnFigure, VpnFather, Name );
      }
      else
      {
        NewTrans = addvpnfunctrans( VpnFigure, VpnFather, Name );
        NewPlace = addvpnfuncplace( VpnFigure, VpnFather, Name );
      }

      addvpnarctrans( VpnFigure, LastTrans, NewPlace );
      addvpnarcplace( VpnFigure, NewPlace , NewTrans );

      FirstAction = ScanAction;
      ScanAction  = ScanAction->NEXT;

      NewTrans->TYPE     = VPN_TRANS_ACT_EXEC;
      NewTrans->ACT      = FirstAction;
      NewTrans->LAST_ACT = &FirstAction->NEXT;
      FirstAction->NEXT  = (vpnact_list *)0;

      LastTrans = NewTrans;
    }

    NewTrans->PLACE_OUT  = LastPlaceOut;
    NewTrans->NUMBER_OUT = 1;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          Expand Vpn Figure                  |
|                                                             |
\------------------------------------------------------------*/

static void ExpandVpnFigure( VpnFigure )

  vpnfig_list *VpnFigure;
{
  vpnproc_list *VpnProc;
  vpnfunc_list *VpnFunc;

  for ( VpnProc  = VpnFigure->PROCESS;
        VpnProc != (vpnproc_list *)0;
        VpnProc  = VpnProc->NEXT )
  {
    ExpandVpnTrans( VpnFigure, VpnProc, VpnProc->TRANS );
  }

  for ( VpnFunc  = VpnFigure->FUNCTION;
        VpnFunc != (vpnfunc_list *)0;
        VpnFunc  = VpnFunc->NEXT )
  {
    ExpandVpnTrans( VpnFigure, VpnFunc, VpnFunc->TRANS );
  }
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
  int   FlagExpand    = 0;
  int   FlagPartial   = 0;
  int   FlagUnreduced = 0;

  mbkenv();
  autenv();
  vexenv();
  vpnenv();

  InputFileName  = (char *)0;
  OutputFileName = (char *)0;

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
          case 'P' : FlagPartial = 1;
          break;
          case 'S' : FlagSave    = 1;
          break;
          case 'E' : FlagExpand  = 1;
          break;
          case 'U' : FlagUnreduced = 1;
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

  if ( FlagUnreduced )
  {
    VpnFigure = addvpnfig( InputFileName );
    loadvpnfig( VpnFigure, InputFileName, 0 );
  }
  else
  {
    VpnFigure = getvpnfig( InputFileName );
  }

  if ( FlagExpand )
  {
    ExpandVpnFigure( VpnFigure );
  }

  if ( FlagVerbose ) viewvpnfig( VpnFigure );

  if ( FlagPartial )
  {
    fprintf( stdout, "  -> Number Process  %ld\n", GetVpnNumProc( VpnFigure ) );
    fprintf( stdout, "  -> Number Function %ld\n", GetVpnNumFunc( VpnFigure ) );
    fprintf( stdout, "  -> Number Place    %ld\n", GetVpnNumPlace( VpnFigure ) );
    fprintf( stdout, "  -> Number Trans    %ld\n", GetVpnNumTrans( VpnFigure ) );
    fprintf( stdout, "  -> Number Arc      %ld\n", GetVpnNumArc( VpnFigure ) );
  }

  if ( FlagSave )
  {
    VpnFigure->NAME = namealloc( OutputFileName );
    savevpnfig( VpnFigure );
  }

  delvpnfig( VpnFigure->NAME );

  return( 0 );
}
