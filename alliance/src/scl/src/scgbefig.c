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
| Tool    :                     Scg                           |
|                                                             |
| File    :                  scgbefig.c                       |
|                                                             |
| Date    :                   28.04.98                        |
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
# include "mlo.h"
# include "abe.h"
# include "scl.h"

# include <stdio.h>

# include "scgbefig.h"

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

  static authtable *ScgHashNameNet = (authtable *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

static void scg_connect_schcon( SchCon, NetName )

  schcon_list *SchCon;
  char        *NetName;
{
  schnet_list *SchNet;
  authelem    *Element;

  Element = searchauthelem( ScgHashNameNet, NetName );

  if ( Element != (authelem *)0 )
  {
    SchNet = (schnet_list *)Element->VALUE;

    addschnetcon( SchNet, SchCon );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    Get Sch Figure From Befig                |
|                                                             |
\------------------------------------------------------------*/

schfig_list *befig2schfig( BeFigure  )

  befig_list *BeFigure;
{
  schfig_list  *SchFigure;
  schcon_list  *SchCon;
  schbox_list  *SchBox;
  schnet_list  *SchNet;
  chain_list   *ScanChain;
  chain_list   *Support;
  biabl_list   *ScanBiAbl;
  bepor_list   *ScanPort;
  beaux_list   *ScanAux;
  bebus_list   *ScanBus;
  bebux_list   *ScanBux;
  bereg_list   *ScanReg;
  beout_list   *ScanOut;
  char         *Value;
  unsigned char SchDir;
  char          Buffer[ 512 ];

  ScgHashNameNet = createauthtable( 100 );

  SchFigure = addschfig( BeFigure->NAME );
  SchFigure->SOURCE = (void *)BeFigure;
  SchFigure->SOURCE_TYPE = SCH_SOURCE_BEFIG;

/*
** First create all figure connectors and nets
*/
  for ( ScanPort  = BeFigure->BEPOR;
        ScanPort != (bepor_list *)0;
        ScanPort  = ScanPort->NEXT )
  {
    if ( ScanPort->DIRECTION == IN ) SchDir = SCH_CON_IN;
    else                             SchDir = SCH_CON_OUT;

    SchCon = addschfigcon( SchFigure, ScanPort->NAME, SchDir );

    SchCon->SOURCE = (void *)ScanPort;
    SchCon->SOURCE_TYPE = SCH_SOURCE_BEPOR;

    SchNet              = addschnet( SchFigure );
    SchNet->SOURCE      = (void *)ScanPort;
    SchNet->SOURCE_TYPE = SCH_SOURCE_BEPOR;

    SchCon->NET = SchNet;
    addschnetcon( SchNet, SchCon );

    addauthelem( ScgHashNameNet, ScanPort->NAME, (long)SchNet );
  }

  for ( ScanAux  = BeFigure->BEAUX;
        ScanAux != (beaux_list *)0;
        ScanAux  = ScanAux->NEXT )
  {
    if ( ! searchauthelem( ScgHashNameNet, ScanAux->NAME ) )
    {
      SchNet              = addschnet( SchFigure );
      SchNet->SOURCE      = (void *)ScanAux;
      SchNet->SOURCE_TYPE = SCH_SOURCE_BEAUX;

      addauthelem( ScgHashNameNet, ScanAux->NAME, (long)SchNet );
    }
  }

  for ( ScanReg  = BeFigure->BEREG;
        ScanReg != (bereg_list *)0;
        ScanReg  = ScanReg->NEXT )
  {
    if ( ! searchauthelem( ScgHashNameNet, ScanReg->NAME ) )
    {
      SchNet              = addschnet( SchFigure );
      SchNet->SOURCE      = (void *)ScanReg;
      SchNet->SOURCE_TYPE = SCH_SOURCE_BEREG;

      addauthelem( ScgHashNameNet, ScanReg->NAME, (long)SchNet );
    }
  }

  for ( ScanBux  = BeFigure->BEBUX;
        ScanBux != (bebux_list *)0;
        ScanBux  = ScanBux->NEXT )
  {
    if ( ! searchauthelem( ScgHashNameNet, ScanBux->NAME ) )
    {
      SchNet              = addschnet( SchFigure );
      SchNet->SOURCE      = (void *)ScanBux;
      SchNet->SOURCE_TYPE = SCH_SOURCE_BEBUX;

      addauthelem( ScgHashNameNet, ScanBux->NAME, (long)SchNet );
    }
  }

  for ( ScanBus  = BeFigure->BEBUS;
        ScanBus != (bebus_list *)0;
        ScanBus  = ScanBus->NEXT )
  {
    if ( ! searchauthelem( ScgHashNameNet, ScanBus->NAME ) )
    {
      SchNet              = addschnet( SchFigure );
      SchNet->SOURCE      = (void *)ScanBus;
      SchNet->SOURCE_TYPE = SCH_SOURCE_BEBUS;

      addauthelem( ScgHashNameNet, ScanBus->NAME, (long)SchNet );
    }
  }

  for ( ScanOut  = BeFigure->BEOUT;
        ScanOut != (beout_list *)0;
        ScanOut  = ScanOut->NEXT )
  {
    if ( ! searchauthelem( ScgHashNameNet, ScanOut->NAME ) )
    {
      SchNet              = addschnet( SchFigure );
      SchNet->SOURCE      = (void *)ScanOut;
      SchNet->SOURCE_TYPE = SCH_SOURCE_BEOUT;

      addauthelem( ScgHashNameNet, ScanOut->NAME, (long)SchNet );
    }
  }

/*
** Now create all boxes for aux, bux, reg etc .. 
*/
  for ( ScanAux  = BeFigure->BEAUX;
        ScanAux != (beaux_list *)0;
        ScanAux  = ScanAux->NEXT )
  {
    sprintf( Buffer, "beaux_%s", ScanAux->NAME );
    SchBox = addschbox( SchFigure, Buffer );
    SchBox->SOURCE = (void *)ScanAux;
    SchBox->SOURCE_TYPE = SCH_SOURCE_BEAUX;

    Support = getablexprsupport( ScanAux->ABL, ABL_SUPPORT_CHAIN );

    for ( ScanChain  = Support;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      Value = (char *)ScanChain->DATA;

      if ( ( Value[ 0 ] == '\'' ) ||
           ( Value[ 0 ] == '"'  ) ) continue;

      SchCon = addschboxcon( SchFigure, SchBox, Value, SCH_CON_IN );
      SchCon->SOURCE = (void *)ScanAux;
      SchCon->SOURCE_TYPE = SCH_SOURCE_BEAUX;

      scg_connect_schcon( SchCon, SchCon->NAME );
    }

    freechain( Support );

    SchCon = addschboxcon( SchFigure, SchBox, ScanAux->NAME, SCH_CON_OUT );
    scg_connect_schcon( SchCon, SchCon->NAME );
  }

  for ( ScanBus  = BeFigure->BEBUS;
        ScanBus != (bebus_list *)0;
        ScanBus  = ScanBus->NEXT )
  {
    sprintf( Buffer, "bebus_%s", ScanBus->NAME );
    SchBox = addschbox( SchFigure, Buffer );
    SchBox->SOURCE = (void *)ScanBus;
    SchBox->SOURCE_TYPE = SCH_SOURCE_BEBUS;

    Support = (chain_list *)0;

    for ( ScanBiAbl  = ScanBus->BIABL;
          ScanBiAbl != (biabl_list *)0;
          ScanBiAbl  = ScanBiAbl->NEXT )
    {
      Support = unionablexprsupport( Support, ScanBiAbl->CNDABL, ABL_SUPPORT_CHAIN );
      Support = unionablexprsupport( Support, ScanBiAbl->VALABL, ABL_SUPPORT_CHAIN );
    }

    for ( ScanChain  = Support;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      Value = (char *)ScanChain->DATA;

      if ( ( Value[ 0 ] == '\'' ) ||
           ( Value[ 0 ] == '"'  ) ) continue;

      SchCon = addschboxcon( SchFigure, SchBox, Value, SCH_CON_IN );
      SchCon->SOURCE = (void *)ScanBus;
      SchCon->SOURCE_TYPE = SCH_SOURCE_BEBUS;

      scg_connect_schcon( SchCon, SchCon->NAME );
    }

    freechain( Support );

    SchCon = addschboxcon( SchFigure, SchBox, ScanBus->NAME, SCH_CON_OUT );
    scg_connect_schcon( SchCon, SchCon->NAME );
  }

  for ( ScanBux  = BeFigure->BEBUX;
        ScanBux != (bebux_list *)0;
        ScanBux  = ScanBux->NEXT )
  {
    sprintf( Buffer, "bebux_%s", ScanBux->NAME );
    SchBox = addschbox( SchFigure, Buffer );
    SchBox->SOURCE = (void *)ScanBux;
    SchBox->SOURCE_TYPE = SCH_SOURCE_BEBUX;

    Support = (chain_list *)0;

    for ( ScanBiAbl  = ScanBux->BIABL;
          ScanBiAbl != (biabl_list *)0;
          ScanBiAbl  = ScanBiAbl->NEXT )
    {
      Support = unionablexprsupport( Support, ScanBiAbl->CNDABL, ABL_SUPPORT_CHAIN );
      Support = unionablexprsupport( Support, ScanBiAbl->VALABL, ABL_SUPPORT_CHAIN );
    }

    for ( ScanChain  = Support;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      Value = (char *)ScanChain->DATA;

      if ( ( Value[ 0 ] == '\'' ) ||
           ( Value[ 0 ] == '"'  ) ) continue;

      SchCon = addschboxcon( SchFigure, SchBox, Value, SCH_CON_IN );
      SchCon->SOURCE = (void *)ScanBux;
      SchCon->SOURCE_TYPE = SCH_SOURCE_BEBUX;

      scg_connect_schcon( SchCon, SchCon->NAME );
    }

    freechain( Support );

    SchCon = addschboxcon( SchFigure, SchBox, ScanBux->NAME, SCH_CON_OUT );
    scg_connect_schcon( SchCon, SchCon->NAME );
  }

  for ( ScanReg  = BeFigure->BEREG;
        ScanReg != (bereg_list *)0;
        ScanReg  = ScanReg->NEXT )
  {
    sprintf( Buffer, "bereg_%s", ScanReg->NAME );
    SchBox = addschbox( SchFigure, Buffer );
    SchBox->SOURCE = (void *)ScanReg;
    SchBox->SOURCE_TYPE = SCH_SOURCE_BEREG;

    Support = (chain_list *)0;

    for ( ScanBiAbl  = ScanReg->BIABL;
          ScanBiAbl != (biabl_list *)0;
          ScanBiAbl  = ScanBiAbl->NEXT )
    {
      Support = unionablexprsupport( Support, ScanBiAbl->CNDABL, ABL_SUPPORT_CHAIN );
      Support = unionablexprsupport( Support, ScanBiAbl->VALABL, ABL_SUPPORT_CHAIN );
    }

    for ( ScanChain  = Support;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      Value = (char *)ScanChain->DATA;

      if ( ( Value[ 0 ] == '\'' ) ||
           ( Value[ 0 ] == '"'  ) ) continue;

      SchCon = addschboxcon( SchFigure, SchBox, Value, SCH_CON_IN );
      SchCon->SOURCE = (void *)ScanReg;
      SchCon->SOURCE_TYPE = SCH_SOURCE_BEREG;

      scg_connect_schcon( SchCon, SchCon->NAME );
    }

    freechain( Support );

    SchCon = addschboxcon( SchFigure, SchBox, ScanReg->NAME, SCH_CON_OUT );
    scg_connect_schcon( SchCon, SchCon->NAME );
  }

  for ( ScanOut  = BeFigure->BEOUT;
        ScanOut != (beout_list *)0;
        ScanOut  = ScanOut->NEXT )
  {
    sprintf( Buffer, "beout_%s", ScanOut->NAME );
    SchBox = addschbox( SchFigure, Buffer );
    SchBox->SOURCE = (void *)ScanOut;
    SchBox->SOURCE_TYPE = SCH_SOURCE_BEOUT;

    Support = getablexprsupport( ScanOut->ABL, ABL_SUPPORT_CHAIN );

    for ( ScanChain  = Support;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      Value  = (char *)ScanChain->DATA;

      if ( ( Value[ 0 ] == '\'' ) ||
           ( Value[ 0 ] == '"'  ) ) continue;

      SchCon = addschboxcon( SchFigure, SchBox, Value, SCH_CON_IN );
      SchCon->SOURCE = (void *)ScanOut;
      SchCon->SOURCE_TYPE = SCH_SOURCE_BEOUT;

      scg_connect_schcon( SchCon, SchCon->NAME );
    }

    SchCon = addschboxcon( SchFigure, SchBox, ScanOut->NAME, SCH_CON_OUT );
    scg_connect_schcon( SchCon, SchCon->NAME );
  }

  destroyauthtable( ScgHashNameNet );

  return( SchFigure );
}
