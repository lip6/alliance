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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                   Panel.c                         |
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
# include <string.h>
# include <Xm/Xm.h>
# include <Xm/FileSB.h>
# include <Xm/SelectioB.h>
# include <Xm/PushBG.h>

# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "rfm.h"
# include "GRM.h"
# include "GMX.h"
# include "GTB.h"
# include "GSB.h"
# include "GMT.h"

# include "GMT_panel.h"
# include "GMT_message.h"
# include "GMT_dialog.h"
# include "GMT_tools.h"
# include "GMT_druc.h"

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
|                             Panel                           |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Panel Tools                        |
|                                                             |
\------------------------------------------------------------*/

 GraalPanelButtonItem GraalToolsGlobalButton[] =

         {
           {
             "Equi",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackToolsGlobalEqui,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "UnEqui",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 1,
             1, 1,
             CallbackToolsGlobalUnEqui,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Flat",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 2,
             1, 1,
             CallbackToolsGlobalFlat,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Unflat",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 3,
             1, 1,
             CallbackToolsGlobalUnflat,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,

           {
             "Peek",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 4,
             1, 1,
             CallbackToolsGlobalPeek,
             (XtPointer)NULL,
             (Widget)NULL
           } 
           ,
           {
             "Unpeek",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 5,
             1, 1,
             CallbackToolsGlobalUnpeek,
             (XtPointer)NULL,
             (Widget)NULL
           } 
           ,
           {
             "Druc",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 6,
             1, 1,
             CallbackToolsGlobalDruc,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Real flat",
             NULL, NULL, 0, 0,
             "Red",
             NULL,
             0, 7,
             1, 1,
             CallbackToolsGlobalRealFlat,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Close",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 8,
             1, 1,
             CallbackToolsCloseGlobal,
             (XtPointer)NULL,
             (Widget)NULL
           }
         };

   GraalPanelItem GraalToolsGlobalPanel =

         {
           "Tools",
           1,
           0,
           GRAAL_TOOLS_GLOBAL_X,
           GRAAL_TOOLS_GLOBAL_Y, 
           100,
           225,
           1,  
           9,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           9,
           GraalToolsGlobalButton
         };

   int GraalToolsGlobalDefaultValues[ 5 ] =
 
         {
           GRAAL_TOOLS_GLOBAL_X,
           GRAAL_TOOLS_GLOBAL_Y,
           100, 225, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                      Panel Message                          |
|                                                             |
\------------------------------------------------------------*/

 GraalPanelButtonItem GraalToolsMessageButton[] =

         {
           {
             "Text",
             "Nothing", NULL, 0, 0,
             NULL,
             NULL,
              0, 0,
              8, 9,
             NULL,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Close",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             3, 9,
             2, 1,
             CallbackToolsCloseMessage,
             (XtPointer)NULL,
             (Widget)NULL
           }
         };

   GraalPanelItem GraalToolsMessagePanel =

         {
           "Message",
           1,
           0,
           GRAAL_TOOLS_MESSAGE_X,
           GRAAL_TOOLS_MESSAGE_Y,
           360,
           250,
           8,
           10,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           2,
           GraalToolsMessageButton
         };

   int GraalToolsMessageDefaultValues[ 5 ] =

         {
           GRAAL_TOOLS_MESSAGE_X,
           GRAAL_TOOLS_MESSAGE_Y,
           360, 250, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                        Druc  Panel                          |
|                                                             |
\------------------------------------------------------------*/

   static GraalPanelButtonItem GraalToolsDrucButton[] =

         {
           {
             "Text",
             "Nothing", NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             8, 5,
             NULL,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Continue",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             1, 5,
             2, 1,
             CallbackToolsDrucContinue,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Abort",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             5, 5,
             2, 1,
             CallbackToolsDrucAbort,
             (XtPointer)NULL,
             (Widget)NULL
           }
         };

   GraalPanelItem GraalToolsDrucPanel  =

         {
           "Druc error",
           1,
           0,
           GRAAL_TOOLS_DRUC_X,
           GRAAL_TOOLS_DRUC_Y,
           360,
           150,
           8,
           6,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           3,
           GraalToolsDrucButton
         };

  int GraalToolsDrucDefaultValues[ 5 ] =

         {
           GRAAL_TOOLS_DRUC_X,
           GRAAL_TOOLS_DRUC_Y,
           360, 150, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                    Hierarchy Panel                          |
|                                                             |
\------------------------------------------------------------*/

 GraalPanelButtonItem GraalToolsHierarchyButton[] =

         {
           {
             "Go up",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackToolsHierarchyUp,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Go down",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 1,
             1, 1,
             CallbackToolsHierarchyDown,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Show path",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 2,
             1, 1,
             CallbackToolsHierarchyShow,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Reload",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 3,
             1, 1,
             CallbackToolsHierarchyReload,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Close",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0,  4,
             1,  1,
             CallbackToolsCloseHierarchy,
             (XtPointer)NULL,
             (Widget)NULL
           }
         };

   GraalPanelItem GraalToolsHierarchyPanel =

         {
           "Hierarchy",
           1,
           0,
           GRAAL_TOOLS_HIERARCHY_X,
           GRAAL_TOOLS_HIERARCHY_Y, 
           100,
           100,
           1,  
           5,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           5,
           GraalToolsHierarchyButton
         };

   int GraalToolsHierarchyDefaultValues[ 5 ] =
 
         {
           GRAAL_TOOLS_HIERARCHY_X,
           GRAAL_TOOLS_HIERARCHY_Y,
           100, 125, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                        Show Hierarchy Panel                 |
|                                                             |
\------------------------------------------------------------*/

   GraalPanelButtonItem GraalToolsHierarchyShowButton[] =

         {
           {
             "Text",
             "Nothing", NULL, 0, 0,
             NULL,
             NULL,
              0, 0,
              8, 9,
             NULL,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Close",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             3, 9,
             2, 1,
             CallbackToolsCloseHierarchyShow,
             (XtPointer)NULL,
             (Widget)NULL
           }
         };

   GraalPanelItem GraalToolsHierarchyShowPanel =

         {
           "Show hierarchy",
           1,
           0,
           GRAAL_TOOLS_HIERARCHY_SHOW_X,
           GRAAL_TOOLS_HIERARCHY_SHOW_Y,
           360,
           250,
           8,
           10,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           2,
           GraalToolsHierarchyShowButton
         };

  int GraalToolsHierarchyShowDefaultValues[ 5 ] =

         {
           GRAAL_TOOLS_HIERARCHY_SHOW_X, 
           GRAAL_TOOLS_HIERARCHY_SHOW_Y,
           360, 250, 0
         };


/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Callback For Tools                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    CallbackToolsGlobalEqui                  |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsGlobalEqui( MyWidget, ClientData, CallData )

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

  GraalChangeEditMode( GRAAL_TOOLS_EQUI,
                       GraalPromptToolsEqui );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackToolsGlobalUnEqui                |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsGlobalUnEqui( MyWidget, ClientData, CallData )

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

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackToolsGlobalFlat                  |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsGlobalFlat( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalChangeEditMode( GRAAL_TOOLS_FLATTEN,
                       GraalPromptToolsFlatten );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackToolsGlobalUnflat                |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsGlobalUnflat( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalChangeEditMode( GRAAL_TOOLS_UNFLATTEN,
                       GraalPromptToolsUnflatten );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackToolsGlobalPeek                  |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsGlobalPeek( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalChangeEditMode( GRAAL_TOOLS_PEEK,
                       GraalPromptToolsPeek );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackToolsGlobalUnpeek                |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsGlobalUnpeek( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalToolsUnpeek();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackToolsGlobalDruc                  |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsGlobalDruc( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalChangeEditMode( GRAAL_TOOLS_DRUC,
                       GraalPromptToolsDruc );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 CallbackToolsGlobalRealFlat                 |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsGlobalRealFlat( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalChangeEditMode( GRAAL_TOOLS_REAL_FLATTEN,
                       GraalPromptToolsRealFlatten );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackToolsCloseGlobal                 |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsCloseGlobal( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalExitPanel( &GraalToolsGlobalPanel );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Callback For Hierarchy                  |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 CallbackToolsHierarchyUp                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsHierarchyUp( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalToolsHierarchyUp();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 CallbackToolsHierarchyDown                  |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsHierarchyDown( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalChangeEditMode( GRAAL_TOOLS_HIERARCHY_DOWN,
                       GraalPromptToolsHierarchyDown );
  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 CallbackToolsHierarchyShow                  |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsHierarchyShow( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalChangeEditMode( GRAAL_TOOLS_HIERARCHY_SHOW,
                       GraalPromptToolsHierarchyShow );
  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 CallbackToolsHierarchyReload                |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsHierarchyReload( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalToolsHierarchyReload();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 CallbackToolsCloseHierarchy                 |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsCloseHierarchy( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalToolsDelAllHierarchy();
  GraalExitPanel( &GraalToolsHierarchyPanel );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Callback For Message                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    CallbackToolsCloseMessage                |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsCloseMessage( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalExitPanel( &GraalToolsMessagePanel );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   GraalDisplayToolsMessage                  |
|                                                             |
\------------------------------------------------------------*/

void GraalDisplayToolsMessage()
{
  char *Message;
  rdsbegin();

  Message = GraalGetErrorMessage();

  if ( Message != (char *)NULL )
  {
    XmTextSetString( GraalToolsHierarchyShowButton[0].BUTTON, Message );

    GraalEnterPanel( &GraalToolsMessagePanel );
  }
  else
  {
    XmTextSetString( GraalToolsMessageButton[0].BUTTON, ""      );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        Callback For Druc                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    CallbackToolsDrucContinue                |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsDrucContinue( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalToolsDrucNextError();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     CallbackToolsDrucAbort                  |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsDrucAbort( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalToolsDrucAbortError();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalDisplayDrucMessage                 |
|                                                             |
\------------------------------------------------------------*/

void GraalDisplayDrucMessage( Message )

  char *Message;
{
  rdsbegin();

  XmTextSetString( GraalToolsDrucButton[0].BUTTON, Message );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Callback For Hierarchy Show             |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    CallbackToolsCloseHierarchyShow          |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsCloseHierarchyShow(  MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalExitPanel( &GraalToolsHierarchyShowPanel );

  GraalExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    GraalDisplayHierarchyShowMessage         |
|                                                             |
\------------------------------------------------------------*/

void GraalDisplayHierarchyShowMessage( Message )

  char *Message;
{
  rdsbegin();

  XmTextSetString( GraalToolsHierarchyShowButton[0].BUTTON, Message );

  rdsend();
}
