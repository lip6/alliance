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
| Tool    :                   DREAL                           |
|                                                             |
| File    :                 Dialog.c                          |
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
# include <string.h>
# include <Xm/Xm.h>
# include <Xm/FileSB.h>
# include <Xm/SelectioB.h>
# include <Xm/PushBG.h>

# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "rfm.h"
# include "GRD.h"
# include "GMX.h"
# include "GTB.h"
# include "GSB.h"
# include "GMT.h"

# include "GMT_dialog.h"
# include "GMT_tools.h"

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
|                          File Dialog                        |
|                                                             |
\------------------------------------------------------------*/

  DrealDialogItem DrealToolsSavePixmapDialog =

  {
    "Save xpm file As",
    DREAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackToolsSavePixmapOk,
    (XtPointer)NULL,
    (void *)CallbackToolsSavePixmapCancel,
    (XtPointer)NULL
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  DrealBuildToolsDialog                      |
|                                                             |
\------------------------------------------------------------*/

void DrealBuildToolsDialog()

{
  rdsbegin();

  DrealBuildDialog( DrealMainWindow, &DrealToolsSavePixmapDialog );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  CallbackToolsSavePixmapOk                  |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsSavePixmapOk( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *FileName;

  rdsbegin();

  XmStringGetLtoR(  CallData->value, 
                    XmSTRING_DEFAULT_CHARSET, 
                   &FileName );

  FileName = DrealPostTreatString( FileName );

  XtUnmanageChild( DrealToolsSavePixmapDialog.WIDGET );

  DrealExitDialog();

  DrealToolsSavePixmap( FileName );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                CallbackToolsSavePixmapCancel                |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsSavePixmapCancel( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( DrealToolsSavePixmapDialog.WIDGET );

  DrealExitDialog();

  rdsend();
}
