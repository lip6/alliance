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
# include <Xm/FileSB.h>
# include <Xm/SelectioB.h>
# include <Xm/PushBG.h>

# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "rfm.h"
# include "GRM.h"
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

  GraalDialogItem GraalToolsLoadErrorDialog =

  {
    "Load error file",
    GRAAL_DIALOG_FILE,
    (Widget)NULL,
    (void *)CallbackToolsLoadErrorOk,
    (XtPointer)NULL,
    (void *)CallbackToolsLoadErrorCancel,
    (XtPointer)NULL
  };

  GraalDialogItem GraalToolsSavePixmapDialog =

  {
    "Save xpm file As",
    GRAAL_DIALOG_PROMPT,
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
|                  GraalBuildToolsDialog                      |
|                                                             |
\------------------------------------------------------------*/

void GraalBuildToolsDialog()

{
  rdsbegin();

  GraalBuildDialog( GraalMainWindow, &GraalToolsLoadErrorDialog  );
  GraalBuildDialog( GraalMainWindow, &GraalToolsSavePixmapDialog );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackToolsLoadErrorOk                 |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsLoadErrorOk( MyWidget, ClientData, FileStruct )

    Widget                            MyWidget;
    caddr_t                           ClientData;
    XmFileSelectionBoxCallbackStruct *FileStruct;
{
  char *FileName;

  rdsbegin();

  XtUnmanageChild( GraalToolsLoadErrorDialog.WIDGET );

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
      GraalToolsLoadError( FileName );

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
|                    CallbackToolsLoadErrorCancel             |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsLoadErrorCancel( MyWidget, ClientData, FileStruct )

    Widget                            MyWidget;
    caddr_t                           ClientData;
    XmFileSelectionBoxCallbackStruct *FileStruct;
{
  rdsbegin();

  XtUnmanageChild( GraalToolsLoadErrorDialog.WIDGET );

  GraalExitDialog();

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

  FileName = GraalPostTreatString( FileName );

  XtUnmanageChild( GraalToolsSavePixmapDialog.WIDGET );

  GraalExitDialog();

  GraalToolsSavePixmap( FileName );

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

  XtUnmanageChild( GraalToolsSavePixmapDialog.WIDGET );

  GraalExitDialog();

  rdsend();
}
