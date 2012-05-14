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
| Tool    :                    XFSM                           |
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
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "XSB.h"
# include "XFS.h"
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

   XfsmPanelItem XfsmLibraryPanel =

         {
           "Library",
           1,
           0,
           XFSM_LIBRARY_X,
           XFSM_LIBRARY_Y,
           250,
           375,
           0,
           0,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (XfsmPanelButtonItem *)NULL
         };

   int XfsmLibraryDefaultValues[ 5 ] =

         {
           XFSM_LIBRARY_X,
           XFSM_LIBRARY_Y,
           250, 375, 0
         };

   static char **XFSM_CATA_LIB    = (char **)NULL;
   static char  *XFSM_WORK_LIB    = (char  *)NULL;
   static char  *XFSM_DEFAULT_LIB = (char *)NULL;
 
   static char   XfsmFirstLibrary = 1;

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

  Widget     XfsmLibraryListLabel;
  Widget     XfsmLibraryList;

  Widget     XfsmLibraryButtonOk;
  Widget     XfsmLibraryButtonUpdate;
  Widget     XfsmLibraryButtonCancel;

  Widget     XfsmLibraryCataLabel;
  Widget     XfsmLibraryCata;

  Widget     XfsmLibraryWorkLabel;
  Widget     XfsmLibraryWork;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         XfsmIsDirectory                     |
|                                                             |
\------------------------------------------------------------*/

char XfsmIsDirectory( FileName )

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
|                       XfsmSetLibraryList                    |
|                                                             |
\------------------------------------------------------------*/

