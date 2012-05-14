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
| File    :                  Tools.c                          |
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
# include <Xm/Xm.h>
# include <Xm/PushBG.h>
# ifdef HAVE_XPM
# include <X11/xpm.h>
# endif

# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rfm.h"
# include "rpr.h"
# include "GTB.h"
# include "GSB.h"
# include "GMX.h"
# include "GRD.h"
# include "GMT.h"

# include "GMT_tools.h"
# include "GMT_message.h"

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
|                     DrealToolsFlatten                       |
|                                                             |
\------------------------------------------------------------*/

void DrealToolsFlatten()

{
  rdsbegin();

  if ( DrealFigureRds != (rdsfig_list *)NULL )
  {
    DrealSetMouseCursor( DrealGraphicWindow, DREAL_WATCH_CURSOR  );
    DrealFlattenFigure();
    DrealComputeBound();
    DrealZoomRefresh();
    DrealDisplayToolsMessage();
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        DrealToolsSavePixmap                 |
|                                                             |
\------------------------------------------------------------*/

void DrealToolsSavePixmap( FileName )

  char *FileName;
{
# ifdef HAVE_XPM
  XpmAttributes Attribute;
  char         *XpmFileName;
  int           Error;

  XpmFileName = rdsallocblock( strlen( FileName ) + 4 );
  sprintf( XpmFileName, "%s.xpm", FileName );

  Attribute.colormap  = DrealColorMap;
  Attribute.valuemask = XpmColormap;

  DrealSetMouseCursor( DrealGraphicWindow, DREAL_WATCH_CURSOR  );

  Error = XpmWriteFileFromPixmap( DrealGraphicDisplay,
                                  XpmFileName,
                                  DrealGraphicPixmap,
                                  (Pixmap)0, &Attribute );

  DrealSetMouseCursor( DrealGraphicWindow, DREAL_NORMAL_CURSOR  );

  if ( Error )
  {
    DrealErrorMessage( DrealMainWindow, "Unable to save xpm file !" );
  }

  rdsfreeblock( XpmFileName );
# else

  DrealErrorMessage( DrealMainWindow, "XPM cannot be used: Not enabled at compile time !" );

# endif
}
