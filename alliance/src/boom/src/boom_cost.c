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
| Tool    :                     BOOM                          |
|                                                             |
| File    :                  boom_cost.c                      |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   06.06.00                        |
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
# include "abe.h"
# include "abv.h"
# include "abt.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "boom_error.h"
# include "boom_debug.h"
# include "boom_shared.h"
# include "boom_cost.h"

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

  static long BoomOperSurfaceCost[ ABL_MAX_OPERATOR ][ 3 ] =
  {
     {  1250,  1500,  1750 },          /*  ABL_OR      */
     {  1250,  1500,  1750 },          /*  ABL_AND     */
     {  2250,  4500,  6750 },          /*  ABL_XOR     */
     {   750,     0,     0 },          /*  ABL_NOT     */
     {  1000,  1250,  1500 },          /*  ABL_NOR     */
     {  1000,  1250,  1500 },          /*  ABL_NAND    */
     {  2250,  4500,  6750 },          /*  ABL_NXOR    */
     {     0,     0,     0 }           /*  ABL_STABLE  */
  };

  static int        BoomFlagDepth   = 0;
  static int        BoomFlagSurface = 0;

  static authtable *BoomHashAux = (authtable *)0;
  static authtable *BoomHashBux = (authtable *)0;
  static authtable *BoomHashBus = (authtable *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    BoomGiveAblSurfaceCost                   |
|                                                             |
\------------------------------------------------------------*/

static long LocalBoomGiveAblSurfaceCost( Expr )

  ablexpr *Expr;
{
  long Oper;
  long Cost;
  long NumOper;
  long NextStep;

  if ( ABL_ATOM( Expr ) )
  {
    return( 0 );
  }

  Oper    = ABL_OPER( Expr );
  Cost    = 0;
  NumOper = 0;

  while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
  {
    Cost += LocalBoomGiveAblSurfaceCost( ABL_CAR( Expr ) );

    NumOper++;
  }

  while ( NumOper )
  {
    NextStep = NumOper / 4;

    if ( NextStep )
    {
      Cost += NextStep * BoomOperSurfaceCost[ Oper ][ BOOM_OPER_ARITY_4 ];
    }

    NumOper %= 4;

    if ( NumOper == 3 )
    {
      if ( NextStep >= 1 ) NextStep += 3;
      else                 Cost     += BoomOperSurfaceCost[ Oper ][ BOOM_OPER_ARITY_3 ];
    }
    else
    if ( NumOper == 2 )
    {
      if ( ( NextStep <= 2 ) && 
           ( NextStep != 0 ) )
      {
        NextStep += 2;
      }
      else
      {
        Cost += BoomOperSurfaceCost[ Oper ][ BOOM_OPER_ARITY_2 ];
      }
    }
    else
    if ( NumOper == 1 )
    {
      if ( Oper == ABL_NOT )
      {
        Cost += BoomOperSurfaceCost[ Oper ][ BOOM_OPER_ARITY_1 ];
      }
      else
      if ( NextStep != 0 )  NextStep++;
    }

    NumOper  = NextStep;
    NextStep = 0;
  }

  return( Cost );
}

static long BoomGiveAblSurfaceCost( Expr )

  ablexpr *Expr;
{
  long Cost;

  if ( ! BoomFlagSurface )
  {
    return( 0 );
  }

  Cost = LocalBoomGiveAblSurfaceCost( Expr );

  if ( IsBoomDebugLevel2() )
  {
    fprintf( stdout, "BoomGiveAblSurfaceCost\n" );
    viewablexprln( Expr, ABL_VIEW_VHDL );
    fprintf( stdout, "%ld\n", Cost );
  }

  return( Cost );
}


/*------------------------------------------------------------\
|                                                             |
|                    BoomGiveAblLiteralCost                   |
|                                                             |
\------------------------------------------------------------*/

static long LocalBoomGiveAblLiteralCost( Expr )

  ablexpr *Expr;
{
  long Oper;
  long Cost;

  if ( ABL_ATOM( Expr ) )
  {
    return( 1 );
  }

  Cost = 0;
  Oper = ABL_OPER( Expr );

  if ( Oper != ABL_STABLE ) 
  {
    while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
    {
      Cost += LocalBoomGiveAblLiteralCost( ABL_CAR( Expr ) );
    }
  }

  return( Cost );
}

long BoomGiveAblLiteralCost( Expr )

  ablexpr *Expr;
{
  long Cost;

  if ( ABL_ATOM( Expr ) )
  {
    Cost = 0;
  }
  else
  {
    Cost = LocalBoomGiveAblLiteralCost( Expr );
  }

  if ( IsBoomDebugLevel2() )
  {
    fprintf( stdout, "BoomGiveAblLiteralCost\n" );
    viewablexprln( Expr, ABL_VIEW_VHDL );
    fprintf( stdout, "%ld\n", Cost );
  }

  return( Cost );
}

/*------------------------------------------------------------\
|                                                             |
|                    BoomGiveAblDepthCost                     |
|                                                             |
\------------------------------------------------------------*/

static long BoomGiveAblDepthCost();

static long LocalBoomGiveAblDepthCost( Expr )

  ablexpr *Expr;
{
  boombeauxinfo *AuxInfo;
  boombebuxinfo *BuxInfo;
  boombebusinfo *BusInfo;
  bebus_list    *BehBus;
  bebux_list    *BehBux;
  beaux_list    *BehAux;
  char          *Value;
  authelem      *Element;
  biabl_list    *BiAbl;
  long           Oper;
  long           Cost;
  long           Arity;
  long           NewCost;

  Cost = 0;

  if ( ABL_ATOM( Expr ) )
  {
    Value = ABL_ATOM_VALUE( Expr );

    if ( ( Value != ABL_ATOM_NAME_ONE      ) &&
         ( Value != ABL_ATOM_NAME_ZERO     ) &&
         ( Value != ABL_ATOM_NAME_DC       ) &&
         ( Value != ABL_ATOM_NAME_TRISTATE ) )
    {
      Element = searchauthelem( BoomHashAux, Value );

      if ( Element != (authelem *)0 )
      {
        BehAux = (beaux_list *)Element->VALUE;
        AuxInfo = GetBoomBehAuxInfo( BehAux );

        if ( ! IsBoomBehAuxTag( BehAux ) )
        {
          SetBoomBehAuxTag( BehAux );
          AuxInfo->DEPTH_COST = BoomGiveAblDepthCost( BehAux->ABL );
        }

        return( AuxInfo->DEPTH_COST );
      }

      Element = searchauthelem( BoomHashBus, Value );

      if ( Element != (authelem *)0 )
      {
        BehBus = (bebus_list *)Element->VALUE;
        BusInfo = GetBoomBehBusInfo( BehBus );

        if ( ! IsBoomBehAuxTag( BehBus ) )
        {
          SetBoomBehAuxTag( BehBus );

          BusInfo->DEPTH_COST = 0;

          for ( BiAbl  = BehBus->BIABL;
                BiAbl != (biabl_list *)0;
                BiAbl  = BiAbl->NEXT )
          {
            if ( BiAbl->VALABL != (chain_list *)0 )
            {
              NewCost = BoomGiveAblDepthCost( BiAbl->VALABL );
      
              if ( NewCost > BusInfo->DEPTH_COST )
              {
                BusInfo->DEPTH_COST = NewCost;
              }
            }
      
            if ( BiAbl->CNDABL != (chain_list *)0 )
            {
              NewCost = BoomGiveAblDepthCost( BiAbl->CNDABL );
      
              if ( NewCost > BusInfo->DEPTH_COST )
              {
                BusInfo->DEPTH_COST = NewCost;
              }
            }
          }
        }

        return( BusInfo->DEPTH_COST );
      }

      Element = searchauthelem( BoomHashBux, Value );

      if ( Element != (authelem *)0 )
      {
        BehBux = (bebux_list *)Element->VALUE;
        BuxInfo = GetBoomBehBuxInfo( BehBux );

        if ( ! IsBoomBehAuxTag( BehBux ) )
        {
          BuxInfo->DEPTH_COST = 0;

          for ( BiAbl  = BehBux->BIABL;
                BiAbl != (biabl_list *)0;
                BiAbl  = BiAbl->NEXT )
          {
            if ( BiAbl->VALABL != (chain_list *)0 )
            {
              NewCost = BoomGiveAblDepthCost( BiAbl->VALABL );
      
              if ( NewCost > BuxInfo->DEPTH_COST )
              {
                BuxInfo->DEPTH_COST = NewCost;
              }
            }
      
            if ( BiAbl->CNDABL != (chain_list *)0 )
            {
              NewCost = BoomGiveAblDepthCost( BiAbl->CNDABL );
      
              if ( NewCost > BuxInfo->DEPTH_COST )
              {
                BuxInfo->DEPTH_COST = NewCost;
              }
            }
          }
      
          SetBoomBehAuxTag( BehBux );
        }

        return( BuxInfo->DEPTH_COST );
      }
    }
  }
  else
  {
    Oper = ABL_OPER( Expr );
  
    if ( Oper != ABL_STABLE ) 
    {
      Arity = 0;

      while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
      {
        Arity++;

        NewCost = LocalBoomGiveAblDepthCost( ABL_CAR( Expr ) );
  
        if ( NewCost > Cost ) Cost = NewCost;
      }

      if ( Oper != ABL_NOT )
      {
        Cost += BoomComputeLog2( Arity );
      }
    }
  }

  return( Cost );
}

static long BoomGiveAblDepthCost( Expr )

  ablexpr *Expr;
{
  long Cost;

  if ( ! BoomFlagDepth )
  {
    return( 0 );
  }

  Cost = LocalBoomGiveAblDepthCost( Expr );

  if ( IsBoomDebugLevel2() )
  {
    fprintf( stdout, "BoomGiveAblDepthCost\n" );
    viewablexprln( Expr, ABL_VIEW_VHDL );
    fprintf( stdout, "%ld\n", Cost );
  }

  return( Cost );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomBehPrintCost                       |
|                                                             |
\------------------------------------------------------------*/

void BoomBehPrintCost( Text, BehFigure )

  char       *Text;
  befig_list *BehFigure;
{
  boombefiginfo *FigInfo;

  FigInfo = GetBoomBehFigInfo( BehFigure );

  fprintf( stdout, "\t--> %s cost\n", Text );
  fprintf( stdout, "\t    Surface  : %ld\n"    , FigInfo->SURFACE_COST );
  fprintf( stdout, "\t    Depth    : %ld\n"    , FigInfo->DEPTH_COST   );
  fprintf( stdout, "\t    Literals : %ld\n"    , FigInfo->LITERAL_COST );
  /*
  fprintf( stdout, "\t    Delay    : %.3f ns\n", FigInfo->DEPTH_COST * 0.200 );
  */
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomBehGiveFullCost                    |
|                                                             |
\------------------------------------------------------------*/

long BoomBehGiveFullCost( BehFigure, Quick )

  befig_list *BehFigure;
  int         Quick;
{
  boombefiginfo *FigInfo;
  beout_list    *BehOut;
  beaux_list    *BehAux;
  bereg_list    *BehReg;
  bebus_list    *BehBus;
  bebux_list    *BehBux;
  biabl_list    *BiAbl;
  boombeauxinfo *AuxInfo;
  boombeoutinfo *OutInfo;
  boombereginfo *RegInfo;
  boombebusinfo *BusInfo;
  boombebuxinfo *BuxInfo;
  long           LiteralCost;
  long           SurfaceCost;
  long           DepthCost;
  long           GlobalCost;
  long           NewCost;
  long           PerCent;
  char           Buffer[ 32 ];

  FigInfo = GetBoomBehFigInfo( BehFigure );

  if ( BoomHashAux == (authtable *)0 )
  {
    BoomHashAux = createauthtable( 100 );
    BoomHashBux = createauthtable( 100 );
    BoomHashBus = createauthtable( 100 );
  }

  for ( BehAux  = BehFigure->BEAUX;
        BehAux != (beaux_list *)0;
        BehAux  = BehAux->NEXT )
  {
    addauthelem( BoomHashAux, BehAux->NAME, (long)BehAux );
  }

  for ( BehBux  = BehFigure->BEBUX;
        BehBux != (bebux_list *)0;
        BehBux  = BehBux->NEXT )
  {
    addauthelem( BoomHashBux, BehBux->NAME, (long)BehBux );
  }

  for ( BehBus  = BehFigure->BEBUS;
        BehBus != (bebus_list *)0;
        BehBus  = BehBus->NEXT )
  {
    addauthelem( BoomHashBus, BehBus->NAME, (long)BehBus );
  }

  LiteralCost = 0;
  SurfaceCost = 0;
  DepthCost   = 0;
  
  BoomFlagDepth   = 1;
  BoomFlagSurface = 1;

  if ( Quick )
  {
    if ( FigInfo->OPTIM_DELAY_PERCENT == 0   ) BoomFlagDepth   = 0;
    if ( FigInfo->USE_LITERAL                ) BoomFlagSurface = 0;

    if ( FigInfo->OPTIM_DELAY_PERCENT == 100 ) BoomFlagSurface  = 0;
  }

  for ( BehAux  = BehFigure->BEAUX;
        BehAux != (beaux_list *)0;
        BehAux  = BehAux->NEXT )
  {
    AuxInfo = GetBoomBehAuxInfo( BehAux );

    AuxInfo->LITERAL_COST = BoomGiveAblLiteralCost( BehAux->ABL );
    LiteralCost += AuxInfo->LITERAL_COST;

    AuxInfo->SURFACE_COST = BoomGiveAblSurfaceCost( BehAux->ABL );
    SurfaceCost += AuxInfo->SURFACE_COST;

    if ( ! IsBoomBehAuxTag( BehAux ) )
    {
      AuxInfo->DEPTH_COST = BoomGiveAblDepthCost( BehAux->ABL );
      SetBoomBehAuxTag( BehAux );
    }
  }

  for ( BehOut  = BehFigure->BEOUT;
        BehOut != (beout_list *)0;
        BehOut  = BehOut->NEXT )
  {
    OutInfo = GetBoomBehOutInfo( BehOut );

    OutInfo->LITERAL_COST = BoomGiveAblLiteralCost( BehOut->ABL );
    LiteralCost += OutInfo->LITERAL_COST;

    OutInfo->SURFACE_COST = BoomGiveAblSurfaceCost( BehOut->ABL );
    SurfaceCost += OutInfo->SURFACE_COST;

    OutInfo->DEPTH_COST   = BoomGiveAblDepthCost( BehOut->ABL );

    if ( OutInfo->DEPTH_COST > DepthCost )
    {
      DepthCost = OutInfo->DEPTH_COST;
    }
  }

  for ( BehBus  = BehFigure->BEBUS;
        BehBus != (bebus_list *)0;
        BehBus  = BehBus->NEXT )
  {
    BusInfo = GetBoomBehBusInfo( BehBus );
    BusInfo->LITERAL_COST = 0;
    BusInfo->SURFACE_COST = 0;

    if ( ! IsBoomBehAuxTag( BehBus ) )
    {
      BusInfo->DEPTH_COST = 0;
    }

    for ( BiAbl  = BehBus->BIABL;
          BiAbl != (biabl_list *)0;
          BiAbl  = BiAbl->NEXT )
    {
      if ( BiAbl->VALABL != (chain_list *)0 )
      {
        BusInfo->LITERAL_COST += BoomGiveAblLiteralCost( BiAbl->VALABL );
        BusInfo->SURFACE_COST += BoomGiveAblSurfaceCost( BiAbl->VALABL );

        if ( ! IsBoomBehAuxTag( BehBus ) )
        {
          NewCost = BoomGiveAblDepthCost( BiAbl->VALABL );

          if ( NewCost > BusInfo->DEPTH_COST )
          {
            BusInfo->DEPTH_COST = NewCost;
          }
        }
      }

      if ( BiAbl->CNDABL != (chain_list *)0 )
      {
        BusInfo->LITERAL_COST += BoomGiveAblLiteralCost( BiAbl->CNDABL );
        BusInfo->SURFACE_COST += BoomGiveAblSurfaceCost( BiAbl->CNDABL );

        if ( ! IsBoomBehAuxTag( BehBus ) )
        {
          NewCost = BoomGiveAblDepthCost( BiAbl->CNDABL );

          if ( NewCost > BusInfo->DEPTH_COST )
          {
            BusInfo->DEPTH_COST = NewCost;
          }
        }
      }
    }

    SetBoomBehAuxTag( BehBus );

    LiteralCost += BusInfo->LITERAL_COST;
    SurfaceCost += BusInfo->SURFACE_COST;

    if ( BusInfo->DEPTH_COST > DepthCost )
    {
      DepthCost = BusInfo->DEPTH_COST;
    }
  }
  for ( BehBux  = BehFigure->BEBUX;
        BehBux != (bebux_list *)0;
        BehBux  = BehBux->NEXT )
  {
    BuxInfo = GetBoomBehBuxInfo( BehBux );
    BuxInfo->LITERAL_COST = 0;
    BuxInfo->SURFACE_COST = 0;

    if ( ! IsBoomBehAuxTag( BehBux ) )
    {
      BuxInfo->DEPTH_COST = 0;
    }

    for ( BiAbl  = BehBux->BIABL;
          BiAbl != (biabl_list *)0;
          BiAbl  = BiAbl->NEXT )
    {
      if ( BiAbl->VALABL != (chain_list *)0 )
      {
        BuxInfo->LITERAL_COST += BoomGiveAblLiteralCost( BiAbl->VALABL );
        BuxInfo->SURFACE_COST += BoomGiveAblSurfaceCost( BiAbl->VALABL );

        if ( ! IsBoomBehAuxTag( BehBux ) )
        {
          NewCost = BoomGiveAblDepthCost( BiAbl->VALABL );

          if ( NewCost > BuxInfo->DEPTH_COST )
          {
            BuxInfo->DEPTH_COST = NewCost;
          }
        }
      }

      if ( BiAbl->CNDABL != (chain_list *)0 )
      {
        BuxInfo->LITERAL_COST += BoomGiveAblLiteralCost( BiAbl->CNDABL );
        BuxInfo->SURFACE_COST += BoomGiveAblSurfaceCost( BiAbl->CNDABL );

        if ( ! IsBoomBehAuxTag( BehBux ) )
        {
          NewCost = BoomGiveAblDepthCost( BiAbl->CNDABL );

          if ( NewCost > BuxInfo->DEPTH_COST )
          {
            BuxInfo->DEPTH_COST = NewCost;
          }
        }
      }
    }

    SetBoomBehAuxTag( BehBux );

    LiteralCost += BuxInfo->LITERAL_COST;
    SurfaceCost += BuxInfo->SURFACE_COST;

    if ( BuxInfo->DEPTH_COST > DepthCost )
    {
      DepthCost = BuxInfo->DEPTH_COST;
    }
  }

  for ( BehReg  = BehFigure->BEREG;
        BehReg != (bereg_list *)0;
        BehReg  = BehReg->NEXT )
  {
    RegInfo = GetBoomBehRegInfo( BehReg );
    RegInfo->LITERAL_COST = 0;
    RegInfo->SURFACE_COST = 0;
    RegInfo->DEPTH_COST   = 0;

    for ( BiAbl  = BehReg->BIABL;
          BiAbl != (biabl_list *)0;
          BiAbl  = BiAbl->NEXT )
    {
      if ( BiAbl->VALABL != (chain_list *)0 )
      {
        RegInfo->LITERAL_COST += BoomGiveAblLiteralCost( BiAbl->VALABL );
        RegInfo->SURFACE_COST += BoomGiveAblSurfaceCost( BiAbl->VALABL );

        NewCost = BoomGiveAblDepthCost( BiAbl->VALABL );

        if ( NewCost > RegInfo->DEPTH_COST )
        {
          RegInfo->DEPTH_COST = NewCost;
        }
      }

      if ( BiAbl->CNDABL != (chain_list *)0 )
      {
        RegInfo->LITERAL_COST += BoomGiveAblLiteralCost( BiAbl->CNDABL );
        RegInfo->SURFACE_COST += BoomGiveAblSurfaceCost( BiAbl->CNDABL );

        NewCost = BoomGiveAblDepthCost( BiAbl->CNDABL );

        if ( NewCost > RegInfo->DEPTH_COST )
        {
          RegInfo->DEPTH_COST = NewCost;
        }
      }
    }

    LiteralCost += RegInfo->LITERAL_COST;
    SurfaceCost += RegInfo->SURFACE_COST;

    if ( RegInfo->DEPTH_COST > DepthCost )
    {
      DepthCost = RegInfo->DEPTH_COST;
    }
  }

  resetauthtable( BoomHashAux );
  resetauthtable( BoomHashBus );
  resetauthtable( BoomHashBux );

  for ( BehAux  = BehFigure->BEAUX;
        BehAux != (beaux_list *)0;
        BehAux  = BehAux->NEXT )
  {
    ClearBoomBehAuxTag( BehAux );
  }

  for ( BehBux  = BehFigure->BEBUX;
        BehBux != (bebux_list *)0;
        BehBux  = BehBux->NEXT )
  {
    ClearBoomBehAuxTag( BehBux );
  }

  for ( BehBus  = BehFigure->BEBUS;
        BehBus != (bebus_list *)0;
        BehBus  = BehBus->NEXT )
  {
    ClearBoomBehAuxTag( BehBus );
  }

  if ( IsBoomDebugLevel1() )
  {
    if ( FigInfo->USE_LITERAL )
    {
      sprintf( Buffer, "%ld", LiteralCost );
    }
    else
    {
      sprintf( Buffer, "%ld", SurfaceCost );
    }

    BoomDebugSaveBehFigure( BehFigure, Buffer );
  }

  FigInfo->LITERAL_COST = LiteralCost;
  FigInfo->SURFACE_COST = SurfaceCost;
  FigInfo->DEPTH_COST   = DepthCost;


  if ( FigInfo->INITIAL_DEPTH_COST == 0 )
  {
    FigInfo->INITIAL_DEPTH_COST = DepthCost + 1;
  }
  
  if ( FigInfo->INITIAL_LITERAL_COST == 0 )
  {
    FigInfo->INITIAL_LITERAL_COST = LiteralCost + 1;
  }

  if ( FigInfo->INITIAL_SURFACE_COST == 0 )
  {
    FigInfo->INITIAL_SURFACE_COST = SurfaceCost + 1;
  }

  SurfaceCost = ( (double) SurfaceCost * 1000.0 / (double) FigInfo->INITIAL_SURFACE_COST );
  LiteralCost = ( (double) LiteralCost * 1000.0 / (double) FigInfo->INITIAL_LITERAL_COST );
  DepthCost   = ( (double) DepthCost   * 1000.0 / (double) FigInfo->INITIAL_DEPTH_COST   );

  PerCent    = FigInfo->OPTIM_DELAY_PERCENT;

  if ( FigInfo->USE_LITERAL )
  {
    GlobalCost = ( ( LiteralCost * ( 100 - PerCent ) ) + 
                   ( DepthCost   * PerCent           ) ) / 100;
  }
  else
  {
    GlobalCost = ( ( SurfaceCost * ( 100 - PerCent ) ) + 
                   ( DepthCost   * PerCent           ) ) / 100;
  }

  FigInfo->GLOBAL_COST = GlobalCost;

  return( GlobalCost );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomBehGiveCost                        |
|                                                             |
\------------------------------------------------------------*/

long BoomBehGiveCost( BehFigure )

  befig_list *BehFigure;
{
  return ( BoomBehGiveFullCost( BehFigure, 1 ) );
}
