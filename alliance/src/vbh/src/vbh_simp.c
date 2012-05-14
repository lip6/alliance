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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     Vbh                           |
|                                                             |
| File    :                   vbhsimp.c                       |
|                                                             |
| Date    :                   03.12.96                        |
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
# include "vbh.h"

# include <stdio.h>
# include <string.h>

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
|                         Private Variables                   |
|                                                             |
\------------------------------------------------------------*/

  static vexexpr *((*VbhFunctionL)()) = NULL;
  static vexexpr *((*VbhFunctionR)()) = NULL;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Simp  Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    Local Simp Vbh Ifs                       |
|                                                             |
\------------------------------------------------------------*/

static void loc_simpvbifs( ScanIfs  )

  vbifs_list  *ScanIfs;
{
  ScanIfs->CND = (*VbhFunctionR)( ScanIfs->CND );

  if ( ScanIfs->CNDTRUE != (ptype_list *)0 )
  {
    vbh_simpvbinst( ScanIfs->CNDTRUE );
  }

  if ( ScanIfs->CNDFALSE != (ptype_list *)0 )
  {
    vbh_simpvbinst( ScanIfs->CNDFALSE );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    Local Simp Vbh Var                       |
|                                                             |
\------------------------------------------------------------*/

static void loc_simpvbvar( ScanVar  )

  vbvar_list  *ScanVar;
{
  ScanVar->TARGET = (*VbhFunctionL)( ScanVar->TARGET );
  ScanVar->VEX    = (*VbhFunctionR)( ScanVar->VEX );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local Simp Vbh Asg                       |
|                                                             |
\------------------------------------------------------------*/

static void loc_simpvbasg( ScanAsg  )

  vbasg_list  *ScanAsg;
{
  ScanAsg->TARGET = (*VbhFunctionL)( ScanAsg->TARGET );
  ScanAsg->VEX    = (*VbhFunctionR)( ScanAsg->VEX );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local Simp Vbh Agr                       |
|                                                             |
\------------------------------------------------------------*/

static void loc_simpvbagr( ScanAgr  )

  vbagr_list  *ScanAgr;
{
  vbh_simpvbinst( ScanAgr->ASSIGN );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local Simp Vbh Case                      |
|                                                             |
\------------------------------------------------------------*/

static void loc_simpvbcas( ScanCase  )

  vbcas_list  *ScanCase;
{
  vbcho_list    *ScanChoice;
  int            Offset;

  ScanCase->VEX = (*VbhFunctionR)( ScanCase->VEX );

  for ( Offset = 0; Offset < ScanCase->SIZE; Offset++ )
  {
    ScanChoice = &ScanCase->CHOICE[ Offset ];
    vbh_simpvbinst( ScanChoice->INSTRUCTION );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    Local Simp Vbh Wait                      |
|                                                             |
\------------------------------------------------------------*/

static void loc_simpvbwas( ScanWait  )

  vbwas_list  *ScanWait;
{
  if ( ScanWait->CND != (vexexpr *)0 )
  {
    ScanWait->CND = (*VbhFunctionR)( ScanWait->CND );
  }

  if ( ScanWait->TIMEOUT != (vexexpr *)0 )
  {
    ScanWait->TIMEOUT = (*VbhFunctionR)( ScanWait->TIMEOUT );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    Local Simp Vbh For                       |
|                                                             |
\------------------------------------------------------------*/

static void loc_simpvbfor( ScanFor )

  vbfor_list  *ScanFor;
{
  ScanFor->VARIABLE = (*VbhFunctionL)( ScanFor->VARIABLE );

  if ( ScanFor->LEFT != (vexexpr *)0 )
  {
    ScanFor->LEFT = (*VbhFunctionR)( ScanFor->LEFT );
  }

  if ( ScanFor->RIGHT != (vexexpr *)0 )
  {
    ScanFor->RIGHT = (*VbhFunctionR)( ScanFor->RIGHT );
  }

  vbh_simpvbinst( ScanFor->INSTRUCTION );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local Simp Vbh While                     |
|                                                             |
\------------------------------------------------------------*/

static void loc_simpvbwhi( ScanWhile )

  vbwhi_list  *ScanWhile;
{
  if ( ScanWhile->CND != (vexexpr *)0 )
  {
    ScanWhile->CND = (*VbhFunctionR)( ScanWhile->CND );
  }

  vbh_simpvbinst( ScanWhile->INSTRUCTION );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local Simp Vbh Loop                      |
|                                                             |
\------------------------------------------------------------*/

static void loc_simpvblop( ScanLoop )

  vblop_list  *ScanLoop;
{
  vbh_simpvbinst( ScanLoop->INSTRUCTION );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local Simp Vbh Exit                      |
|                                                             |
\------------------------------------------------------------*/

static void loc_simpvbext( ScanExit  )

  vbext_list  *ScanExit;
{
  if ( ScanExit->CND != (vexexpr *)0 )
  {
    ScanExit->CND = (*VbhFunctionR)( ScanExit->CND );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    Local Simp Vbh Next                      |
|                                                             |
\------------------------------------------------------------*/

static void loc_simpvbnxt( ScanNext  )

  vbnxt_list  *ScanNext;
{
  if ( ScanNext->CND != (vexexpr *)0 )
  {
    ScanNext->CND = (*VbhFunctionR)( ScanNext->CND );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    Local Simp Vbh Return                    |
|                                                             |
\------------------------------------------------------------*/

static void loc_simpvbret( ScanReturn  )

  vbret_list  *ScanReturn;
{
  if ( ScanReturn->RET != (vexexpr *)0 )
  {
    ScanReturn->RET = (*VbhFunctionR)( ScanReturn->RET );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    Local Simp Vbh Call                      |
|                                                             |
\------------------------------------------------------------*/

static void loc_simpvbcal( ScanCall  )

  vbcal_list  *ScanCall;
{
  if ( ScanCall->CALL != (vexexpr *)0 )
  {
    ScanCall->CALL = (*VbhFunctionR)( ScanCall->CALL );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    Local Simp Vbh Instruction               |
|                                                             |
\------------------------------------------------------------*/

void vbh_simpvbinst( Instruction )

  ptype_list *Instruction;
{
  while ( Instruction != (ptype_list *)0 )
  {
    switch ( Instruction->TYPE )
    {
      case VBH_BEIFS : loc_simpvbifs( Instruction->DATA );
      break;

      case VBH_BEASG : loc_simpvbasg( Instruction->DATA );
      break;

      case VBH_BEVAR : loc_simpvbvar( Instruction->DATA );
      break;

      case VBH_BECAS : loc_simpvbcas( Instruction->DATA );
      break;

      case VBH_BEWAS : loc_simpvbwas( Instruction->DATA );
      break;

      case VBH_BEFOR : loc_simpvbfor( Instruction->DATA );
      break;

      case VBH_BEWHI : loc_simpvbwhi( Instruction->DATA );
      break;

      case VBH_BELOP : loc_simpvblop( Instruction->DATA );
      break;

      case VBH_BENXT : loc_simpvbnxt( Instruction->DATA );
      break;

      case VBH_BEEXT : loc_simpvbext( Instruction->DATA );
      break;

      case VBH_BERET : loc_simpvbret( Instruction->DATA );
      break;

      case VBH_BECAL : loc_simpvbcal( Instruction->DATA );
      break;

      case VBH_BEAGR : loc_simpvbagr( Instruction->DATA );
      break;
    }

    Instruction = Instruction->NEXT;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                         Simp Vbh Instance                   |
|                                                             |
\------------------------------------------------------------*/

void loc_simpvbins( BeIns )

  vbins_list *BeIns;
{
  vbmap_list *BeMap;

  while ( BeIns != (vbins_list *)0 )
  {
    for ( BeMap  = BeIns->PORT_MAP;
          BeMap != (vbmap_list *)0;
          BeMap  = BeMap->NEXT )
    {
      if ( BeMap->FORMAL != (vexexpr *)0 )
      {
        BeMap->FORMAL = (*VbhFunctionL)( BeMap->FORMAL );
      }

      if ( BeMap->ACTUAL != (vexexpr *)0 )
      {
        BeMap->ACTUAL = (*VbhFunctionR)( BeMap->ACTUAL );
      }
    }

    for ( BeMap  = BeIns->GEN_MAP;
          BeMap != (vbmap_list *)0;
          BeMap  = BeMap->NEXT )
    {
      if ( BeMap->FORMAL != (vexexpr *)0 )
      {
        BeMap->FORMAL = (*VbhFunctionR)( BeMap->FORMAL );
      }

      if ( BeMap->ACTUAL != (vexexpr *)0 )
      {
        BeMap->ACTUAL = (*VbhFunctionL)( BeMap->ACTUAL );
      }
    }

    BeIns = BeIns->NEXT;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                         Simp Vbh Aux                        |
|                                                             |
\------------------------------------------------------------*/

void loc_simpvbaux( BeAux )

  vbaux_list *BeAux;
{
  while ( BeAux != (vbaux_list *)0 )
  {
    BeAux->TARGET = (*VbhFunctionL)( BeAux->TARGET );
    if ( BeAux->VEX != (vexexpr *)0 )
    {
      BeAux->VEX = (*VbhFunctionR)( BeAux->VEX );
    }

    BeAux = BeAux->NEXT;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                         Simp Vbh Pcs                        |
|                                                             |
\------------------------------------------------------------*/

void loc_simpvbpcs( BePcs )

  vbpcs_list *BePcs;
{
  ptype_list *ScanPtype;
  vbvar_list *BeVar;

  while( BePcs != (vbpcs_list *)0 )
  {
    for ( ScanPtype  = BePcs->VARIABLE;
          ScanPtype != (ptype_list *)0;
          ScanPtype  = ScanPtype->NEXT )
    {
      BeVar = (vbvar_list *)ScanPtype->DATA;

      BeVar->TARGET = (*VbhFunctionL)( BeVar->TARGET );
      if ( BeVar->VEX != (vexexpr *)0 )
      {
        BeVar->VEX = (*VbhFunctionR)( BeVar->VEX );
      }
    }

    vbh_simpvbinst( BePcs->INSTRUCTION );
    BePcs = BePcs->NEXT;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                         Simp Vbh Generates                  |
|                                                             |
\------------------------------------------------------------*/

void loc_simpvbgnr( BeGnr )

  vbgnr_list *BeGnr;
{
  while( BeGnr != (vbgnr_list *)0 )
  {
    if ( BeGnr->FOR_VARIABLE != (vexexpr *)0 )
    {
      BeGnr->FOR_VARIABLE = (*VbhFunctionL)( BeGnr->FOR_VARIABLE );
    }

    if ( BeGnr->FOR_LEFT != (vexexpr *)0 )
    {
      BeGnr->FOR_LEFT = (*VbhFunctionR)( BeGnr->FOR_LEFT );
    }

    if ( BeGnr->FOR_RIGHT != (vexexpr *)0 )
    {
      BeGnr->FOR_RIGHT = (*VbhFunctionR)( BeGnr->FOR_RIGHT );
    }

    if ( BeGnr->IF_COND != (vexexpr *)0 )
    {
      BeGnr->IF_COND = (*VbhFunctionR)( BeGnr->IF_COND );
    }

    loc_simpvbpcs( BeGnr->BEPCS );
    loc_simpvbins( BeGnr->BEINS );
    loc_simpvbgnr( BeGnr->BEGNR );

    BeGnr = BeGnr->NEXT;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      For All Vex Vbh Figure                 |
|                                                             |
\------------------------------------------------------------*/

void vbh_forallvexvbfig( Figure, FunctionL, FunctionR )

  vbfig_list *Figure;
  vexexpr    *(*FunctionL)();
  vexexpr    *(*FunctionR)();
{
  vbmod_list *BeMod;
  vbgen_list *BeGen;
  vbtyp_list *BeTyp;
  vbcst_list *BeCst;
  vbpor_list *BePor;
  vbfun_list *BeFun;
  vbarg_list *BePar;
  vbvar_list *BeVar;
  ptype_list *ScanPtype;
  long        Left;
  long        Right;

  VbhFunctionL = FunctionL;
  VbhFunctionR = FunctionR;

  for ( BeTyp  = Figure->BETYP;
        BeTyp != (vbtyp_list *)0;
        BeTyp  = BeTyp->NEXT )
  {
    if ( BeTyp->DYNAMIC )
    {
      BeTyp->DYNAMIC_LEFT  = (*VbhFunctionR)( BeTyp->DYNAMIC_LEFT  );
      BeTyp->DYNAMIC_RIGHT = (*VbhFunctionR)( BeTyp->DYNAMIC_RIGHT );

      if ( ( ! evalvexatomlong( BeTyp->DYNAMIC_LEFT , &Left  ) ) &&
           ( ! evalvexatomlong( BeTyp->DYNAMIC_RIGHT, &Right ) ) )
      {
        BeTyp->LEFT    = Left;
        BeTyp->RIGHT   = Right;
        BeTyp->DYNAMIC = 0;

        freevexexpr( BeTyp->DYNAMIC_LEFT  );
        freevexexpr( BeTyp->DYNAMIC_RIGHT );

        BeTyp->DYNAMIC_RIGHT = (vexexpr *)0;
        BeTyp->DYNAMIC_LEFT  = (vexexpr *)0;
      }
    }
  }

  for ( BePor  = Figure->BEPOR;
        BePor != (vbpor_list *)0;
        BePor  = BePor->NEXT )
  {
    BePor->TARGET = (*VbhFunctionL)( BePor->TARGET );
    if ( BePor->VEX != (vexexpr *)0 )
    {
      BePor->VEX = (*VbhFunctionR)( BePor->VEX );
    }
  }

  for ( BeGen  = Figure->BEGEN;
        BeGen != (vbgen_list *)0;
        BeGen  = BeGen->NEXT )
  {
    BeGen->TARGET = (*VbhFunctionL)( BeGen->TARGET );
    if ( BeGen->VEX != (vexexpr *)0 )
    {
      BeGen->VEX = (*VbhFunctionR)( BeGen->VEX );
    }
  }

  for ( BeMod  = Figure->BEMOD;
        BeMod != (vbmod_list *)0;
        BeMod  = BeMod->NEXT )
  {
    for ( BePor  = BeMod->BEPOR;
          BePor != (vbpor_list *)0;
          BePor  = BePor->NEXT )
    {
      BePor->TARGET = (*VbhFunctionL)( BePor->TARGET );
      if ( BePor->VEX != (vexexpr *)0 )
      {
        BePor->VEX = (*VbhFunctionR)( BePor->VEX );
      }
    }

    for ( BeGen  = BeMod->BEGEN;
          BeGen != (vbgen_list *)0;
          BeGen  = BeGen->NEXT )
    {
      BeGen->TARGET = (*VbhFunctionL)( BeGen->TARGET );
      if ( BeGen->VEX != (vexexpr *)0 )
      {
        BeGen->VEX = (*VbhFunctionR)( BeGen->VEX );
      }
    }
  }

  for ( BeCst  = Figure->BECST;
        BeCst != (vbcst_list *)0;
        BeCst  = BeCst->NEXT )
  {
    BeCst->TARGET = (*VbhFunctionL)( BeCst->TARGET );
    if ( BeCst->VEX != (vexexpr *)0 )
    {
      BeCst->VEX = (*VbhFunctionR)( BeCst->VEX );
    }
  }

  for ( BeFun  = Figure->BEFUN;
        BeFun != (vbfun_list *)0;
        BeFun  = BeFun->NEXT )
  {
    for ( BePar  = BeFun->ARGUMENT;
          BePar != (vbarg_list *)0;
          BePar  = BePar->NEXT )
    {
      BePar->TARGET = (*VbhFunctionL)( BePar->TARGET );
    }

    if ( BeFun->RETURN != (vbarg_list *)0 )
    {
      BePar = BeFun->RETURN;

      BePar->TARGET = (*VbhFunctionL)( BePar->TARGET );
    }

    for ( ScanPtype  = BeFun->VARIABLE;
          ScanPtype != (ptype_list *)0;
          ScanPtype  = ScanPtype->NEXT )
    {
      BeVar = (vbvar_list *)ScanPtype->DATA;

      BeVar->TARGET = (*VbhFunctionL)( BeVar->TARGET );
      if ( BeVar->VEX != (vexexpr *)0 )
      {
        BeVar->VEX = (*VbhFunctionR)( BeVar->VEX );
      }
    }

    vbh_simpvbinst( BeFun->INSTRUCTION );
  }

  loc_simpvbaux( Figure->BEAUX );
  loc_simpvbins( Figure->BEINS );
  loc_simpvbpcs( Figure->BEPCS );
  loc_simpvbgnr( Figure->BEGNR );
}

/*------------------------------------------------------------\
|                                                             |
|                      Simplify Vbh Figure                    |
|                                                             |
\------------------------------------------------------------*/

void vbh_simpvbfig( Figure )

  vbfig_list *Figure;
{
  vbh_forallvexvbfig( Figure, simpvexexpr, simpvexexpr );
}
