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
| Tool    :                   DREAL                           |
|                                                             |
| File    :                   Panel.c                         |
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
# include <sys/types.h>
# include <sys/stat.h>
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
# include "GRD.h"
# include "GMX.h"
# include "GTB.h"
# include "GSB.h"
# include "GMF.h"
# include "GMF_panel.h"
# include "GMF_dialog.h"
# include "GTB_dialog.h"

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

   DrealPanelItem DrealLibraryPanel =

         {
           "Library",
           1,
           0,
           DREAL_LIBRARY_X,
           DREAL_LIBRARY_Y,
           250,
           375,
           1,
           1,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           0,
           (DrealPanelButtonItem *)NULL
         };

   int DrealLibraryDefaultValues[ 5 ] =

         {
           DREAL_LIBRARY_X,
           DREAL_LIBRARY_Y,
           250, 375, 0
         };

   static char **DREAL_CATA_LIB    = (char **)NULL;
   static char  *DREAL_DEFAULT_LIB = (char *)NULL;
          char  *DREAL_WORK_LIB    = (char  *)NULL;
 
   static char   DrealFirstLibrary = 1;


/*------------------------------------------------------------\
|                                                             |
|                    Widget For Library Panel                 |
|                                                             |
\------------------------------------------------------------*/

  Widget     DrealLibraryListLabel;
  Widget     DrealLibraryList;

  Widget     DrealLibraryButtonOk;
  Widget     DrealLibraryButtonUpdate;
  Widget     DrealLibraryButtonCancel;

  Widget     DrealLibraryCataLabel;
  Widget     DrealLibraryCata;

  Widget     DrealLibraryWorkLabel;
  Widget     DrealLibraryWork;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        DrealIsDirectory                     |
|                                                             |
\------------------------------------------------------------*/

char DrealIsDirectory( FileName )

   char *FileName;
{
  struct stat Buffer;

  rdsbegin();

  if ( stat( FileName, &Buffer ) != -1 )
  {
    if ( ( Buffer.st_mode & S_IFMT ) == S_IFDIR ) 
    {
      rdsend();
      return( 1 );
    }
  }

  rdsend();
  return( 0 );
}
 
/*------------------------------------------------------------\
|                                                             |
|                      DrealSetLibraryList                    |
|                                                             |
\------------------------------------------------------------*/

