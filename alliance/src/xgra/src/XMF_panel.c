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
| Tool    :                    XGRA                           |
|                                                             |
| File    :                   Panel.c                         |
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
# include "aut.h"
# include "XSB.h"
# include "XMX.h"
# include "XTB.h"
# include "XMF.h"

# include "XMF_panel.h"
# include "XMF_dialog.h"

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

   XgraPanelItem XgraLibraryPanel =

         {
           "Library",
           1,
           0,
           XGRA_LIBRARY_X,
           XGRA_LIBRARY_Y,
           250,
           375,
           0,
           0,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (XgraPanelButtonItem *)NULL
         };

   int XgraLibraryDefaultValues[ 5 ] =

         {
           XGRA_LIBRARY_X,
           XGRA_LIBRARY_Y,
           250, 375, 0
         };

   static char **XGRA_CATA_LIB    = (char **)NULL;
   static char  *XGRA_WORK_LIB    = (char  *)NULL;
   static char  *XGRA_DEFAULT_LIB = (char *)NULL;
 
   static char   XgraFirstLibrary = 1;

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

  Widget     XgraLibraryListLabel;
  Widget     XgraLibraryList;

  Widget     XgraLibraryButtonOk;
  Widget     XgraLibraryButtonUpdate;
  Widget     XgraLibraryButtonCancel;

  Widget     XgraLibraryCataLabel;
  Widget     XgraLibraryCata;

  Widget     XgraLibraryWorkLabel;
  Widget     XgraLibraryWork;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         XgraIsDirectory                     |
|                                                             |
\------------------------------------------------------------*/

char XgraIsDirectory( FileName )

   char *FileName;
{
  struct stat Buffer;

  autbegin();

  if ( stat( FileName, &Buffer ) != -1 )
  {
    if ( ( Buffer.st_mode & S_IFMT ) == S_IFDIR ) 
    {
      autend();
      return( 1 );
    }
  }

  autend();
  return( 0 );
}
 
/*------------------------------------------------------------\
|                                                             |
|                       XgraSetLibraryList                    |
|                                                             |
\------------------------------------------------------------*/

