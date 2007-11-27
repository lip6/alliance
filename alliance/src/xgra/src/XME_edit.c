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
| Tool    :                   XGRA                            |
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
# include "XSB.h"
# include "XTB.h"
# include "XMX.h"
# include "XGR.h"
# include "XMV.h"
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

  static char  XgraIdentifyMessage[ XGRA_IDENTIFY_MESSAGE_SIZE ];
  static char  XgraIdentifyBuffer [ XGRA_IDENTIFY_BUFFER_SIZE  ];
  static char  XgraIdentifyLocout [ XGRA_IDENTIFY_LOCOUT_SIZE  ];

  static char *XgraScanIdentify;
  static long  XgraIdentifyLength;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         XgraAddIdentify                     |
|                                                             |
\------------------------------------------------------------*/

short XgraAddIdentify( Obj )

  xgraobj_list *Obj;
{
  autnode_list  *Node;
  autarc_list   *Arc;
  char          *ScanString;
  long           Length;

  autbegin();

  XgraIdentifyBuffer[0] = '\0';

  if ( IsXgraNode( Obj ) )
  {
    Node = (autnode_list *)( Obj->USER );

    sprintf( XgraIdentifyBuffer, "NODE :\n\n  NAME : %s\n  FLAGS : %ld\n\n",
             Node->NAME, Node->FLAGS );
  }
  else
  if ( IsXgraArc( Obj ) )
  {
    Arc = (autarc_list *)( Obj->USER );

    sprintf( XgraIdentifyBuffer,
             "ARC :\n\n  FROM : %s TO : %s\n  FLAGS : %ld\n\n",
             Arc->NODE_FROM->NAME, Arc->NODE_TO->NAME, Arc->FLAGS );
  }

  Length = strlen( XgraIdentifyBuffer );

  if ( ( XgraIdentifyLength + Length ) < ( XGRA_IDENTIFY_MESSAGE_SIZE - 1 ) )
  {
    strcpy( XgraScanIdentify, XgraIdentifyBuffer );

    XgraScanIdentify   += Length; 
    XgraIdentifyLength += Length;

    autend();
    return( XGRA_TRUE );
  }

  strcpy( XgraScanIdentify, "#" );
 
  autend();
  return( XGRA_FALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                         XgraEditIdentify                    |
|                                                             |
\------------------------------------------------------------*/

void XgraEditIdentify( X1, Y1 )

  long X1;
  long Y1;
{
  xgraselect_list *Select;
  xgraobj_list    *Obj;

  strcpy( XgraIdentifyMessage, "No element found !" );

  XgraScanIdentify   = XgraIdentifyMessage;
  XgraIdentifyLength = 0;

  XgraEditSelectPoint( X1, Y1 );

  for ( Select  = XgraHeadSelect;
        Select != (xgraselect_list *)NULL;
        Select  = Select->NEXT )
  {
    if ( ! XgraAddIdentify( Select->OBJECT ) ) break;
  }

  XgraDelSelect();

  XgraDisplayEditIdentify( XgraIdentifyMessage );
  XgraEnterPanel( &XgraEditIdentifyPanel       );
}

/*------------------------------------------------------------\
|                                                             |
|                         XgraEditConnected                   |
|                                                             |
\------------------------------------------------------------*/

void XgraEditConnected( X1, Y1 )

  long X1;
  long Y1;
{
  xgraselect_list  *Select;
  xgraselect_list **PrevSelect;
  xgraobj_list     *Obj;

  if ( XgraHeadConnect != (xgraselect_list *)NULL )
  {
    XgraDelConnect();
    XgraZoomRefresh();
  }

  XgraEditSelectPoint( X1, Y1 );

  XgraDisplaySelectList( (char *)NULL );

  for ( Select  = XgraHeadSelect;
        Select != (xgraselect_list *)NULL;
        Select  = Select->NEXT )
  {
    XgraAddSelectList( Select->OBJECT );
  }

  if ( XgraHeadSelect == (xgraselect_list *)NULL  )
  {
    XgraWarningMessage( XgraMainWindow, "No element found !" );
  }
  else
  {
    if ( XgraHeadSelect->NEXT != (xgraselect_list *)NULL )
    {
      XgraEnterPanel( &XgraEditSelectPanel );
      XgraLimitedLoop( XgraEditSelectPanel.PANEL );
    }
    else
    {
      XgraAcceptObject( XgraHeadSelect->OBJECT );
      XgraDisplayObject( XgraHeadSelect->OBJECT );
    }
  }

  XgraPurgeSelect();

  for ( Select  = XgraHeadSelect;
        Select != (xgraselect_list *)NULL;
        Select  = Select->NEXT )
  {
    XgraAddConnect( Select->OBJECT );
  }

  if ( XgraHeadSelect != (xgraselect_list *)NULL )
  {
    if ( XgraHeadSelect->NEXT == (xgraselect_list *)NULL )
    {
      Obj = XgraHeadSelect->OBJECT;
      XgraDelSelect();
      XgraDisplayObject( Obj );
    }
    else
    {
      XgraDelSelect();
      XgraZoomRefresh();
    }
  }
}
