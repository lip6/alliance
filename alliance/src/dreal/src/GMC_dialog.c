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

# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "rfm.h"
# include "GRD.h"
# include "GMX.h"
# include "GTB.h"
# include "GSB.h"
# include "GMC.h"

# include "GMC_dialog.h"
# include "GMC_create.h"
# include "GMC_message.h"

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

  char DrealCreateDialogCancel = 1;

/*------------------------------------------------------------\
|                                                             |
|                        Create Dialog                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Rectangle Dialog                    |
|                                                             |
\------------------------------------------------------------*/
 
  DrealDialogItem DrealChangeRectangleNameDialog =

  {
    "Rectangle name",
    DREAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackChangeRectangleNameOk,
    (XtPointer)NULL,
    (void *)CallbackChangeRectangleNameCancel,
    (XtPointer)NULL
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    DrealBuildCreateDialog                   |
|                                                             |
\------------------------------------------------------------*/

void DrealBuildCreateDialog()

{
  rdsbegin();

  DrealBuildDialog( DrealMainWindow, &DrealChangeRectangleNameDialog );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      Callback For Dialog                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 CallbackChangeRectangleNameOk               |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeRectangleNameOk( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *NameSet;

  rdsbegin();

  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &NameSet );

  XtUnmanageChild( DrealChangeRectangleNameDialog.WIDGET );

  DrealExitDialog();

  DrealCreateDialogCancel = 0;

  NameSet = DrealPostTreatString( NameSet );

  if ( NameSet != (char *)NULL )
  {
    NameSet = namealloc( NameSet );
  }

  DrealRectangleName = NameSet;

  DrealChangeEditMode( DREAL_CREATE_RECTANGLE,
                       DrealPromptCreateRectangle ); 
  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 CallbackChangeRectangleNameCancel           |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackChangeRectangleNameCancel( MyWidget, ClientData, CallData )
 
    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( DrealChangeRectangleNameDialog.WIDGET );
 
  DrealExitDialog();

  DrealCreateDialogCancel = 1;

  rdsend();
}
