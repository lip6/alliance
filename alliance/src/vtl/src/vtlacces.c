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
| Tool    :                     Vtl                           |
|                                                             |
| File    :                  vtlacces.c                       |
|                                                             |
| Date    :                   04.07.96                        |
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
# include <stdlib.h>
# include <unistd.h>

# include "mut.h"
# include "aut.h"
# include "vex.h"
# include "vpn.h"
# include "vtl.h"

# include "vtlacces.h"
# include "vtlerror.h"

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
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          loadvpnfig                         |
|                                                             |
\------------------------------------------------------------*/

void loadvpnfig( Figure, Name, Reduced )

  vpnfig_list *Figure;
  char        *Name;
  int          Reduced;
{
  if ( ( VPN_IN == VPN_VHDL_FORMAT ) ||
       ( VPN_IN == VPN_SYN_FORMAT  ) ||
       ( VPN_IN == VPN_FSM_FORMAT  ) ||
       ( VPN_IN == VPN_VST_FORMAT  ) ||
       ( VPN_IN == VPN_VBE_FORMAT  ) )
  {
    vhdlloadvpnfig( Figure, Name, Reduced );
  }
  else
  if ( VPN_IN == VPN_VPN_FORMAT )
  {
    vpnloadvpnfig( Figure, Name );
  }
  else
  {
    vtlerror( VTL_UNKNOWN_VPN_IN, VPN_IN );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          savevpnfig                         |
|                                                             |
\------------------------------------------------------------*/

void savevpnfig( Figure )

  vpnfig_list *Figure;
{
  if ( VPN_OUT == VPN_VHDL_FORMAT )
  {
    vhdlsavevpnfig( Figure );
  }
  else
  if ( VPN_OUT == VPN_VPN_FORMAT )
  {
    vpnsavevpnfig( Figure );
  }
  else
  {
    vtlerror( VTL_UNKNOWN_VPN_OUT, VPN_OUT );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          getvpnfig                          |
|                                                             |
\------------------------------------------------------------*/

vpnfig_list *getvpnfig( Name )

   char *Name;
{
  vpnfig_list *Figure;
  char        *FigName;

  FigName = namealloc( Name );

  for ( Figure  = HEAD_VPNFIG;
        Figure != (vpnfig_list *)0;
        Figure  = Figure->NEXT )
  {
    if ( Figure->NAME == FigName ) break;
  }

  if ( Figure == (vpnfig_list *)0 )
  {
    Figure = addvpnfig( Name );

    loadvpnfig( Figure, Name, 1 );
  }

  return( Figure );
}
