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
| Tool    :                    XGRA                           |
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

  Widget   XgraGraphicFrame;
  Widget   XgraGraphicWindow;
  Display *XgraGraphicDisplay = (Display *)NULL;

  Dimension  XgraGraphicDx    = 0;
  Dimension  XgraGraphicDy    = 0;
  Dimension  XgraOldGraphicDx = 0;
  Dimension  XgraOldGraphicDy = 0;

  Pixmap     XgraGraphicPixmap   = (Pixmap)NULL;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 XgraInitializeGraphicWindow                |
|                                                             |
\------------------------------------------------------------*/

void XgraInitializeGraphicWindow()
 
{
  autbegin();

  XtVaGetValues( XgraGraphicWindow,
                 XmNwidth, &XgraGraphicDx,
                 XmNheight, &XgraGraphicDy,
                 NULL
               );

  XgraOldGraphicDx = XgraGraphicDx;
  XgraOldGraphicDy = XgraGraphicDy;

  XgraGraphicPixmap =

     XCreatePixmap ( XgraGraphicDisplay,
                     RootWindowOfScreen( XtScreen ( XgraGraphicWindow ) ),
                     XgraGraphicDx,
                     XgraGraphicDy,
                     DefaultDepthOfScreen ( XtScreen ( XgraGraphicWindow ) )
                   );

  XgraClearGraphicWindow( 0, 0, XgraGraphicDx, XgraGraphicDy );
  XgraInitializeUnitGrid();

  XgraSetMouseCursor( XgraGraphicWindow, XGRA_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XgraClearGraphicWindow                 |
|                                                             |
\------------------------------------------------------------*/

void XgraClearGraphicWindow( GraphicX, GraphicY, GraphicDx, GraphicDy )

   long GraphicX;
   long GraphicY;
   long GraphicDx;
   long GraphicDy;
{
  XFillRectangle( XgraGraphicDisplay,
                  XgraGraphicPixmap,
                  XgraBackgroundGC, 
                  GraphicX, GraphicY,
                  GraphicDx,
                  GraphicDy );
}

/*------------------------------------------------------------\
|                                                             |
|                      XgraRefreshGraphicWindow               |
|                                                             |
\------------------------------------------------------------*/

void XgraRefreshGraphicWindow( GraphicX, GraphicY, GraphicDx, GraphicDy )

   Dimension GraphicX;
   Dimension GraphicY;
   Dimension GraphicDx;
   Dimension GraphicDy;
{
  autbegin();

  XgraUndisplayCursor();

  XCopyArea( XgraGraphicDisplay,
             XgraGraphicPixmap,
             XtWindow ( XgraGraphicWindow ),
             XgraBackgroundGC,
             GraphicX, GraphicY,
             GraphicDx, GraphicDy,
             GraphicX, GraphicY 
           ); 

  XgraDisplayUnitGrid( GraphicX, GraphicY, 
                          GraphicX + GraphicDx, 
                          GraphicY + GraphicDy );
  XgraUndisplayCursor();

  autend();
}


/*------------------------------------------------------------\
|                                                             |
|                      XgraResizeGraphicWindow                |
|                                                             |
\------------------------------------------------------------*/

void XgraResizeGraphicWindow()

{
  Pixmap    OldPixmap;
  Dimension SourceY;
  Dimension TargetY;
  Dimension TargetDx;
  Dimension TargetDy;

  autbegin();

  XgraOldGraphicDx = XgraGraphicDx;
  XgraOldGraphicDy = XgraGraphicDy;

  XtVaGetValues( XgraGraphicWindow,
                 XmNwidth, &XgraGraphicDx,
                 XmNheight, &XgraGraphicDy,
                 NULL
               );   

  if ( ( XgraOldGraphicDx != XgraGraphicDx ) ||
       ( XgraOldGraphicDy != XgraGraphicDy ) )
  {
    OldPixmap = XgraGraphicPixmap;

    XgraGraphicPixmap = 

      XCreatePixmap ( XgraGraphicDisplay,
                      RootWindowOfScreen( XtScreen ( XgraGraphicWindow ) ),
                      XgraGraphicDx,
                      XgraGraphicDy,
                      DefaultDepthOfScreen ( XtScreen ( XgraGraphicWindow ) )
                    );

    XgraClearGraphicWindow( 0, 0, XgraGraphicDx, XgraGraphicDy );

    XgraResizeUnitGrid();

    if ( XgraOldGraphicDx < XgraGraphicDx )
    {
      TargetDx = XgraOldGraphicDx;

      XgraDisplayFigure( TargetDx, 0, 
                          XgraGraphicDx, XgraGraphicDy );
    }
    else
    {
      TargetDx = XgraGraphicDx;
    }
    
    if ( XgraOldGraphicDy < XgraGraphicDy )
    {
      SourceY  = 0;
      TargetDy = XgraOldGraphicDy;
      TargetY  = XgraGraphicDy - XgraOldGraphicDy;

      XgraDisplayFigure( 0, TargetDy, 
                          TargetDx, XgraGraphicDy ); 
    }
    else
    {
      TargetDy = XgraGraphicDy;
      TargetY  = 0;
      SourceY  = XgraOldGraphicDy - XgraGraphicDy;
    }

    XCopyArea( XgraGraphicDisplay,
               OldPixmap,
               XgraGraphicPixmap,
               XgraBackgroundGC,
               0, SourceY,
               TargetDx, TargetDy, 
               0, TargetY
             );

    XFreePixmap( XgraGraphicDisplay, OldPixmap );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XgraInitializeGraphic                   |
|                                                             |
\------------------------------------------------------------*/

void XgraInitializeGraphic()

{
  autbegin();

  XgraGraphicFrame = XtVaCreateManagedWidget( "XgraGraphicFrame",
                                               xmFrameWidgetClass,
                                               XgraMainForm,
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

  XgraGraphicWindow = XtVaCreateManagedWidget(
                                                "XgraGraphicWindow",
                                                xmDrawingAreaWidgetClass,
                                                XgraGraphicFrame,
                                                XmNtopAttachment,    XmATTACH_FORM,
                                                XmNrightAttachment,  XmATTACH_FORM,
                                                XmNbottomAttachment, XmATTACH_FORM,
                                                XmNleftAttachment,   XmATTACH_FORM,
                                                XmNbackground,       1,
                                                XmNforeground,       0,
                                                NULL
                                             );

  XgraGraphicDisplay = XtDisplay( XgraGraphicWindow );

  autend();
}
