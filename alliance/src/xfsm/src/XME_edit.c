/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
| Tool    :                   XFSM                            |
|                                                             |
| File    :                   Edit.c                          |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   04.12.96                        |
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
# include "fsm.h"
# include "XSB.h"
# include "XTB.h"
# include "XMX.h"
# include "XFS.h"
# include "XME.h"

# include "XME_edit.h"
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

  static char  XfsmIdentifyMessage[ XFSM_IDENTIFY_MESSAGE_SIZE ];
  static char  XfsmIdentifyBuffer [ XFSM_IDENTIFY_BUFFER_SIZE  ];
  static char  XfsmIdentifyLocout [ XFSM_IDENTIFY_LOCOUT_SIZE  ];

  static char *XfsmScanIdentify;
  static long  XfsmIdentifyLength;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         XfsmAddIdentify                     |
|                                                             |
\------------------------------------------------------------*/

short XfsmAddIdentify( Obj )

  xfsmobj_list *Obj;
{
  fsmstate_list  *State;
  fsmtrans_list  *Trans;
  fsmlocout_list *Locout;
  char           *ScanString;
  char           *AblString;
  long            Length;

  autbegin();

  XfsmIdentifyBuffer[0] = '\0';

  if ( IsXfsmState( Obj ) )
  {
    State = (fsmstate_list *)( Obj->USER );

    ScanString  = XfsmIdentifyLocout;
    strcpy( ScanString, "  " );
    ScanString += 2;

    for ( Locout  = State->LOCOUT;
          Locout != (fsmlocout_list *)NULL;
          Locout  = Locout->NEXT )
    {
      if ( Locout->ABL != (chain_list *)NULL )
      {
        strcpy( ScanString, Locout->OUT->NAME );
        ScanString += strlen( ScanString );
        strcpy( ScanString, " <= " );
        ScanString += 4;
        AblString = viewablexprstr( Locout->ABL, ABL_VIEW_VHDL );
        Length = strlen( AblString );
        strcpy( ScanString, AblString );
        ScanString += Length;
        strcpy( ScanString, "\n  " );
        ScanString += 3;
      }
    }

    sprintf( XfsmIdentifyBuffer, 
             "STATE :\n\n  NAME : %s\n  LOCOUT : \n%s\n\n",
             State->NAME, 
             XfsmIdentifyLocout );
  }
  else
  if ( IsXfsmTrans( Obj ) )
  {
    Trans = (fsmtrans_list *)( Obj->USER );

    if ( Trans->ABL != (chain_list *)NULL )
    {
      AblString = viewablexprstr( Trans->ABL, ABL_VIEW_VHDL );
    }
    else
    {
      AblString = "";
    }

    sprintf( XfsmIdentifyBuffer,
             "TRANSITION :\n\n  FROM : %s TO : %s\n  ABL : %s\n\n",
             Trans->FROM->NAME, 
             Trans->TO->NAME,
             AblString );
  }

  Length = strlen( XfsmIdentifyBuffer );

  if ( ( XfsmIdentifyLength + Length ) < ( XFSM_IDENTIFY_MESSAGE_SIZE - 1 ) )
  {
    strcpy( XfsmScanIdentify, XfsmIdentifyBuffer );

    XfsmScanIdentify   += Length; 
    XfsmIdentifyLength += Length;

    autend();
    return( XFSM_TRUE );
  }

/*\
  strcpy( XfsmScanIdentify, "#" );
\*/
 
  autend();
  return( XFSM_FALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                         XfsmEditIdentify                    |
|                                                             |
\------------------------------------------------------------*/

void XfsmEditIdentify( X1, Y1 )

  long X1;
  long Y1;
{
  xfsmselect_list *Select;
  xfsmobj_list    *Obj;

  strcpy( XfsmIdentifyMessage, "No element found !" );

  XfsmScanIdentify   = XfsmIdentifyMessage;
  XfsmIdentifyLength = 0;

  XfsmEditSelectPoint( X1, Y1 );

  for ( Select  = XfsmHeadSelect;
        Select != (xfsmselect_list *)NULL;
        Select  = Select->NEXT )
  {
    if ( ! XfsmAddIdentify( Select->OBJECT ) ) break;
  }

  XfsmDelSelect();

  XfsmDisplayEditIdentify( XfsmIdentifyMessage );
  XfsmEnterPanel( &XfsmEditIdentifyPanel       );
}

/*------------------------------------------------------------\
|                                                             |
|                         XfsmEditConnected                   |
|                                                             |
\------------------------------------------------------------*/

void XfsmEditConnected( X1, Y1 )

  long X1;
  long Y1;
{
  xfsmselect_list  *Select;
  xfsmselect_list **PrevSelect;
  xfsmobj_list     *Obj;

  if ( XfsmHeadConnect != (xfsmselect_list *)NULL )
  {
    XfsmDelConnect();
    XfsmZoomRefresh();
  }

  XfsmEditSelectPoint( X1, Y1 );

  XfsmDisplaySelectList( (char *)NULL );

  for ( Select  = XfsmHeadSelect;
        Select != (xfsmselect_list *)NULL;
        Select  = Select->NEXT )
  {
    XfsmAddSelectList( Select->OBJECT );
  }

  if ( XfsmHeadSelect == (xfsmselect_list *)NULL  )
  {
    XfsmWarningMessage( XfsmMainWindow, "No element found !" );
  }
  else
  {
    if ( XfsmHeadSelect->NEXT != (xfsmselect_list *)NULL )
    {
      XfsmEnterPanel( &XfsmEditSelectPanel );
      XfsmLimitedLoop( XfsmEditSelectPanel.PANEL );
    }
    else
    {
      XfsmAcceptObject( XfsmHeadSelect->OBJECT );
      XfsmDisplayObject( XfsmHeadSelect->OBJECT );
    }
  }

  XfsmPurgeSelect();

  for ( Select  = XfsmHeadSelect;
        Select != (xfsmselect_list *)NULL;
        Select  = Select->NEXT )
  {
    XfsmAddConnect( Select->OBJECT );
  }

  if ( XfsmHeadSelect != (xfsmselect_list *)NULL )
  {
    if ( XfsmHeadSelect->NEXT == (xfsmselect_list *)NULL )
    {
      Obj = XfsmHeadSelect->OBJECT;
      XfsmDelSelect();
      XfsmDisplayObject( Obj );
    }
    else
    {
      XfsmDelSelect();
      XfsmZoomRefresh();
    }
  }
}
