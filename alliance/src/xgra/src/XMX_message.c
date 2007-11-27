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
| Tool    :                   XGRA                           |
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

  Widget        XgraMessageMainForm;
  Widget        XgraMessageText  [ XGRA_MAX_MESSAGE ];
  Widget        XgraMessageLabel [ XGRA_MAX_MESSAGE ];
  Widget        XgraMessageFrame [ XGRA_MAX_MESSAGE ];
  Widget        XgraMessageForm  [ XGRA_MAX_MESSAGE ];

  static char *XgraMessageLabelName [ XGRA_MAX_MESSAGE ] =

         {
           "X :",
           "Y :",
           "Dx :",
           "Dy :",
           NULL,
           NULL,
           NULL
         };

  static char *XgraMessageTextName [ XGRA_MAX_MESSAGE ] =

         {
           "0",
           "0",
           "0",
           "0",
           "Measure",
           "Select point",
           "Enter base point"
         };

  static char *XgraMessageFormName [ XGRA_MAX_MESSAGE ] =

         {
           "XgraMessageFormX",
           "XgraMessageFormY",
           "XgraMessageFormDx",
           "XgraMessageFormDy",
           "XgraMessageFormMode",
           "XgraMessageFormInfo",
           "XgraMessageFormPrompt"
         };

  static char *XgraMessageFrameName [ XGRA_MAX_MESSAGE ] =

         {
           "XgraMessageFrameX",
           "XgraMessageFrameY",
           "XgraMessageFrameDx",
           "XgraMessageFrameDy",
           "XgraMessageFrameMode",
           "XgraMessageFrameInfo",
           "XgraMessageFramePrompt"
         };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XgraDisplayMessage                    |
|                                                             |
\------------------------------------------------------------*/

void XgraDisplayMessage( Where, Text )
 
     char  Where;
     char *Text;
{
  XmString TextString;

  autbegin();

  TextString = XmStringCreateSimple( Text ); 

  XtVaSetValues( XgraMessageText[ Where ], XmNlabelString, TextString, NULL );

  XmStringFree( TextString );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XgraPromptEditMeasure                  |
|                                                             |
\------------------------------------------------------------*/

void XgraPromptEditMeasure()

{
  autbegin();

  XgraDisplayMessage( XGRA_MESSAGE_MODE  , "Measure" );
  XgraDisplayMessage( XGRA_MESSAGE_PROMPT, "Enter base point" );
  XgraDisplayMessage( XGRA_MESSAGE_INFO  , "Select point" );

  autend();
}  

/*------------------------------------------------------------\
|                                                             |
|                    XgraInitializeMessage                   |
|                                                             |
\------------------------------------------------------------*/

void XgraInitializeMessage()

{
  int Counter;
  int Column;
  int Left;
  int Width;

  autbegin();

  XgraMessageMainForm = 

    XtVaCreateManagedWidget( "XgraMessageMainForm",
                              xmFormWidgetClass    ,
                              XgraMainForm        ,
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

  for ( Counter = 0; Counter < XGRA_MAX_MESSAGE; Counter++ )
  {
    if ( Counter == 4 ) Width = 22;
    if ( Counter == 5 ) Width = 68;
    if ( Counter == 6 ) Width = 40;

    XgraMessageFrame[ Counter ] =

      XtVaCreateManagedWidget( XgraMessageFrameName[ Counter ],
                               xmFrameWidgetClass,
                               XgraMessageMainForm,
                               XmNshadowType            , XmSHADOW_ETCHED_IN,
                               XmNleftAttachment        , XmATTACH_POSITION,
                               XmNleftPosition          , Column,
                               XmNrightAttachment       , XmATTACH_POSITION,
                               XmNrightPosition         , Column + Width,
                               XmNtopAttachment         , XmATTACH_FORM,
                               XmNbottomAttachment      , XmATTACH_FORM,
                               NULL
                             );

    XgraMessageForm[ Counter ] =

      XtVaCreateManagedWidget( XgraMessageFormName[ Counter ],
                               xmFormWidgetClass   ,
                               XgraMessageFrame[ Counter ],
                               XmNleftAttachment           , XmATTACH_FORM,
                               XmNrightAttachment          , XmATTACH_FORM,
                               XmNtopAttachment            , XmATTACH_FORM,
                               XmNbottomAttachment         , XmATTACH_FORM,
                               XmNfractionBase             , 18,
                               NULL
                             );

    if ( XgraMessageLabelName[ Counter ] != (char *)NULL )
    {
      XgraMessageLabel [ Counter ] = 

        XtVaCreateManagedWidget( XgraMessageLabelName[ Counter ], 
                                 xmLabelWidgetClass,
                                 XgraMessageForm [ Counter ],
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

    XgraMessageText [ Counter ]  = 

      XtVaCreateManagedWidget( XgraMessageTextName[ Counter ], 
                               xmLabelWidgetClass,
                               XgraMessageForm [ Counter ],
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