void DrealSetLibraryList()
{
  int      Index;
  XmString Label;

  rdsbegin();

  XmListDeleteAllItems( DrealLibraryList );

  for ( Index = 0;
        DREAL_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    Label = XmStringCreateSimple( DREAL_CATA_LIB[ Index ] );
    XmListAddItem( DrealLibraryList , Label , 0 );
    XmStringFree( Label );
  }

  Label = XmStringCreateSimple( DREAL_WORK_LIB );
  XmListAddItem( DrealLibraryList , Label , 0 );
  XmStringFree( Label );

  DREAL_DEFAULT_LIB = (char *)NULL;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealSetWorkLibrary                    |
|                                                             |
\------------------------------------------------------------*/

void DrealSetWorkLibrary()
{
  rdsbegin();

  XmTextSetString( DrealLibraryWork, DREAL_WORK_LIB );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealSetCataLibrary                    |
|                                                             |
\------------------------------------------------------------*/

void DrealSetCataLibrary()
{
  int   Index;
  long  Length;
  char *Buffer;
  char *Scan;

  rdsbegin();

  for ( Index = 0, Length = 0; 
        DREAL_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    Length = Length + strlen( DREAL_CATA_LIB[ Index ] ) + 1;
  }

  Buffer = rdsallocblock( Length + 1 );
  Scan   = Buffer;

  for ( Index = 0;
        DREAL_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    strcpy( Scan, DREAL_CATA_LIB[ Index ] );
    strcat( Scan, "\n" );

    Scan = Scan + strlen( Scan );
  }

  XmTextSetString( DrealLibraryCata, Buffer );

  rdsfreeblock( Buffer );

  rdsend();
}
 
/*------------------------------------------------------------\
|                                                             |
|                     DrealInitializeLibrary                  |
|                                                             |
\------------------------------------------------------------*/

void DrealInitializeLibrary()
{
  int Index;

  rdsbegin();

  for ( Index = 0; CATA_LIB[ Index ] != (char *)NULL; Index++ );

  DREAL_CATA_LIB = (char **)rdsallocblock( sizeof(char *) * ( Index + 1 ));

  for ( Index = 0; 
        CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    DREAL_CATA_LIB[ Index ] = rdsallocblock( strlen( CATA_LIB[ Index ] ) + 1 );
    strcpy( DREAL_CATA_LIB[ Index ], CATA_LIB[ Index ] );
  }

  DREAL_CATA_LIB[ Index ] = (char *)NULL;

  DREAL_WORK_LIB = rdsallocblock( strlen( WORK_LIB ) + 1 );
  strcpy( DREAL_WORK_LIB, WORK_LIB );

  DrealSetCataLibrary();
  DrealSetWorkLibrary();
  DrealSetLibraryList();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     DrealGetCataLibrary                     |
|                                                             |
\------------------------------------------------------------*/

 void DrealGetCataLibrary()
 {
  char *NewCata;
  char *Scan;
  int   Index;
  int   Count;
  int   Length;

  rdsbegin();

  for ( Index = 0; 
        DREAL_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    rdsfreeblock( DREAL_CATA_LIB[ Index ] );
  }

  rdsfreeblock( DREAL_CATA_LIB );

  NewCata = XmTextGetString( DrealLibraryCata );

  Count = 1;

  for ( Index = 0; NewCata[ Index ] != '\0'; Index++ ) 
  {
    if ( NewCata[ Index ] == '\n' )
    {
      Count = Count + 1;
    }
  }

  DREAL_CATA_LIB = (char **)rdsallocblock( sizeof(char *) * ( Count + 1 ));

  Count = 0;
  Scan  = NewCata;

  for ( Index = 0; NewCata[ Index ] != '\0'; Index++ )
  {
    if ( NewCata[ Index ] == '\n' ) 
    {
      NewCata[ Index ] = '\0';

      Length = strlen( Scan );

      if ( ( Length > 0               ) &&
           ( DrealIsDirectory( Scan ) ) )
      {
        DREAL_CATA_LIB[ Count ] = rdsallocblock( Length + 1 );
        strcpy( DREAL_CATA_LIB[ Count ], Scan );
        Count = Count + 1;
      }

      Scan  = NewCata + Index + 1;
    }
    else
    if ( NewCata[ Index ] == ' ' )
    {
      NewCata[ Index ] = '\0';
    }
  }
 
  Length = strlen( Scan );

  if ( Length > 0 )
  {
    DREAL_CATA_LIB[ Count ] = rdsallocblock( Length + 1 );
    strcpy( DREAL_CATA_LIB[ Count ], Scan );
    Count = Count + 1;
  }

  DREAL_CATA_LIB[ Count ] = (char *)NULL;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     DrealGetWorkLibrary                     |
|                                                             |
\------------------------------------------------------------*/

void DrealGetWorkLibrary()
{
  char *NewWork;

  rdsbegin();

  rdsfreeblock( DREAL_WORK_LIB );

  NewWork = XmTextGetString( DrealLibraryWork );

  if ( DrealIsDirectory( NewWork ) )
  {
    DREAL_WORK_LIB = rdsallocblock( strlen( NewWork ) + 1 );
    strcpy( DREAL_WORK_LIB, NewWork );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       CallbackLibraryList                   |
|                                                             |
\------------------------------------------------------------*/

void CallbackLibraryList( MyWidget, ClientData, CallData )

     Widget                MyWidget;
     caddr_t               ClientData;
     XmListCallbackStruct *CallData;
{
  rdsbegin();

  DREAL_DEFAULT_LIB = DREAL_CATA_LIB[ CallData->item_position - 1 ];

  if ( DREAL_DEFAULT_LIB == (char *)NULL )
  {
    DREAL_DEFAULT_LIB = DREAL_WORK_LIB;
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        CallbackLibraryOk                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackLibraryOk( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  XmString Label;
  int      Index;

  rdsbegin();

  DrealExitPanel( &DrealLibraryPanel );
 
  DrealExitDialog();

  if ( DrealFirstLibrary == 0 )
  {
    for ( Index = 0;
          CATA_LIB[ Index ] != (char *)NULL;
          Index++ )
    {
      rdsfreeblock( CATA_LIB[ Index ] );
    }

    rdsfreeblock( CATA_LIB );
    rdsfreeblock( WORK_LIB );
  }

  DrealFirstLibrary = 0;

  CATA_LIB = DREAL_CATA_LIB;
  WORK_LIB = DREAL_WORK_LIB;

  if ( DREAL_DEFAULT_LIB != (char *)NULL )
  {
    Label = XmStringCreateSimple( DREAL_DEFAULT_LIB );

    XtVaSetValues( DrealFileOpenDialog.WIDGET,
                   XmNdirectory, Label, NULL );
 
    XmStringFree( Label );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        CallbackLibraryUpdate                |
|                                                             |
\------------------------------------------------------------*/

void CallbackLibraryUpdate( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealGetCataLibrary();
  DrealGetWorkLibrary();
  DrealSetLibraryList();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       CallbackLibraryCancel                 |
|                                                             |
\------------------------------------------------------------*/

void CallbackLibraryCancel( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  int Index;

  rdsbegin();

  for ( Index = 0;
        DREAL_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    rdsfreeblock( DREAL_CATA_LIB[ Index ] );
  }

  rdsfreeblock( DREAL_CATA_LIB );
  rdsfreeblock( DREAL_WORK_LIB );

  XtUnmanageChild( DrealLibraryPanel.PANEL );

  DrealExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       CallbackEnterLibrary                  |
|                                                             |
\------------------------------------------------------------*/

void CallbackEnterLibrary( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealInitializeLibrary();

  DrealEnterPanel( &DrealLibraryPanel );
  DrealLimitedLoop( DrealLibraryPanel.PANEL );
  DrealReEnterDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealBuildPanelLibrary                 |
|                                                             |
\------------------------------------------------------------*/

void DrealBuildPanelLibrary()
{
  Arg      Args[15];
  XmString Label;

  rdsbegin();

  XtSetArg( Args[0], XmNshadowType    , XmSHADOW_ETCHED_IN      );
  XtSetArg( Args[1], XmNdeleteResponse, XmDO_NOTHING            );
  XtSetArg( Args[2], XmNtitle         , DrealLibraryPanel.TITLE );

  DrealLibraryPanel.PANEL = 

    XmCreateFormDialog( DrealMainWindow, "DrealLibraryPanel", Args, 3);

  XtAddCallback( DrealLibraryPanel.PANEL, XmNdestroyCallback,
                 DrealDestroyDialogCallback, NULL  );

  DrealLibraryPanel.PANEL_FORM = 

    XtVaCreateManagedWidget( "DrealLibraryPanelForm",
                             xmFormWidgetClass,
                             DrealLibraryPanel.PANEL,
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             XmNfractionBase     , 10,
                             NULL
                           );
 
  DrealLibraryPanel.FRAME = 

    XtVaCreateManagedWidget( "DrealLibraryFrame",
                             xmFrameWidgetClass,
                             DrealLibraryPanel.PANEL_FORM, 
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             NULL
                           );
 
  DrealLibraryPanel.FORM = 

    XtVaCreateManagedWidget( "DrealLibraryForm",
                             xmFormWidgetClass,
                             DrealLibraryPanel.FRAME, 
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             XmNfractionBase     , 120,
                             NULL
                           );

  DrealLibraryButtonUpdate =

    XtVaCreateManagedWidget( "Update",
                             xmPushButtonWidgetClass,
                             DrealLibraryPanel.FORM,
                             XmNleftAttachment   , XmATTACH_POSITION,
                             XmNleftPosition     , 10,
                             XmNrightAttachment  , XmATTACH_POSITION,
                             XmNrightPosition    , 40,
                             XmNtopAttachment    , XmATTACH_POSITION,
                             XmNtopPosition      , 105,
                             XmNbottomAttachment , XmATTACH_POSITION,
                             XmNbottomPosition   , 115,
                             NULL
                           );

  DrealLibraryButtonOk = 

    XtVaCreateManagedWidget( "Ok",
                             xmPushButtonWidgetClass,
                             DrealLibraryPanel.FORM, 
                             XmNleftAttachment   , XmATTACH_POSITION,
                             XmNleftPosition     , 50,
                             XmNrightAttachment  , XmATTACH_POSITION,
                             XmNrightPosition    , 70,
                             XmNtopAttachment    , XmATTACH_POSITION,
                             XmNtopPosition      , 105,
                             XmNbottomAttachment , XmATTACH_POSITION,
                             XmNbottomPosition   , 115,
                             NULL
                           );

  DrealLibraryButtonCancel = 

    XtVaCreateManagedWidget( "Cancel",
                             xmPushButtonWidgetClass,
                             DrealLibraryPanel.FORM, 
                             XmNleftAttachment   , XmATTACH_POSITION,
                             XmNleftPosition     , 80,
                             XmNrightAttachment  , XmATTACH_POSITION,
                             XmNrightPosition    , 110,
                             XmNtopAttachment    , XmATTACH_POSITION,
                             XmNtopPosition      , 105,
                             XmNbottomAttachment , XmATTACH_POSITION,
                             XmNbottomPosition   , 115,
                             NULL
                           );

  Label = XmStringCreateSimple( "Directories" );

  DrealLibraryListLabel = 

     XtVaCreateManagedWidget( "DrealLibraryListLabel",
                              xmLabelGadgetClass      , 
                              DrealLibraryPanel.FORM, 
                              XmNlabelString          , Label,
                              XmNleftAttachment       , XmATTACH_POSITION,
                              XmNleftPosition         , 5,
                              XmNtopAttachment        , XmATTACH_POSITION,
                              XmNtopPosition          , 5,
                              XmNbottomAttachment     , XmATTACH_POSITION,
                              XmNbottomPosition       , 10,
                              NULL );

  XmStringFree( Label );

  XtSetArg( Args[0]  , XmNtopAttachment         , XmATTACH_POSITION );
  XtSetArg( Args[1]  , XmNtopPosition           , 10 );
  XtSetArg( Args[2]  , XmNbottomAttachment      , XmATTACH_POSITION );
  XtSetArg( Args[3]  , XmNbottomPosition        , 40 );
  XtSetArg( Args[4]  , XmNrightAttachment       , XmATTACH_POSITION ); 
  XtSetArg( Args[5]  , XmNrightPosition         , 115 );
  XtSetArg( Args[6]  , XmNleftAttachment        , XmATTACH_POSITION );
  XtSetArg( Args[7]  , XmNleftPosition          , 5 );
  XtSetArg( Args[8]  , XmNscrollBarDisplayPolicy, XmSTATIC );
  XtSetArg( Args[9]  , XmNscrollHorizontal      , True );
  XtSetArg( Args[10] , XmNscrollVertical        , True );
  XtSetArg( Args[11] , XmNlistSizePolicy        , XmCONSTANT );

  DrealLibraryList = 

    XmCreateScrolledList( DrealLibraryPanel.FORM,
                          "DrealLibraryList", Args, 12 );

  Label = XmStringCreateSimple( "Catalog libraries" );

  DrealLibraryCataLabel = 

     XtVaCreateManagedWidget( "DrealLibraryCataLabel",
                              xmLabelGadgetClass      ,
                              DrealLibraryPanel.FORM,
                              XmNlabelString          , Label,
                              XmNleftAttachment       , XmATTACH_POSITION,
                              XmNleftPosition         , 5,
                              XmNtopAttachment        , XmATTACH_POSITION,
                              XmNtopPosition          , 45,
                              XmNbottomAttachment     , XmATTACH_POSITION,
                              XmNbottomPosition       , 50,
                              NULL );

  XmStringFree( Label );

  XtSetArg( Args[0]  , XmNrows                  , 4 );
  XtSetArg( Args[1]  , XmNeditable              , True );
  XtSetArg( Args[2]  , XmNcursorPositionVisible , True );
  XtSetArg( Args[3]  , XmNscrollHorizontal      , True );
  XtSetArg( Args[4]  , XmNscrollVertical        , True );
  XtSetArg( Args[5]  , XmNeditMode              , XmMULTI_LINE_EDIT );
  XtSetArg( Args[6]  , XmNtopAttachment         , XmATTACH_POSITION );
  XtSetArg( Args[7]  , XmNtopPosition           , 50 );
  XtSetArg( Args[8]  , XmNleftAttachment        , XmATTACH_POSITION );
  XtSetArg( Args[9]  , XmNleftPosition          , 5 );
  XtSetArg( Args[10] , XmNrightAttachment       , XmATTACH_POSITION );
  XtSetArg( Args[11] , XmNrightPosition         , 115 );
  XtSetArg( Args[12] , XmNbottomAttachment      , XmATTACH_POSITION );
  XtSetArg( Args[13] , XmNbottomPosition        , 80 );

  DrealLibraryCata  = 
 
    XmCreateScrolledText( DrealLibraryPanel.FORM,
                          "DrealLibraryCata", Args, 14 );

  Label = XmStringCreateSimple( "Work library" );

  DrealLibraryWorkLabel =

     XtVaCreateManagedWidget( "DrealLibraryWorkLabel",
                              xmLabelGadgetClass      ,
                              DrealLibraryPanel.FORM,
                              XmNlabelString          , Label,
                              XmNleftAttachment       , XmATTACH_POSITION,
                              XmNleftPosition         , 5,
                              XmNtopAttachment        , XmATTACH_POSITION,
                              XmNtopPosition          , 85,
                              XmNbottomAttachment     , XmATTACH_POSITION,
                              XmNbottomPosition       , 90,
                              NULL );
  XmStringFree( Label );

  XtSetArg( Args[0]  , XmNrows                  , 1 );
  XtSetArg( Args[1]  , XmNeditable              , True );
  XtSetArg( Args[2]  , XmNeditMode              , XmSINGLE_LINE_EDIT );
  XtSetArg( Args[3]  , XmNscrollHorizontal      , False );
  XtSetArg( Args[4]  , XmNscrollVertical        , False );
  XtSetArg( Args[5]  , XmNcursorPositionVisible , True  );
  XtSetArg( Args[6]  , XmNtopAttachment         , XmATTACH_POSITION );
  XtSetArg( Args[7]  , XmNtopPosition           , 90 );
  XtSetArg( Args[8]  , XmNleftAttachment        , XmATTACH_POSITION );
  XtSetArg( Args[9]  , XmNleftPosition          , 5 );
  XtSetArg( Args[10] , XmNrightAttachment       , XmATTACH_POSITION );
  XtSetArg( Args[11] , XmNrightPosition         , 115 );
  XtSetArg( Args[12] , XmNbottomAttachment      , XmATTACH_POSITION );
  XtSetArg( Args[13] , XmNbottomPosition        , 100 );

  DrealLibraryWork  =

    XmCreateText( DrealLibraryPanel.FORM,
                  "DrealLibraryWork", Args, 14 );

  XtManageChild( DrealLibraryWork );
  XtManageChild( DrealLibraryList );
  XtManageChild( DrealLibraryCata );

  XtAddCallback( DrealLibraryList,
                 XmNdefaultActionCallback,
                 CallbackLibraryList, NULL );

  XtAddCallback( DrealLibraryButtonOk, 
                 XmNactivateCallback,
                 CallbackLibraryOk, NULL );

  XtAddCallback( DrealLibraryButtonCancel, 
                 XmNactivateCallback,
                 CallbackLibraryCancel, NULL );

  XtAddCallback( DrealLibraryButtonUpdate, 
                 XmNactivateCallback,
                 CallbackLibraryUpdate, NULL );

  XtVaSetValues( DrealLibraryPanel.PANEL,
                 XmNheight, DrealLibraryPanel.HEIGHT,
                 XmNwidth , DrealLibraryPanel.WIDTH,
                 XmNx     , DrealLibraryPanel.X,
                 XmNy     , DrealLibraryPanel.Y,
                 NULL );

  Label = XmStringCreateSimple( "Library" );

  XtVaSetValues( XmFileSelectionBoxGetChild( DrealFileOpenDialog.WIDGET,
                 XmDIALOG_HELP_BUTTON ),
                 XmNlabelString, Label, NULL  );

  XmStringFree( Label );

  XtAddCallback( XmFileSelectionBoxGetChild( DrealFileOpenDialog.WIDGET,
                 XmDIALOG_HELP_BUTTON ),
                 XmNactivateCallback, 
                 CallbackEnterLibrary, NULL );

  rdsend();
}
