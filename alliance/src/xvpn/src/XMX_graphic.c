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
| Tool    :                    XVPN                           |
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

  Widget   XvpnGraphicFrame;
  Widget   XvpnGraphicWindow;
  Display *XvpnGraphicDisplay = (Display *)NULL;

  Dimension  XvpnGraphicDx    = 0;
  Dimension  XvpnGraphicDy    = 0;
  Dimension  XvpnOldGraphicDx = 0;
  Dimension  XvpnOldGraphicDy = 0;

  Pixmap     XvpnGraphicPixmap   = (Pixmap)NULL;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 XvpnInitializeGraphicWindow                |
|                                                             |
\------------------------------------------------------------*/

void XvpnInitializeGraphicWindow()
 
{
  autbegin();

  XtVaGetValues( XvpnGraphicWindow,
                 XmNwidth, &XvpnGraphicDx,
                 XmNheight, &XvpnGraphicDy,
                 NULL
               );

  XvpnOldGraphicDx = XvpnGraphicDx;
  XvpnOldGraphicDy = XvpnGraphicDy;

  XvpnGraphicPixmap =

     XCreatePixmap ( XvpnGraphicDisplay,
                     RootWindowOfScreen( XtScreen ( XvpnGraphicWindow ) ),
                     XvpnGraphicDx,
                     XvpnGraphicDy,
                     DefaultDepthOfScreen ( XtScreen ( XvpnGraphicWindow ) )
                   );

  XvpnClearGraphicWindow( 0, 0, XvpnGraphicDx, XvpnGraphicDy );
  XvpnInitializeUnitGrid();

  XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XvpnClearGraphicWindow                 |
|                                                             |
\------------------------------------------------------------*/

void XvpnClearGraphicWindow( GraphicX, GraphicY, GraphicDx, GraphicDy )

   long GraphicX;
   long GraphicY;
   long GraphicDx;
   long GraphicDy;
{
  XFillRectangle( XvpnGraphicDisplay,
                  XvpnGraphicPixmap,
                  XvpnBackgroundGC, 
                  GraphicX, GraphicY,
                  GraphicDx,
                  GraphicDy );
}

/*------------------------------------------------------------\
|                                                             |
|                      XvpnRefreshGraphicWindow               |
|                                                             |
\------------------------------------------------------------*/

void XvpnRefreshGraphicWindow( GraphicX, GraphicY, GraphicDx, GraphicDy )

   Dimension GraphicX;
   Dimension GraphicY;
   Dimension GraphicDx;
   Dimension GraphicDy;
{
  autbegin();

  XvpnUndisplayCursor();

  XCopyArea( XvpnGraphicDisplay,
             XvpnGraphicPixmap,
             XtWindow ( XvpnGraphicWindow ),
             XvpnBackgroundGC,
             GraphicX, GraphicY,
             GraphicDx, GraphicDy,
             GraphicX, GraphicY 
           ); 

  XvpnDisplayUnitGrid( GraphicX, GraphicY, 
                          GraphicX + GraphicDx, 
                          GraphicY + GraphicDy );
  XvpnUndisplayCursor();

  autend();
}


/*------------------------------------------------------------\
|                                                             |
|                      XvpnResizeGraphicWindow                |
|                                                             |
\------------------------------------------------------------*/

void XvpnResizeGraphicWindow()

{
  Pixmap    OldPixmap;
  Dimension SourceY;
  Dimension TargetY;
  Dimension TargetDx;
  Dimension TargetDy;

  autbegin();

  XvpnOldGraphicDx = XvpnGraphicDx;
  XvpnOldGraphicDy = XvpnGraphicDy;

  XtVaGetValues( XvpnGraphicWindow,
                 XmNwidth, &XvpnGraphicDx,
                 XmNheight, &XvpnGraphicDy,
                 NULL
               );   

  if ( ( XvpnOldGraphicDx != XvpnGraphicDx ) ||
       ( XvpnOldGraphicDy != XvpnGraphicDy ) )
  {
    OldPixmap = XvpnGraphicPixmap;

    XvpnGraphicPixmap = 

      XCreatePixmap ( XvpnGraphicDisplay,
                      RootWindowOfScreen( XtScreen ( XvpnGraphicWindow ) ),
                      XvpnGraphicDx,
                      XvpnGraphicDy,
                      DefaultDepthOfScreen ( XtScreen ( XvpnGraphicWindow ) )
                    );

    XvpnClearGraphicWindow( 0, 0, XvpnGraphicDx, XvpnGraphicDy );

    XvpnResizeUnitGrid();

    if ( XvpnOldGraphicDx < XvpnGraphicDx )
    {
      TargetDx = XvpnOldGraphicDx;

      XvpnDisplayFigure( TargetDx, 0, 
                          XvpnGraphicDx, XvpnGraphicDy );
    }
    else
    {
      TargetDx = XvpnGraphicDx;
    }
    
    if ( XvpnOldGraphicDy < XvpnGraphicDy )
    {
      SourceY  = 0;
      TargetDy = XvpnOldGraphicDy;
      TargetY  = XvpnGraphicDy - XvpnOldGraphicDy;

      XvpnDisplayFigure( 0, TargetDy, 
                          TargetDx, XvpnGraphicDy ); 
    }
    else
    {
      TargetDy = XvpnGraphicDy;
      TargetY  = 0;
      SourceY  = XvpnOldGraphicDy - XvpnGraphicDy;
    }

    XCopyArea( XvpnGraphicDisplay,
               OldPixmap,
               XvpnGraphicPixmap,
               XvpnBackgroundGC,
               0, SourceY,
               TargetDx, TargetDy, 
               0, TargetY
             );

    XFreePixmap( XvpnGraphicDisplay, OldPixmap );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XvpnInitializeGraphic                   |
|                                                             |
\------------------------------------------------------------*/

void XvpnInitializeGraphic()

{
  autbegin();

  XvpnGraphicFrame = XtVaCreateManagedWidget( "XvpnGraphicFrame",
                                               xmFrameWidgetClass,
                                               XvpnMainForm,
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

  XvpnGraphicWindow = XtVaCreateManagedWidget(
                                                "XvpnGraphicWindow",
                                                xmDrawingAreaWidgetClass,
                                                XvpnGraphicFrame,
                                                XmNtopAttachment,    XmATTACH_FORM,
                                                XmNrightAttachment,  XmATTACH_FORM,
                                                XmNbottomAttachment, XmATTACH_FORM,
                                                XmNleftAttachment,   XmATTACH_FORM,
                                                XmNbackground,       1,
                                                XmNforeground,       0,
                                                NULL
                                             );

  XvpnGraphicDisplay = XtDisplay( XvpnGraphicWindow );

  autend();
}
