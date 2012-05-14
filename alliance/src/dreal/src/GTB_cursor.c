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
| Tool    :                   DREAL                           |
|                                                             |
| File    :                  cursor.c                         |
|                                                             |
| Authors :              Jacomme Ludovic                      |
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
# include <X11/cursorfont.h>
# include <X11/Intrinsic.h>
# include "mut.h"
# include "rds.h"
# include "GTB.h"
# include "GTB_cursor.h"

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

  static DrealMouseCursor DrealMouseCursorArray [ DREAL_MAX_CURSOR ] =

         {
           { XC_left_ptr      , True  },
           { XC_watch         , True  },
           { XC_pirate        , True  },
           { XC_cross_reverse , True  },
           { 0                , True  }

         };

  static char  DrealCursorMaskOr [ 8 ] = 

  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  static char  DrealCursorMaskAnd [ 8 ] =

  {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      DrealSetMouseCursor                    |
|                                                             |
\------------------------------------------------------------*/

void DrealSetMouseCursor( MainWidget, CursorType )

     Widget MainWidget;
     char   CursorType;
{
  Display          *DisplayId;
  XEvent            MouseEvent;
  Widget            TopShell;
  Widget            ScanShell;
  Window            TopWindow;
  Window            ScanWindow;
  XWindowAttributes WindowAtt;
  XtAppContext      Context;
  XColor            White;
  XColor            Black;
  XColor            ForgetIt;
  Colormap          ColorMap;
  Pixmap            MaskOr;
  Pixmap            MaskAnd;
  Cursor            NewCursor;

  rdsbegin();

  ScanShell = MainWidget;

  while ( ! XtIsShell( ScanShell ) )
  {
    ScanShell = XtParent( ScanShell );
  }

  TopShell = ScanShell;

  while ( ! XtIsTopLevelShell( TopShell ) )
  {
    TopShell = XtParent( TopShell );
  }

  XFlush(XtDisplay (TopShell));

  DisplayId = XtDisplay( ScanShell );

  TopWindow  = XtWindow( TopShell  );
  ScanWindow = XtWindow( ScanShell );

  if ( DrealMouseCursorArray[ CursorType ].MAKE == True )
  {
    if ( DrealMouseCursorArray[ CursorType ].CURSOR != 0 )
    {
      NewCursor = XCreateFontCursor( DisplayId, 
                                     DrealMouseCursorArray[ CursorType ].CURSOR );
    }
    else
    {
      ColorMap = DefaultColormapOfScreen ( XtScreen( MainWidget ) );

      XAllocNamedColor( DisplayId, ColorMap, "black", &Black, &ForgetIt );
      XAllocNamedColor( DisplayId, ColorMap, "white", &White, &ForgetIt );

      MaskOr = 

        XCreatePixmapFromBitmapData( DisplayId,
                                     XtWindow( MainWidget ),
                                     DrealCursorMaskOr,
                                     8, 8,
                                     Black.pixel,
                                     White.pixel, 1 );
      MaskAnd =

        XCreatePixmapFromBitmapData( DisplayId,
                                     XtWindow( MainWidget ),
                                     DrealCursorMaskAnd,
                                     8, 8,
                                     Black.pixel,
                                     White.pixel, 1 );

      NewCursor = XCreatePixmapCursor( DisplayId, 
                                       MaskAnd, MaskOr,
                                       &Black, &White, 0, 0 );
    }

    DrealMouseCursorArray[ CursorType ].CURSOR = NewCursor;
    DrealMouseCursorArray[ CursorType ].MAKE   = False;
  }

  XDefineCursor( XtDisplay( TopShell ), 
                 XtWindow( MainWidget ),
                 DrealMouseCursorArray[ CursorType ].CURSOR );

  Context = XtWidgetToApplicationContext( TopShell );

  if ( XtIsRealized( ScanShell ) && XtIsRealized( TopShell ) )
  {
    while ( ( XGetWindowAttributes( DisplayId, ScanWindow, &WindowAtt ) ) &&
            ( WindowAtt.map_state != IsViewable                         ) )
    {
      if ( ( XGetWindowAttributes( DisplayId, TopWindow, &WindowAtt ) ) && 
           ( WindowAtt.map_state != IsViewable                        ) ) 

          break;

      XtAppNextEvent( Context, &MouseEvent );
      XtDispatchEvent( &MouseEvent );
    }
  }

  XmUpdateDisplay( TopShell );

  rdsend();
}
