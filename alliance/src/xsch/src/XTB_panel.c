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
| Tool    :                   XSCH                           |
|                                                             |
| File    :                  panel.c                          |
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
# include <X11/Intrinsic.h>
# include <Xm/Xm.h>
# include <Xm/Frame.h>
# include <Xm/Form.h>
# include <Xm/List.h>
# include <Xm/Text.h>
# include <Xm/PushB.h>
# include <Xm/DialogS.h>

# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "XTB.h"
# include "XSB.h"
# include "XTB_panel.h"
# include "XTB_dialog.h"

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

  static char *XschPanelButtonDisable = "xxxx";

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       XschBuildPanel                       |
|                                                             |
\------------------------------------------------------------*/

void XschBuildPanel( Father, Panel ) 

     Widget           Father;
     XschPanelItem  *Panel;
{
  Arg                   Args[13];
  XschPanelButtonItem *Button;
  char                 *Label;
  int                   Counter;
  int                   Fraction;
  int                   X1;
  int                   Y1;
  int                   X2;
  int                   Y2;
  Pixmap                ButtonPixmap;

  autbegin();

  Fraction = Panel->ROW * Panel->COLUMN;

  XtSetArg( Args[0], XmNshadowType, XmSHADOW_ETCHED_IN ); 
  XtSetArg( Args[1], XmNdeleteResponse, XmDO_NOTHING   );
  XtSetArg( Args[2], XmNtitle         , Panel->TITLE   );

  Panel->PANEL = XmCreateFormDialog( Father, Panel->TITLE, Args, 3); 

  XtAddCallback( Panel->PANEL, XmNdestroyCallback,
                 XschDestroyDialogCallback, NULL );

  Panel->PANEL_FORM = XtVaCreateManagedWidget( "",
                                               xmFormWidgetClass,
                                               Panel->PANEL,
                                               XmNtopAttachment    , XmATTACH_FORM,
                                               XmNbottomAttachment , XmATTACH_FORM,
                                               XmNleftAttachment   , XmATTACH_FORM,
                                               XmNrightAttachment  , XmATTACH_FORM,
                                               XmNfractionBase     , 100,
                                               NULL
                                             );

  Panel->FRAME = XtVaCreateManagedWidget( "",
                                          xmFrameWidgetClass,
                                          Panel->PANEL_FORM,
                                          XmNtopAttachment    , XmATTACH_FORM,
                                          XmNbottomAttachment , XmATTACH_FORM,
                                          XmNleftAttachment   , XmATTACH_FORM,
                                          XmNrightAttachment  , XmATTACH_FORM, 
                                          NULL
                                        );

  Panel->FORM  = XtVaCreateManagedWidget( "",
                                          xmFormWidgetClass,
                                          Panel->FRAME,
                                          XmNtopAttachment    , XmATTACH_FORM,
                                          XmNbottomAttachment , XmATTACH_FORM,
                                          XmNleftAttachment   , XmATTACH_FORM,
                                          XmNrightAttachment  , XmATTACH_FORM,
                                          XmNfractionBase     , Fraction,
                                          NULL
                                        ); 
  Button = Panel->LIST;

  for ( Counter = 0; 
        Button[ Counter ].LABEL != NULL;
        Counter++ )
  {
    X1 = Button[ Counter ].X * Panel->ROW;
    Y1 = Button[ Counter ].Y * Panel->COLUMN;
    X2 = X1 + Button[ Counter ].DX * Panel->ROW;
    Y2 = Y1 + Button[ Counter ].DY * Panel->COLUMN;

    if ( *Button[ Counter ].LABEL[0] != '\0' )
    {
      Label = *Button[ Counter ].LABEL;
    }
    else
    {
      Label = XschPanelButtonDisable; 
    }

    if ( ( Button[ Counter ].TEXT   != (char *)NULL ) &&
         ( Button[ Counter ].BITMAP != (char *)NULL ) )
    {
      XtSetArg( Args[0]  , XmNtopAttachment         , XmATTACH_POSITION );
      XtSetArg( Args[1]  , XmNtopPosition           , Y1 );
      XtSetArg( Args[2]  , XmNbottomAttachment      , XmATTACH_POSITION );
      XtSetArg( Args[3]  , XmNbottomPosition        , Y2 );
      XtSetArg( Args[4]  , XmNrightAttachment       , XmATTACH_POSITION );
      XtSetArg( Args[5]  , XmNrightPosition         , X2 );
      XtSetArg( Args[6]  , XmNleftAttachment        , XmATTACH_POSITION );
      XtSetArg( Args[7]  , XmNleftPosition          , X1 );
      XtSetArg( Args[8]  , XmNscrollBarDisplayPolicy, XmSTATIC );
      XtSetArg( Args[9]  , XmNscrollHorizontal      , True );
      XtSetArg( Args[10] , XmNscrollVertical        , True );
      XtSetArg( Args[11] , XmNlistSizePolicy        , XmCONSTANT );
      XtSetArg( Args[12] , XmNselectionPolicy       , XmMULTIPLE_SELECT );

      Button[ Counter ].BUTTON =

        XmCreateScrolledList( Panel->FORM, Button[ Counter ].TEXT, Args, 13 );

      XtManageChild ( Button[ Counter ].BUTTON );
    }
    else
    if ( Button[ Counter ].TEXT != (char *)NULL )
    {
      XtSetArg( Args[0] , XmNrows                 , 10                );
      XtSetArg( Args[1] , XmNeditable             , False             );
      XtSetArg( Args[2] , XmNeditMode             , XmMULTI_LINE_EDIT );
      XtSetArg( Args[3] , XmNcursorPositionVisible, False             );
      XtSetArg( Args[4] , XmNtopAttachment        , XmATTACH_POSITION );
      XtSetArg( Args[5] , XmNtopPosition          , Y1                );
      XtSetArg( Args[6] , XmNleftAttachment       , XmATTACH_POSITION );
      XtSetArg( Args[7] , XmNleftPosition         , X1                );
      XtSetArg( Args[8] , XmNbottomAttachment     , XmATTACH_POSITION );
      XtSetArg( Args[9] , XmNbottomPosition       , Y2                );
      XtSetArg( Args[10], XmNrightAttachment      , XmATTACH_POSITION );
      XtSetArg( Args[11], XmNrightPosition        , X2                );

      Button[ Counter ].BUTTON = 

        XmCreateScrolledText( Panel->FORM, Button[ Counter ].TEXT, Args, 12);

      XtManageChild ( Button[ Counter ].BUTTON );
    }
    else
    if ( Button[ Counter ].BITMAP != (char *)NULL )
    {
      ButtonPixmap = XschCreatePixmap( Father, 
                                        Button[ Counter ].BITMAP, 
                                        Button[ Counter ].WIDTH,
                                        Button[ Counter ].HEIGHT );
      Button[ Counter ].BUTTON = 

        XtVaCreateManagedWidget( Label,
                                 xmPushButtonWidgetClass,
                                 Panel->FORM,
                                 XmNlabelType        , XmPIXMAP,
                                 XmNlabelPixmap      , ButtonPixmap,
                                 XmNleftAttachment   , XmATTACH_POSITION,
                                 XmNleftPosition     , X1,
                                 XmNrightAttachment  , XmATTACH_POSITION,
                                 XmNrightPosition    , X2,
                                 XmNtopAttachment    , XmATTACH_POSITION,
                                 XmNtopPosition      , Y1,
                                 XmNbottomAttachment , XmATTACH_POSITION,
                                 XmNbottomPosition   , Y2,
                                 NULL
                               );
    }
    else
    {
      Button[ Counter ].BUTTON = 

        XtVaCreateManagedWidget( Label,
                                 xmPushButtonWidgetClass,
                                 Panel->FORM,
                                 XmNleftAttachment   , XmATTACH_POSITION,
                                 XmNleftPosition     , X1,
                                 XmNrightAttachment  , XmATTACH_POSITION,
                                 XmNrightPosition    , X2,
                                 XmNtopAttachment    , XmATTACH_POSITION,
                                 XmNtopPosition      , Y1,
                                 XmNbottomAttachment , XmATTACH_POSITION,
                                 XmNbottomPosition   , Y2,
                                 NULL
                               );
    }


    if ( Label != XschPanelButtonDisable )
    {
      if ( Button[ Counter ].FOREGROUND != NULL )
      {
        XtVaSetValues ( Button[ Counter ].BUTTON              , 
                        XtVaTypedArg                          , XmNforeground, 
                        XmRString                             , 
                        *Button[ Counter ].FOREGROUND         , 
                        strlen ( *Button[ Counter ].FOREGROUND ) + 1,
                        NULL
                      );
      }

      if ( Button[ Counter ].BACKGROUND != NULL )
      {
        XtVaSetValues ( Button[ Counter ].BUTTON              , 
                        XtVaTypedArg                          , XmNbackground, 
                        XmRString                             , 
                        *Button[ Counter ].BACKGROUND         , 
                        strlen ( *Button[ Counter ].BACKGROUND ) + 1,
                        NULL
                      );
      }

      if ( Button[ Counter ].CALLBACK    != NULL )
      {
        if ( ( Button[ Counter ].TEXT   != (char *)NULL ) &&
             ( Button[ Counter ].BITMAP != (char *)NULL ) )
        {
          XtAddCallback( Button[ Counter ].BUTTON,  
                         XmNdefaultActionCallback,
                         Button[ Counter ].CALLBACK,
                         (XtPointer)NULL );

          XtAddCallback( Button[ Counter ].BUTTON,  
                         XmNmultipleSelectionCallback,
                         Button[ Counter ].CALLBACK,
                         Button[ Counter ].CALLDATA );

        }
        else
        {
          XtAddCallback( Button[ Counter ].BUTTON,  
                         XmNactivateCallback, 
                         Button[ Counter ].CALLBACK,
                         Button[ Counter ].CALLDATA );
        }
      }
    }
    else
    {
      Button[ Counter ].BUTTON = NULL;
    }
  }

  XtVaSetValues( Panel->PANEL ,
                 XmNresizable , False,
                 XmNx         , Panel->X,
                 XmNy         , Panel->Y,
                 XmNheight    , Panel->HEIGHT,
                 XmNwidth     , Panel->WIDTH,
                 NULL );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XschEnterPanel                       |
|                                                             |
\------------------------------------------------------------*/

void XschEnterPanel( Panel )
 
   XschPanelItem  *Panel;
{
  autbegin();

  XtManageChild( Panel->PANEL );

  if ( Panel->COMPUTE == 1 )
  {
    XtVaSetValues( Panel->PANEL ,
                   XmNresizable , False,
                   XmNx         , Panel->X,
                   XmNy         , Panel->Y,
                   XmNheight    , Panel->HEIGHT,
                   XmNwidth     , Panel->WIDTH,
                   NULL );

    Panel->COMPUTE = 0;
  }

  Panel->MANAGED = 1;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XschExitPanel                       |
|                                                             |
\------------------------------------------------------------*/

void XschExitPanel( Panel )

   XschPanelItem  *Panel;
{
  autbegin();

  XtUnmanageChild( Panel->PANEL );

  Panel->MANAGED = 0;

  autend();
}
