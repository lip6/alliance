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
| Tool    :                     Vbh                           |
|                                                             |
| File    :                   vbhview.c                       |
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

  static char *VBH_TIME_UNIT_NAME[ VBH_MAX_TIME_UNIT ] =
  {
    "UNKNOWN",
    "FS",
    "PS",
    "NS",
    "MS"
  };

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        View  Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    Local Get Vbh Type                       |
|                                                             |
\------------------------------------------------------------*/

static char *loc_getvbhtypename( Type )

    vbtyp_list *Type;
{
  static char NotDefined[] = "not defined !";

  if ( Type == (vbtyp_list *)0 )
  {
    return( NotDefined );
  }

  return( Type->NAME );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local View Vbh Port                      |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewvbpor( BePor  )

  vbpor_list *BePor;
{
  fprintf( stdout, "  > [%ld]", BePor->LINE );
  viewvexexprbound( BePor->TARGET );
  fprintf( stdout, " TYPE %s ", loc_getvbhtypename( BePor->TYPE ) );
  fprintf( stdout, " DIR  %c ", BePor->DIR );
  fprintf( stdout, " KIND %d ", BePor->KIND );
  if ( BePor->VEX != (vexexpr *)0 )
  {
    fprintf( stdout, " := " );
    viewvexexprbound( BePor->VEX );
  }
  fprintf( stdout, "\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local View Vbh Generic                   |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewvbgen( BeGen  )

  vbgen_list *BeGen;
{
  fprintf( stdout, "  > [%ld]", BeGen->LINE );
  viewvexexprbound( BeGen->TARGET );
  fprintf( stdout, " TYPE %s ", loc_getvbhtypename( BeGen->TYPE ) );
  if ( BeGen->VEX != (vexexpr *)0 )
  {
    fprintf( stdout, " := " );
    viewvexexprbound( BeGen->VEX );
  }
  fprintf( stdout, "\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local View Vbh Ifs                       |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewvbifs( ScanIfs  )

  vbifs_list  *ScanIfs;
{
  fprintf( stdout, "[%ld] ", ScanIfs->LINE );
  fprintf( stdout, "IF (" );
  viewvexexprbound( ScanIfs->CND );
  fprintf( stdout, ")\nTHEN\n" );

  if ( ScanIfs->CNDTRUE != (ptype_list *)0 )
  {
    vbh_viewvbinst( ScanIfs->CNDTRUE );
  }

  if ( ScanIfs->CNDFALSE != (ptype_list *)0 )
  {
    fprintf( stdout, "ELSE\n" );
    vbh_viewvbinst( ScanIfs->CNDFALSE );
  }
  fprintf( stdout, "END IF\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local View Vbh Var                       |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewvbvar( ScanVar  )

  vbvar_list  *ScanVar;
{
  fprintf( stdout, "[%ld] ", ScanVar->LINE );
  viewvexexprbound( ScanVar->TARGET );
  fprintf( stdout, " TYPE %s ", loc_getvbhtypename( ScanVar->TYPE ) );
  fprintf( stdout, " := " );
  viewvexexprbound( ScanVar->VEX );
  fprintf( stdout, "\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local View Vbh Asg                       |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewvbasg( ScanAsg  )

  vbasg_list  *ScanAsg;
{
  fprintf( stdout, "[%ld] ", ScanAsg->LINE );
  viewvexexprbound( ScanAsg->TARGET );
  fprintf( stdout, " TYPE %s ", loc_getvbhtypename( ScanAsg->TYPE ) );
  fprintf( stdout, " <= " );
  viewvexexprbound( ScanAsg->VEX );
  fprintf( stdout, "\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local View Vbh Case                      |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewvbcas( ScanCase  )

  vbcas_list  *ScanCase;
{
  vbcho_list    *ScanChoice;
  chain_list    *ScanChain;
  int            Offset;

  fprintf( stdout, "[%ld] ", ScanCase->LINE );
  fprintf( stdout, "CASE (" );
  viewvexexprbound( ScanCase->VEX );
  fprintf( stdout, ") IS\n" );

  for ( Offset = 0; Offset < ScanCase->SIZE; Offset++ )
  {
    ScanChoice = &ScanCase->CHOICE[ Offset ];

    fprintf( stdout, "WHEN " );

    for ( ScanChain  = ScanChoice->VALUES;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      fprintf( stdout, "%s ", (char *)ScanChain->DATA );

      if ( ScanChain->NEXT != (chain_list *)0 )
      {
        fprintf( stdout, " | " );
      }
    }

    fprintf( stdout, "=> " );

    vbh_viewvbinst( ScanChoice->INSTRUCTION );
  }

  fprintf( stdout, "END CASE\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local View Vbh Wait                      |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewvbwas( ScanWait  )

  vbwas_list  *ScanWait;
{
  chain_list *ScanSens;

  fprintf( stdout, "[%ld] ", ScanWait->LINE );
  fprintf( stdout, "WAIT" );

  if ( ScanWait->SEN != (chain_list *)0 )
  {
    fprintf( stdout, " ON " );
    for ( ScanSens  = ScanWait->SEN;
          ScanSens != (chain_list *)0;
          ScanSens  = ScanSens->NEXT )
    {
      fprintf( stdout, "%s", (char *)ScanSens->DATA );
      if ( ScanSens->NEXT != (chain_list *)0 )
      {
        fprintf( stdout, ", " );
      }
    }
  }

  if ( ScanWait->CND != (vexexpr *)0 )
  {
    fprintf( stdout, " UNTIL " );
    viewvexexprbound( ScanWait->CND );
  }

  if ( ScanWait->TIMEOUT != (vexexpr *)0 )
  {
    fprintf( stdout, " FOR " );
    viewvexexprbound( ScanWait->TIMEOUT );

    if ( ScanWait->TIME_UNIT < VBH_MAX_TIME_UNIT )
    {
      fprintf( stdout, " %s", VBH_TIME_UNIT_NAME[ ScanWait->TIME_UNIT ] );
    }
    else
    {
      fprintf( stdout, " unknown (%d)", ScanWait->TIME_UNIT );
    }
  }

  fprintf( stdout, "\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local View Vbh For                       |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewvbfor( ScanFor )

  vbfor_list  *ScanFor;
{
  fprintf( stdout, "[%ld] ", ScanFor->LINE );
  fprintf( stdout, "%s : ", ScanFor->LABEL );

  fprintf( stdout, "FOR " );
  viewvexexprbound( ScanFor->VARIABLE );
  fprintf( stdout, "IN " );

  viewvexexprbound( ScanFor->LEFT );
  if ( ScanFor->UP ) fprintf( stdout, " TO "     );
  else               fprintf( stdout, " DOWNTO " );
  viewvexexprbound( ScanFor->RIGHT );

  fprintf( stdout, "\nLOOP\n" );
  
  vbh_viewvbinst( ScanFor->INSTRUCTION );

  fprintf( stdout, "END LOOP %s\n", ScanFor->LABEL );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local View Vbh Loop                      |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewvblop( ScanLoop )

  vblop_list  *ScanLoop;
{
  fprintf( stdout, "[%ld] ", ScanLoop->LINE );
  fprintf( stdout, "%s : ", ScanLoop->LABEL );

  fprintf( stdout, "LOOP\n" );
  
  vbh_viewvbinst( ScanLoop->INSTRUCTION );

  fprintf( stdout, "END LOOP %s\n", ScanLoop->LABEL );
  fprintf( stdout, "\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local View Vbh While                     |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewvbwhi( ScanWhile )

  vbwhi_list  *ScanWhile;
{
  fprintf( stdout, "[%ld] ", ScanWhile->LINE );
  fprintf( stdout, "%s : ", ScanWhile->LABEL );

  fprintf( stdout, "WHILE ( " );
  viewvexexprbound( ScanWhile->CND );
  fprintf( stdout, " )\n" );
  fprintf( stdout, "LOOP\n" );
  
  vbh_viewvbinst( ScanWhile->INSTRUCTION );

  fprintf( stdout, "END LOOP %s\n", ScanWhile->LABEL );
  fprintf( stdout, "\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local View Vbh Aggregate                 |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewvbagr( ScanAggreg )

  vbagr_list  *ScanAggreg;
{
  fprintf( stdout, "[%ld] ", ScanAggreg->LINE );
  fprintf( stdout, "AGGREGATE (\n" );
  vbh_viewvbinst( ScanAggreg->ASSIGN );
  fprintf( stdout, ")\n\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local View Vbh Exit                      |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewvbext( ScanExit  )

  vbext_list  *ScanExit;
{
  fprintf( stdout, "[%ld] ", ScanExit->LINE );
  fprintf( stdout, "EXIT" );

  if ( ScanExit->LABEL != (char *)0 )
  {
    fprintf( stdout, " %s", ScanExit->LABEL );
  }

  if ( ScanExit->CND != (vexexpr *)0 )
  {
    fprintf( stdout," WHEN ( " );
    viewvexexprbound( ScanExit->CND );
    fprintf( stdout, " )\n" );
  }
  else
  {
    fprintf( stdout, "\n" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    Local View Vbh Next                      |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewvbnxt( ScanNext  )

  vbnxt_list  *ScanNext;
{
  fprintf( stdout, "[%ld] ", ScanNext->LINE );
  fprintf( stdout, "NEXT" );

  if ( ScanNext->LABEL != (char *)0 )
  {
    fprintf( stdout, " %s", ScanNext->LABEL );
  }

  if ( ScanNext->CND != (vexexpr *)0 )
  {
    fprintf( stdout," WHEN ( " );
    viewvexexprbound( ScanNext->CND );
    fprintf( stdout, " )\n" );
  }
  else
  {
    fprintf( stdout, "\n" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    Local View Vbh Return                    |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewvbret( ScanReturn  )

  vbret_list  *ScanReturn;
{
  fprintf( stdout, "[%ld] ", ScanReturn->LINE );
  fprintf( stdout, "RETURN " );

  viewvexexprbound( ScanReturn->RET );

  fprintf( stdout, "\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local View Vbh Call                      |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewvbcal( ScanCall  )

  vbcal_list  *ScanCall;
{
  fprintf( stdout, "[%ld] ", ScanCall->LINE );
  fprintf( stdout, "CALL " );

  viewvexexprbound( ScanCall->CALL );

  fprintf( stdout, "\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local View Vbh Instruction               |
|                                                             |
\------------------------------------------------------------*/

void vbh_viewvbinst( Instruction )

  ptype_list *Instruction;
{
  while ( Instruction != (ptype_list *)0 )
  {
    switch ( Instruction->TYPE )
    {
      case VBH_BEIFS : loc_viewvbifs( Instruction->DATA );
      break;

      case VBH_BEASG : loc_viewvbasg( Instruction->DATA );
      break;

      case VBH_BEVAR : loc_viewvbvar( Instruction->DATA );
      break;

      case VBH_BECAS : loc_viewvbcas( Instruction->DATA );
      break;

      case VBH_BEWAS : loc_viewvbwas( Instruction->DATA );
      break;

      case VBH_BEFOR : loc_viewvbfor( Instruction->DATA );
      break;

      case VBH_BEWHI : loc_viewvbwhi( Instruction->DATA );
      break;

      case VBH_BELOP : loc_viewvblop( Instruction->DATA );
      break;

      case VBH_BENXT : loc_viewvbnxt( Instruction->DATA );
      break;

      case VBH_BEEXT : loc_viewvbext( Instruction->DATA );
      break;

      case VBH_BERET : loc_viewvbret( Instruction->DATA );
      break;

      case VBH_BECAL : loc_viewvbcal( Instruction->DATA );
      break;

      case VBH_BEAGR : loc_viewvbagr( Instruction->DATA );
      break;

      default        : fprintf( stdout, "ERROR unknown instruction %ld\n",
                                Instruction->TYPE );
    }

    Instruction = Instruction->NEXT;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                         View Vbh Type                       |
|                                                             |
\------------------------------------------------------------*/

void vbh_viewvbtyp( BeTyp )

  vbtyp_list *BeTyp;
{
  int Index;

  fprintf( stdout, " > [%ld]\n", BeTyp->LINE );
  fprintf( stdout, "   NAME   : %s\n",      BeTyp->NAME  ); 
  fprintf( stdout, "   LEFT   : %d\n", (int)BeTyp->LEFT  ); 
  fprintf( stdout, "   RIGHT  : %d\n", (int)BeTyp->RIGHT ); 
  fprintf( stdout, "   VALUE  : \n" );

  for ( Index = 0; Index < BeTyp->SIZE; Index++ )
  {
    fprintf( stdout, "%s ", BeTyp->VALUE[ Index ] );
  }

  if ( BeTyp->SIZE != 0 ) fprintf( stdout, "\n" );

  fprintf( stdout, "   SIZE   : %d\n", BeTyp->SIZE   );
  fprintf( stdout, "   BYTE   : %d\n", BeTyp->BYTE   );
  fprintf( stdout, "   CLASS  : %c\n", BeTyp->CLASS  );
  fprintf( stdout, "   INDEX  : %d\n", BeTyp->INDEX  );

  if ( BeTyp->BASE != (vbtyp_list *)0 )
  {
    fprintf( stdout, "   BASE   : %s\n", BeTyp->BASE->NAME );
  }
  else
  {
    fprintf( stdout, "   BASE   : none\n" );
  }

  if ( BeTyp->RESOLV != (vbfun_list *)0 )
  {
    fprintf( stdout, "   RESOLV : %s\n", BeTyp->RESOLV->NAME );
  }
  else
  {
    fprintf( stdout, "   RESOLV : none\n" );
  }

  if ( BeTyp->DYNAMIC )
  {
    fprintf( stdout, "   DYNAMIC: %lx %lx ",
             (long)BeTyp->DYNAMIC_LEFT, (long)BeTyp->DYNAMIC_RIGHT );

    viewvexexprbound( BeTyp->DYNAMIC_LEFT );
    if ( BeTyp->DYNAMIC == VBH_DYNAMIC_TO ) fprintf( stdout, " TO "     );
    else                                    fprintf( stdout, " DOWNTO " );
    viewvexexprboundln( BeTyp->DYNAMIC_RIGHT );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                         View Vbh Instance                   |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewvbins( BeIns )

  vbins_list *BeIns;
{
  vbmap_list *BeMap;

  fprintf( stdout, "  > [%ld]", BeIns->LINE );
  fprintf( stdout, " NAME   %s ", BeIns->NAME   );
  fprintf( stdout, " MODEL  %s\n", BeIns->MODEL  );
  fprintf( stdout, " PORT_MAP\n" );

  for ( BeMap  = BeIns->PORT_MAP;
        BeMap != (vbmap_list *)0;
        BeMap  = BeMap->NEXT )
  {
    fprintf( stdout, "  > [%ld] FORMAL: ", BeMap->LINE );
    viewvexexprbound( BeMap->FORMAL );
    fprintf( stdout, " ACTUAL: " );
    viewvexexprboundln( BeMap->ACTUAL );
  }

  fprintf( stdout, "\n" );

  fprintf( stdout, " GEN_MAP\n" );

  for ( BeMap  = BeIns->GEN_MAP;
        BeMap != (vbmap_list *)0;
        BeMap  = BeMap->NEXT )
  {
    fprintf( stdout, "  > [%ld] FORMAL: ", BeMap->LINE );
    viewvexexprbound( BeMap->FORMAL );
    fprintf( stdout, " ACTUAL: " );
    viewvexexprboundln( BeMap->ACTUAL );
  }

  fprintf( stdout, "\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                         View Vbh Aux                        |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewvbaux( BeAux )

  vbaux_list *BeAux;
{
  fprintf( stdout, "  > [%ld]", BeAux->LINE );
  viewvexexprbound( BeAux->TARGET );
  fprintf( stdout, " TYPE %s ", loc_getvbhtypename( BeAux->TYPE ) );
  fprintf( stdout, " KIND %d ", (int)BeAux->KIND );
  if ( BeAux->VEX != (vexexpr *)0 )
  {
    fprintf( stdout, " := " );
    viewvexexprbound( BeAux->VEX );
  }
  fprintf( stdout, "\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                         View Vbh Pcs                        |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewvbpcs( BePcs )

  vbpcs_list *BePcs;
{
  ptype_list *ScanPtype;
  vbvar_list *BeVar;

  fprintf( stdout, "  > [%ld]\n", BePcs->LINE );

  if ( BePcs->LABEL != (char *)0 )
  {
    fprintf( stdout, "LABEL : %s\n", BePcs->LABEL );
  }

  fprintf( stdout, "TYPE  : %x\n", (int)BePcs->TYPE );

  fprintf( stdout, "VARIABLE\n" );

  for ( ScanPtype  = BePcs->VARIABLE;
        ScanPtype != (ptype_list *)0;
        ScanPtype  = ScanPtype->NEXT )
  {
    BeVar = (vbvar_list *)ScanPtype->DATA;

    fprintf( stdout, "  > [%ld]\n", BeVar->LINE );
    fprintf( stdout, " TYPE %s ", loc_getvbhtypename( BeVar->TYPE ) );
    viewvexexprbound( BeVar->TARGET );
    if ( BeVar->VEX != (vexexpr *)0 )
    {
      fprintf( stdout, " := " );
      viewvexexprbound( BeVar->VEX );
    }
    fprintf( stdout, "\n" );
  }

  fprintf( stdout, "INSTRUCTION\n" );

  vbh_viewvbinst( BePcs->INSTRUCTION );
}

/*------------------------------------------------------------\
|                                                             |
|                         View Vbh Gnr                        |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewvbgnr( BeGnr )

  vbgnr_list *BeGnr;
{
  vbins_list *BeIns;
  vbpcs_list *BePcs;

  fprintf( stdout, "  > [%ld]\n", BeGnr->LINE );

  if ( BeGnr->LABEL != (char *)0 )
  {
    fprintf( stdout, "LABEL : %s\n", BeGnr->LABEL );
  }

  fprintf( stdout, "TYPE  : %x\n", (int)BeGnr->TYPE );

  if ( BeGnr->TYPE == VBH_GENERATE_IF )
  {
    fprintf( stdout, "IF (" );
    viewvexexprbound( BeGnr->IF_COND );
    fprintf( stdout, ")" );
  }
  else
  {
    fprintf( stdout, "FOR " );
    viewvexexprbound( BeGnr->FOR_VARIABLE );
    fprintf( stdout, " IN " );

    viewvexexprbound( BeGnr->FOR_LEFT );
    if ( BeGnr->FOR_UP ) fprintf( stdout, " TO "     );
    else                 fprintf( stdout, " DOWNTO " );
    viewvexexprbound( BeGnr->FOR_RIGHT );
  }

  fprintf( stdout, "\nGENERATE\n" );
  
  fprintf( stdout, "BEINS\n" );

  for ( BeIns  = BeGnr->BEINS;
        BeIns != (vbins_list *)0;
        BeIns  = BeIns->NEXT )
  {
    loc_viewvbins( BeIns );
  }

  fprintf( stdout, "BEPCS\n" );
 
  for ( BePcs  = BeGnr->BEPCS;
        BePcs != (vbpcs_list *)0;
        BePcs  = BePcs->NEXT )
  {
    loc_viewvbpcs( BePcs );
  }

  fprintf( stdout, "BEGNR\n" );
 
  for ( BeGnr  = BeGnr->BEGNR;
        BeGnr != (vbgnr_list *)0;
        BeGnr  = BeGnr->NEXT )
  {
    loc_viewvbgnr( BeGnr );
  }

  fprintf( stdout, "\nEND GENERATE\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                         View Vbh Figure                     |
|                                                             |
\------------------------------------------------------------*/

void vbh_viewvbfig( Figure )

  vbfig_list *Figure;
{
  vbgnr_list *BeGnr;
  vbgen_list *BeGen;
  vbins_list *BeIns;
  vbmod_list *BeMod;
  vbaux_list *BeAux;
  vbatr_list *BeAtr;
  vbcst_list *BeCst;
  vbpor_list *BePor;
  vbpcs_list *BePcs;
  vbfun_list *BeFun;
  vbarg_list *BePar;
  vbvar_list *BeVar;
  vbtyp_list *BeTyp;
  chain_list *ScanPack;
  ptype_list *ScanPtype;

  fprintf( stdout, "NAME: %s\n", Figure->NAME );
  fprintf( stdout, "FLAG: %d\n", Figure->FLAG );
  fprintf( stdout, "PACK: %d\n", Figure->IS_PACKAGE );

  fprintf( stdout, "PACK_LIST\n" );

  for ( ScanPack  = Figure->PACK_LIST;
        ScanPack != (chain_list *)0;
        ScanPack  = ScanPack->NEXT )
  {
    fprintf( stdout, "%s\n", (char *)ScanPack->DATA );
  }

  fprintf( stdout, "BETYP\n" );

  for ( BeTyp  = Figure->BETYP;
        BeTyp != (vbtyp_list *)0;
        BeTyp  = BeTyp->NEXT )
  {
    vbh_viewvbtyp( BeTyp );
  }

  fprintf( stdout, "BEPOR\n" );

  for ( BePor  = Figure->BEPOR;
        BePor != (vbpor_list *)0;
        BePor  = BePor->NEXT )
  {
    loc_viewvbpor( BePor );
  }

  fprintf( stdout, "BEGEN\n" );

  for ( BeGen  = Figure->BEGEN;
        BeGen != (vbgen_list *)0;
        BeGen  = BeGen->NEXT )
  {
    loc_viewvbgen( BeGen );
  }

  fprintf( stdout, "BEMOD\n" );

  for ( BeMod  = Figure->BEMOD;
        BeMod != (vbmod_list *)0;
        BeMod  = BeMod->NEXT )
  {
    fprintf( stdout, "  > [%ld]", BeMod->LINE );
    fprintf( stdout, " NAME   %s\n", BeMod->NAME   );
    fprintf( stdout, " BEPOR\n" );

    for ( BePor  = BeMod->BEPOR;
          BePor != (vbpor_list *)0;
          BePor  = BePor->NEXT )
    {
      loc_viewvbpor( BePor );
    }

    fprintf( stdout, " BEGEN\n" );

    for ( BeGen  = BeMod->BEGEN;
          BeGen != (vbgen_list *)0;
          BeGen  = BeGen->NEXT )
    {
      loc_viewvbgen( BeGen );
    }

    fprintf( stdout, "\n" );
  }

  fprintf( stdout, "BEATR\n" );

  for ( BeAtr  = Figure->BEATR;
        BeAtr != (vbatr_list *)0;
        BeAtr  = BeAtr->NEXT )
  {
    fprintf( stdout, "  > [%ld]", BeAtr->LINE );
    fprintf( stdout, " NAME   %s ", BeAtr->NAME   );
    fprintf( stdout, " ENTITY %s ", BeAtr->ENTITY );
    fprintf( stdout, " TYPE   %d ", BeAtr->TYPE   );
    fprintf( stdout, " VALUE  %s ", BeAtr->VALUE  );

    fprintf( stdout, "\n" );
  }

  fprintf( stdout, "BECST\n" );

  for ( BeCst  = Figure->BECST;
        BeCst != (vbcst_list *)0;
        BeCst  = BeCst->NEXT )
  {
    fprintf( stdout, "  > [%ld]", BeCst->LINE );
    viewvexexprbound( BeCst->TARGET );
    fprintf( stdout, " TYPE %s ", loc_getvbhtypename( BeCst->TYPE ) );
    if ( BeCst->VEX != (vexexpr *)0 )
    {
      fprintf( stdout, " := " );
      viewvexexprbound( BeCst->VEX );
    }
    fprintf( stdout, "\n" );
  }

  fprintf( stdout, "BEFUN\n" );
 
  for ( BeFun  = Figure->BEFUN;
        BeFun != (vbfun_list *)0;
        BeFun  = BeFun->NEXT )
  {
    fprintf( stdout, "  > [%ld]\n", BeFun->LINE );
    fprintf( stdout, "    NAME : %s\n", BeFun->NAME );
    fprintf( stdout, "    TYPE : %x\n", (int)BeFun->TYPE );

    fprintf( stdout, "ARGUMENT\n" );

    for ( BePar  = BeFun->ARGUMENT;
          BePar != (vbarg_list *)0;
          BePar  = BePar->NEXT )
    {
      fprintf( stdout, "  > [%ld]\n", BePar->LINE );
      viewvexexprbound( BePar->TARGET );
      fprintf( stdout, " TYPE  %s ", loc_getvbhtypename( BePar->TYPE ) );
      fprintf( stdout, " DIR   %c ", BePar->DIR );
      fprintf( stdout, " CLASS %d ", BePar->CLASS );
      fprintf( stdout, "\n" );
    }

    if ( BeFun->RETURN != (vbarg_list *)0 )
    {
      BePar = BeFun->RETURN;

      fprintf( stdout, "  > [%ld]\n", BePar->LINE );
      fprintf( stdout, "RETURN\n" );
     
      fprintf( stdout, "  < " );
      viewvexexprbound( BePar->TARGET );
      fprintf( stdout, " TYPE %s ", loc_getvbhtypename( BePar->TYPE ) );
      fprintf( stdout, "\n" );
    }

    fprintf( stdout, "VARIABLE\n" );

    for ( ScanPtype  = BeFun->VARIABLE;
          ScanPtype != (ptype_list *)0;
          ScanPtype  = ScanPtype->NEXT )
    {
      BeVar = (vbvar_list *)ScanPtype->DATA;

      fprintf( stdout, "  > [%ld]\n", BeVar->LINE );
      fprintf( stdout, " TYPE %s ", loc_getvbhtypename( BeVar->TYPE ) );
      viewvexexprbound( BeVar->TARGET );
      if ( BeVar->VEX != (vexexpr *)0 )
      {
        fprintf( stdout, " := " );
        viewvexexprbound( BeVar->VEX );
      }
      fprintf( stdout, "\n" );
    }

    fprintf( stdout, "INSTRUCTION\n" );

    vbh_viewvbinst( BeFun->INSTRUCTION );
  }

  fprintf( stdout, "BEINS\n" );

  for ( BeIns  = Figure->BEINS;
        BeIns != (vbins_list *)0;
        BeIns  = BeIns->NEXT )
  {
    loc_viewvbins( BeIns );
  }

  fprintf( stdout, "BEAUX\n" );

  for ( BeAux  = Figure->BEAUX;
        BeAux != (vbaux_list *)0;
        BeAux  = BeAux->NEXT )
  {
    loc_viewvbaux( BeAux );
  }

  fprintf( stdout, "BEPCS\n" );
 
  for ( BePcs  = Figure->BEPCS;
        BePcs != (vbpcs_list *)0;
        BePcs  = BePcs->NEXT )
  {
    loc_viewvbpcs( BePcs );
  }

  fprintf( stdout, "BEGNR\n" );
 
  for ( BeGnr  = Figure->BEGNR;
        BeGnr != (vbgnr_list *)0;
        BeGnr  = BeGnr->NEXT )
  {
    loc_viewvbgnr( BeGnr );
  }
}
