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
| File    :                   vpnenv.c                        |
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

# include "mut.h"
# include "aut.h"
# include "vex.h"
# include "vpn.h"

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "vpnenv.h"
# include "vpnerror.h"

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

  char *VPN_IN;
  char *VPN_OUT;

  char *VPN_VHDL_FORMAT = (char *)0;
  char *VPN_VPN_FORMAT  = (char *)0;
  char *VPN_VBE_FORMAT  = (char *)0;
  char *VPN_VST_FORMAT  = (char *)0;
  char *VPN_SYN_FORMAT  = (char *)0;
  char *VPN_FSM_FORMAT  = (char *)0;

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
|                            vpnenv                           |
|                                                             |
\------------------------------------------------------------*/

void vpnenv()
{
  VPN_IN = namealloc( mbkgetenv( "VPN_IN") );

  if ( VPN_IN == (char *)0 )
  {
    VPN_IN = namealloc( VPN_DEFAULT_IN );
  }
 
  VPN_OUT = namealloc( mbkgetenv( "VPN_OUT") );

  if ( VPN_OUT == (char *)0 )
  {
    VPN_OUT = namealloc( VPN_DEFAULT_OUT );
  }

  VPN_VHDL_FORMAT = namealloc( "vhdl" );
  VPN_VPN_FORMAT  = namealloc( "vpn"  );
  VPN_SYN_FORMAT  = namealloc( "vhd"  );
  VPN_VBE_FORMAT  = namealloc( "vbe"  );
  VPN_VST_FORMAT  = namealloc( "vst"  );
  VPN_FSM_FORMAT  = namealloc( "fsm"  );
}
