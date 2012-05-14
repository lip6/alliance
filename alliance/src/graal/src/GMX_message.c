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
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                Message.c                          |
|                                                             |
| Author  :                Jacomme Ludovic                    |
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

  Widget        GraalMessageMainForm;
  Widget        GraalMessageText  [ GRAAL_MAX_MESSAGE ];
  Widget        GraalMessageLabel [ GRAAL_MAX_MESSAGE ];
  Widget        GraalMessageFrame [ GRAAL_MAX_MESSAGE ];
  Widget        GraalMessageForm  [ GRAAL_MAX_MESSAGE ];

  static char *GraalMessageLabelName [ GRAAL_MAX_MESSAGE ] =

         {
           "X :",
           "Y :",
           "Dx :",
           "Dy :",
           NULL,
           NULL,
           NULL
         };

  static char *GraalMessageTextName [ GRAAL_MAX_MESSAGE ] =

         {
           "0",
           "0",
           "0",
           "0",
           "Measure",
           "Select point",
           "Enter base point"
         };

  static char *GraalMessageFormName [ GRAAL_MAX_MESSAGE ] =

         {
           "GraalMessageFormX",
           "GraalMessageFormY",
           "GraalMessageFormDx",
           "GraalMessageFormDy",
           "GraalMessageFormMode",
           "GraalMessageFormInfo",
           "GraalMessageFormPrompt"
         };

  static char *GraalMessageFrameName [ GRAAL_MAX_MESSAGE ] =

         {
           "GraalMessageFrameX",
           "GraalMessageFrameY",
           "GraalMessageFrameDx",
           "GraalMessageFrameDy",
           "GraalMessageFrameMode",
           "GraalMessageFrameInfo",
           "GraalMessageFramePrompt"
         };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      GraalDisplayMessage                    |
|                                                             |
\------------------------------------------------------------*/

void GraalDisplayMessage( Where, Text )
 
     char  Where;
     char *Text;
{
  XmString TextString;

  rdsbegin();

  TextString = XmStringCreateSimple( Text ); 

  XtVaSetValues( GraalMessageText[ Where ], XmNlabelString, TextString, NULL );

  XmStringFree( TextString );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalPromptEditMeasure                  |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptEditMeasure()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE  , "Measure" );
  GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter base point" );
  GraalDisplayMessage( GRAAL_MESSAGE_INFO  , "Select point" );

  rdsend();
}  

/*------------------------------------------------------------\
|                                                             |
|                    GraalInitializeMessage                   |
|                                                             |
\------------------------------------------------------------*/

void GraalInitializeMessage()

{
  int Counter;
  int Column;
  int Left;
  int Width;

  rdsbegin();

  GraalMessageMainForm = 

    XtVaCreateManagedWidget( "GraalMessageMainForm",
                              xmFormWidgetClass    ,
                              GraalMainForm        ,
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

  for ( Counter = 0; Counter < GRAAL_MAX_MESSAGE; Counter++ )
  {
    if ( Counter == 4 ) Width = 22;
    if ( Counter == 5 ) Width = 68;
    if ( Counter == 6 ) Width = 40;

    GraalMessageFrame[ Counter ] =

      XtVaCreateManagedWidget( GraalMessageFrameName[ Counter ],
                               xmFrameWidgetClass,
                               GraalMessageMainForm,
                               XmNshadowType            , XmSHADOW_ETCHED_IN,
                               XmNleftAttachment        , XmATTACH_POSITION,
                               XmNleftPosition          , Column,
                               XmNrightAttachment       , XmATTACH_POSITION,
                               XmNrightPosition         , Column + Width,
                               XmNtopAttachment         , XmATTACH_FORM,
                               XmNbottomAttachment      , XmATTACH_FORM,
                               NULL
                             );

    GraalMessageForm[ Counter ] =

      XtVaCreateManagedWidget( GraalMessageFormName[ Counter ],
                               xmFormWidgetClass   ,
                               GraalMessageFrame[ Counter ],
                               XmNleftAttachment           , XmATTACH_FORM,
                               XmNrightAttachment          , XmATTACH_FORM,
                               XmNtopAttachment            , XmATTACH_FORM,
                               XmNbottomAttachment         , XmATTACH_FORM,
                               XmNfractionBase             , 18,
                               NULL
                             );

    if ( GraalMessageLabelName[ Counter ] != (char *)NULL )
    {
      GraalMessageLabel [ Counter ] = 

        XtVaCreateManagedWidget( GraalMessageLabelName[ Counter ], 
                                 xmLabelWidgetClass,
                                 GraalMessageForm [ Counter ],
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

    GraalMessageText [ Counter ]  = 

      XtVaCreateManagedWidget( GraalMessageTextName[ Counter ], 
                               xmLabelWidgetClass,
                               GraalMessageForm [ Counter ],
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