void XfsmSetLibraryList()
{
  int      Index;
  XmString Label;

  autbegin();

  XmListDeleteAllItems( XfsmLibraryList );

  for ( Index = 0;
        XFSM_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    Label = XmStringCreateSimple( XFSM_CATA_LIB[ Index ] );
    XmListAddItem( XfsmLibraryList , Label , 0 );
    XmStringFree( Label );
  }

  Label = XmStringCreateSimple( XFSM_WORK_LIB );
  XmListAddItem( XfsmLibraryList , Label , 0 );
  XmStringFree( Label );

  XFSM_DEFAULT_LIB = (char *)NULL;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XfsmSetWorkLibrary                    |
|                                                             |
\------------------------------------------------------------*/

void XfsmSetWorkLibrary()
{
  autbegin();

  XmTextSetString( XfsmLibraryWork, XFSM_WORK_LIB );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XfsmSetCataLibrary                    |
|                                                             |
\------------------------------------------------------------*/

void XfsmSetCataLibrary()
{
  int   Index;
  long  Length;
  char *Buffer;
  char *Scan;

  autbegin();

  for ( Index = 0, Length = 0; 
        XFSM_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    Length = Length + strlen( XFSM_CATA_LIB[ Index ] ) + 1;
  }

  Buffer = (char *)autallocblock( Length + 1 );
  Buffer[ 0 ] = '\0';
  Scan   = Buffer;

  for ( Index = 0;
        XFSM_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    strcpy( Scan, XFSM_CATA_LIB[ Index ] );
    strcat( Scan, "\n" );

    Scan = Scan + strlen( Scan );
  }

  XmTextSetString( XfsmLibraryCata, Buffer );

  autfreeblock( Buffer );

  autend();
}
 
/*------------------------------------------------------------\
|                                                             |
|                      XfsmInitializeLibrary                  |
|                                                             |
\------------------------------------------------------------*/

void XfsmInitializeLibrary()
{
  int Index;

  autbegin();

  for ( Index = 0; CATA_LIB[ Index ] != (char *)NULL; Index++ );

  XFSM_CATA_LIB = (char **)autallocblock( sizeof(char *) * ( Index + 1 ));

  for ( Index = 0; 
        CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    XFSM_CATA_LIB[ Index ] = autallocblock( strlen( CATA_LIB[ Index ] ) + 1 );
    strcpy( XFSM_CATA_LIB[ Index ], CATA_LIB[ Index ] );
  }

  XFSM_CATA_LIB[ Index ] = (char *)NULL;

  XFSM_WORK_LIB = autallocblock( strlen( WORK_LIB ) + 1 );
  strcpy( XFSM_WORK_LIB, WORK_LIB );

  XfsmSetCataLibrary();
  XfsmSetWorkLibrary();
  XfsmSetLibraryList();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XfsmGetCataLibrary                     |
|                                                             |
\------------------------------------------------------------*/

 void XfsmGetCataLibrary()
 {
  char *NewCata;
  char *Scan;
  int   Index;
  int   Count;
  int   Length;

  autbegin();

  for ( Index = 0; 
        XFSM_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    autfreeblock( XFSM_CATA_LIB[ Index ] );
  }

  autfreeblock( XFSM_CATA_LIB );

  NewCata = XmTextGetString( XfsmLibraryCata );

  Count = 1;

  for ( Index = 0; NewCata[ Index ] != '\0'; Index++ ) 
  {
    if ( NewCata[ Index ] == '\n' )
    {
      Count = Count + 1;
    }
  }

  XFSM_CATA_LIB = (char **)autallocblock( sizeof(char *) * ( Count + 1 ));

  Count = 0;
  Scan  = NewCata;

  for ( Index = 0; NewCata[ Index ] != '\0'; Index++ )
  {
    if ( NewCata[ Index ] == '\n' ) 
    {
      NewCata[ Index ] = '\0';

      Length = strlen( Scan );

      if ( ( Length > 0               ) &&
           ( XfsmIsDirectory( Scan ) ) )
      {
        XFSM_CATA_LIB[ Count ] = autallocblock( Length + 1 );
        strcpy( XFSM_CATA_LIB[ Count ], Scan );
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
    XFSM_CATA_LIB[ Count ] = autallocblock( Length + 1 );
    strcpy( XFSM_CATA_LIB[ Count ], Scan );
    Count = Count + 1;
  }

  XFSM_CATA_LIB[ Count ] = (char *)NULL;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XfsmGetWorkLibrary                     |
|                                                             |
\------------------------------------------------------------*/

void XfsmGetWorkLibrary()
{
  char *NewWork;

  autbegin();

  autfreeblock( XFSM_WORK_LIB );

  NewWork = XmTextGetString( XfsmLibraryWork );

  if ( XfsmIsDirectory( NewWork ) )
  {
    XFSM_WORK_LIB = autallocblock( strlen( NewWork ) + 1 );
    strcpy( XFSM_WORK_LIB, NewWork );
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

  XFSM_DEFAULT_LIB = XFSM_CATA_LIB[ CallData->item_position - 1 ];

  if ( XFSM_DEFAULT_LIB == (char *)NULL )
  {
    XFSM_DEFAULT_LIB = XFSM_WORK_LIB;
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

  XfsmExitPanel( &XfsmLibraryPanel );
 
  XfsmExitDialog();

  if ( XfsmFirstLibrary == 0 )
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

  XfsmFirstLibrary = 0;

  CATA_LIB = XFSM_CATA_LIB;
  WORK_LIB = XFSM_WORK_LIB;

  if ( XFSM_DEFAULT_LIB != (char *)NULL )
  {
    Label = XmStringCreateSimple( XFSM_DEFAULT_LIB );

    XtVaSetValues( XfsmFileOpenDialog.WIDGET,
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

  XfsmGetCataLibrary();
  XfsmGetWorkLibrary();
  XfsmSetLibraryList();

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
        XFSM_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    autfreeblock( XFSM_CATA_LIB[ Index ] );
  }

  autfreeblock( XFSM_CATA_LIB );
  autfreeblock( XFSM_WORK_LIB );

  XfsmExitPanel( &XfsmLibraryPanel );

  XfsmExitDialog();

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

  XfsmInitializeLibrary();

  XfsmEnterPanel( &XfsmLibraryPanel );
  XfsmLimitedLoop( XfsmLibraryPanel.PANEL );
  XfsmReEnterDialog();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XfsmBuildPanelLibrary                 |
|                                                             |
\------------------------------------------------------------*/

void XfsmBuildPanelLibrary()
{
  Arg      Args[15];
  XmString Label;

  autbegin();

  XtSetArg( Args[0], XmNshadowType    , XmSHADOW_ETCHED_IN      );
  XtSetArg( Args[1], XmNdeleteResponse, XmDO_NOTHING            );
  XtSetArg( Args[2], XmNtitle         , XfsmLibraryPanel.TITLE );

  XfsmLibraryPanel.PANEL = 

    XmCreateFormDialog( XfsmMainWindow, "XfsmLibraryPanel", Args, 3);

  XtAddCallback( XfsmLibraryPanel.PANEL, XmNdestroyCallback,
                 XfsmDestroyDialogCallback, NULL  );

  XfsmLibraryPanel.PANEL_FORM = 

    XtVaCreateManagedWidget( "XfsmLibraryPanelForm",
                             xmFormWidgetClass,
                             XfsmLibraryPanel.PANEL,
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             XmNfractionBase     , 10,
                             NULL
                           );
 
  XfsmLibraryPanel.FRAME = 

    XtVaCreateManagedWidget( "XfsmLibraryFrame",
                             xmFrameWidgetClass,
                             XfsmLibraryPanel.PANEL_FORM, 
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             NULL
                           );
 
  XfsmLibraryPanel.FORM = 

    XtVaCreateManagedWidget( "XfsmLibraryForm",
                             xmFormWidgetClass,
                             XfsmLibraryPanel.FRAME, 
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             XmNfractionBase     , 120,
                             NULL
                           );

  XfsmLibraryButtonUpdate =

    XtVaCreateManagedWidget( "Update",
                             xmPushButtonWidgetClass,
                             XfsmLibraryPanel.FORM,
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

  XfsmLibraryButtonOk = 

    XtVaCreateManagedWidget( "Ok",
                             xmPushButtonWidgetClass,
                             XfsmLibraryPanel.FORM, 
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

  XfsmLibraryButtonCancel = 

    XtVaCreateManagedWidget( "Cancel",
                             xmPushButtonWidgetClass,
                             XfsmLibraryPanel.FORM, 
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

  XfsmLibraryListLabel = 

     XtVaCreateManagedWidget( "XfsmLibraryListLabel",
                              xmLabelGadgetClass      , 
                              XfsmLibraryPanel.FORM, 
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

  XfsmLibraryList = 

    XmCreateScrolledList( XfsmLibraryPanel.FORM,
                          "XfsmLibraryList", Args, 12 );

  Label = XmStringCreateSimple( "Catalog libraries" );

  XfsmLibraryCataLabel = 

     XtVaCreateManagedWidget( "XfsmLibraryCataLabel",
                              xmLabelGadgetClass      ,
                              XfsmLibraryPanel.FORM,
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

  XfsmLibraryCata  = 
 
    XmCreateScrolledText( XfsmLibraryPanel.FORM,
                          "XfsmLibraryCata", Args, 14 );

  Label = XmStringCreateSimple( "Work library" );

  XfsmLibraryWorkLabel =

     XtVaCreateManagedWidget( "XfsmLibraryWorkLabel",
                              xmLabelGadgetClass      ,
                              XfsmLibraryPanel.FORM,
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

  XfsmLibraryWork  =

    XmCreateText( XfsmLibraryPanel.FORM,
                  "XfsmLibraryWork", Args, 14 );

  XtManageChild( XfsmLibraryWork );
  XtManageChild( XfsmLibraryList );
  XtManageChild( XfsmLibraryCata );

  XtAddCallback( XfsmLibraryList,
                 XmNdefaultActionCallback,
                 CallbackLibraryList, NULL );

  XtAddCallback( XfsmLibraryButtonOk, 
                 XmNactivateCallback,
                 CallbackLibraryOk, NULL );

  XtAddCallback( XfsmLibraryButtonCancel, 
                 XmNactivateCallback,
                 CallbackLibraryCancel, NULL );

  XtAddCallback( XfsmLibraryButtonUpdate, 
                 XmNactivateCallback,
                 CallbackLibraryUpdate, NULL );

  XtVaSetValues( XfsmLibraryPanel.PANEL,
                 XmNheight, XfsmLibraryPanel.HEIGHT,
                 XmNwidth , XfsmLibraryPanel.WIDTH,
                 XmNx     , XfsmLibraryPanel.X,
                 XmNy     , XfsmLibraryPanel.Y,
                 NULL );

  Label = XmStringCreateSimple( "Library" );

  XtVaSetValues( XmFileSelectionBoxGetChild( XfsmFileOpenDialog.WIDGET,
                 XmDIALOG_HELP_BUTTON ),
                 XmNlabelString, Label, NULL  );

  XmStringFree( Label );

  XtAddCallback( XmFileSelectionBoxGetChild( XfsmFileOpenDialog.WIDGET,
                 XmDIALOG_HELP_BUTTON ),
                 XmNactivateCallback, 
                 CallbackEnterLibrary, NULL );

  autend();
}
