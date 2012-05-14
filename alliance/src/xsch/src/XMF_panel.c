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
| Tool    :                    XSCH                           |
|                                                             |
| File    :                   Panel.c                         |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   01.06.96                        |
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
# include "mlo.h"
# include "scl.h"
# include "XSB.h"
# include "XSC.h"
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

   XschPanelItem XschLibraryPanel =

         {
           "Library",
           1,
           0,
           XSCH_LIBRARY_X,
           XSCH_LIBRARY_Y,
           250,
           375,
           0,
           0,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (XschPanelButtonItem *)NULL
         };

   int XschLibraryDefaultValues[ 5 ] =

         {
           XSCH_LIBRARY_X,
           XSCH_LIBRARY_Y,
           250, 375, 0
         };

   static char **XSCH_CATA_LIB    = (char **)NULL;
   static char  *XSCH_WORK_LIB    = (char  *)NULL;
   static char  *XSCH_DEFAULT_LIB = (char *)NULL;
 
   static char   XschFirstLibrary = 1;

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

  Widget     XschLibraryListLabel;
  Widget     XschLibraryList;

  Widget     XschLibraryButtonOk;
  Widget     XschLibraryButtonUpdate;
  Widget     XschLibraryButtonCancel;

  Widget     XschLibraryCataLabel;
  Widget     XschLibraryCata;

  Widget     XschLibraryWorkLabel;
  Widget     XschLibraryWork;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         XschIsDirectory                     |
|                                                             |
\------------------------------------------------------------*/

char XschIsDirectory( FileName )

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
|                       XschSetLibraryList                    |
|                                                             |
\------------------------------------------------------------*/

