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
| File    :                   Panel.c                         |
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
# include <Xm/PushB.h>
# include <Xm/Form.h>
# include <Xm/Frame.h>
 
# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "XSB.h"
# include "XTB.h"
# include "XMF.h"
# include "XME.h"
# include "XMV.h"
# include "XMT.h"
# include "XMS.h"
# include "XMH.h"
# include "XMX.h"

# include "XMX_panel.h"

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
|                     XschInitializePanel                     |
|                                                             |
\------------------------------------------------------------*/

void XschInitializePanel()

{
  autbegin();

  XschBuildPanelLayer();

  XschBuildPanel( XschMainWindow, &XschViewArrowsPanel        );
  XschBuildPanel( XschMainWindow, &XschViewZoomPanel          );
  XschBuildPanel( XschMainWindow, &XschViewGridPanel          );
  XschBuildPanel( XschMainWindow, &XschViewLayerPanel         );
  XschBuildPanel( XschMainWindow, &XschEditSearchViewPanel    );
  XschBuildPanel( XschMainWindow, &XschEditSelectPanel        );
  XschBuildPanel( XschMainWindow, &XschEditIdentifyPanel      );
  XschBuildPanel( XschMainWindow, &XschToolsMessagePanel      );
  XschBuildPanel( XschMainWindow, &XschSetupInformationsPanel );
  XschBuildPanel( XschMainWindow, &XschToolsHierarchyPanel    );
  

  XschBuildPresentPanel();

  autend();
}
