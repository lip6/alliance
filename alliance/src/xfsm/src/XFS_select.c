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
| Tool    :                   XFSM                            |
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
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "XSB.h" 
# include "XFS.h"
# include "XFS_select.h"

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

  xfsmselect_list *XfsmHeadSelect  = (xfsmselect_list *)NULL;
  xfsmselect_list *XfsmHeadConnect = (xfsmselect_list *)NULL;

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

xfsmselect_list *XfsmAllocSelect()
{
  return( (xfsmselect_list *)autallocheap( sizeof( xfsmselect_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                         Free Functions                      |
|                                                             |
\------------------------------------------------------------*/

void XfsmFreeSelect( Select )

  xfsmselect_list *Select;
{
  autfreeheap( Select, sizeof( xfsmselect_list ) );
}

/*------------------------------------------------------------\
|                                                             |
|                          Select Functions                   |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XfsmSelectObject                   |
|                                                             |
\------------------------------------------------------------*/

void XfsmSelectObject( Object )

  xfsmobj_list *Object;
{
  xfsmobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    SetXfsmSelect( Scan );

    Scan = Scan->LINK;
  }
  while ( Scan != Object );
  
  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XfsmUnselectObject                 |
|                                                             |
\------------------------------------------------------------*/

void XfsmUnselectObject( Object )

  xfsmobj_list *Object;
{
  xfsmobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    ClearXfsmSelect( Scan );

    Scan = Scan->LINK;
  }
  while ( Scan != Object );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XfsmAcceptObject                   |
|                                                             |
\------------------------------------------------------------*/

void XfsmAcceptObject( Object )

  xfsmobj_list *Object;
{
  xfsmobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    SetXfsmAccept( Scan );

    Scan = Scan->LINK;
  }
  while ( Scan != Object );
  
  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XfsmRejectObject                   |
|                                                             |
\------------------------------------------------------------*/

void XfsmRejectObject( Object )

  xfsmobj_list *Object;
{
  xfsmobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    ClearXfsmAccept( Scan );

    Scan = Scan->LINK;
  }
  while ( Scan != Object );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XfsmConnectObject                  |
|                                                             |
\------------------------------------------------------------*/

void XfsmConnectObject( Object )

  xfsmobj_list *Object;
{
  xfsmobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    SetXfsmConnect( Scan );

    Scan = Scan->LINK;
  }
  while ( Scan != Object );
  
  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XfsmDisconnectObject                |
|                                                             |
\------------------------------------------------------------*/

void XfsmDisconnectObject( Object )

  xfsmobj_list *Object;
{
  xfsmobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    ClearXfsmConnect( Scan );

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
|                          XfsmAddSelect                      |
|                                                             |
\------------------------------------------------------------*/

void XfsmAddSelect( Object )

  xfsmobj_list *Object;
{
  xfsmselect_list *Select;

  autbegin();

  Select         = XfsmAllocSelect();
  Select->NEXT   = XfsmHeadSelect;
  Select->OBJECT = Object;
  XfsmHeadSelect = Select;

  XfsmSelectObject( Object );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XfsmAddConnect                     |
|                                                             |
\------------------------------------------------------------*/

void XfsmAddConnect( Object )

  xfsmobj_list *Object;
{
  xfsmselect_list *Connect;

  autbegin();

  Connect         = XfsmAllocSelect();
  Connect->NEXT   = XfsmHeadConnect;
  Connect->OBJECT = Object;
  XfsmHeadConnect = Connect;

  XfsmConnectObject( Object );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         Del Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XfsmDelSelect                      |
|                                                             |
\------------------------------------------------------------*/

void XfsmDelSelect()
{
  xfsmselect_list *Select;
  xfsmselect_list *DelSelect;

  autbegin();

  Select         = XfsmHeadSelect;
  XfsmHeadSelect = (xfsmselect_list *)NULL;

  while ( Select != (xfsmselect_list *)NULL )
  {
    XfsmRejectObject( Select->OBJECT );

    DelSelect = Select;
    Select    = Select->NEXT;

    XfsmFreeSelect( DelSelect );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XfsmPurgeSelect                      |
|                                                             |
\------------------------------------------------------------*/

void XfsmPurgeSelect()
{
  xfsmselect_list    *DelSelect;
  xfsmselect_list    *Select;
  xfsmselect_list   **Previous;

  autbegin();

  Previous = &XfsmHeadSelect;
  Select   =  XfsmHeadSelect;

  while( Select != (xfsmselect_list *)NULL )
  {
    if ( ! IsXfsmAccept( Select->OBJECT ) )
    {
      DelSelect  = Select;
      Select     = Select->NEXT;
      *Previous  = Select;

      XfsmFreeSelect( DelSelect );
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
|                          XfsmDelConnect                     |
|                                                             |
\------------------------------------------------------------*/

void XfsmDelConnect()
{
  xfsmselect_list *Connect;
  xfsmselect_list *DelConnect;

  autbegin();

  Connect         = XfsmHeadConnect;
  XfsmHeadConnect = (xfsmselect_list *)NULL;

  while ( Connect != (xfsmselect_list *)NULL )
  {
    XfsmDisconnectObject( Connect->OBJECT );

    DelConnect = Connect;
    Connect    = Connect->NEXT;

    XfsmFreeSelect( DelConnect );
  }

  autend();
}
