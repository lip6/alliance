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

# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "rfm.h"
# include "GRM.h"
# include "GMX.h"
# include "GTB.h"
# include "GSB.h"
# include "GMC.h"
# include "GMF.h"

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

  char GraalCreateDialogCancel = 1;

  char GraalModelFilter[ 10 ]    = "*.";
  char GraalModelExtention[ 10 ] = ".";

  char GraalModelBuffer[ 128 ];
  char GraalModelDirectoryBuffer[ 512 ];

/*------------------------------------------------------------\
|                                                             |
|                        Create Dialog                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Segment Dialog                      |
|                                                             |
\------------------------------------------------------------*/
 
  GraalDialogItem GraalChangeSegmentWidthDialog =

  {
    "Segment width",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackChangeSegmentWidthOk,
    (XtPointer)NULL,
    (void *)CallbackChangeSegmentWidthCancel,
    (XtPointer)NULL
  };

  GraalDialogItem GraalChangeSegmentNameDialog =

  {
    "Segment name",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackChangeSegmentNameOk,
    (XtPointer)NULL,
    (void *)CallbackChangeSegmentNameCancel,
    (XtPointer)NULL
  };

/*------------------------------------------------------------\
|                                                             |
|                         Via Dialog                          |
|                                                             |
\------------------------------------------------------------*/

  GraalDialogItem GraalChangeViaNameDialog =

  {
    "Via name",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackChangeViaNameOk,
    (XtPointer)NULL,
    (void *)CallbackChangeViaNameCancel,
    (XtPointer)NULL
  };

  GraalDialogItem GraalChangeBigViaNameDialog =

  {
    "BigVia name",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackChangeBigViaNameOk,
    (XtPointer)NULL,
    (void *)CallbackChangeBigViaNameCancel,
    (XtPointer)NULL
  };

/*------------------------------------------------------------\
|                                                             |
|                       Transistor Dialog                     |
|                                                             |
\------------------------------------------------------------*/

  GraalDialogItem GraalChangeTransistorWidthDialog =

  {
    "Transistor width",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackChangeTransistorWidthOk,
    (XtPointer)NULL,
    (void *)CallbackChangeTransistorWidthCancel,
    (XtPointer)NULL
  };

  GraalDialogItem GraalChangeTransistorNameDialog =

  {
    "Transistor name",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackChangeTransistorNameOk,
    (XtPointer)NULL,
    (void *)CallbackChangeTransistorNameCancel,
    (XtPointer)NULL
  };

/*------------------------------------------------------------\
|                                                             |
|                        Connector Dialog                     |
|                                                             |
\------------------------------------------------------------*/

  GraalDialogItem GraalChangeConnectorWidthDialog =

  {
    "Connector width",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackChangeConnectorWidthOk,
    (XtPointer)NULL,
    (void *)CallbackChangeConnectorWidthCancel,
    (XtPointer)NULL
  };
    
  GraalDialogItem GraalChangeConnectorNameDialog =
 
  {
    "Connector name",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackChangeConnectorNameOk,
    (XtPointer)NULL,
    (void *)CallbackChangeConnectorNameCancel,
    (XtPointer)NULL
  };

/*------------------------------------------------------------\
|                                                             |
|                        Reference Dialog                     |
|                                                             |
\------------------------------------------------------------*/

  GraalDialogItem GraalChangeReferenceNameDialog =

  {
    "Reference name",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackChangeReferenceNameOk,
    (XtPointer)NULL,
    (void *)CallbackChangeReferenceNameCancel,
    (XtPointer)NULL
  };

/*------------------------------------------------------------\
|                                                             |
|                         Instance Dialog                     |
|                                                             |
\------------------------------------------------------------*/

  GraalDialogItem GraalChangeInstanceNameDialog =

  {
    "Instance name",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackChangeInstanceNameOk,
    (XtPointer)NULL,
    (void *)CallbackChangeInstanceNameCancel,
    (XtPointer)NULL
  };

  GraalDialogItem GraalChangeInstanceModelDialog =

  {
    "Instance model name",
    GRAAL_DIALOG_FILE,
    (Widget)NULL,
    (void *)CallbackChangeInstanceModelOk,
    (XtPointer)NULL,
    (void *)CallbackChangeInstanceModelCancel,
    (XtPointer)NULL
  };
   
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    GraalBuildCreateDialog                   |
|                                                             |
\------------------------------------------------------------*/

