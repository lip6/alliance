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
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                   Menu.c                          |
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

# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "rtl.h"
# include "GSB.h"
# include "GTB.h"
# include "GRM.h"
# include "GMV.h"
# include "GMT.h"
# include "GMX.h"

# include "GMT_menu.h" 
# include "GMT_dialog.h" 
# include "GMT_tools.h" 
# include "GMT_message.h" 

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

   GraalMenuItem GraalToolsMenu[] =

         {
           {
             "Equi",
             'E',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackToolsEqui,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "UnEqui",
             'i',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackToolsUnEqui,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Flat",
             'F',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackToolsFlatInstance,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Unflat",
             't',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackToolsUnflatInstance,
             (XtPointer)NULL,
             (Widget)NULL,  
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Peek",
             'P',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackToolsPeek,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           } 
           ,
           {
             "Unpeek",
             'k',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackToolsUnpeek,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Druc",
             'D',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackToolsDruc,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Real flat",
             'R',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackToolsRealFlatInstance,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Hierarchy",
             'H',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackToolsHierarchy,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Load Error",
             'L',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackToolsLoadError,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Save Image",
             'S',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackToolsSavePixmap,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Message",
             'M',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackToolsMessage,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           NULL
         };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     CallbackToolsEqui                       |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsEqui( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterPanel( &GraalToolsGlobalPanel );

  if ( GraalHeadEqui != (rdsrec_list *)NULL )
  {
    GraalDelEqui();
    GraalZoomRefresh();
  }

  GraalChangeEditMode( GRAAL_TOOLS_EQUI, 
                       GraalPromptToolsEqui );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackToolsUnEqui                      |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsUnEqui( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  if ( GraalHeadEqui != (rdsrec_list *)NULL )
  {
    GraalDelEqui();
    GraalZoomRefresh();
  }

  GraalEnterPanel( &GraalToolsGlobalPanel );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   CallbackToolsFlatInstance                 |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsFlatInstance( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterPanel( &GraalToolsGlobalPanel );

  GraalChangeEditMode( GRAAL_TOOLS_FLATTEN,
                       GraalPromptToolsFlatten );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                CallbackToolsRealFlatInstance                |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsRealFlatInstance( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterPanel( &GraalToolsGlobalPanel );

  GraalChangeEditMode( GRAAL_TOOLS_REAL_FLATTEN,
                       GraalPromptToolsRealFlatten );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 CallbackToolsUnflatInstance                 |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsUnflatInstance( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterPanel( &GraalToolsGlobalPanel );

  GraalChangeEditMode( GRAAL_TOOLS_UNFLATTEN,
                       GraalPromptToolsUnflatten );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackToolsPeek                        |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsPeek( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterPanel( &GraalToolsGlobalPanel );

  GraalChangeEditMode( GRAAL_TOOLS_PEEK,
                       GraalPromptToolsPeek );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackToolsUnpeek                      |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsUnpeek( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterPanel( &GraalToolsGlobalPanel );

  GraalToolsUnpeek();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackToolsDruc                        |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsDruc( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterPanel( &GraalToolsGlobalPanel );

  GraalChangeEditMode( GRAAL_TOOLS_DRUC,
                       GraalPromptToolsDruc );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackToolsMessage                     |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsMessage( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterPanel( &GraalToolsMessagePanel );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   CallbackToolsHierarchy                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsHierarchy( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterPanel( &GraalToolsHierarchyPanel );

  GraalChangeEditMode( GRAAL_TOOLS_HIERARCHY_DOWN,
                       GraalPromptToolsHierarchyDown );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   CallbackToolsSavePixmap                   |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsSavePixmap( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  if ( GraalFigureMbk != (phfig_list *)NULL )
  {
    GraalEnterDialog( &GraalToolsSavePixmapDialog );
  }
  else
  {
    GraalErrorMessage( GraalMainWindow, "No current figure !" ); 
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   CallbackToolsLoadError                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsLoadError( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  XmString Filter;

  rdsbegin();

  if ( GraalToolsFilter[2] == '\0' )
  {
    strcat( GraalToolsFilter, RDS_IN );
    strcat( GraalToolsExtention, RDS_IN );

    Filter = XmStringCreateSimple( GraalToolsFilter ); 

    XtVaSetValues( GraalToolsLoadErrorDialog.WIDGET, 
                   XmNpattern, Filter, NULL);

    XmStringFree( Filter );
  }

  if ( GraalFigureMbk != (phfig_list *)NULL )
  {
    GraalEnterDialog( &GraalToolsLoadErrorDialog );
  }
  else
  {
    GraalErrorMessage( GraalMainWindow, "No current figure !" ); 
  }

  rdsend();
}
