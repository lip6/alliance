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
| Tool    :                   XVPN                            |
|                                                             |
| File    :                 Select.c                          |
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
# include <stdlib.h>
# include <signal.h>
# include <string.h>
# include <setjmp.h>
# include "mut.h"
# include "aut.h"
# include "vex.h"
# include "vpn.h"
# include "XSB.h" 
# include "XVP.h"
# include "XVP_select.h"

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

  xvpnselect_list *XvpnHeadSelect  = (xvpnselect_list *)NULL;
  xvpnselect_list *XvpnHeadConnect = (xvpnselect_list *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Alloc Functions                     |
|                                                             |
\------------------------------------------------------------*/

xvpnselect_list *XvpnAllocSelect()
{
  return( (xvpnselect_list *)autallocheap( sizeof( xvpnselect_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                         Free Functions                      |
|                                                             |
\------------------------------------------------------------*/

void XvpnFreeSelect( Select )

  xvpnselect_list *Select;
{
  autfreeheap( Select, sizeof( xvpnselect_list ) );
}

/*------------------------------------------------------------\
|                                                             |
|                          Select Functions                   |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XvpnSelectObject                   |
|                                                             |
\------------------------------------------------------------*/

void XvpnSelectObject( Object )

  xvpnobj_list *Object;
{
  xvpnobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    SetXvpnSelect( Scan );

    Scan = Scan->LINK;
  }
  while ( Scan != Object );
  
  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XvpnUnselectObject                 |
|                                                             |
\------------------------------------------------------------*/

void XvpnUnselectObject( Object )

  xvpnobj_list *Object;
{
  xvpnobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    ClearXvpnSelect( Scan );

    Scan = Scan->LINK;
  }
  while ( Scan != Object );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XvpnAcceptObject                   |
|                                                             |
\------------------------------------------------------------*/

void XvpnAcceptObject( Object )

  xvpnobj_list *Object;
{
  xvpnobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    SetXvpnAccept( Scan );

    Scan = Scan->LINK;
  }
  while ( Scan != Object );
  
  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XvpnRejectObject                   |
|                                                             |
\------------------------------------------------------------*/

void XvpnRejectObject( Object )

  xvpnobj_list *Object;
{
  xvpnobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    ClearXvpnAccept( Scan );

    Scan = Scan->LINK;
  }
  while ( Scan != Object );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XvpnConnectObject                  |
|                                                             |
\------------------------------------------------------------*/

void XvpnConnectObject( Object )

  xvpnobj_list *Object;
{
  xvpnobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    SetXvpnConnect( Scan );

    Scan = Scan->LINK;
  }
  while ( Scan != Object );
  
  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XvpnDisconnectObject                |
|                                                             |
\------------------------------------------------------------*/

void XvpnDisconnectObject( Object )

  xvpnobj_list *Object;
{
  xvpnobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    ClearXvpnConnect( Scan );

    Scan = Scan->LINK;
  }
  while ( Scan != Object );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         Add Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XvpnAddSelect                      |
|                                                             |
\------------------------------------------------------------*/

void XvpnAddSelect( Object )

  xvpnobj_list *Object;
{
  xvpnselect_list *Select;

  autbegin();

  Select         = XvpnAllocSelect();
  Select->NEXT   = XvpnHeadSelect;
  Select->OBJECT = Object;
  XvpnHeadSelect = Select;

  XvpnSelectObject( Object );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XvpnAddConnect                     |
|                                                             |
\------------------------------------------------------------*/

void XvpnAddConnect( Object )

  xvpnobj_list *Object;
{
  xvpnselect_list *Connect;

  autbegin();

  Connect         = XvpnAllocSelect();
  Connect->NEXT   = XvpnHeadConnect;
  Connect->OBJECT = Object;
  XvpnHeadConnect = Connect;

  XvpnConnectObject( Object );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         Del Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XvpnDelSelect                      |
|                                                             |
\------------------------------------------------------------*/

void XvpnDelSelect()
{
  xvpnselect_list *Select;
  xvpnselect_list *DelSelect;

  autbegin();

  Select         = XvpnHeadSelect;
  XvpnHeadSelect = (xvpnselect_list *)NULL;

  while ( Select != (xvpnselect_list *)NULL )
  {
    XvpnRejectObject( Select->OBJECT );

    DelSelect = Select;
    Select    = Select->NEXT;

    XvpnFreeSelect( DelSelect );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XvpnPurgeSelect                      |
|                                                             |
\------------------------------------------------------------*/

void XvpnPurgeSelect()
{
  xvpnselect_list    *DelSelect;
  xvpnselect_list    *Select;
  xvpnselect_list   **Previous;

  autbegin();

  Previous = &XvpnHeadSelect;
  Select   =  XvpnHeadSelect;

  while( Select != (xvpnselect_list *)NULL )
  {
    if ( ! IsXvpnAccept( Select->OBJECT ) )
    {
      DelSelect  = Select;
      Select     = Select->NEXT;
      *Previous  = Select;

      XvpnFreeSelect( DelSelect );
    }
    else
    {
      Previous = &Select->NEXT;
      Select   = Select->NEXT;
    }
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XvpnDelConnect                     |
|                                                             |
\------------------------------------------------------------*/

void XvpnDelConnect()
{
  xvpnselect_list *Connect;
  xvpnselect_list *DelConnect;

  autbegin();

  Connect         = XvpnHeadConnect;
  XvpnHeadConnect = (xvpnselect_list *)NULL;

  while ( Connect != (xvpnselect_list *)NULL )
  {
    XvpnDisconnectObject( Connect->OBJECT );

    DelConnect = Connect;
    Connect    = Connect->NEXT;

    XvpnFreeSelect( DelConnect );
  }

  autend();
}
