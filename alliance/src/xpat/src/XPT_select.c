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
| Tool    :                   XPAT                            |
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
# include "pat.h"
# include "XSB.h" 
# include "XPT.h"
# include "XPT_select.h"

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

  xpatselect_list *XpatHeadSelect  = (xpatselect_list *)NULL;
  xpatselect_list *XpatHeadConnect = (xpatselect_list *)NULL;

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

xpatselect_list *XpatAllocSelect()
{
  return( (xpatselect_list *)autallocheap( sizeof( xpatselect_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                         Free Functions                      |
|                                                             |
\------------------------------------------------------------*/

void XpatFreeSelect( Select )

  xpatselect_list *Select;
{
  autfreeheap( Select, sizeof( xpatselect_list ) );
}

/*------------------------------------------------------------\
|                                                             |
|                          Select Functions                   |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XpatSelectObject                   |
|                                                             |
\------------------------------------------------------------*/

void XpatSelectObject( Object )

  xpatobj_list *Object;
{
  xpatobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    SetXpatSelect( Scan );

    Scan = Scan->LINK;
  }
  while ( Scan != Object );
  
  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XpatUnselectObject                 |
|                                                             |
\------------------------------------------------------------*/

void XpatUnselectObject( Object )

  xpatobj_list *Object;
{
  xpatobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    ClearXpatSelect( Scan );

    Scan = Scan->LINK;
  }
  while ( Scan != Object );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XpatAcceptObject                   |
|                                                             |
\------------------------------------------------------------*/

void XpatAcceptObject( Object )

  xpatobj_list *Object;
{
  xpatobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    SetXpatAccept( Scan );

    Scan = Scan->LINK;
  }
  while ( Scan != Object );
  
  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XpatRejectObject                   |
|                                                             |
\------------------------------------------------------------*/

void XpatRejectObject( Object )

  xpatobj_list *Object;
{
  xpatobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    ClearXpatAccept( Scan );

    Scan = Scan->LINK;
  }
  while ( Scan != Object );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XpatConnectObject                  |
|                                                             |
\------------------------------------------------------------*/

void XpatConnectObject( Object )

  xpatobj_list *Object;
{
  xpatobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    SetXpatConnect( Scan );

    Scan = Scan->LINK;
  }
  while ( Scan != Object );
  
  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XpatDisconnectObject                |
|                                                             |
\------------------------------------------------------------*/

void XpatDisconnectObject( Object )

  xpatobj_list *Object;
{
  xpatobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    ClearXpatConnect( Scan );

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
|                          XpatAddSelect                      |
|                                                             |
\------------------------------------------------------------*/

void XpatAddSelect( Object )

  xpatobj_list *Object;
{
  xpatselect_list *Select;

  autbegin();

  Select         = XpatAllocSelect();
  Select->NEXT   = XpatHeadSelect;
  Select->OBJECT = Object;
  XpatHeadSelect = Select;

  XpatSelectObject( Object );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XpatAddConnect                     |
|                                                             |
\------------------------------------------------------------*/

void XpatAddConnect( Object )

  xpatobj_list *Object;
{
  xpatselect_list *Connect;

  autbegin();

  Connect         = XpatAllocSelect();
  Connect->NEXT   = XpatHeadConnect;
  Connect->OBJECT = Object;
  XpatHeadConnect = Connect;

  XpatConnectObject( Object );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         Del Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XpatDelSelect                      |
|                                                             |
\------------------------------------------------------------*/

void XpatDelSelect()
{
  xpatselect_list *Select;
  xpatselect_list *DelSelect;

  autbegin();

  Select         = XpatHeadSelect;
  XpatHeadSelect = (xpatselect_list *)NULL;

  while ( Select != (xpatselect_list *)NULL )
  {
    XpatRejectObject( Select->OBJECT );

    DelSelect = Select;
    Select    = Select->NEXT;

    XpatFreeSelect( DelSelect );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XpatPurgeSelect                      |
|                                                             |
\------------------------------------------------------------*/

void XpatPurgeSelect()
{
  xpatselect_list    *DelSelect;
  xpatselect_list    *Select;
  xpatselect_list   **Previous;

  autbegin();

  Previous = &XpatHeadSelect;
  Select   =  XpatHeadSelect;

  while( Select != (xpatselect_list *)NULL )
  {
    if ( ! IsXpatAccept( Select->OBJECT ) )
    {
      DelSelect  = Select;
      Select     = Select->NEXT;
      *Previous  = Select;

      XpatFreeSelect( DelSelect );
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
|                          XpatDelConnect                     |
|                                                             |
\------------------------------------------------------------*/

void XpatDelConnect()
{
  xpatselect_list *Connect;
  xpatselect_list *DelConnect;

  autbegin();

  Connect         = XpatHeadConnect;
  XpatHeadConnect = (xpatselect_list *)NULL;

  while ( Connect != (xpatselect_list *)NULL )
  {
    XpatDisconnectObject( Connect->OBJECT );

    DelConnect = Connect;
    Connect    = Connect->NEXT;

    XpatFreeSelect( DelConnect );
  }

  autend();
}