void XgraSetLibraryList()
{
  int      Index;
  XmString Label;

  autbegin();

  XmListDeleteAllItems( XgraLibraryList );

  for ( Index = 0;
        XGRA_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    Label = XmStringCreateSimple( XGRA_CATA_LIB[ Index ] );
    XmListAddItem( XgraLibraryList , Label , NULL );
    XmStringFree( Label );
  }

  Label = XmStringCreateSimple( XGRA_WORK_LIB );
  XmListAddItem( XgraLibraryList , Label , NULL );
  XmStringFree( Label );

  XGRA_DEFAULT_LIB = (char *)NULL;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XgraSetWorkLibrary                    |
|                                                             |
\------------------------------------------------------------*/

void XgraSetWorkLibrary()
{
  autbegin();

  XmTextSetString( XgraLibraryWork, XGRA_WORK_LIB );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XgraSetCataLibrary                    |
|                                                             |
\------------------------------------------------------------*/

void XgraSetCataLibrary()
{
  int   Index;
  long  Length;
  char *Buffer;
  char *Scan;

  autbegin();

  for ( Index = 0, Length = 0; 
        XGRA_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    Length = Length + strlen( XGRA_CATA_LIB[ Index ] ) + 1;
  }

  Buffer = (char *)autallocblock( Length + 1 );
  Buffer[ 0 ] = '\0';
  Scan   = Buffer;

  for ( Index = 0;
        XGRA_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    strcpy( Scan, XGRA_CATA_LIB[ Index ] );
    strcat( Scan, "\n" );

    Scan = Scan + strlen( Scan );
  }

  XmTextSetString( XgraLibraryCata, Buffer );

  autfreeblock( Buffer );

  autend();
}
 
/*------------------------------------------------------------\
|                                                             |
|                      XgraInitializeLibrary                  |
|                                                             |
\------------------------------------------------------------*/

void XgraInitializeLibrary()
{
  int Index;

  autbegin();

  for ( Index = 0; CATA_LIB[ Index ] != (char *)NULL; Index++ );

  XGRA_CATA_LIB = (char **)autallocblock( sizeof(char *) * ( Index + 1 ));

  for ( Index = 0; 
        CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    XGRA_CATA_LIB[ Index ] = autallocblock( strlen( CATA_LIB[ Index ] ) + 1 );
    strcpy( XGRA_CATA_LIB[ Index ], CATA_LIB[ Index ] );
  }

  XGRA_CATA_LIB[ Index ] = (char *)NULL;

  XGRA_WORK_LIB = autallocblock( strlen( WORK_LIB ) + 1 );
  strcpy( XGRA_WORK_LIB, WORK_LIB );

  XgraSetCataLibrary();
  XgraSetWorkLibrary();
  XgraSetLibraryList();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XgraGetCataLibrary                     |
|                                                             |
\------------------------------------------------------------*/

 void XgraGetCataLibrary()
 {
  char *NewCata;
  char *Scan;
  int   Index;
  int   Count;
  int   Length;

  autbegin();

  for ( Index = 0; 
        XGRA_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    autfreeblock( XGRA_CATA_LIB[ Index ] );
  }

  autfreeblock( XGRA_CATA_LIB );

  NewCata = XmTextGetString( XgraLibraryCata );

  Count = 1;

  for ( Index = 0; NewCata[ Index ] != '\0'; Index++ ) 
  {
    if ( NewCata[ Index ] == '\n' )
    {
      Count = Count + 1;
    }
  }

  XGRA_CATA_LIB = (char **)autallocblock( sizeof(char *) * ( Count + 1 ));

  Count = 0;
  Scan  = NewCata;

  for ( Index = 0; NewCata[ Index ] != '\0'; Index++ )
  {
    if ( NewCata[ Index ] == '\n' ) 
    {
      NewCata[ Index ] = '\0';

      Length = strlen( Scan );

      if ( ( Length > 0               ) &&
           ( XgraIsDirectory( Scan ) ) )
      {
        XGRA_CATA_LIB[ Count ] = autallocblock( Length + 1 );
        strcpy( XGRA_CATA_LIB[ Count ], Scan );
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
    XGRA_CATA_LIB[ Count ] = autallocblock( Length + 1 );
    strcpy( XGRA_CATA_LIB[ Count ], Scan );
    Count = Count + 1;
  }

  XGRA_CATA_LIB[ Count ] = (char *)NULL;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XgraGetWorkLibrary                     |
|                                                             |
\------------------------------------------------------------*/

void XgraGetWorkLibrary()
{
  char *NewWork;

  autbegin();

  autfreeblock( XGRA_WORK_LIB );

  NewWork = XmTextGetString( XgraLibraryWork );

  if ( XgraIsDirectory( NewWork ) )
  {
    XGRA_WORK_LIB = autallocblock( strlen( NewWork ) + 1 );
    strcpy( XGRA_WORK_LIB, NewWork );
  }

  autend();
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
  autbegin();

  XGRA_DEFAULT_LIB = XGRA_CATA_LIB[ CallData->item_position - 1 ];

  if ( XGRA_DEFAULT_LIB == (char *)NULL )
  {
    XGRA_DEFAULT_LIB = XGRA_WORK_LIB;
  }

  autend();
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

  autbegin();

  XgraExitPanel( &XgraLibraryPanel );
 
  XgraExitDialog();

  if ( XgraFirstLibrary == 0 )
  {
    for ( Index = 0;
          CATA_LIB[ Index ] != (char *)NULL;
          Index++ )
    {
      autfreeblock( CATA_LIB[ Index ] );
    }

    autfreeblock( CATA_LIB );
    autfreeblock( WORK_LIB );
  }

  XgraFirstLibrary = 0;

  CATA_LIB = XGRA_CATA_LIB;
  WORK_LIB = XGRA_WORK_LIB;

  if ( XGRA_DEFAULT_LIB != (char *)NULL )
  {
    Label = XmStringCreateSimple( XGRA_DEFAULT_LIB );

    XtVaSetValues( XgraFileOpenDialog.WIDGET,
                   XmNdirectory, Label, NULL );
 
    XmStringFree( Label );
  }

  autend();
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
  autbegin();

  XgraGetCataLibrary();
  XgraGetWorkLibrary();
  XgraSetLibraryList();

  autend();
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

  autbegin();

  for ( Index = 0;
        XGRA_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    autfreeblock( XGRA_CATA_LIB[ Index ] );
  }

  autfreeblock( XGRA_CATA_LIB );
  autfreeblock( XGRA_WORK_LIB );

  XgraExitPanel( &XgraLibraryPanel );

  XgraExitDialog();

  autend();
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
  autbegin();

  XgraInitializeLibrary();

  XgraEnterPanel( &XgraLibraryPanel );
  XgraLimitedLoop( XgraLibraryPanel.PANEL );
  XgraReEnterDialog();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XgraBuildPanelLibrary                 |
|                                                             |
\------------------------------------------------------------*/

void XgraBuildPanelLibrary()
{
  Arg      Args[15];
  XmString Label;

  autbegin();

  XtSetArg( Args[0], XmNshadowType    , XmSHADOW_ETCHED_IN      );
  XtSetArg( Args[1], XmNdeleteResponse,          XmDESTROY      );
  XtSetArg( Args[2], XmNtitle         , XgraLibraryPanel.TITLE );

  XgraLibraryPanel.PANEL = 

    XmCreateFormDialog( XgraMainWindow, "XgraLibraryPanel", Args, 3);

  XtAddCallback( XgraLibraryPanel.PANEL, XmNdestroyCallback,
                 XgraDestroyDialogCallback, NULL  );

  XgraLibraryPanel.PANEL_FORM = 

    XtVaCreateManagedWidget( "XgraLibraryPanelForm",
                             xmFormWidgetClass,
                             XgraLibraryPanel.PANEL,
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             XmNfractionBase     , 10,
                             NULL
                           );
 
  XgraLibraryPanel.FRAME = 

    XtVaCreateManagedWidget( "XgraLibraryFrame",
                             xmFrameWidgetClass,
                             XgraLibraryPanel.PANEL_FORM, 
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             NULL
                           );
 
  XgraLibraryPanel.FORM = 

    XtVaCreateManagedWidget( "XgraLibraryForm",
                             xmFormWidgetClass,
                             XgraLibraryPanel.FRAME, 
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             XmNfractionBase     , 120,
                             NULL
                           );

  XgraLibraryButtonUpdate =

    XtVaCreateManagedWidget( "Update",
                             xmPushButtonWidgetClass,
                             XgraLibraryPanel.FORM,
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

  XgraLibraryButtonOk = 

    XtVaCreateManagedWidget( "Ok",
                             xmPushButtonWidgetClass,
                             XgraLibraryPanel.FORM, 
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

  XgraLibraryButtonCancel = 

    XtVaCreateManagedWidget( "Cancel",
                             xmPushButtonWidgetClass,
                             XgraLibraryPanel.FORM, 
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

  XgraLibraryListLabel = 

     XtVaCreateManagedWidget( "XgraLibraryListLabel",
                              xmLabelGadgetClass      , 
                              XgraLibraryPanel.FORM, 
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

  XgraLibraryList = 

    XmCreateScrolledList( XgraLibraryPanel.FORM,
                          "XgraLibraryList", Args, 12 );

  Label = XmStringCreateSimple( "Catalog libraries" );

  XgraLibraryCataLabel = 

     XtVaCreateManagedWidget( "XgraLibraryCataLabel",
                              xmLabelGadgetClass      ,
                              XgraLibraryPanel.FORM,
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

  XgraLibraryCata  = 
 
    XmCreateScrolledText( XgraLibraryPanel.FORM,
                          "XgraLibraryCata", Args, 14 );

  Label = XmStringCreateSimple( "Work library" );

  XgraLibraryWorkLabel =

     XtVaCreateManagedWidget( "XgraLibraryWorkLabel",
                              xmLabelGadgetClass      ,
                              XgraLibraryPanel.FORM,
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

  XgraLibraryWork  =

    XmCreateText( XgraLibraryPanel.FORM,
                  "XgraLibraryWork", Args, 14 );

  XtManageChild( XgraLibraryWork );
  XtManageChild( XgraLibraryList );
  XtManageChild( XgraLibraryCata );

  XtAddCallback( XgraLibraryList,
                 XmNdefaultActionCallback,
                 CallbackLibraryList, NULL );

  XtAddCallback( XgraLibraryButtonOk, 
                 XmNactivateCallback,
                 CallbackLibraryOk, NULL );

  XtAddCallback( XgraLibraryButtonCancel, 
                 XmNactivateCallback,
                 CallbackLibraryCancel, NULL );

  XtAddCallback( XgraLibraryButtonUpdate, 
                 XmNactivateCallback,
                 CallbackLibraryUpdate, NULL );

  XtVaSetValues( XgraLibraryPanel.PANEL,
                 XmNheight, XgraLibraryPanel.HEIGHT,
                 XmNwidth , XgraLibraryPanel.WIDTH,
                 XmNx     , XgraLibraryPanel.X,
                 XmNy     , XgraLibraryPanel.Y,
                 NULL );

  Label = XmStringCreateSimple( "Library" );

  XtVaSetValues( XmFileSelectionBoxGetChild( XgraFileOpenDialog.WIDGET,
                 XmDIALOG_HELP_BUTTON ),
                 XmNlabelString, Label, NULL  );

  XmStringFree( Label );

  XtAddCallback( XmFileSelectionBoxGetChild( XgraFileOpenDialog.WIDGET,
                 XmDIALOG_HELP_BUTTON ),
                 XmNactivateCallback, 
                 CallbackEnterLibrary, NULL );

  autend();
}
