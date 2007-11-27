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
# include "XSB.h"
# include "XGR.h"
# include "XGR_select.h"

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

  xgraselect_list *XgraHeadSelect  = (xgraselect_list *)NULL;
  xgraselect_list *XgraHeadConnect = (xgraselect_list *)NULL;

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

xgraselect_list *XgraAllocSelect()
{
  return( (xgraselect_list *)autallocheap( sizeof( xgraselect_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                         Free Functions                      |
|                                                             |
\------------------------------------------------------------*/

void XgraFreeSelect( Select )

  xgraselect_list *Select;
{
  autfreeheap( Select, sizeof( xgraselect_list ) );
}

/*------------------------------------------------------------\
|                                                             |
|                          Select Functions                   |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XgraSelectObject                   |
|                                                             |
\------------------------------------------------------------*/

void XgraSelectObject( Object )

  xgraobj_list *Object;
{
  xgraobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    SetXgraSelect( Scan );

    Scan = Scan->LINK;
  }
  while ( Scan != Object );
  
  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XgraUnselectObject                 |
|                                                             |
\------------------------------------------------------------*/

void XgraUnselectObject( Object )

  xgraobj_list *Object;
{
  xgraobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    ClearXgraSelect( Scan );

    Scan = Scan->LINK;
  }
  while ( Scan != Object );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XgraAcceptObject                   |
|                                                             |
\------------------------------------------------------------*/

void XgraAcceptObject( Object )

  xgraobj_list *Object;
{
  xgraobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    SetXgraAccept( Scan );

    Scan = Scan->LINK;
  }
  while ( Scan != Object );
  
  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XgraRejectObject                   |
|                                                             |
\------------------------------------------------------------*/

void XgraRejectObject( Object )

  xgraobj_list *Object;
{
  xgraobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    ClearXgraAccept( Scan );

    Scan = Scan->LINK;
  }
  while ( Scan != Object );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XgraConnectObject                  |
|                                                             |
\------------------------------------------------------------*/

void XgraConnectObject( Object )

  xgraobj_list *Object;
{
  xgraobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    SetXgraConnect( Scan );

    Scan = Scan->LINK;
  }
  while ( Scan != Object );
  
  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XgraDisconnectObject                |
|                                                             |
\------------------------------------------------------------*/

void XgraDisconnectObject( Object )

  xgraobj_list *Object;
{
  xgraobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    ClearXgraConnect( Scan );

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
|                          XgraAddSelect                      |
|                                                             |
\------------------------------------------------------------*/

void XgraAddSelect( Object )

  xgraobj_list *Object;
{
  xgraselect_list *Select;

  autbegin();

  Select         = XgraAllocSelect();
  Select->NEXT   = XgraHeadSelect;
  Select->OBJECT = Object;
  XgraHeadSelect = Select;

  XgraSelectObject( Object );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XgraAddConnect                     |
|                                                             |
\------------------------------------------------------------*/

void XgraAddConnect( Object )

  xgraobj_list *Object;
{
  xgraselect_list *Connect;

  autbegin();

  Connect         = XgraAllocSelect();
  Connect->NEXT   = XgraHeadConnect;
  Connect->OBJECT = Object;
  XgraHeadConnect = Connect;

  XgraConnectObject( Object );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         Del Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XgraDelSelect                      |
|                                                             |
\------------------------------------------------------------*/

void XgraDelSelect()
{
  xgraselect_list *Select;
  xgraselect_list *DelSelect;

  autbegin();

  Select         = XgraHeadSelect;
  XgraHeadSelect = (xgraselect_list *)NULL;

  while ( Select != (xgraselect_list *)NULL )
  {
    XgraRejectObject( Select->OBJECT );

    DelSelect = Select;
    Select    = Select->NEXT;

    XgraFreeSelect( DelSelect );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XgraPurgeSelect                      |
|                                                             |
\------------------------------------------------------------*/

void XgraPurgeSelect()
{
  xgraselect_list    *DelSelect;
  xgraselect_list    *Select;
  xgraselect_list   **Previous;

  autbegin();

  Previous = &XgraHeadSelect;
  Select   =  XgraHeadSelect;

  while( Select != (xgraselect_list *)NULL )
  {
    if ( ! IsXgraAccept( Select->OBJECT ) )
    {
      DelSelect  = Select;
      Select     = Select->NEXT;
      *Previous  = Select;

      XgraFreeSelect( DelSelect );
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
|                          XgraDelConnect                     |
|                                                             |
\------------------------------------------------------------*/

void XgraDelConnect()
{
  xgraselect_list *Connect;
  xgraselect_list *DelConnect;

  autbegin();

  Connect         = XgraHeadConnect;
  XgraHeadConnect = (xgraselect_list *)NULL;

  while ( Connect != (xgraselect_list *)NULL )
  {
    XgraDisconnectObject( Connect->OBJECT );

    DelConnect = Connect;
    Connect    = Connect->NEXT;

    XgraFreeSelect( DelConnect );
  }

  autend();
}
