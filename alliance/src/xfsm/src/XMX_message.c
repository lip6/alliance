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
| Tool    :                   XFSM                           |
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
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
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

  Widget        XfsmMessageMainForm;
  Widget        XfsmMessageText  [ XFSM_MAX_MESSAGE ];
  Widget        XfsmMessageLabel [ XFSM_MAX_MESSAGE ];
  Widget        XfsmMessageFrame [ XFSM_MAX_MESSAGE ];
  Widget        XfsmMessageForm  [ XFSM_MAX_MESSAGE ];

  static char *XfsmMessageLabelName [ XFSM_MAX_MESSAGE ] =

         {
           "X :",
           "Y :",
           "Dx :",
           "Dy :",
           NULL,
           NULL,
           NULL
         };

  static char *XfsmMessageTextName [ XFSM_MAX_MESSAGE ] =

         {
           "0",
           "0",
           "0",
           "0",
           "Measure",
           "Select point",
           "Enter base point"
         };

  static char *XfsmMessageFormName [ XFSM_MAX_MESSAGE ] =

         {
           "XfsmMessageFormX",
           "XfsmMessageFormY",
           "XfsmMessageFormDx",
           "XfsmMessageFormDy",
           "XfsmMessageFormMode",
           "XfsmMessageFormInfo",
           "XfsmMessageFormPrompt"
         };

  static char *XfsmMessageFrameName [ XFSM_MAX_MESSAGE ] =

         {
           "XfsmMessageFrameX",
           "XfsmMessageFrameY",
           "XfsmMessageFrameDx",
           "XfsmMessageFrameDy",
           "XfsmMessageFrameMode",
           "XfsmMessageFrameInfo",
           "XfsmMessageFramePrompt"
         };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XfsmDisplayMessage                    |
|                                                             |
\------------------------------------------------------------*/

void XfsmDisplayMessage( Where, Text )
 
     char  Where;
     char *Text;
{
  XmString TextString;

  autbegin();

  TextString = XmStringCreateSimple( Text ); 

  XtVaSetValues( XfsmMessageText[ Where ], XmNlabelString, TextString, NULL );

  XmStringFree( TextString );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XfsmPromptEditMeasure                  |
|                                                             |
\------------------------------------------------------------*/

void XfsmPromptEditMeasure()

{
  autbegin();

  XfsmDisplayMessage( XFSM_MESSAGE_MODE  , "Measure" );
  XfsmDisplayMessage( XFSM_MESSAGE_PROMPT, "Enter base point" );
  XfsmDisplayMessage( XFSM_MESSAGE_INFO  , "Select point" );

  autend();
}  

/*------------------------------------------------------------\
|                                                             |
|                    XfsmInitializeMessage                   |
|                                                             |
\------------------------------------------------------------*/

void XfsmInitializeMessage()

{
  int Counter;
  int Column;
  int Left;
  int Width;

  autbegin();

  XfsmMessageMainForm = 

    XtVaCreateManagedWidget( "XfsmMessageMainForm",
                              xmFormWidgetClass    ,
                              XfsmMainForm        ,
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

  for ( Counter = 0; Counter < XFSM_MAX_MESSAGE; Counter++ )
  {
    if ( Counter == 4 ) Width = 22;
    if ( Counter == 5 ) Width = 68;
    if ( Counter == 6 ) Width = 40;

    XfsmMessageFrame[ Counter ] =

      XtVaCreateManagedWidget( XfsmMessageFrameName[ Counter ],
                               xmFrameWidgetClass,
                               XfsmMessageMainForm,
                               XmNshadowType            , XmSHADOW_ETCHED_IN,
                               XmNleftAttachment        , XmATTACH_POSITION,
                               XmNleftPosition          , Column,
                               XmNrightAttachment       , XmATTACH_POSITION,
                               XmNrightPosition         , Column + Width,
                               XmNtopAttachment         , XmATTACH_FORM,
                               XmNbottomAttachment      , XmATTACH_FORM,
                               NULL
                             );

    XfsmMessageForm[ Counter ] =

      XtVaCreateManagedWidget( XfsmMessageFormName[ Counter ],
                               xmFormWidgetClass   ,
                               XfsmMessageFrame[ Counter ],
                               XmNleftAttachment           , XmATTACH_FORM,
                               XmNrightAttachment          , XmATTACH_FORM,
                               XmNtopAttachment            , XmATTACH_FORM,
                               XmNbottomAttachment         , XmATTACH_FORM,
                               XmNfractionBase             , 18,
                               NULL
                             );

    if ( XfsmMessageLabelName[ Counter ] != (char *)NULL )
    {
      XfsmMessageLabel [ Counter ] = 

        XtVaCreateManagedWidget( XfsmMessageLabelName[ Counter ], 
                                 xmLabelWidgetClass,
                                 XfsmMessageForm [ Counter ],
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

    XfsmMessageText [ Counter ]  = 

      XtVaCreateManagedWidget( XfsmMessageTextName[ Counter ], 
                               xmLabelWidgetClass,
                               XfsmMessageForm [ Counter ],
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
