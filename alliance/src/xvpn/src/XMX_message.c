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
| Tool    :                   XVPN                            |
|                                                             |
| File    :                Message.c                          |
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
# include <Xm/Xm.h>
# include <Xm/Form.h>
# include <Xm/Frame.h>
# include <Xm/Label.h>
# include <Xm/LabelG.h>
# include <Xm/TextF.h>
# include <Xm/Text.h>
 
# include "mut.h"
# include "aut.h"
# include "XSB.h"
# include "XTB.h"
# include "XMX.h"
# include "XMX_message.h"

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

  Widget        XvpnMessageMainForm;
  Widget        XvpnMessageText  [ XVPN_MAX_MESSAGE ];
  Widget        XvpnMessageLabel [ XVPN_MAX_MESSAGE ];
  Widget        XvpnMessageFrame [ XVPN_MAX_MESSAGE ];
  Widget        XvpnMessageForm  [ XVPN_MAX_MESSAGE ];

  static char *XvpnMessageLabelName [ XVPN_MAX_MESSAGE ] =

         {
           "X :",
           "Y :",
           "Dx :",
           "Dy :",
           NULL,
           NULL,
           NULL
         };

  static char *XvpnMessageTextName [ XVPN_MAX_MESSAGE ] =

         {
           "0",
           "0",
           "0",
           "0",
           "Identify",
           "Select object",
           "Enter select point"
         };

  static char *XvpnMessageFormName [ XVPN_MAX_MESSAGE ] =

         {
           "XvpnMessageFormX",
           "XvpnMessageFormY",
           "XvpnMessageFormDx",
           "XvpnMessageFormDy",
           "XvpnMessageFormMode",
           "XvpnMessageFormInfo",
           "XvpnMessageFormPrompt"
         };

  static char *XvpnMessageFrameName [ XVPN_MAX_MESSAGE ] =

         {
           "XvpnMessageFrameX",
           "XvpnMessageFrameY",
           "XvpnMessageFrameDx",
           "XvpnMessageFrameDy",
           "XvpnMessageFrameMode",
           "XvpnMessageFrameInfo",
           "XvpnMessageFramePrompt"
         };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XvpnDisplayMessage                    |
|                                                             |
\------------------------------------------------------------*/

void XvpnDisplayMessage( Where, Text )
 
     short Where;
     char *Text;
{
  XmString TextString;

  autbegin();

  TextString = XmStringCreateSimple( Text ); 

  XtVaSetValues( XvpnMessageText[ Where ], XmNlabelString, TextString, NULL );

  XmStringFree( TextString );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XvpnPromptEditMeasure                  |
|                                                             |
\------------------------------------------------------------*/

void XvpnPromptEditMeasure()

{
  autbegin();

  XvpnDisplayMessage( XVPN_MESSAGE_MODE  , "Measure" );
  XvpnDisplayMessage( XVPN_MESSAGE_PROMPT, "Enter base point" );
  XvpnDisplayMessage( XVPN_MESSAGE_INFO  , "Select point" );

  autend();
}  

/*------------------------------------------------------------\
|                                                             |
|                    XvpnInitializeMessage                   |
|                                                             |
\------------------------------------------------------------*/

void XvpnInitializeMessage()

{
  int Counter;
  int Column;
  int Left;
  int Width;

  autbegin();

  XvpnMessageMainForm = 

    XtVaCreateManagedWidget( "XvpnMessageMainForm",
                              xmFormWidgetClass    ,
                              XvpnMainForm        ,
                              XmNtopAttachment     , XmATTACH_POSITION,
                              XmNtopPosition       , 193,
                              XmNbottomAttachment  , XmATTACH_POSITION,
                              XmNbottomPosition    , 199,
                              XmNleftAttachment    , XmATTACH_POSITION,
                              XmNleftPosition      , 0,
                              XmNrightAttachment   , XmATTACH_POSITION,
                              XmNrightPosition     , 200,
                              XmNfractionBase      , 200,
                              NULL );

  Column = 1;
  Width  = 17;

  for ( Counter = 0; Counter < XVPN_MAX_MESSAGE; Counter++ )
  {
    if ( Counter == 4 ) Width = 22;
    if ( Counter == 5 ) Width = 68;
    if ( Counter == 6 ) Width = 40;

    XvpnMessageFrame[ Counter ] =

      XtVaCreateManagedWidget( XvpnMessageFrameName[ Counter ],
                               xmFrameWidgetClass,
                               XvpnMessageMainForm,
                               XmNshadowType            , XmSHADOW_ETCHED_IN,
                               XmNleftAttachment        , XmATTACH_POSITION,
                               XmNleftPosition          , Column,
                               XmNrightAttachment       , XmATTACH_POSITION,
                               XmNrightPosition         , Column + Width,
                               XmNtopAttachment         , XmATTACH_FORM,
                               XmNbottomAttachment      , XmATTACH_FORM,
                               NULL
                             );

    XvpnMessageForm[ Counter ] =

      XtVaCreateManagedWidget( XvpnMessageFormName[ Counter ],
                               xmFormWidgetClass   ,
                               XvpnMessageFrame[ Counter ],
                               XmNleftAttachment           , XmATTACH_FORM,
                               XmNrightAttachment          , XmATTACH_FORM,
                               XmNtopAttachment            , XmATTACH_FORM,
                               XmNbottomAttachment         , XmATTACH_FORM,
                               XmNfractionBase             , 18,
                               NULL
                             );

    if ( XvpnMessageLabelName[ Counter ] != (char *)NULL )
    {
      XvpnMessageLabel [ Counter ] = 

        XtVaCreateManagedWidget( XvpnMessageLabelName[ Counter ], 
                                 xmLabelWidgetClass,
                                 XvpnMessageForm [ Counter ],
                                 XmNleftAttachment           , XmATTACH_FORM,
                                 XmNrightAttachment          , XmATTACH_POSITION,
                                 XmNrightPosition            , 6,
                                 XmNbottomAttachment         , XmATTACH_FORM,
                                 XmNtopAttachment            , XmATTACH_FORM,
                                 XmNrecomputeSize            , False,
                                 NULL
                               );
      Left = 6;
    }
    else
    {
      Left = 0;
    }

    XvpnMessageText [ Counter ]  = 

      XtVaCreateManagedWidget( XvpnMessageTextName[ Counter ], 
                               xmLabelWidgetClass,
                               XvpnMessageForm [ Counter ],
                               XmNtopAttachment            , XmATTACH_FORM,
                               XmNbottomAttachment         , XmATTACH_FORM,
                               XmNrightAttachment          , XmATTACH_FORM,
                               XmNleftAttachment           , XmATTACH_POSITION,
                               XmNleftPosition             , Left,
                               XmNrecomputeSize            , False,
                               NULL
                             );

    Column = Column + Width;
  }

  autend();
}
