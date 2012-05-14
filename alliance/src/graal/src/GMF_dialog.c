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
# include <Xm/Form.h>
# include <Xm/LabelG.h>
# include <Xm/Label.h>
# include <Xm/Frame.h>
# include <Xm/List.h>
# include <Xm/Text.h>
# include <Xm/PushB.h>
# include <Xm/FileSB.h>

# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "rfm.h"
# include "GRM.h"
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

  GraalDialogItem GraalFileOpenDialog =

  {
    "Open File",
    GRAAL_DIALOG_FILE,
    (Widget)NULL,
    (void *)CallbackFileOpenOk,
    (XtPointer)NULL,
    (void *)CallbackFileOpenCancel,
    (XtPointer)NULL
  };

  GraalDialogItem GraalFileQuitDialog =

  {
    "Do you really want to quit Graal ?",
    GRAAL_DIALOG_WARNING,
    (Widget)NULL,
    (void *)CallbackFileQuitOk,
    (XtPointer)NULL,
    (void *)CallbackFileQuitCancel,
    (XtPointer)NULL
  };

  GraalDialogItem GraalFileSaveAsDialog =

  {
    "Save As",
    GRAAL_DIALOG_PROMPT,
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
|                  GraalBuildFileDialog                       |
|                                                             |
\------------------------------------------------------------*/

void GraalBuildFileDialog()

{
  rdsbegin();

  GraalBuildDialog( GraalMainWindow, &GraalFileSaveAsDialog );
  GraalBuildDialog( GraalMainWindow, &GraalFileOpenDialog   );
  GraalBuildDialog( GraalMainWindow, &GraalFileQuitDialog   );

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

  GraalExitDialog();

  if ( GraalHeadUndo != (graalundo *)NULL )
  { 
    CallbackFileSaveAs( NULL, NULL, NULL );
    GraalHeadUndo = (graalundo *)0;
  }

  XtCloseDisplay( XtDisplay( XtParent( MyWidget ) ) );
  GraalExitErrorMessage( 0 );

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

  GraalExitDialog();

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

  FileName = GraalPostTreatString( FileName );

  XtUnmanageChild( GraalFileSaveAsDialog.WIDGET );

  GraalExitDialog();

  GraalFileSaveAs( FileName );

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

  XtUnmanageChild( GraalFileSaveAsDialog.WIDGET );

  GraalExitDialog();

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

  XtUnmanageChild( GraalFileOpenDialog.WIDGET );

  GraalExitDialog();

  if ( FileStruct->value != NULL )
  {
    XmStringGetLtoR( FileStruct->value,
                     XmSTRING_DEFAULT_CHARSET,
                     &FileName 
                   );

    FileName = GraalPostTreatString( FileName ); 

    if ( FileName != (char *)NULL )
    {
      if ( GraalHeadUndo != (graalundo *)NULL )
      {
        CallbackFileSaveAs( NULL, NULL, NULL );
      }

      GraalFileOpen( FileName, 1 );

      GraalChangeEditMode( GRAAL_EDIT_MEASURE, 
                           GraalPromptEditMeasure );
    }
    else
    {
      GraalErrorMessage( GraalMainWindow, "Unable to load this file !" );
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

  XtUnmanageChild( GraalFileOpenDialog.WIDGET );

  GraalExitDialog();

  rdsend();
}
