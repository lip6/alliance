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
| Tool    :                   DREAL                           |
|                                                             |
| File    :                   Menu.c                          |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                  28.03.95                         |
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
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "GTB.h"
# include "GSB.h"
# include "GMX.h"
# include "GMV.h"

# include "GMV_menu.h"
# include "GMV_dialog.h"
# include "GMV_view.h"
# include "GMV_map.h"
# include "GMV_panel.h"

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

  char DrealFirstViewLayer = 1;

/*------------------------------------------------------------\
|                                                             |
|                            Menu                             |
|                                                             |
\------------------------------------------------------------*/

   DrealMenuItem DrealViewMenu[] =

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
             (DrealMenuItem *)NULL
           }
           ,
           {
             "Layers",
             'L',
             "Meta<Key>L",
             "Meta L",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackViewLayers,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (DrealMenuItem *)NULL
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
             (DrealMenuItem *)NULL
           }
           ,
           {
             "Arrows",
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
             (DrealMenuItem *)NULL
           }
           ,
           {
             "Grid     ",
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
             (DrealMenuItem *)NULL
           }
           ,
           {
             NULL
           }
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
|                      CallbackViewLayers                     |
|                                                             |
\------------------------------------------------------------*/

void CallbackViewLayers( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  if ( DrealFirstViewLayer )
  {
    DrealInitializeLayer();

    DrealFirstViewLayer = 0;
  }

  DrealEnterPanel( &DrealViewLayerPanel );

  rdsend();
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
  rdsbegin();

  DrealEnterPanel( &DrealViewGridPanel );

  rdsend();
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
  rdsbegin();

  DrealEnterPanel( &DrealViewArrowsPanel );

  rdsend();
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
  rdsbegin();

  DrealEnterPanel( &DrealViewZoomPanel );

  rdsend();
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
  rdsbegin();

  DrealEnterMapPanel();

  rdsend();
}
