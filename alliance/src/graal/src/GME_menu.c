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
# include "GSB.h"
# include "GTB.h"
# include "GRM.h"
# include "GMV.h"
# include "GME.h"
# include "GMX.h"

# include "GME_menu.h" 
# include "GME_edit.h" 
# include "GME_modify.h" 
# include "GME_message.h" 

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

   GraalMenuItem GraalEditMenu[] =

         {
           {
             "Undo", 
             'U',   
             "Ctrl<Key>U", 
             "Ctrl U",  
             &xmPushButtonGadgetClass, 
             False, 
             False, 
             False, 
             CallbackEditUndo, 
             (XtPointer)NULL, 
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           } 
           , 
           {
             "Redo",
             'R',
             "Ctrl<Key>R",
             "Ctrl R",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackEditRedo,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Copy",
             'C',
             "Ctrl<Key>C",
             "Ctrl C",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackEditCopy,
             (XtPointer)0,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Move",
             'M',
             "Ctrl<Key>M",
             "Ctrl M",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackEditMove,
             (XtPointer)0,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Delete",
             'D',
             "Ctrl<Key>D",
             "Ctrl D",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackEditDelete,
             (XtPointer)0,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Stretch", 
             'S',  
             "Ctrl<Key>S",
             "Ctrl S", 
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackEditStretch,
             (XtPointer)0,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Modify           ",
             'y',
             "Ctrl<Key>Y",
             "Ctrl Y",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackEditModify,
             (XtPointer)0,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Identify",
             'I',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackEditIdentify,
             (XtPointer)0,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Search           ", 
             'h',   
             "Ctrl<Key>H", 
             "Ctrl H",  
             &xmPushButtonGadgetClass,
             False, 
             False, 
             False, 
             CallbackEditSearch,
             (XtPointer)0,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           } 
           , 
           NULL
         };

   GraalMenuItem GraalWindowMenu[] =

         {
           {
             "Undo", 
             'U',   
             "Meta<Key>U", 
             "Meta U",  
             &xmPushButtonGadgetClass, 
             False, 
             False, 
             False, 
             CallbackEditUndo, 
             (XtPointer)1, 
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           } 
           , 
           {
             "Redo",
             'R',
             "Meta<Key>R",
             "Meta R",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackEditRedo,
             (XtPointer)1,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Copy",
             'C',
             "Meta<Key>C",
             "Meta C",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackEditCopy,
             (XtPointer)1,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Move",
             'M',
             "Meta<Key>M",
             "Meta M",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackEditMove,
             (XtPointer)1,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Delete",
             'D',
             "Meta<Key>D",
             "Meta D",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackEditDelete,
             (XtPointer)1,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Stretch", 
             'S',  
             "Meta<Key>S",
             "Meta S", 
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackEditStretch,
             (XtPointer)1,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Modify           ",
             'y',
             "Meta<Key>Y",
             "Meta Y",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackEditModify,
             (XtPointer)1,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Identify          ",
             'I',
             "Meta<Key>I",
             "Meta I",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackEditIdentify,
             (XtPointer)1,
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
|                     CallbackEditIdentify                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditIdentify( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  if ( ClientData )
  {
    GraalEnterPanel( &GraalWindowGlobalPanel );

    GraalChangeEditMode( GRAAL_SELECT_WINDOW_IDENTIFY,
                         GraalPromptSelectWindowIdentify );
  }
  else
  {
    GraalEnterPanel( &GraalEditGlobalPanel );

    GraalChangeEditMode( GRAAL_SELECT_POINT_IDENTIFY,
                         GraalPromptSelectPointIdentify );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     CallbackEditCopy                        |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditCopy( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  if ( ClientData )
  {
    GraalEnterPanel( &GraalWindowGlobalPanel );

    GraalChangeEditMode( GRAAL_SELECT_WINDOW_COPY,
                         GraalPromptSelectWindowCopy );
  }
  else
  {
    GraalEnterPanel( &GraalEditGlobalPanel );

    GraalChangeEditMode( GRAAL_SELECT_POINT_COPY,
                         GraalPromptSelectPointCopy );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     CallbackEditMove                        |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditMove( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  if ( ClientData )
  {
    GraalEnterPanel( &GraalWindowGlobalPanel );

    GraalChangeEditMode( GRAAL_SELECT_WINDOW_MOVE,
                         GraalPromptSelectWindowMove );
  }
  else
  {
    GraalEnterPanel( &GraalEditGlobalPanel );

    GraalChangeEditMode( GRAAL_SELECT_POINT_MOVE,
                         GraalPromptSelectPointMove );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     CallbackEditDelete                      |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditDelete( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  if ( ClientData )
  {
    GraalEnterPanel( &GraalWindowGlobalPanel );

    GraalChangeEditMode( GRAAL_SELECT_WINDOW_DELETE,
                         GraalPromptSelectWindowDelete );
  }
  else
  {
    GraalEnterPanel( &GraalEditGlobalPanel );

    GraalChangeEditMode( GRAAL_SELECT_POINT_DELETE,
                         GraalPromptSelectPointDelete );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   CallbackEditStretch                       |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditStretch( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  if ( ClientData )
  {
    GraalEnterPanel( &GraalWindowGlobalPanel );

    GraalChangeEditMode( GRAAL_SELECT_WINDOW_STRETCH,
                         GraalPromptSelectWindowStretch );
  }
  else
  {
    GraalEnterPanel( &GraalEditGlobalPanel );

    GraalChangeEditMode( GRAAL_SELECT_POINT_STRETCH,
                         GraalPromptSelectPointStretch );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   CallbackEditModify                        |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditModify( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  if ( ClientData )
  {
    GraalEnterPanel( &GraalWindowGlobalPanel );

    GraalChangeEditMode( GRAAL_SELECT_WINDOW_MODIFY,
                         GraalPromptSelectWindowModify );
  }
  else
  {
    GraalEnterPanel( &GraalEditGlobalPanel );

    GraalChangeEditMode( GRAAL_SELECT_POINT_MODIFY,
                         GraalPromptSelectPointModify );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       CallbackEditSearch                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditSearch( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterPanel( &GraalEditSearchPanel );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       CallbackEditUndo                      |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditUndo( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEditUndo();

  if ( ClientData )
  {
    GraalEnterPanel( &GraalWindowGlobalPanel );
  }
  else
  {
    GraalEnterPanel( &GraalEditGlobalPanel );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       CallbackEditRedo                      |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditRedo( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEditRedo();

  if ( ClientData )
  {
    GraalEnterPanel( &GraalWindowGlobalPanel );
  }
  else
  {
    GraalEnterPanel( &GraalEditGlobalPanel );
  }

  rdsend();
}

