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
| Tool    :                    XPAT                           |
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
# include "pat.h"
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

  Widget   XpatGraphicFrame;
  Widget   XpatGraphicWindow;
  Display *XpatGraphicDisplay = (Display *)NULL;

  Dimension  XpatGraphicDx    = 0;
  Dimension  XpatGraphicDy    = 0;
  Dimension  XpatOldGraphicDx = 0;
  Dimension  XpatOldGraphicDy = 0;

  Pixmap     XpatGraphicPixmap   = (Pixmap)NULL;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 XpatInitializeGraphicWindow                |
|                                                             |
\------------------------------------------------------------*/

void XpatInitializeGraphicWindow()
 
{
  autbegin();

  XtVaGetValues( XpatGraphicWindow,
                 XmNwidth, &XpatGraphicDx,
                 XmNheight, &XpatGraphicDy,
                 NULL
               );

  XpatOldGraphicDx = XpatGraphicDx;
  XpatOldGraphicDy = XpatGraphicDy;

  XpatGraphicPixmap =

     XCreatePixmap ( XpatGraphicDisplay,
                     RootWindowOfScreen( XtScreen ( XpatGraphicWindow ) ),
                     XpatGraphicDx,
                     XpatGraphicDy,
                     DefaultDepthOfScreen ( XtScreen ( XpatGraphicWindow ) )
                   );

  XpatClearGraphicWindow( 0, 0, XpatGraphicDx, XpatGraphicDy );
  XpatInitializeUnitGrid();

  XpatSetMouseCursor( XpatGraphicWindow, XPAT_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XpatClearGraphicWindow                 |
|                                                             |
\------------------------------------------------------------*/

void XpatClearGraphicWindow( GraphicX, GraphicY, GraphicDx, GraphicDy )

   long GraphicX;
   long GraphicY;
   long GraphicDx;
   long GraphicDy;
{
  XFillRectangle( XpatGraphicDisplay,
                  XpatGraphicPixmap,
                  XpatBackgroundGC, 
                  GraphicX, GraphicY,
                  GraphicDx,
                  GraphicDy );
}

/*------------------------------------------------------------\
|                                                             |
|                      XpatRefreshGraphicWindow               |
|                                                             |
\------------------------------------------------------------*/

void XpatRefreshGraphicWindow( GraphicX, GraphicY, GraphicDx, GraphicDy )

   Dimension GraphicX;
   Dimension GraphicY;
   Dimension GraphicDx;
   Dimension GraphicDy;
{
  autbegin();

  XpatUndisplayCursor();

  XCopyArea( XpatGraphicDisplay,
             XpatGraphicPixmap,
             XtWindow ( XpatGraphicWindow ),
             XpatBackgroundGC,
             GraphicX, GraphicY,
             GraphicDx, GraphicDy,
             GraphicX, GraphicY 
           ); 

  XpatDisplayUnitGrid( GraphicX, GraphicY, 
                          GraphicX + GraphicDx, 
                          GraphicY + GraphicDy );
  XpatUndisplayCursor();

  autend();
}


/*------------------------------------------------------------\
|                                                             |
|                      XpatResizeGraphicWindow                |
|                                                             |
\------------------------------------------------------------*/

void XpatResizeGraphicWindow()

{
  Pixmap    OldPixmap;
  Dimension SourceY;
  Dimension TargetY;
  Dimension TargetDx;
  Dimension TargetDy;

  autbegin();

  XpatOldGraphicDx = XpatGraphicDx;
  XpatOldGraphicDy = XpatGraphicDy;

  XtVaGetValues( XpatGraphicWindow,
                 XmNwidth, &XpatGraphicDx,
                 XmNheight, &XpatGraphicDy,
                 NULL
               );   

  if ( ( XpatOldGraphicDx != XpatGraphicDx ) ||
       ( XpatOldGraphicDy != XpatGraphicDy ) )
  {
    OldPixmap = XpatGraphicPixmap;

    XpatGraphicPixmap = 

      XCreatePixmap ( XpatGraphicDisplay,
                      RootWindowOfScreen( XtScreen ( XpatGraphicWindow ) ),
                      XpatGraphicDx,
                      XpatGraphicDy,
                      DefaultDepthOfScreen ( XtScreen ( XpatGraphicWindow ) )
                    );

    XpatClearGraphicWindow( 0, 0, XpatGraphicDx, XpatGraphicDy );

    XpatResizeUnitGrid();

    if ( XpatOldGraphicDx < XpatGraphicDx )
    {
      TargetDx = XpatOldGraphicDx;

      XpatDisplayFigure( TargetDx, 0, 
                          XpatGraphicDx, XpatGraphicDy );
    }
    else
    {
      TargetDx = XpatGraphicDx;
    }
    
    if ( XpatOldGraphicDy < XpatGraphicDy )
    {
      SourceY  = 0;
      TargetDy = XpatOldGraphicDy;
      TargetY  = XpatGraphicDy - XpatOldGraphicDy;

      XpatDisplayFigure( 0, TargetDy, 
                          TargetDx, XpatGraphicDy ); 
    }
    else
    {
      TargetDy = XpatGraphicDy;
      TargetY  = 0;
      SourceY  = XpatOldGraphicDy - XpatGraphicDy;
    }

    XCopyArea( XpatGraphicDisplay,
               OldPixmap,
               XpatGraphicPixmap,
               XpatBackgroundGC,
               0, SourceY,
               TargetDx, TargetDy, 
               0, TargetY
             );

    XFreePixmap( XpatGraphicDisplay, OldPixmap );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XpatInitializeGraphic                   |
|                                                             |
\------------------------------------------------------------*/

void XpatInitializeGraphic()

{
  autbegin();

  XpatGraphicFrame = XtVaCreateManagedWidget( "XpatGraphicFrame",
                                               xmFrameWidgetClass,
                                               XpatMainForm,
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

  XpatGraphicWindow = XtVaCreateManagedWidget(
                                                "XpatGraphicWindow",
                                                xmDrawingAreaWidgetClass,
                                                XpatGraphicFrame,
                                                XmNtopAttachment,    XmATTACH_FORM,
                                                XmNrightAttachment,  XmATTACH_FORM,
                                                XmNbottomAttachment, XmATTACH_FORM,
                                                XmNleftAttachment,   XmATTACH_FORM,
                                                XmNbackground,       1,
                                                XmNforeground,       0,
                                                NULL
                                             );

  XpatGraphicDisplay = XtDisplay( XpatGraphicWindow );

  autend();
}
