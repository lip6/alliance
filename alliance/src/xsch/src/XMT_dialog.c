/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
| Tool    :                   XSCH                            |
|                                                             |
| File    :                 Dialog.c                          |
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
# include <string.h>
# include <Xm/Xm.h>
# include <Xm/FileSB.h>
# include <Xm/SelectioB.h>
# include <Xm/PushBG.h>

# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "scl.h"
# include "XSB.h"
# include "XSC.h"
# include "XMX.h"
# include "XTB.h"
# include "XMT.h"

# include "XMT_dialog.h"
# include "XMT_tools.h"

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

  XschDialogItem XschToolsSaveXfigDialog =

  {
    "Save Xfig file As",
    XSCH_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackToolsSaveXfigOk,
    (XtPointer)NULL,
    (void *)CallbackToolsSaveXfigCancel,
    (XtPointer)NULL
  };

/*------------------------------------------------------------\
|                                                             |
|                          Function                           |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  XschBuildToolsDialog                      |
|                                                             |
\------------------------------------------------------------*/

void XschBuildToolsDialog()

{
  autbegin();

  XschBuildDialog( XschMainWindow, &XschToolsSaveXfigDialog );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                  CallbackToolsSaveXfigOk                   |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsSaveXfigOk( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *FileName;

  autbegin();

  XmStringGetLtoR(  CallData->value, 
                    XmSTRING_DEFAULT_CHARSET, 
                   &FileName );

  FileName = XschPostTreatString( FileName );

  XtUnmanageChild( XschToolsSaveXfigDialog.WIDGET );

  XschExitDialog();

  XschToolsSaveXfig( FileName );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                CallbackToolsSaveXfigCancel                  |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsSaveXfigCancel( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  autbegin();

  XtUnmanageChild( XschToolsSaveXfigDialog.WIDGET );

  XschExitDialog();

  autend();
}
