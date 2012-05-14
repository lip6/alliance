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
| Tool    :                    XVPN                           |
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
# include <stdlib.h>
# include <string.h>
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

  GC   XvpnBackgroundGC;
  GC   XvpnGridGC;
  GC   XvpnAcceptDrawGC;
  GC   XvpnConnectDrawGC;
  GC   XvpnLayerDrawGC[ XVPN_MAX_LAYER ];

  GC   XvpnSmallTextGC;
  GC   XvpnMediumTextGC;
  GC   XvpnLargeTextGC;
  GC   XvpnXorGC;
  GC   XvpnXorGCXor;
  GC   XvpnXorGCInvert;

  XFontStruct *XvpnLargeTextFont;
  XFontStruct *XvpnMediumTextFont;
  XFontStruct *XvpnSmallTextFont;

  Colormap XvpnColorMap;

/*------------------------------------------------------------\
|                                                             |
|                      Layer Color Define                     |
|                                                             |
\------------------------------------------------------------*/

 static char **XvpnColorName[ 5 ] =

  {
    &XVPN_BACKGROUND_COLOR_NAME,
    &XVPN_FOREGROUND_COLOR_NAME,
    &XVPN_CURSOR_COLOR_NAME,
    &XVPN_ACCEPT_COLOR_NAME,
    &XVPN_CONNECT_COLOR_NAME
  };

  static int XvpnColor[ XVPN_MAX_COLOR ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     XvpnSwitchColormap                      |
|                                                             |
\------------------------------------------------------------*/

void XvpnInitializeColorMap()
{
  Display *ADisplay;
  Screen  *AScreen;

  ADisplay      = XtDisplay( XvpnTopLevel );
  AScreen       = XtScreen( XvpnTopLevel );
  XvpnColorMap = DefaultColormapOfScreen( AScreen );

  if ( XVPN_SWITCH_COLOR_MAP )
  {
    XvpnColorMap = XCopyColormapAndFree( ADisplay, XvpnColorMap );
    XInstallColormap( ADisplay, XvpnColorMap );
    XtVaSetValues( XvpnTopLevel, XmNcolormap, XvpnColorMap, NULL );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      XvpnInitializeColors                   |
|                                                             |
\------------------------------------------------------------*/

void XvpnInitializeColors()

{
  XColor   ForgetIt;
  XColor   GetIt;
  int      Counter;
  char     Error;
  char    *ColorName;

  autbegin();

  Error = False;

  for ( Counter = 0; Counter < XVPN_MAX_COLOR; Counter++ )
  {
    if ( Counter < 5 )
    {
       ColorName = *XvpnColorName[ Counter ];
    }
    else
    {
      ColorName = XVPN_LAYER_NAME_TABLE[ Counter - 5 ][ 1 ];
    }

    if ( ColorName != (char *)NULL )
    {
      if ( ! XAllocNamedColor( XvpnGraphicDisplay, 
                               XvpnColorMap, ColorName, 
                              &GetIt, &ForgetIt ) ) 
      {
        fprintf ( stderr, "\nXmx: Unable to allocate color '%s' !\n",
                          ColorName );

        Error = True;
      }

      XvpnColor[ Counter ] = GetIt.pixel;
    }
    else 
    {
      XvpnColor[ Counter ] = XvpnColor[ 0 ];
    }
  }

  if ( Error == True ) exit( 1 );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                  XvpnChangeCursorContext                    |
|                                                             |
\------------------------------------------------------------*/

void XvpnChangeCursorContext()
{
  if ( XVPN_XOR_CURSOR == XVPN_TRUE )
  {
    XvpnXorGC = XvpnXorGCXor;
  }
  else
  {
    XvpnXorGC = XvpnXorGCInvert;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                  XvpnInitializeGraphicContext               |
|                                                             |
\------------------------------------------------------------*/

void XvpnInitializeGraphicContext()

{
  XGCValues GraphicValue;
  int       Counter;
  Window    Root;

  autbegin();

  XvpnLargeTextFont = XLoadQueryFont( XvpnGraphicDisplay,
                                       XVPN_LARGE_TEXT_FONT_NAME );

  XvpnMediumTextFont = XLoadQueryFont( XvpnGraphicDisplay,
                                        XVPN_MEDIUM_TEXT_FONT_NAME );

  XvpnSmallTextFont = XLoadQueryFont( XvpnGraphicDisplay,
                                       XVPN_SMALL_TEXT_FONT_NAME );

  GraphicValue.foreground = XvpnColor[ 0 ];  /* Black */
  GraphicValue.background = XvpnColor[ 0 ];
  GraphicValue.plane_mask = AllPlanes;

  Root = RootWindowOfScreen ( XtScreen ( XvpnGraphicWindow ) ),

  XvpnBackgroundGC = XCreateGC ( XvpnGraphicDisplay, 
                                  Root,
                                  ( GCForeground |
                                    GCBackground |
                                    GCPlaneMask ),
                                    &GraphicValue );

  GraphicValue.foreground = XvpnColor[ 1 ];  /* White */

  XvpnGridGC = XCreateGC ( XvpnGraphicDisplay, 
                            Root,
                            ( GCForeground |
                              GCBackground |
                              GCPlaneMask ),
                              &GraphicValue );

  GraphicValue.foreground = XvpnColor[ 1 ];  /* Text  */

  XvpnSmallTextGC = XCreateGC ( XvpnGraphicDisplay, 
                                 Root,
                                 ( GCForeground |
                                   GCBackground |
                                   GCPlaneMask ),
                                   &GraphicValue );

  XvpnMediumTextGC = XCreateGC ( XvpnGraphicDisplay, 
                                  Root,
                                  ( GCForeground |
                                    GCBackground |
                                    GCPlaneMask ),
                                   &GraphicValue );

  XvpnLargeTextGC = XCreateGC ( XvpnGraphicDisplay, 
                                 Root,
                                 ( GCForeground |
                                   GCBackground |
                                   GCPlaneMask ),
                                  &GraphicValue );

  XSetFont( XvpnGraphicDisplay,
            XvpnSmallTextGC, XvpnSmallTextFont->fid );

  XSetFont( XvpnGraphicDisplay,
            XvpnMediumTextGC, XvpnMediumTextFont->fid );

  XSetFont( XvpnGraphicDisplay,
            XvpnLargeTextGC, XvpnLargeTextFont->fid );

  GraphicValue.background = XvpnColor[ 2 ];  /* Xor Color */
  GraphicValue.foreground = XvpnColor[ 2 ];  /* Xor Color */
  GraphicValue.function   = GXxor;

  XvpnXorGCXor = XCreateGC ( XvpnGraphicDisplay, 
                           Root,
                           ( GCForeground |
                             GCBackground |
                             GCFunction   |
                             GCPlaneMask ),
                             &GraphicValue );

  GraphicValue.function   = GXinvert;

  XvpnXorGCInvert = XCreateGC ( XvpnGraphicDisplay, 
                                Root,
                                ( GCFunction   |
                                  GCPlaneMask ),
                                  &GraphicValue );

  XvpnChangeCursorContext();

  GraphicValue.background = XvpnColor[ 0 ];  /* Black        */
  GraphicValue.foreground = XvpnColor[ 3 ];  /* Accept Color */

  XvpnAcceptDrawGC = XCreateGC ( XvpnGraphicDisplay,
                                 Root,
                                 ( GCForeground |
                                   GCBackground |
                                   GCPlaneMask ),
                                   &GraphicValue );

  GraphicValue.foreground = XvpnColor[ 4 ];  /* Connect Color */

  XvpnConnectDrawGC = XCreateGC ( XvpnGraphicDisplay,
                                  Root,
                                  ( GCForeground |
                                    GCBackground |
                                    GCPlaneMask ),
                                    &GraphicValue );

  for ( Counter = 0; Counter < XVPN_MAX_LAYER; Counter++ )
  {
    GraphicValue.foreground = XvpnColor[ Counter + 5 ];

    XvpnLayerDrawGC[ Counter ] =

         XCreateGC ( XvpnGraphicDisplay,
                     Root,
                     ( GCForeground |
                       GCBackground |
                       GCPlaneMask  ),
                      &GraphicValue  );
  }

  autend();
}
