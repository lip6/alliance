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
| Tool    :                    XFSM                           |
|                                                             |
| File    :                  Panel.c                          |
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
# include <X11/Intrinsic.h>
# include <Xm/Xm.h>
# include <Xm/Frame.h>
# include <Xm/Form.h>
# include <Xm/List.h>
# include <Xm/Text.h>
# include <Xm/TextF.h>
# include <Xm/PushB.h>
# include <Xm/DialogS.h>
# include <Xm/Label.h>
# include <Xm/LabelG.h>
 
# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "XSB.h"
# include "XTB.h"
# include "XMX.h"
# include "XMH.h"

# include "XMH_panel.h" 
# include "LIP6bw.h" 

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

   XfsmPanelItem XfsmHelpPresentPanel =

         {
           "Xfsm present",
           1,
           0,
           XFSM_HELP_PRESENT_X,
           XFSM_HELP_PRESENT_Y,
           700,
           360,
           0,
           0,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (XfsmPanelButtonItem *)NULL
         };

   int XfsmHelpPresentDefaultValues[ 5 ] =

         {
           XFSM_HELP_PRESENT_X,
           XFSM_HELP_PRESENT_Y,
           700, 360, 0
         };

 
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    XfsmBuildPresentPanel                   |
|                                                             |
\------------------------------------------------------------*/

void XfsmBuildPresentPanel()

