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
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                   Panel.c                         |
|                                                             |
| Author  :                Jacomme Ludovic                    |
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
# include <Xm/PushB.h>
# include <Xm/Form.h>
# include <Xm/Frame.h>
 
# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "GSB.h"
# include "GTB.h"
# include "GMF.h"
# include "GME.h"
# include "GMC.h"
# include "GMV.h"
# include "GMT.h"
# include "GMS.h"
# include "GMH.h"
# include "GMX.h"

# include "GMX_panel.h"

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
|                    GraalInitializePanel                     |
|                                                             |
\------------------------------------------------------------*/

void GraalInitializePanel()

{
  rdsbegin();

  GraalBuildPanelLayer();
  GraalBuildPanelModify();
  GraalBuildPanelCreate();

  GraalBuildPanel( GraalMainWindow, &GraalToolsGlobalPanel        );
  GraalBuildPanel( GraalMainWindow, &GraalToolsMessagePanel       );
  GraalBuildPanel( GraalMainWindow, &GraalToolsDrucPanel          );
  GraalBuildPanel( GraalMainWindow, &GraalToolsHierarchyPanel     );
  GraalBuildPanel( GraalMainWindow, &GraalToolsHierarchyShowPanel );

  GraalBuildPanel( GraalMainWindow, &GraalViewArrowsPanel        );
  GraalBuildPanel( GraalMainWindow, &GraalViewZoomPanel          );
  GraalBuildPanel( GraalMainWindow, &GraalViewLayerPanel         );
  GraalBuildPanel( GraalMainWindow, &GraalViewGridPanel          );

  GraalBuildPanel( GraalMainWindow, &GraalCreateSegmentPanel     );
  GraalBuildPanel( GraalMainWindow, &GraalCreateViaPanel         );
  GraalBuildPanel( GraalMainWindow, &GraalCreateBigViaPanel      );
  GraalBuildPanel( GraalMainWindow, &GraalCreateTransistorPanel  );
  GraalBuildPanel( GraalMainWindow, &GraalCreateConnectorPanel   );
  GraalBuildPanel( GraalMainWindow, &GraalCreateReferencePanel   );
  GraalBuildPanel( GraalMainWindow, &GraalCreateInstancePanel    );

  GraalBuildPanel( GraalMainWindow, &GraalEditGlobalPanel        );
  GraalBuildPanel( GraalMainWindow, &GraalWindowGlobalPanel      );
  GraalBuildPanel( GraalMainWindow, &GraalEditSelectPanel        );
  GraalBuildPanel( GraalMainWindow, &GraalEditSearchPanel        );
  GraalBuildPanel( GraalMainWindow, &GraalEditSearchViewPanel    );
  GraalBuildPanel( GraalMainWindow, &GraalEditIdentifyPanel      );

  GraalBuildPanel( GraalMainWindow, &GraalModifySegmentPanel     );
  GraalBuildPanel( GraalMainWindow, &GraalModifyConnectorPanel   );
  GraalBuildPanel( GraalMainWindow, &GraalModifyTransistorPanel  );
  GraalBuildPanel( GraalMainWindow, &GraalModifyReferencePanel   );
  GraalBuildPanel( GraalMainWindow, &GraalModifyViaPanel         );
  GraalBuildPanel( GraalMainWindow, &GraalModifyBigViaPanel      );
  GraalBuildPanel( GraalMainWindow, &GraalModifyInstancePanel    );

  GraalBuildPanel( GraalMainWindow, &GraalSetupInformationsPanel );

  GraalBuildPresentPanel();

  rdsend();
}
