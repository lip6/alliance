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
| Tool    :                   XSCH                           |
|                                                             |
| File    :                  Pixmap.c                         |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   01.06.96                        |
|                                                             |
\------------------------------------------------------------*/

# include <Xm/Xm.h>
# include <X11/Intrinsic.h>
# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "XTB.h"
# include "XSB.h"

# include "XTB_pixmap.h"

/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/
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
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     XschCreatePixmap                       |
|                                                             |
\------------------------------------------------------------*/

Pixmap XschCreatePixmap( MainWidget, IconBits, IconWidth, IconHeight )

       Widget  MainWidget;
       char   *IconBits;
       int     IconWidth;
       int     IconHeight;
{
  Pixel  Foreground;
  Pixel  Background;
  Pixmap IconPixmap;

  autbegin();

  XtVaGetValues( MainWidget, XmNforeground, &Foreground, 
                             XmNbackground, &Background, NULL );

  IconPixmap = XCreatePixmapFromBitmapData( XtDisplay( MainWidget ),
                                            RootWindowOfScreen( XtScreen( MainWidget ) ), 
                                            IconBits, IconWidth, IconHeight,
                                            Foreground, Background,
                                            DefaultDepthOfScreen( XtScreen( MainWidget ) ) );

  autend();
  return( IconPixmap );
}

