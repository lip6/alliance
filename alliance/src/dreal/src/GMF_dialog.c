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
# include "GMF.h"

# include "GMF_dialog.h"
# include "GMF_file.h"

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

  DrealDialogItem DrealFileOpenDialog =

  {
    "Open File",
    DREAL_DIALOG_FILE,
    (Widget)NULL,
    (void *)CallbackFileOpenOk,
    (XtPointer)NULL,
    (void *)CallbackFileOpenCancel,
    (XtPointer)NULL
  };

  DrealDialogItem DrealFileQuitDialog =

  {
    "Do you really want to quit Dreal ?",
    DREAL_DIALOG_WARNING,
    (Widget)NULL,
    (void *)CallbackFileQuitOk,
    (XtPointer)NULL,
    (void *)CallbackFileQuitCancel,
    (XtPointer)NULL
  };

  DrealDialogItem DrealFileSaveAsDialog =

  {
    "Save As",
    DREAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackFileSaveAsOk,
    (XtPointer)NULL,
    (void *)CallbackFileSaveAsCancel,
    (XtPointer)NULL
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  DrealBuildFileDialog                       |
|                                                             |
\------------------------------------------------------------*/

void DrealBuildFileDialog()

{
  rdsbegin();

  DrealBuildDialog( DrealMainWindow, &DrealFileSaveAsDialog );
  DrealBuildDialog( DrealMainWindow, &DrealFileOpenDialog   );
  DrealBuildDialog( DrealMainWindow, &DrealFileQuitDialog   );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  CallbackFileQuitOk                         |
|                                                             |
\------------------------------------------------------------*/

void CallbackFileQuitOk( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    caddr_t                       CallData;
{
  rdsbegin();

  DrealExitDialog();

  if ( DrealHeadUndo != (drealundo *)NULL )
  {
    CallbackFileSaveAs( NULL, NULL, NULL );
    DrealHeadUndo = (drealundo *)0;
  }

  XtCloseDisplay( XtDisplay( XtParent( MyWidget ) ) );

  DrealExitErrorMessage( 0 );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  CallbackFileSaveAsOk                       |
|                                                             |
\------------------------------------------------------------*/

void CallbackFileSaveAsOk( MyWidget, ClientData, CallData )

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

  XtUnmanageChild( DrealFileSaveAsDialog.WIDGET );

  DrealExitDialog();

  DrealFileSaveAs( FileName );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  CallbackFileSaveAsCancel                   |
|                                                             |
\------------------------------------------------------------*/

void CallbackFileSaveAsCancel( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( DrealFileSaveAsDialog.WIDGET );

  DrealExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  CallbackFileQuitCancel                     |
|                                                             |
\------------------------------------------------------------*/

void CallbackFileQuitCancel( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    caddr_t                       CallData;
{
  rdsbegin();

  DrealExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackFileOpenOk                       |
|                                                             |
\------------------------------------------------------------*/

void CallbackFileOpenOk( MyWidget, ClientData, FileStruct )

    Widget                            MyWidget;
    caddr_t                           ClientData;
    XmFileSelectionBoxCallbackStruct *FileStruct;
{
  char *FileName;

  rdsbegin();

  XtUnmanageChild( DrealFileOpenDialog.WIDGET );

  DrealExitDialog();

  if ( FileStruct->value != NULL )
  {
    XmStringGetLtoR( FileStruct->value,
                     XmSTRING_DEFAULT_CHARSET,
                     &FileName 
                   );

    FileName = DrealPostTreatString( FileName ); 

    if ( FileName != (char *)NULL )
    {
      if ( DrealHeadUndo != (drealundo *)NULL )
      {
        CallbackFileSaveAs( NULL, NULL, NULL );
      }

      DrealFileOpen( FileName );

      DrealChangeEditMode( DREAL_EDIT_MEASURE, 
                           DrealPromptEditMeasure );
    }
    else
    {
      DrealErrorMessage( DrealMainWindow, "Unable to load this file !" );
    }
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackFileOpenCancel                   |
|                                                             |
\------------------------------------------------------------*/

void CallbackFileOpenCancel( MyWidget, ClientData, FileStruct )

    Widget                            MyWidget;
    caddr_t                           ClientData;
    XmFileSelectionBoxCallbackStruct *FileStruct;
{
  rdsbegin();

  XtUnmanageChild( DrealFileOpenDialog.WIDGET );

  DrealExitDialog();

  rdsend();
}
