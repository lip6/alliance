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
| File    :                  Panel.c                          |
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
# include <Xm/PushBG.h>
# include <Xm/CascadeBG.h>
 
# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "GTB.h"
# include "GSB.h"
# include "GMX.h"
# include "GRD.h"
# include "GMC.h"

# include "GMC_panel.h"
# include "GMC_dialog.h"
# include "GMC_create.h"
# include "GMC_message.h"

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
/*------------------------------------------------------------\
|                                                             |
|                         Create Panel                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Rectangle Panel                       |
|                                                             |
\------------------------------------------------------------*/

   static DrealPanelButtonItem DrealCreateRectangleButton[RDS_ALL_LAYER+2] =

         {
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             "Name",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             2, 1,
             CallbackChangeRectangleName,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Close",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             2, 1,
             CallbackChangeRectangleClose,
             (XtPointer)NULL,
             (Widget)NULL
           }
         };

   DrealPanelItem DrealCreateRectanglePanel  =

         {
           "Rectangle",
           1,
           0,
           DREAL_CREATE_RECTANGLE_X,
           DREAL_CREATE_RECTANGLE_Y, 
           100,
           300,
           2,  
           ( RDS_ALL_LAYER + 2 ) / 2,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           RDS_ALL_LAYER + 2,
           DrealCreateRectangleButton
         };

   int DrealCreateRectangleDefaultValues[ 5 ] =

         {
           DREAL_CREATE_RECTANGLE_X,
           DREAL_CREATE_RECTANGLE_Y, 
           100, 300, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    DrealBuildPanelCreate                    |
|                                                             |
\------------------------------------------------------------*/

void DrealBuildPanelCreate()

{
  DrealPanelButtonItem *Button;
  long                  Layer;
  long                  Index;
  unsigned char         X1;
  unsigned char         Y1;

  X1    = 0;
  Y1    = 0;
  Index = 0;

  for ( Layer = 0; Layer < RDS_LAYER_ABOX; Layer++ )
  {
    if ( ( DREAL_RDS_LAYER_NAME_TABLE[ Layer ][ 0 ] != NULL ) &&
         ( RDS_DYNAMIC_LAYER[ Layer ] != RDS_LAYER_UNUSED   ) )
    {
      Button = &DrealCreateRectangleButton[ Index ];

      Button->LABEL      = DREAL_RDS_LAYER_NAME_TABLE[ Layer ][ 0 ];
      Button->FOREGROUND = DREAL_RDS_LAYER_NAME_TABLE[ Layer ][ 1 ];
      Button->BACKGROUND = DREAL_RDS_LAYER_NAME_TABLE[ Layer ][ 2 ];

      Button->X        = X1;
      Button->Y        = Y1;
      Button->CALLDATA = (XtPointer)(long)RDS_DYNAMIC_LAYER[ Layer ];

      if ( X1 == 1 )
      {
        X1 = 0; Y1 = Y1 + 1;
      }
      else
      {
        X1 = X1 + 1;
      }

      Index = Index + 1;
    }
  }

  if ( X1 == 1 ) Y1 = Y1 + 1;

  for ( Index = 0; Index < 2; Index++ )
  {
    DrealCreateRectangleButton[ RDS_ALL_LAYER + Index ].Y = Y1;
    Y1 = Y1 + 1;
  }

  DrealCreateRectanglePanel.ROW = Y1;
  DrealCreateRectangleDefaultValues[ 3 ] = Y1 * 25;
}

/*------------------------------------------------------------\
|                                                             |
|                    Callback For Rectangle                   |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                CallbackChangeRectangleLayer                 |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeRectangleLayer( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealRectangleLayer = (int)ClientData;

  DrealChangeEditMode( DREAL_CREATE_RECTANGLE,
                       DrealPromptCreateRectangle );
  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                DrealChangeRectangleLayer                    |
|                                                             |
\------------------------------------------------------------*/

void DrealChangeRectangleLayer( LayerUp )

  int LayerUp;
{
  int  RdsLayer;
  int  Step;
  long RdsWidth;

  if ( LayerUp ) Step =  1;
  else           Step = -1;

  RdsLayer = RDS_STATIC_LAYER[ DrealRectangleLayer ];

  do
  {
    RdsLayer += Step;

    if ( RdsLayer >= RDS_LAYER_ABOX ) RdsLayer = 0;
    else
    if ( RdsLayer <  0              ) RdsLayer = RDS_LAYER_ABOX - 1;
  }
  while ( ( DREAL_RDS_LAYER_NAME_TABLE[ RdsLayer ][ 0 ] == (char *)0 ) ||
          ( RDS_DYNAMIC_LAYER[ RdsLayer ] == RDS_LAYER_UNUSED        ) );

  DrealRectangleLayer = RDS_DYNAMIC_LAYER[ RdsLayer ];
}

/*------------------------------------------------------------\
|                                                             |
|                CallbackChangeRectangleName                  |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeRectangleName( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealEnterDialog( &DrealChangeRectangleNameDialog );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                CallbackChangeRectangleClose                 |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeRectangleClose( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealExitPanel( &DrealCreateRectanglePanel );

  DrealChangeEditMode( DREAL_EDIT_MEASURE,
                       DrealPromptEditMeasure );

  rdsend();
}
