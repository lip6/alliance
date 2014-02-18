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
| File    :                  Colors.c                         |
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
 
# include "mut.h"
# include "aut.h"
# include "XSB.h"
# include "XMX.h"
# include "XMX_color.h" 

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

  GC   XgraBackgroundGC;
  GC   XgraGridGC;
  GC   XgraAcceptDrawGC;
  GC   XgraConnectDrawGC;
  GC   XgraLayerDrawGC[ XGRA_MAX_LAYER ];

  GC   XgraSmallTextGC;
  GC   XgraMediumTextGC;
  GC   XgraLargeTextGC;
  GC   XgraXorGC;

  XFontStruct *XgraLargeTextFont;
  XFontStruct *XgraMediumTextFont;
  XFontStruct *XgraSmallTextFont;

/*------------------------------------------------------------\
|                                                             |
|                      Layer Color Define                     |
|                                                             |
\------------------------------------------------------------*/

  static char **XgraColorName[ 5 ] =

  {
    &XGRA_BACKGROUND_COLOR_NAME,
    &XGRA_FOREGROUND_COLOR_NAME,
    &XGRA_CURSOR_COLOR_NAME,
    &XGRA_ACCEPT_COLOR_NAME,
    &XGRA_CONNECT_COLOR_NAME
  };

  static int XgraColor[ XGRA_MAX_COLOR ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XgraInitializeColors                   |
|                                                             |
\------------------------------------------------------------*/

void XgraInitializeColors()

{
  XColor   ForgetIt;
  XColor   GetIt;
  int      Counter;
  Colormap ColorMap;
  char     Error;
  char    *ColorName;

  autbegin();

  ColorMap  = DefaultColormapOfScreen ( XtScreen ( XgraGraphicWindow ) );
  Error     = False;

  for ( Counter = 0; Counter < XGRA_MAX_COLOR; Counter++ )
  {
    if ( Counter < 5 )
    {
       ColorName = *XgraColorName[ Counter ];
    }
    else
    {
      ColorName = XGRA_LAYER_NAME_TABLE[ Counter - 5 ][ 1 ];
    }

    if ( ColorName != (char *)NULL )
    {
      if ( ! XAllocNamedColor( XgraGraphicDisplay, 
                               ColorMap, ColorName, 
                              &GetIt, &ForgetIt ) ) 
      {
        fprintf ( stderr, "\nXmx100: Color name '%s' is not in X11 database !\n",
                          ColorName );

        Error = True;
      }

      XgraColor[ Counter ] = GetIt.pixel;
    }
    else 
    {
      XgraColor[ Counter ] = XgraColor[ 0 ];
    }
  }

  if ( Error == True ) exit( 1 );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                  XgraInitializeGraphicContext               |
|                                                             |
\------------------------------------------------------------*/

void XgraInitializeGraphicContext()

{
  XGCValues GraphicValue;
  int       Counter;
  Window    Root;

  autbegin();

  XgraLargeTextFont = XLoadQueryFont( XgraGraphicDisplay,
                                       XGRA_LARGE_TEXT_FONT_NAME );

  XgraMediumTextFont = XLoadQueryFont( XgraGraphicDisplay,
                                        XGRA_MEDIUM_TEXT_FONT_NAME );

  XgraSmallTextFont = XLoadQueryFont( XgraGraphicDisplay,
                                       XGRA_SMALL_TEXT_FONT_NAME );

  GraphicValue.foreground = XgraColor[ 0 ];  /* Black */
  GraphicValue.background = XgraColor[ 0 ];
  GraphicValue.plane_mask = AllPlanes;

  Root = RootWindowOfScreen ( XtScreen ( XgraGraphicWindow ) ),

  XgraBackgroundGC = XCreateGC ( XgraGraphicDisplay, 
                                  Root,
                                  ( GCForeground |
                                    GCBackground |
                                    GCPlaneMask ),
                                    &GraphicValue );

  GraphicValue.foreground = XgraColor[ 1 ];  /* White */

  XgraGridGC = XCreateGC ( XgraGraphicDisplay, 
                            Root,
                            ( GCForeground |
                              GCBackground |
                              GCPlaneMask ),
                              &GraphicValue );

  GraphicValue.foreground = XgraColor[ 1 ];  /* Text  */

  XgraSmallTextGC = XCreateGC ( XgraGraphicDisplay, 
                                 Root,
                                 ( GCForeground |
                                   GCBackground |
                                   GCPlaneMask ),
                                   &GraphicValue );

  XgraMediumTextGC = XCreateGC ( XgraGraphicDisplay, 
                                  Root,
                                  ( GCForeground |
                                    GCBackground |
                                    GCPlaneMask ),
                                   &GraphicValue );

  XgraLargeTextGC = XCreateGC ( XgraGraphicDisplay, 
                                 Root,
                                 ( GCForeground |
                                   GCBackground |
                                   GCPlaneMask ),
                                  &GraphicValue );

  XSetFont( XgraGraphicDisplay,
            XgraSmallTextGC, XgraSmallTextFont->fid );

  XSetFont( XgraGraphicDisplay,
            XgraMediumTextGC, XgraMediumTextFont->fid );

  XSetFont( XgraGraphicDisplay,
            XgraLargeTextGC, XgraLargeTextFont->fid );

  GraphicValue.background = XgraColor[ 2 ];  /* Xor Color */
  GraphicValue.foreground = XgraColor[ 2 ];  /* Xor Color */
  GraphicValue.function   = GXxor;

  XgraXorGC = XCreateGC ( XgraGraphicDisplay, 
                           Root,
                           ( GCForeground |
                             GCBackground |
                             GCFunction   |
                             GCPlaneMask ),
                             &GraphicValue );

  GraphicValue.background = XgraColor[ 0 ];  /* Black        */
  GraphicValue.foreground = XgraColor[ 3 ];  /* Accept Color */

  XgraAcceptDrawGC = XCreateGC ( XgraGraphicDisplay,
                                 Root,
                                 ( GCForeground |
                                   GCBackground |
                                   GCPlaneMask ),
                                   &GraphicValue );

  GraphicValue.foreground = XgraColor[ 4 ];  /* Connect Color */

  XgraConnectDrawGC = XCreateGC ( XgraGraphicDisplay,
                                  Root,
                                  ( GCForeground |
                                    GCBackground |
                                    GCPlaneMask ),
                                    &GraphicValue );

  for ( Counter = 0; Counter < XGRA_MAX_LAYER; Counter++ )
  {
    GraphicValue.foreground = XgraColor[ Counter + 5 ];

    XgraLayerDrawGC[ Counter ] =

         XCreateGC ( XgraGraphicDisplay,
                     Root,
                     ( GCForeground |
                       GCBackground |
                       GCPlaneMask  ),
                      &GraphicValue  );
  }

  autend();
}
