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
# include "GMF.h"

# include "GMF_menu.h"
# include "GMF_dialog.h"
# include "GMF_file.h"

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
|                            Menu                             |
|                                                             |
\------------------------------------------------------------*/

   GraalMenuItem GraalFileMenu[] =

         {
           {
             "New",
             'N',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackFileNew,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Open",
             'O',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackFileOpen,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Save     ",
             'S',
             "Ctrl<Key>W",
             "Ctrl W",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackFileSave,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Save As",
             'A',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackFileSaveAs,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Quit      ",
             'Q',
             "Ctrl<Key>Q",
             "Ctrl Q",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackFileQuit,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             NULL
           }
         };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     CallbackFileNew                         |
|                                                             |
\------------------------------------------------------------*/

void CallbackFileNew( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  if ( GraalHeadUndo != (graalundo *)NULL )
  {
    CallbackFileSaveAs( NULL, NULL, NULL );
  }

  GraalFileNew();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     CallbackFileOpen                        |
|                                                             |
\------------------------------------------------------------*/

void CallbackFileOpen( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  XmString Filter;

  rdsbegin();

  if ( GraalFileFilter[2] == '\0' )
  {
    strcat( GraalFileFilter, IN_PH );
    strcat( GraalFileExtention, IN_PH );

    Filter    = XmStringCreateSimple( GraalFileFilter ); 

    XtVaSetValues( GraalFileOpenDialog.WIDGET, 
                   XmNpattern, Filter, NULL);

    XmStringFree( Filter );
  }

  GraalEnterDialog( &GraalFileOpenDialog );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     CallbackFileSave                        |
|                                                             |
\------------------------------------------------------------*/

void CallbackFileSave( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  if ( GraalFigureMbk != (phfig_list *)NULL )
  {
    if ( ( GraalFigureMbk->NAME == (char *)NULL  ) ||
         ( ! strcmp( GraalFigureMbk->NAME, 
                     GRAAL_DEFAULT_FIGURE_NAME ) ) )

    {
      CallbackFileSaveAs( NULL, NULL, NULL );
    }
    else
    { 
      GraalFileSave();
    }
  }
  else
  {
    GraalErrorMessage( GraalMainWindow, "No current figure !" ); 
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     CallbackFileSaveAs                      |
|                                                             |
\------------------------------------------------------------*/

void CallbackFileSaveAs( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  XmString SaveAsString;

  rdsbegin();

  if ( GraalFigureMbk == (phfig_list *)NULL )
  {
    GraalErrorMessage( GraalMainWindow, "No current figure !" ); 
  }
  else
  {
    SaveAsString = XmStringCreateLtoR( GraalFigureMbk->NAME,
                                       XmSTRING_DEFAULT_CHARSET );

    XtVaSetValues( GraalFileSaveAsDialog.WIDGET,
                   XmNtextString, SaveAsString, NULL );

    XmStringFree( SaveAsString );

    GraalEnterDialog( &GraalFileSaveAsDialog );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     CallbackFileQuit                        |
|                                                             |
\------------------------------------------------------------*/

void CallbackFileQuit( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterDialog( &GraalFileQuitDialog );

  rdsend();
}
