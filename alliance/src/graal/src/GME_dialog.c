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
# include <stdlib.h>
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
# include "GRM.h"
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
|                    Search Connector Dialog                  |
|                                                             |
\------------------------------------------------------------*/

  GraalDialogItem GraalSearchConnectorDialog =

  {
    "Connector name",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackSearchConnectorOk,
    (XtPointer)NULL,
    (void *)CallbackSearchConnectorCancel,
    (XtPointer)NULL
  };

/*------------------------------------------------------------\
|                                                             |
|                    Search Instance  Dialog                  |
|                                                             |
\------------------------------------------------------------*/

  GraalDialogItem GraalSearchInstanceDialog =

  {
    "Instance name",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackSearchInstanceOk,
    (XtPointer)NULL,
    (void *)CallbackSearchInstanceCancel,
    (XtPointer)NULL
  };

/*------------------------------------------------------------\
|                                                             |
|                    Search Via Dialog                        |
|                                                             |
\------------------------------------------------------------*/

  GraalDialogItem GraalSearchViaDialog =

  {
    "Via name",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackSearchViaOk,
    (XtPointer)NULL,
    (void *)CallbackSearchViaCancel,
    (XtPointer)NULL
  };

/*------------------------------------------------------------\
|                                                             |
|                    Search Reference Dialog                  |
|                                                             |
\------------------------------------------------------------*/

  GraalDialogItem GraalSearchReferenceDialog =

  {
    "Reference name",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackSearchReferenceOk,
    (XtPointer)NULL,
    (void *)CallbackSearchReferenceCancel,
    (XtPointer)NULL
  };

/*------------------------------------------------------------\
|                                                             |
|                      Search Segment Dialog                  |
|                                                             |
\------------------------------------------------------------*/

  GraalDialogItem GraalSearchSegmentDialog =

  {
    "Segment name",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackSearchSegmentOk,
    (XtPointer)NULL,
    (void *)CallbackSearchSegmentCancel,
    (XtPointer)NULL
  };

/*------------------------------------------------------------\
|                                                             |
|                        Modify Dialog                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Segment Dialog                      |
|                                                             |
\------------------------------------------------------------*/
 
  GraalDialogItem GraalModifySegmentWidthDialog =

  {
    "Segment width",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackModifySegmentWidthOk,
    (XtPointer)NULL,
    (void *)CallbackModifySegmentWidthCancel,
    (XtPointer)NULL
  };

  GraalDialogItem GraalModifySegmentNameDialog =

  {
    "Segment name",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackModifySegmentNameOk,
    (XtPointer)NULL,
    (void *)CallbackModifySegmentNameCancel,
    (XtPointer)NULL
  };

/*------------------------------------------------------------\
|                                                             |
|                       Transistor Dialog                     |
|                                                             |
\------------------------------------------------------------*/

  GraalDialogItem GraalModifyTransistorWidthDialog =

  {
    "Transistor length",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackModifyTransistorWidthOk,
    (XtPointer)NULL,
    (void *)CallbackModifyTransistorWidthCancel,
    (XtPointer)NULL
  };

  GraalDialogItem GraalModifyTransistorNameDialog =

  {
    "Transistor name",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackModifyTransistorNameOk,
    (XtPointer)NULL,
    (void *)CallbackModifyTransistorNameCancel,
    (XtPointer)NULL
  };

/*------------------------------------------------------------\
|                                                             |
|                        Connector Dialog                     |
|                                                             |
\------------------------------------------------------------*/

  GraalDialogItem GraalModifyConnectorWidthDialog =

  {
    "Connector width",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackModifyConnectorWidthOk,
    (XtPointer)NULL,
    (void *)CallbackModifyConnectorWidthCancel,
    (XtPointer)NULL
  };
    
  GraalDialogItem GraalModifyConnectorNameDialog =
 
  {
    "Connector name",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackModifyConnectorNameOk,
    (XtPointer)NULL,
    (void *)CallbackModifyConnectorNameCancel,
    (XtPointer)NULL
  };

/*------------------------------------------------------------\
|                                                             |
|                        Reference Dialog                     |
|                                                             |
\------------------------------------------------------------*/

  GraalDialogItem GraalModifyReferenceNameDialog =

  {
    "Reference name",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackModifyReferenceNameOk,
    (XtPointer)NULL,
    (void *)CallbackModifyReferenceNameCancel,
    (XtPointer)NULL
  };

