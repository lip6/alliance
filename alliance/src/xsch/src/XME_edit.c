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
| Tool    :                   XSCH                            |
|                                                             |
| File    :                   Edit.c                          |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                        Miramond  Benoit                     |
| 			 Picault Stephane                     |
|                                                             |
| Date    :                   01.06.96                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <math.h>
# include <Xm/Xm.h>
# include <Xm/PushBG.h>

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "mlo.h"
# include "abe.h"

# include "scl.h"
# include "XSB.h"
# include "XTB.h"
# include "XMX.h"
# include "XSC.h"
# include "XME.h"
# include "XMV.h"
 
# include "XME_edit.h"
# include "XME_select.h"
# include "XME_panel.h"
# include "XME_message.h"

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
|                        Identify Buffer                      |
|                                                             |
\------------------------------------------------------------*/

static char     XschIdentifyMessage[XSCH_IDENTIFY_MESSAGE_SIZE];
static char     XschIdentifyBuffer[XSCH_IDENTIFY_BUFFER_SIZE];


static char    *XschScanIdentify;
static long     XschIdentifyLength;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         XschAddIdentify                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         XschAddIdentify                     |
|                                                             |
\------------------------------------------------------------*/

short XschAddIdentify(Obj)

  xschobj_list   *Obj;
{
  schbox_list    *SchBox;
  schnet_list    *SchNet;
  schcon_list    *SchCon;
  loins_list     *LoIns;
  losig_list     *LoSig;
  lotrs_list     *LoTrs;
  locon_list     *LoCon;
  beaux_list     *BeAux;
  beout_list     *BeOut;
  bereg_list     *BeReg;
  biabl_list     *BiAbl;
  chain_list     *ScanChain;
  char           *Scan;
  char           *Comment;
  long            Length;

  autbegin();

  XschIdentifyBuffer[0] = '\0';
  Comment = (char *)0;

  if ( IsXschSchBox( Obj ) )
  {
    SchBox = (schbox_list *)Obj->USER;
    Comment = SchBox->USER;

    if ( SchBox->SOURCE_TYPE == SCH_SOURCE_LOINS )
    {
      LoIns = (loins_list *)SchBox->SOURCE;

      sprintf( XschIdentifyBuffer,
               "Instance :\n> Name     %s\n> Model    %s\n\n",
               LoIns->INSNAME, LoIns->FIGNAME );
    }  
    else
    if ( SchBox->SOURCE_TYPE == SCH_SOURCE_LOTRS )
    {
      LoTrs = (lotrs_list *)SchBox->SOURCE;

      sprintf( XschIdentifyBuffer,
                "Transistor :\n> Type     %s\n> Name     %s\n> X,Y      %.3f,%.3f\n",
                ( LoTrs->TYPE   == TRANSN ) ? "NTRANS" : "PTRANS",
                ( LoTrs->TRNAME == NULL   ) ? "none"   : LoTrs->TRNAME,
                (float)LoTrs->X / (float)SCALE_X,
                (float)LoTrs->Y / (float)SCALE_X );
    }
    else
    if ( SchBox->SOURCE_TYPE == SCH_SOURCE_BEAUX )
    {
      BeAux = (beaux_list *)SchBox->SOURCE;

      sprintf( XschIdentifyBuffer, 
               "BeAux :\n> Name     %s\n> Abl    %s\n\n", 
               BeAux->NAME, viewablexprstr( BeAux->ABL, ABL_VIEW_VHDL ) );
    }
    else
    if ( SchBox->SOURCE_TYPE == SCH_SOURCE_BEOUT )
    {
      BeOut = (beout_list *)SchBox->SOURCE;

      sprintf( XschIdentifyBuffer, 
               "BeOut :\n> Name     %s\n> Abl    %s\n\n",
               BeOut->NAME, viewablexprstr( BeOut->ABL, ABL_VIEW_VHDL ) );
    }
    else
    if ( SchBox->SOURCE_TYPE == SCH_SOURCE_BEREG )
    {
      BeReg = (bereg_list *)SchBox->SOURCE;

      Scan = XschIdentifyBuffer;
      sprintf( XschIdentifyBuffer, "BeReg :\n> Name     %s\n\n", BeReg->NAME );
      Scan += strlen( Scan );

      for ( BiAbl  = BeReg->BIABL;
            BiAbl != (biabl_list *)0;
            BiAbl  = BiAbl->NEXT )
      {
        sprintf( Scan, "> Cond %s\n", 
                 viewablexprstr( BiAbl->CNDABL, ABL_VIEW_VHDL ) );
        Scan += strlen( Scan );
        sprintf( Scan, "> Value %s\n", 
                 viewablexprstr( BiAbl->VALABL, ABL_VIEW_VHDL ) );
        Scan += strlen( Scan );
      }

      strcpy( Scan, "\n" );
    }
  }
  else
  if ( IsXschSchCon( Obj ) )
  {
    SchCon  = (schcon_list *) Obj->USER;
    Comment = SchCon->USER;

    if ( SchCon->SOURCE_TYPE == SCH_SOURCE_LOCON )
    {
      LoCon = (locon_list *)SchCon->SOURCE;

      sprintf( XschIdentifyBuffer, "Connector :\n> Name %s\n\n", LoCon->NAME );
    }
    else
    {
      sprintf( XschIdentifyBuffer, "Connector :\n> Name %s\n\n", SchCon->NAME );
    }
  }
  else
  if ( IsXschSchNet( Obj ) )
  {
    SchNet  = (schnet_list *)Obj->USER;
    Comment = SchNet->USER;

    if ( SchNet->SOURCE_TYPE == SCH_SOURCE_LOSIG )
    {
      LoSig = (losig_list *)SchNet->SOURCE;

      Scan = XschIdentifyBuffer;
      strcpy( Scan, "Signal :\n\n");
      Scan += strlen( Scan );
      sprintf( Scan, "> Index  %ld\n> Name   ", LoSig->INDEX );
      Scan += strlen( Scan );

      for ( ScanChain  = LoSig->NAMECHAIN;
            ScanChain != (chain_list *)0;
            ScanChain  = ScanChain->NEXT )
      {
        strcpy( Scan, (char *)ScanChain->DATA );
        Scan += strlen( Scan );
        strcpy( Scan, " " );
        Scan += strlen( Scan );
      }

      strcpy( Scan, "\n" );
      Scan += strlen( Scan );

      for ( ScanChain  = SchNet->CON_NET;
            ScanChain != (chain_list *)0;
            ScanChain  = ScanChain->NEXT )
      {
        SchCon = (schcon_list *)ScanChain->DATA;

        if ( SchCon->ROOT_TYPE == SCH_ROOT_CON_BOX )
        {
          SchBox = (schbox_list *)SchCon->ROOT;

          if ( ( ! IsSchBoxTransparence( SchBox ) ) &&
               ( ! IsSchBoxClusterNet( SchBox   ) ) )
          {
            sprintf( Scan, "> Port %s of %s\n", SchCon->NAME, SchBox->NAME );
            Scan += strlen(Scan);
          }
        }
      }
    }
    else
    {
      sprintf( XschIdentifyBuffer, "Net :\n" );
    }
  }

  Length = strlen( XschIdentifyBuffer );

  if ( Comment != (char *)0 )
  {
    Scan = XschIdentifyBuffer + Length;
    sprintf( Scan, "[%s]\n", Comment );

    Length += strlen( Scan );
  }

  if ( ( XschIdentifyLength + Length ) < (XSCH_IDENTIFY_MESSAGE_SIZE - 1))
  {
    strcpy( XschScanIdentify, XschIdentifyBuffer );

    XschScanIdentify   += Length;
    XschIdentifyLength += Length;

    autend();
    return ( XSCH_TRUE );
  }

  autend();
  return ( XSCH_FALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                         XschEditIdentify                    |
|                                                             |
\------------------------------------------------------------*/

void XschEditIdentify( X1, Y1 )

  long X1;
  long Y1;
{
  xschselect_list *Select;

  strcpy( XschIdentifyMessage, "No element found !" );

  XschScanIdentify   = XschIdentifyMessage;
  XschIdentifyLength = 0;

  XschEditSelectPoint( X1, Y1 );

  for ( Select  = XschHeadSelect;
        Select != (xschselect_list *)0;
        Select  = Select->NEXT )
  {
    if ( ! XschAddIdentify( Select->OBJECT ) ) break;
  }

  XschDelSelect();

  XschDisplayEditIdentify( XschIdentifyMessage );
  XschEnterPanel( &XschEditIdentifyPanel       );
}

/*------------------------------------------------------------\
|                                                             |
|                         XschEditConnected                   |
|                                                             |
\------------------------------------------------------------*/

void XschEditConnected( X1, Y1 )

  long X1;
  long Y1;
{
  xschselect_list *Select;
  xschobj_list    *Obj;

  if ( XschHeadConnect != (xschselect_list *)NULL )
  {
    XschDelConnect();
    XschZoomRefresh();
  }

  XschEditSelectPoint( X1, Y1 );

  XschDisplaySelectList( (char *)NULL );

  for ( Select  = XschHeadSelect;
        Select != (xschselect_list *)NULL;
        Select  = Select->NEXT)
  {
    XschAddSelectList( Select->OBJECT );
  }

  if ( XschHeadSelect == (xschselect_list *)NULL )
  {
    XschWarningMessage(XschMainWindow, "No element found !");
  }
  else
  {
    if ( XschHeadSelect->NEXT != (xschselect_list *)NULL )
    {
      XschEnterPanel(&XschEditSelectPanel);
      XschLimitedLoop(XschEditSelectPanel.PANEL);
    }
    else
    {
      XschAcceptObject(XschHeadSelect->OBJECT);
      XschDisplayObject(XschHeadSelect->OBJECT);
    }
  }

  XschPurgeSelect();

  for ( Select  = XschHeadSelect;
        Select != (xschselect_list *)NULL;
        Select  = Select->NEXT )
  {
    XschAddConnect( Select->OBJECT );
  }

  if ( XschHeadSelect != (xschselect_list *)NULL )
  {
    if ( XschHeadSelect->NEXT == (xschselect_list *)NULL )
    {
      Obj = XschHeadSelect->OBJECT;
      XschDelSelect();
      XschDisplayObject(Obj);
    }
    else
    {
      XschDelSelect();
      XschZoomRefresh();
    }
  }
}
