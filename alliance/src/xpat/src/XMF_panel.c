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
| Tool    :                    XPAT                           |
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
# include "pat.h"
# include "XSB.h"
# include "XPT.h"
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

   XpatPanelItem XpatLibraryPanel =

         {
           "Library",
           1,
           0,
           XPAT_LIBRARY_X,
           XPAT_LIBRARY_Y,
           250,
           375,
           0,
           0,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (XpatPanelButtonItem *)NULL
         };

   int XpatLibraryDefaultValues[ 5 ] =

         {
           XPAT_LIBRARY_X,
           XPAT_LIBRARY_Y,
           250, 375, 0
         };

   static char **XPAT_CATA_LIB    = (char **)NULL;
   static char  *XPAT_WORK_LIB    = (char  *)NULL;
   static char  *XPAT_DEFAULT_LIB = (char *)NULL;
 
   static char   XpatFirstLibrary = 1;

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

  Widget     XpatLibraryListLabel;
  Widget     XpatLibraryList;

  Widget     XpatLibraryButtonOk;
  Widget     XpatLibraryButtonUpdate;
  Widget     XpatLibraryButtonCancel;

  Widget     XpatLibraryCataLabel;
  Widget     XpatLibraryCata;

  Widget     XpatLibraryWorkLabel;
  Widget     XpatLibraryWork;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         XpatIsDirectory                     |
|                                                             |
\------------------------------------------------------------*/

char XpatIsDirectory( FileName )

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
|                       XpatSetLibraryList                    |
|                                                             |
\------------------------------------------------------------*/