/*------------------------------------------------------------\
|                                                             |
|                        Instance  Dialog                     |
|                                                             |
\------------------------------------------------------------*/

  GraalDialogItem GraalModifyInstanceNameDialog =

  {
    "Instance name",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackModifyInstanceNameOk,
    (XtPointer)NULL,
    (void *)CallbackModifyInstanceNameCancel,
    (XtPointer)NULL
  };

  GraalDialogItem GraalModifyInstanceModelDialog =

  {
    "Instance model",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackModifyInstanceModelOk,
    (XtPointer)NULL,
    (void *)CallbackModifyInstanceModelCancel,
    (XtPointer)NULL
  };

/*------------------------------------------------------------\
|                                                             |
|                             Via  Dialog                     |
|                                                             |
\------------------------------------------------------------*/

  GraalDialogItem GraalModifyViaNameDialog =

  {
    "Via name",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackModifyViaNameOk,
    (XtPointer)NULL,
    (void *)CallbackModifyViaNameCancel,
    (XtPointer)NULL
  };

  GraalDialogItem GraalModifyBigViaNameDialog =

  {
    "BigVia name",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackModifyBigViaNameOk,
    (XtPointer)NULL,
    (void *)CallbackModifyBigViaNameCancel,
    (XtPointer)NULL
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     GraalBuildEditDialog                    |
|                                                             |
\------------------------------------------------------------*/

void GraalBuildEditDialog()

{
  rdsbegin();

  GraalBuildDialog( GraalMainWindow, &GraalSearchConnectorDialog        );
  GraalBuildDialog( GraalMainWindow, &GraalSearchInstanceDialog         );
  GraalBuildDialog( GraalMainWindow, &GraalSearchReferenceDialog        );
  GraalBuildDialog( GraalMainWindow, &GraalSearchSegmentDialog          );
  GraalBuildDialog( GraalMainWindow, &GraalModifySegmentWidthDialog     );
  GraalBuildDialog( GraalMainWindow, &GraalModifySegmentNameDialog      );
  GraalBuildDialog( GraalMainWindow, &GraalModifyViaNameDialog          );
  GraalBuildDialog( GraalMainWindow, &GraalModifyBigViaNameDialog       );
  GraalBuildDialog( GraalMainWindow, &GraalModifyTransistorWidthDialog  );
  GraalBuildDialog( GraalMainWindow, &GraalModifyTransistorNameDialog   );
  GraalBuildDialog( GraalMainWindow, &GraalModifyConnectorWidthDialog   );
  GraalBuildDialog( GraalMainWindow, &GraalModifyConnectorNameDialog    );
  GraalBuildDialog( GraalMainWindow, &GraalModifyReferenceNameDialog    );
  GraalBuildDialog( GraalMainWindow, &GraalModifyInstanceNameDialog     );
  GraalBuildDialog( GraalMainWindow, &GraalModifyInstanceModelDialog    );
  GraalBuildDialog( GraalMainWindow, &GraalSearchConnectorDialog        );
  GraalBuildDialog( GraalMainWindow, &GraalSearchInstanceDialog         );
  GraalBuildDialog( GraalMainWindow, &GraalSearchReferenceDialog        );
  GraalBuildDialog( GraalMainWindow, &GraalSearchSegmentDialog          );
  GraalBuildDialog( GraalMainWindow, &GraalSearchViaDialog              );

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
|                  CallbackModifySegmentWidthOk               |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifySegmentWidthOk( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *WidthSet;
  float NewWidth;
  float Factor;

  rdsbegin();

  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &WidthSet );

  XtUnmanageChild( GraalModifySegmentWidthDialog.WIDGET );

  GraalExitDialog();

  if ( WidthSet != (char *)NULL )
  {
    Factor   = RDS_LAMBDA / RDS_PHYSICAL_GRID;
    NewWidth = atof( WidthSet );
    NewWidth = (float)( (long)( NewWidth * Factor / 2.0 ) * 2 / Factor );

    if ( NewWidth >=  GRAAL_SEGMENT_VALUE_TABLE[ GraalSegmentMLayer ][0] )    
    {
      GraalSegmentMWidth = NewWidth;
    }
    else
    {  
      GraalErrorMessage( GraalMainWindow, "Incorrect segment width !" );
    }

    GraalPromptModifySegment();
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                CallbackModifySegmentWidthCancel             |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackModifySegmentWidthCancel( MyWidget, ClientData, CallData )
 
    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( GraalModifySegmentWidthDialog.WIDGET );
 
  GraalExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   CallbackModifySegmentNameOk               |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifySegmentNameOk( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *NameSet;

  rdsbegin();

  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &NameSet );

  XtUnmanageChild( GraalModifySegmentNameDialog.WIDGET );

  GraalExitDialog();

  NameSet = GraalPostTreatString( NameSet );

  if ( NameSet != (char *)NULL )
  {
    NameSet = namealloc( NameSet );
  }
  else
  if ( ( GraalSegmentMLayer >= CALU1 ) &&
       ( GraalSegmentMLayer <= CALU9 ) )
  {
    NameSet = GraalSegmentMName;
    GraalErrorMessage( GraalMainWindow, "Connector must have name !" );
  }

  GraalSegmentMName = NameSet;

  GraalPromptModifySegment();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 CallbackModifySegmentNameCancel             |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackModifySegmentNameCancel( MyWidget, ClientData, CallData )
 
    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( GraalModifySegmentNameDialog.WIDGET );
 
  GraalExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    Callback For Transistor                  |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 CallbackModifyTransistorWidthOk             |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyTransistorWidthOk( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *WidthSet;
  float NewWidth;
  float Factor;

  rdsbegin();

  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &WidthSet );

  XtUnmanageChild( GraalModifyTransistorWidthDialog.WIDGET );

  GraalExitDialog();

  if ( WidthSet != (char *)NULL )
  {
    Factor   = RDS_LAMBDA / RDS_PHYSICAL_GRID;
    NewWidth = atof( WidthSet );
    NewWidth = (float)( (long)( NewWidth * Factor / 2.0 ) * 2 / Factor );

    if ( NewWidth >= GRAAL_SEGMENT_VALUE_TABLE[ GraalTransistorMType ][0] )
    {
      GraalTransistorMWidth = NewWidth;
    }
    else
    {   
      GraalErrorMessage( GraalMainWindow, "Incorrect transistor length!" );
    }

    GraalPromptModifyTransistor();
  }

  rdsend();
}  

/*------------------------------------------------------------\
|                                                             |
|                CallbackModifyTransistorWidthCancel          |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackModifyTransistorWidthCancel( MyWidget, ClientData, CallData )
 
    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( GraalModifyTransistorWidthDialog.WIDGET );

  GraalExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   CallbackModifyTransistorNameOk            |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyTransistorNameOk( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *NameSet;

  rdsbegin();

  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &NameSet );

  XtUnmanageChild( GraalModifyTransistorNameDialog.WIDGET );

  GraalExitDialog();

  NameSet = GraalPostTreatString( NameSet );

  if ( NameSet != (char *)NULL )
  {
    NameSet = namealloc( NameSet );
  }

  GraalTransistorMName = NameSet;

  GraalPromptModifyTransistor();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 CallbackModifyTransistorNameCancel          |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackModifyTransistorNameCancel( MyWidget, ClientData, CallData )
 
    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( GraalModifyTransistorNameDialog.WIDGET );
 
  GraalExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Callback For Connector                  |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 CallbackModifyConnectorWidthOk              |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyConnectorWidthOk( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *WidthSet;
  float NewWidth;
  float Factor;

  rdsbegin();
 
  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &WidthSet );
 
  XtUnmanageChild( GraalModifyConnectorWidthDialog.WIDGET );
 
  GraalExitDialog();
 
  if ( WidthSet != (char *)NULL )
  {
    Factor   = RDS_LAMBDA / RDS_PHYSICAL_GRID;
    NewWidth = atof( WidthSet );
    NewWidth = (float)( (long)( NewWidth * Factor / 2.0 ) * 2 / Factor );

    if ( NewWidth >= GRAAL_SEGMENT_VALUE_TABLE[ GraalConnectorMLayer ][0] )
    {
      GraalConnectorMWidth = NewWidth;
    }
    else
    {   
      GraalErrorMessage( GraalMainWindow, "Incorrect connector width !" );
    }
 
    GraalPromptModifyConnector();
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|               CallbackModifyConnectorWidthCancel            |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyConnectorWidthCancel( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( GraalModifyConnectorWidthDialog.WIDGET );

  GraalExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  CallbackModifyConnectorNameOk              |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyConnectorNameOk( MyWidget, ClientData, CallData )
 
    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *NameSet;

  rdsbegin();
 
  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &NameSet );
 
  XtUnmanageChild( GraalModifyConnectorNameDialog.WIDGET );
 
  GraalExitDialog();

  NameSet = GraalPostTreatString( NameSet );
 
  if ( NameSet != (char *)NULL )
  {
    NameSet = namealloc( NameSet );

    GraalConnectorMName = NameSet;

    GraalPromptModifyConnector();
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                CallbackModifyConnectorNameCancel            |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackModifyConnectorNameCancel( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( GraalModifyConnectorNameDialog.WIDGET );

  GraalExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Callback For Reference                  |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  CallbackModifyReferenceNameOk              |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyReferenceNameOk( MyWidget, ClientData, CallData )
 
    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *NameSet;

  rdsbegin();
 
  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &NameSet );
 
  XtUnmanageChild( GraalModifyReferenceNameDialog.WIDGET );
 
  GraalExitDialog();

  NameSet = GraalPostTreatString( NameSet );

  if ( NameSet != (char *)NULL ) 
  {
    NameSet = namealloc( NameSet );

    GraalReferenceMName = NameSet;

    GraalPromptModifyReference();
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                CallbackModifyReferenceNameCancel            |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackModifyReferenceNameCancel( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( GraalModifyReferenceNameDialog.WIDGET );

  GraalExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Callback For Instance                   |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   CallbackModifyInstanceNameOk              |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyInstanceNameOk( MyWidget, ClientData, CallData )
 
    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *NameSet;

  rdsbegin();
 
  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &NameSet );
 
  XtUnmanageChild( GraalModifyInstanceNameDialog.WIDGET );
 
  GraalExitDialog();

  NameSet = GraalPostTreatString( NameSet );

  if ( NameSet != (char *)NULL ) 
  {
    NameSet = namealloc( NameSet );

    GraalInstanceMName = NameSet;
  }

  GraalPromptModifyInstance();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 CallbackModifyInstanceNameCancel            |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackModifyInstanceNameCancel( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( GraalModifyInstanceNameDialog.WIDGET );

  GraalExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  CallbackModifyInstanceModelOk              |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyInstanceModelOk( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *ModelSet;

  rdsbegin();

  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &ModelSet );

  XtUnmanageChild( GraalModifyInstanceModelDialog.WIDGET );

  GraalExitDialog();

  ModelSet = GraalPostTreatString( ModelSet );

  if ( ModelSet != (char *)NULL )
  {
    ModelSet = namealloc( ModelSet );

    GraalInstanceMModel = ModelSet;
  }

  GraalPromptModifyInstance();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                CallbackModifyInstanceModelCancel            |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyInstanceModelCancel( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( GraalModifyInstanceModelDialog.WIDGET );

  GraalExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   CallbackModifyViaNameOk                   |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyViaNameOk( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *NameSet;

  rdsbegin();

  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &NameSet );

  XtUnmanageChild( GraalModifyViaNameDialog.WIDGET );

  GraalExitDialog();

  NameSet = GraalPostTreatString( NameSet );

  if ( NameSet != (char *)NULL )
  {
    NameSet = namealloc( NameSet );
  }

  GraalViaMName = NameSet;

  GraalPromptModifyVia();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     CallbackModifyViaNameCancel             |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackModifyViaNameCancel( MyWidget, ClientData, CallData )
 
    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( GraalModifyViaNameDialog.WIDGET );
 
  GraalExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 CallbackModifyBigViaNameOk                  |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyBigViaNameOk( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *NameSet;

  rdsbegin();

  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &NameSet );

  XtUnmanageChild( GraalModifyBigViaNameDialog.WIDGET );

  GraalExitDialog();

  NameSet = GraalPostTreatString( NameSet );

  if ( NameSet != (char *)NULL )
  {
    NameSet = namealloc( NameSet );
  }

  GraalViaMName = NameSet;

  GraalPromptModifyBigVia();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   CallbackModifyBigViaNameCancel            |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackModifyBigViaNameCancel( MyWidget, ClientData, CallData )
 
    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( GraalModifyBigViaNameDialog.WIDGET );
 
  GraalExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        Search Callback                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     Callback For Connector                  |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   CallbackSearchConnectorOk                 |
|                                                             |
\------------------------------------------------------------*/

void CallbackSearchConnectorOk( MyWidget, ClientData, CallData )
 
    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *NameSet;

  rdsbegin();
 
  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &NameSet );
 
  XtUnmanageChild( GraalSearchConnectorDialog.WIDGET );
 
  GraalExitDialog();

  NameSet = GraalPostTreatString( NameSet );

  if ( NameSet != (char *)NULL ) 
  {
    NameSet = namealloc( NameSet );

    GraalEditSearchConnector( NameSet );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 CallbackSearchConnectorCancel               |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackSearchConnectorCancel( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( GraalSearchConnectorDialog.WIDGET );

  GraalExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Callback For Instance                   |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    CallbackSearchInstanceOk                 |
|                                                             |
\------------------------------------------------------------*/

void CallbackSearchInstanceOk( MyWidget, ClientData, CallData )
 
    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *NameSet;

  rdsbegin();
 
  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &NameSet );
 
  XtUnmanageChild( GraalSearchInstanceDialog.WIDGET );
 
  GraalExitDialog();

  NameSet = GraalPostTreatString( NameSet );

  if ( NameSet != (char *)NULL ) 
  {
    NameSet = namealloc( NameSet );

    GraalEditSearchInstance( NameSet );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  CallbackSearchInstanceCancel               |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackSearchInstanceCancel( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( GraalSearchInstanceDialog.WIDGET );

  GraalExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Callback For Reference                  |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   CallbackSearchReferenceOk                 |
|                                                             |
\------------------------------------------------------------*/

void CallbackSearchReferenceOk( MyWidget, ClientData, CallData )
 
    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *NameSet;

  rdsbegin();
 
  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &NameSet );
 
  XtUnmanageChild( GraalSearchReferenceDialog.WIDGET );
 
  GraalExitDialog();

  NameSet = GraalPostTreatString( NameSet );

  if ( NameSet != (char *)NULL ) 
  {
    NameSet = namealloc( NameSet );

    GraalEditSearchReference( NameSet );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 CallbackSearchReferenceCancel               |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackSearchReferenceCancel( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( GraalSearchReferenceDialog.WIDGET );

  GraalExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Callback For Segment                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     CallbackSearchSegmentOk                 |
|                                                             |
\------------------------------------------------------------*/

void CallbackSearchSegmentOk( MyWidget, ClientData, CallData )
 
    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *NameSet;

  rdsbegin();
 
  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &NameSet );
 
  XtUnmanageChild( GraalSearchSegmentDialog.WIDGET );
 
  GraalExitDialog();

  NameSet = GraalPostTreatString( NameSet );

  if ( NameSet != (char *)NULL ) 
  {
    NameSet = namealloc( NameSet );

    GraalEditSearchSegment( NameSet );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   CallbackSearchSegmentCancel               |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackSearchSegmentCancel( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( GraalSearchSegmentDialog.WIDGET );

  GraalExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       Callback For Via                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     CallbackSearchViaOk                     |
|                                                             |
\------------------------------------------------------------*/

void CallbackSearchViaOk( MyWidget, ClientData, CallData )
 
    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *NameSet;

  rdsbegin();
 
  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &NameSet );
 
  XtUnmanageChild( GraalSearchViaDialog.WIDGET );
 
  GraalExitDialog();

  NameSet = GraalPostTreatString( NameSet );

  if ( NameSet != (char *)NULL ) 
  {
    NameSet = namealloc( NameSet );

    GraalEditSearchVia( NameSet );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       CallbackSearchViaCancel               |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackSearchViaCancel( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( GraalSearchViaDialog.WIDGET );

  GraalExitDialog();

  rdsend();
}

