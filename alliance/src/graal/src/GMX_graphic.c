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
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                 Graphic.c                         |
|                                                             |
| Author  :                Jacomme Ludovic                    |
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
# include <Xm/Form.h>
# include <Xm/Frame.h>
# include <Xm/DrawingA.h>
 
# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "GSB.h"
# include "GTB.h"
# include "GMX.h"

# include "GMX_graphic.h"
# include "GMX_view.h"
# include "GMX_grid.h"

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

  Widget   GraalGraphicFrame;
  Widget   GraalGraphicWindow;
  Display *GraalGraphicDisplay = (Display *)NULL;

  Dimension  GraalGraphicDx    = 0;
  Dimension  GraalGraphicDy    = 0;
  Dimension  GraalOldGraphicDx = 0;
  Dimension  GraalOldGraphicDy = 0;

  Pixmap     GraalGraphicPixmap   = (Pixmap)NULL;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 GraalInitializeGraphicWindow                |
|                                                             |
\------------------------------------------------------------*/

void GraalInitializeGraphicWindow()
 
{
  rdsbegin();

  XtVaGetValues( GraalGraphicWindow,
                 XmNwidth, &GraalGraphicDx,
                 XmNheight, &GraalGraphicDy,
                 NULL
               );

  GraalOldGraphicDx = GraalGraphicDx;
  GraalOldGraphicDy = GraalGraphicDy;

  GraalGraphicPixmap =

     XCreatePixmap ( GraalGraphicDisplay,
                     RootWindowOfScreen( XtScreen ( GraalGraphicWindow ) ),
                     GraalGraphicDx,
                     GraalGraphicDy,
                     DefaultDepthOfScreen ( XtScreen ( GraalGraphicWindow ) )
                   );

  GraalClearGraphicWindow( 0, 0, GraalGraphicDx, GraalGraphicDy );
  GraalInitializeLambdaGrid();

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_NORMAL_CURSOR );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalClearGraphicWindow                 |
|                                                             |
\------------------------------------------------------------*/

void GraalClearGraphicWindow( GraphicX, GraphicY, GraphicDx, GraphicDy )

   long GraphicX;
   long GraphicY;
   long GraphicDx;
   long GraphicDy;
{
  XFillRectangle( GraalGraphicDisplay,
                  GraalGraphicPixmap,
                  GraalBackgroundGC, 
                  GraphicX, GraphicY,
                  GraphicDx,
                  GraphicDy );
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalRefreshGraphicWindow               |
|                                                             |
\------------------------------------------------------------*/

void GraalRefreshGraphicWindow( GraphicX, GraphicY, GraphicDx, GraphicDy )

   Dimension GraphicX;
   Dimension GraphicY;
   Dimension GraphicDx;
   Dimension GraphicDy;
{
  rdsbegin();

  GraalUndisplayCursor();

  XCopyArea( GraalGraphicDisplay,
             GraalGraphicPixmap,
             XtWindow ( GraalGraphicWindow ),
             GraalBackgroundGC,
             GraphicX, GraphicY,
             GraphicDx, GraphicDy,
             GraphicX, GraphicY 
           ); 

  GraalDisplayLambdaGrid( GraphicX, GraphicY, 
                          GraphicX + GraphicDx, 
                          GraphicY + GraphicDy );
  GraalUndisplayCursor();

  rdsend();
}


/*------------------------------------------------------------\
|                                                             |
|                     GraalResizeGraphicWindow                |
|                                                             |
\------------------------------------------------------------*/

void GraalResizeGraphicWindow()

{
  Pixmap    OldPixmap;
  Dimension SourceY;
  Dimension TargetY;
  Dimension TargetDx;
  Dimension TargetDy;

  rdsbegin();

  GraalOldGraphicDx = GraalGraphicDx;
  GraalOldGraphicDy = GraalGraphicDy;

  XtVaGetValues( GraalGraphicWindow,
                 XmNwidth, &GraalGraphicDx,
                 XmNheight, &GraalGraphicDy,
                 NULL
               );   

  if ( ( GraalOldGraphicDx != GraalGraphicDx ) ||
       ( GraalOldGraphicDy != GraalGraphicDy ) )
  {
    OldPixmap = GraalGraphicPixmap;

    GraalGraphicPixmap = 

      XCreatePixmap ( GraalGraphicDisplay,
                      RootWindowOfScreen( XtScreen ( GraalGraphicWindow ) ),
                      GraalGraphicDx,
                      GraalGraphicDy,
                      DefaultDepthOfScreen ( XtScreen ( GraalGraphicWindow ) )
                    );

    GraalClearGraphicWindow( 0, 0, GraalGraphicDx, GraalGraphicDy );

    GraalResizeLambdaGrid();

    if ( GraalOldGraphicDx < GraalGraphicDx )
    {
      TargetDx = GraalOldGraphicDx;

      GraalDisplayFigure( TargetDx, 0, 
                          GraalGraphicDx, GraalGraphicDy );
    }
    else
    {
      TargetDx = GraalGraphicDx;
    }
    
    if ( GraalOldGraphicDy < GraalGraphicDy )
    {
      SourceY  = 0;
      TargetDy = GraalOldGraphicDy;
      TargetY  = GraalGraphicDy - GraalOldGraphicDy;

      GraalDisplayFigure( 0, TargetDy, 
                          TargetDx, GraalGraphicDy ); 
    }
    else
    {
      TargetDy = GraalGraphicDy;
      TargetY  = 0;
      SourceY  = GraalOldGraphicDy - GraalGraphicDy;
    }

    XCopyArea( GraalGraphicDisplay,
               OldPixmap,
               GraalGraphicPixmap,
               GraalBackgroundGC,
               0, SourceY,
               TargetDx, TargetDy, 
               0, TargetY
             );

    XFreePixmap( GraalGraphicDisplay, OldPixmap );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    GraalInitializeGraphic                   |
|                                                             |
\------------------------------------------------------------*/

void GraalInitializeGraphic()

{
  rdsbegin();

  GraalGraphicFrame = XtVaCreateManagedWidget( "GraalGraphicFrame",
                                               xmFrameWidgetClass,
                                               GraalMainForm,
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

  GraalGraphicWindow = XtVaCreateManagedWidget(
                                                "GraalGraphicWindow",
                                                xmDrawingAreaWidgetClass,
                                                GraalGraphicFrame,
                                                XmNtopAttachment,    XmATTACH_FORM,
                                                XmNrightAttachment,  XmATTACH_FORM,
                                                XmNbottomAttachment, XmATTACH_FORM,
                                                XmNleftAttachment,   XmATTACH_FORM,
                                                XmNbackground,       1,
                                                XmNforeground,       0,
                                                NULL
                                             );

  GraalGraphicDisplay = XtDisplay( GraalGraphicWindow );

  rdsend();
}
