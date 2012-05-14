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
| Tool    :                   XVPN                           |
|                                                             |
| File    :                  cursor.c                         |
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
# include <X11/cursorfont.h>
# include <X11/Intrinsic.h>
# include "mut.h"
# include "aut.h"
# include "XTB.h"
# include "XSB.h"
# include "XTB_cursor.h"

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

  static XvpnMouseCursor XvpnMouseCursorArray [ XVPN_MAX_CURSOR ] =

         {
           { XC_left_ptr      , True  },
           { XC_watch         , True  },
           { XC_pirate        , True  },
           { XC_cross_reverse , True  },
           { 0                , True  } 
         };

  static char  XvpnCursorMaskOr [ 8 ] =

  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  static char  XvpnCursorMaskAnd [ 8 ] =

  {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XvpnSetMouseCursor                    |
|                                                             |
\------------------------------------------------------------*/

void XvpnSetMouseCursor( MainWidget, CursorType )

     Widget MainWidget;
     short  CursorType;
{
  Display          *DisplayId;
  Window            MainWindow;
  XColor            White;
  XColor            Black;
  XColor            ForgetIt;
  Colormap          ColorMap;
  Pixmap            MaskOr;
  Pixmap            MaskAnd;
  Cursor            Type;
  Cursor            NewCursor;

  autbegin();

  DisplayId  = XtDisplay( MainWidget );
  MainWindow = XtWindow( MainWidget  );

  Type = XvpnMouseCursorArray[ CursorType ].CURSOR;

  if ( XvpnMouseCursorArray[ CursorType ].MAKE == True )
  {
    if ( Type != 0 )
    {
      NewCursor = XCreateFontCursor( DisplayId, Type );
    }
    else
    {
      ColorMap = DefaultColormapOfScreen ( XtScreen( MainWidget ) );

      XAllocNamedColor( DisplayId, ColorMap, "black", &Black, &ForgetIt );
      XAllocNamedColor( DisplayId, ColorMap, "white", &White, &ForgetIt );

      MaskOr = 

        XCreatePixmapFromBitmapData( DisplayId,
                                     MainWindow,
                                     XvpnCursorMaskOr,
                                     8, 8,
                                     Black.pixel,
                                     White.pixel, 1 );

      MaskAnd =

        XCreatePixmapFromBitmapData( DisplayId,
                                     MainWindow,
                                     XvpnCursorMaskAnd,
                                     8, 8,
                                     Black.pixel,
                                     White.pixel, 1 );

      NewCursor = XCreatePixmapCursor( DisplayId, 
                                       MaskAnd, MaskOr,
                                       &Black, &White, 0, 0 );
    }

    XvpnMouseCursorArray[ CursorType ].CURSOR = NewCursor;
    XvpnMouseCursorArray[ CursorType ].MAKE   = False;

    Type = NewCursor;
  }

  XDefineCursor( DisplayId, MainWindow, Type );

  XmUpdateDisplay( MainWidget );

  autend();
}
