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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                    XSCH                           |
|                                                             |
| File    :                 Graphic.c                         |
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
# include <Xm/Form.h>
# include <Xm/Frame.h>
# include <Xm/DrawingA.h>
 
# include "mut.h"
# include "aut.h"
# include "mlo.h"
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

  Widget   XschGraphicFrame;
  Widget   XschGraphicWindow;
  Display *XschGraphicDisplay = (Display *)NULL;

  Dimension  XschGraphicDx    = 0;
  Dimension  XschGraphicDy    = 0;
  Dimension  XschOldGraphicDx = 0;
  Dimension  XschOldGraphicDy = 0;

  Pixmap     XschGraphicPixmap   = (Pixmap)NULL;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 XschInitializeGraphicWindow                |
|                                                             |
\------------------------------------------------------------*/

void XschInitializeGraphicWindow()
 
{
  autbegin();

  XtVaGetValues( XschGraphicWindow,
                 XmNwidth, &XschGraphicDx,
                 XmNheight, &XschGraphicDy,
                 NULL
               );

  XschOldGraphicDx = XschGraphicDx;
  XschOldGraphicDy = XschGraphicDy;

  XschGraphicPixmap =

     XCreatePixmap ( XschGraphicDisplay,
                     RootWindowOfScreen( XtScreen ( XschGraphicWindow ) ),
                     XschGraphicDx,
                     XschGraphicDy,
                     DefaultDepthOfScreen ( XtScreen ( XschGraphicWindow ) )
                   );

  XschClearGraphicWindow( 0, 0, XschGraphicDx, XschGraphicDy );
  XschInitializeUnitGrid();

  XschSetMouseCursor( XschGraphicWindow, XSCH_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XschClearGraphicWindow                 |
|                                                             |
\------------------------------------------------------------*/

void XschClearGraphicWindow( GraphicX, GraphicY, GraphicDx, GraphicDy )

   long GraphicX;
   long GraphicY;
   long GraphicDx;
   long GraphicDy;
{
  XFillRectangle( XschGraphicDisplay,
                  XschGraphicPixmap,
                  XschBackgroundGC, 
                  GraphicX, GraphicY,
                  GraphicDx,
                  GraphicDy );
}

/*------------------------------------------------------------\
|                                                             |
|                      XschRefreshGraphicWindow               |
|                                                             |
\------------------------------------------------------------*/

void XschRefreshGraphicWindow( GraphicX, GraphicY, GraphicDx, GraphicDy )

   Dimension GraphicX;
   Dimension GraphicY;
   Dimension GraphicDx;
   Dimension GraphicDy;
{
  autbegin();

  XschUndisplayCursor();

  XCopyArea( XschGraphicDisplay,
             XschGraphicPixmap,
             XtWindow ( XschGraphicWindow ),
             XschBackgroundGC,
             GraphicX, GraphicY,
             GraphicDx, GraphicDy,
             GraphicX, GraphicY 
           ); 

  XschDisplayUnitGrid( GraphicX, GraphicY, 
                          GraphicX + GraphicDx, 
                          GraphicY + GraphicDy );
  XschUndisplayCursor();

  autend();
}


/*------------------------------------------------------------\
|                                                             |
|                      XschResizeGraphicWindow                |
|                                                             |
\------------------------------------------------------------*/

void XschResizeGraphicWindow()

{
  Pixmap    OldPixmap;
  Dimension SourceY;
  Dimension TargetY;
  Dimension TargetDx;
  Dimension TargetDy;

  autbegin();

  XschOldGraphicDx = XschGraphicDx;
  XschOldGraphicDy = XschGraphicDy;

  XtVaGetValues( XschGraphicWindow,
                 XmNwidth, &XschGraphicDx,
                 XmNheight, &XschGraphicDy,
                 NULL
               );   

  if ( ( XschOldGraphicDx != XschGraphicDx ) ||
       ( XschOldGraphicDy != XschGraphicDy ) )
  {
    OldPixmap = XschGraphicPixmap;

    XschGraphicPixmap = 

      XCreatePixmap ( XschGraphicDisplay,
                      RootWindowOfScreen( XtScreen ( XschGraphicWindow ) ),
                      XschGraphicDx,
                      XschGraphicDy,
                      DefaultDepthOfScreen ( XtScreen ( XschGraphicWindow ) )
                    );

    XschClearGraphicWindow( 0, 0, XschGraphicDx, XschGraphicDy );

    XschResizeUnitGrid();

    if ( XschOldGraphicDx < XschGraphicDx )
    {
      TargetDx = XschOldGraphicDx;

      XschDisplayFigure( TargetDx, 0, 
                          XschGraphicDx, XschGraphicDy );
    }
    else
    {
      TargetDx = XschGraphicDx;
    }
    
    if ( XschOldGraphicDy < XschGraphicDy )
    {
      SourceY  = 0;
      TargetDy = XschOldGraphicDy;
      TargetY  = XschGraphicDy - XschOldGraphicDy;

      XschDisplayFigure( 0, TargetDy, 
                          TargetDx, XschGraphicDy ); 
    }
    else
    {
      TargetDy = XschGraphicDy;
      TargetY  = 0;
      SourceY  = XschOldGraphicDy - XschGraphicDy;
    }

    XCopyArea( XschGraphicDisplay,
               OldPixmap,
               XschGraphicPixmap,
               XschBackgroundGC,
               0, SourceY,
               TargetDx, TargetDy, 
               0, TargetY
             );

    XFreePixmap( XschGraphicDisplay, OldPixmap );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XschInitializeGraphic                   |
|                                                             |
\------------------------------------------------------------*/

void XschInitializeGraphic()

{
  autbegin();

  XschGraphicFrame = XtVaCreateManagedWidget( "XschGraphicFrame",
                                               xmFrameWidgetClass,
                                               XschMainForm,
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

  XschGraphicWindow = XtVaCreateManagedWidget(
                                                "XschGraphicWindow",
                                                xmDrawingAreaWidgetClass,
                                                XschGraphicFrame,
                                                XmNtopAttachment,    XmATTACH_FORM,
                                                XmNrightAttachment,  XmATTACH_FORM,
                                                XmNbottomAttachment, XmATTACH_FORM,
                                                XmNleftAttachment,   XmATTACH_FORM,
                                                XmNbackground,       1,
                                                XmNforeground,       0,
                                                NULL
                                             );

  XschGraphicDisplay = XtDisplay( XschGraphicWindow );

  autend();
}
