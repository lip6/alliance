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
| Tool    :                   DREAL                           |
|                                                             |
| File    :                Message.c                          |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                  28.03.95                         |
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
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "GSB.h"
# include "GTB.h"
# include "GMX.h"
# include "GMX_message.h"

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

  Widget        DrealMessageMainForm;
  Widget        DrealMessageText  [ DREAL_MAX_MESSAGE ];
  Widget        DrealMessageLabel [ DREAL_MAX_MESSAGE ];
  Widget        DrealMessageFrame [ DREAL_MAX_MESSAGE ];
  Widget        DrealMessageForm  [ DREAL_MAX_MESSAGE ];

  static char *DrealMessageLabelName [ DREAL_MAX_MESSAGE ] =

         {
           "X :",
           "Y :",
           "Dx :",
           "Dy :",
           NULL,
           NULL,
           NULL
         };

  static char *DrealMessageTextName [ DREAL_MAX_MESSAGE ] =

         {
           "0",
           "0",
           "0",
           "0",
           "Measure",
           "Select point",
           "Enter base point"
         };

  static char *DrealMessageFormName [ DREAL_MAX_MESSAGE ] =

         {
           "DrealMessageFormX",
           "DrealMessageFormY",
           "DrealMessageFormDx",
           "DrealMessageFormDy",
           "DrealMessageFormMode",
           "DrealMessageFormInfo",
           "DrealMessageFormPrompt"
         };

  static char *DrealMessageFrameName [ DREAL_MAX_MESSAGE ] =

         {
           "DrealMessageFrameX",
           "DrealMessageFrameY",
           "DrealMessageFrameDx",
           "DrealMessageFrameDy",
           "DrealMessageFrameMode",
           "DrealMessageFrameInfo",
           "DrealMessageFramePrompt"
         };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      DrealDisplayMessage                    |
|                                                             |
\------------------------------------------------------------*/

void DrealDisplayMessage( Where, Text )
 
     char  Where;
     char *Text;
{
  XmString TextString;

  rdsbegin();

  TextString = XmStringCreateSimple( Text ); 

  XtVaSetValues( DrealMessageText[ Where ], XmNlabelString, TextString, NULL );

  XmStringFree( TextString );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     DrealPromptEditMeasure                  |
|                                                             |
\------------------------------------------------------------*/

void DrealPromptEditMeasure()

{
  rdsbegin();

  DrealDisplayMessage( DREAL_MESSAGE_MODE  , "Measure" );
  DrealDisplayMessage( DREAL_MESSAGE_PROMPT, "Enter base point" );
  DrealDisplayMessage( DREAL_MESSAGE_INFO  , "Select point" );

  rdsend();
}  

/*------------------------------------------------------------\
|                                                             |
|                    DrealInitializeMessage                   |
|                                                             |
\------------------------------------------------------------*/

void DrealInitializeMessage()

{
  int Counter;
  int Column;
  int Left;
  int Width;

  rdsbegin();

  DrealMessageMainForm = 

    XtVaCreateManagedWidget( "DrealMessageMainForm",
                              xmFormWidgetClass    ,
                              DrealMainForm        ,
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

  for ( Counter = 0; Counter < DREAL_MAX_MESSAGE; Counter++ )
  {
    if ( Counter == 4 ) Width = 22;
    if ( Counter == 5 ) Width = 68;
    if ( Counter == 6 ) Width = 40;

    DrealMessageFrame[ Counter ] =

      XtVaCreateManagedWidget( DrealMessageFrameName[ Counter ],
                               xmFrameWidgetClass,
                               DrealMessageMainForm,
                               XmNshadowType            , XmSHADOW_ETCHED_IN,
                               XmNleftAttachment        , XmATTACH_POSITION,
                               XmNleftPosition          , Column,
                               XmNrightAttachment       , XmATTACH_POSITION,
                               XmNrightPosition         , Column + Width,
                               XmNtopAttachment         , XmATTACH_FORM,
                               XmNbottomAttachment      , XmATTACH_FORM,
                               NULL
                             );

    DrealMessageForm[ Counter ] =

      XtVaCreateManagedWidget( DrealMessageFormName[ Counter ],
                               xmFormWidgetClass   ,
                               DrealMessageFrame[ Counter ],
                               XmNleftAttachment           , XmATTACH_FORM,
                               XmNrightAttachment          , XmATTACH_FORM,
                               XmNtopAttachment            , XmATTACH_FORM,
                               XmNbottomAttachment         , XmATTACH_FORM,
                               XmNfractionBase             , 18,
                               NULL
                             );

    if ( DrealMessageLabelName[ Counter ] != (char *)NULL )
    {
      DrealMessageLabel [ Counter ] = 

        XtVaCreateManagedWidget( DrealMessageLabelName[ Counter ], 
                                 xmLabelWidgetClass,
                                 DrealMessageForm [ Counter ],
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

    DrealMessageText [ Counter ]  = 

      XtVaCreateManagedWidget( DrealMessageTextName[ Counter ], 
                               xmLabelWidgetClass,
                               DrealMessageForm [ Counter ],
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

  rdsend();
}