{
  Widget       PanelLabel;
  Widget       PanelButton;
  Pixmap       PanelPixmap;
  XmString     PanelString;
  XmString     PanelString1;
  XmString     PanelString2;
  XmFontList   PanelFontList;
  XFontStruct *PanelFont;
  Display     *PanelDisplay;
  char         Buffer[ 64 ];
  Arg          Args[3];

  autbegin();
  
  PanelDisplay  = XtDisplay( XfsmMainWindow );
  PanelFont     = XLoadQueryFont( PanelDisplay, "-*-helvetica-bold-o-*--24-*" );
  PanelFontList = XmFontListCreate( PanelFont, "Panel_charset1" );
  PanelFont     = XLoadQueryFont( PanelDisplay, "-*-helvetica-bold-r-*--18-*" );
  PanelFontList = XmFontListAdd( PanelFontList, PanelFont, "Panel_charset2" );
  PanelFont     = XLoadQueryFont( PanelDisplay, "-*-helvetica-bold-r-*--12-*" );
  PanelFontList = XmFontListAdd( PanelFontList, PanelFont, "Panel_charset4" );

  XtSetArg( Args[0], XmNshadowType    , XmSHADOW_ETCHED_IN );
  XtSetArg( Args[1], XmNdeleteResponse, XmDO_NOTHING       );
  XtSetArg( Args[2], XmNtitle         , "Xfsm present"    );

  XfsmHelpPresentPanel.PANEL = 

    XmCreateFormDialog( XfsmMainWindow, XfsmHelpPresentPanel.TITLE, Args, 3);

  XtAddCallback( XfsmHelpPresentPanel.PANEL, XmNdestroyCallback,
                 XfsmDestroyDialogCallback, NULL );

  XfsmHelpPresentPanel.PANEL_FORM = 

     XtVaCreateManagedWidget( "",
                              xmFormWidgetClass,
                              XfsmHelpPresentPanel.PANEL,
                              XmNtopAttachment    , XmATTACH_FORM,
                              XmNbottomAttachment , XmATTACH_FORM,
                              XmNleftAttachment   , XmATTACH_FORM,
                              XmNrightAttachment  , XmATTACH_FORM,
                              NULL
                            );

  XfsmHelpPresentPanel.FRAME = 

     XtVaCreateManagedWidget( "",
                              xmFrameWidgetClass,
                              XfsmHelpPresentPanel.PANEL_FORM,
                              XmNtopAttachment   , XmATTACH_POSITION,
                              XmNtopPosition     , 05,
                              XmNbottomAttachment, XmATTACH_POSITION,
                              XmNbottomPosition  , 95,
                              XmNleftAttachment  , XmATTACH_POSITION,
                              XmNleftPosition    , 05,
                              XmNrightAttachment , XmATTACH_POSITION,
                              XmNrightPosition   , 95,
                              NULL
                            );

  XfsmHelpPresentPanel.FORM = 

    XtVaCreateManagedWidget( "",
                             xmFormWidgetClass,
                             XfsmHelpPresentPanel.FRAME,
                             XmNtopAttachment   , XmATTACH_POSITION,
                             XmNtopPosition     , 05,
                             XmNbottomAttachment, XmATTACH_POSITION,
                             XmNbottomPosition  , 95,
                             XmNleftAttachment  , XmATTACH_POSITION,
                             XmNleftPosition    , 05,
                             XmNrightAttachment , XmATTACH_POSITION,
                             XmNrightPosition   , 95,
                             NULL
                           );

  PanelPixmap = XfsmCreatePixmap( XfsmMainWindow,
                                    LIP6bw_bits,
                                    LIP6bw_width,
                                    LIP6bw_height);

  PanelLabel = XtVaCreateManagedWidget( "",
                                         xmLabelGadgetClass, 
                                         XfsmHelpPresentPanel.FORM,
			                   XmNlabelType   , XmPIXMAP,
			                   XmNlabelPixmap , PanelPixmap,
			                   NULL
                                       );

  sprintf( Buffer, "ALLIANCE CAD SYSTEM  %s\n", ALLIANCE_VERSION );

  PanelString = XmStringCreateLtoR( Buffer, "Panel_charset2" );

  PanelLabel = XtVaCreateManagedWidget( "",
                                        xmLabelWidgetClass,
                                        XfsmHelpPresentPanel.FORM,
                                        XmNfontList        , PanelFontList,
                                        XmNlabelString     , PanelString,
                                        XmNtopAttachment   , XmATTACH_POSITION,
                                        XmNtopPosition     , 15,
                                        XmNrightAttachment , XmATTACH_POSITION,
                                        XmNrightPosition   , 75,
                                        XmNleftAttachment  , XmATTACH_POSITION,
                                        XmNleftPosition    , 25,
                                        NULL
                                      );
  XmStringFree( PanelString );

  PanelString1 = XmStringCreateLtoR( "Xfsm"         , "Panel_charset1" );
  PanelString2 = XmStringCreateLtoR( "\n( Touch Me )", "Panel_charset4" );
  PanelString  = XmStringConcat( PanelString1, PanelString2 );

  PanelButton = XtVaCreateManagedWidget( "",
                                          xmPushButtonWidgetClass, 
                                          XfsmHelpPresentPanel.FORM,
                                          XmNfontList        , PanelFontList,
                                          XmNlabelString     , PanelString,
                                          XmNshadowThickness , 3,
                                          XmNtopAttachment   , XmATTACH_WIDGET,
                                          XmNtopWidget       , PanelLabel,
                                          XmNtopOffset       , 5,
                                          XmNrightAttachment , XmATTACH_POSITION,
                                          XmNrightPosition   , 60,
                                          XmNleftAttachment  , XmATTACH_POSITION,
                                          XmNleftPosition    , 40,
                                          NULL
                                        );
  XmStringFree( PanelString  );
  XmStringFree( PanelString1 );
  XmStringFree( PanelString2 );

  XtAddCallback( PanelButton, 
                 XmNactivateCallback,
                 XfsmExitDialogCallback, NULL );

  sprintf( Buffer, "\nGraphic FSM Viewer\nVersion %s", XFSM_VERSION );

  PanelString = XmStringCreateLtoR( Buffer, "Panel_charset2" );

  PanelLabel = XtVaCreateManagedWidget( "",
                                         xmLabelWidgetClass,
                                         XfsmHelpPresentPanel.FORM,
                                         XmNfontList        , PanelFontList,
                                         XmNlabelString     , PanelString,
                                         XmNtopAttachment   , XmATTACH_WIDGET,
                                         XmNtopWidget       , PanelButton,
                                         XmNtopOffset       , 5,
                                         XmNrightAttachment , XmATTACH_POSITION,
                                         XmNrightPosition   , 75,
                                         XmNleftAttachment  , XmATTACH_POSITION,
                                         XmNleftPosition    , 25,
                                         NULL
                                       );
  XmStringFree( PanelString );

  PanelString = XmStringCreateLtoR( "copyright \251 1995-2000 ASIM, \
CAO-VLSI Team\nWritten by Ludovic Jacomme\nE-mail support:  alliance-support@asim.lip6.fr", "Panel_charset4" );

  PanelLabel = XtVaCreateManagedWidget( "",
                                         xmLabelWidgetClass,
                                         XfsmHelpPresentPanel.FORM,
                                         XmNfontList        , PanelFontList,
                                         XmNlabelString     , PanelString,
                                         XmNtopAttachment   , XmATTACH_WIDGET,
                                         XmNtopWidget       , PanelLabel,
                                         XmNtopOffset       , 5,
                                         XmNrightAttachment , XmATTACH_FORM,
                                         XmNleftAttachment  , XmATTACH_FORM,
                                         NULL
                                       );
  XmStringFree( PanelString );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XfsmEnterPresentPanel                   |
|                                                             |
\------------------------------------------------------------*/

void XfsmEnterPresentPanel()

{
  autbegin();

  XfsmEnterPanel( &XfsmHelpPresentPanel );

  XfsmLimitedLoop( XfsmHelpPresentPanel.PANEL );

  XfsmExitPanel( &XfsmHelpPresentPanel );

  autend();
}