void XpatSetLibraryList()
{
  int      Index;
  XmString Label;

  autbegin();

  XmListDeleteAllItems( XpatLibraryList );

  for ( Index = 0;
        XPAT_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    Label = XmStringCreateSimple( XPAT_CATA_LIB[ Index ] );
    XmListAddItem( XpatLibraryList , Label , NULL );
    XmStringFree( Label );
  }

  Label = XmStringCreateSimple( XPAT_WORK_LIB );
  XmListAddItem( XpatLibraryList , Label , NULL );
  XmStringFree( Label );

  XPAT_DEFAULT_LIB = (char *)NULL;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XpatSetWorkLibrary                    |
|                                                             |
\------------------------------------------------------------*/

void XpatSetWorkLibrary()
{
  autbegin();

  XmTextSetString( XpatLibraryWork, XPAT_WORK_LIB );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XpatSetCataLibrary                    |
|                                                             |
\------------------------------------------------------------*/

void XpatSetCataLibrary()
{
  int   Index;
  long  Length;
  char *Buffer;
  char *Scan;

  autbegin();

  for ( Index = 0, Length = 0; 
        XPAT_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    Length = Length + strlen( XPAT_CATA_LIB[ Index ] ) + 1;
  }

  Buffer = (char *)autallocblock( Length + 1 );
  Buffer[ 0 ] = '\0';
  Scan   = Buffer;

  for ( Index = 0;
        XPAT_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    strcpy( Scan, XPAT_CATA_LIB[ Index ] );
    strcat( Scan, "\n" );

    Scan = Scan + strlen( Scan );
  }

  XmTextSetString( XpatLibraryCata, Buffer );

  autfreeblock( Buffer );

  autend();
}
 
/*------------------------------------------------------------\
|                                                             |
|                      XpatInitializeLibrary                  |
|                                                             |
\------------------------------------------------------------*/

void XpatInitializeLibrary()
{
  int Index;

  autbegin();

  for ( Index = 0; CATA_LIB[ Index ] != (char *)NULL; Index++ );

  XPAT_CATA_LIB = (char **)autallocblock( sizeof(char *) * ( Index + 1 ));

  for ( Index = 0; 
        CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    XPAT_CATA_LIB[ Index ] = autallocblock( strlen( CATA_LIB[ Index ] ) + 1 );
    strcpy( XPAT_CATA_LIB[ Index ], CATA_LIB[ Index ] );
  }

  XPAT_CATA_LIB[ Index ] = (char *)NULL;

  XPAT_WORK_LIB = autallocblock( strlen( WORK_LIB ) + 1 );
  strcpy( XPAT_WORK_LIB, WORK_LIB );

  XpatSetCataLibrary();
  XpatSetWorkLibrary();
  XpatSetLibraryList();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XpatGetCataLibrary                     |
|                                                             |
\------------------------------------------------------------*/

 void XpatGetCataLibrary()
 {
  char *NewCata;
  char *Scan;
  int   Index;
  int   Count;
  int   Length;

  autbegin();

  for ( Index = 0; 
        XPAT_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    autfreeblock( XPAT_CATA_LIB[ Index ] );
  }

  autfreeblock( XPAT_CATA_LIB );

  NewCata = XmTextGetString( XpatLibraryCata );

  Count = 1;

  for ( Index = 0; NewCata[ Index ] != '\0'; Index++ ) 
  {
    if ( NewCata[ Index ] == '\n' )
    {
      Count = Count + 1;
    }
  }

  XPAT_CATA_LIB = (char **)autallocblock( sizeof(char *) * ( Count + 1 ));

  Count = 0;
  Scan  = NewCata;

  for ( Index = 0; NewCata[ Index ] != '\0'; Index++ )
  {
    if ( NewCata[ Index ] == '\n' ) 
    {
      NewCata[ Index ] = '\0';

      Length = strlen( Scan );

      if ( ( Length > 0               ) &&
           ( XpatIsDirectory( Scan ) ) )
      {
        XPAT_CATA_LIB[ Count ] = autallocblock( Length + 1 );
        strcpy( XPAT_CATA_LIB[ Count ], Scan );
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
    XPAT_CATA_LIB[ Count ] = autallocblock( Length + 1 );
    strcpy( XPAT_CATA_LIB[ Count ], Scan );
    Count = Count + 1;
  }

  XPAT_CATA_LIB[ Count ] = (char *)NULL;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XpatGetWorkLibrary                     |
|                                                             |
\------------------------------------------------------------*/

void XpatGetWorkLibrary()
{
  char *NewWork;

  autbegin();

  autfreeblock( XPAT_WORK_LIB );

  NewWork = XmTextGetString( XpatLibraryWork );

  if ( XpatIsDirectory( NewWork ) )
  {
    XPAT_WORK_LIB = autallocblock( strlen( NewWork ) + 1 );
    strcpy( XPAT_WORK_LIB, NewWork );
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

  XPAT_DEFAULT_LIB = XPAT_CATA_LIB[ CallData->item_position - 1 ];

  if ( XPAT_DEFAULT_LIB == (char *)NULL )
  {
    XPAT_DEFAULT_LIB = XPAT_WORK_LIB;
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

  XpatExitPanel( &XpatLibraryPanel );
 
  XpatExitDialog();

  if ( XpatFirstLibrary == 0 )
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

  XpatFirstLibrary = 0;

  CATA_LIB = XPAT_CATA_LIB;
  WORK_LIB = XPAT_WORK_LIB;

  if ( XPAT_DEFAULT_LIB != (char *)NULL )
  {
    Label = XmStringCreateSimple( XPAT_DEFAULT_LIB );

    XtVaSetValues( XpatFileOpenDialog.WIDGET,
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

  XpatGetCataLibrary();
  XpatGetWorkLibrary();
  XpatSetLibraryList();

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
        XPAT_CATA_LIB[ Index ] != (char *)NULL;
        Index++ )
  {
    autfreeblock( XPAT_CATA_LIB[ Index ] );
  }

  autfreeblock( XPAT_CATA_LIB );
  autfreeblock( XPAT_WORK_LIB );

  XpatExitPanel( &XpatLibraryPanel );

  XpatExitDialog();

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

  XpatInitializeLibrary();

  XpatEnterPanel( &XpatLibraryPanel );
  XpatLimitedLoop( XpatLibraryPanel.PANEL );
  XpatReEnterDialog();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XpatBuildPanelLibrary                 |
|                                                             |
\------------------------------------------------------------*/

void XpatBuildPanelLibrary()
{
  Arg      Args[15];
  XmString Label;

  autbegin();

  XtSetArg( Args[0], XmNshadowType    , XmSHADOW_ETCHED_IN     );
  XtSetArg( Args[1], XmNdeleteResponse, XmDO_NOTHING           );
  XtSetArg( Args[2], XmNtitle         , XpatLibraryPanel.TITLE );

  XpatLibraryPanel.PANEL = 

    XmCreateFormDialog( XpatMainWindow, "XpatLibraryPanel", Args, 3);

  XtAddCallback( XpatLibraryPanel.PANEL, XmNdestroyCallback,
                 XpatDestroyDialogCallback, NULL  );

  XpatLibraryPanel.PANEL_FORM = 

    XtVaCreateManagedWidget( "XpatLibraryPanelForm",
                             xmFormWidgetClass,
                             XpatLibraryPanel.PANEL,
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             XmNfractionBase     , 10,
                             NULL
                           );
 
  XpatLibraryPanel.FRAME = 

    XtVaCreateManagedWidget( "XpatLibraryFrame",
                             xmFrameWidgetClass,
                             XpatLibraryPanel.PANEL_FORM, 
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             NULL
                           );
 
  XpatLibraryPanel.FORM = 

    XtVaCreateManagedWidget( "XpatLibraryForm",
                             xmFormWidgetClass,
                             XpatLibraryPanel.FRAME, 
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             XmNfractionBase     , 120,
                             NULL
                           );

  XpatLibraryButtonUpdate =

    XtVaCreateManagedWidget( "Update",
                             xmPushButtonWidgetClass,
                             XpatLibraryPanel.FORM,
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

  XpatLibraryButtonOk = 

    XtVaCreateManagedWidget( "Ok",
                             xmPushButtonWidgetClass,
                             XpatLibraryPanel.FORM, 
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

  XpatLibraryButtonCancel = 

    XtVaCreateManagedWidget( "Cancel",
                             xmPushButtonWidgetClass,
                             XpatLibraryPanel.FORM, 
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

  XpatLibraryListLabel = 

     XtVaCreateManagedWidget( "XpatLibraryListLabel",
                              xmLabelGadgetClass      , 
                              XpatLibraryPanel.FORM, 
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

  XpatLibraryList = 

    XmCreateScrolledList( XpatLibraryPanel.FORM,
                          "XpatLibraryList", Args, 12 );

  Label = XmStringCreateSimple( "Catalog libraries" );

  XpatLibraryCataLabel = 

     XtVaCreateManagedWidget( "XpatLibraryCataLabel",
                              xmLabelGadgetClass      ,
                              XpatLibraryPanel.FORM,
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

  XpatLibraryCata  = 
 
    XmCreateScrolledText( XpatLibraryPanel.FORM,
                          "XpatLibraryCata", Args, 14 );

  Label = XmStringCreateSimple( "Work library" );

  XpatLibraryWorkLabel =

     XtVaCreateManagedWidget( "XpatLibraryWorkLabel",
                              xmLabelGadgetClass      ,
                              XpatLibraryPanel.FORM,
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

  XpatLibraryWork  =

    XmCreateText( XpatLibraryPanel.FORM,
                  "XpatLibraryWork", Args, 14 );

  XtManageChild( XpatLibraryWork );
  XtManageChild( XpatLibraryList );
  XtManageChild( XpatLibraryCata );

  XtAddCallback( XpatLibraryList,
                 XmNdefaultActionCallback,
                 CallbackLibraryList, NULL );

  XtAddCallback( XpatLibraryButtonOk, 
                 XmNactivateCallback,
                 CallbackLibraryOk, NULL );

  XtAddCallback( XpatLibraryButtonCancel, 
                 XmNactivateCallback,
                 CallbackLibraryCancel, NULL );

  XtAddCallback( XpatLibraryButtonUpdate, 
                 XmNactivateCallback,
                 CallbackLibraryUpdate, NULL );

  XtVaSetValues( XpatLibraryPanel.PANEL,
                 XmNheight, XpatLibraryPanel.HEIGHT,
                 XmNwidth , XpatLibraryPanel.WIDTH,
                 XmNx     , XpatLibraryPanel.X,
                 XmNy     , XpatLibraryPanel.Y,
                 NULL );

  Label = XmStringCreateSimple( "Library" );

  XtVaSetValues( XmFileSelectionBoxGetChild( XpatFileOpenDialog.WIDGET,
                 XmDIALOG_HELP_BUTTON ),
                 XmNlabelString, Label, NULL  );

  XmStringFree( Label );

  XtAddCallback( XmFileSelectionBoxGetChild( XpatFileOpenDialog.WIDGET,
                 XmDIALOG_HELP_BUTTON ),
                 XmNactivateCallback, 
                 CallbackEnterLibrary, NULL );

  autend();
}
