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
| Tool    :                   DREAL                           |
|                                                             |
| File    :                 Dialog.c                          |
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
# include "GSB.h"
# include "GRD.h"
# include "GMX.h"
# include "GTB.h"
# include "GME.h"

# include "GME_dialog.h"
# include "GME_modify.h"
# include "GME_search.h"
# include "GME_edit.h"

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
|                         Search Dialog                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Search Rectangle Dialog                |
|                                                             |
\------------------------------------------------------------*/

  DrealDialogItem DrealSearchRectangleDialog =

  {
    "Rectangle name",
    DREAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackSearchRectangleOk,
    (XtPointer)NULL,
    (void *)CallbackSearchRectangleCancel,
    (XtPointer)NULL
  };

/*------------------------------------------------------------\
|                                                             |
|                        Modify Dialog                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Rectangle Dialog                    |
|                                                             |
\------------------------------------------------------------*/
 
  DrealDialogItem DrealModifyRectangleNameDialog =

  {
    "Rectangle name",
    DREAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackModifyRectangleNameOk,
    (XtPointer)NULL,
    (void *)CallbackModifyRectangleNameCancel,
    (XtPointer)NULL
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     DrealBuildEditDialog                    |
|                                                             |
\------------------------------------------------------------*/

void DrealBuildEditDialog()

{
  rdsbegin();

  DrealBuildDialog( DrealMainWindow, &DrealSearchRectangleDialog     );
  DrealBuildDialog( DrealMainWindow, &DrealModifyRectangleNameDialog );
  DrealBuildDialog( DrealMainWindow, &DrealSearchRectangleDialog     );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      Callback For Dialog                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Callback For Modify                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   CallbackModifyRectangleNameOk             |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyRectangleNameOk( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *NameSet;

  rdsbegin();

  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &NameSet );

  XtUnmanageChild( DrealModifyRectangleNameDialog.WIDGET );

  DrealExitDialog();

  NameSet = DrealPostTreatString( NameSet );

  if ( NameSet != (char *)NULL )
  {
    NameSet = namealloc( NameSet );
  }

  DrealRectangleMName = NameSet;

  DrealPromptModifyRectangle();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 CallbackModifyRectangleNameCancel           |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackModifyRectangleNameCancel( MyWidget, ClientData, CallData )
 
    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( DrealModifyRectangleNameDialog.WIDGET );
 
  DrealExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        Search Callback                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     Callback For Rectangle                  |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     CallbackSearchRectangleOk               |
|                                                             |
\------------------------------------------------------------*/

void CallbackSearchRectangleOk( MyWidget, ClientData, CallData )
 
    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *NameSet;

  rdsbegin();
 
  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &NameSet );
 
  XtUnmanageChild( DrealSearchRectangleDialog.WIDGET );
 
  DrealExitDialog();

  NameSet = DrealPostTreatString( NameSet );

  if ( NameSet != (char *)NULL ) 
  {
    NameSet = namealloc( NameSet );

    DrealEditSearchRectangle( NameSet );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   CallbackSearchRectangleCancel             |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackSearchRectangleCancel( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( DrealSearchRectangleDialog.WIDGET );

  DrealExitDialog();

  rdsend();
}
