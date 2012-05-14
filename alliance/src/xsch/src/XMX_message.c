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
| Tool    :                   XSCH                           |
|                                                             |
| File    :                Message.c                          |
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
# include <Xm/Xm.h>
# include <Xm/Form.h>
# include <Xm/Frame.h>
# include <Xm/Label.h>
# include <Xm/LabelG.h>
# include <Xm/TextF.h>
# include <Xm/Text.h>
 
# include "mut.h"
# include "aut.h"
# include "mlo.h"
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

  Widget        XschMessageMainForm;
  Widget        XschMessageText  [ XSCH_MAX_MESSAGE ];
  Widget        XschMessageLabel [ XSCH_MAX_MESSAGE ];
  Widget        XschMessageFrame [ XSCH_MAX_MESSAGE ];
  Widget        XschMessageForm  [ XSCH_MAX_MESSAGE ];

  static char *XschMessageLabelName [ XSCH_MAX_MESSAGE ] =

         {
           "X :",
           "Y :",
           "Dx :",
           "Dy :",
           NULL,
           NULL,
           NULL
         };

  static char *XschMessageTextName [ XSCH_MAX_MESSAGE ] =

         {
           "0",
           "0",
           "0",
           "0",
           "Measure",
           "Select point",
           "Enter base point"
         };

  static char *XschMessageFormName [ XSCH_MAX_MESSAGE ] =

         {
           "XschMessageFormX",
           "XschMessageFormY",
           "XschMessageFormDx",
           "XschMessageFormDy",
           "XschMessageFormMode",
           "XschMessageFormInfo",
           "XschMessageFormPrompt"
         };

  static char *XschMessageFrameName [ XSCH_MAX_MESSAGE ] =

         {
           "XschMessageFrameX",
           "XschMessageFrameY",
           "XschMessageFrameDx",
           "XschMessageFrameDy",
           "XschMessageFrameMode",
           "XschMessageFrameInfo",
           "XschMessageFramePrompt"
         };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XschDisplayMessage                    |
|                                                             |
\------------------------------------------------------------*/

void XschDisplayMessage( Where, Text )
 
     int   Where;
     char *Text;
{
  XmString TextString;

  autbegin();

  TextString = XmStringCreateSimple( Text ); 

  XtVaSetValues( XschMessageText[ Where ], XmNlabelString, TextString, NULL );

  XmStringFree( TextString );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XschPromptEditMeasure                  |
|                                                             |
\------------------------------------------------------------*/

void XschPromptEditMeasure()

{
  autbegin();

  XschDisplayMessage( XSCH_MESSAGE_MODE  , "Measure" );
  XschDisplayMessage( XSCH_MESSAGE_PROMPT, "Enter base point" );
  XschDisplayMessage( XSCH_MESSAGE_INFO  , "Select point" );

  autend();
}  

/*------------------------------------------------------------\
|                                                             |
|                    XschInitializeMessage                   |
|                                                             |
\------------------------------------------------------------*/

void XschInitializeMessage()

{
  int Counter;
  int Column;
  int Left;
  int Width;

  autbegin();

  XschMessageMainForm = 

    XtVaCreateManagedWidget( "XschMessageMainForm",
                              xmFormWidgetClass    ,
                              XschMainForm        ,
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

  for ( Counter = 0; Counter < XSCH_MAX_MESSAGE; Counter++ )
  {
    if ( Counter == 4 ) Width = 22;
    if ( Counter == 5 ) Width = 68;
    if ( Counter == 6 ) Width = 40;

    XschMessageFrame[ Counter ] =

      XtVaCreateManagedWidget( XschMessageFrameName[ Counter ],
                               xmFrameWidgetClass,
                               XschMessageMainForm,
                               XmNshadowType            , XmSHADOW_ETCHED_IN,
                               XmNleftAttachment        , XmATTACH_POSITION,
                               XmNleftPosition          , Column,
                               XmNrightAttachment       , XmATTACH_POSITION,
                               XmNrightPosition         , Column + Width,
                               XmNtopAttachment         , XmATTACH_FORM,
                               XmNbottomAttachment      , XmATTACH_FORM,
                               NULL
                             );

    XschMessageForm[ Counter ] =

      XtVaCreateManagedWidget( XschMessageFormName[ Counter ],
                               xmFormWidgetClass   ,
                               XschMessageFrame[ Counter ],
                               XmNleftAttachment           , XmATTACH_FORM,
                               XmNrightAttachment          , XmATTACH_FORM,
                               XmNtopAttachment            , XmATTACH_FORM,
                               XmNbottomAttachment         , XmATTACH_FORM,
                               XmNfractionBase             , 18,
                               NULL
                             );

    if ( XschMessageLabelName[ Counter ] != (char *)NULL )
    {
      XschMessageLabel [ Counter ] = 

        XtVaCreateManagedWidget( XschMessageLabelName[ Counter ], 
                                 xmLabelWidgetClass,
                                 XschMessageForm [ Counter ],
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

    XschMessageText [ Counter ]  = 

      XtVaCreateManagedWidget( XschMessageTextName[ Counter ], 
                               xmLabelWidgetClass,
                               XschMessageForm [ Counter ],
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