void XschSetLibraryList()
{
  int      Index;
  XmString Label;

  autbegin();

  XmListDeleteAllItems( XschLibraryList );

  for ( Index = 0;
        XSCH_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    Label = XmStringCreateSimple( XSCH_CATA_LIB[ Index ] );
    XmListAddItem( XschLibraryList , Label , 0 );
    XmStringFree( Label );
  }

  Label = XmStringCreateSimple( XSCH_WORK_LIB );
  XmListAddItem( XschLibraryList , Label , 0 );
  XmStringFree( Label );

  XSCH_DEFAULT_LIB = (char *)NULL;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XschSetWorkLibrary                    |
|                                                             |
\------------------------------------------------------------*/

void XschSetWorkLibrary()
{
  autbegin();

  XmTextSetString( XschLibraryWork, XSCH_WORK_LIB );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XschSetCataLibrary                    |
|                                                             |
\------------------------------------------------------------*/

void XschSetCataLibrary()
{
  int   Index;
  long  Length;
  char *Buffer;
  char *Scan;

  autbegin();

  for ( Index = 0, Length = 0; 
        XSCH_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    Length = Length + strlen( XSCH_CATA_LIB[ Index ] ) + 1;
  }

  Buffer = (char *)autallocblock( Length + 1 );
  Buffer[ 0 ] = '\0';
  Scan   = Buffer;

  for ( Index = 0;
        XSCH_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    strcpy( Scan, XSCH_CATA_LIB[ Index ] );
    strcat( Scan, "\n" );

    Scan = Scan + strlen( Scan );
  }

  XmTextSetString( XschLibraryCata, Buffer );

  autfreeblock( Buffer );

  autend();
}
 
/*------------------------------------------------------------\
|                                                             |
|                      XschInitializeLibrary                  |
|                                                             |
\------------------------------------------------------------*/

void XschInitializeLibrary()
{
  int Index;

  autbegin();

  for ( Index = 0; CATA_LIB[ Index ] != (char *)NULL; Index++ );

  XSCH_CATA_LIB = (char **)autallocblock( sizeof(char *) * ( Index + 1 ));

  for ( Index = 0; 
        CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    XSCH_CATA_LIB[ Index ] = autallocblock( strlen( CATA_LIB[ Index ] ) + 1 );
    strcpy( XSCH_CATA_LIB[ Index ], CATA_LIB[ Index ] );
  }

  XSCH_CATA_LIB[ Index ] = (char *)NULL;

  XSCH_WORK_LIB = autallocblock( strlen( WORK_LIB ) + 1 );
  strcpy( XSCH_WORK_LIB, WORK_LIB );

  XschSetCataLibrary();
  XschSetWorkLibrary();
  XschSetLibraryList();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XschGetCataLibrary                     |
|                                                             |
\------------------------------------------------------------*/

 void XschGetCataLibrary()
 {
  char *NewCata;
  char *Scan;
  int   Index;
  int   Count;
  int   Length;

  autbegin();

  for ( Index = 0; 
        XSCH_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    autfreeblock( XSCH_CATA_LIB[ Index ] );
  }

  autfreeblock( XSCH_CATA_LIB );

  NewCata = XmTextGetString( XschLibraryCata );

  Count = 1;

  for ( Index = 0; NewCata[ Index ] != '\0'; Index++ ) 
  {
    if ( NewCata[ Index ] == '\n' )
    {
      Count = Count + 1;
    }
  }

  XSCH_CATA_LIB = (char **)autallocblock( sizeof(char *) * ( Count + 1 ));

  Count = 0;
  Scan  = NewCata;

  for ( Index = 0; NewCata[ Index ] != '\0'; Index++ )
  {
    if ( NewCata[ Index ] == '\n' ) 
    {
      NewCata[ Index ] = '\0';

      Length = strlen( Scan );

      if ( ( Length > 0               ) &&
           ( XschIsDirectory( Scan ) ) )
      {
        XSCH_CATA_LIB[ Count ] = autallocblock( Length + 1 );
        strcpy( XSCH_CATA_LIB[ Count ], Scan );
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
    XSCH_CATA_LIB[ Count ] = autallocblock( Length + 1 );
    strcpy( XSCH_CATA_LIB[ Count ], Scan );
    Count = Count + 1;
  }

  XSCH_CATA_LIB[ Count ] = (char *)NULL;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XschGetWorkLibrary                     |
|                                                             |
\------------------------------------------------------------*/

void XschGetWorkLibrary()
{
  char *NewWork;

  autbegin();

  autfreeblock( XSCH_WORK_LIB );

  NewWork = XmTextGetString( XschLibraryWork );

  if ( XschIsDirectory( NewWork ) )
  {
    XSCH_WORK_LIB = autallocblock( strlen( NewWork ) + 1 );
    strcpy( XSCH_WORK_LIB, NewWork );
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

  XSCH_DEFAULT_LIB = XSCH_CATA_LIB[ CallData->item_position - 1 ];

  if ( XSCH_DEFAULT_LIB == (char *)NULL )
  {
    XSCH_DEFAULT_LIB = XSCH_WORK_LIB;
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

  XschExitPanel( &XschLibraryPanel );
 
  XschExitDialog();

  if ( XschFirstLibrary == 0 )
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

  XschFirstLibrary = 0;

  CATA_LIB = XSCH_CATA_LIB;
  WORK_LIB = XSCH_WORK_LIB;

  if ( XSCH_DEFAULT_LIB != (char *)NULL )
  {
    Label = XmStringCreateSimple( XSCH_DEFAULT_LIB );

    XtVaSetValues( XschFileOpenDialog.WIDGET,
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

  XschGetCataLibrary();
  XschGetWorkLibrary();
  XschSetLibraryList();

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
        XSCH_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    autfreeblock( XSCH_CATA_LIB[ Index ] );
  }

  autfreeblock( XSCH_CATA_LIB );
  autfreeblock( XSCH_WORK_LIB );

  XschExitPanel( &XschLibraryPanel );

  XschExitDialog();

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

  XschInitializeLibrary();

  XschEnterPanel( &XschLibraryPanel );
  XschLimitedLoop( XschLibraryPanel.PANEL );
  XschReEnterDialog();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XschBuildPanelLibrary                 |
|                                                             |
\------------------------------------------------------------*/

void XschBuildPanelLibrary()
{
  Arg      Args[15];
  XmString Label;

  autbegin();

  XtSetArg( Args[0], XmNshadowType    , XmSHADOW_ETCHED_IN     );
  XtSetArg( Args[1], XmNdeleteResponse, XmDO_NOTHING           );
  XtSetArg( Args[2], XmNtitle         , XschLibraryPanel.TITLE );

  XschLibraryPanel.PANEL = 

    XmCreateFormDialog( XschMainWindow, "XschLibraryPanel", Args, 3);

  XtAddCallback( XschLibraryPanel.PANEL, XmNdestroyCallback,
                 XschDestroyDialogCallback, NULL  );

  XschLibraryPanel.PANEL_FORM = 

    XtVaCreateManagedWidget( "XschLibraryPanelForm",
                             xmFormWidgetClass,
                             XschLibraryPanel.PANEL,
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             XmNfractionBase     , 10,
                             NULL
                           );
 
  XschLibraryPanel.FRAME = 

    XtVaCreateManagedWidget( "XschLibraryFrame",
                             xmFrameWidgetClass,
                             XschLibraryPanel.PANEL_FORM, 
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             NULL
                           );
 
  XschLibraryPanel.FORM = 

    XtVaCreateManagedWidget( "XschLibraryForm",
                             xmFormWidgetClass,
                             XschLibraryPanel.FRAME, 
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             XmNfractionBase     , 120,
                             NULL
                           );

  XschLibraryButtonUpdate =

    XtVaCreateManagedWidget( "Update",
                             xmPushButtonWidgetClass,
                             XschLibraryPanel.FORM,
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

  XschLibraryButtonOk = 

    XtVaCreateManagedWidget( "Ok",
                             xmPushButtonWidgetClass,
                             XschLibraryPanel.FORM, 
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

  XschLibraryButtonCancel = 

    XtVaCreateManagedWidget( "Cancel",
                             xmPushButtonWidgetClass,
                             XschLibraryPanel.FORM, 
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

  XschLibraryListLabel = 

     XtVaCreateManagedWidget( "XschLibraryListLabel",
                              xmLabelGadgetClass      , 
                              XschLibraryPanel.FORM, 
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

  XschLibraryList = 

    XmCreateScrolledList( XschLibraryPanel.FORM,
                          "XschLibraryList", Args, 12 );

  Label = XmStringCreateSimple( "Catalog libraries" );

  XschLibraryCataLabel = 

     XtVaCreateManagedWidget( "XschLibraryCataLabel",
                              xmLabelGadgetClass      ,
                              XschLibraryPanel.FORM,
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

  XschLibraryCata  = 
 
    XmCreateScrolledText( XschLibraryPanel.FORM,
                          "XschLibraryCata", Args, 14 );

  Label = XmStringCreateSimple( "Work library" );

  XschLibraryWorkLabel =

     XtVaCreateManagedWidget( "XschLibraryWorkLabel",
                              xmLabelGadgetClass      ,
                              XschLibraryPanel.FORM,
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

  XschLibraryWork  =

    XmCreateText( XschLibraryPanel.FORM,
                  "XschLibraryWork", Args, 14 );

  XtManageChild( XschLibraryWork );
  XtManageChild( XschLibraryList );
  XtManageChild( XschLibraryCata );

  XtAddCallback( XschLibraryList,
                 XmNdefaultActionCallback,
                 CallbackLibraryList, NULL );

  XtAddCallback( XschLibraryButtonOk, 
                 XmNactivateCallback,
                 CallbackLibraryOk, NULL );

  XtAddCallback( XschLibraryButtonCancel, 
                 XmNactivateCallback,
                 CallbackLibraryCancel, NULL );

  XtAddCallback( XschLibraryButtonUpdate, 
                 XmNactivateCallback,
                 CallbackLibraryUpdate, NULL );

  XtVaSetValues( XschLibraryPanel.PANEL,
                 XmNheight, XschLibraryPanel.HEIGHT,
                 XmNwidth , XschLibraryPanel.WIDTH,
                 XmNx     , XschLibraryPanel.X,
                 XmNy     , XschLibraryPanel.Y,
                 NULL );

  Label = XmStringCreateSimple( "Library" );

  XtVaSetValues( XmFileSelectionBoxGetChild( XschFileOpenDialog.WIDGET,
                 XmDIALOG_HELP_BUTTON ),
                 XmNlabelString, Label, NULL  );

  XmStringFree( Label );

  XtAddCallback( XmFileSelectionBoxGetChild( XschFileOpenDialog.WIDGET,
                 XmDIALOG_HELP_BUTTON ),
                 XmNactivateCallback, 
                 CallbackEnterLibrary, NULL );

  autend();
}
