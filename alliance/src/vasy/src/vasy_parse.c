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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                     VASY                          |
|                                                             |
| File    :                  vasy_parse.c                     |
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
# include "abl.h"
# include "bdd.h"
# include "vex.h"
# include "vbh.h"
# include "vbl.h"
# include "vpn.h"
# include "vtl.h"

# include "vasy_shared.h"
# include "vasy_parse.h"
# include "vasy_debug.h"
# include "vasy_generic.h"
# include "vasy_generate.h"
# include "vasy_loop.h"
# include "vasy_array.h"
# include "vasy_dynfunc.h"
# include "vasy_error.h"
# include "vasy_main.h"

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
|                      Private variables                      |
|                                                             |
\------------------------------------------------------------*/

  static char         VasyBuffer[ 512 ];
  static authtable   *VasyHashTreatedModel = (authtable *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    VasyParseVerifyVexWidth                  |
|                                                             |
\------------------------------------------------------------*/

static void VasyParseVerifyVexWidth( VexExpr )

  vexexpr *VexExpr;
{
  chain_list *ScanOper;
  char       *FuncName;
  vexexpr    *VexOperand;
  int         FuncId;
  short       Width;
  long        Oper;

  if ( IsVexNodeAtom( VexExpr ) )
  {
    if ( IsVexNodeVarWidth( VexExpr ) )
    {
      VasyPrintf( stdout, "ERROR size of atom %s should be static\n", 
               GetVexAtomValue( VexExpr ) );
      autexit( 1 );
    }
  }
  else
  if ( IsVexNodeFunc( VexExpr ) )
  {
    for ( ScanOper  = VexExpr->OPERAND;
          ScanOper != (chain_list *)0;
          ScanOper  = ScanOper->NEXT )
    {
      VasyParseVerifyVexWidth( GetVexOperand( ScanOper ) );
    }

    FuncName = GetVexFuncValue( VexExpr );
    FuncId   = getvexstdfuncid( FuncName );

    if ( FuncId != -1 )
    {
      switch ( FuncId )
      {
        case VEX_STD_TO_BIT                :
        case VEX_STD_TO_BITVECTOR          :
        case VEX_STD_TO_STDULOGIC          :
        case VEX_STD_TO_STDLOGICVECTOR     :
        case VEX_STD_TO_STDULOGICVECTOR    :
        case VEX_STD_TO_X01                :
        case VEX_STD_TO_X01Z               :
        case VEX_STD_TO_UX01               :
        case VEX_STD_CONV_INTEGER          :
        case VEX_STD_RISING_EDGE           :
        case VEX_STD_FALLING_EDGE          :
        case VEX_STD_STD_LOGIC_VECTOR      :
        case VEX_STD_STD_ULOGIC_VECTOR     :
        case VEX_STD_SIGNED                :
        case VEX_STD_UNSIGNED              :
        case VEX_STD_TO_INTEGER            :

            VexOperand = GetVexOperand( VexExpr->OPERAND );
            VexExpr->WIDTH = VexOperand->WIDTH;

        break;
      }
    }
  }
  else
  {
    Oper       = GetVexOperValue( VexExpr );
    VexOperand = GetVexOperand( VexExpr->OPERAND );
    VasyParseVerifyVexWidth( VexOperand );

    if ( ( Oper == VEX_EVENT ) ||
         ( Oper == VEX_NOT   ) ||
         ( Oper == VEX_NEG   ) ||
         ( Oper == VEX_ABS   ) )
    {
      VexExpr->WIDTH = VexOperand->WIDTH;
      ClearVexNodeVarWidth( VexExpr );
    }
    else
    if ( ( Oper == VEX_OR   ) ||
         ( Oper == VEX_AND  ) ||
         ( Oper == VEX_XOR  ) ||
         ( Oper == VEX_NOR  ) ||
         ( Oper == VEX_NAND ) ||
         ( Oper == VEX_NXOR ) )
    {
      Width = VexOperand->WIDTH;

      VexExpr->WIDTH = VexOperand->WIDTH;
      ClearVexNodeVarWidth( VexExpr );

      for ( ScanOper  = VexExpr->OPERAND->NEXT;
            ScanOper != (chain_list *)0;
            ScanOper  = ScanOper->NEXT )
      {
        VexOperand = GetVexOperand( ScanOper );
        VasyParseVerifyVexWidth( VexOperand );

        if ( VexOperand->WIDTH != Width )
        {
          viewvexexprln( VexExpr );
          VasyPrintf( stdout, "ERROR illegal use of %s, different size width operands\n", 
                   getvexopername( Oper ) );

        }
      }
    }
    else
    if ( ( Oper == VEX_CONCAT ) ||
         ( Oper == VEX_MUL    ) )
    {
      Width = VexOperand->WIDTH;

      for ( ScanOper  = VexExpr->OPERAND->NEXT;
            ScanOper != (chain_list *)0;
            ScanOper  = ScanOper->NEXT )
      {
        VexOperand = GetVexOperand( ScanOper );
        VasyParseVerifyVexWidth( VexOperand );

        Width += VexOperand->WIDTH;
      }

      ClearVexNodeVarWidth( VexExpr );
      VexExpr->WIDTH = Width;
    }
    else
    if ( ( Oper == VEX_EQ ) ||
         ( Oper == VEX_NE ) ||
         ( Oper == VEX_LT ) ||
         ( Oper == VEX_LE ) ||
         ( Oper == VEX_GT ) ||
         ( Oper == VEX_GE ) )
    {
      for ( ScanOper  = VexExpr->OPERAND->NEXT;
            ScanOper != (chain_list *)0;
            ScanOper  = ScanOper->NEXT )
      {
        VexOperand = GetVexOperand( ScanOper );
        VasyParseVerifyVexWidth( VexOperand );
      }

      ClearVexNodeVarWidth( VexExpr );
      VexExpr->WIDTH = 1;
    }
    else
    if ( ( Oper == VEX_ADD ) ||
         ( Oper == VEX_SUB ) )
    {
      Width = VexOperand->WIDTH;

      for ( ScanOper  = VexExpr->OPERAND->NEXT;
            ScanOper != (chain_list *)0;
            ScanOper  = ScanOper->NEXT )
      {
        VexOperand = GetVexOperand( ScanOper );
        VasyParseVerifyVexWidth( VexOperand );

        if ( VexOperand->WIDTH > Width ) Width = VexOperand->WIDTH;
      }

      ClearVexNodeVarWidth( VexExpr );
      VexExpr->WIDTH = Width;
    }
    else
    if ( Oper == VEX_DIV )
    {
      Width = VexOperand->WIDTH;

      for ( ScanOper  = VexExpr->OPERAND->NEXT;
            ScanOper != (chain_list *)0;
            ScanOper  = ScanOper->NEXT )
      {
        VexOperand = GetVexOperand( ScanOper );
        VasyParseVerifyVexWidth( VexOperand );
      }

      ClearVexNodeVarWidth( VexExpr );
      VexExpr->WIDTH = Width;
    }
    else
    if ( Oper != VEX_MOD )
    {
      viewvexexprln( VexExpr );
      VasyPrintf( stdout, "ERROR operator %s not supported !\n", 
                   getvexopername( Oper ) );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyParseVerifyVex                        |
|                                                             |
\------------------------------------------------------------*/

vexexpr *VasyParseVerifyVex( VexExpr )

  vexexpr *VexExpr;
{
  VasyParseVerifyVexWidth( VexExpr );

  return( VexExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyParseVerifyVbhFig                     |
|                                                             |
\------------------------------------------------------------*/

static void VasyParseVerifyVbhFig( VbhFigure )

  vbfig_list *VbhFigure;
{
  vbpor_list *VbhPort;
  vbaux_list *VbhAux;
  vbcst_list *VbhCst;
  vbmod_list *VbhMod;
  vbins_list *VbhIns;
  vbmap_list *VbhMap;
  vexexpr    *VexTarget;
  vexexpr    *VexExpr;
  int         Error;

  Error = 0;

  for ( VbhPort  = VbhFigure->BEPOR;
        VbhPort != (vbpor_list *)0;
        VbhPort  = VbhPort->NEXT )
  {
    VexTarget = VbhPort->TARGET;
    VexExpr   = VbhPort->VEX;

    if ( ( ! IsVexNodeAtom( VexTarget          ) ) ||
         ( ( VexExpr != (vexexpr *)0         ) &&
           ( ( ! IsVexNodeAtom( VexExpr ) ) ||
             ( ! IsVexAtomLiteral( VexExpr ) ) ) ) )
    {
      fprintf( stdout, "ERROR size of port %s should be static\n", 
                getvexatomname( VexTarget ) );
      Error = 1;
    }
  }

  for ( VbhCst  = VbhFigure->BECST;
        VbhCst != (vbcst_list *)0;
        VbhCst  = VbhCst->NEXT )
  {
    VexTarget = VbhCst->TARGET;
    VexExpr   = VbhCst->VEX;

    if ( ( ! IsVexNodeAtom( VexTarget          ) ) ||
         ( ( VexExpr != (vexexpr *)0         ) &&
           ( ( ! IsVexNodeAtom( VexExpr ) ) ||
             ( ! IsVexAtomLiteral( VexExpr ) ) ) ) )
    {
      fprintf( stdout, "ERROR size of constant %s should be static\n", 
                getvexatomname( VexTarget ) );
      Error = 1;
    }
  }

  for ( VbhAux  = VbhFigure->BEAUX;
        VbhAux != (vbaux_list *)0;
        VbhAux  = VbhAux->NEXT )
  {
    VexTarget = VbhAux->TARGET;
    VexExpr   = VbhAux->VEX;

    if ( ( ! IsVexNodeAtom( VexTarget          ) ) ||
         ( ( VexExpr != (vexexpr *)0         ) &&
           ( ( ! IsVexNodeAtom( VexExpr ) ) ||
             ( ! IsVexAtomLiteral( VexExpr ) ) ) ) )
    {
      fprintf( stdout, "ERROR size of signal %s should be static\n", 
                getvexatomname( VexTarget ) );
      Error = 1;
    }
  }

  for ( VbhMod  = VbhFigure->BEMOD;
        VbhMod != (vbmod_list *)0;
        VbhMod  = VbhMod->NEXT )
  {
    for ( VbhPort  = VbhMod->BEPOR;
          VbhPort != (vbpor_list *)0;
          VbhPort  = VbhPort->NEXT )
    {
      VexTarget = VbhPort->TARGET;
      VexExpr   = VbhPort->VEX;

      if ( ( ! IsVexNodeAtom( VexTarget   ) ) ||
           ( ( VexExpr != (vexexpr *)0    ) &&
             ( ! IsVexNodeAtom( VexExpr ) ) ) )
      {
        fprintf( stdout, "ERROR size of port %s in model %s should be static\n", 
                  getvexatomname( VexTarget ), VbhMod->NAME );
        Error = 1;
      }
    }
  }

  for ( VbhIns  = VbhFigure->BEINS;
        VbhIns != (vbins_list *)0;
        VbhIns  = VbhIns->NEXT )
  {
    for ( VbhMap   = VbhIns->PORT_MAP;
          VbhMap != (vbmap_list *)0;
          VbhMap  = VbhMap->NEXT )
    {
      VexTarget = VbhMap->FORMAL;
      VexExpr   = VbhMap->ACTUAL;

      if ( ! IsVexNodeAtom( VexTarget ) )
      {
        fprintf( stdout, "ERROR size of port map %s in instance %s should be static\n", 
                  getvexatomname( VexTarget ), VbhIns->NAME );
        Error = 1;
      }
    }
  }

  if ( Error ) autexit( 1 );

  vbh_forallvexvbfig( VbhFigure, simpvexexpr, VasyParseVerifyVex );

  if ( IsVasyDebugLevel1() )
  {
    vbh_viewvbfig( VbhFigure );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyParseVbhInstanceAddPower              |
|                                                             |
\------------------------------------------------------------*/

static void VasyParseVbhInstanceAddPower( VbhFigure )

  vbfig_list *VbhFigure;
{
  vbins_list *VbhInstance;
  vbmap_list *VbhMap;
//vbtyp_list *VbhType;
  vexexpr    *VexFormal;
  vexexpr    *VexActual;
  char       *Name;
  int         AddVdd;
  int         AddVss;

//VbhType = vbh_getvbtyp( VbhFigure, "bit" );

  for ( VbhInstance  = VbhFigure->BEINS;
        VbhInstance != (vbins_list *)0;
        VbhInstance  = VbhInstance->NEXT )
  {
    AddVdd = 1;
    AddVss = 1;

    for ( VbhMap   = VbhInstance->PORT_MAP;
          VbhMap != (vbmap_list *)0;
          VbhMap  = VbhMap->NEXT )
    {
      Name = getvexarrayname( VbhMap->FORMAL );

      if ( Name == VasyPowerVdd ) AddVdd = 0;
      else
      if ( Name == VasyPowerVss ) AddVss = 0;
    }

    if ( AddVdd )
    {
      VexFormal = createvexatombit( VasyPowerVdd );
      VexActual = dupvexexpr( VexFormal );

      VbhInstance->PORT_MAP = vbh_addvbmap( VbhInstance->PORT_MAP, VexFormal, VexActual, 0 );
    }

    if ( AddVss )
    {
      VexFormal = createvexatombit( VasyPowerVss );
      VexActual = dupvexexpr( VexFormal );

      VbhInstance->PORT_MAP = vbh_addvbmap( VbhInstance->PORT_MAP, VexFormal, VexActual, 0 );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyParseVbhPortAddPower                  |
|                                                             |
\------------------------------------------------------------*/

static vbpor_list *VasyParseVbhPortAddPower( VbhFigure, HeadPort, ModelName )

  vbfig_list *VbhFigure;
  vbpor_list *HeadPort;
  char       *ModelName;
{
  vbpor_list *VbhPort;
  vbtyp_list *VbhType;
  vexexpr    *VexAtom;
  char       *Name;
  int         CreateVdd;
  int         CreateVss;

  CreateVdd = 1;
  CreateVss = 1;

  VbhType = vbh_getvbtyp( VbhFigure, "bit" );

  for ( VbhPort  = HeadPort;
        VbhPort != (vbpor_list *)0;
        VbhPort  = VbhPort->NEXT )
  {
    Name = getvexarrayname( VbhPort->TARGET );

    if ( Name == VasyPowerVdd )
    {
      CreateVdd = 0;
      VasyPrintf( stdout, "WARNING connector vdd already exists in model %s\n", ModelName );
    }
    else
    if ( Name == VasyPowerVss )
    {
      CreateVss = 0;

      VasyPrintf( stdout, "WARNING connector vss already exists in model %s\n", ModelName );
    }
  }

  if ( CreateVdd )
  {
    VexAtom  = createvexatombit( VasyPowerVdd );
    HeadPort = vbh_addvbpor( HeadPort, VexAtom, (vexexpr *)0, 'I', 0, VbhType, 0 );
  }

  if ( CreateVss )
  {
    VexAtom  = createvexatombit( VasyPowerVss );
    HeadPort = vbh_addvbpor( HeadPort, VexAtom, (vexexpr *)0, 'I', 0, VbhType, 0 );
  }

  return( HeadPort );
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyParseAddPower                     |
|                                                             |
\------------------------------------------------------------*/

static void VasyParseAddPower( VbhFigure )

  vbfig_list *VbhFigure;
{
  vbmod_list *VbhModel; 

  if ( VasyPowerVdd == (char *)0 )
  {
    VasyPowerVdd = namealloc( "vdd" );
    VasyPowerVss = namealloc( "vss" );
  }

  VbhFigure->BEPOR = VasyParseVbhPortAddPower( VbhFigure, VbhFigure->BEPOR, VbhFigure->NAME );

  for ( VbhModel  = VbhFigure->BEMOD;
        VbhModel != (vbmod_list *)0;
        VbhModel  = VbhModel->NEXT )
  {
    VbhModel->BEPOR = VasyParseVbhPortAddPower( VbhFigure, VbhModel->BEPOR, VbhModel->NAME );
  }

  VasyParseVbhInstanceAddPower( VbhFigure );
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyParseVbhFig                       |
|                                                             |
\------------------------------------------------------------*/

vbfig_list *VasyParseVbhFig( FileName, GenMap, Deep )

  char       *FileName;
  vbmap_list *GenMap;
  int         Deep;
{
  authtable   *HashModel;
  authelem    *Element;
  vbfig_list  *VbhFigure;
  vbfig_list  *VbhNewFigure;
  vbmod_list **PrevModel;
  vbmod_list  *VbhModel;
  vbgen_list  *VbhGen;
  vbmod_list  *DelModel;
  vbmod_list  *VbhNewModel;
  vbins_list  *VbhInstance;
  vbmap_list  *VbhGenMap;
  vbtyp_list  *VbhType;
  vbpor_list  *VbhPort;
  char        *InstanceName;
  char        *ModelName;
  char        *NewModelName;
  char        *Name;
  char        *GenName;
  int          Generic;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "--> VasyParseVbhFig %s\n", FileName );
  }

  if ( IsVasyDebugStatistics() )
  {
    VasyDebugStartChrono(1);
  }

  VbhFigure = getvbfiggenmap( FileName, VPN_IN, GenMap );

  if ( VasyHashTreatedModel == (authtable *)0 )
  {
    VasyHashTreatedModel = createauthtable( 100 );
  }

  HashModel = createauthtable( 100 );

  for ( VbhModel  = VbhFigure->BEMOD;
        VbhModel != (vbmod_list *)0;
        VbhModel  = VbhModel->NEXT )
  {
    addauthelem( HashModel, VbhModel->NAME, (long)VbhModel );
  }

  for ( VbhInstance  = VbhFigure->BEINS;
        VbhInstance != (vbins_list *)0;
        VbhInstance  = VbhInstance->NEXT )
  {
    ModelName = VbhInstance->MODEL;
    Element   = searchauthelem( HashModel, ModelName );

    if ( Element == (authelem *)0 )
    {
      VasyPrintf( stdout, "ERROR unknown model %s for instance %s\n",
                  ModelName, VbhInstance->NAME );
      autexit( 1 );
    }
  }

  for ( VbhInstance  = VbhFigure->BEINS;
        VbhInstance != (vbins_list *)0;
        VbhInstance  = VbhInstance->NEXT )
  {
    Element  = searchauthelem( HashModel, VbhInstance->MODEL );
    VbhModel = (vbmod_list *)Element->VALUE;

    if ( ( VbhInstance->GEN_MAP != (vbmap_list *)0 ) ||
         ( VbhModel->BEGEN      != (vbgen_list *)0 ) ) Generic = 1;
    else                                               Generic = 0;

    if ( (   Generic                           ) ||
         ( ( VasyFlagHier                        ) &&
           ( ! incatalog( VbhInstance->MODEL ) ) ) )
    {
      InstanceName = VbhInstance->NAME;
      ModelName    = VbhInstance->MODEL;

      if ( Generic )
      {
        sprintf( VasyBuffer, "%s_%s", ModelName, InstanceName );
        NewModelName = namealloc( VasyBuffer );

        VbhInstance->MODEL = NewModelName;
      }
      else
      {
        NewModelName = ModelName;
      }

      Element = searchauthelem( VasyHashTreatedModel, NewModelName );

      if ( ( Generic                  ) ||
           ( Element == (authelem *)0 ) )
      {
        for ( VbhGen  = VbhModel->BEGEN;
              VbhGen != (vbgen_list *)0;
              VbhGen  = VbhGen->NEXT )
        {
          if ( ( IsVexNodeAtom( VbhGen->TARGET ) ) &&
               ( VbhGen->VEX != (vexexpr *)0     ) )
          {
            GenName = GetVexAtomValue( VbhGen->TARGET );

            for ( VbhGenMap  = VbhInstance->GEN_MAP;
                  VbhGenMap != (vbmap_list *)0;
                  VbhGenMap  = VbhGenMap->NEXT )
            {
              if ( IsVexNodeAtom( VbhGenMap->FORMAL ) )
              {
                Name = GetVexAtomValue( VbhGenMap->FORMAL );

                if ( Name == GenName ) break;
              }
            }

            if ( VbhGenMap == (vbmap_list *)0 )
            {
              VbhInstance->GEN_MAP = vbh_addvbmap( VbhInstance->GEN_MAP,
                  dupvexexpr( VbhGen->TARGET ), dupvexexpr( VbhGen->VEX ), VbhGen->LINE );
            }
          }
        }

        VbhNewFigure = VasyMainTreatModel( ModelName, NewModelName, 
                 VbhInstance->GEN_MAP, Deep + 1 );

        addauthelem( VasyHashTreatedModel, NewModelName, (long)VbhNewFigure );

        if ( Generic )
        {
          VbhFigure->BEMOD = vbh_addvbmod( VbhFigure->BEMOD, NewModelName, 0 );
          VbhNewModel      = VbhFigure->BEMOD;
  
          for ( VbhPort  = VbhNewFigure->BEPOR;
                VbhPort != (vbpor_list *)0;
                VbhPort  = VbhPort->NEXT )
          {
            VbhType = vbh_getvbtyp( VbhFigure, VbhPort->TYPE->NAME );
  
            VbhNewModel->BEPOR = vbh_addvbpor( VbhNewModel->BEPOR,
                 dupvexexpr( VbhPort->TARGET ), 
                 dupvexexpr( VbhPort->VEX    ),
                 VbhPort->DIR, VbhPort->KIND, VbhType, 0 );
          }
        }
      }
    }
  }

  resetauthtable( HashModel );

  VasyGenerateVbhFig( VbhFigure );
  VasyGenericVbhFig( VbhFigure );
/*
** Hash used model !
*/
  for ( VbhInstance  = VbhFigure->BEINS;
        VbhInstance != (vbins_list *)0;
        VbhInstance  = VbhInstance->NEXT )
  {
    ModelName = VbhInstance->MODEL;
    addauthelem( HashModel, ModelName, 0 );
  }

  VbhModel  = VbhFigure->BEMOD;
  PrevModel = &VbhFigure->BEMOD;
/*
** Delete all unused model !
*/
  while ( VbhModel != (vbmod_list *)0 )
  {
    Element = searchauthelem( HashModel, VbhModel->NAME );

    if ( Element == (authelem *)0 ) 
    {
      *PrevModel = VbhModel->NEXT;
      DelModel   = VbhModel;
      VbhModel   = VbhModel->NEXT;

      if ( IsVasyDebugLevel0() )
      {
        VasyPrintf( stdout, "Delete model %s\n", DelModel->NAME );
      }

      DelModel->NEXT = (vbmod_list *)0;

      vbh_frevbmod( DelModel );
    }
    else
    {
      PrevModel = &VbhModel->NEXT;
      VbhModel  =  VbhModel->NEXT;
    }
  }

  destroyauthtable( HashModel );

  if ( VasyFlagPower )
  {
    VasyParseAddPower( VbhFigure );
  }

  VasyUnrollLoopVbhFig( VbhFigure );
  VasyExpandArrayVbhFig( VbhFigure );
  VasyExpandDynFuncVbhFig( VbhFigure );

  VasyParseVerifyVbhFig( VbhFigure );

  if ( IsVasyDebugStatistics() )
  {
    VasyPrintf( stdout, "--> Compilation : %ld sec\n", VasyDebugReadChrono(1) );
    VasyDebugStartChrono(1);
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "<-- VasyParseVbhFig %s\n\n", FileName );
  }

  return( VbhFigure );
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyParseVbh2VpnFig                   |
|                                                             |
\------------------------------------------------------------*/

vpnfig_list *VasyParseVbh2VpnFig( VbhFigure )

  vbfig_list *VbhFigure;
{
  vpnfig_list   *VpnFigure;
  vpnproc_list  *VpnProc;
  vpntrans_list *VpnTrans;
  vpntrans_list *AsgTrans;
  vpnplace_list *VpnPlace;
  vpnplace_list *AsgPlace;
  vpnarc        *VpnArc;
  vpnact_list   *VpnAction = NULL;
  vpndecl_list  *VpnDeclar;
  vexexpr       *VexAtom = NULL;
  vexexpr       *VexExpr;
  char          *Value;
  long           NumberBus;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "--> VasyParseVbh2VpnFig %s\n", VbhFigure->NAME );
  }

  if ( IsVasyDebugStatistics() )
  {
    VasyDebugStartChrono(1);
  }

  VpnFigure = addvpnfig( VbhFigure->NAME );

  VvhVbh2Vpn( VbhFigure, VpnFigure, 1 );

  if ( IsVasyDebugStatistics() )
  {
    VasyPrintf( stdout, "--> Translation : %ld sec\n", VasyDebugReadChrono(1) );
  }

  if ( VpnFigure->FUNCTION != (vpnfunc_list *)0 )
  {
    VasyErrorLine( VASY_NOT_YET_IMPLEMENTED_ERROR, VpnFigure->FUNCTION->LINE, "Function" );
  }

  NumberBus = 0;
/*
** Modify all guarded bus process
*/
  for ( VpnProc  = VpnFigure->PROCESS;
        VpnProc != (vpnproc_list *)0;
        VpnProc  = VpnProc->NEXT )
  {
    if ( IsVpnProcGuarded( VpnProc ) )
    {
/*
** Check if it's guarded bus assignment
*/
      for ( VpnTrans  = VpnProc->TRANS;
            VpnTrans != (vpntrans_list *)0;
            VpnTrans  = VpnTrans->NEXT )
      {
        for ( VpnAction  = VpnTrans->ACT;
              VpnAction != (vpnact_list *)0;
              VpnAction  = VpnAction->NEXT )
        {
          VexAtom = VpnAction->VEX_ATOM;

          if ( VexAtom != (vexexpr *)0 )
          {
            Value     = getvexatomname( VexAtom );
            VpnDeclar = searchvpndeclall( VpnFigure, Value );

            if ( VpnDeclar->KIND == VPN_KIND_BUS ) break;
          }
        }

        if ( VpnAction != (vpnact_list *)0 ) break;
      }

      if ( VpnAction == (vpnact_list *)0 ) continue;
/*
** Add a new assignment : VexAtom <= (others => 'Z');
*/
      VpnTrans = VpnProc->FIRST;
      VpnArc   = GetVpnArc( VpnTrans->PLACE_OUT );
      VpnPlace = GetVpnArcTargetPlace( VpnArc );

      delvpnarc( VpnFigure, VpnArc );

      sprintf( VasyBuffer, "asg_bus_%ld", NumberBus++ );
      AsgPlace = addvpnprocplace( VpnFigure, VpnProc, VasyBuffer );
      AsgPlace->TYPE = VPN_PLACE_ASSIGN;
      AsgTrans = addvpnproctrans( VpnFigure, VpnProc, VasyBuffer );
      AsgTrans->TYPE = VPN_TRANS_ACT_EXEC;

      VexAtom   = dupvexexpr( VexAtom );
      VexExpr   = createvexatomveclit( VEX_TRISTATE, VexAtom->WIDTH );
      VpnAction = addvpnactasg( VpnFigure, AsgTrans, VexAtom, VexExpr );

      addvpnarctrans( VpnFigure, VpnTrans, AsgPlace );
      addvpnarcplace( VpnFigure, AsgPlace, AsgTrans );
      addvpnarctrans( VpnFigure, AsgTrans, VpnPlace );
    }
  }

  for ( VpnProc  = VpnFigure->PROCESS;
        VpnProc != (vpnproc_list *)0;
        VpnProc  = VpnProc->NEXT )
  {
    if ( IsVasyDebugLevel1() )
    {
      VasyPrintf( stdout, "+++ VasyAddProcInfo %s\n", VpnProc->NAME );
    }

    VasyAddVpnProcInfo( VpnProc );
  }

  if ( IsVasyDebugLevel1() )
  {
    VasyDebugSaveVpnFig( VpnFigure );
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "<-- VasyParseVbh2VpnFig %s\n\n", VbhFigure->NAME );
  }

  return( VpnFigure );
}

/*------------------------------------------------------------\
|                                                             |
|                         VasyParsePackage                    |
|                                                             |
\------------------------------------------------------------*/

void VasyParsePackage( FileName )

  char *FileName;
{
  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "--> VasyParsePackage %s\n\n", FileName );
  }

  if ( FileName == (char *)0 ) return;

  getvbpkg( FileName, VPN_IN );

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "<-- VasyParsePackage %s\n\n", FileName );
  }
}
