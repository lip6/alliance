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
| Tool    :                     VASY                          |
|                                                             |
| File    :                  vasy_reduce.c                    |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
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
# include <stdlib.h>
# include <string.h>

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "vex.h"
# include "vpn.h"
# include "vtl.h"
# include "rtn.h"

# include "vasy_shared.h"
# include "vasy_redact.h"
# include "vasy_redinst.h"
# include "vasy_redwait.h"
# include "vasy_reduce.h"

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
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         VasyReduceVpnFig                    |
|                                                             |
\------------------------------------------------------------*/

void VasyReduceVpnFig( VpnFigure )

  vpnfig_list *VpnFigure;
{
  vpnproc_list *VpnProc;
  int           Reduced;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "--> VasyReduceVpnFig %s\n", VpnFigure->NAME );
    VasyDebugSaveVpnFig( VpnFigure );
  }
/*
**  Partial reduction instruction by instruction
*/
  if ( IsVasyDebugStatistics() )
  {
    VasyPrintf( stdout, "--> Process     : %ld\n", GetVpnNumProc( VpnFigure ) );
    VasyPrintf( stdout, "--> Transition  : %ld\n", GetVpnNumTrans( VpnFigure ) );
    VasyPrintf( stdout, "--> Place       : %ld\n", GetVpnNumPlace( VpnFigure ) );
    VasyDebugStartChrono(1);
  }

  for ( VpnProc  = VpnFigure->PROCESS;
        VpnProc != (vpnproc_list *)0;
        VpnProc  = VpnProc->NEXT )
  {
    if ( IsVasyVpnProcToAnalyse( VpnProc ) )
    {
      VasyRedInstVpnProc( VpnFigure, VpnProc );
    }
  }

  if ( IsVasyDebugStatistics() )
  {
    VasyPrintf( stdout, "--> Local reduction : %ld sec\n", VasyDebugReadChrono(1) );
    VasyPrintf( stdout, "--> Process     : %ld\n", GetVpnNumProc( VpnFigure ) );
    VasyPrintf( stdout, "--> Transition  : %ld\n", GetVpnNumTrans( VpnFigure ) );
    VasyPrintf( stdout, "--> Place       : %ld\n", GetVpnNumPlace( VpnFigure ) );
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyDebugSaveVpnFig( VpnFigure );
  }
/*
**  Global reduction wait to wait
*/
  if ( IsVasyDebugStatistics() ) VasyDebugStartChrono(1);

  Reduced = 0; 

  for ( VpnProc  = VpnFigure->PROCESS;
        VpnProc != (vpnproc_list *)0;
        VpnProc  = VpnProc->NEXT )
  {
    if ( IsVasyVpnProcToAnalyse( VpnProc ) )
    {
      Reduced += VasyRedWaitVpnProc( VpnFigure, VpnProc );
    }
  }

  if ( IsVasyDebugStatistics() )
  {
    VasyPrintf( stdout, "--> Process minimal : %d\n", Reduced );

    VasyPrintf( stdout, "--> Global reduction : %ld sec\n", VasyDebugReadChrono(1) );
    VasyPrintf( stdout, "--> Process          : %ld\n", GetVpnNumProc( VpnFigure ) );
    VasyPrintf( stdout, "--> Transition       : %ld\n", GetVpnNumTrans( VpnFigure ) );
    VasyPrintf( stdout, "--> Place            : %ld\n", GetVpnNumPlace( VpnFigure ) );
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyDebugSaveVpnFig( VpnFigure );
    VasyPrintf( stdout, "<-- VasyReduceVpnFig %s\n", VpnFigure->NAME );
  }
}
