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
| File    :                  vasy_analys.c                    |
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

# include "vasy_error.h"
# include "vasy_debug.h"
# include "vasy_shared.h"
# include "vasy_onewait.h"
# include "vasy_mulwait.h"
# include "vasy_synth.h"
# include "vasy_analys.h"

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
|                      VasyAnalysisVpnProc                    |
|                                                             |
\------------------------------------------------------------*/

static void VasyAnalysisVpnProc( VpnFigure, RtlFigure, VpnProc )

  vpnfig_list  *VpnFigure;
  rtlfig_list  *RtlFigure;
  vpnproc_list *VpnProc;
{
  vpntrans_list *VpnTrans;
  vpnact_list   *VpnAction;
  vasyprocinfo   *ProcInfo;
  vexexpr       *VexAtom;
  long           NumberWait;
  long           NumberAction;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  --> VasyAnalysisVpnProc %s\n", VpnProc->NAME );
  }

  ProcInfo = GetVasyVpnProcInfo( VpnProc );

  NumberWait   = 0;
  NumberAction = 0;

  for ( VpnTrans  = VpnProc->TRANS;
        VpnTrans != (vpntrans_list *)0;
        VpnTrans  = VpnTrans->NEXT )
  {
    if ( VpnTrans->TYPE == VPN_TRANS_INF_WAIT ) NumberWait++;
    else
    if ( ( VpnTrans->TYPE == VPN_TRANS_ACT_EXEC    ) ||
         ( VpnTrans->TYPE == VPN_TRANS_ACT_GUARDED ) ) NumberAction++;

    for ( VpnAction  = VpnTrans->ACT;
          VpnAction != (vpnact_list *)0;
          VpnAction  = VpnAction->NEXT )
    {
      if ( ( VpnAction->TYPE == VPN_ACT_CALL   ) ||
           ( VpnAction->TYPE == VPN_ACT_RETURN ) )
      {
        VasyErrorLine( VASY_NOT_YET_IMPLEMENTED_ERROR, VpnAction->LINE, "Call/Return" );
      }

      VexAtom = VpnAction->VEX_ATOM;

      if ( ! IsVexNodeAtom( VexAtom ) )
      {
        VasyErrorLine( VASY_NOT_YET_IMPLEMENTED_ERROR, VpnAction->LINE, "Indirect assignment" );
      }
    }
  }

  ProcInfo->NUMBER_WAIT   = NumberWait;
  ProcInfo->NUMBER_ACTION = NumberAction;

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "  +++ Proc %s NumWait   : %ld\n", VpnProc->NAME, NumberWait   );
    VasyPrintf( stdout, "  +++ Proc %s NumAction : %ld\n", VpnProc->NAME, NumberAction );
  }
/*
**  No action in process, nothing more to do !
*/
  if ( ProcInfo->NUMBER_ACTION == 0 )
  {
    if ( IsVasyDebugLevel1() )
    {
      VasyPrintf( stdout, "  +++ No action in process nothing to do\n" );
    }

    ProcInfo->TYPE = VASY_PROC_UNUSED;

    return;
  }

  if ( ProcInfo->NUMBER_WAIT != 1 )
  {
    VasyMultiWaitVpnProc( VpnFigure, RtlFigure, VpnProc );
  }
  else
  {
    VasyOneWaitVpnProc( VpnFigure, RtlFigure, VpnProc );
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  <-- VasyAnalysisVpnProc %s\n", VpnProc->NAME );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyAnalysisVpnFig                    |
|                                                             |
\------------------------------------------------------------*/

rtlfig_list *VasyAnalysisVpnFig( VpnFigure )

 vpnfig_list *VpnFigure;
{
  rtlfig_list  *RtlFigure;
  rtlasg_list  *RtlAssign;
  vpnproc_list *VpnProc;
  long          NumberReg;
  long          NumberTrs;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "--> VasyAnalysisVpnFig %s\n", VpnFigure->NAME );
  }

  RtlFigure = addrtlfig( VpnFigure->NAME );

  VasySynthesisVpnDecl( VpnFigure, RtlFigure );
  VasySynthesisVpnModel( VpnFigure, RtlFigure );
  VasySynthesisVpnInstance( VpnFigure, RtlFigure );

  if ( IsVasyDebugStatistics() )
  {
    VasyDebugStartChrono(1);
  }

  for ( VpnProc  = VpnFigure->PROCESS;
        VpnProc != (vpnproc_list *)0;
        VpnProc  = VpnProc->NEXT )
  {
    if ( IsVasyVpnProcToAnalyse( VpnProc ) )
    {
      VasyAnalysisVpnProc( VpnFigure, RtlFigure, VpnProc );
    }
    else
    {
      VasySynthesisCombinatorialVpnProc( VpnFigure, RtlFigure, VpnProc );
    }
  }

  if ( IsVasyDebugStatistics() )
  {
    VasyPrintf( stdout, "--> Semantic Analysis : %ld sec\n",
                VasyDebugReadChrono(1) );

    NumberReg = 0;
    NumberTrs = 0;

    for ( RtlAssign  = RtlFigure->ASSIGN;
          RtlAssign != (rtlasg_list *)0;
          RtlAssign  = RtlAssign->NEXT )
    {
      if ( RtlAssign->TYPE == RTL_ASG_REGISTER ) NumberReg++;
      else
      if ( ( RtlAssign->TYPE == RTL_ASG_TRISTATE  ) ||
           ( RtlAssign->TYPE == RTL_ASG_PULL_UP   ) ||
           ( RtlAssign->TYPE == RTL_ASG_PULL_DOWN ) ) NumberTrs++;
    }

    VasyPrintf( stdout, "--> Number Register : %ld\n", NumberReg ); 
    VasyPrintf( stdout, "--> Number Tristate : %ld\n", NumberTrs ); 
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "<-- VasyAnalysisVpnFig %s\n\n", VpnFigure->NAME );
  }

  return( RtlFigure );
}
