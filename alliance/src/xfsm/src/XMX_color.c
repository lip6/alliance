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
| Tool    :                    XFSM                           |
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
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
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

  GC   XfsmBackgroundGC;
  GC   XfsmGridGC;
  GC   XfsmAcceptDrawGC;
  GC   XfsmConnectDrawGC;
  GC   XfsmLayerDrawGC[ XFSM_MAX_LAYER ];

  GC   XfsmSmallTextGC;
  GC   XfsmMediumTextGC;
  GC   XfsmLargeTextGC;
  GC   XfsmXorGC;
  GC   XfsmXorGCXor;
  GC   XfsmXorGCInvert;

  XFontStruct *XfsmLargeTextFont;
  XFontStruct *XfsmMediumTextFont;
  XFontStruct *XfsmSmallTextFont;

  Colormap XfsmColorMap;

/*------------------------------------------------------------\
|                                                             |
|                      Layer Color Define                     |
|                                                             |
\------------------------------------------------------------*/

  static char **XfsmColorName[ 5 ] =

  {
    &XFSM_BACKGROUND_COLOR_NAME,
    &XFSM_FOREGROUND_COLOR_NAME,
    &XFSM_CURSOR_COLOR_NAME,
    &XFSM_ACCEPT_COLOR_NAME,
    &XFSM_CONNECT_COLOR_NAME
  };

  static int XfsmColor[ XFSM_MAX_COLOR ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     XfsmSwitchColormap                      |
|                                                             |
\------------------------------------------------------------*/

void XfsmInitializeColorMap()
{
  Display *ADisplay;
  Screen  *AScreen;

  ADisplay      = XtDisplay( XfsmTopLevel );
  AScreen       = XtScreen( XfsmTopLevel );
  XfsmColorMap = DefaultColormapOfScreen( AScreen );

  if ( XFSM_SWITCH_COLOR_MAP )
  {
    XfsmColorMap = XCopyColormapAndFree( ADisplay, XfsmColorMap );
    XInstallColormap( ADisplay, XfsmColorMap );
    XtVaSetValues( XfsmTopLevel, XmNcolormap, XfsmColorMap, NULL );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      XfsmInitializeColors                   |
|                                                             |
\------------------------------------------------------------*/

void XfsmInitializeColors()

{
  XColor   ForgetIt;
  XColor   GetIt;
  Window   Root;
  int      Counter;
  char     Error;
  char    *ColorName;

  autbegin();

  Error = False;

  for ( Counter = 0; Counter < XFSM_MAX_COLOR; Counter++ )
  {
    if ( Counter < 5 )
    {
       ColorName = *XfsmColorName[ Counter ];
    }
    else
    {
      ColorName = XFSM_LAYER_NAME_TABLE[ Counter - 5 ][ 1 ];
    }

    if ( ColorName != (char *)NULL )
    {
      if ( ! XAllocNamedColor( XfsmGraphicDisplay, 
                               XfsmColorMap, ColorName, 
                              &GetIt, &ForgetIt ) ) 
      {
        fprintf ( stderr, "\nXmx: Unable to allocate color '%s' !\n",
                          ColorName );

        Error = True;
      }

      XfsmColor[ Counter ] = GetIt.pixel;
    }
    else 
    {
      XfsmColor[ Counter ] = XfsmColor[ 0 ];
    }
  }

  if ( Error == True ) exit( 1 );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                  XfsmChangeCursorContext                    |
|                                                             |
\------------------------------------------------------------*/

void XfsmChangeCursorContext()
{
  XGCValues GraphicValue;

  if ( XFSM_XOR_CURSOR == XFSM_TRUE )
  {
    XfsmXorGC = XfsmXorGCXor;
  }
  else
  {
    XfsmXorGC = XfsmXorGCInvert;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                  XfsmInitializeGraphicContext               |
|                                                             |
\------------------------------------------------------------*/

void XfsmInitializeGraphicContext()

{
  XGCValues GraphicValue;
  int       Counter;
  Window    Root;

  autbegin();

  XfsmLargeTextFont = XLoadQueryFont( XfsmGraphicDisplay,
                                       XFSM_LARGE_TEXT_FONT_NAME );

  XfsmMediumTextFont = XLoadQueryFont( XfsmGraphicDisplay,
                                        XFSM_MEDIUM_TEXT_FONT_NAME );

  XfsmSmallTextFont = XLoadQueryFont( XfsmGraphicDisplay,
                                       XFSM_SMALL_TEXT_FONT_NAME );

  GraphicValue.foreground = XfsmColor[ 0 ];  /* Black */
  GraphicValue.background = XfsmColor[ 0 ];
  GraphicValue.plane_mask = AllPlanes;

  Root = RootWindowOfScreen ( XtScreen ( XfsmGraphicWindow ) ),

  XfsmBackgroundGC = XCreateGC ( XfsmGraphicDisplay, 
                                  Root,
                                  ( GCForeground |
                                    GCBackground |
                                    GCPlaneMask ),
                                    &GraphicValue );

  GraphicValue.foreground = XfsmColor[ 1 ];  /* White */

  XfsmGridGC = XCreateGC ( XfsmGraphicDisplay, 
                            Root,
                            ( GCForeground |
                              GCBackground |
                              GCPlaneMask ),
                              &GraphicValue );

  GraphicValue.foreground = XfsmColor[ 1 ];  /* Text  */

  XfsmSmallTextGC = XCreateGC ( XfsmGraphicDisplay, 
                                 Root,
                                 ( GCForeground |
                                   GCBackground |
                                   GCPlaneMask ),
                                   &GraphicValue );

  XfsmMediumTextGC = XCreateGC ( XfsmGraphicDisplay, 
                                  Root,
                                  ( GCForeground |
                                    GCBackground |
                                    GCPlaneMask ),
                                   &GraphicValue );

  XfsmLargeTextGC = XCreateGC ( XfsmGraphicDisplay, 
                                 Root,
                                 ( GCForeground |
                                   GCBackground |
                                   GCPlaneMask ),
                                  &GraphicValue );

  XSetFont( XfsmGraphicDisplay,
            XfsmSmallTextGC, XfsmSmallTextFont->fid );

  XSetFont( XfsmGraphicDisplay,
            XfsmMediumTextGC, XfsmMediumTextFont->fid );

  XSetFont( XfsmGraphicDisplay,
            XfsmLargeTextGC, XfsmLargeTextFont->fid );

  GraphicValue.background = XfsmColor[ 2 ];
  GraphicValue.foreground = XfsmColor[ 2 ];
  GraphicValue.function   = GXxor;

  XfsmXorGCXor = XCreateGC ( XfsmGraphicDisplay, 
                           Root,
                           ( GCForeground |
                             GCBackground |
                             GCFunction   |
                             GCPlaneMask ),
                             &GraphicValue );

  GraphicValue.function   = GXinvert;

  XfsmXorGCInvert = XCreateGC ( XfsmGraphicDisplay, 
                                Root,
                                ( GCFunction   |
                                  GCPlaneMask ),
                                  &GraphicValue );

  XfsmChangeCursorContext();

  GraphicValue.background = XfsmColor[ 0 ];  /* Black        */
  GraphicValue.foreground = XfsmColor[ 3 ];  /* Accept Color */

  XfsmAcceptDrawGC = XCreateGC ( XfsmGraphicDisplay,
                                 Root,
                                 ( GCForeground |
                                   GCBackground |
                                   GCPlaneMask ),
                                   &GraphicValue );

  GraphicValue.foreground = XfsmColor[ 4 ];  /* Connect Color */

  XfsmConnectDrawGC = XCreateGC ( XfsmGraphicDisplay,
                                  Root,
                                  ( GCForeground |
                                    GCBackground |
                                    GCPlaneMask ),
                                    &GraphicValue );

  for ( Counter = 0; Counter < XFSM_MAX_LAYER; Counter++ )
  {
    GraphicValue.foreground = XfsmColor[ Counter + 5 ];

    XfsmLayerDrawGC[ Counter ] =

         XCreateGC ( XfsmGraphicDisplay,
                     Root,
                     ( GCForeground |
                       GCBackground |
                       GCPlaneMask  ),
                      &GraphicValue  );
  }

  autend();
}
