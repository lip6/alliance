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
| File    :                  Colors.c                         |
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
 
# include "mut.h"
# include "aut.h"
# include "mlo.h"
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

  GC   XschBackgroundGC;
  GC   XschGridGC;
  GC   XschAcceptDrawGC;
  GC   XschConnectDrawGC;
  GC   XschLayerDrawGC[ XSCH_MAX_LAYER ];

  GC   XschSmallTextGC;
  GC   XschMediumTextGC;
  GC   XschLargeTextGC;
  GC   XschXorGCXor;
  GC   XschXorGCInvert;
  GC   XschXorGC;

  XFontStruct *XschLargeTextFont;
  XFontStruct *XschMediumTextFont;
  XFontStruct *XschSmallTextFont;

  Colormap XschColorMap;

/*------------------------------------------------------------\
|                                                             |
|                      Layer Color Define                     |
|                                                             |
\------------------------------------------------------------*/

  static char **XschColorName[ 5 ] =

  {
    &XSCH_BACKGROUND_COLOR_NAME,
    &XSCH_FOREGROUND_COLOR_NAME,
    &XSCH_CURSOR_COLOR_NAME,
    &XSCH_ACCEPT_COLOR_NAME,
    &XSCH_CONNECT_COLOR_NAME
  };

  static int XschColor[ XSCH_MAX_COLOR ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     XschSwitchColormap                      |
|                                                             |
\------------------------------------------------------------*/

void XschInitializeColorMap()
{
  Display *ADisplay;
  Screen  *AScreen;

  ADisplay      = XtDisplay( XschTopLevel );
  AScreen       = XtScreen( XschTopLevel );
  XschColorMap = DefaultColormapOfScreen( AScreen );

  if ( XSCH_SWITCH_COLOR_MAP )
  {
    XschColorMap = XCopyColormapAndFree( ADisplay, XschColorMap );
    XInstallColormap( ADisplay, XschColorMap );
    XtVaSetValues( XschTopLevel, XmNcolormap, XschColorMap, NULL );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      XschInitializeColors                   |
|                                                             |
\------------------------------------------------------------*/

void XschInitializeColors()

{
  XColor   ForgetIt;
  XColor   GetIt;
  int      Counter;
  char     Error;
  char    *ColorName;

  autbegin();

  Error = False;

  for ( Counter = 0; Counter < XSCH_MAX_COLOR; Counter++ )
  {
    if ( Counter < 5 )
    {
       ColorName = *XschColorName[ Counter ];
    }
    else
    {
      ColorName = XSCH_LAYER_NAME_TABLE[ Counter - 5 ][ 1 ];
    }

    if ( ColorName != (char *)NULL )
    {
      if ( ! XAllocNamedColor( XschGraphicDisplay, 
                               XschColorMap, ColorName, 
                              &GetIt, &ForgetIt ) ) 
      {
        fprintf ( stderr, "\nXmx: Unable to allocate color '%s' !\n",
                          ColorName );

        Error = True;
      }

      XschColor[ Counter ] = GetIt.pixel;
    }
    else 
    {
      XschColor[ Counter ] = XschColor[ 0 ];
    }
  }

  if ( Error == True ) exit( 1 );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                  XschChangeCursorContext                    |
|                                                             |
\------------------------------------------------------------*/

void XschChangeCursorContext()
{
  if ( XSCH_XOR_CURSOR == XSCH_TRUE )
  {
    XschXorGC = XschXorGCXor;
  }
  else
  {
    XschXorGC = XschXorGCInvert;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                  XschInitializeGraphicContext               |
|                                                             |
\------------------------------------------------------------*/

void XschInitializeGraphicContext()

{
  XGCValues GraphicValue;
  int       Counter;
  Window    Root;

  autbegin();

  XschLargeTextFont = XLoadQueryFont( XschGraphicDisplay,
                                       XSCH_LARGE_TEXT_FONT_NAME );

  XschMediumTextFont = XLoadQueryFont( XschGraphicDisplay,
                                        XSCH_MEDIUM_TEXT_FONT_NAME );

  XschSmallTextFont = XLoadQueryFont( XschGraphicDisplay,
                                       XSCH_SMALL_TEXT_FONT_NAME );

  GraphicValue.foreground = XschColor[ 0 ];  /* Black */
  GraphicValue.background = XschColor[ 0 ];
  GraphicValue.plane_mask = AllPlanes;

  Root = RootWindowOfScreen ( XtScreen ( XschGraphicWindow ) ),

  XschBackgroundGC = XCreateGC ( XschGraphicDisplay, 
                                  Root,
                                  ( GCForeground |
                                    GCBackground |
                                    GCPlaneMask ),
                                    &GraphicValue );

  GraphicValue.foreground = XschColor[ 1 ];  /* White */

  XschGridGC = XCreateGC ( XschGraphicDisplay, 
                            Root,
                            ( GCForeground |
                              GCBackground |
                              GCPlaneMask ),
                              &GraphicValue );

  GraphicValue.foreground = XschColor[ 1 ];  /* Text  */

  XschSmallTextGC = XCreateGC ( XschGraphicDisplay, 
                                 Root,
                                 ( GCForeground |
                                   GCBackground |
                                   GCPlaneMask ),
                                   &GraphicValue );

  XschMediumTextGC = XCreateGC ( XschGraphicDisplay, 
                                  Root,
                                  ( GCForeground |
                                    GCBackground |
                                    GCPlaneMask ),
                                   &GraphicValue );

  XschLargeTextGC = XCreateGC ( XschGraphicDisplay, 
                                 Root,
                                 ( GCForeground |
                                   GCBackground |
                                   GCPlaneMask ),
                                  &GraphicValue );

  XSetFont( XschGraphicDisplay,
            XschSmallTextGC, XschSmallTextFont->fid );

  XSetFont( XschGraphicDisplay,
            XschMediumTextGC, XschMediumTextFont->fid );

  XSetFont( XschGraphicDisplay,
            XschLargeTextGC, XschLargeTextFont->fid );

  GraphicValue.background = XschColor[ 2 ];  /* Xor Color */
  GraphicValue.foreground = XschColor[ 2 ];  /* Xor Color */
  GraphicValue.function   = GXxor;

  XschXorGCXor = XCreateGC ( XschGraphicDisplay, 
                           Root,
                           ( GCForeground |
                             GCBackground |
                             GCFunction   |
                             GCPlaneMask ),
                             &GraphicValue );

  GraphicValue.function   = GXinvert;

  XschXorGCInvert = XCreateGC ( XschGraphicDisplay, 
                                Root,
                                ( GCFunction   |
                                  GCPlaneMask ),
                                  &GraphicValue );

  XschChangeCursorContext();

  GraphicValue.background = XschColor[ 0 ];  /* Black        */
  GraphicValue.foreground = XschColor[ 3 ];  /* Accept Color */

  XschAcceptDrawGC = XCreateGC ( XschGraphicDisplay,
                                 Root,
                                 ( GCForeground |
                                   GCBackground |
                                   GCPlaneMask ),
                                   &GraphicValue );

  GraphicValue.foreground = XschColor[ 4 ];  /* Connect Color */

  XschConnectDrawGC = XCreateGC ( XschGraphicDisplay,
                                  Root,
                                  ( GCForeground |
                                    GCBackground |
                                    GCPlaneMask ),
                                    &GraphicValue );

  for ( Counter = 0; Counter < XSCH_MAX_LAYER; Counter++ )
  {
    GraphicValue.foreground = XschColor[ Counter + 5 ];

    XschLayerDrawGC[ Counter ] =

         XCreateGC ( XschGraphicDisplay,
                     Root,
                     ( GCForeground |
                       GCBackground |
                       GCPlaneMask  ),
                      &GraphicValue  );
  }

  autend();
}
