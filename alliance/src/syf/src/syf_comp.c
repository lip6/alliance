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
| Tool    :                     SYF                           |
|                                                             |
| File    :                  syf_comp.c                       |
|                                                             |
| Authors :                  C. Sarwary                       |
|                     Modified by Jacomme Ludovic             |
|                                                             |
| Date    :                   01.11.94                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "fvh.h"
# include "ftl.h"


# include <stdio.h>
# include <stdlib.h>
# include "syf_fsm.h"
# include "syf_error.h"
# include "syf_comp.h"
# include "kis_parse.h"
# include "fbh_parse.h"

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
|                         SyfRemoveStable                     |
|                                                             |
\------------------------------------------------------------*/

static void SyfRemoveStable( Expr )

  ablexpr *Expr;
{
  if ( Expr == (ablexpr *)0 ) return;

  if ( ! ABL_ATOM( Expr ) )
  {
    if ( ABL_OPER( Expr ) == ABL_STABLE )
    {
      freechain( ABL_CADR( Expr ) );
      freechain( ABL_CDR( Expr ) );
      freechain( ABL_CAR( Expr ) );

      ABL_CDR( Expr ) = (ablexpr *)0;
      ABL_ATOM_VALUE( Expr ) = getablatomzero();

      return;
    }

    while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
    {
      SyfRemoveStable( ABL_CAR( Expr ) );
    }
  }
}


/*------------------------------------------------------------\
|                                                             |
|                          SyfCompile                         |
|                                                             |
\------------------------------------------------------------*/

fsmfig_list *SyfCompile( FileName, FlagScan, FlagSynopsys )

  char *FileName;
  char  FlagScan;
  char  FlagSynopsys;
{
  fsmfig_list *FsmFigure;
  fsmfig_list *ScanFigure;
  chain_list  *ScanChain;

  if ( FSM_IN == FSM_VHDL_FORMAT )
  {
    FsmFigure = SyfFbhParse( FileName, FlagScan );
  }
  else
  {
    FsmFigure = SyfKissParse( FileName, FlagScan );
  }
 
  if ( FlagSynopsys )
  {
    SyfRemoveStable( FsmFigure->CLOCK_ABL );

    for ( ScanChain  = FsmFigure->MULTI;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      ScanFigure = (fsmfig_list *)ScanChain->DATA;

      SyfRemoveStable( ScanFigure->CLOCK_ABL );
    }
  }

  return( FsmFigure );
}
