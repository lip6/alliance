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
| Tool    :                    XFSM                           |
|                                                             |
| File    :                 Graphic.c                         |
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
# include <Xm/Xm.h>
# include <Xm/Form.h>
# include <Xm/Frame.h>
# include <Xm/DrawingA.h>
 
# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "XSB.h"
# include "XTB.h"
# include "XMX.h"

# include "XMX_graphic.h"
# include "XMX_grid.h"

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

  Widget   XfsmGraphicFrame;
  Widget   XfsmGraphicWindow;
  Display *XfsmGraphicDisplay = (Display *)NULL;

  Dimension  XfsmGraphicDx    = 0;
  Dimension  XfsmGraphicDy    = 0;
  Dimension  XfsmOldGraphicDx = 0;
  Dimension  XfsmOldGraphicDy = 0;

  Pixmap     XfsmGraphicPixmap   = (Pixmap)NULL;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 XfsmInitializeGraphicWindow                |
|                                                             |
\------------------------------------------------------------*/

void XfsmInitializeGraphicWindow()
 
{
  autbegin();

  XtVaGetValues( XfsmGraphicWindow,
                 XmNwidth, &XfsmGraphicDx,
                 XmNheight, &XfsmGraphicDy,
                 NULL
               );

  XfsmOldGraphicDx = XfsmGraphicDx;
  XfsmOldGraphicDy = XfsmGraphicDy;

  XfsmGraphicPixmap =

     XCreatePixmap ( XfsmGraphicDisplay,
                     RootWindowOfScreen( XtScreen ( XfsmGraphicWindow ) ),
                     XfsmGraphicDx,
                     XfsmGraphicDy,
                     DefaultDepthOfScreen ( XtScreen ( XfsmGraphicWindow ) )
                   );

  XfsmClearGraphicWindow( 0, 0, XfsmGraphicDx, XfsmGraphicDy );
  XfsmInitializeUnitGrid();

  XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XfsmClearGraphicWindow                 |
|                                                             |
\------------------------------------------------------------*/

void XfsmClearGraphicWindow( GraphicX, GraphicY, GraphicDx, GraphicDy )

   long GraphicX;
   long GraphicY;
   long GraphicDx;
   long GraphicDy;
{
  XFillRectangle( XfsmGraphicDisplay,
                  XfsmGraphicPixmap,
                  XfsmBackgroundGC, 
                  GraphicX, GraphicY,
                  GraphicDx,
                  GraphicDy );
}

/*------------------------------------------------------------\
|                                                             |
|                      XfsmRefreshGraphicWindow               |
|                                                             |
\------------------------------------------------------------*/

void XfsmRefreshGraphicWindow( GraphicX, GraphicY, GraphicDx, GraphicDy )

   Dimension GraphicX;
   Dimension GraphicY;
   Dimension GraphicDx;
   Dimension GraphicDy;
{
  autbegin();

  XfsmUndisplayCursor();

  XCopyArea( XfsmGraphicDisplay,
             XfsmGraphicPixmap,
             XtWindow ( XfsmGraphicWindow ),
             XfsmBackgroundGC,
             GraphicX, GraphicY,
             GraphicDx, GraphicDy,
             GraphicX, GraphicY 
           ); 

  XfsmDisplayUnitGrid( GraphicX, GraphicY, 
                          GraphicX + GraphicDx, 
                          GraphicY + GraphicDy );
  XfsmUndisplayCursor();

  autend();
}


/*------------------------------------------------------------\
|                                                             |
|                      XfsmResizeGraphicWindow                |
|                                                             |
\------------------------------------------------------------*/

void XfsmResizeGraphicWindow()

{
  Pixmap    OldPixmap;
  Dimension SourceY;
  Dimension TargetY;
  Dimension TargetDx;
  Dimension TargetDy;

  autbegin();

  XfsmOldGraphicDx = XfsmGraphicDx;
  XfsmOldGraphicDy = XfsmGraphicDy;

  XtVaGetValues( XfsmGraphicWindow,
                 XmNwidth, &XfsmGraphicDx,
                 XmNheight, &XfsmGraphicDy,
                 NULL
               );   

  if ( ( XfsmOldGraphicDx != XfsmGraphicDx ) ||
       ( XfsmOldGraphicDy != XfsmGraphicDy ) )
  {
    OldPixmap = XfsmGraphicPixmap;

    XfsmGraphicPixmap = 

      XCreatePixmap ( XfsmGraphicDisplay,
                      RootWindowOfScreen( XtScreen ( XfsmGraphicWindow ) ),
                      XfsmGraphicDx,
                      XfsmGraphicDy,
                      DefaultDepthOfScreen ( XtScreen ( XfsmGraphicWindow ) )
                    );

    XfsmClearGraphicWindow( 0, 0, XfsmGraphicDx, XfsmGraphicDy );

    XfsmResizeUnitGrid();

    if ( XfsmOldGraphicDx < XfsmGraphicDx )
    {
      TargetDx = XfsmOldGraphicDx;

      XfsmDisplayFigure( TargetDx, 0, 
                          XfsmGraphicDx, XfsmGraphicDy );
    }
    else
    {
      TargetDx = XfsmGraphicDx;
    }
    
    if ( XfsmOldGraphicDy < XfsmGraphicDy )
    {
      SourceY  = 0;
      TargetDy = XfsmOldGraphicDy;
      TargetY  = XfsmGraphicDy - XfsmOldGraphicDy;

      XfsmDisplayFigure( 0, TargetDy, 
                          TargetDx, XfsmGraphicDy ); 
    }
    else
    {
      TargetDy = XfsmGraphicDy;
      TargetY  = 0;
      SourceY  = XfsmOldGraphicDy - XfsmGraphicDy;
    }

    XCopyArea( XfsmGraphicDisplay,
               OldPixmap,
               XfsmGraphicPixmap,
               XfsmBackgroundGC,
               0, SourceY,
               TargetDx, TargetDy, 
               0, TargetY
             );

    XFreePixmap( XfsmGraphicDisplay, OldPixmap );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XfsmInitializeGraphic                   |
|                                                             |
\------------------------------------------------------------*/

void XfsmInitializeGraphic()

{
  autbegin();

  XfsmGraphicFrame = XtVaCreateManagedWidget( "XfsmGraphicFrame",
                                               xmFrameWidgetClass,
                                               XfsmMainForm,
                                               XmNshadowType       , XmSHADOW_ETCHED_IN,
                                               XmNtopAttachment    , XmATTACH_POSITION,
                                               XmNtopPosition      , 1,
                                               XmNbottomAttachment , XmATTACH_POSITION,
                                               XmNbottomPosition   , 191,
                                               XmNleftAttachment   , XmATTACH_POSITION,
                                               XmNleftPosition     , 1,
                                               XmNrightAttachment  , XmATTACH_POSITION,
                                               XmNrightPosition    , 199,
                                               NULL );

  XfsmGraphicWindow = XtVaCreateManagedWidget(
                                                "XfsmGraphicWindow",
                                                xmDrawingAreaWidgetClass,
                                                XfsmGraphicFrame,
                                                XmNtopAttachment,    XmATTACH_FORM,
                                                XmNrightAttachment,  XmATTACH_FORM,
                                                XmNbottomAttachment, XmATTACH_FORM,
                                                XmNleftAttachment,   XmATTACH_FORM,
                                                XmNbackground,       1,
                                                XmNforeground,       0,
                                                NULL
                                             );

  XfsmGraphicDisplay = XtDisplay( XfsmGraphicWindow );

  autend();
}
