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
| Tool    :                     VASY                           |
|                                                             |
| File    :                  vasy_elabo.c                      |
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

# include "vasy_elabo.h"
# include "vasy_simul.h"
# include "vasy_debug.h"
# include "vasy_error.h"

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
|                   VasyElaborateVpnFig                       |
|                                                             |
\------------------------------------------------------------*/

void VasyElaborateVpnFig( VpnFigure )

  vpnfig_list *VpnFigure;
{
  vpnproc_list *VpnProc;
  vpndecl_list *VpnDeclar;
  vpnsym       *VpnSymbol;
  vexexpr      *DeclAtom;
  vexexpr      *DeclInit;
  vasysimul    *SimulInit;
  char         *SimulValue;
  char          Buffer[ 2048 ];
  int           LiteralId;
  short         Width;
  short         Position;
  short         Type;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "--> VasyElaborateVpnFig %s\n", VpnFigure->NAME );
  }

  SimulInit = (vasysimul *)0;
/*
**  For all declarations set initial value
*/
  for ( Type = 0; Type < VPN_MAX_DECLAR_TYPE; Type++ )
  {
    for ( VpnDeclar  = VpnFigure->DECLAR[ Type ];
          VpnDeclar != (vpndecl_list *)0;
          VpnDeclar  = VpnDeclar->NEXT )
    {
      DeclAtom = VpnDeclar->VEX_ATOM;
      DeclInit = VpnDeclar->VEX_INIT;
      Width    = DeclAtom->WIDTH;

      if ( DeclInit != (vexexpr *)0 )
      {
        if ( IsVasyDebugLevel2() )
        {
          VasyPrintf( stdout, "  --> Initial value of " );
          viewvexexprboundln( DeclAtom );
        }

        SimulInit  = VasySimulateVexExpr( VpnFigure, DeclAtom, DeclInit );
        SimulValue = SimulInit->VALUE;

        if ( IsVasyDebugLevel2() )
        {
          VasyPrintf( stdout, "  <-- Initial value\n" );
        }
      }
      else
      {
        SimulValue = (char *)0;
      }

      for ( Position = 0; Position < Width; Position++ )
      {
        VpnSymbol = &VpnDeclar->DECL_SYM[ Position ];

        if ( SimulValue != (char *)0 ) VpnSymbol->INIT = SimulValue[ Position ];
        else                           VpnSymbol->INIT = VEX_ZERO_ID;

        VpnSymbol->EFFEC = VpnSymbol->INIT;
        VpnSymbol->DRIVE = VpnSymbol->INIT;
        VpnSymbol->EVENT = 1;
      }

      if ( SimulValue != (char *)0 ) VasySimulateDelSimul( SimulInit );
    }
  }
/*
**  For all process make elaboration
*/
  for ( VpnProc  = VpnFigure->PROCESS;
        VpnProc != (vpnproc_list *)0;
        VpnProc  = VpnProc->NEXT )
  {
    VasySimulateVpnProc( VpnFigure, VpnProc );
  }
/*
**  For all declarations set new init value
*/
  for ( Type = 0; Type < VPN_MAX_DECLAR_TYPE; Type++ ) 
  {
    for ( VpnDeclar  = VpnFigure->DECLAR[ Type ];
          VpnDeclar != (vpndecl_list *)0;
          VpnDeclar  = VpnDeclar->NEXT )
    {
      DeclAtom = VpnDeclar->VEX_ATOM;
      Width    = DeclAtom->WIDTH;

      for ( Position = 0; Position < Width; Position++ )
      {
        VpnSymbol = &VpnDeclar->DECL_SYM[ Position ];

        if ( ( VpnDeclar->TYPE == VPN_DECLAR_DEFINE   ) ||
             ( VpnDeclar->TYPE == VPN_DECLAR_VARIABLE ) ) LiteralId = VpnSymbol->EFFEC;
        else                                              LiteralId = VpnSymbol->DRIVE;

        Buffer[ Position + 1 ] = VEX_LITERAL_BY_ID[ LiteralId ]; 
      }

      Position++;

      if ( IsVexAtomVector( DeclAtom ) )
      {
        Buffer[ 0        ] = '\"';
        Buffer[ Position ] = '\"';
      }
      else
      {
        Buffer[ 0        ] = '\'';
        Buffer[ Position ] = '\'';
      }

      Buffer[ Position + 1 ] = '\0';

      freevexexpr( VpnDeclar->VEX_INIT );
      VpnDeclar->VEX_INIT = createvexatomlit( Buffer );
    }
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "<-- VasyElaborateVpnFig %s\n", VpnFigure->NAME );
  }
}
