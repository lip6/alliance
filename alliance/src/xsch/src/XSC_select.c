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
| Tool    :                   XSCH                            |
|                                                             |
| File    :                 Select.c                          |
|                                                             |
| Authors :              Jacomme Ludovic                      |
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
# include <stdlib.h>
# include <signal.h>
# include <string.h>
# include <setjmp.h>
# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "XSB.h" 
# include "XSC.h"
# include "XSC_select.h"

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

  xschselect_list *XschHeadSelect  = (xschselect_list *)NULL;
  xschselect_list *XschHeadConnect = (xschselect_list *)NULL;

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

xschselect_list *XschAllocSelect()
{
  return( (xschselect_list *)autallocheap( sizeof( xschselect_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                         Free Functions                      |
|                                                             |
\------------------------------------------------------------*/

void XschFreeSelect( Select )

  xschselect_list *Select;
{
  autfreeheap( Select, sizeof( xschselect_list ) );
}

/*------------------------------------------------------------\
|                                                             |
|                          Select Functions                   |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XschSelectObject                   |
|                                                             |
\------------------------------------------------------------*/

void XschSelectObject( Object )

  xschobj_list *Object;
{
  xschobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    SetXschSelect( Scan );

    Scan = Scan->LINK;
  }
  while ( Scan != Object );
  
  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XschUnselectObject                 |
|                                                             |
\------------------------------------------------------------*/

void XschUnselectObject( Object )

  xschobj_list *Object;
{
  xschobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    ClearXschSelect( Scan );

    Scan = Scan->LINK;
  }
  while ( Scan != Object );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XschAcceptObject                   |
|                                                             |
\------------------------------------------------------------*/

void XschAcceptObject( Object )

  xschobj_list *Object;
{
  xschobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    SetXschAccept( Scan );

    Scan = Scan->LINK;
  }
  while ( Scan != Object );
  
  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XschRejectObject                   |
|                                                             |
\------------------------------------------------------------*/

void XschRejectObject( Object )

  xschobj_list *Object;
{
  xschobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    ClearXschAccept( Scan );

    Scan = Scan->LINK;
  }
  while ( Scan != Object );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XschConnectObject                  |
|                                                             |
\------------------------------------------------------------*/

void XschConnectObject( Object )

  xschobj_list *Object;
{
  xschobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    SetXschConnect( Scan );

    Scan = Scan->LINK;
  }
  while ( Scan != Object );
  
  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XschDisconnectObject                |
|                                                             |
\------------------------------------------------------------*/

void XschDisconnectObject( Object )

  xschobj_list *Object;
{
  xschobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    ClearXschConnect( Scan );

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
|                          XschAddSelect                      |
|                                                             |
\------------------------------------------------------------*/

void XschAddSelect( Object )

  xschobj_list *Object;
{
  xschselect_list *Select;

  autbegin();

  Select         = XschAllocSelect();
  Select->NEXT   = XschHeadSelect;
  Select->OBJECT = Object;
  XschHeadSelect = Select;

  XschSelectObject( Object );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XschAddConnect                     |
|                                                             |
\------------------------------------------------------------*/

void XschAddConnect( Object )

  xschobj_list *Object;
{
  xschselect_list *Connect;

  autbegin();

  Connect         = XschAllocSelect();
  Connect->NEXT   = XschHeadConnect;
  Connect->OBJECT = Object;
  XschHeadConnect = Connect;

  XschConnectObject( Object );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         Del Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XschDelSelect                      |
|                                                             |
\------------------------------------------------------------*/

void XschDelSelect()
{
  xschselect_list *Select;
  xschselect_list *DelSelect;

  autbegin();

  Select         = XschHeadSelect;
  XschHeadSelect = (xschselect_list *)NULL;

  while ( Select != (xschselect_list *)NULL )
  {
    XschRejectObject( Select->OBJECT );

    DelSelect = Select;
    Select    = Select->NEXT;

    XschFreeSelect( DelSelect );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XschPurgeSelect                      |
|                                                             |
\------------------------------------------------------------*/

void XschPurgeSelect()
{
  xschselect_list    *DelSelect;
  xschselect_list    *Select;
  xschselect_list   **Previous;

  autbegin();

  Previous = &XschHeadSelect;
  Select   =  XschHeadSelect;

  while( Select != (xschselect_list *)NULL )
  {
    if ( ! IsXschAccept( Select->OBJECT ) )
    {
      DelSelect  = Select;
      Select     = Select->NEXT;
      *Previous  = Select;

      XschFreeSelect( DelSelect );
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
|                          XschDelConnect                     |
|                                                             |
\------------------------------------------------------------*/

void XschDelConnect()
{
  xschselect_list *Connect;
  xschselect_list *DelConnect;

  autbegin();

  Connect         = XschHeadConnect;
  XschHeadConnect = (xschselect_list *)NULL;

  while ( Connect != (xschselect_list *)NULL )
  {
    XschDisconnectObject( Connect->OBJECT );

    DelConnect = Connect;
    Connect    = Connect->NEXT;

    XschFreeSelect( DelConnect );
  }

  autend();
}
