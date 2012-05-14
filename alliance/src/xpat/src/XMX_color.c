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
# include "pat.h"
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

  GC   XpatBackgroundGC;
  GC   XpatGridGC;
  GC   XpatAcceptDrawGC;
  GC   XpatConnectDrawGC;
  GC   XpatLayerDrawGC[ XPAT_MAX_LAYER ];

  GC   XpatSmallTextGC;
  GC   XpatMediumTextGC;
  GC   XpatLargeTextGC;
  GC   XpatXorGCXor;
  GC   XpatXorGCInvert;
  GC   XpatXorGC;

  XFontStruct *XpatLargeTextFont;
  XFontStruct *XpatMediumTextFont;
  XFontStruct *XpatSmallTextFont;

  Colormap XpatColorMap;

/*------------------------------------------------------------\
|                                                             |
|                      Layer Color Define                     |
|                                                             |
\------------------------------------------------------------*/

  static char **XpatColorName[ 5 ] =

  {
    &XPAT_BACKGROUND_COLOR_NAME,
    &XPAT_FOREGROUND_COLOR_NAME,
    &XPAT_CURSOR_COLOR_NAME,
    &XPAT_ACCEPT_COLOR_NAME,
    &XPAT_CONNECT_COLOR_NAME
  };

  static int XpatColor[ XPAT_MAX_COLOR ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     XpatSwitchColormap                      |
|                                                             |
\------------------------------------------------------------*/

void XpatInitializeColorMap()
{
  Display *ADisplay;
  Screen  *AScreen;

  ADisplay      = XtDisplay( XpatTopLevel );
  AScreen       = XtScreen( XpatTopLevel );
  XpatColorMap = DefaultColormapOfScreen( AScreen );

  if ( XPAT_SWITCH_COLOR_MAP )
  {
    XpatColorMap = XCopyColormapAndFree( ADisplay, XpatColorMap );
    XInstallColormap( ADisplay, XpatColorMap );
    XtVaSetValues( XpatTopLevel, XmNcolormap, XpatColorMap, NULL );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      XpatInitializeColors                   |
|                                                             |
\------------------------------------------------------------*/

void XpatInitializeColors()

{
  XColor   ForgetIt;
  XColor   GetIt;
  Window   Root;
  int      Counter;
  char     Error;
  char    *ColorName;

  autbegin();

  Error = False;

  for ( Counter = 0; Counter < XPAT_MAX_COLOR; Counter++ )
  {
    if ( Counter < 5 )
    {
       ColorName = *XpatColorName[ Counter ];
    }
    else
    {
      ColorName = XPAT_LAYER_NAME_TABLE[ Counter - 5 ][ 1 ];
    }

    if ( ColorName != (char *)NULL )
    {
      if ( ! XAllocNamedColor( XpatGraphicDisplay, 
                               XpatColorMap, ColorName, 
                              &GetIt, &ForgetIt ) ) 
      {
        fprintf ( stderr, "\nXmx: Unable to allocate color '%s' !\n",
                          ColorName );

        Error = True;
      }

      XpatColor[ Counter ] = GetIt.pixel;
    }
    else 
    {
      XpatColor[ Counter ] = XpatColor[ 0 ];
    }
  }

  if ( Error == True ) exit( 1 );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                 XpatChangeCursorContext                    |
|                                                             |
\------------------------------------------------------------*/

void XpatChangeCursorContext()
{
  XGCValues GraphicValue;

  if ( XPAT_XOR_CURSOR == XPAT_TRUE )
  {
    XpatXorGC = XpatXorGCXor;
  }
  else
  {
    XpatXorGC = XpatXorGCInvert;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                  XpatInitializeGraphicContext               |
|                                                             |
\------------------------------------------------------------*/

void XpatInitializeGraphicContext()

{
  XGCValues GraphicValue;
  int       Counter;
  Window    Root;

  autbegin();

  XpatLargeTextFont = XLoadQueryFont( XpatGraphicDisplay,
                                       XPAT_LARGE_TEXT_FONT_NAME );

  XpatMediumTextFont = XLoadQueryFont( XpatGraphicDisplay,
                                        XPAT_MEDIUM_TEXT_FONT_NAME );

  XpatSmallTextFont = XLoadQueryFont( XpatGraphicDisplay,
                                       XPAT_SMALL_TEXT_FONT_NAME );

  GraphicValue.foreground = XpatColor[ 0 ];  /* Black */
  GraphicValue.background = XpatColor[ 0 ];
  GraphicValue.plane_mask = AllPlanes;

  Root = RootWindowOfScreen ( XtScreen ( XpatGraphicWindow ) ),

  XpatBackgroundGC = XCreateGC ( XpatGraphicDisplay, 
                                  Root,
                                  ( GCForeground |
                                    GCBackground |
                                    GCPlaneMask ),
                                    &GraphicValue );

  GraphicValue.foreground = XpatColor[ 1 ];  /* White */

  XpatGridGC = XCreateGC ( XpatGraphicDisplay, 
                            Root,
                            ( GCForeground |
                              GCBackground |
                              GCPlaneMask ),
                              &GraphicValue );

  GraphicValue.foreground = XpatColor[ 1 ];  /* Text  */

  XpatSmallTextGC = XCreateGC ( XpatGraphicDisplay, 
                                 Root,
                                 ( GCForeground |
                                   GCBackground |
                                   GCPlaneMask ),
                                   &GraphicValue );

  XpatMediumTextGC = XCreateGC ( XpatGraphicDisplay, 
                                  Root,
                                  ( GCForeground |
                                    GCBackground |
                                    GCPlaneMask ),
                                   &GraphicValue );

  XpatLargeTextGC = XCreateGC ( XpatGraphicDisplay, 
                                 Root,
                                 ( GCForeground |
                                   GCBackground |
                                   GCPlaneMask ),
                                  &GraphicValue );

  XSetFont( XpatGraphicDisplay,
            XpatSmallTextGC, XpatSmallTextFont->fid );

  XSetFont( XpatGraphicDisplay,
            XpatMediumTextGC, XpatMediumTextFont->fid );

  XSetFont( XpatGraphicDisplay,
            XpatLargeTextGC, XpatLargeTextFont->fid );

  GraphicValue.background = XpatColor[ 2 ];
  GraphicValue.foreground = XpatColor[ 2 ];
  GraphicValue.function   = GXxor;

  XpatXorGCXor = XCreateGC ( XpatGraphicDisplay, 
                           Root,
                           ( GCForeground |
                             GCBackground |
                             GCFunction   |
                             GCPlaneMask ),
                             &GraphicValue );

  GraphicValue.function   = GXinvert;

  XpatXorGCInvert = XCreateGC ( XpatGraphicDisplay, 
                                Root,
                                ( GCFunction   |
                                  GCPlaneMask ),
                                  &GraphicValue );
  XpatChangeCursorContext();
  

  GraphicValue.background = XpatColor[ 0 ];  /* Black        */
  GraphicValue.foreground = XpatColor[ 3 ];  /* Accept Color */

  XpatAcceptDrawGC = XCreateGC ( XpatGraphicDisplay,
                                 Root,
                                 ( GCForeground |
                                   GCBackground |
                                   GCPlaneMask ),
                                   &GraphicValue );

  GraphicValue.foreground = XpatColor[ 4 ];  /* Connect Color */

  XpatConnectDrawGC = XCreateGC ( XpatGraphicDisplay,
                                  Root,
                                  ( GCForeground |
                                    GCBackground |
                                    GCPlaneMask ),
                                    &GraphicValue );

  for ( Counter = 0; Counter < XPAT_MAX_LAYER; Counter++ )
  {
    GraphicValue.foreground = XpatColor[ Counter + 5 ];

    XpatLayerDrawGC[ Counter ] =

         XCreateGC ( XpatGraphicDisplay,
                     Root,
                     ( GCForeground |
                       GCBackground |
                       GCPlaneMask  ),
                      &GraphicValue  );
  }

  autend();
}
