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
| File    :                   Panel.c                         |
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
# include "GRM.h"
# include "GMX.h"
# include "GTB.h"
# include "GSB.h"
# include "GMF.h"

# include "GMF_panel.h"
# include "GMF_dialog.h"
# include "GMC_dialog.h"
# include "GMT_dialog.h"

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

   GraalPanelItem GraalLibraryPanel =

         {
           "Library",
           1,
           0,
           GRAAL_LIBRARY_X,
           GRAAL_LIBRARY_Y,
           250,
           375,
           1,
           1,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           0,
           (GraalPanelButtonItem *)NULL
         };

   int GraalLibraryDefaultValues[ 5 ] =

         {
           GRAAL_LIBRARY_X,
           GRAAL_LIBRARY_Y,
           250, 375, 0
         };

   static char **GRAAL_CATA_LIB    = (char **)NULL;
   static char  *GRAAL_DEFAULT_LIB = (char *)NULL;
          char  *GRAAL_WORK_LIB    = (char  *)NULL;
 
   static char   GraalFirstLibrary = 1;

/*------------------------------------------------------------\
|                                                             |
|                            Buffer                           |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    Widget For Library Panel                 |
|                                                             |
\------------------------------------------------------------*/

  Widget     GraalLibraryListLabel;
  Widget     GraalLibraryList;

  Widget     GraalLibraryButtonOk;
  Widget     GraalLibraryButtonUpdate;
  Widget     GraalLibraryButtonCancel;

  Widget     GraalLibraryCataLabel;
  Widget     GraalLibraryCata;

  Widget     GraalLibraryWorkLabel;
  Widget     GraalLibraryWork;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        GraalIsDirectory                     |
|                                                             |
\------------------------------------------------------------*/

char GraalIsDirectory( FileName )

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

/*-----------------------------------------------------------*\
|                                                             |
|                     GraalAddDirectoryToCataLibrary          |
|                                                             |
\------------------------------------------------------------*/

void GraalAddDirectoryToCataLibrary( Directory )

  char *Directory;
{
  char **NewCataLib;
  int    Index;

  if ( ! GraalIsDirectory( Directory ) ) return;

  for ( Index = 0;
        CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    if ( ! strcmp( CATA_LIB[ Index ], Directory ) ) return;
  }

  NewCataLib = (char **)rdsallocblock( sizeof(char *) * ( Index + 2 ));

  for ( Index = 0;
        CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    NewCataLib[ Index ] = CATA_LIB[ Index ];
  }

  NewCataLib[ Index ] = rdsallocblock( strlen( Directory ) + 1 );
  strcpy( NewCataLib[ Index ], Directory );

  rdsfreeblock( CATA_LIB );
  CATA_LIB = NewCataLib;
}
 
/*------------------------------------------------------------\
|                                                             |
|                      GraalSetLibraryList                    |
|                                                             |
\------------------------------------------------------------*/

