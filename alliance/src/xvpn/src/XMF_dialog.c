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
| Tool    :                   XVPN                            |
|                                                             |
| File    :                 Dialog.c                          |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   04.12.96                        |
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
# include "XSB.h"
# include "XMX.h"
# include "XTB.h"
# include "XMF.h"

# include "XMF_dialog.h"
# include "XMF_file.h"

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

  XvpnDialogItem XvpnFileOpenDialog =

  {
    "Open File",
    XVPN_DIALOG_FILE,
    (Widget)NULL,
    (void *)CallbackFileOpenOk,
    (XtPointer)NULL,
    (void *)CallbackFileOpenCancel,
    (XtPointer)NULL
  };

  XvpnDialogItem XvpnFileQuitDialog =

  {
    "Do you really want to quit Xvpn ?",
    XVPN_DIALOG_WARNING,
    (Widget)NULL,
    (void *)CallbackFileQuitOk,
    (XtPointer)NULL,
    (void *)CallbackFileQuitCancel,
    (XtPointer)NULL
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  XvpnBuildFileDialog                       |
|                                                             |
\------------------------------------------------------------*/

void XvpnBuildFileDialog()

{
  autbegin();

  XvpnBuildDialog( XvpnMainWindow, &XvpnFileOpenDialog   );
  XvpnBuildDialog( XvpnMainWindow, &XvpnFileQuitDialog   );

  autend();
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
  autbegin();

  XvpnExitDialog();

  XtCloseDisplay( XtDisplay( XtParent( MyWidget ) ) );
  XvpnExitErrorMessage( 0 );

  autend();
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
  autbegin();

  XvpnExitDialog();

  autend();
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

  autbegin();

  XtUnmanageChild( XvpnFileOpenDialog.WIDGET );

  XvpnExitDialog();

  if ( FileStruct->value != NULL )
  {
    XmStringGetLtoR( FileStruct->value,
                     XmSTRING_DEFAULT_CHARSET,
                     &FileName 
                   );

    FileName = XvpnPostTreatString( FileName ); 

    if ( FileName != (char *)NULL )
    {
      XvpnFileOpen( FileName );

      XvpnChangeEditMode( XVPN_EDIT_IDENTIFY, 
                           XvpnPromptEditIdentify );
    }
    else
    {
      XvpnErrorMessage( XvpnMainWindow, "Unable to load this file !" );
    }
  }

  autend();
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
  autbegin();

  XtUnmanageChild( XvpnFileOpenDialog.WIDGET );

  XvpnExitDialog();

  autend();
}
