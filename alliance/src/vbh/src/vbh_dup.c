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

#include "mut.h" 
#include "aut.h"
#include "vex.h" 
#include "vbh.h" 

/*------------------------------------------------------------\
|                                                             |
|                      Local Variables                        |
|                                                             |
\------------------------------------------------------------*/

  static vbfig_list *LocalDupFigure = (vbfig_list *)0;
  static authtable  *LocalHashTable = (authtable  *)0;

  static ptype_list *loc_dupvbinst();

/*------------------------------------------------------------\
|                                                             |
|                    Local  Dup Vbh Port                      |
|                                                             |
\------------------------------------------------------------*/

static vbpor_list *loc_dupvbpor( DupPor, BePor  )

  vbpor_list *DupPor;
  vbpor_list *BePor;
{
  vbtyp_list *DupType;

  DupType = vbh_getvbtyp( LocalDupFigure, BePor->TYPE->NAME );
  DupPor  = vbh_addvbpor( DupPor, dupvexexpr( BePor->TARGET ),
                  dupvexexpr( BePor->VEX ), BePor->DIR, BePor->KIND, DupType, BePor->LINE );

  return( DupPor );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local  Dup Vbh Generic                   |
|                                                             |
\------------------------------------------------------------*/

static vbgen_list *loc_dupvbgen( DupGen, BeGen  )

  vbgen_list *DupGen;
  vbgen_list *BeGen;
{
  vbtyp_list *DupType;
  
  DupType = vbh_getvbtyp( LocalDupFigure, BeGen->TYPE->NAME );
  DupGen  = vbh_addvbgen( DupGen, dupvexexpr( BeGen->TARGET ),
                 dupvexexpr( BeGen->VEX ), DupType, BeGen->LINE );

  return( DupGen );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local  Dup Vbh Ifs                       |
|                                                             |
\------------------------------------------------------------*/

static ptype_list *loc_dupvbifs( PType, ScanIfs  )

  ptype_list  *PType;
  vbifs_list  *ScanIfs;
{
  vbifs_list *DupIfs;

  PType  = vbh_addvbifs( PType, dupvexexpr( ScanIfs->CND ), ScanIfs->LINE );
  DupIfs = (vbifs_list *)PType->DATA;

  DupIfs->CNDTRUE  = loc_dupvbinst( ScanIfs->CNDTRUE  );
  DupIfs->CNDFALSE = loc_dupvbinst( ScanIfs->CNDFALSE );


  return( PType );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local  Dup Vbh Var                       |
|                                                             |
\------------------------------------------------------------*/

static ptype_list *loc_dupvbvar( PType, ScanVar  )

  ptype_list  *PType;
  vbvar_list  *ScanVar;
{
  vbtyp_list *DupType;

  DupType = vbh_getvbtyp( LocalDupFigure, ScanVar->TYPE->NAME );
  PType   = vbh_addvbvar( PType, dupvexexpr( ScanVar->TARGET ), 
                       dupvexexpr( ScanVar->VEX    ), DupType, ScanVar->LINE );
  return( PType );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local  Dup Vbh Asg                       |
|                                                             |
\------------------------------------------------------------*/

static ptype_list *loc_dupvbasg( PType, ScanAsg  )

  ptype_list  *PType;
  vbasg_list  *ScanAsg;
{
  vbtyp_list *DupType;

  DupType = vbh_getvbtyp( LocalDupFigure, ScanAsg->TYPE->NAME );
  PType   = vbh_addvbasg( PType, dupvexexpr( ScanAsg->TARGET ), 
                       dupvexexpr( ScanAsg->VEX    ), DupType, ScanAsg->LINE );
  return( PType );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local  Dup Vbh Case                      |
|                                                             |
\------------------------------------------------------------*/

static ptype_list *loc_dupvbcas( PType, ScanCase  )

  ptype_list  *PType;
  vbcas_list  *ScanCase;
{
  vbtyp_list *DupType;
  vbcas_list *DupCase;
  vbcho_list *DupChoice;
  vbcho_list *ScanChoice;
  chain_list *ScanChain;
  int         Offset;

  DupType = vbh_getvbtyp( LocalDupFigure, ScanCase->TYPE->NAME );
  PType   = vbh_addvbcas( PType, dupvexexpr( ScanCase->VEX ), DupType, ScanCase->LINE );


  DupCase   = (vbcas_list *)PType->DATA;
  DupChoice = (vbcho_list *)autallocblock( sizeof( vbcho_list ) * ScanCase->SIZE );

  DupCase->SIZE   = ScanCase->SIZE;
  DupCase->CHOICE = DupChoice;

  for ( Offset = 0; Offset < ScanCase->SIZE; Offset++ )
  {
    ScanChoice = &ScanCase->CHOICE[ Offset ];
    DupChoice  = &DupCase->CHOICE[ Offset ];

    DupChoice->VALUES = (chain_list *)0;

    for ( ScanChain  = ScanChoice->VALUES;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      DupChoice->VALUES = addchain( DupChoice->VALUES, ScanChain->DATA );
    }

    DupChoice->SIZE        = ScanChoice->SIZE;
    DupChoice->INSTRUCTION = loc_dupvbinst( ScanChoice->INSTRUCTION );
    DupChoice->LINE        = ScanChoice->LINE;
  }

  return( PType );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local  Dup Vbh Wait                      |
|                                                             |
\------------------------------------------------------------*/

static ptype_list *loc_dupvbwas( PType, ScanWait  )

  ptype_list  *PType;
  vbwas_list  *ScanWait;
{
  vbwas_list *DupWait;
  chain_list *ScanSens;
  chain_list *DupSens;

  DupSens = (chain_list *)0;

  for ( ScanSens  = ScanWait->SEN;
        ScanSens != (chain_list *)0;
        ScanSens  = ScanSens->NEXT )
  {
    DupSens = addchain( DupSens, ScanSens->DATA );
  }

  DupWait = vbh_addvbwas( DupSens, dupvexexpr( ScanWait->CND ), 
        ScanWait->TIME_UNIT, dupvexexpr( ScanWait->TIMEOUT ), ScanWait->LINE );

  PType = addptype( PType, VBH_BEWAS, (void *)DupWait );

  return( PType );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local  Dup Vbh For                       |
|                                                             |
\------------------------------------------------------------*/

static ptype_list *loc_dupvbfor( PType, ScanFor )

  ptype_list  *PType;
  vbfor_list  *ScanFor;
{
  vbfor_list *DupFor;

  PType = vbh_addvbfor( PType, ScanFor->LABEL, 
            dupvexexpr( ScanFor->VARIABLE ), 
            dupvexexpr( ScanFor->LEFT     ),
            dupvexexpr( ScanFor->RIGHT    ), ScanFor->UP, ScanFor->LINE );

  DupFor = (vbfor_list *)PType->DATA;
  DupFor->INSTRUCTION = loc_dupvbinst( ScanFor->INSTRUCTION );

  return( PType );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local  Dup Vbh Loop                      |
|                                                             |
\------------------------------------------------------------*/

static ptype_list *loc_dupvblop( PType, ScanLoop )

  ptype_list  *PType;
  vblop_list  *ScanLoop;
{
  vblop_list *DupLoop;

  PType = vbh_addvblop( PType, ScanLoop->LABEL, ScanLoop->LINE );

  DupLoop = (vblop_list *)PType->DATA;
  DupLoop->INSTRUCTION = loc_dupvbinst( ScanLoop->INSTRUCTION );

  return( PType );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local  Dup Vbh While                     |
|                                                             |
\------------------------------------------------------------*/

static ptype_list *loc_dupvbwhi( PType, ScanWhile )

  ptype_list  *PType;
  vbwhi_list  *ScanWhile;
{
  vbwhi_list *DupWhile;

  PType = vbh_addvbwhi( PType, 
      ScanWhile->LABEL, dupvexexpr( ScanWhile->CND ), ScanWhile->LINE );

  DupWhile = (vbwhi_list *)PType->DATA;
  DupWhile->INSTRUCTION = loc_dupvbinst( ScanWhile->INSTRUCTION );

  return( PType );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local  Dup Vbh Aggregate                 |
|                                                             |
\------------------------------------------------------------*/

static ptype_list *loc_dupvbagr( PType, ScanAggreg )

  ptype_list  *PType;
  vbagr_list  *ScanAggreg;
{
  PType = vbh_addvbagr( PType, loc_dupvbinst( ScanAggreg->ASSIGN, ScanAggreg->LINE ) );

  return( PType );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local  Dup Vbh Exit                      |
|                                                             |
\------------------------------------------------------------*/

static ptype_list *loc_dupvbext( PType, ScanExit  )

  ptype_list  *PType;
  vbext_list  *ScanExit;
{
  PType = addptype( PType, VBH_BEEXT, 
             vbh_addvbext( ScanExit->LABEL, dupvexexpr( ScanExit->CND ), ScanExit->LINE ) );

  return( PType );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local  Dup Vbh Next                      |
|                                                             |
\------------------------------------------------------------*/

static ptype_list *loc_dupvbnxt( PType, ScanNext  )

  ptype_list  *PType;
  vbnxt_list  *ScanNext;
{

  PType = addptype( PType, VBH_BENXT, 
             vbh_addvbnxt( ScanNext->LABEL, dupvexexpr( ScanNext->CND ), ScanNext->LINE ) );

  return( PType );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local  Dup Vbh Return                    |
|                                                             |
\------------------------------------------------------------*/

static ptype_list *loc_dupvbret( PType, ScanReturn  )

  ptype_list  *PType;
  vbret_list  *ScanReturn;
{
  PType = addptype( PType, VBH_BERET,
      vbh_addvbret( dupvexexpr( ScanReturn->RET ), ScanReturn->LINE ) );

  return( PType );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local  Dup Vbh Call                      |
|                                                             |
\------------------------------------------------------------*/

static ptype_list *loc_dupvbcal( PType, ScanCall  )

  ptype_list  *PType;
  vbcal_list  *ScanCall;
{
  PType = addptype( PType, VBH_BECAL,
      vbh_addvbcal( dupvexexpr( ScanCall->CALL ), ScanCall->LINE ) );

  return( PType );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local  Dup Vbh Instruction               |
|                                                             |
\------------------------------------------------------------*/

static ptype_list *loc_dupvbinst( Instruction )

  ptype_list *Instruction;
{
  ptype_list *DupInstruction;

  DupInstruction = (ptype_list *)0;

  while ( Instruction != (ptype_list *)0 )
  {
    switch ( Instruction->TYPE )
    {
      case VBH_BEIFS : DupInstruction = loc_dupvbifs( DupInstruction, Instruction->DATA );
      break;

      case VBH_BEASG : DupInstruction = loc_dupvbasg( DupInstruction, Instruction->DATA );
      break;

      case VBH_BEVAR : DupInstruction = loc_dupvbvar( DupInstruction, Instruction->DATA );
      break;

      case VBH_BECAS : DupInstruction = loc_dupvbcas( DupInstruction, Instruction->DATA );
      break;

      case VBH_BEWAS : DupInstruction = loc_dupvbwas( DupInstruction, Instruction->DATA );
      break;

      case VBH_BEFOR : DupInstruction = loc_dupvbfor( DupInstruction, Instruction->DATA );
      break;

      case VBH_BEWHI : DupInstruction = loc_dupvbwhi( DupInstruction, Instruction->DATA );
      break;

      case VBH_BELOP : DupInstruction = loc_dupvblop( DupInstruction, Instruction->DATA );
      break;

      case VBH_BENXT : DupInstruction = loc_dupvbnxt( DupInstruction, Instruction->DATA );
      break;

      case VBH_BEEXT : DupInstruction = loc_dupvbext( DupInstruction, Instruction->DATA );
      break;

      case VBH_BERET : DupInstruction = loc_dupvbret( DupInstruction, Instruction->DATA );
      break;

      case VBH_BECAL : DupInstruction = loc_dupvbcal( DupInstruction, Instruction->DATA );
      break;

      case VBH_BEAGR : DupInstruction = loc_dupvbagr( DupInstruction, Instruction->DATA );
      break;
    }

    Instruction = Instruction->NEXT;
  }

  DupInstruction = (ptype_list *)reverse( (chain_list *)DupInstruction );

  return ( DupInstruction );
}

/*------------------------------------------------------------\
|                                                             |
|                          Dup Vbh Type                       |
|                                                             |
\------------------------------------------------------------*/

static void loc_dupvbtyp( Figure, DupFigure )

  vbfig_list *Figure;
  vbfig_list *DupFigure;
{
  vbtyp_list *BeTyp;
  vbtyp_list *DupTyp;
  vbtyp_list *DupTypeBase;
  int         DupTypeIndex;

  DupTypeIndex = 0;

  for ( DupTyp  = DupFigure->BETYP;
        DupTyp != (vbtyp_list *)0;
        DupTyp  = DupTyp->NEXT )
  {
    DupTypeIndex++;
  }

  for ( BeTyp  = Figure->BETYP;
        BeTyp != (vbtyp_list *)0;
        BeTyp  = BeTyp->NEXT )
  {
    if ( BeTyp->INDEX < VBH_MAX_TYPE ) continue;
  
    DupTyp = vbh_getvbtyp( DupFigure, BeTyp->NAME );
  
    if ( DupTyp != (vbtyp_list *)0 ) continue;
  
    if ( BeTyp->BASE != (vbtyp_list *)0 )
    {
      DupTypeBase = vbh_getvbtyp( DupFigure, BeTyp->BASE->NAME );
    }
    else
    {
      DupTypeBase = (vbtyp_list *)0;
    }

    DupTyp = vbh_addvbtyp( LocalDupFigure, 
            BeTyp->NAME, BeTyp->LEFT, BeTyp->RIGHT, BeTyp->VALUE, BeTyp->SIZE, 
            BeTyp->BYTE,
            DupTypeBase, DupTypeIndex, (vbfun_list *)0, BeTyp->CLASS, BeTyp->LINE );

    DupTyp->DYNAMIC       = BeTyp->DYNAMIC;
    DupTyp->DYNAMIC_LEFT  = dupvexexpr( BeTyp->DYNAMIC_LEFT  );
    DupTyp->DYNAMIC_RIGHT = dupvexexpr( BeTyp->DYNAMIC_RIGHT );

    DupTypeIndex++;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          Dup Vbh Map                        |
|                                                             |
\------------------------------------------------------------*/

static vbmap_list *loc_dupvbmap( DupMap, BeMap )

  vbmap_list *DupMap;
  vbmap_list *BeMap;
{
  DupMap = vbh_addvbmap( DupMap, 
               dupvexexpr( BeMap->FORMAL ), dupvexexpr( BeMap->ACTUAL ), BeMap->LINE );

  return( DupMap );
}


/*------------------------------------------------------------\
|                                                             |
|                          Dup Vbh Instance                   |
|                                                             |
\------------------------------------------------------------*/

static vbins_list *loc_dupvbins( DupIns, BeIns )

  vbins_list *DupIns;
  vbins_list *BeIns;
{
  vbmap_list *BeMap;

  DupIns = vbh_addvbins( DupIns, BeIns->NAME, BeIns->MODEL, BeIns->LINE );

  for ( BeMap  = BeIns->PORT_MAP;
        BeMap != (vbmap_list *)0;
        BeMap  = BeMap->NEXT )
  {
    DupIns->PORT_MAP = loc_dupvbmap( DupIns->PORT_MAP, BeMap );
  }

  for ( BeMap  = BeIns->GEN_MAP;
        BeMap != (vbmap_list *)0;
        BeMap  = BeMap->NEXT )
  {
    DupIns->GEN_MAP = loc_dupvbmap( DupIns->GEN_MAP, BeMap );
  }

  return( DupIns );
}

/*------------------------------------------------------------\
|                                                             |
|                          Dup Vbh Aux                        |
|                                                             |
\------------------------------------------------------------*/

static vbaux_list *loc_dupvbaux( DupAux, BeAux )

  vbaux_list *DupAux;
  vbaux_list *BeAux;
{
  vbtyp_list *DupType;

  DupType = vbh_getvbtyp( LocalDupFigure, BeAux->TYPE->NAME );

  DupAux = vbh_addvbaux( DupAux, dupvexexpr( BeAux->TARGET ), dupvexexpr( BeAux->VEX ),
              BeAux->KIND, DupType, BeAux->LINE );

  return( DupAux );
}

/*------------------------------------------------------------\
|                                                             |
|                          Dup Vbh Arg                        |
|                                                             |
\------------------------------------------------------------*/

static vbarg_list *loc_dupvbarg( DupArg, BeArg  )

  vbarg_list  *DupArg;
  vbarg_list  *BeArg;
{
  vbtyp_list *DupType;

  DupType = vbh_getvbtyp( LocalDupFigure, BeArg->TYPE->NAME );
  DupArg  = vbh_addvbarg( DupArg, dupvexexpr( BeArg->TARGET ),
                          BeArg->DIR, BeArg->CLASS, DupType, BeArg->LINE );
  return( DupArg );
}

/*------------------------------------------------------------\
|                                                             |
|                          Dup Vbh Fun                        |
|                                                             |
\------------------------------------------------------------*/

static vbfun_list *loc_dupvbfun( DupFun, BeFun )

  vbfun_list *DupFun;
  vbfun_list *BeFun;
{
  vbarg_list *BeArg;

  DupFun = vbh_addvbfun( DupFun, BeFun->NAME, 
               (vbarg_list *)0, (vbarg_list *)0, (ptype_list *)0, BeFun->LINE );

  DupFun->TYPE = BeFun->TYPE;

  for ( BeArg  = BeFun->ARGUMENT;
        BeArg != (vbarg_list *)0;
        BeArg  = BeArg->NEXT )
  {
    DupFun->ARGUMENT = loc_dupvbarg( DupFun->ARGUMENT, BeArg );
  }

  DupFun->ARGUMENT = (vbarg_list *)reverse( (chain_list *)DupFun->ARGUMENT );

  if ( BeFun->RETURN != (vbarg_list *)0 )
  {
    DupFun->RETURN = loc_dupvbarg( (vbarg_list *)0, BeFun->RETURN );
  }

  DupFun->VARIABLE    = loc_dupvbinst( BeFun->VARIABLE );
  DupFun->INSTRUCTION = loc_dupvbinst( BeFun->INSTRUCTION );

  return( DupFun );
}

/*------------------------------------------------------------\
|                                                             |
|                          Dup Vbh Pcs                        |
|                                                             |
\------------------------------------------------------------*/

static vbpcs_list *loc_dupvbpcs( DupPcs, BePcs )

  vbpcs_list *DupPcs;
  vbpcs_list *BePcs;
{
  chain_list *ScanSens;
  chain_list *DupSens;

  DupPcs = vbh_addvbpcs( DupPcs, BePcs->LABEL, (chain_list *)0, (ptype_list *)0, BePcs->LINE );

  DupSens = (chain_list *)0;

  for ( ScanSens  = BePcs->SENSITIVITY;
        ScanSens != (chain_list *)0;
        ScanSens  = ScanSens->NEXT )
  {
    DupSens = addchain( DupSens, ScanSens->DATA );
  }

  DupPcs->SENSITIVITY = DupSens;
  DupPcs->TYPE        = BePcs->TYPE;
  DupPcs->VARIABLE    = loc_dupvbinst( BePcs->VARIABLE );
  DupPcs->INSTRUCTION = loc_dupvbinst( BePcs->INSTRUCTION );

  return( DupPcs );
}

/*------------------------------------------------------------\
|                                                             |
|                          Dup Vbh Gnr                        |
|                                                             |
\------------------------------------------------------------*/

static vbgnr_list *loc_dupvbgnr( DupGnr, BeGnr )

  vbgnr_list *DupGnr;
  vbgnr_list *BeGnr;
{
  vbins_list *BeIns;
  vbpcs_list *BePcs;

  DupGnr = vbh_addvbgnr( DupGnr, BeGnr->LABEL, BeGnr->LINE );

  DupGnr->TYPE         = BeGnr->TYPE;
  DupGnr->FOR_UP       = BeGnr->FOR_UP;
  DupGnr->IF_COND      = dupvexexpr( BeGnr->IF_COND      );
  DupGnr->FOR_VARIABLE = dupvexexpr( BeGnr->FOR_VARIABLE );
  DupGnr->FOR_LEFT     = dupvexexpr( BeGnr->FOR_LEFT     );
  DupGnr->FOR_RIGHT    = dupvexexpr( BeGnr->FOR_RIGHT    );

  for ( BeIns  = BeGnr->BEINS;
        BeIns != (vbins_list *)0;
        BeIns  = BeIns->NEXT )
  {
    DupGnr->BEINS = loc_dupvbins( DupGnr->BEINS, BeIns );
  }

  for ( BePcs  = BeGnr->BEPCS;
        BePcs != (vbpcs_list *)0;
        BePcs  = BePcs->NEXT )
  {
    DupGnr->BEPCS = loc_dupvbpcs( DupGnr->BEPCS, BePcs );
  }

  for ( BeGnr  = BeGnr->BEGNR;
        BeGnr != (vbgnr_list *)0;
        BeGnr  = BeGnr->NEXT )
  {
    DupGnr->BEGNR = loc_dupvbgnr( DupGnr->BEGNR, BeGnr );
  }

  return( DupGnr );
}

/*------------------------------------------------------------\
|                                                             |
|                        vbh_dupvbfig                         |
|                                                             |
\------------------------------------------------------------*/

void vbh_dupvbfig( DupFigure, Figure )

  vbfig_list *DupFigure;
  vbfig_list *Figure;
{
  vbgnr_list *BeGnr;
  vbgen_list *BeGen;
  vbins_list *BeIns;
  vbmod_list *BeMod;
  vbmod_list *DupMod;
  vbaux_list *BeAux;
  vbaux_list *DupAux;
  vbatr_list *BeAtr;
  vbatr_list *DupAtr;
  vbcst_list *BeCst;
  vbcst_list *DupCst;
  vbpor_list *BePor;
  vbpcs_list *BePcs;
  vbfun_list *BeFun;
  vbtyp_list *DupType;
  chain_list *PackList;
  authelem   *Element;

  LocalDupFigure = DupFigure;

  if ( LocalHashTable == (authtable *)0 )
  {
    LocalHashTable = createauthtable( 100 );
  }

  for ( PackList  = Figure->PACK_LIST;
        PackList != (chain_list *)0;
        PackList  = PackList->NEXT )
  {
    DupFigure->PACK_LIST = addchain( DupFigure->PACK_LIST, PackList->DATA );
  }

  loc_dupvbtyp( Figure, DupFigure );

  for ( BePor  = Figure->BEPOR;
        BePor != (vbpor_list *)0;
        BePor  = BePor->NEXT )
  {
    DupFigure->BEPOR = loc_dupvbpor( DupFigure->BEPOR, BePor );
  }

  for ( BeGen  = Figure->BEGEN;
        BeGen != (vbgen_list *)0;
        BeGen  = BeGen->NEXT )
  {
    DupFigure->BEGEN = loc_dupvbgen( DupFigure->BEGEN, BeGen );
  }

  for ( DupMod  = DupFigure->BEMOD;
        DupMod != (vbmod_list *)0;
        DupMod  = DupMod->NEXT )
  {
    addauthelem( LocalHashTable, DupMod->NAME, 0 );
  }

  for ( BeMod  = Figure->BEMOD;
        BeMod != (vbmod_list *)0;
        BeMod  = BeMod->NEXT )
  {
    Element = searchauthelem( LocalHashTable, BeMod->NAME );

    if ( Element != (authelem *)0 ) continue;

    DupFigure->BEMOD = vbh_addvbmod( DupFigure->BEMOD, BeMod->NAME, BeMod->LINE );
    DupMod           = DupFigure->BEMOD;

    for ( BePor  = BeMod->BEPOR;
          BePor != (vbpor_list *)0;
          BePor  = BePor->NEXT )
    {
      DupMod->BEPOR = loc_dupvbpor( DupMod->BEPOR, BePor );
    }

    for ( BeGen  = BeMod->BEGEN;
          BeGen != (vbgen_list *)0;
          BeGen  = BeGen->NEXT )
    {
      DupMod->BEGEN = loc_dupvbgen( DupMod->BEGEN, BeGen );
    }
  }

  for ( DupAtr  = DupFigure->BEATR;
        DupAtr != (vbatr_list *)0;
        DupAtr  = DupAtr->NEXT )
  {
    addauthelem( LocalHashTable, DupAtr->NAME, 0 );
  }

  for ( BeAtr  = Figure->BEATR;
        BeAtr != (vbatr_list *)0;
        BeAtr  = BeAtr->NEXT )
  {
    Element = searchauthelem( LocalHashTable, BeAtr->NAME );

    if ( Element != (authelem *)0 ) continue;

    DupFigure->BEATR = vbh_addvbatr( DupFigure->BEATR, BeAtr->NAME, BeAtr->ENTITY, 
                                     BeAtr->TYPE, BeAtr->VALUE, BeAtr->LINE );
  }

  for ( DupCst  = DupFigure->BECST;
        DupCst != (vbcst_list *)0;
        DupCst  = DupCst->NEXT )
  {
    addauthelem( LocalHashTable, getvexatomname( DupCst->TARGET ), 0 );
  }

  for ( BeCst  = Figure->BECST;
        BeCst != (vbcst_list *)0;
        BeCst  = BeCst->NEXT )
  {
    Element = searchauthelem( LocalHashTable, getvexatomname( BeCst->TARGET ) );

    if ( Element != (authelem *)0 ) continue;

    DupType = vbh_getvbtyp( DupFigure, BeCst->TYPE->NAME );

    DupFigure->BECST = vbh_addvbcst( DupFigure->BECST, dupvexexpr( BeCst->TARGET ),
                 dupvexexpr( BeCst->VEX ), DupType, BeCst->LINE );
  }

  for ( DupAux  = DupFigure->BEAUX;
        DupAux != (vbaux_list *)0;
        DupAux  = DupAux->NEXT )
  {
    addauthelem( LocalHashTable, getvexatomname( DupAux->TARGET ), 0 );
  }

  for ( BeAux  = Figure->BEAUX;
        BeAux != (vbaux_list *)0;
        BeAux  = BeAux->NEXT )
  {
    Element = searchauthelem( LocalHashTable, getvexatomname( BeAux->TARGET ) );

    if ( Element != (authelem *)0 ) continue;

    DupFigure->BEAUX = loc_dupvbaux( DupFigure->BEAUX, BeAux );
  }

  for ( BeFun  = Figure->BEFUN;
        BeFun != (vbfun_list *)0;
        BeFun  = BeFun->NEXT )
  {
    DupFigure->BEFUN = loc_dupvbfun( DupFigure->BEFUN, BeFun );
  }

  for ( BeIns  = Figure->BEINS;
        BeIns != (vbins_list *)0;
        BeIns  = BeIns->NEXT )
  {
    DupFigure->BEINS = loc_dupvbins( DupFigure->BEINS, BeIns );
  }

  for ( BePcs  = Figure->BEPCS;
        BePcs != (vbpcs_list *)0;
        BePcs  = BePcs->NEXT )
  {
    DupFigure->BEPCS = loc_dupvbpcs( DupFigure->BEPCS, BePcs );
  }

  for ( BeGnr  = Figure->BEGNR;
        BeGnr != (vbgnr_list *)0;
        BeGnr  = BeGnr->NEXT )
  {
    DupFigure->BEGNR = loc_dupvbgnr( DupFigure->BEGNR, BeGnr );
  }

  resetauthtable( LocalHashTable );
}

/*------------------------------------------------------------\
|                                                             |
|                        vbh_dupvbinst                        |
|                                                             |
\------------------------------------------------------------*/

ptype_list *vbh_dupvbinst( Figure, Instruction )

  vbfig_list *Figure;
  ptype_list *Instruction;
{
  LocalDupFigure = Figure;

  return( loc_dupvbinst( Instruction ) );
}