void GraalSetLibraryList()
{
  int      Index;
  XmString Label;

  rdsbegin();

  XmListDeleteAllItems( GraalLibraryList );

  for ( Index = 0;
        GRAAL_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    Label = XmStringCreateSimple( GRAAL_CATA_LIB[ Index ] );
    XmListAddItem( GraalLibraryList , Label , 0 );
    XmStringFree( Label );
  }

  Label = XmStringCreateSimple( GRAAL_WORK_LIB );
  XmListAddItem( GraalLibraryList , Label , 0 );
  XmStringFree( Label );

  GRAAL_DEFAULT_LIB = (char *)NULL;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalSetWorkLibrary                    |
|                                                             |
\------------------------------------------------------------*/

void GraalSetWorkLibrary()
{
  rdsbegin();

  XmTextSetString( GraalLibraryWork, GRAAL_WORK_LIB );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalSetCataLibrary                    |
|                                                             |
\------------------------------------------------------------*/

void GraalSetCataLibrary()
{
  int   Index;
  long  Length;
  char *Buffer;
  char *Scan;

  rdsbegin();

  for ( Index = 0, Length = 0; 
        GRAAL_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    Length = Length + strlen( GRAAL_CATA_LIB[ Index ] ) + 1;
  }

  Buffer = rdsallocblock( Length + 1 ); 
  Scan   = Buffer;

  for ( Index = 0;
        GRAAL_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    strcpy( Scan, GRAAL_CATA_LIB[ Index ] );
    strcat( Scan, "\n" );

    Scan = Scan + strlen( Scan );
  }

  XmTextSetString( GraalLibraryCata, Buffer );

  rdsfreeblock( Buffer );

  rdsend();
}
 
/*------------------------------------------------------------\
|                                                             |
|                     GraalInitializeLibrary                  |
|                                                             |
\------------------------------------------------------------*/

void GraalInitializeLibrary()
{
  int Index;

  rdsbegin();

  for ( Index = 0; CATA_LIB[ Index ] != (char *)NULL; Index++ );

  GRAAL_CATA_LIB = (char **)rdsallocblock( sizeof(char *) * ( Index + 1 ));

  for ( Index = 0; 
        CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    GRAAL_CATA_LIB[ Index ] = rdsallocblock( strlen( CATA_LIB[ Index ] ) + 1 );
    strcpy( GRAAL_CATA_LIB[ Index ], CATA_LIB[ Index ] );
  }

  GRAAL_CATA_LIB[ Index ] = (char *)NULL;

  GRAAL_WORK_LIB = rdsallocblock( strlen( WORK_LIB ) + 1 );
  strcpy( GRAAL_WORK_LIB, WORK_LIB );

  GraalSetCataLibrary();
  GraalSetWorkLibrary();
  GraalSetLibraryList();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalGetCataLibrary                     |
|                                                             |
\------------------------------------------------------------*/

 void GraalGetCataLibrary()
 {
  char *NewCata;
  char *Scan;
  int   Index;
  int   Count;
  int   Length;

  rdsbegin();

  for ( Index = 0; 
        GRAAL_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    rdsfreeblock( GRAAL_CATA_LIB[ Index ] );
  }

  rdsfreeblock( GRAAL_CATA_LIB );

  NewCata = XmTextGetString( GraalLibraryCata );

  Count = 1;

  for ( Index = 0; NewCata[ Index ] != '\0'; Index++ ) 
  {
    if ( NewCata[ Index ] == '\n' )
    {
      Count = Count + 1;
    }
  }

  GRAAL_CATA_LIB = (char **)rdsallocblock( sizeof(char *) * ( Count + 1 ));

  Count = 0;
  Scan  = NewCata;

  for ( Index = 0; NewCata[ Index ] != '\0'; Index++ )
  {
    if ( NewCata[ Index ] == '\n' ) 
    {
      NewCata[ Index ] = '\0';

      Length = strlen( Scan );

      if ( ( Length > 0               ) &&
           ( GraalIsDirectory( Scan ) ) )
      {
        GRAAL_CATA_LIB[ Count ] = rdsallocblock( Length + 1 );
        strcpy( GRAAL_CATA_LIB[ Count ], Scan );
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
    GRAAL_CATA_LIB[ Count ] = rdsallocblock( Length + 1 );
    strcpy( GRAAL_CATA_LIB[ Count ], Scan );
    Count = Count + 1;
  }

  GRAAL_CATA_LIB[ Count ] = (char *)NULL;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalGetWorkLibrary                     |
|                                                             |
\------------------------------------------------------------*/

void GraalGetWorkLibrary()
{
  char *NewWork;

  rdsbegin();

  rdsfreeblock( GRAAL_WORK_LIB );

  NewWork = XmTextGetString( GraalLibraryWork );

  if ( GraalIsDirectory( NewWork ) )
  {
    GRAAL_WORK_LIB = rdsallocblock( strlen( NewWork ) + 1 );
    strcpy( GRAAL_WORK_LIB, NewWork );
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

  GRAAL_DEFAULT_LIB = GRAAL_CATA_LIB[ CallData->item_position - 1 ];

  if ( GRAAL_DEFAULT_LIB == (char *)NULL )
  {
    GRAAL_DEFAULT_LIB = GRAAL_WORK_LIB;
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

  GraalExitPanel( &GraalLibraryPanel );
 
  GraalExitDialog();

  if ( GraalFirstLibrary == 0 )
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

  GraalFirstLibrary = 0;

  CATA_LIB = GRAAL_CATA_LIB;
  WORK_LIB = GRAAL_WORK_LIB;

  if ( GRAAL_DEFAULT_LIB != (char *)NULL )
  {
    Label = XmStringCreateSimple( GRAAL_DEFAULT_LIB );

    XtVaSetValues( GraalFileOpenDialog.WIDGET,
                   XmNdirectory, Label, NULL );

    XtVaSetValues( GraalChangeInstanceModelDialog.WIDGET,
                   XmNdirectory, Label, NULL );

    XtVaSetValues( GraalToolsLoadErrorDialog.WIDGET,
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

  GraalGetCataLibrary();
  GraalGetWorkLibrary();
  GraalSetLibraryList();

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
        GRAAL_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    rdsfreeblock( GRAAL_CATA_LIB[ Index ] );
  }

  rdsfreeblock( GRAAL_CATA_LIB );
  rdsfreeblock( GRAAL_WORK_LIB );

  GraalExitPanel( &GraalLibraryPanel );

  GraalExitDialog();

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

  GraalInitializeLibrary();

  GraalEnterPanel( &GraalLibraryPanel );
  GraalLimitedLoop( GraalLibraryPanel.PANEL );
  GraalReEnterDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalBuildPanelLibrary                 |
|                                                             |
\------------------------------------------------------------*/

void GraalBuildPanelLibrary()
{
  Arg      Args[15];
  XmString Label;

  rdsbegin();

  XtSetArg( Args[0], XmNshadowType    , XmSHADOW_ETCHED_IN      );
  XtSetArg( Args[1], XmNdeleteResponse, XmDO_NOTHING            );
  XtSetArg( Args[2], XmNtitle         , GraalLibraryPanel.TITLE );

  GraalLibraryPanel.PANEL = 

    XmCreateFormDialog( GraalMainWindow, "GraalLibraryPanel", Args, 3);

  XtAddCallback( GraalLibraryPanel.PANEL, XmNdestroyCallback,
                 GraalDestroyDialogCallback, NULL  );

  GraalLibraryPanel.PANEL_FORM = 

    XtVaCreateManagedWidget( "GraalLibraryPanelForm",
                             xmFormWidgetClass,
                             GraalLibraryPanel.PANEL,
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             XmNfractionBase     , 10,
                             NULL
                           );
 
  GraalLibraryPanel.FRAME = 

    XtVaCreateManagedWidget( "GraalLibraryFrame",
                             xmFrameWidgetClass,
                             GraalLibraryPanel.PANEL_FORM, 
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             NULL
                           );
 
  GraalLibraryPanel.FORM = 

    XtVaCreateManagedWidget( "GraalLibraryForm",
                             xmFormWidgetClass,
                             GraalLibraryPanel.FRAME, 
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             XmNfractionBase     , 120,
                             NULL
                           );

  GraalLibraryButtonUpdate =

    XtVaCreateManagedWidget( "Update",
                             xmPushButtonWidgetClass,
                             GraalLibraryPanel.FORM,
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

  GraalLibraryButtonOk = 

    XtVaCreateManagedWidget( "Ok",
                             xmPushButtonWidgetClass,
                             GraalLibraryPanel.FORM, 
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

  GraalLibraryButtonCancel = 

    XtVaCreateManagedWidget( "Cancel",
                             xmPushButtonWidgetClass,
                             GraalLibraryPanel.FORM, 
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

  GraalLibraryListLabel = 

     XtVaCreateManagedWidget( "GraalLibraryListLabel",
                              xmLabelGadgetClass      , 
                              GraalLibraryPanel.FORM, 
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

  GraalLibraryList = 

    XmCreateScrolledList( GraalLibraryPanel.FORM,
                          "GraalLibraryList", Args, 12 );

  Label = XmStringCreateSimple( "Catalog libraries" );

  GraalLibraryCataLabel = 

     XtVaCreateManagedWidget( "GraalLibraryCataLabel",
                              xmLabelGadgetClass      ,
                              GraalLibraryPanel.FORM,
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

  GraalLibraryCata  = 
 
    XmCreateScrolledText( GraalLibraryPanel.FORM,
                          "GraalLibraryCata", Args, 14 );

  Label = XmStringCreateSimple( "Work library" );

  GraalLibraryWorkLabel =

     XtVaCreateManagedWidget( "GraalLibraryWorkLabel",
                              xmLabelGadgetClass      ,
                              GraalLibraryPanel.FORM,
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

  GraalLibraryWork  =

    XmCreateText( GraalLibraryPanel.FORM,
                  "GraalLibraryWork", Args, 14 );

  XtManageChild( GraalLibraryWork );
  XtManageChild( GraalLibraryList );
  XtManageChild( GraalLibraryCata );

  XtAddCallback( GraalLibraryList,
                 XmNdefaultActionCallback,
                 CallbackLibraryList, NULL );

  XtAddCallback( GraalLibraryButtonOk, 
                 XmNactivateCallback,
                 CallbackLibraryOk, NULL );

  XtAddCallback( GraalLibraryButtonCancel, 
                 XmNactivateCallback,
                 CallbackLibraryCancel, NULL );

  XtAddCallback( GraalLibraryButtonUpdate, 
                 XmNactivateCallback,
                 CallbackLibraryUpdate, NULL );

  XtVaSetValues( GraalLibraryPanel.PANEL,
                 XmNheight, GraalLibraryPanel.HEIGHT,
                 XmNwidth , GraalLibraryPanel.WIDTH,
                 XmNx     , GraalLibraryPanel.X,
                 XmNy     , GraalLibraryPanel.Y,
                 NULL );

  Label = XmStringCreateSimple( "Library" );

  XtVaSetValues( XmFileSelectionBoxGetChild( GraalFileOpenDialog.WIDGET,
                 XmDIALOG_HELP_BUTTON ),
                 XmNlabelString, Label, NULL  );

  XtVaSetValues( XmFileSelectionBoxGetChild( GraalChangeInstanceModelDialog.WIDGET,
                 XmDIALOG_HELP_BUTTON ),
                 XmNlabelString, Label, NULL  );

  XtVaSetValues( XmFileSelectionBoxGetChild( GraalToolsLoadErrorDialog.WIDGET,
                 XmDIALOG_HELP_BUTTON ),
                 XmNlabelString, Label, NULL  );

  XmStringFree( Label );

  XtAddCallback( XmFileSelectionBoxGetChild( GraalFileOpenDialog.WIDGET,
                 XmDIALOG_HELP_BUTTON ),
                 XmNactivateCallback, CallbackEnterLibrary, NULL );

  XtAddCallback( XmFileSelectionBoxGetChild( GraalChangeInstanceModelDialog.WIDGET,
                 XmDIALOG_HELP_BUTTON ),
                 XmNactivateCallback, CallbackEnterLibrary, NULL );

  XtAddCallback( XmFileSelectionBoxGetChild( GraalToolsLoadErrorDialog.WIDGET,
                 XmDIALOG_HELP_BUTTON ),
                 XmNactivateCallback, CallbackEnterLibrary, NULL );
  rdsend();
}