void GraalBuildCreateDialog()

{
  rdsbegin();

  GraalBuildDialog( GraalMainWindow, &GraalChangeSegmentWidthDialog    );
  GraalBuildDialog( GraalMainWindow, &GraalChangeSegmentNameDialog     );
  GraalBuildDialog( GraalMainWindow, &GraalChangeTransistorWidthDialog );
  GraalBuildDialog( GraalMainWindow, &GraalChangeTransistorNameDialog  );
  GraalBuildDialog( GraalMainWindow, &GraalChangeViaNameDialog         );
  GraalBuildDialog( GraalMainWindow, &GraalChangeBigViaNameDialog      );
  GraalBuildDialog( GraalMainWindow, &GraalChangeConnectorWidthDialog  );
  GraalBuildDialog( GraalMainWindow, &GraalChangeConnectorNameDialog   );
  GraalBuildDialog( GraalMainWindow, &GraalChangeReferenceNameDialog   );
  GraalBuildDialog( GraalMainWindow, &GraalChangeInstanceNameDialog    );
  GraalBuildDialog( GraalMainWindow, &GraalChangeInstanceModelDialog   );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      Callback For Dialog                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  CallbackChangeSegmentWidthOk               |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeSegmentWidthOk( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char     *WidthSet;
  long      NewWidth;

  rdsbegin();

  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &WidthSet );

  XtUnmanageChild( GraalChangeSegmentWidthDialog.WIDGET );

  GraalExitDialog();

  GraalCreateDialogCancel = 0;

  if ( WidthSet != (char *)NULL )
  {
    NewWidth = atoi( WidthSet );

    if ( NewWidth >=  GRAAL_SEGMENT_VALUE_TABLE[ GraalSegmentLayer ][0] )    
    {
      GraalSegmentWidth = NewWidth;
    }
    else
    {  
      GraalErrorMessage( GraalMainWindow, "Incorrect segment width !" );
    }

    if ( GraalSegmentWire == GRAAL_FALSE )
    {
      GraalChangeEditMode( GRAAL_CREATE_SEGMENT, 
                           GraalPromptCreateSegment );
    }
    else
    {   
      GraalChangeEditMode( GRAAL_CREATE_SEGMENT_WIRE,
                           GraalPromptCreateSegment );
    }
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                CallbackChangeSegmentWidthCancel             |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackChangeSegmentWidthCancel( MyWidget, ClientData, CallData )
 
    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( GraalChangeSegmentWidthDialog.WIDGET );
 
  GraalExitDialog();

  GraalCreateDialogCancel = 1;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   CallbackChangeSegmentNameOk               |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeSegmentNameOk( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *NameSet;

  rdsbegin();

  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &NameSet );

  XtUnmanageChild( GraalChangeSegmentNameDialog.WIDGET );

  GraalExitDialog();

  GraalCreateDialogCancel = 0;

  NameSet = GraalPostTreatString( NameSet );

  if ( NameSet != (char *)NULL )
  {
    NameSet = namealloc( NameSet );
  }

  GraalSegmentName = NameSet;

  if ( GraalSegmentWire == GRAAL_FALSE ) 
  { 
    GraalChangeEditMode( GRAAL_CREATE_SEGMENT,
                         GraalPromptCreateSegment ); 
  }  
  else 
  {    
    GraalChangeEditMode( GRAAL_CREATE_SEGMENT_WIRE,
                         GraalPromptCreateSegment );
  }  

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 CallbackChangeSegmentNameCancel             |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackChangeSegmentNameCancel( MyWidget, ClientData, CallData )
 
    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( GraalChangeSegmentNameDialog.WIDGET );
 
  GraalExitDialog();

  GraalCreateDialogCancel = 1;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    Callback For Transistor                  |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 CallbackChangeTransistorWidthOk             |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeTransistorWidthOk( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *WidthSet;
  long  NewWidth;

  rdsbegin();

  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &WidthSet );

  XtUnmanageChild( GraalChangeTransistorWidthDialog.WIDGET );

  GraalExitDialog();

  GraalCreateDialogCancel = 0;

  if ( WidthSet != (char *)NULL )
  {
    NewWidth = atoi( WidthSet );

    if ( NewWidth >=  GRAAL_SEGMENT_VALUE_TABLE[ GraalTransistorType ][0] )
    {
      GraalTransistorWidth = NewWidth;
    }
    else
    {   
      GraalErrorMessage( GraalMainWindow, "Incorrect transistor width !" );
    }

    if ( GraalTransistorWire == GRAAL_FALSE ) 
    { 
      GraalChangeEditMode( GRAAL_CREATE_TRANSISTOR,
                           GraalPromptCreateTransistor ); 
    }  
    else 
    {    
      GraalChangeEditMode( GRAAL_CREATE_TRANSISTOR_WIRE,
                           GraalPromptCreateTransistor );
    }  
  }

  rdsend();
}  

/*------------------------------------------------------------\
|                                                             |
|               CallbackChangeTransistorWidthCancel           |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackChangeTransistorWidthCancel( MyWidget, ClientData, CallData )
 
    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( GraalChangeTransistorWidthDialog.WIDGET );

  GraalExitDialog();

  GraalCreateDialogCancel = 1;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   CallbackChangeTransistorNameOk            |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeTransistorNameOk( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *NameSet;

  rdsbegin();

  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &NameSet );

  XtUnmanageChild( GraalChangeTransistorNameDialog.WIDGET );

  GraalExitDialog();

  GraalCreateDialogCancel = 0;

  NameSet = GraalPostTreatString( NameSet );

  if ( NameSet != (char *)NULL )
  {
    NameSet = namealloc( NameSet );
  }

  GraalTransistorName = NameSet;

  if ( GraalTransistorWire == GRAAL_FALSE ) 
  { 
    GraalChangeEditMode( GRAAL_CREATE_TRANSISTOR,
                         GraalPromptCreateTransistor ); 
  }  
  else 
  {    
    GraalChangeEditMode( GRAAL_CREATE_TRANSISTOR_WIRE,
                         GraalPromptCreateTransistor );
  }  

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 CallbackChangeTransistorNameCancel          |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackChangeTransistorNameCancel( MyWidget, ClientData, CallData )
 
    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( GraalChangeTransistorNameDialog.WIDGET );
 
  GraalExitDialog();

  GraalCreateDialogCancel = 1;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Callback For Connector                  |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 CallbackChangeConnectorWidthOk              |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeConnectorWidthOk( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *WidthSet;
  long  NewWidth;

  rdsbegin();
 
  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &WidthSet );
 
  XtUnmanageChild( GraalChangeConnectorWidthDialog.WIDGET );
 
  GraalExitDialog();

  GraalCreateDialogCancel = 0;
 
  if ( WidthSet != (char *)NULL )
  {
    NewWidth = atoi( WidthSet );

    if ( NewWidth >=  GRAAL_SEGMENT_VALUE_TABLE[ GraalConnectorLayer ][0] )
    {
      GraalConnectorWidth = NewWidth;
    }
    else
    {   
      GraalErrorMessage( GraalMainWindow, "Incorrect connector width !" );
    }
 
    GraalChangeEditMode( GRAAL_CREATE_CONNECTOR,
                         GraalPromptCreateConnector );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|               CallbackChangeConnectorWidthCancel            |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeConnectorWidthCancel( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( GraalChangeConnectorWidthDialog.WIDGET );

  GraalExitDialog();

  GraalCreateDialogCancel = 1;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  CallbackChangeConnectorNameOk              |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeConnectorNameOk( MyWidget, ClientData, CallData )
 
    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *NameSet;

  rdsbegin();
 
  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &NameSet );
 
  XtUnmanageChild( GraalChangeConnectorNameDialog.WIDGET );
 
  GraalExitDialog();

  GraalCreateDialogCancel = 0;

  NameSet = GraalPostTreatString( NameSet );
 
  if ( NameSet != (char *)NULL )
  {
    NameSet = namealloc( NameSet );
  }

  GraalConnectorName = NameSet;

  GraalChangeEditMode( GRAAL_CREATE_CONNECTOR,
                       GraalPromptCreateConnector );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                CallbackChangeConnectorNameCancel            |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackChangeConnectorNameCancel( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( GraalChangeConnectorNameDialog.WIDGET );

  GraalExitDialog();

  GraalCreateDialogCancel = 1;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Callback For Reference                  |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  CallbackChangeReferenceNameOk              |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeReferenceNameOk( MyWidget, ClientData, CallData )
 
    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *NameSet;

  rdsbegin();
 
  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &NameSet );
 
  XtUnmanageChild( GraalChangeReferenceNameDialog.WIDGET );
 
  GraalExitDialog();
 
  GraalCreateDialogCancel = 0;

  NameSet = GraalPostTreatString( NameSet );

  if ( NameSet != (char *)NULL ) 
  {
    NameSet = namealloc( NameSet );
  }

  GraalReferenceName = NameSet;

  GraalChangeEditMode( GRAAL_CREATE_REFERENCE,
                       GraalPromptCreateReference );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                CallbackChangeReferenceNameCancel            |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackChangeReferenceNameCancel( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( GraalChangeReferenceNameDialog.WIDGET );

  GraalExitDialog();

  GraalCreateDialogCancel = 1;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Callback For Instance                   |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   CallbackChangeInstanceNameOk              |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeInstanceNameOk( MyWidget, ClientData, CallData )
 
    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *NameSet;

  rdsbegin();
 
  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &NameSet );
 
  XtUnmanageChild( GraalChangeInstanceNameDialog.WIDGET );
 
  GraalExitDialog();

  GraalCreateDialogCancel = 0;
 
  NameSet = GraalPostTreatString( NameSet );

  if ( NameSet != (char *)NULL )
  {
    NameSet = namealloc( NameSet );

    GraalInstanceName = NameSet;
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 CallbackChangeInstanceNameCancel            |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackChangeInstanceNameCancel( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( GraalChangeInstanceNameDialog.WIDGET );

  GraalExitDialog();

  GraalCreateDialogCancel = 1;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   CallbackChangeInstanceModelOk             |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeInstanceModelOk( MyWidget, ClientData, FileStruct )
 
    Widget                            MyWidget;
    caddr_t                           ClientData;
    XmFileSelectionBoxCallbackStruct *FileStruct;
{
  char *Directory;
  char *FileName;
  int   Index;

  rdsbegin();

  FileName  = (char *)0;
  Directory = (char *)0;

  XtUnmanageChild( GraalChangeInstanceModelDialog.WIDGET );

  GraalExitDialog();

  if ( FileStruct->value != NULL )
  {
    XmStringGetLtoR( FileStruct->value,
                     XmSTRING_DEFAULT_CHARSET,
                     &FileName 
                   );

    FileName = GraalPostTreatString( FileName ); 
  }
 
  if ( FileName != (char *)NULL )
  {
    GraalCreateDialogCancel = 0;

    if ( GRAAL_WORK_LIB == (char *)NULL )
    {
      GRAAL_WORK_LIB = WORK_LIB;
    }

    for ( Index = strlen( FileName ); Index >= 0; Index-- )
    {
      if ( FileName[ Index ] == '/' ) break;
    }
  
    if ( Index >= 0 ) 
    {
      strcpy( GraalModelDirectoryBuffer, FileName );
      strcpy( GraalModelBuffer, FileName + Index + 1);

      GraalModelDirectoryBuffer[ Index + 1 ] = '\0';
      Directory = GraalModelDirectoryBuffer;
    }
    else
    {
      strcpy( GraalModelBuffer, FileName );
      Directory = GRAAL_WORK_LIB;
    }

    Index = strlen( GraalModelBuffer ) - strlen( GraalModelExtention );

    if ( Index >= 0 )
    {
      if ( ! strcmp( GraalModelBuffer + Index, GraalModelExtention ) )
      {
        GraalModelBuffer[ Index ] = '\0';
      }
    }

    GraalInstanceModel = namealloc( GraalModelBuffer );

    GraalChangeEditMode( GRAAL_CREATE_INSTANCE,
                         GraalPromptCreateInstance );

    if ( Directory != GRAAL_WORK_LIB )
    {
      GraalAddDirectoryToCataLibrary( Directory );
    }
  }
  else
  {
    GraalCreateDialogCancel = 1;

    GraalErrorMessage( GraalMainWindow, "Unable to load this model !" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 CallbackChangeInstanceModelCancel           |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackChangeInstanceModelCancel( MyWidget, ClientData, FileStruct )

    Widget                            MyWidget;
    caddr_t                           ClientData;
    XmFileSelectionBoxCallbackStruct *FileStruct;
{
  rdsbegin();

  XtUnmanageChild( GraalChangeInstanceModelDialog.WIDGET );

  GraalExitDialog();

  GraalCreateDialogCancel = 1;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 GraalEnterChangeInstanceModelDialog         |
|                                                             |
\------------------------------------------------------------*/

void GraalEnterChangeInstanceModelDialog()
{
  XmString Label;
  XmString Filter;

  if ( GraalModelFilter[2] == '\0' )
  {
    strcat( GraalModelFilter, IN_PH );
    strcat( GraalModelExtention, IN_PH );

    Filter = XmStringCreateSimple( GraalModelFilter ); 

    XtVaSetValues( GraalChangeInstanceModelDialog.WIDGET, 
                   XmNpattern, Filter, NULL);

    XmStringFree( Filter );
  }

  GraalEnterDialog( &GraalChangeInstanceModelDialog );
}

/*------------------------------------------------------------\
|                                                             |
|                   CallbackChangeViaNameOk                   |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeViaNameOk( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *NameSet;

  rdsbegin();

  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &NameSet );

  XtUnmanageChild( GraalChangeViaNameDialog.WIDGET );

  GraalExitDialog();

  GraalCreateDialogCancel = 0;

  NameSet = GraalPostTreatString( NameSet );

  if ( NameSet != (char *)NULL )
  {
    NameSet = namealloc( NameSet );
  }

  GraalViaName = NameSet;

  GraalChangeEditMode( GRAAL_CREATE_VIA, GraalPromptCreateVia ); 

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     CallbackChangeViaNameCancel             |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackChangeViaNameCancel( MyWidget, ClientData, CallData )
 
    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( GraalChangeViaNameDialog.WIDGET );
 
  GraalExitDialog();

  GraalCreateDialogCancel = 1;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   CallbackChangeBigViaNameOk                |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeBigViaNameOk( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *NameSet;

  rdsbegin();

  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &NameSet );

  XtUnmanageChild( GraalChangeBigViaNameDialog.WIDGET );

  GraalExitDialog();

  GraalCreateDialogCancel = 0;

  NameSet = GraalPostTreatString( NameSet );

  if ( NameSet != (char *)NULL )
  {
    NameSet = namealloc( NameSet );
  }

  GraalBigViaName = NameSet;

  GraalChangeEditMode( GRAAL_CREATE_BIGVIA, GraalPromptCreateBigVia ); 

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     CallbackChangeBigViaNameCancel          |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackChangeBigViaNameCancel( MyWidget, ClientData, CallData )
 
    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  rdsbegin();

  XtUnmanageChild( GraalChangeBigViaNameDialog.WIDGET );
 
  GraalExitDialog();

  GraalCreateDialogCancel = 1;

  rdsend();
}

