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
| Tool    :                   XSCH                           |
|                                                             |
| File    :                   Menu.c                          |
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
# include <Xm/Xm.h>
# include <Xm/PushBG.h>
# include <Xm/CascadeBG.h>

# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "XSB.h"
# include "XTB.h"
# include "XMX.h"
# include "XMV.h"

# include "XMV_menu.h"
# include "XMV_dialog.h"
# include "XMV_view.h"
# include "XMV_map.h"
# include "XMV_panel.h"

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

  char XschFirstViewLayer = 1;

/*------------------------------------------------------------\
|                                                             |
|                            Menu                             |
|                                                             |
\------------------------------------------------------------*/

   XschMenuItem XschViewMenu[] =

         {
           {
             "Zoom",
             'Z',
             "Meta<Key>Z",
             "Meta Z",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackViewZoom,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (XschMenuItem *)NULL
           }
           ,
           {
             "Layers   ",
             'L',
             "Meta<Key>L",
             "Meta L",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackViewLayer,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (XschMenuItem *)NULL
           }
           ,
           {
             "Map",
             'M',
             "Meta<Key>P",
             "Meta P",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackViewMap,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (XschMenuItem *)NULL
           }
           ,
           {
             "Arrows    ",
             'A',
             "Meta<Key>A",
             "Meta A",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackViewArrows,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (XschMenuItem *)NULL
           }
           ,
           {
             "Grid      ",
             'G',
             "Meta<Key>G",
             "Meta G",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackViewGrid,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (XschMenuItem *)NULL
           }
           ,
           NULL
         };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Callback For Menu                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      CallbackViewLayer                      |
|                                                             |
\------------------------------------------------------------*/

void CallbackViewLayer( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  if ( XschFirstViewLayer )
  {
    XschFirstViewLayer = 0;

    XschInitializeLayer();
  }

  XschEnterPanel( &XschViewLayerPanel );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      CallbackViewGrid                       |
|                                                             |
\------------------------------------------------------------*/

void CallbackViewGrid( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  XschEnterPanel( &XschViewGridPanel );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     CallbackViewArrows                      |
|                                                             |
\------------------------------------------------------------*/

void CallbackViewArrows( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  XschEnterPanel( &XschViewArrowsPanel );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     CallbackViewZoom                        |
|                                                             |
\------------------------------------------------------------*/

void CallbackViewZoom( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  XschEnterPanel( &XschViewZoomPanel );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       CallbackViewMap                       |
|                                                             |
\------------------------------------------------------------*/

void CallbackViewMap( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  XschEnterMapPanel();

  autend();
}
