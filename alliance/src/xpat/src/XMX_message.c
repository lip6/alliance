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
| Tool    :                   XPAT                           |
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
# include "pat.h"
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

  Widget        XpatMessageMainForm;
  Widget        XpatMessageText  [ XPAT_MAX_MESSAGE ];
  Widget        XpatMessageLabel [ XPAT_MAX_MESSAGE ];
  Widget        XpatMessageFrame [ XPAT_MAX_MESSAGE ];
  Widget        XpatMessageForm  [ XPAT_MAX_MESSAGE ];

  static char *XpatMessageLabelName [ XPAT_MAX_MESSAGE ] =

         {
           "Time :",
           "Name :",
           "Delta :",
           NULL,
           NULL,
           NULL
         };

  static char *XpatMessageTextName [ XPAT_MAX_MESSAGE ] =

         {
           "0",
           "0",
           "0",
           "Measure",
           "Select point",
           "Enter base point"
         };

  static char *XpatMessageFormName [ XPAT_MAX_MESSAGE ] =

         {
           "XpatMessageFormX",
           "XpatMessageFormY",
           "XpatMessageFormDx",
           "XpatMessageFormMode",
           "XpatMessageFormInfo",
           "XpatMessageFormPrompt"
         };

  static char *XpatMessageFrameName [ XPAT_MAX_MESSAGE ] =

         {
           "XpatMessageFrameX",
           "XpatMessageFrameY",
           "XpatMessageFrameDx",
           "XpatMessageFrameMode",
           "XpatMessageFrameInfo",
           "XpatMessageFramePrompt"
         };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XpatDisplayMessage                    |
|                                                             |
\------------------------------------------------------------*/

void XpatDisplayMessage( Where, Text )
 
     char  Where;
     char *Text;
{
  XmString TextString;

  autbegin();

  TextString = XmStringCreateSimple( Text ); 

  XtVaSetValues( XpatMessageText[ Where ], XmNlabelString, TextString, NULL );

  XmStringFree( TextString );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XpatPromptEditMeasure                  |
|                                                             |
\------------------------------------------------------------*/

void XpatPromptEditMeasure()

{
  autbegin();

  XpatDisplayMessage( XPAT_MESSAGE_MODE  , "Measure" );
  XpatDisplayMessage( XPAT_MESSAGE_PROMPT, "Enter base point" );
  XpatDisplayMessage( XPAT_MESSAGE_INFO  , "Select point" );

  autend();
}  

/*------------------------------------------------------------\
|                                                             |
|                    XpatInitializeMessage                   |
|                                                             |
\------------------------------------------------------------*/

void XpatInitializeMessage()

{
  int Counter;
  int Column;
  int Left;
  int Width;

  autbegin();

  XpatMessageMainForm = 

    XtVaCreateManagedWidget( "XpatMessageMainForm",
                              xmFormWidgetClass    ,
                              XpatMainForm        ,
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
  Width  = 25;

  for ( Counter = 0; Counter < XPAT_MAX_MESSAGE; Counter++ )
  {
    if ( Counter == 1 ) Width = 60;
    if ( Counter == 2 ) Width = 25;
    if ( Counter == 3 ) Width = 22;
    if ( Counter == 4 ) Width = 32;
    if ( Counter == 5 ) Width = 32;

    XpatMessageFrame[ Counter ] =

      XtVaCreateManagedWidget( XpatMessageFrameName[ Counter ],
                               xmFrameWidgetClass,
                               XpatMessageMainForm,
                               XmNshadowType            , XmSHADOW_ETCHED_IN,
                               XmNleftAttachment        , XmATTACH_POSITION,
                               XmNleftPosition          , Column,
                               XmNrightAttachment       , XmATTACH_POSITION,
                               XmNrightPosition         , Column + Width,
                               XmNtopAttachment         , XmATTACH_FORM,
                               XmNbottomAttachment      , XmATTACH_FORM,
                               NULL
                             );

    XpatMessageForm[ Counter ] =

      XtVaCreateManagedWidget( XpatMessageFormName[ Counter ],
                               xmFormWidgetClass   ,
                               XpatMessageFrame[ Counter ],
                               XmNleftAttachment           , XmATTACH_FORM,
                               XmNrightAttachment          , XmATTACH_FORM,
                               XmNtopAttachment            , XmATTACH_FORM,
                               XmNbottomAttachment         , XmATTACH_FORM,
                               XmNfractionBase             , 18,
                               NULL
                             );

    if ( XpatMessageLabelName[ Counter ] != (char *)NULL )
    {
      XpatMessageLabel [ Counter ] = 

        XtVaCreateManagedWidget( XpatMessageLabelName[ Counter ], 
                                 xmLabelWidgetClass,
                                 XpatMessageForm [ Counter ],
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

    XpatMessageText [ Counter ]  = 

      XtVaCreateManagedWidget( XpatMessageTextName[ Counter ], 
                               xmLabelWidgetClass,
                               XpatMessageForm [ Counter ],
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
